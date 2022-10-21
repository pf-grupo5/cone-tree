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
#include <glm/vec3.hpp>
#include <glm/gtc/random.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/compatibility.hpp>

#include "camera.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "hittable_bvh.hpp"
#include "lambertian.hpp"
#include "material.hpp"
#include "metal.hpp"
#include "print.hpp"
#include "ray.hpp"
#include "rtweekend.hpp"
#include "sphere.hpp"

glm::vec3 ray_color(const ray& r, const hittable& world, int depth)
{
	hit_record rec;

	if(depth <= 0)
		return glm::vec3(0.f);

	if(world.hit(r, 0.001f, HUGE_VALF, rec))
	{
		ray scattered;
		glm::vec3 attenuation;

		if(rec.mat_ptr && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth-1);

		return glm::vec3(0.f);
	}

	// Background
	glm::vec3 unit_direction = glm::normalize(r.direction);
	float t = 0.5f*(unit_direction.y + 1.f);
	return glm::lerp(glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.5f, 0.7f, 1.f), t);
}

int main()
{
	bool stdout_tty = isatty(STDOUT_FILENO);

	// Image
	const float aspect_ratio = 16.f/9.f;
	const int image_width = 400;
	const int image_height = (float) image_width / aspect_ratio;
	const int samples_per_pixel = 100;
	const int max_depth = 50;

    auto material_ground = std::make_shared<lambertian>(glm::vec3(0.8f, 0.8f, 0.f));
    auto material_center = std::make_shared<lambertian>(glm::vec3(0.7f, 0.3f, 0.3f));
    auto material_left = std::make_shared<metal>(glm::vec3(0.8f, 0.8f, 0.8f), 0.3f);
    auto material_right = std::make_shared<metal>(glm::vec3(0.8f, 0.6f, 0.2f), 1.f);

	// World
	hittable_bvh world;
	world.add<sphere>(glm::vec3(0.f,  -100.5f, -1.f), 100.f, material_ground);
	world.add<sphere>(glm::vec3(0.f,  0.f,     -1.f), 0.5f, material_center);
	world.add<sphere>(glm::vec3(-1.f, 0.f,     -1.f), 0.5f, material_left);
	world.add<sphere>(glm::vec3(1.f,  0.f,     -1.f), 0.5f, material_right);
    world.construct();

	// Camera
	float viewport_height = 2.f;
	float viewport_width = aspect_ratio*viewport_height;
	float focal_length = 1.f;
	camera cam(viewport_height, viewport_width, focal_length);

	// Render
	fmt::print("P3\n{} {}\n255\n", image_width, image_height);


	for (int j = image_height-1; j >= 0; --j)
	{
		if(!stdout_tty)
			fmt::print(stderr, "\rScanlines ramaining: {} ", j);

		for (int i = 0; i < image_width; ++i)
		{
			glm::vec3 pixel_color(0.f);

			for(int s = 0; s < samples_per_pixel; ++s)
			{
				float u = (i+random_float())/(image_width-1);
				float v = (j+random_float())/(image_height-1);
				ray r = cam.get_ray(u, v);
                const auto color = ray_color(r, world, max_depth);
				pixel_color += color;
			}
			fmt::print("{}\n", sampled_color(pixel_color, samples_per_pixel));
		}
	}

	if(!stdout_tty)
		fmt::print(stderr, "\n");

	return EXIT_SUCCESS;
}
