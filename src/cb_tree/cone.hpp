

#ifndef CONE_TREE_CONE_HPP
#define CONE_TREE_CONE_HPP

#include <glm/glm.hpp>

struct cone {
    glm::vec3 top;
    glm::vec3 base_center;
    float base_radius;

    // TODO: this defines a finite cone. I'm not sure if an infinite cone would be better
};


#endif //CONE_TREE_CONE_HPP
