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

#include <cmath>
#include <unistd.h>

#include <fmt/core.h>
#include <glm/geometric.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/vec3.hpp>

#include "rtx/rtweekend.hpp"
#include "rtx/camera.hpp"
#include "rtx/ray.hpp"

#include "scene/scene_bvh.hpp"
#include "scene/scene_list.hpp"
#include "scene/scene_kd6.hpp"

#include "loader.hpp"
#include "print.hpp"
#include "timer.hpp"


glm::vec3 ray_color(const ray& r, const scene& world, int depth)
{
    hit_record rec;

    if (depth <= 0)
        return glm::vec3(0.f);

    if (world.hit(r, 0.001f, HUGE_VALF, rec))
    {
        ray scattered;
        glm::vec3 attenuation;

        if (rec.mat_ptr && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);

        return glm::vec3(0.f);
    }

    glm::vec3 unit_direction = glm::normalize(r.direction);
    float t = 0.5f * (unit_direction.y + 1.f);
    return glm::lerp(glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.5f, 0.7f, 1.f), t);
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        fmt::print(stderr, "Usage: {} <scene.sce>", argv[0]);
        return 1;
    }
    bool stdout_tty = isatty(STDOUT_FILENO);

    // World
    scene_kd6 world;
    load_scene(argv[1], world);

    Timer timer;
    timer.reset();
    world.freeze();
    fmt::print(stderr, "Freeze: {}s\n", timer.elapsed());

    // Image
    const float aspect_ratio = 16.f / 9.f;
    const int image_width = 800;
    const int image_height = (float)image_width / aspect_ratio;
    const int samples_per_pixel = 50;
    const int max_depth = 50;
//    camera cam = camera::pointing(glm::vec3(-1.f, 0.f, -2.f), glm::vec3(0.f, 0.f, 0.f),
        camera cam = camera::pointing(glm::vec3(0, 0, 1), glm::vec3(0.f, 0.f, -1.f),
                                  2 * glm::atan(1.f), aspect_ratio, 1.0f);

    fmt::print("P3\n{} {}\n255\n", image_width, image_height);

    std::vector<glm::vec3> image(image_width * image_height);

    timer.reset();
    for (int j = image_height - 1; j >= 0; --j)
    {
        for (int i = 0; i < image_width; ++i)
        {
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                float u = (i + random_float()) / (image_width - 1);
                float v = (j + random_float()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                image[i + j * image_width] += ray_color(r, world, max_depth);
            }
        }
    }

    double t = timer.elapsed();
    fmt::print(stderr, "Elapsed time: {}ms\n", 1000.f * t);
    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            fmt::print("{}\n", sampled_color(image[i + j * image_width], samples_per_pixel));
        }
    }

    return EXIT_SUCCESS;
}
