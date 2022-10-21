#include "camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 calculate_basis(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& up)
{
    return glm::lookAt(pos, dir, up);
}

glm::mat4 calculate_scale(float dist, float v_fov, float ratio)
{
    const auto height = 2 * dist * glm::tan(v_fov / 2.f);
    const auto width = height * ratio;
    return glm::scale(glm::mat4(1.f), glm::vec3{width, height, dist});
}

camera::camera(const glm::vec3& pos_, const glm::vec3& dir_, float v_fov_, float aspect_,
               float dist_)
    : unit_basis(calculate_basis(glm::vec3(0.f), dir_, glm::vec3(0, 1, 0))),
      scale(calculate_scale(dist_, v_fov_, aspect_)), scaled_basis(unit_basis * scale),
      position(glm::vec4(pos_, 1))
{
}

camera camera::pointing(const glm::vec3& st_, const glm::vec3& ed_, float v_fov_, float aspect_,
                        float dist_)
{
    return {st_, glm::normalize(ed_ - st_), v_fov_, aspect_, dist_};
}