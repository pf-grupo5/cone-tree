

#ifndef CONE_TREE_CPR_TREE_ACTIVE_OBJECT_H
#define CONE_TREE_CPR_TREE_ACTIVE_OBJECT_H

#include "cpr_tree_node.hpp"
#include "../hittable.hpp"

class cpr_tree_active_object : public cpr_tree_node {
    hittable* data;
    ~cpr_tree_active_object() noexcept override = default;
};


#endif //CONE_TREE_CPR_TREE_ACTIVE_OBJECT_H
