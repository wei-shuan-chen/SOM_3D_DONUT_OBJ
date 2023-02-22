#include <cstdlib>
#include <ctime>
#include <iostream>

#include "SOM.h"

glm::fvec3 **lattice;
glm::fvec3 *dataset;

glm::ivec2 latticeEtraPoint[4];

int go = 0;
bool tmp = true;
bool is_som_finished = false;
const int map_width = 61;
const int map_height = 61;
int datasetNum = 0;
const int max_iter = 20000;
int iter = 0;
double learning_rate = 0.1;
double radius = 30;
double n_learning_rate = 0.1;
double neighbor = 30;


glm::fvec3 **createMap(int map_width, int map_height, glm::fvec3 m_MaxPos, glm::fvec3 m_MinPos);
glm::fvec3 *createInputDataset(std::vector<glm::fvec3> vertex_tri, int pointNum);
glm::ivec2 computNeiborhood(int i,int j,glm::ivec2 node,glm::ivec2 bmu);
const glm::fvec3 getInput(glm::fvec3 *dataset, int datasetNum);
void destroy(glm::fvec3 **arr, int width, int height);
void destroyDataset(glm::fvec3 *arr, int datasetNum);
void updateNode(glm::fvec3** lattice, glm::fvec3 nowInput,glm::ivec2 bmuId, glm::ivec2 nodeId, double radius, double learning_rate);
double computerate(int iter, double fun);
double computeradius(int iter, double fun);
double computeScale(double sigma, double dist);
bool isInNeighborhood(double squaredDist, double radius);
void findEtrapoint(double i0, double j0, double k0, int i, int j, glm::fvec3 *leftPos, glm::fvec3 *rightPos, glm::fvec3 *topPos, glm::fvec3 *bottomPos);
void fixExtraPoint(int *extraPoint);

void SOM_Create(std::vector<glm::fvec3> vertex_tri, int pointNum, glm::fvec3 m_MaxPos, glm::fvec3 m_MinPos, int *extraPoint)
{
    srand(time(NULL));

    // 1. Create lattice
    // 1. find lattice exrtapoint
    lattice = createMap(map_width, map_height, m_MaxPos, m_MinPos);
    // 2. Create input dataset
    dataset = createInputDataset(vertex_tri, pointNum);
    // 3. fix etrapoint
    fixExtraPoint(extraPoint);

    datasetNum = pointNum;
    std::cout << pointNum << std::endl;
}

void SOM_IterateOnce()
{
    // 1. Get one input from the dataset
    // 2. Find BMU
    // 3. Update BMU and the neighbors
    n_learning_rate = computerate(iter, learning_rate);
    neighbor = computeradius(iter, radius);

    glm::ivec2 bmu = {0, 0};
    const glm::fvec3 nowInput = getInput(dataset, datasetNum);
    double minDist = -1.0;
    // compute winner point
    
    for (int i = 0; i < map_width; i++)
    {
        for (int j = 0; j < map_height; j++)
        {
            double tmp1 = 0.0;
            float dx = (lattice[i][j].x - nowInput.x);
            float dy = (lattice[i][j].y - nowInput.y);
            float dz = (lattice[i][j].z - nowInput.z);
            tmp1 = dx*dx + dy*dy+ dz*dz;

            if (minDist < 0.0)
            {
                minDist = tmp1;
                continue;
            }
            
            if (minDist > tmp1)
            {
                minDist = tmp1;
                bmu = {i, j};
            }
            
        }
    }
    
    // renew winner point and neighnorhood
    for (int i = 0; i < map_width; i++)
    {
        for (int j = 0; j < map_height; j++)
        {
            glm::ivec2 node = glm::ivec2(i, j);
            glm::ivec2 diff = computNeiborhood(i,j, node, bmu);
            double squaredDist = static_cast<double>(diff.x * diff.x + diff.y * diff.y);
            // std::cout << "Dist " <<squaredDist<<std::endl;
            // std::cout << i<<", " << j << std::endl;
            double no_extra = true;
            for(int k = 0; k < 4; k++){
                if(i == latticeEtraPoint[k].x && j == latticeEtraPoint[k].y){
                    no_extra = false;
                }
            }
            if (isInNeighborhood(squaredDist, neighbor) && no_extra)
            {
                double n_radius = computeScale(neighbor, squaredDist);
                updateNode(lattice, nowInput, bmu, node, n_radius, n_learning_rate);
                
            }
        }
    }
    iter++; 
    is_som_finished = (iter > max_iter);
}

void SOM_Destroy()
{
    // 1. Destroy lattice
    destroy(lattice, map_width, map_height);
    // 2. Destroy input dataset
    destroyDataset(dataset, datasetNum);
}

