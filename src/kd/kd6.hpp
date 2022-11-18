
#pragma once

#include "../math/aabb.hpp"
#include "../object/hittable.hpp"
#include <vector>

struct SplitPlane
{
    SplitPlane(const int axis, const float pos) : axis(axis), pos(pos){};
    SplitPlane(){};

    int axis; // 0=x, 1=y, 2=z;
    float pos;

    bool operator==(const SplitPlane& sp) { return (axis == sp.axis && pos == sp.pos); }
};

struct KDTree;

struct KDTreeNode {
    KDTreeNode() = default;
    virtual ~KDTreeNode() = default;

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec, const KDTree& tree) const = 0;
    virtual bool is_leaf() const noexcept = 0;
};

struct KDTreeNodeLeaf : public KDTreeNode {
    AABB aabb;
    SplitPlane splitPlane;
    std::vector<int> objectIds;


    KDTreeNodeLeaf() = default;

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec, const KDTree& tree) const override;
    inline bool is_leaf() const noexcept override { return true; }
};

struct KDTreeNodeInternal : public KDTreeNode {
    AABB aabb;
    SplitPlane splitPlane;
    std::unique_ptr<KDTreeNode> left;
    std::unique_ptr<KDTreeNode> right;

    KDTreeNodeInternal() = default;

    bool hit(const ray& r, float t_min, float t_max, hit_record& hit, const KDTree& tree) const override;
    inline bool is_leaf() const noexcept override { return false; }
};

struct KDTree
{
    std::vector<std::unique_ptr<hittable>> objects;
    std::unique_ptr<KDTreeNode> root;
    std::unique_ptr<AABB[]> aabbs;

public:
    void add(std::unique_ptr<hittable>&& object);
    void build();
    void clear();
    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

};