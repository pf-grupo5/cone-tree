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

#include <vector>
#include <glm/vec3.hpp>


struct cone {
    glm::vec3 top;
    glm::vec3 bottom;
    float radius;

    // TODO: 
};


class cpr_tree_node {
public:
    virtual ~cpr_tree_node() noexcept = default;
};


class cpr_tree_active_object : public cpr_tree_node {
    void* data;
    ~cpr_tree_active_object() noexcept = default;
};


class cpr_tree_branch_node : public cpr_tree_node {
private:
    cone cone;
    std::vector<cpr_tree_node*> children;
public:
    ~cpr_tree_branch_node() noexcept {
        for (auto child : children) {
            delete child;
        }
    }
};


class cpr_tree
{
private:
    cpr_tree_node* root;
public:

};
