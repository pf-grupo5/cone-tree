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

#include "../bvh/aabb.hpp"
#include "../rtx/hit_record.hpp"
#include "../rtx/ray.hpp"
#include <glm/geometric.hpp>
#include <memory>

class hittable
{
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
    virtual glm::vec3 centroid() const = 0;
    virtual AABB bounding_box() const = 0;
    virtual ~hittable() = default;
};

template <class T>
concept Hittable = std::derived_from<T, hittable>;
