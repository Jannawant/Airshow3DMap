#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "scene.h"
#include "scenenode.h"

using std::cout;
using std::endl;

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    vpath = vertexPath;
    fpath = fragmentPath;
}

void Shader::setupRender(Scene *pScene, SceneNode *pNode)
{
    Mat4x4 matMVP = pScene->getCamera()->getWorldViewProjection(pScene);
    Mat4x4 matModel = pScene->getTopMatrix();
    Mat4x4 matView = pScene->getCamera()->getView();
    cout << endl << "View = "<< matView<<endl;
    cout << "Model = " << matModel<<endl;
    cout << "MVP = " << matMVP <<endl;
    //glUniformMatrix4fv( glGetUniformLocation(shader.m_program,"g_matModelView"), 1, GL_FALSE, glm::value_ptr(mv) );
    //glUniformMatrix4fv( glGetUniformLocation(shader.m_program,"g_matProj"), 1, GL_FALSE, glm::value_ptr(proj) );
    //glUniformMatrix4fv( glGetUniformLocation(m_program, "g_World"), 1, GL_FALSE, glm::value_ptr(matModel.getMat4x4()) );
    glUniformMatrix4fv( glGetUniformLocation(m_program,"g_WVP"), 1, GL_FALSE, glm::value_ptr(matMVP.getMat4x4()));
    //glUniform4fv( glGetUniformLocation(shader.m_program, "g_cameraEye"), 1, glm::value_ptr( eyePosW ) );

    glUniformMatrix4fv(glGetUniformLocation(m_program, "gWorld"),1,GL_FALSE, glm::value_ptr(matModel.getMat4x4()));
    glUniform3fv(glGetUniformLocation(m_program, "gEyeWorldPos"), 1, glm::value_ptr(glm::vec3(0.0,0.0,3.0)));
    glUniform1f(glGetUniformLocation(m_program, "gSpecularPower"), 1.0);
    glUniform1f(glGetUniformLocation(m_program, "gMatSpecularIntensity"), 1.0);
    glUniform3fv(glGetUniformLocation(m_program,"gDirectionalLight.Base.Color"),1, glm::value_ptr(glm::vec3(1.0,1.0,1.0)));
    glUniform1f(glGetUniformLocation(m_program, "gDirectionalLight.Base.AmbientIntensity"), 1.0);
    glUniform3fv(glGetUniformLocation(m_program,"gDirectionalLight.Direction"),1, glm::value_ptr(glm::vec3(0.0,0.0,0.0)));
    glUniform1f(glGetUniformLocation(m_program, "gDirectionalLight.Base.DiffuseIntensity"), 1.0);

    updateUniforms();
    activateTextures();
}

void Shader::Use()
{
    glUseProgram(m_program);
}

void Shader::setPath(const char *vName, const char *fName)
{
    vpath.assign(vName);
    fpath.assign(fName);
}

struct BaseLight
{
    Vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;

    BaseLight()
    {
        Color = Vec3(0.0f, 0.0f, 0.0f);
        AmbientIntensity = 0.0f;
        DiffuseIntensity = 0.0f;
    }
};

struct DirectionalLight : public BaseLight
{
    Vec3 Direction;

    DirectionalLight()
    {
        Direction = Vec3(0.0f, 0.0f, 0.0f);
    }
};

void Shader::init()
{
    cout<<"shader init"<<endl;
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

    glBindAttribLocation(m_program, SCENE_POSITION_ATTRIB_LOCATION, "Position");
    glBindAttribLocation(m_program, SCENE_NORMAL_ATTRIB_LOCATION, "Normal");
    glBindAttribLocation(m_program, SCENE_TEXCOORD_ATTRIB_LOCATION, "TexCoord");
    glBindAttribLocation(m_program, SCENE_TANGENT_ATTRIB_LOCATION, "Tangent");

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

void Shader::activateTextures()
{
    m_diffuseTexture->bind(GL_TEXTURE0);
    m_bumpTexture->bind(GL_TEXTURE1);
    m_normalTexture->bind(GL_TEXTURE2);
}

void Shader::updateUniforms()
{
    glUniform1i(glGetUniformLocation(m_program, "s_texture"), 0);
    glUniform1i(glGetUniformLocation(m_program, "s_bumpMap"),1);
    glUniform1i(glGetUniformLocation(m_program, "s_normalMap"), 2);
}

//get sampler locations
void Shader::setDiffuseTexture(Texture *difTexture)
{
    m_diffuseTexture = difTexture;
}

void Shader::setBumpTexture(Texture *heightTexture)
{
    m_bumpTexture = heightTexture;
}

void Shader::setNormalTexture(Texture *normalTexture)
{
    m_normalTexture = normalTexture;
}














