
#include "OBJmodel.h"

OBJmodel::OBJmodel()
{
    vertex_tri.clear();
    normal_tri.clear();
    texcor_tri.clear();
    vertex_index.clear();
    normal_index.clear();
    texcor_index.clear();
    m_MeshTri.clear();
}
OBJmodel::~OBJmodel()
{
    vertex_tri.clear();
    normal_tri.clear();
    texcor_tri.clear();
    vertex_index.clear();
    normal_index.clear();
    texcor_index.clear();
    m_MeshTri.clear();
}
bool OBJmodel::LoadOBJfile(const char *objFileName)
{

    // bool no_tex = true;

    FILE *file = NULL;
    errno_t err;
    pointNum = 0;
    file = fopen(objFileName, "r"); // err = 0 success,err != 0 fail
    if (file == NULL)
    {
        std::cout << "Failed to open objfile" << std::endl;
        return false;
    }

    const int SIZE = 2048;
    char buffer[SIZE];
    char type[80];
    int lineno = 0; // record line number

    // read file
    fgets(buffer, SIZE, file); // # Blender ....
    // std::cout <<buffer<<std::endl;
    fgets(buffer, SIZE, file); // # www.blender.org
    // std::cout <<buffer<<std::endl;
    fgets(buffer, SIZE, file); // mtllib halfCylinder.mtl
    // std::cout <<buffer<<std::endl;
    fgets(buffer, SIZE, file); // o halfCylinder_big
    // std::cout <<buffer<<std::endl;
    sscanf(buffer, "%s", type);

    if (strcmp(type, "o") != 0)
    {
        std::cout << "Failed to read o fileName" << std::endl;
        return false;
    }
    lineno += 4;
    
    while (fgets(buffer, SIZE, file) != NULL)
    {
        // read
        int res = sscanf(buffer, "%s", type);
        // std::cout << type<< " " <<std::endl;

        if (strcmp(type, "v") == 0)
        {
            glm::fvec3 vertex;
            sscanf(buffer, "v %f %f %f", (&vertex.x), (&vertex.y), (&vertex.z));
            // std::cout <<buffer<<std::endl;
            // std::cout << vertex.x << ", " << vertex.y << ", "<< vertex.z<< std::endl;
            vertex_tri.push_back(vertex);

            findExtraPos(vertex);
            pointNum++;
        }
        else if (strcmp(type, "vt") == 0)
        {
            glm::fvec2 texcor;
            sscanf(buffer, "vt %f %f", &texcor.x, &texcor.y);
            // std::cout << texcor.x << ", " << texcor.y << std::endl;
            texcor_tri.push_back(texcor);
        }
        else if (strcmp(type, "vn") == 0)
        {
            glm::fvec3 normal;
            sscanf(buffer, "vn %f %f %f", (&normal.x), (&normal.y), (&normal.z));
            // std::cout << normal.x << ", " << normal.y << ", "<< normal.z<< std::endl;
            normal_tri.push_back(normal);
        }
        else if (strcmp(type, "f") == 0)
        {
            glm::ivec4 ver;
            glm::ivec4 tex;
            glm::ivec4 nor;

            int tmp;
            char p[12];
            char p2[12];
            char p3[12];
            char p4[12];
            sscanf(buffer, "f%s %s %s %s", p, p2, p3, p4);
            // std::cout <<"1  "<<p<<"  1,"<<"1  "<<p2<<"  1,"<<"1  "<<p3<<"  1,"<<"1  "<<p4<<"  1,"<<std::endl;
            sscanf(buffer, "f %d/%s", &tmp, p);
            // std::cout << buffer<<std::endl;

            if (strncmp(p, "/", 1) == 0 && strcmp(p4, "") == 0)
            {
                // f 1//1 2//2 3//3 GL_TRIANGLES
                sscanf(buffer, "f %d//%d %d//%d %d//%d",
                       &ver.x, &nor.x, &ver.y, &nor.y, &ver.z, &nor.z);
                ver.w = nor.w = 0;
            }
            else if (strncmp(p, "/", 1) != 0 && strcmp(p4, "") == 0)
            {
                // f 1/1/1 2/2/2 3/3/3 GL_TRIANGLES
                sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                       &ver.x, &tex.x, &nor.x, &ver.y, &tex.y, &nor.y, &ver.z, &tex.z, &nor.z);
                ver.w = nor.w = tex.w = 0;
            }
            else if (strncmp(p, "/", 1) == 0 && strcmp(p4, "") != 0)
            {
                // f 1//1 2//2 3//3 4//4 GL_LINES
                sscanf(buffer, "f %d//%d %d//%d %d//%d %d//%d",
                       &ver.x, &nor.x, &ver.y, &nor.y, &ver.z, &nor.z, &ver.w, &nor.w);
            }
            else
            {
                // f 1/1/1 2/2/2 3/3/3 4/4/4 GL_LINES
                sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
                       &ver.x, &tex.x, &nor.x, &ver.y, &tex.y, &nor.y, &ver.z, &tex.z, &nor.z, &ver.w, &tex.w, &nor.w);
            }

            // std::cout << ver.x<<"/" << tex.x<<"/" <<nor.x<<"  "<< ver.y<< "/" <<tex.y<< "/" <<nor.y<< "  " <<ver.z<< "/" <<tex.z<< "/" <<nor.z<<std::endl;
            vertex_index.push_back(ver);
            texcor_index.push_back(tex);
            normal_index.push_back(nor);
            triNum++;
        }
    }
    if (feof(file))
    {
        std::cout << "End of file reached!" << std::endl;
    }
    else if (ferror(file))
    {
        std::cout << "Encountered an error while reading the file!" << std::endl;
    }

    fclose(file);
    std::cout << "max : " << m_MaxPos.x << ", " << m_MaxPos.y << ", " << m_MaxPos.z << std::endl;
    std::cout << "min : " << m_MinPos.x << ", " << m_MinPos.y << ", " << m_MinPos.z << std::endl;
    putVertex();
    return true;
}
void OBJmodel::findExtraPos(glm::fvec3 vertex){
    if (pointNum == 0)
    {
        m_MaxPos = m_MinPos = leftPos = rightPos = topPos = bottomPos = vertex;
        extraPoint[0] = extraPoint[1] = extraPoint[2] = extraPoint[3] = 0;
        return;
    }
    // maximum
    if (m_MaxPos.x < vertex.x){
        m_MaxPos.x = vertex.x;
        // right
        rightPos = vertex;
        extraPoint[3] = pointNum;
    }
    if (m_MaxPos.y < vertex.y){
        m_MaxPos.y = vertex.y;
        // top
        topPos = vertex;
        extraPoint[0] = pointNum;
    }
    if (m_MaxPos.z < vertex.z)
        m_MaxPos.z = vertex.z;

    // minmum
    if (m_MinPos.x > vertex.x){
        m_MinPos.x = vertex.x;
        // left
        leftPos = vertex;
        extraPoint[2] = pointNum;
    }
    if (m_MinPos.y > vertex.y){
        m_MinPos.y = vertex.y;
        // bottom
        bottomPos = vertex;
        extraPoint[1] = pointNum;
    }
    if (m_MinPos.z > vertex.z)
        m_MinPos.z = vertex.z;


    
}
void OBJmodel::putVertex(){
    if (vertex_index[0].w == 0)
    {
        //GL_TRIANGLES
        for (int i = 0; i < triNum; i++)
        {
            // if have normal or tex will be change
            m_MeshTri.push_back(
                Vertex{vertex_tri[vertex_index[i].x - 1].x,
                       vertex_tri[vertex_index[i].x - 1].y,
                       vertex_tri[vertex_index[i].x - 1].z});
            m_MeshTri.push_back(
                Vertex{vertex_tri[vertex_index[i].y - 1].x,
                       vertex_tri[vertex_index[i].y - 1].y,
                       vertex_tri[vertex_index[i].y - 1].z});
            m_MeshTri.push_back(
                Vertex{vertex_tri[vertex_index[i].z - 1].x,
                       vertex_tri[vertex_index[i].z - 1].y,
                       vertex_tri[vertex_index[i].z - 1].z});
        }
        Isline = false;
        triNum *= 3;
    }else{
        // GL_LINES
        for (int i = 0; i < triNum; i++)
        {
            // if have normal or tex will be change
            m_MeshTri.push_back(
                Vertex{vertex_tri[vertex_index[i].x - 1].x,
                       vertex_tri[vertex_index[i].x - 1].y,
                       vertex_tri[vertex_index[i].x - 1].z});
            m_MeshTri.push_back(
                Vertex{vertex_tri[vertex_index[i].y - 1].x,
                       vertex_tri[vertex_index[i].y - 1].y,
                       vertex_tri[vertex_index[i].y - 1].z});

            m_MeshTri.push_back(
                Vertex{vertex_tri[vertex_index[i].y - 1].x,
                       vertex_tri[vertex_index[i].y - 1].y,
                       vertex_tri[vertex_index[i].y - 1].z});
            m_MeshTri.push_back(
                Vertex{vertex_tri[vertex_index[i].z - 1].x,
                       vertex_tri[vertex_index[i].z - 1].y,
                       vertex_tri[vertex_index[i].z - 1].z});
            
            m_MeshTri.push_back(
                Vertex{vertex_tri[vertex_index[i].z - 1].x,
                       vertex_tri[vertex_index[i].z - 1].y,
                       vertex_tri[vertex_index[i].z - 1].z});
            m_MeshTri.push_back(
                Vertex{vertex_tri[vertex_index[i].w - 1].x,
                       vertex_tri[vertex_index[i].w - 1].y,
                       vertex_tri[vertex_index[i].w - 1].z});
            
            m_MeshTri.push_back(
                Vertex{vertex_tri[vertex_index[i].w - 1].x,
                       vertex_tri[vertex_index[i].w - 1].y,
                       vertex_tri[vertex_index[i].w - 1].z});
            m_MeshTri.push_back(
                Vertex{vertex_tri[vertex_index[i].x - 1].x,
                       vertex_tri[vertex_index[i].x - 1].y,
                       vertex_tri[vertex_index[i].x - 1].z});
        }
        Isline = true;
        triNum *= 8;
    }
// triNum = 88;
    std::cout << Isline << " triNum : " << triNum << std::endl;
}