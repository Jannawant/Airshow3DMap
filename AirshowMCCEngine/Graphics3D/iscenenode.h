#ifndef ISCENENODE_H
#define ISCENENODE_H

#include <memory>
#include "codestd.h"

using namespace std;
class Scene;
class SceneNodeProperties;



enum RenderPass
{
    RenderPass_0,                       //A constant to define the starting pass
    RenderPass_Static = RenderPass_0,   //environments and level geometry
    RenderPass_Actor,                   //objects and things that can move
    RenderPass_Sky,                     //the background 'behind' everything
    RenderPass_NotRendered,             //objects that dont render but exist
    RenderPass_Last                     //not used - a counter for for loops
};

class SceneNodeProperties
{
    friend class SceneNode;
protected:
    ActorId     m_ActorId;
    std::string m_Name;
    Mat4x4      m_ToWorld, m_FromWorld;
    float       m_Radius;
    RenderPass  m_RenderPass;
    //Material    m_Material;
    //AlphaType   m_AlphaType;

    void setAlpha(const float alpha)
    {
        //m_AlphaType=AlphaMaterial;
        //m_Material.setAlpha(alpha);
    }

public:
    const ActorId &actorId() const { return m_ActorId;}
    Mat4x4 const &toWorld() const{return m_ToWorld;}
    Mat4x4 const &fromWorld() const { return m_FromWorld;}
    void transform(Mat4x4 *toWorld, Mat4x4 *fromWorld) const
    {
        if(toWorld)
            *toWorld=m_ToWorld;
        if(fromWorld)
            *fromWorld=m_FromWorld;
    }
    const char * name() const { return m_Name.c_str();}
    //bool hasAlpha() const {return m_Material.hasAlpha();}
    //virtual float alpha() const { return m_Material.getAlpha();}
    RenderPass renderPass() const{ return m_RenderPass;}
    float radius() const { return m_Radius;} // Or can use AABB axis aligned bounding box
    //Material const &getMaterial() const { return m_Material;}
};

class ISceneNode
{
public:
    virtual const SceneNodeProperties *  vGet() const = 0;
    virtual void vSetTransform(const Mat4x4 *toWorld, const Mat4x4 *fromWorld=nullptr) = 0;
    virtual HRESULT vOnUpdate(Scene *pScene, DWORD const elapsedMs) = 0;
    virtual HRESULT vOnRestore(Scene *pScene)=0;

    virtual HRESULT vPreRender(Scene *pScene) = 0;
    virtual bool vIsVisible(Scene *pScene) const=0;
    virtual HRESULT vRender(Scene *pScene)=0;
    virtual HRESULT vRenderChildren(Scene *pScene)=0;
    virtual HRESULT vPostRender(Scene *pScene)=0;

    virtual bool vAddChild(shared_ptr<ISceneNode> kid)=0;
    virtual bool vRemoveChild(ActorId id)=0;
    virtual HRESULT vOnLostDevice(Scene *pScene)=0;
    virtual ~ISceneNode();
    //ISceneNode();
};

#endif // ISCENENODE_H
