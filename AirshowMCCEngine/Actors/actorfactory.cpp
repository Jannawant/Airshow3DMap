#include "actorfactory.h"
#include "codestd.h"
#include "actorcomponent.h"
#include "rendercomponent.h"
#include "actor.h"
#include "ResourceCache/xmlresource.h"
#include "transformcomponent.h"
#include "AppCode/mainapp.h"

ActorFactory::ActorFactory()
{
    m_lastActorId = INVALID_ACTOR_ID;
    m_componentFactory.Register<TransformComponent>(ActorComponent::getIdFromName(TransformComponent::g_Name));
    m_componentFactory.Register<MeshRenderComponent>(ActorComponent::getIdFromName(MeshRenderComponent::g_Name));
}

StrongActorPtr ActorFactory::createActor(const char *actorResource, TiXmlElement *overrides, const Mat4x4 *pInitialTransform, const ActorId serversActorId)
{
    //Grab the root XML node
    TiXmlElement* pRoot = XmlResourceLoader::loadAndReturnRootXmlElement(actorResource);
    if(!pRoot)
    {
        GCC_ERROR("Failed to create actor from resource:" + std::string(actorResource));
        return StrongActorPtr();
    }
    //create the actor instance
    ActorId nextActorId = serversActorId;
    if(nextActorId == INVALID_ACTOR_ID)
    {
        nextActorId = getNextActorId();
    }
    StrongActorPtr pActor(GCC_NEW Actor(nextActorId));
    if(!pActor->init(pRoot))
    {
        GCC_ERROR("Failed to initialize actor: "+std::string(actorResource));
        return StrongActorPtr();
    }
    // Loop through each child element and load the component
    for(TiXmlElement* pNode=pRoot->FirstChildElement(); pNode; pNode=pNode->NextSiblingElement())
    {
        StrongActorComponentPtr pComponent(vCreateComponent(pNode));
        if(pComponent)
        {
            pActor->addComponent(pComponent);
            pComponent->setOwner(pActor);
        }
        else
        {
            return StrongActorPtr();
        }
    }
    if(overrides)
    {
        modifyActor(pActor,overrides);
    }
    // This is a bit of a hack to get the initial transform of the transform component set before the
    // other components (like PhysicsComponent) read it.
    shared_ptr<TransformComponent> pTransformComponent = makeStrongPtr(pActor->getComponent<TransformComponent>(TransformComponent::g_Name));
    if (pInitialTransform && pTransformComponent)
    {
        pTransformComponent->setPosition(pInitialTransform->getPosition());
    }

    //Now that the actor has been fully created, run the post init phase
    pActor->postInit();
    return pActor;
}

StrongActorComponentPtr ActorFactory::vCreateComponent(TiXmlElement *pData)
{
    const char* name(pData->Value());
    std::cout<<"ActorFactory vCreateComponent: create from name : "<<name<<std::endl;
    StrongActorComponentPtr pComponent(m_componentFactory.create(ActorComponent::getIdFromName(name)));
    //initialize the component if we found one
    if(pComponent)
    {
        if(!pComponent->vInit(pData))
        {
            GCC_ERROR("Component failed to initialize:"+std::string(name));
            return StrongActorComponentPtr();
        }
    }
    else
    {
        GCC_ERROR("Couldn't find ActorComponent named "+std::string(name));
        return StrongActorComponentPtr(); //fail
    }
    // pComponent will be Null if the component wasn't found. This isnt
    // necessarily an error since you might have a custom CreateComponent()
    // function in a subclass.
    return pComponent;
}

void ActorFactory::modifyActor(StrongActorPtr pActor, TiXmlElement* overrides)
{
    // Loop through each child element and load the component
    for (TiXmlElement* pNode = overrides->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
    {
        ComponentId componentId = ActorComponent::getIdFromName(pNode->Value());
        StrongActorComponentPtr pComponent = makeStrongPtr(pActor->getComponent<ActorComponent>(componentId));
        if (pComponent)
        {
            pComponent->vInit(pNode);

            // [mrmike] - added post press to ensure that components that need it have
            //            Events generated that can notify subsystems when changes happen.
            //            This was done to have SceneNode derived classes respond to RenderComponent
            //            changes.

            pComponent->vOnChanged();
        }
        else
        {
            pComponent = vCreateComponent(pNode);
            if (pComponent)
            {
                pActor->addComponent(pComponent);
                pComponent->setOwner(pActor);
            }
        }
    }
}






































