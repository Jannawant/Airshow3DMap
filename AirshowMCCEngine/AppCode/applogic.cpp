#include "applogic.h"
#include <Actors/actorfactory.h>
#include "AppCode/mainapp.h"
#include <Actors/transformcomponent.h>
#include <Graphics3D/scene.h>

#include "Graphics3D/texture.h"

AppLogic::AppLogic()
{
    m_LastActorId = 0;
    m_pProcessManager = GCC_NEW ProcessManager;
    m_pActorFactory = nullptr;
}

AppLogic::~AppLogic()
{
    SAFE_DELETE(m_pProcessManager);
    SAFE_DELETE(m_pActorFactory);
    for(auto it=m_actors.begin();it!=m_actors.end(); ++it)
        it->second->destroy();
    m_actors.clear();
}

bool AppLogic::init()
{
    m_pActorFactory = vCreateActorFactory();
    if(!m_pActorFactory)
    {
        GCC_ERROR("Failed to initialize ActorFactory");
        return false;
    }
    StrongActorPtr pt = vCreateActor("assets/actors/earth.xml");
    StrongProcessPtr earthAnimation;
    earthAnimation.reset(GCC_NEW EarthProcess(pt->getId()));
    m_pProcessManager->attachProcess(earthAnimation);
    shared_ptr<SceneNode> earthNode = static_pointer_cast<SceneNode>(g_pApp->getScene()->findActor(pt->getId()));
    g_pApp->getScene()->getCamera()->setTarget(earthNode);

    return true;
}

ActorFactory *AppLogic::vCreateActorFactory()
{
    return GCC_NEW ActorFactory;
}


// TiXmlEleement parameter need for creating initial actors from xml file, where overridedis is next TiXmlElement that contain Attribute resource name
// Set to nullptr by developing
StrongActorPtr AppLogic::vCreateActor(const std::string &actorResource, TiXmlElement* overrides,
                                     const Mat4x4* initialTransform, const ActorId serversActorId)
{
    GCC_ASSERT(m_pActorFactory);

    StrongActorPtr pActor = m_pActorFactory->createActor(actorResource.c_str(), overrides, initialTransform, serversActorId);
    if (pActor)
    {
        m_actors.insert(std::make_pair(pActor->getId(), pActor));
        //scene.addChild(pActor->getId(),);
        return pActor;
    }
    cout<<"createActor fail: return empty Actor"<<endl;
    return StrongActorPtr();
}

WeakActorPtr AppLogic::vGetActor(const ActorId actorId)
{
    ActorMap::iterator findIt = m_actors.find(actorId);
    if(findIt != m_actors.end())
        return findIt->second;
    cout<<"getActor "<<actorId<<" failed"<<endl;
    return WeakActorPtr();
}

void AppLogic::vDestroyActor(const ActorId actorId)
{
    // We need to trigger a synchronous event to ensure that any systems responding to this event can still access a
    // valid actor if need be.  The actor will be destroyed after this.
    //shared_ptr<EvtData_Destroy_Actor> pEvent(GCC_NEW EvtData_Destroy_Actor(actorId));
    //IEventManager::Get()->VTriggerEvent(pEvent);
    auto findIt = m_actors.find(actorId);
    if(findIt!=m_actors.end())
    {
        findIt->second->destroy();
        m_actors.erase(findIt);
    }
}

void EarthProcess::vOnUpdate(float deltaMs)
{
    static int scalingTime = 0;
    scalingTime += deltaMs;
    cout<<"Update EarthProcess: "<<deltaMs<<endl;
    StrongActorPtr pTarget = makeStrongPtr(g_pApp->m_logic->vGetActor(m_target));
    shared_ptr<TransformComponent> pTargetTransform = makeStrongPtr(pTarget->getComponent<TransformComponent>(TransformComponent::g_Name));

    if (!pTarget || !pTargetTransform )
    {
       GCC_ERROR("This shouldn't happen");
       fail();
    }
//    m_rotation += 0.1f;
//    glm::mat4x4 rot = glm::rotate(glm::mat4x4(1.0), glm::radians(m_rotation), glm::vec3(0,1,0));
//    glm::mat4x4 tr = glm::translate(glm::mat4x4(1.0), glm::vec3(0,0,-3));
//    glm::mat4x4 mv = tr * rot;
//    pTargetTransform->setTransform(mv);

//    if(scalingTime >= 5000) m_state = INCREASE;
//    if(scalingTime >= 20000) m_state = NORMAL;
//    if(scalingTime >= 30000) {m_state = DECREASE; }
//    if(scalingTime >= 45000) {m_state = NORMAL; scalingTime = 0;}
    float angleVInMs = 0.01f;
    Mat4x4 r ;
    r.buildRotationY(angleVInMs * deltaMs);
    //r.buildRotationY(0.16f);
    Mat4x4 t = pTargetTransform->getTransform();
    Mat4x4 scale;
    Mat4x4 mw;
    switch (m_state)
    {
        case NORMAL:
            cout<<"NORMAL "<<scalingTime<<endl;
            mw = t*r;
            break;
        case DECREASE:
            cout<<"DECREASE "<< scalingTime<< endl;
            scale.buildScale(1,1,1.0005f);
            mw = t*r*scale;
            break;
        case INCREASE:
            cout<<"INCREASE "<<scalingTime<<endl;
            scale.buildScale(1,1,0.9995f);
            mw = t*r*scale;
            break;
    }

    cout<<"Transformed matrix: "<<mw<<endl;
    pTargetTransform->setTransform(mw);
}

void AppLogic::vOnUpdate(float time, float elapsedTime)
{
    m_pProcessManager->updateProcesses(elapsedTime);
    int deltaMilliseconds = int(elapsedTime * 1000.0f);
    for(auto it=m_actors.begin();it!=m_actors.end();++it)
    {
        it->second->update(deltaMilliseconds);
    }
}
















































