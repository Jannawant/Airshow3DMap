#include "scenenode.h"
#include <list>
#include "scene.h"
#include "geometry.h"
#include <iostream>

#include "Actors/rendercomponent.h"
#include "Actors/transformcomponent.h"
#include "Utilities/templates.h"
#include "AppCode/mainapp.h"

using std::cout;
using std::endl;

SceneNode::SceneNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Mat4x4 *to, const Mat4x4 *from)
{
    m_Parent = nullptr;
    m_Props.m_ActorId = actorId;
    m_Props.m_Name = (renderComponent) ? renderComponent->vGetName() : "SceneNode";
    m_Props.m_RenderPass = renderPass;
    //m_Props.m_AlphaType = AlphaOpaque;
    m_RenderComponent = renderComponent;
    vSetTransform(to,from);
    setRadius(0);
    //m_Props.m_Material.setDiffuse(diffuseColor);
}

void SceneNode::vSetTransform(const Mat4x4 *toWorld, const Mat4x4 *fromWorld)
{
    m_Props.m_ToWorld = *toWorld;
    if(!fromWorld)
    {
        m_Props.m_FromWorld = m_Props.m_ToWorld.inverse();
//        cout<<"m_FromWorld ="<<endl;
//        for(int i=0; i<m_Props.m_FromWorld.getMat4x4().length(); i++)
//            for(int j=0; j<m_Props.m_FromWorld.getMat4x4()[i].length();j++)
//                cout << m_Props.m_FromWorld.getMat4x4()[i][j] << " ";
    }
    else
    {
        m_Props.m_FromWorld = *fromWorld;
    }
}

HRESULT SceneNode::vOnRestore(Scene *pScene)
{
    //Color color = (m_RenderComponent) ? m_RenderComponent->getColor() : g_White;
    //m_Props.m_Material.setDiffuse(color);
    SceneNodeList::iterator i   = m_Children.begin();
    SceneNodeList::iterator end = m_Children.end();
    while(i!=end)
    {
        (*i)->vOnRestore(pScene);
        i++;
    }
    return S_OK;
}

HRESULT SceneNode::vOnUpdate(Scene *pScene, DWORD const elapsedMs)
{
    SceneNodeList::iterator i   = m_Children.begin();
    SceneNodeList::iterator end = m_Children.end();
    while(i!=end)
    {
        (*i)->vOnUpdate(pScene, elapsedMs);
        i++;
    }
    return S_OK;
}

HRESULT SceneNode::vPreRender(Scene *pScene)
{
    StrongActorPtr pActor = makeStrongPtr(g_pApp->getAppLogic()->vGetActor(m_Props.m_ActorId));
    if(pActor)
    {
        shared_ptr<TransformComponent> pTc = makeStrongPtr(pActor->getComponent<TransformComponent>(TransformComponent::g_Name));
        if(pTc)
        {
            m_Props.m_ToWorld = pTc->getTransform();
        }
    }
    pScene->pushAndSetMatrix(m_Props.m_ToWorld);
    return S_OK;
}

HRESULT SceneNode::vPostRender(Scene *pScene)
{
    pScene->popMatrix();
    return S_OK;
}


bool SceneNode::vIsVisible(Scene *pScene) const
{
    // transform the location of this node into the camera space
    // of the camera attached to the scene

    Mat4x4 toWorld, fromWorld;
    pScene->getCamera()->vGet()->transform(&toWorld, &fromWorld);
    Vec3 pos = vGet()->toWorld().getPosition();

    pos = fromWorld.xForm(pos);
    Frustum const &frustum = pScene->getCamera()->getFrustum();
    return frustum.inside(pos, vGet()->radius());
}

struct AlphaSceneNode
{
    shared_ptr<ISceneNode> m_pNode;
    Mat4x4 m_Concat;
    float m_ScreenZ;
    // For the STL sort...
    bool operator < (AlphaSceneNode const &other)
    { return m_ScreenZ < other.m_ScreenZ; }
};
typedef std::list<AlphaSceneNode *> AlphaSceneNodes;

