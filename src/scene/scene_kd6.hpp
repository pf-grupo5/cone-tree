#pragma once
#include "../kd/kd6.hpp"
#include "scene.hpp"

class scene_kd6 : public scene
{
    KDTree tree;
public:
    bool hit(const ray& ray, float min_time, float max_time, hit_record& hit) const override;
    void add(std::unique_ptr<hittable>&& object) override;
    void clear() override;
    void freeze() override;
};
