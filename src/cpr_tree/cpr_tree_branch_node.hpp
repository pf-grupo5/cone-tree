

#ifndef CONE_TREE_CPR_TREE_BRANCH_NODE_HPP
#define CONE_TREE_CPR_TREE_BRANCH_NODE_HPP

#include "cone.hpp"
#include "cpr_tree_node.hpp"
#include <vector>

class cpr_tree_branch_node : public cpr_tree_node {
private:
    cone enclosed_volume;
    std::vector<cpr_tree_node*> children;
public:
    ~cpr_tree_branch_node() noexcept override {
        for (auto child : children) {
            delete child;
        }
    }
};


#endif //CONE_TREE_CPR_TREE_BRANCH_NODE_HPP
