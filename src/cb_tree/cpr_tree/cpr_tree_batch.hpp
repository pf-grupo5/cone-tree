

#ifndef CONE_TREE_CPR_TREE_BRANCH_NODE_HPP
#define CONE_TREE_CPR_TREE_BRANCH_NODE_HPP

#include "../cone.hpp"
#include <vector>
#include "cpr_tree_active_object.hpp"

struct cpr_tree_branch_node {
    cone enclosed_volume;
    std::vector<cpr_tree_active_object> children;
};


#endif //CONE_TREE_CPR_TREE_BRANCH_NODE_HPP
