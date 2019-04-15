#include "texture.h"

bool Texture::loadFromFile(std::string filename, GLenum type)
{
    cout<<"Load from filename"<<endl;
    this->type = type;

    GLuint textureID =  SOIL_load_OGL_texture(filename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    if(!textureID)
    {
        std::cout<<"ERROR::TEXTURE::TEXTURE_LOADING_FAILED"<<filename<<endl;
        return false;
    }
    this->id = textureID;
    glLoadTexture();
}

bool Texture::loadFromBuffer(const char *rawbuffer, int size)
{
    this->type = GL_TEXTURE_2D;
    GLuint textureID =  SOIL_load_OGL_texture_from_memory((const unsigned char*)rawbuffer, size, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    if(!textureID)
    {
        std::cout<<"ERROR::TEXTURE::TEXTURE_LOADING_FROM_BUFFER_FAILED"<<endl;
        return false;
    }
    this->id = textureID;
    glLoadTexture();
}

Texture::~Texture()
{
    unbind();
//    glDeleteTextures(1, &this->id);
}

void Texture::bind(GLint unit)
{
    cout<<"Texture BIND"<<endl;
    glActiveTexture(unit);
    glBindTexture(type, this->id);
    //glEnable(GL_TEXTURE_2D);
}

void Texture::unbind()
{
    glActiveTexture(0);
    glBindTexture(type,0);
}

GLuint Texture::glLoadTexture()
{
    //glGenTextures( 1, texture );
    //unsigned char * image= SOIL_load_image(fileName, &this->width, &this->height, NULL, SOIL_LOAD_RGBA);
    //GLuint textureID = SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    //glGenTextures(1, &this->id);
    //glBindTexture(type,this->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //SOIL_free_image_data(image);
}
