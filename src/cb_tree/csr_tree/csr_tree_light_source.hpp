

#ifndef CONE_TREE_CSR_TREE_LIGHT_SOURCE_HPP
#define CONE_TREE_CSR_TREE_LIGHT_SOURCE_HPP

#include <vector>
#include "csr_tree_first_object.hpp"

struct csr_tree_light_source {
    std::vector<csr_tree_first_object> all_objects; // this apparently needs to have every single object. bruh.
};

#endif //CONE_TREE_CSR_TREE_LIGHT_SOURCE_HPP
