

#pragma once

#include <fstream>
#include <sstream>
#include <string_view>
#include <vector>
#include <iostream>

#include "material/lambertian.hpp"
#include "material/material.hpp"
#include "material/metal.hpp"

#include "object/sphere.hpp"
#include "object/triangle.hpp"

#include "scene/scene.hpp"

using MaterialList = std::vector<std::shared_ptr<material>>;
using ObjectList = std::vector<std::unique_ptr<hittable>>;

MaterialList load_materials(std::ifstream& file)
{
    std::vector<std::shared_ptr<material>> materials;

    std::string lines_str;
    int lines;
    std::getline(file, lines_str);
    lines = std::stoi(lines_str);
    materials.reserve(lines);

    for (int i = 0; i < lines; ++i)
    {
        std::string line;
        std::getline(file, line);
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "lambertian")
        {
            float r, g, b;
            iss >> r >> g >> b;
            materials.push_back(std::make_shared<lambertian>(glm::vec3(r, g, b)));
        }
        else if (type == "metal")
        {
            float r, g, b, fuzz;
            iss >> r >> g >> b >> fuzz;
            materials.push_back(std::make_shared<metal>(glm::vec3(r, g, b), fuzz));
        }
        else
        {
            throw std::runtime_error("Unknown material type: \"" + type + "\"");
        }
    }

    return materials;
}

ObjectList load_objects(std::ifstream& file, const MaterialList& materials)
{
    std::vector<std::unique_ptr<hittable>> objects;

    std::string lines_str;
    int lines;
    std::getline(file, lines_str);
    lines = std::stoi(lines_str);
    objects.reserve(lines);

    for (int i = 0; i < lines; ++i)
    {
        std::string line;
        std::getline(file, line);
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "sphere")
        {
            float x, y, z, radius;
            int material;
            iss >> x >> y >> z >> radius >> material;
            objects.push_back(
                std::make_unique<sphere>(glm::vec3(x, y, z), radius, materials[material]));
        }
        else if (type == "tri")
        {
            float x1, y1, z1, x2, y2, z2, x3, y3, z3;
            int material;
            iss >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3 >> material;
            objects.push_back(
                std::make_unique<triangle>(glm::vec3(x1, y1, z1), glm::vec3(x2, y2, z2),
                                           glm::vec3(x3, y3, z3), materials[material]));
        }
        else
        {
            throw std::runtime_error("Unknown object type: " + type);
        }
    }

    return objects;
}

void load_scene(std::string_view filename, scene& scene)
{
    std::ifstream file(filename.data());
    if (!file.is_open())
        throw std::runtime_error("Failed to open file");

    auto materials = load_materials(file);
    std::cerr << "Loaded " << materials.size() << " materials" << std::endl;
    auto objects = load_objects(file, materials);
    std::cerr << "Loaded " << objects.size() << " objects" << std::endl;
    for (auto& object : objects)
        scene.add(std::move(object));
}
