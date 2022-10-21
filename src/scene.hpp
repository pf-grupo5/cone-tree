#pragma once

#include "ray.hpp"
#include "hittable.hpp"

struct scene {
    virtual bool hit(const ray& ray, float min_time, float max_time, hit_record& hit) const = 0;
    virtual void add(std::unique_ptr<hittable>&& object) = 0;
    virtual void clear() = 0;
    virtual void build() = 0;
    virtual ~scene() = default;
};
