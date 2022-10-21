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

#include "hittable_list.hpp"

bool hittable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
    bool hit_anything = false;
    float closest_so_far = t_max;

    for (const auto& object : objects)
    {
        hit_record temp_rec;
        if (object && object->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

void hittable_list::add(std::unique_ptr<hittable>&& object)
{
    objects.push_back(std::move(object));
}

void hittable_list::build() {}
void hittable_list::clear() { objects.clear(); }

// glm::vec3 hittable_list::centroid() const
//{
//     glm::vec3 result(0.f);
//     for (const auto& object : objects)
//     {
//         if (object)
//         {
//             result += object->centroid();
//         }
//     }
//     return result / (float)objects.size();
// }

// AABB hittable_list::bounding_box() const
//{
//     AABB output{glm::vec3(HUGE_VALF), -glm::vec3(HUGE_VALF)};
//     for (const auto& object : objects)
//     {
//         output.min = glm::min(output.min, object->bounding_box().min);
//         output.max = glm::max(output.max, object->bounding_box().max);
//     }
//     return output;
// }