glm::fvec3 *createInputDataset(std::vector<glm::fvec3> vertex_tri, int pointNum)
{
    glm::fvec3 *dataset = (glm::fvec3 *)malloc(sizeof(glm::fvec3) * pointNum);

   
    for (int i = 0; i < pointNum; i++)
    {
        double i0 = vertex_tri[i].x;
        double j0 = vertex_tri[i].y;
        double k0 = vertex_tri[i].z;
        dataset[i] = {i0, j0, k0};
        // std::cout << i0<<", "<<j0<<", " <<k0<<", "<<std::endl;
    }
    return dataset;
}
glm::fvec3 **createMap(int map_width, int map_height, glm::fvec3 m_MaxPos, glm::fvec3 m_MinPos)
{

    glm::fvec3 **lattice = (glm::fvec3 **)malloc(sizeof(glm::fvec3 *) * map_width);
    for (int i = 0; i < map_width; i++)
        lattice[i] = (glm::fvec3 *)malloc(sizeof(glm::fvec3) * map_height);

    glm::fvec3 leftPos;
    glm::fvec3 rightPos;
    glm::fvec3 topPos;
    glm::fvec3 bottomPos;

    float R = ((m_MaxPos.x - m_MinPos.x) + (m_MaxPos.y - m_MinPos.y)) / 4;
    float r = R / 5.0;
    double rad = 360.0/(double)(map_width-1);
    for (int i = 0; i < map_width; i++)
    {
        for (int j = 0; j < map_height; j++)
        {
            double fi = (double)i*rad *PI/180.0;
            double theta = (double)j*rad *PI/180.0;

            double i0 =  cos(fi)* ( R + r*sin(theta));
            double j0 = sin(fi)* ( R + r*sin(theta));
            double k0 = r * cos(theta)-0.2;
            // 1. find lattice exrtapoint
            findEtrapoint(i0,j0,k0,i,j,&leftPos,&rightPos,&topPos,&bottomPos);

            lattice[i][j] = {i0, j0, k0};
        }
    }
    return lattice;
}
void fixExtraPoint(int *extraPoint){
 
    for(int i = 0; i < 4; i++){
        int i0 = latticeEtraPoint[i].x;
        int j0 = latticeEtraPoint[i].y;
        int i1 = extraPoint[i];
        std::cout << i0<<", "<<j0<<" : {"<< lattice[i0][j0].x <<","<<lattice[i0][j0].y<<","<<lattice[i0][j0].z<<"}"<<std::endl;
        std::cout << i1<<" : {"<< dataset[i1].x <<","<<dataset[i1].y<<","<<dataset[i1].z<<"}\n"<<std::endl;
        
        lattice[i0][j0] = dataset[i1];
    }

}
void destroy(glm::fvec3 **arr, int width, int height){
    for (int i = 0; i < width; i++)
        free(arr[i]);
}
void destroyDataset(glm::fvec3 *arr, int datasetNum){
    free(arr);
}

void findEtrapoint(double i0, double j0, double k0, int i, int j, glm::fvec3 *leftPos, glm::fvec3 *rightPos, glm::fvec3 *topPos, glm::fvec3 *bottomPos){
    if(i == 0 && j == 0){
        *leftPos = *rightPos = *topPos = *bottomPos = {i0,j0,k0};
        latticeEtraPoint[0] = latticeEtraPoint[1] = latticeEtraPoint[2] = latticeEtraPoint[3] = {i,j};
        return;
    }
    if((*topPos).y < j0){
        *topPos = {i0,j0,k0};
        latticeEtraPoint[0] = {i,j};
    }
    if((*bottomPos).y > j0){
        *bottomPos = {i0,j0,k0};
        latticeEtraPoint[1] = {i,j};
    }
    if((*leftPos).x > i0){
        *leftPos = {i0,j0,k0};
        latticeEtraPoint[2] = {i,j};
    }
    if((*rightPos).x < i0){
        *rightPos = {i0,j0,k0};
        latticeEtraPoint[3] = {i,j};
    }

}
glm::ivec2 computNeiborhood(int i, int j, glm::ivec2 node, glm::ivec2 bmu){
    
    glm::ivec2 diff = {0,0};
    int half = (map_width-1)/2;
    if(bmu.x <= half){
        if(node.x >= half+bmu.x){
            diff.x = bmu.x+((map_width-1) - node.x); 
        }else{
            diff.x = node.x - bmu.x;
        }
    }else if(bmu.x > half){
        if(node.x >= bmu.x - half){
            diff.x = bmu.x - node.x;
        }else{
            diff.x = node.x + ((map_width-1) - bmu.x);
        }
    }

    half = (map_height-1)/2;
    if(bmu.y <= half){
        if(node.y >= half+bmu.y){
            diff.y = bmu.y+((map_height-1) - node.y); 
        }else{
            diff.y = node.y - bmu.y;
        }
    }else if(bmu.y > half){
        if(node.y >= bmu.y - half){
            diff.y = bmu.y - node.y;
        }else{
            diff.y = node.y + ((map_height-1) - bmu.y);
        }
    }
    
    
    return diff;
}
double computeradius(int iter, double fun)
{
    double lamda = ((double)(max_iter)) / log(fun);
    double sigma = fun * exp(-1 * ((double)iter) / lamda);
    return sigma;
}
double computerate(int iter, double fun)
{
    double sigma = fun * exp(-1 * ((double)iter) / ((double)(max_iter)) );
    return sigma;
}

const glm::fvec3 getInput(glm::fvec3 *dataset, int datasetNum)
{
    int i = rand() % datasetNum;
// std:: cout <<i << std::endl;
    return dataset[i];
}

bool isInNeighborhood(double squaredDist, double radius)
{
    if (squaredDist <= (radius * radius))
    {
        // std::cout << "dist : " << squaredDist << " radius : " << radius*radius << std::endl;
        return true;
    }
    return false;
}

double computeScale(double sigma, double dist)
{
    double theta = exp((-1 * dist) / (2 * pow(sigma, 2)));
    
    return theta;
}

void updateNode(glm::fvec3** lattice, glm::fvec3 nowInput,glm::ivec2 bmuId, glm::ivec2 nodeId, double radius, double learning_rate){
    lattice[nodeId.x][nodeId.y].x = lattice[nodeId.x][nodeId.y].x + radius * learning_rate*(nowInput.x-lattice[nodeId.x][nodeId.y].x);
    lattice[nodeId.x][nodeId.y].y = lattice[nodeId.x][nodeId.y].y + radius * learning_rate*(nowInput.y-lattice[nodeId.x][nodeId.y].y);
    lattice[nodeId.x][nodeId.y].z = lattice[nodeId.x][nodeId.y].z + radius * learning_rate*(nowInput.z-lattice[nodeId.x][nodeId.y].z);
    
}