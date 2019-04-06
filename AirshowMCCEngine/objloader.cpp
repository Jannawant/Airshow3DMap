#include <cstdlib>
#include "objloader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::istringstream;
using std::map;
//using std::iterator;

bool getSimilarVertexIndex(PackedVertex &packed, std::map<PackedVertex,unsigned short> & VertexToOutIndex, unsigned short & result )
{
    auto it = VertexToOutIndex.find(packed);
    if(it == VertexToOutIndex.end())
    {
        return false;
    }
    result = it->second;
    return true;
}

static int vertAddings  = 0;

void indexVBO(
    std::vector<glm::vec3> & in_vertices,
    std::vector<glm::vec2> & in_uvs,
    std::vector<glm::vec3> & in_normals,

    std::vector<unsigned short> & out_indices,
    std::vector<glm::vec3> & out_vertices,
    std::vector<glm::vec2> & out_uvs,
    std::vector<glm::vec3> & out_normals
)
{
    std::map<PackedVertex,unsigned short> VertexToOutIndex;

    // For each input vertex
    cout << "in index vbo vert size = "<<in_vertices.size() << endl;
    for ( unsigned int i=0; i<in_vertices.size(); i++ ){

        PackedVertex packed = {in_vertices[i], in_uvs[i], in_normals[i]};
        // Try to find a similar vertex in out_XXXX
        unsigned short index;
        bool found = getSimilarVertexIndex( packed, VertexToOutIndex, index);
        if ( found ){ // A similar vertex is already in the VBO, use it instead !
        {
            vertAddings++;
            out_indices.push_back( index );
        }
        }else{ // If not, it needs to be added in the output data.
            out_vertices.push_back( in_vertices[i]);
            out_uvs     .push_back( in_uvs[i]);
            out_normals .push_back( in_normals[i]);
            unsigned short newindex = static_cast<unsigned short>(out_vertices.size()) - 1;
            out_indices .push_back( newindex );
            VertexToOutIndex[ packed ] = newindex;
        }
    }
    cout << "Redundant vertices count = "<< vertAddings<<endl;
}

bool loadObj(const char *path, vector<glm::vec3> &outVertex, vector<glm::vec2> &outUv, vector<glm::vec3> &outNormals)
{
    cout<<"Load Object "<<path<<endl;
    ifstream myfile;
    myfile.open(path);
    if(!myfile.is_open())
    {
        cout<<"Cant open"<<endl;
        return false;
    }
    unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;
    glm::vec3 ver;
    glm::vec2 tex;
    glm::vec3 nrm;
    std::vector<unsigned> vertexIndices, uvIndices, normalIndices;

    std::vector<unsigned short> outIndicies;
    std::map<PackedVertex,unsigned short>  VertexToOutIndex;
    cout<<"begin read file"<<endl;
    string line;
    string v, vt, vn ,f;

    while(getline(myfile,line))
    {
        istringstream ss(line);
        //cout<<line<<endl;
        if(strncmp(line.c_str(), "v ", 2) == 0)
        {
            ss >> v;
            ss >> ver.x >> ver.y >> ver.z;
            temp_vertices.push_back(ver);
        }
        else if(strncmp(line.c_str(), "vt",2) == 0)
        {
            ss >> vt;
            ss >> tex.x >> tex.y;
            tex.y = 1 -tex.y;
            temp_uvs.push_back(tex);
        }
        else if(strncmp(line.c_str(), "vn",2) == 0)
        {
            ss >> vn;
            ss >> nrm.x >> nrm.y >> nrm.z;
            temp_normals.push_back(nrm);
        }
        else if(strncmp(line.c_str(), "f",1) == 0)
        {
            char d;
            ss >> f;
            for(unsigned i = 0 ; i < 3; i++)
                ss >> vertexIndex[i] >> d  >> uvIndex[i]  >>d>> normalIndex[i];
//            cout << " " << vertexIndex[0]<< " "<< uvIndex[0] << " " <<  normalIndex[0] ;
//            cout << " " << vertexIndex[1]<< " "<< uvIndex[1] << " " <<  normalIndex[1] ;
//            cout << " " << vertexIndex[2]<< " "<< uvIndex[2] << " " <<  normalIndex[2] <<endl;
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    }
    cout << "Vertexes point readed " << temp_vertices.size()<<endl;
    cout << "Texcoords point readed " << temp_uvs.size()<<endl;
    cout << "Normals point readed " << temp_normals.size()<<endl;
    for(uint i=0; i<vertexIndices.size(); i++)
    {
        uint vidx = vertexIndices[i];
        uint uidx = uvIndices[i];
        uint nidx = normalIndices[i];
        outVertex.push_back(temp_vertices[vidx-1]);
        outUv.push_back(temp_uvs[uidx-1]);
        outNormals.push_back(temp_normals[nidx-1]);
    }
    cout << "out_vertices size: "<<outVertex.size()<<endl;
    cout << "out_uv size: "<<outUv.size() << endl;
    cout << "out_normals size: "<<outNormals.size()<<endl;
    cout<<"read file successfully"<<endl;
    myfile.close();
    return true;
}

