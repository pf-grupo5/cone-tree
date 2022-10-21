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

#include <fmt/format.h>
#include <glm/glm.hpp>

#include "ray.hpp"

struct camera
{
    glm::mat4 unit_basis;
    glm::mat4 scale;
    glm::mat4 scaled_basis;
    glm::vec4 position;

    camera() = default;
    camera(const glm::vec3& pos_, const glm::vec3& dir_, float v_fov_, float aspect_, float dist_);
    static camera pointing(const glm::vec3& from, const glm::vec3& to, float v_fov, float aspect,
                           float dist);

    ray get_ray(float u, float v) const
    {
        glm::vec4 vp_point =
            this->position + this->scaled_basis * glm::vec4(u - 0.5, v - 0.5, -1.0, 1.0);
        return ray(this->position, glm::normalize(glm::vec3(vp_point - this->position)));
    }
};

class sampled_color
{
private:
    const glm::vec3& pixel_color;
    int samples_per_pixel;

public:
    sampled_color(const glm::vec3& pixel_color, int samples_per_pixel)
        : pixel_color(pixel_color), samples_per_pixel(samples_per_pixel){};

    friend struct fmt::formatter<sampled_color>;
};
