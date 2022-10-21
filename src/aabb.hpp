

#ifndef UNTITLED31_AABB_HPP
#define UNTITLED31_AABB_HPP

#include "hit_record.hpp"
#include "ray.hpp"
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

    [[nodiscard]] float intersection_time(const ray& ray,
                                          const hit_record& hit) const
    {
        auto t_min = (min - ray.origin) / ray.direction;
        auto t_max = (max - ray.origin) / ray.direction;

        auto t_lo = glm::min(t_min, t_max);
        auto t_hi = glm::max(t_min, t_max);

        float t_0 = glm::max(glm::max(t_lo.x, t_lo.y), t_lo.z);
        float t_1 = glm::min(glm::min(t_hi.x, t_hi.y), t_hi.z);

        return t_1 >= t_0 && t_1 >= 0 && t_0 < hit.t ? t_0 : 1e30f;
    }
};

#endif // UNTITLED31_AABB_HPP
