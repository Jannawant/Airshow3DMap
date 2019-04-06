#include "scene.h"
#include "preamble.h"
#include <glm/glm.hpp>
#include "tiny_obj_loader.h"
#include <iostream>

using std::cout;
using std::endl;

void Scene::init()
{

    try {
        meshes = packed_freelist<Mesh>(512);
        nodes = packed_freelist<SceneNode>(100);
    }
    catch (...)
    {
        cout << "cant allocate memory"<<endl;
    }
}

void loadMeshes(std::string filename, Scene &scene, vector<GLuint> &meshIDs)
{
#ifdef TINY_OBJ_LOADER_H_
    // assume mtl is in the same folder as the obj
        std::string mtl_basepath = filename;
        size_t last_slash = mtl_basepath.find_last_of("/");
        if (last_slash == std::string::npos)
            mtl_basepath = "./";
        else
            mtl_basepath = mtl_basepath.substr(0, last_slash + 1);

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    if(!tinyobj::LoadObj(shapes,materials,err,filename.c_str(),mtl_basepath.c_str(),
                         tinyobj::calculate_normals|tinyobj::triangulation))
    {
        cout<<"tinyobject::LoadObj "<<filename<<" error: "<<err<<endl;
        return;
    }
    if(!err.empty())
    {
        cout<<"tinyobject::LoadObj "<<filename<<" warning: "<<err<<endl;
    }

    for(const auto &shape : shapes)
    {
        const tinyobj::mesh_t mesh = shape.mesh;
        Mesh newMesh;

        newMesh.name = shape.name;
        newMesh.indexCount = mesh.indices.size();
        newMesh.vertexCount = mesh.positions.size()/3;
        cout << "index count = " << newMesh.indexCount<<endl;

        GLuint newPositionBO;
        glGenBuffers(1, &newPositionBO);
        glBindBuffer(GL_ARRAY_BUFFER, newPositionBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>((mesh.positions.size()/3) * sizeof(glm::vec3)),
                mesh.positions.data(), GL_STATIC_DRAW);
        newMesh.positionBO = newPositionBO;
        glBindBuffer(GL_ARRAY_BUFFER,0);
        cout << "Pos buf = "<< mesh.positions.size()/3 * sizeof(glm::vec3)<<endl;

        GLuint newTexCoordBO;
        glGenBuffers(1,&newTexCoordBO);
        glBindBuffer(GL_ARRAY_BUFFER, newTexCoordBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>((mesh.texcoords.size()/2) * sizeof(glm::vec2)),
                mesh.texcoords.data(), GL_STATIC_DRAW);
        newMesh.texCoordBO = newTexCoordBO;
        glBindBuffer(GL_ARRAY_BUFFER,0);
        cout<<"Tex buf = "<<mesh.texcoords.size()/2 * sizeof(glm::vec2)<<endl;

        GLuint newNormalsBO;
        glGenBuffers(1,&newNormalsBO);
        glBindBuffer(GL_ARRAY_BUFFER, newNormalsBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(mesh.normals.size() * 3*sizeof(float)),
                mesh.normals.data(), GL_STATIC_DRAW);
        newMesh.normalBO = newNormalsBO;
        glBindBuffer(GL_ARRAY_BUFFER,0);

        GLuint newIndexBo;
        glGenBuffers(1,&newIndexBo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newIndexBo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(mesh.indices.size() * sizeof(unsigned)),
                mesh.indices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        newMesh.indexBO = newIndexBo;
        cout<<"Index buf = "<<mesh.indices.size() * sizeof(unsigned)<<endl;

        GLuint newMeshId = scene.meshes.insert(newMesh);
        meshIDs.push_back(newMeshId);
    }
    /* Add materials */
    //Code....
    //........
    /////

    /*Alternative download object file with objloaderfile */
    {
#elif
#include "objloader.h"
        vector<glm::vec3> Pos;
        vector<glm::vec2> Tex;
        vector<glm::vec3> Norm;
        vector<glm::vec3> OutPos;
        vector<glm::vec2> OutTex;
        vector<glm::vec3> OutNorm;
        vector<unsigned short> OutIndicies;
        loadObj(filename.c_str(),Pos, Tex, Norm);
        indexVBO(Pos,Tex,Norm,OutIndicies, OutPos,OutTex,OutNorm);

        Mesh newMesh;
        newMesh.name = "earth";
        newMesh.indexCount = OutIndicies.size();
        newMesh.vertexCount = Pos.size();

        GLuint newPositionBO;
        glGenBuffers(1, &newPositionBO);
        glBindBuffer(GL_ARRAY_BUFFER, newPositionBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(OutPos.size() * sizeof(glm::vec3)),
                                OutPos.data(), GL_STATIC_DRAW);
        newMesh.positionBO = newPositionBO;
        glBindBuffer(GL_ARRAY_BUFFER,0);
        cout << "Pos buf = "<<OutPos.size() * sizeof(glm::vec3)<<endl;

        GLuint newTexCoordBO;
        glGenBuffers(1,&newTexCoordBO);
        glBindBuffer(GL_ARRAY_BUFFER, newTexCoordBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(OutTex.size() * sizeof(glm::vec2)),
                                OutTex.data(), GL_STATIC_DRAW);
        newMesh.texCoordBO = newTexCoordBO;
        glBindBuffer(GL_ARRAY_BUFFER,0);
        cout << "Tex buf = "<<OutTex.size() * sizeof(glm::vec2)<<endl;

        GLuint newNormalsBO;
        glGenBuffers(1,&newNormalsBO);
        glBindBuffer(GL_ARRAY_BUFFER, newNormalsBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(OutNorm.size() * sizeof(glm::vec3)),
                                OutNorm.data(), GL_STATIC_DRAW);
        newMesh.normalBO = newNormalsBO;
        glBindBuffer(GL_ARRAY_BUFFER,0);

        GLuint newIndexBo;
        glGenBuffers(1,&newIndexBo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newIndexBo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(OutIndicies.size() * sizeof(unsigned)),
                                OutIndicies.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        newMesh.indexBO = newIndexBo;
        cout << "Index buf = "<<OutIndicies.size() * sizeof(unsigned)<<endl;

    //       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh.indexBO);
    //        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

        GLuint newMeshId = scene.meshes.insert(newMesh);
        meshIDs.push_back(newMeshId);

        cout<<OutPos.size()*3<<endl;
        cout << OutTex.size()*2 << endl;
        for(size_t i=0; i<OutPos.size(); i++)
        {
            cout <<OutPos[i].x << " "<< OutPos[i].y <<" "<<OutPos[i].z<< endl;
        }
        cout << endl << endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl;
        for(size_t i=0; i<OutTex.size(); i++)
        {
            cout <<OutTex[i].x << " "<< OutTex[i].y << endl;
        }
        for(size_t i=0; i<OutIndicies.size();i+=3)
        {
            cout<<OutIndicies[i]<<" "<<OutIndicies[i+1]<<" "<<OutIndicies[i+2]<<endl;
        }
    }
#endif
    }
}

void print(vector<float> pos, vector<float> tex, vector<int> indicies)
{
    cout<<pos.size() << endl;
    cout << tex.size() << endl;
    for(size_t i=0; i<pos.size(); i+=3)
    {
        cout << pos[i] << " "<< pos[i+1] << " "<<pos[i+2] << endl;
    }
    cout << endl << endl<< endl << endl<< endl << endl<< endl << endl<< endl << endl<< endl << endl<< endl << endl<< endl << endl;
    for(size_t i=0; i<tex.size(); i+=2)
    {
        cout << tex[i] << " "<< tex[i+1] << endl;
    }
    cout << endl << endl<< endl << endl<< endl << endl<< endl << endl<< endl << endl<< endl << endl<< endl << endl<< endl << endl;
    for(size_t i=0; i<indicies.size(); i+=3)
        cout<<indicies[i]<<" "<<indicies[i+1]<<" "<<indicies[i+2]<<endl;
    cout<<endl;
}


void Draw(SceneNode & node)
{
    Mesh *mesh = 0;
}

void AddInstance(Scene &scene, uint32_t meshID, uint32_t &newInstanceID)
{
//    Transform newTransform;
//    newTransform.Scale = glm::vec3(1.0f);

//    uint32_t newTransformID = scene.Transforms.insert(newTransform);

    SceneNode newInstance;
    newInstance.meshId = meshID;
    //newInstance.transformId = newTransformID;

    uint32_t tmpNewInstanceID = scene.nodes.insert(newInstance);
    if (newInstanceID)
    {
        newInstanceID = tmpNewInstanceID;
    }
}