HRESULT SceneNode::vRenderChildren(Scene *pScene)
{
    SceneNodeList::iterator i   = m_Children.begin();
    SceneNodeList::iterator end = m_Children.end();
    if(i != m_Children.end())
        cout<<(*i)->vGet()->name()<<endl;
    while(i!=end)
    {
        if((*i)->vPreRender(pScene)==S_OK)
        {
            (*i)->vRender(pScene);
            //you could short-circuit rendering
            //if an object returns E_FAIL from
            //vPreRender()

            //Dont render this node if you cant see it
//            if((*i)->vIsVisible(pScene))
//            {
//                  float alpha = (*i)->vGet()->m_Material.getAlpha();
//                  if(alpha==fOPAQUE)
//                  {
//                      (*i)->vRender(pScene);
//                  }
//                  else if(alpha!=fTRANSPARENT)
//                  {
//                      //The object isnt totally transparent...
//                      AlphaSceneNode *asn == GCC_NEW AlphaSceneNode;
//                      assert(asn);
//                      asn->m_pNode = *i;
//                      asn->m_Concat = *pScene->getTopMatrix();

//                      Vec4 worldPos(asn->m_Concat.getPosition());
//                      Mat4x4 fromWorld = pScene->getCamera()->vGet()->fromWorld();
//                      Vec4 screenPos = fromWorld.xForm(worldPos);
//                      asn->m_ScreenZ = screenPos.z;
//                      pScene->addAlphaSceneNode(asn);
//                  }
//            }
            (*i)->vRenderChildren(pScene);
        }
        (*i)->vPostRender(pScene);
        i++;
    }
    return S_OK;
}


bool SceneNode::vAddChild(shared_ptr<ISceneNode> kid)
{
    m_Children.push_back(kid);

    //the radius of the sphere should be fixed right here

    Vec3 kidPos = kid->vGet()->toWorld().getPosition();
    float newRadius = kidPos.length() + kid->vGet()->radius();

    //Vec3 dir = kidPos - m_Props.toWorld().getPosition();
    //float newRadius = dir.length() + kid->vGet()->radius();
    if(newRadius > m_Props.m_Radius)
        m_Props.m_Radius = newRadius;
    return true;
}

bool SceneNode::vRemoveChild(ActorId id)
{
    for(SceneNodeList::iterator i=m_Children.begin();i!=m_Children.end(); ++i)
    {
        const SceneNodeProperties *pProps = (*i)->vGet();
        if(pProps->actorId() != INVALID_ACTOR_ID && id==pProps->actorId())
        {
            i = m_Children.erase(i);    //this can be expensive for vectors
            return true;
        }
    }
    return false;
}

HRESULT SceneNode::vOnLostDevice(Scene *pScene)
{
    //This is meant to be called from any class
    //that inherits from SceneNode and overloads
    //VOnRestore
    SceneNodeList::iterator i = m_Children.begin();
    SceneNodeList::iterator end = m_Children.end();
    while(i!=end)
    {
        (*i)->vOnLostDevice(pScene);
        ++i;
    }
    return S_OK;
}

const Vec3 SceneNode::getWorldPosition() const
{
    Vec3 pos = getPosition();
    if(m_Parent)
    {
        pos+=m_Parent->getWorldPosition();
    }
    return pos;
}

HRESULT CameraNode::vOnRestore(UNUSED Scene *pScene)
{
    //float width = 1920;
    //float height = 1080;
    //m_Frustum.setAspect(width / height);
    glm::mat4x4 p = (glm::perspective(glm::radians(m_Frustum.m_Fov),m_Frustum.m_Aspect, m_Frustum.m_Near, m_Frustum.m_Far));
    m_Projection = Mat4x4(p);
    return S_OK;
}

//Returns the concatenation of the world and view projection , which is generally sent into vertex shaders
HRESULT CameraNode::vRender(UNUSED Scene *pScene)
{
    if(m_DebugCamera)
    {
        pScene->popMatrix();
        m_Frustum.render();
        pScene->pushAndSetMatrix(m_Props.toWorld());
    }
    return S_OK;
}

Mat4x4 CameraNode::getWorldViewProjection(Scene *pScene)
{
    Mat4x4 world = pScene->getTopMatrix();
    cout<<"SceneTopMatrix and World cam: "<<world<<endl;
    Mat4x4 view = vGet()->fromWorld();
    cout<<"View cam: "<<view<<endl;
//    Mat4x4 worldView =  world * view;
//    return worldView * m_Projection;
    Mat4x4 worldView =  view*world;
    return m_Projection * worldView;
}

