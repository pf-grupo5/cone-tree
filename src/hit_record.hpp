

#ifndef CONE_TREE_HIT_RECORD_H
#define CONE_TREE_HIT_RECORD_H

#include <glm/glm.hpp>
#include <memory>

class material;

struct hit_record
{
    glm::vec3 p;
    glm::vec3 normal;
    std::shared_ptr<material> mat_ptr;
    float t = HUGE_VALF;
    bool front_face;
};


#endif //CONE_TREE_HIT_RECORD_H
