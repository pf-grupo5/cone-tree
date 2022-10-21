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

#include "../rtx/hit_record.hpp"
#include "../rtx/ray.hpp"
#include <glm/glm.hpp>

struct AABB
{
    glm::vec3 min = glm::vec3{HUGE_VALF};
    glm::vec3 max = glm::vec3{-HUGE_VALF};

    [[nodiscard]] auto area() const noexcept -> float
    {
        auto e = max - min;
        return e.x * e.y + e.y * e.z + e.z * e.x;
    }
    [[nodiscard]] float intersection_time(const ray& ray, float max_time) const
    {
        auto t_min = (min - ray.origin) / ray.direction;
        auto t_max = (max - ray.origin) / ray.direction;

        auto t_lo = glm::min(t_min, t_max);
        auto t_hi = glm::max(t_min, t_max);

        float t_0 = glm::max(glm::max(t_lo.x, t_lo.y), t_lo.z);
        float t_1 = glm::min(glm::min(t_hi.x, t_hi.y), t_hi.z);

        return t_1 >= t_0 && t_1 >= 0 && t_0 < max_time ? t_0 : 1e30f;
    }
};
