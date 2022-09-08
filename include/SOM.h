#ifndef SOM_H
#define SOM_H
#define PI 3.14159265
#include <vector>
#include <glm/glm.hpp>

#include "Vertex.h"

void SOM_Create(std::vector<glm::fvec3> vertex_tri, int pointNum, glm::fvec3 m_MaxPos, glm::fvec3 m_MinPos, int *extraPoint);
void SOM_IterateOnce();
void SOM_Destroy();

extern glm::fvec3** lattice;
extern glm::fvec3* dataset;
extern bool is_som_finished;
extern const int map_width;
extern const int map_height;
extern int go;
extern bool tmp;
extern int iter;
extern double n_learning_rate;
extern double neighbor;

// enum Alphabet : int {
//     A,
//     B,
//     C,
// };

// Alphabet::A;

// class A {
// public:
//     static print();
// };
// class B {
// private:
//     B();
// public:
//     static B* Get();
//     static print();
// };

// B* b1 = B::Get();

// static int count = 0;
// void print() {
//     //static int count = 0;
//     count++;
// }

// void print2() {
//     count = 100;
// }

#endif