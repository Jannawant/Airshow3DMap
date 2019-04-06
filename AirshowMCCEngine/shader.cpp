#include "shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using std::cout;
using std::endl;

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath) :  vpath(vertexPath), fpath(fragmentPath)
{
}

void Shader::Use()
{
    glUseProgram(this->m_program);
}

void Shader::init()
{
    //std::ifstream ifs;
    //ifs.open(shaderFile); //shaderFile - vertexPath or fragmentPath
    //std::string source( std::istreambuf_iterator<char>(ifs), (std::istreambuf_iterator<char>()) );
    std::string  vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);
    try
    {
        vShaderFile.open(vpath);
        fShaderFile.open(fpath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure &e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ " << e.what() << std::endl;
    }
    const char * vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;
    GLint success = 0;
    char infoLog[512];
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex,1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
         std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
         std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    m_program = glCreateProgram();
    glAttachShader(m_program, vertex);
    glAttachShader(m_program, fragment);

    glBindAttribLocation(m_program, SCENE_POSITION_ATTRIB_LOCATION, "g_vPosition");
    glBindAttribLocation(m_program, SCENE_NORMAL_ATTRIB_LOCATION, "g_vNormal");
    glBindAttribLocation(m_program, SCENE_TEXCOORD_ATTRIB_LOCATION, "g_vTexCoord");

    m_stexture = glGetUniformLocation(m_program, "s_texture");
    glUniform1i(m_stexture, 0);

    glLinkProgram(m_program);
    glGetShaderiv(m_program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(m_program, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    std::cout << "CREATE SHADER OBJECT"<<std::endl;
}















