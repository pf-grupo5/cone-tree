#pragma once

#include "../object/hittable.hpp"
#include "../rtx/ray.hpp"

struct scene
{
    virtual bool hit(const ray& ray, float min_time, float max_time, hit_record& hit) const = 0;
    virtual void add(std::unique_ptr<hittable>&& object) = 0;
    virtual void clear() = 0;
    virtual void freeze() = 0;
    virtual ~scene() = default;
};
