
#pragma once

#include <glm/glm.hpp>
#include "hittable.hpp"

struct triangle : public hittable
{
    glm::vec3 vertex0;
    glm::vec3 vertex1;
    glm::vec3 vertex2;
    std::shared_ptr<material> mat_ptr;

    glm::vec3 m_normal = glm::normalize(glm::cross(vertex2 - vertex0, vertex1 - vertex0));
    glm::vec3 m_centroid = (vertex0 + vertex1 + vertex2) / 3.0f;

    triangle() = default;
    triangle(const glm::vec3& vertex0, const glm::vec3& vertex1, const glm::vec3& vertex2, std::shared_ptr<material> m)
        : vertex0(vertex0), vertex1(vertex1), vertex2(vertex2), mat_ptr(std::move(m)) {};


    [[nodiscard]] auto centroid() const noexcept -> glm::vec3 override { return m_centroid; }
    [[nodiscard]] auto bounding_box() const noexcept -> AABB override {
        return {
            glm::min(glm::min(vertex0, vertex1), vertex2),
            glm::max(glm::max(vertex0, vertex1), vertex2)
        };
    }
    bool hit(const ray& ray, float t_min, float t_max, hit_record& hit) const noexcept override {
        const auto edge1 = vertex1 - vertex0;
        const auto edge2 = vertex2 - vertex0;
        const auto h = cross(ray.direction, edge2);
        const float a = dot(edge1, h);
        if (a > -0.0001f && a < 0.0001f) return false; // ray parallel to triangle
        const float f = 1 / a;
        const auto s = ray.origin - vertex0;
        const float u = f * dot(s, h);
        if (u < 0 || u > 1) return false;
        const auto q = cross(s, edge1);
        const float v = f * dot(ray.direction, q);
        if (v < 0 || u + v > 1) return false;
        const float t = f * dot(edge2, q);
        if (t > t_min && t < t_max) {
            hit.t = t;
            hit.p = ray.at(t);

            hit.front_face = dot(ray.direction, m_normal) < 0;
            hit.normal = m_normal * (hit.front_face ? 1.0f : -1.0f);
            hit.mat_ptr = mat_ptr;
            return true;
        }
        return false;
    }


};