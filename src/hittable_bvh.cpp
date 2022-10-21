#include "hittable_bvh.hpp"

glm::vec3 hittable_bvh::centroid() const
{
    glm::vec3 result(0.f);
    for (const auto& object : bvh.objects)
    {
        if (object)
        {
            result += object->centroid();
        }
    }
    return result / (float)bvh.objects.size();
}

AABB hittable_bvh::bounding_box() const
{
    AABB output{glm::vec3(HUGE_VALF), -glm::vec3(HUGE_VALF)};
    for (const auto& object : bvh.objects)
    {
        output.min = glm::min(output.min, object->bounding_box().min);
        output.max = glm::max(output.max, object->bounding_box().max);
    }
    return output;
}

bool hittable_bvh::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
    return bvh.hit(r, t_min, t_max, rec);
}
