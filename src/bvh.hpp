// Ray tracing with a cone tree
// Copyright © 2022 otreblan
//
// cone-tree is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// cone-tree is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with cone-tree.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "aabb.hpp"
#include "hittable.hpp"
#include <vector>

struct BVHNode
{
    AABB aabb;
    int leftFirst = 0;
    int triCount = 0;

    [[nodiscard]] bool isLeaf() const noexcept { return triCount > 0; }
};

struct BVHBestAxisResult
{
    int axis;
    float pos;
    float cost;
};

struct BVH
{
public:
    std::vector<std::unique_ptr<hittable>> objects;

private:
    std::unique_ptr<BVHNode[]> bvhNode = nullptr;
    std::unique_ptr<glm::vec3[]> centroid = nullptr;
    std::unique_ptr<AABB[]> aabb = nullptr;
    std::unique_ptr<int[]> triIdx = nullptr;
    int nodesUsed = 1;

public:
    void add(std::unique_ptr<hittable>&& object) { objects.push_back(std::move(object)); }
    void build() noexcept
    {
        auto n = (int)objects.size();
        triIdx = std::make_unique<int[]>(n);
        //        triIdx = std::vector<int>(n);
        centroid = std::make_unique<glm::vec3[]>(n);
        //        centroid = std::vector<glm::vec3>(n);
        aabb = std::make_unique<AABB[]>(n);
        //        aabb = std::vector<AABB>(n);
        bvhNode = std::make_unique<BVHNode[]>(n * 2);
        //        bvhNode = std::vector<BVHNode>(n * 2);

        for (int i = 0; i < n; i++)
        {
            triIdx[i] = i;
        }

        for (int i = 0; i < n; i++)
        {
            centroid[i] = objects[i]->centroid();
            aabb[i] = objects[i]->bounding_box();
        }

        BVHNode& root = bvhNode[0];
        root.leftFirst = 0, root.triCount = n;

        update_node_bounds(0);
        subdivide(0);
    }
    void clear() noexcept
    {
        bvhNode.reset();
        centroid.reset();
        aabb.reset();
        triIdx.reset();
        nodesUsed = 1;
    }
    bool hit(const ray& ray, float min_time, float max_time, hit_record& hit) const
    {
        const BVHNode *node = &bvhNode[0], *stack[64];
        int stackPtr = 0;
        bool hitSomething = false;

        while (true)
        {
            if (node->isLeaf())
            {
                for (int i = 0; i < node->triCount; i++)
                {
                    hit_record temp_hit;
                    const auto object_idx = triIdx[node->leftFirst + i];
                    const auto& object = objects[object_idx];
                    if (object && object->hit(ray, min_time, max_time, temp_hit))
                    {
                        hit = temp_hit;
                        hitSomething = true;
                        max_time = temp_hit.t;
                    }
                }
                if (stackPtr == 0)
                    break;
                else
                    node = stack[--stackPtr];
                continue;
            }
            const BVHNode* child1 = &bvhNode[node->leftFirst];
            const BVHNode* child2 = &bvhNode[node->leftFirst + 1];
            float dist1 = child1->aabb.intersection_time(ray, hit);
            float dist2 = child2->aabb.intersection_time(ray, hit);
            if (dist1 > dist2)
            {
                std::swap(dist1, dist2);
                std::swap(child1, child2);
            }
            if (dist1 == 1e30f)
            {
                if (stackPtr == 0)
                    break;
                else
                    node = stack[--stackPtr];
            }
            else
            {
                node = child1;
                if (dist2 != 1e30f)
                    stack[stackPtr++] = child2;
            }
        }
        return hitSomething;
    }

private:
    void update_node_bounds(int nodeIdx) noexcept
    {
        BVHNode& node = bvhNode[nodeIdx];
        node.aabb.min = glm::vec3(1e30f);
        node.aabb.max = glm::vec3(-1e30f);
        for (int first = node.leftFirst, i = 0; i < node.triCount; i++)
        {
            int leafTriIdx = triIdx[first + i];
            const auto& leafTri = objects[leafTriIdx];
            const AABB& leafTriBounds = aabb[leafTriIdx];

            node.aabb.min = glm::min(node.aabb.min, leafTriBounds.min);
            node.aabb.max = glm::max(node.aabb.max, leafTriBounds.max);
        }
    }
    void subdivide(int nodeIdx) noexcept
    {
        // terminate recursion
        BVHNode& node = bvhNode[nodeIdx];
        BVHBestAxisResult best = find_best_axis(node);

        float parentArea = node.aabb.area();
        float parentCost = (float)node.triCount * parentArea;
        if (best.cost >= parentCost)
            return;

        int splitIdx = split(node, best);

        int leftCount = splitIdx - node.leftFirst;
        if (leftCount == 0 || leftCount == node.triCount)
            return;
        // create child nodes
        int leftChildIdx = nodesUsed++;
        int rightChildIdx = nodesUsed++;
        bvhNode[leftChildIdx].leftFirst = node.leftFirst;
        bvhNode[leftChildIdx].triCount = leftCount;
        bvhNode[rightChildIdx].leftFirst = splitIdx;
        bvhNode[rightChildIdx].triCount = node.triCount - leftCount;
        node.leftFirst = leftChildIdx;
        node.triCount = 0;

        update_node_bounds(leftChildIdx);
        update_node_bounds(rightChildIdx);
        // recurse
        subdivide(leftChildIdx);
        subdivide(rightChildIdx);
    }
    int split(const BVHNode& node, const BVHBestAxisResult& best) noexcept
    {
        int i = node.leftFirst;
        int j = i + node.triCount - 1;
        while (i <= j)
        {
            if (centroid[triIdx[i]][best.axis] < best.pos)
            {
                i++;
            }
            else
            {
                std::swap(triIdx[i], triIdx[j--]);
            }
        }
        return i;
    }
    [[nodiscard]] auto find_best_axis(const BVHNode& node) const -> BVHBestAxisResult
    {
        BVHBestAxisResult best = {-1, 0, 1e30f};

        for (int axis = 0; axis < 3; axis++)
        {
            for (int i = 0; i < node.triCount; i++)
            {
                float candidatePos = centroid[triIdx[node.leftFirst + i]][axis];
                float cost = evaluate_sah(node, axis, candidatePos);
                if (cost < best.cost)
                {
                    best.pos = candidatePos, best.axis = axis, best.cost = cost;
                }
            }
        }
        return best;
    }
    [[nodiscard]] float evaluate_sah(const BVHNode& node, int axis, float pos) const noexcept
    {
        // determine triangle counts and bounds for this split candidate
        AABB leftBox, rightBox;
        int leftCount = 0, rightCount = 0;
        for (int i = 0; i < node.triCount; i++)
        {
            auto tri_idx = triIdx[node.leftFirst + i];
            const auto& triangle = objects[tri_idx];
            const AABB& triBounds = aabb[tri_idx];
            if (centroid[tri_idx][axis] < pos)
            {
                leftBox.min = glm::min(leftBox.min, triBounds.min);
                leftBox.max = glm::max(leftBox.max, triBounds.max);
                leftCount++;
            }
            else
            {
                rightBox.min = glm::min(rightBox.min, triBounds.min);
                rightBox.max = glm::max(rightBox.max, triBounds.max);
                rightCount++;
            }
        }
        float left_area = leftBox.area(), right_area = rightBox.area();
        float cost = (float)leftCount * left_area + (float)rightCount * right_area;
        return cost > 0 ? cost : 1e30f;
    }
};
