#include "world.h"


struct World world = {};

void create_world() {
   
    for(int i = 0; i < map_width-1; i++){
        for(int j = 0; j < map_height-1; j++){
            
            world.lattice_square_four_edges.push_back(Vertex{ lattice[i][j].x, lattice[i][j].y, lattice[i][j].z});
            world.lattice_square_four_edges.push_back(Vertex{ lattice[i+1][j].x, lattice[i+1][j].y, lattice[i+1][j].z});

            world.lattice_square_four_edges.push_back(Vertex{ lattice[i+1][j].x, lattice[i+1][j].y, lattice[i+1][j].z});
            world.lattice_square_four_edges.push_back(Vertex{ lattice[i+1][j+1].x, lattice[i+1][j+1].y, lattice[i+1][j+1].z});

            world.lattice_square_four_edges.push_back(Vertex{ lattice[i+1][j+1].x, lattice[i+1][j+1].y, lattice[i+1][j+1].z});
            world.lattice_square_four_edges.push_back(Vertex{ lattice[i][j+1].x, lattice[i][j+1].y, lattice[i][j+1].z});

            world.lattice_square_four_edges.push_back(Vertex{ lattice[i][j+1].x, lattice[i][j+1].y, lattice[i][j+1].z});
            world.lattice_square_four_edges.push_back(Vertex{ lattice[i][j].x, lattice[i][j].y, lattice[i][j].z});

        }
    }

    world.square = {
            Vertex{ { 0.0f, 0.0f, 0.0f}},
            Vertex{ { 1.0f, 0.0f, 0.0f}},
            Vertex{ { 1.0f, 1.0f, 0.0f}},
            Vertex{ { 0.0f, 1.0f, 0.0f}},
    };
    world.line = {
            Vertex{ { 0.0f, 0.0f, 0.0}},
            Vertex{ { 0.1f, 0.0f, 0.0f}},
            Vertex{ { 0.1f, 0.005f, 0.0f}},
            Vertex{ { 0.0f, 0.005f, 0.0f}},
    };
    world.squ_indices = {
        0, 1, 2, // first triangle
        0, 2, 3  // second triangle
    };
    world.tri= {
		Vertex{ { 0.5f, -0.5f, 0.0f}},
        Vertex{ {-0.5f, -0.5f, 0.0f}},
        Vertex{ { 0.0f,  0.5f, 0.0f}}
	};

}
void renew_world(){
    world.lattice_square_four_edges.clear(); 
    for(int i = 0; i < map_width-1; i++){
        for(int j = 0; j < map_height-1; j++){
            
            world.lattice_square_four_edges.push_back(Vertex{ lattice[i][j].x, lattice[i][j].y, lattice[i][j].z});
            world.lattice_square_four_edges.push_back(Vertex{ lattice[i+1][j].x, lattice[i+1][j].y, lattice[i+1][j].z});

            world.lattice_square_four_edges.push_back(Vertex{ lattice[i+1][j].x, lattice[i+1][j].y, lattice[i+1][j].z});
            world.lattice_square_four_edges.push_back(Vertex{ lattice[i+1][j+1].x, lattice[i+1][j+1].y, lattice[i+1][j+1].z});

            world.lattice_square_four_edges.push_back(Vertex{ lattice[i+1][j+1].x, lattice[i+1][j+1].y, lattice[i+1][j+1].z});
            world.lattice_square_four_edges.push_back(Vertex{ lattice[i][j+1].x, lattice[i][j+1].y, lattice[i][j+1].z});

            world.lattice_square_four_edges.push_back(Vertex{ lattice[i][j+1].x, lattice[i][j+1].y, lattice[i][j+1].z});
            world.lattice_square_four_edges.push_back(Vertex{ lattice[i][j].x, lattice[i][j].y, lattice[i][j].z});

        }
    }
    // std::cout << lattice[0][0].x << ", " <<lattice[0][0].y<<std::endl;
}
void destroy_world() {
    world.dataset_square.clear();
    world.lattice_square_four_edges.clear();
    world.dataset_square_four_edges.clear();

    world.square.clear();
    world.line.clear();
    world.squ_indices.clear();
    world.tri.clear();
}