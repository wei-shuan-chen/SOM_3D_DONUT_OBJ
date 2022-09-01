#ifndef WORLD_H
#define WORLD_H

#include <array>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdlib> 
#include <ctime> 
#include <glm/glm.hpp>
#include "Vertex.h"
#include "SOM.h"


struct World {
    std::vector<Vertex> dataset_square;
    std::vector<Vertex> lattice_square_four_edges;
    std::vector<Vertex> dataset_square_four_edges;

    std::vector<Vertex> square;
    std::vector<Vertex> line;
    std::vector<unsigned int> squ_indices;
    std::vector<Vertex> tri;
};

void create_world();
void destroy_world();
void renew_world();

extern struct World world;

#endif