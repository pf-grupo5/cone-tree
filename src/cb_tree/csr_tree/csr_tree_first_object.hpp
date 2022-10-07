

#ifndef CONE_TREE_CSR_TREE_FIRST_OBJECT_HPP
#define CONE_TREE_CSR_TREE_FIRST_OBJECT_HPP

#include "../cone.hpp"
#include "csr_tree_second_object.hpp"
#include <vector>

struct csr_tree_first_object {
    cone cone_from_light;
    std::vector<csr_tree_second_object> occluded_objects;
};

#endif //CONE_TREE_CSR_TREE_FIRST_OBJECT_HPP
