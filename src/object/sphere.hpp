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

#include "hittable.hpp"

class sphere : public hittable
{
public:
    glm::vec3 center;
    float radius;
    std::shared_ptr<material> mat_ptr;

    sphere(const glm::vec3& center, float radius, std::shared_ptr<material> m)
        : center(center), radius(radius), mat_ptr(std::move(m)){};

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
    [[nodiscard]] glm::vec3 centroid() const override;
    [[nodiscard]] AABB bounding_box() const override;

    ~sphere() override = default;
};
