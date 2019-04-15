#include "actor.h"
#include "actorcomponent.h"
#include "Utilities/stringUtility.h"

#include <iostream>

Actor::Actor(ActorId id)
{
    m_id = id;
    m_type = "Unknown";
    //m_resource = "Unknown";
}

Actor::~Actor()
{
    //GCC_LOG("Actor",std::string("Destoying Actor") + toStr(m_id));
    GCC_ASSERT(m_components.empty()); // if the assert fires, the actor was destroyed without calling Actor::destroy()
}

bool Actor::init(TiXmlElement* pData)
{
    //GCC_LOG("Actor", std::string("Initializing Actor ") + std::to_string(m_id));
    std::cout<<"Actor initialize "<<std::to_string(m_id)<<std::endl;
    m_type = pData->Attribute("type");
    m_resource = pData->Attribute("resource");
    return true;
}

void Actor::postInit()
{
    for(ActorComponents::iterator it = m_components.begin(); it!=m_components.end(); ++it)
    {
        it->second->vPostInit();
    }
}

void Actor::destroy()
{
    m_components.clear();
}

void Actor::update(int deltaMs)
{
    for(ActorComponents::iterator it =m_components.begin(); it!=m_components.end(); ++it)
    {
        it->second->vUpdate(deltaMs);
    }
}


//std::string Actor::ToXML()
//{
//    TiXmlDocument outDoc;

//    // Actor element
//    TiXmlElement* pActorElement = GCC_NEW TiXmlElement("Actor");
//    pActorElement->SetAttribute("type", m_type.c_str());
//	pActorElement->SetAttribute("resource", m_resource.c_str());

//    // components
//    for (auto it = m_components.begin(); it != m_components.end(); ++it)
//    {
//        StrongActorComponentPtr pComponent = it->second;
//        TiXmlElement* pComponentElement = pComponent->VGenerateXml();
//        pActorElement->LinkEndChild(pComponentElement);
//    }

//    outDoc.LinkEndChild(pActorElement);
//	TiXmlPrinter printer;
//	outDoc.Accept(&printer);

//	return printer.CStr();
//}

void Actor::addComponent(StrongActorComponentPtr pComponent)
{
    std::pair<ActorComponents::iterator, bool> success = m_components.insert(std::make_pair(pComponent->vGetId(), pComponent));
    GCC_ASSERT(success.second);
}






















