#include "kd6.hpp"
#include <algorithm>
#include <numeric>

struct AABBSplit
{
    AABB left;
    AABB right;
};

enum class PlaneSide
{
    LEFT = -1,
    RIGHT = 1,
};

struct SAHResult
{
    float cost;
    PlaneSide side;
};

struct SplitResult
{
    float cost;
    SplitPlane plane;
    PlaneSide side;
};

struct ObjectSplit
{
    std::vector<int> left;
    std::vector<int> right;
};

struct Event
{
    typedef enum
    {
        endingOnPlane = 0,
        lyingOnPlane = 1,
        startingOnPlane = 2
    } EventType;
    int objectId;
    SplitPlane splitPlane;
    EventType type;

    Event(int objectId, int k, float ee0, EventType type)
        : objectId(objectId), type(type), splitPlane(SplitPlane(k, ee0))
    {
    }

    inline bool operator<(const Event& e) const
    {
        return ((splitPlane.pos < e.splitPlane.pos) ||
                (splitPlane.pos == e.splitPlane.pos && type < e.type));
    }
};

constexpr auto COST_TRAVERSE = 1.0f;
constexpr auto COST_INTERSECT = 1.5f;

static AABBSplit splitAABB(const AABB& aabb, const SplitPlane& plane);

static float surfaceArea(const AABB& V);

static float hitProbability(const AABB& Vsub, const AABB& V);

static float lambda(int NL, int NR, float PL, float PR);

static float cost(float PL, float PR, int NL, int NR);

static bool stopSplitting(int N, float minCv);

static std::unique_ptr<KDTreeNode> build(const std::vector<int>& objectIds, const AABB& aabb,
                                         int depth);

// TODO: Use SAH
static SAHResult SAH(const SplitPlane& p, const AABB& V, int NL, int NR, int NP);

static AABB clipTriangleToBox(int objectId, const AABB& V, const KDTree& tree)
{
    AABB b = tree.aabbs[objectId];
    for (int k = 0; k < 3; k++)
    {
        if (V.min[k] > b.min[k])
            b.min[k] = V.min[k];
        if (V.max[k] < b.max[k])
            b.max[k] = V.max[k];
    }
    return b;
}

static SplitResult findPlane(const std::vector<int>& T, const AABB& V, int depth,
                             const KDTree& tree)
{
    // static int count = 0;
    SplitResult bestSplit = {INFINITY};

    for (int k = 0; k < 3; ++k)
    {
        std::vector<Event> events;
        events.reserve(T.size() * 2);
        for (auto pit = T.begin(); pit != T.end(); pit++)
        {
            auto t = *pit;
            auto B = clipTriangleToBox(t, V, tree);
            if (B.min.x - B.max.x <= 0.01f || B.min.y - B.max.y <= 0.01f ||
                B.min.z - B.max.z <= 0.01f)
            {
                events.emplace_back(t, k, B.min[k], Event::lyingOnPlane);
            }
            else
            {
                events.emplace_back(t, k, B.min[k], Event::startingOnPlane);
                events.emplace_back(t, k, B.max[k], Event::endingOnPlane);
            }
        }
        std::sort(events.begin(), events.end());
        int NL = 0, NP = 0, NR = T.size();
        for (std::vector<Event>::size_type Ei = 0; Ei < events.size(); ++Ei)
        {
            const SplitPlane& p = events[Ei].splitPlane;
            int pLyingOnPlane = 0, pStartingOnPlane = 0, pEndingOnPlane = 0;
            while (Ei < events.size() && events[Ei].splitPlane.pos == p.pos &&
                   events[Ei].type == Event::endingOnPlane)
            {
                ++pEndingOnPlane;
                Ei++;
            }
            while (Ei < events.size() && events[Ei].splitPlane.pos == p.pos &&
                   events[Ei].type == Event::lyingOnPlane)
            {
                ++pLyingOnPlane;
                Ei++;
            }
            while (Ei < events.size() && events[Ei].splitPlane.pos == p.pos &&
                   events[Ei].type == Event::startingOnPlane)
            {
                ++pStartingOnPlane;
                Ei++;
            }
            NP = pLyingOnPlane;
            NR -= pLyingOnPlane;
            NR -= pEndingOnPlane;

            const auto [C, pside] = SAH(p, V, NL, NR, NP);
            if (C < bestSplit.cost)
            {
                bestSplit.cost = C;
                bestSplit.side = pside;
                bestSplit.plane = p;
            }
            NL += pStartingOnPlane;
            NL += pLyingOnPlane;
            NP = 0;
        }
    }
}

static ObjectSplit sortTriangles(const std::vector<int>& T, const SplitPlane& p,
                                 const PlaneSide& pside, const KDTree& tree)
{
    ObjectSplit split;
    for (int id : T)
    {
        AABB tbox = tree.aabbs[id];
        if (tbox.min[p.axis] == p.pos && tbox.max[p.axis] == p.pos)
        {
            if (pside == PlaneSide::LEFT)
            {
                split.left.push_back(id);
            }
            else
            {
                split.right.push_back(id);
            }
        }
        else
        {
            if (tbox.min[p.axis] < p.pos)
                split.left.push_back(id);
            if (tbox.max[p.axis] > p.pos)
                split.right.push_back(id);
        }
    }
    return split;
}

