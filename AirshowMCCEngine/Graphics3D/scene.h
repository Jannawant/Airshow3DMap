#ifndef SCENE2_H
#define SCENE2_H

#include <map>
#include <stack>
#include <vector>
#include "scenenode.h"
#include "codestd.h"

using namespace std;

typedef std::map<ActorId, shared_ptr<ISceneNode>> SceneActorMap;
typedef std::stack<Mat4x4 ,vector<Mat4x4> > MatrixStack;

class CameraNode;
class SkyNode;
class LightNode;
class LightManager;

class Scene
{
protected:

    //shared_ptr<IRenderer>   m_Renderer;
    MatrixStack m_MatrixStack;
    //AlphaSceneNodes m_AlphaSceneNodes;
    SceneActorMap   m_ActorMap;
    LightManager    *m_LightManager;
    //void renderAlphaPass();
public:
    shared_ptr<SceneNode>   m_Root;
    shared_ptr<CameraNode>   m_Camera;

    Scene();
    virtual ~Scene();
    HRESULT onRender();
    HRESULT onRestore();
    HRESULT onUpdate(const int deltaMilliseconds);
    HRESULT onLostDevice();
    shared_ptr<ISceneNode> findActor(ActorId id);
    bool addChild(ActorId id, shared_ptr<ISceneNode> kid)
    {
        cout<<"add child to scene"<<endl;
        if(id != INVALID_ACTOR_ID)
        {
            //this allows us to search for this later based on actor id
            m_ActorMap[id]=kid;
        }
//        shared_ptr<LightNode> pLight = dynamic_pointer_cast<LightNode>(kid);
//        if(pLight != nullptr && m_LightManager->m-Lights.size()+1 < MAXIMUM_LIGHTS_SUPPORTED)
//        {
//            m_LightManager->m_Lights.push_back(pLight);
//        }
        cout<<"KID name= "<<kid->vGet()->name()<<endl;
        cout<<"KID pass ="<<kid->vGet()->renderPass()<<endl;
        return m_Root->vAddChild(kid);
    }

    bool removeChild(ActorId id)
    {
        if(id == INVALID_ACTOR_ID)
            return false;

        shared_ptr<ISceneNode> kid = findActor(id);
        //shared_ptr<LightNode> pLight = dynamic_pointer_cast<LightNode>(kid);
//        if(pLight != nullptr)
//            m_LightManager->m_Lights.remove(pLight);
        m_ActorMap.erase(id);
        return m_Root->vRemoveChild(id);
    }
    //Camera accessor / modifier
    void setCamera(shared_ptr<CameraNode> camera) { m_Camera = camera;}
    inline const shared_ptr<CameraNode> getCamera() const { return m_Camera;}
    void pushAndSetMatrix(const Mat4x4 &toWorld);
    void popMatrix();
    const Mat4x4 &getTopMatrix();

    LightManager *getLightManager() { return m_LightManager;}
//    void addAlphaSceneNode(AlphaSceneNode *asn)
//    {
//        m_AlphaSceneNodes.push_back(asn);
//    }
    //shared_ptr<IRenderer> getRenderer() { return m_Renderer; }
};

#endif // SCENE2_H
