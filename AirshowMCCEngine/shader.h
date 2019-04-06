#ifndef SHADER_H
#define SHADER_H

#include "GLES2/gl2.h"
#include "preamble.h"

class Shader
{
public:
    const char * vpath;
    const char * fpath;
    GLuint m_program = 0;
    GLuint m_stexture;
    Shader(const GLchar* vertexPath, const GLchar * fragmentPath);
    void Use();
    void init();
private:

};

#endif // SHADER_H
