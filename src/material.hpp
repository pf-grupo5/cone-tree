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

#include <glm/vec3.hpp>

#include "rtweekend.hpp"

struct hit_record;
struct ray;

class material
{
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         glm::vec3& attenutation, ray& scattered) const = 0;
};
