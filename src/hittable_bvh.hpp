

#ifndef CONE_TREE_HITTABLE_BVH_H
#define CONE_TREE_HITTABLE_BVH_H

#include <concepts>
#include <memory>
#include <vector>

#include "bvh.hpp"
#include "hittable.hpp"

class hittable_bvh : public hittable
{
public:
    hittable_bvh() = default;
    virtual ~hittable_bvh() override = default;

    void clear() { bvh.clear(); }

    template <class T, typename... Args>
    requires(std::derived_from<T, hittable>) void add(Args&&... args)
    {
        bvh.add<T>(std::forward<Args>(args)...);
    }

    void construct() { bvh.build(); }

    virtual bool hit(const ray& r, float t_min, float t_max,
                     hit_record& rec) const override;

    virtual glm::vec3 centroid() const override;

    virtual AABB bounding_box() const override;

private:
    BVH bvh;
};

#endif // CONE_TREE_HITTABLE_BVH_H
