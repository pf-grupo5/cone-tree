
#pragma once

#include "hittable.hpp"
#include <glm/glm.hpp>

struct triangle : public hittable
{
    glm::vec3 vertex0;
    glm::vec3 vertex1;
    glm::vec3 vertex2;
    std::shared_ptr<material> mat_ptr;

    glm::vec3 m_normal = glm::normalize(glm::cross(vertex2 - vertex0, vertex1 - vertex0));
    glm::vec3 m_centroid = (vertex0 + vertex1 + vertex2) / 3.0f;

    triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
             std::shared_ptr<material> m);

    [[nodiscard]] auto centroid() const noexcept -> glm::vec3 override;
    [[nodiscard]] auto bounding_box() const noexcept -> AABB override;
    bool hit(const ray& ray, float t_min, float t_max, hit_record& hit) const noexcept override;
};