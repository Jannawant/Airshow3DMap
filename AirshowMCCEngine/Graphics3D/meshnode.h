#ifndef MESHNODE_H
#define MESHNODE_H

#include "scenenode.h"
#include <vector>
#include <iostream>
#include "preamble.h"
#include <glm/glm.hpp>
#include "tiny_obj_loader.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "shader.h"
#include <ResourceCache/iresourceloader.h>

using std::vector;
using std::cout;
using std::endl;

std::shared_ptr<IResourceLoader> createMeshObjectResourceLoader();

class Mesh
{
public:
    size_t indexCount;
    size_t vertexCount;
    ushort meshVAO;
    ushort positionBO;
    ushort texCoordBO;
    ushort normalBO;
    ushort tangentBO;
    ushort indexBO;
    void debugPrintMesh()
    {
        cout<<"indexCount = "<<indexCount<< " vertexCount = "<<vertexCount<<endl;
    }
};

class MeshResourceLoader : public IResourceLoader
{
public:
    virtual bool vUseRawFile() override {return false;}
    virtual bool vDiscardRawBufferAfterLoad() override {return false;}
    virtual unsigned int vGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) override;
    virtual bool vLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle) override;
    virtual std::string vGetPattern() override{return "*.obj";}
};

class MeshResourceExtraData : public IResourceExtraData
{
    friend class MeshResourceLoader;
public:
    MeshResourceExtraData(){}
    virtual ~MeshResourceExtraData(){}
    virtual std::string vToString() override {return "MeshResourceExtraData";}
    void parseObjectFile(char * rawBuffer, int size);
    Mesh m_Mesh;
};


class MeshNode : public SceneNode
{
public:
    Shader meshShader;
    std::string m_meshFileName;
    MeshNode(const ActorId actorId, WeakBaseRenderComponentPtr renderComponent, std::string meshFileName, RenderPass renderPass, const Mat4x4 *t )
        : SceneNode(actorId,renderComponent, renderPass, t)
    {
        m_meshFileName = meshFileName;
        meshShader.init();
    }
    virtual ~MeshNode() override {}
    virtual HRESULT vOnRestore(Scene *pScene) override;
    virtual HRESULT vOnLostDevice(UNUSED Scene *pScene) override { return S_OK;}
    virtual HRESULT vRender(Scene *pScene) override;
    //virtual HRESULT vOnUpdate(Scene *pScene, DWORD const elapsedMs) override;
    //virtual HRESULT vPick(Scene *pScene, RayCast *pRayCast);
    Shader & getShader() { return meshShader;}
    void setupShader(const char *vertexShaderName, const char *fragmentShaderName);
    void setupShader(const Shader & shader);
protected:
    float calcBoundingSphere();
};



#endif // MESHNODE_H
