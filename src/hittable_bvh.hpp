

#ifndef CONE_TREE_HITTABLE_BVH_H
#define CONE_TREE_HITTABLE_BVH_H


#include <memory>
#include <vector>
#include <concepts>

#include "hittable.hpp"
#include "bvh.hpp"

class hittable_bvh: public hittable
{
public:
    hittable_bvh() = default;
    virtual ~hittable_bvh() override = default;

    void clear()
    {
        bvh.Reset();
    }

    template<class T, typename... Args>
    requires(std::derived_from<T, hittable>)
    void add(Args&&... args)
    {
        bvh.Add<T>(std::forward<Args>(args)...);
    }

    void construct()
    {
        bvh.Build();
    }

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

    virtual  glm::vec3 centroid() const override;

    virtual  AABB bounding_box() const override;

private:
    BVH bvh;
};

#endif //CONE_TREE_HITTABLE_BVH_H