HRESULT CameraNode::setViewTransform(UNUSED Scene *pScene)
{
    //if there is a target make sure the camera is
    //rigidly attached right behind the target
//    if(m_pTarget)
//    {
//        cout<<"if m_pTarget true"<<endl;
//        Mat4x4 mat = m_pTarget->vGet()->toWorld();
//        Vec3 pos = mat.getPosition() + Vec3(mat.xForm(m_CamOffsetVector));
//        mat.setPosition(pos);
//        Mat4x4 m = mat.inverse();
//        vSetTransform(&mat);
//    }
    cout<<"Camera fromWorld: "<< vGet()->fromWorld()<<endl;
    m_View = vGet()->fromWorld();
    //pScene->getRenderer()->vSetViewTransform(&m_View);
    return S_OK;
}

void CameraNode::setLookAt()
{
    Mat4x4 mat = m_pTarget->vGet()->toWorld();
    Mat4x4 matView;
    //Vec3 vFromPt = Vec3( 0.00, 1.0, 0.0 );
    Vec3 vFromPt = this->getPosition();
    Vec3 vLookatPt = mat.getPosition();//Vec3( 0.0, 0.0, -3.0 );
    Vec3 vUpVec = Vec3( 0.0, 1.0, 0.0 );
    matView.buildRotationLookAt(vFromPt, vLookatPt, vUpVec );
    Mat4x4 m = matView.inverse();
    vSetTransform(&m);
    m_View = vGet()->fromWorld();
}

RootNode::RootNode()
    :SceneNode (INVALID_ACTOR_ID,WeakBaseRenderComponentPtr(),RenderPass_0, &Mat4x4::g_Identity)
{
    m_Children.reserve(RenderPass_Last);
    shared_ptr<SceneNode> staticGroup( GCC_NEW SceneNode(INVALID_ACTOR_ID,WeakBaseRenderComponentPtr(),RenderPass_Static,&Mat4x4::g_Identity));
    m_Children.push_back(staticGroup);  //RenderPass_Static = 0
    shared_ptr<SceneNode> actorGroup(GCC_NEW SceneNode(INVALID_ACTOR_ID,WeakBaseRenderComponentPtr(),RenderPass_Actor,&Mat4x4::g_Identity));
    m_Children.push_back(actorGroup);  //RenderPass_Static = 1
    shared_ptr<SceneNode> skyGroup(GCC_NEW SceneNode(INVALID_ACTOR_ID,WeakBaseRenderComponentPtr(), RenderPass_Sky, &Mat4x4::g_Identity));
    m_Children.push_back(skyGroup);// RenderPass_Sky = 2
    shared_ptr<SceneNode> invisibleGroup( GCC_NEW SceneNode(INVALID_ACTOR_ID,WeakBaseRenderComponentPtr(), RenderPass_NotRendered, &Mat4x4::g_Identity));
    m_Children.push_back(invisibleGroup); // RenderPass_NotRendered = 3
}

HRESULT RootNode::vRenderChildren(Scene *pScene)
{
    for(int pass = RenderPass_0; pass < RenderPass_Last; ++pass)
    {
        switch(pass)
        {
        case RenderPass_Static:
        case RenderPass_Actor:
            m_Children[pass]->vRenderChildren(pScene);
            break;
        case RenderPass_Sky:
        {
            //shared_ptr<IRenderState> skyPass = pScene->getRenderer()->vPrepareSkyBoxPass();
            m_Children[pass]->vRenderChildren(pScene);
            break;
        }
        }
    }
    return S_OK;
}

bool RootNode::vRemoveChild(ActorId id)
{
    bool anythingRemoved = false;
    for(int i=RenderPass_0; i<RenderPass_Last; ++i)
    {
        if(m_Children[i]->vRemoveChild(id))
        {
            anythingRemoved = true;
        }
    }
    return anythingRemoved;
}

bool RootNode::vAddChild(shared_ptr<ISceneNode> kid)
{
    //The Root node has children that divide the scene graph into render passes.
    //Scene nodes will get added to these childre based on the value of the
    //render pass member variable
    RenderPass pass = kid->vGet()->renderPass();
    if(static_cast<unsigned>(pass) >= m_Children.size() || !m_Children[pass])
    {
        //GCC_ASSERT(0 && _T("There is no such render pass"));
        std::cout << "There is no such render pass" << endl;
        return false;
    }
    return m_Children[pass]->vAddChild(kid);
}






















