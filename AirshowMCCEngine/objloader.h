#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
using std::vector;

struct PackedVertex
{
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
    bool operator<(const PackedVertex that) const
    {
        return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) > 0;
    }
};

void indexVBO(
    std::vector<glm::vec3> & in_vertices,
    std::vector<glm::vec2> & in_uvs,
    std::vector<glm::vec3> & in_normals,

    std::vector<unsigned short> & out_indices,
    std::vector<glm::vec3> & out_vertices,
    std::vector<glm::vec2> & out_uvs,
    std::vector<glm::vec3> & out_normals
);

bool loadObj(const char *path, vector<glm::vec3> &outVertex, vector<glm::vec2> &outUv, vector<glm::vec3> &outNormals);
#endif // OBJLOADER_H
