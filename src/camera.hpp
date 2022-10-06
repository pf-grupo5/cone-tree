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

#pragma once

#include <fmt/format.h>
#include <glm/vec3.hpp>

#include "ray.hpp"

class camera
{
private:
	glm::vec3 origin;
	glm::vec3 horizontal;
	glm::vec3 vertical;
	glm::vec3 lower_left_corner;

public:
	camera(float viewport_height, float viewport_width, float focal_length = 1.f):
		origin(0.f, 0.f, 0.f),
		horizontal(viewport_width, 0.f, 0.f),
		vertical(0.f, viewport_height, 0.f),
		lower_left_corner(
			origin -
			horizontal/2.f -
			vertical/2.f -
			glm::vec3(0.f, 0.f, focal_length)
		)
	{};

	ray get_ray(float u, float v) const
	{
		return {origin, lower_left_corner + u*horizontal + v*vertical - origin};
	}
};

class sampled_color
{
private:
	const glm::vec3& pixel_color;
	int samples_per_pixel;

public:
	sampled_color(const glm::vec3& pixel_color, int samples_per_pixel):
		pixel_color(pixel_color),
		samples_per_pixel(samples_per_pixel)
	{};

	friend struct fmt::formatter<sampled_color>;
};
