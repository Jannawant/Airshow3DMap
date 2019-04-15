#ifndef SCENENODE_H
#define SCENENODE_H

#include "iscenenode.h"
#include <vector>
#include <memory>
#include <iostream>
using std::cout;
using std::endl;
using std::vector;
using std::shared_ptr;

class BaseRenderComponent;

typedef vector<shared_ptr<ISceneNode>> SceneNodeList;
typedef BaseRenderComponent* WeakBaseRenderComponentPtr;

class SceneNode : public ISceneNode
{
    friend class Scene;
protected:
    SceneNodeList   m_Children;
    SceneNode       *m_Parent;
    SceneNodeProperties m_Props;
    WeakBaseRenderComponentPtr m_RenderComponent;
public:
    SceneNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass,
              const Mat4x4 *to, const Mat4x4 *from=nullptr);
    virtual ~SceneNode() override{}
    virtual const SceneNodeProperties *  vGet()  const override { return &m_Props;}
    virtual void vSetTransform(const Mat4x4 *toWorld, const Mat4x4 *fromWorld=nullptr) override;
    virtual HRESULT vOnRestore(Scene *pScene) override;
    virtual HRESULT vOnUpdate(Scene *pScene, DWORD const elapsedMs) override;
    virtual HRESULT vPreRender(Scene *pScene) override;
    virtual bool vIsVisible(Scene *pScene) const override;
    virtual HRESULT vRender(UNUSED Scene *pScene) override{return S_OK;}
    virtual HRESULT vRenderChildren(Scene *pScene) override;
    virtual HRESULT vPostRender(Scene *pScene) override;

    virtual bool vAddChild(shared_ptr<ISceneNode> kid) override;
    virtual bool vRemoveChild(ActorId id) override;
    virtual HRESULT vOnLostDevice(Scene  *pScene) override;
    void setAlpha(float alpha) {m_Props.setAlpha(alpha);}
    //float getAlpha() const { return m_Props.alpha();}
    Vec3 getPosition() const { return m_Props.m_ToWorld.getPosition();}
    void setPosition(const Vec3 &pos) { return m_Props.m_ToWorld.setPosition(pos);}
    Vec3 getDirection(const Vec3 &pos) const {return m_Props.m_ToWorld.getDirection(pos);}
    void setRadius(const float radius) { m_Props.m_Radius = radius;}
    //void setMaterial(const Material &mat) { m_Props.m_Material = mat;}
    const Vec3 getWorldPosition() const;
};

class RootNode: public SceneNode
{
public:
    RootNode();
    virtual bool vAddChild(shared_ptr<ISceneNode> kid) override;
    virtual HRESULT vRenderChildren(Scene *pScene) override;
    virtual bool vRemoveChild(ActorId id) override;
    virtual bool vIsVisible(UNUSED Scene *pScene) const override {return true;}
};

//  A camera node controls the D3D view transform and holds the view
//  frustum definition
class CameraNode: public SceneNode
{
public:
    CameraNode(Mat4x4 const *t, Frustum const &frustum)
        :SceneNode(INVALID_ACTOR_ID,WeakBaseRenderComponentPtr(),RenderPass_0, t),
          m_Frustum(frustum),
          m_pTarget(shared_ptr<SceneNode>()),
          m_CamOffsetVector(0.0f,0.0f,4.0f,0.0f),
          m_bActive(true),
          m_DebugCamera(false)
    {}
    virtual HRESULT vRender(Scene *pScene) override;
    virtual HRESULT vOnRestore(Scene *pScene) override;
    virtual bool vIsVisible(UNUSED Scene *pScene) const override {return m_bActive;}
    const Frustum &getFrustum(){return m_Frustum;}
    void setTarget(shared_ptr<SceneNode> pTarget){m_pTarget = pTarget;}
    void clearTarget(){m_pTarget = shared_ptr<SceneNode>();}
    shared_ptr<SceneNode> getTarget() { return m_pTarget;}
    Mat4x4 getWorldViewProjection(Scene *pScene);
    HRESULT setViewTransform(Scene *pScene);
    void setLookAt();
    Mat4x4 getProjection(){return m_Projection;}
    Mat4x4 getView() {return m_View;}
    void setCameraOffset(const Vec4 & cameraOffset){ m_CamOffsetVector = cameraOffset; }
    void setViewMatrix(const Mat4x4 & view) { m_View = view; }
protected:
    Frustum m_Frustum;
    Mat4x4 m_Projection;
    Mat4x4 m_View;
    shared_ptr<SceneNode> m_pTarget;
    Vec4 m_CamOffsetVector; //Direction of camera relative to target
    bool m_bActive;
    bool m_DebugCamera;
};

#endif // SCENENODE_H

