#ifndef SHADER_H
#define SHADER_H

#include "GLES2/gl2.h"
#include "preamble.h"
#include "glm/glm.hpp"
#include "geometry.h"
#include <string>
#include "texture.h"

using std::string;

class Scene;
class SceneNode;
class Texture;

class Shader
{
public:
    Shader(const char* vertexPath = "shader.vert", const char * fragmentPath = "shader.frag");
    string vpath ;
    string fpath ;
    GLuint m_program = 0;
    void setupRender(Scene *pScene, SceneNode *pNode);
    void Use();
    void init();
    void setPath(const char *vName, const char *fName);
    void setDiffuseTexture(Texture *difTexture);
    void setBumpTexture(Texture *heightTexture);
    void setNormalTexture(Texture *normalTexture);
    void activateTextures();
    void updateUniforms();
private:
    Mat4x4 m_matrixTransform;
    Texture *m_diffuseTexture;
    Texture *m_bumpTexture;
    Texture *m_normalTexture;
};

#endif // SHADER_H
