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

#include "camera.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include "ray.hpp"

#include <glm/vec3.hpp>

class lambertian: public material
{
public:
	glm::vec3 albedo;

	lambertian(glm::vec3 albedo):
		albedo(albedo)
	{};

	virtual bool scatter(
		const ray&,
		const hit_record& rec,
		glm::vec3& attenutation,
		ray& scattered
	) const override
	{
		glm::vec3 scatter_direction = rec.normal + glm::sphericalRand(1.f);

		// Catch degenerate scatter direction
		if(near_zero(scatter_direction))
			scatter_direction = rec.normal;

		scattered = ray(rec.p, scatter_direction);
		attenutation = albedo;

		return true;
	}
};
