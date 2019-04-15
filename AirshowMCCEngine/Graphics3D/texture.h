#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <iostream>
#include "GLES2/gl2.h"
#include <SOIL.h>

using std::string;
using std::cout;
using std::endl;

class Texture
{
private:
    GLuint id;
    unsigned int type;
public:
    bool loadFromFile(string filename, GLenum type);
    bool loadFromBuffer(const char *buffer, int size);
    ~Texture();
    inline GLuint getId() const { return id;}
    void bind(GLint unit);
    void unbind();
    GLuint glLoadTexture();
};

#endif // TEXTURE_H
