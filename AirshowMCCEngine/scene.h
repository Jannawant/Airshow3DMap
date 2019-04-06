#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "packed_freelist.h"

using std::vector;
using std::string;

struct Mesh
{
    string name;
    GLuint meshVAO;
    GLuint positionBO;
    GLuint texCoordBO;
    GLuint normalBO;
    GLuint indexBO;

    GLuint indexCount;
    GLuint vertexCount;
};

struct SceneNode
{
    uint32_t meshId;
    uint32_t transformId;
};

class Scene
{
public:
    Scene() = default;
    packed_freelist<Mesh> meshes ;
    packed_freelist<SceneNode> nodes;

    void init();
};

void loadMeshes(string filename, Scene & scene, vector<GLuint>& meshIDs);
void AddInstance(Scene& scene, uint32_t meshID, uint32_t& newInstanceID);

#endif // SCENE_H
