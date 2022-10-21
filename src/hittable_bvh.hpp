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

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

    virtual glm::vec3 centroid() const override;

    virtual AABB bounding_box() const override;

private:
    BVH bvh;
};

#endif // CONE_TREE_HITTABLE_BVH_H
