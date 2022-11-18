

#include "scene_kd6.hpp"

bool scene_kd6::hit(const ray& ray, float min_time, float max_time, hit_record& hit) const
{
    return tree.hit(ray, min_time, max_time, hit);
}

void scene_kd6::add(std::unique_ptr<hittable>&& object) {
    tree.add(std::move(object));
}

void scene_kd6::clear()
{
    tree.objects.clear();
    tree.root.reset();
}

void scene_kd6::freeze() {
    tree.build();
}
