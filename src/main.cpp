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
#include <iostream>
#include <unistd.h>

#include <fmt/core.h>
#include <glm/geometric.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/vec3.hpp>

#include "camera.hpp"
#include "hittable.hpp"
#include "hittable_bvh.hpp"
#include "hittable_list.hpp"
#include "lambertian.hpp"
#include "loader.hpp"
#include "metal.hpp"
#include "print.hpp"
#include "ray.hpp"
#include "rtweekend.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include "triangle.h"

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

    // Image
    const float aspect_ratio = 16.f / 9.f;
    const int image_width = 400;
    const int image_height = (float)image_width / aspect_ratio;
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    auto material_ground = std::make_shared<lambertian>(glm::vec3(0.8f, 0.8f, 0.f));
    //    auto material_center = std::make_shared<lambertian>(glm::vec3(0.7f, 0.3f, 0.3f));
    //    auto material_left = std::make_shared<metal>(glm::vec3(0.8f, 0.8f, 0.8f), 0.3f);
    //    auto material_right = std::make_shared<metal>(glm::vec3(0.8f, 0.6f, 0.2f), 1.f);

    // World
    hittable_bvh world;
    load_scene(argv[1], world);
    world.freeze();

    //    world.add(std::make_unique<sphere>(glm::vec3(0.f, -100.5f, -1.f), 100.f,
    //    material_ground)); world.add(std::make_unique<sphere>(glm::vec3(0.f, 0.f, -1.f), 0.5f,
    //    material_center)); world.add(std::make_unique<sphere>(glm::vec3(-1.f, 0.f, -1.f), 0.5f,
    //    material_left)); world.add(std::make_unique<sphere>(glm::vec3(1.f, 0.f, -1.f), 0.5f,
    //    material_right));

    // Camera
    //    camera cam(viewport_height, viewport_width, focal_length);
    camera cam = camera::pointing(glm::vec3(-1.f, 0.f, -2.f), glm::vec3(0.f, 0.f, 0.f),
                                  2 * glm::atan(1.f), aspect_ratio, 1.0f);

    // Render
    fmt::print("P3\n{} {}\n255\n", image_width, image_height);

    for (int j = image_height - 1; j >= 0; --j)
    {
        if (!stdout_tty)
            fmt::print(stderr, "\rScanlines ramaining: {} ", j);

        for (int i = 0; i < image_width; ++i)
        {
            glm::vec3 pixel_color(0.f);

            for (int s = 0; s < samples_per_pixel; ++s)
            {
                float u = (i + random_float()) / (image_width - 1);
                float v = (j + random_float()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                const auto color = ray_color(r, world, max_depth);
                pixel_color += color;
            }
            fmt::print("{}\n", sampled_color(pixel_color, samples_per_pixel));
        }
    }

    if (!stdout_tty)
        fmt::print(stderr, "\n");

    return EXIT_SUCCESS;
}