static std::unique_ptr<KDTreeNode> buildRec(const std::vector<int>& objectIds, const AABB& aabb,
                                         int depth, const KDTree& tree)
{
    auto plane = findPlane(objectIds, aabb, depth, tree);
    if (stopSplitting(objectIds.size(), plane.cost))
    {
        auto node = std::make_unique<KDTreeNodeLeaf>();
        node->objectIds = objectIds;
        node->splitPlane = plane.plane;
        node->aabb = aabb;
        return node;
    }

    const auto aabbsplit = splitAABB(aabb, plane.plane);
    const auto split = sortTriangles(objectIds, plane.plane, plane.side, tree);
    auto node = std::make_unique<KDTreeNodeInternal>();
    node->splitPlane = plane.plane;
    node->aabb = aabb;
    node->left = buildRec(split.left, aabbsplit.left, depth + 1, tree);
    node->right = buildRec(split.right, aabbsplit.right, depth + 1, tree);
    return node;
}

bool KDTreeNodeLeaf::hit(const ray& r, float t_min, float t_max, hit_record& rec,
                         const KDTree& tree) const
{
    auto bound_times = aabb.intersection_time(r, 0, t_min);
    t_min = glm::max(t_min, bound_times.first);
    t_max = glm::min(t_max, bound_times.second);

    bool hit = false;
    float closest_so_far = t_max;
    for (int object_idx : objectIds)
    {
        hit_record temp_rec;
        const auto& object = tree.objects[object_idx];
        if (object->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit;
}

bool KDTreeNodeInternal::hit(const ray& ray, float t_min, float t_max, hit_record& hit,
                             const KDTree& tree) const
{
    auto bound_times = aabb.intersection_time(ray, 0, t_max);
    t_min = glm::max(t_min, bound_times.first);
    t_max = glm::min(t_max, bound_times.second);

    float t_split =
        (splitPlane.pos - ray.origin[splitPlane.axis]) *
        (ray.direction[splitPlane.axis] == 0 ? 1e30f : 1 / ray.direction[splitPlane.axis]);

    auto near = ray.origin[splitPlane.axis] < splitPlane.pos ? left.get() : right.get();
    auto far = ray.origin[splitPlane.axis] < splitPlane.pos ? right.get() : left.get();

    if (t_split > t_max || t_split < 0)
    {
        return near->hit(ray, t_min, t_max, hit, tree);
    }
    else if (t_split < t_min)
    {
        return far->hit(ray, t_min, t_max, hit, tree);
    }
    else
    {
        bool hit_anything = near->hit(ray, t_min, t_split, hit, tree);
        if (hit.t < t_split)
            return hit_anything;

        hit_anything |= far->hit(ray, t_split, t_max, hit, tree);
        return hit_anything;
    }
}

bool KDTree::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
    return root->hit(r, t_min, t_max, rec, *this);
}

void KDTree::clear()
{
    objects.clear();
    root.reset();
}

void KDTree::add(std::unique_ptr<hittable>&& object) { objects.push_back(std::move(object)); }

void KDTree::build()
{
    const auto n = objects.size();
    aabbs = std::make_unique<AABB[]>(n);
    for (int i = 0; i < n; ++i)
    {
        aabbs[i] = objects[i]->bounding_box();
    }
    auto node = std::make_unique<KDTreeNodeLeaf>();
    node->objectIds.resize(n);
    std::iota(node->objectIds.begin(), node->objectIds.end(), 0);

    AABB aabb;
    for (int i = 0; i < n; ++i)
    {
        aabb.min = glm::min(aabb.min, aabbs[i].min);
        aabb.max = glm::max(aabb.max, aabbs[i].max);
    }
    root = buildRec(node->objectIds, aabb, 0, *this);
}

AABBSplit splitAABB(const AABB& aabb, const SplitPlane& plane)
{
    AABBSplit split;
    split.left = aabb;
    split.right = aabb;
    split.left.max[plane.axis] = plane.pos;
    split.right.min[plane.axis] = plane.pos;
    return split;
}

float surfaceArea(const AABB& V) { return 2 * V.area(); }

float hitProbability(const AABB& Vsub, const AABB& V) { return surfaceArea(Vsub) / surfaceArea(V); }

float lambda(int NL, int NR, float PL, float PR)
{
    if ((NL == 0 || NR == 0) && !(PL == 1 || PR == 1))
        return 0.8f;
    return 1.0f;
}

float cost(float PL, float PR, int NL, int NR)
{
    return (lambda(NL, NR, PL, PR) * (COST_TRAVERSE + COST_INTERSECT * (PL * NL + PR * NR)));
}

bool stopSplitting(int N, float minCv) { return (minCv > COST_INTERSECT * (float)N); }

SAHResult SAH(const SplitPlane& p, const AABB& V, int NL, int NR, int NP)
{
    auto [left, right] = splitAABB(V, p);
    float PL = hitProbability(left, V);
    float PR = hitProbability(right, V);
    if (PL == 0 || PR == 0)
        return {INFINITY};
    if (V.min[p.axis] - V.max[p.axis] == 0)
        return {INFINITY};

    float CPL = cost(PL, PR, NL + NP, NR);
    float CPR = cost(PL, PR, NL, NP + NR);
    if (CPL < CPR)
        return {CPL, PlaneSide::LEFT};
    else
        return {CPR, PlaneSide::RIGHT};
}
std::unique_ptr<KDTreeNode> build(const std::vector<int>& objectIds, const AABB& aabb, int depth) {}
