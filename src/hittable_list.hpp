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

#include <concepts>
#include <memory>
#include <vector>

#include "hittable.hpp"
#include "scene.hpp"

class hittable_list : public scene
{
public:
    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
    void add(std::unique_ptr<hittable>&& object) override;
    void freeze() override;
    void clear() override;

    ~hittable_list() override = default;

private:
    std::vector<std::unique_ptr<hittable>> objects;
};
