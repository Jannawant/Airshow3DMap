#ifndef ACTOR_H
#define ACTOR_H

#include "actorcomponent.h"
#include <map>

using std::static_pointer_cast;
using std::map;

//class TiXmlElement;
typedef std::string ActorType;

class Actor
{
    friend class ActorFactory;
    typedef std::map<ComponentId, StrongActorComponentPtr> ActorComponents;
    ActorId m_id;
    ActorComponents m_components;
    ActorType m_type;
    // [mrmike] - these were added post press as editor helpers, but will also be great for save game files if we ever make them.
    std::string m_resource;			// the XML file from which this actor was initialized (considered the "Archetype" file)
public:
    explicit Actor(ActorId id);
    ~Actor(void);
    bool init(TiXmlElement * pData);
    void postInit(void);
    void destroy(void);
    void update(int deltams);
    // editor functions
    //bool SaveActorFromEditor(const char* path);      //[mrmike] - we can delete this.
    //std::string Actor::ToXML();

    ActorId getId(void) const { return m_id;}
    ActorType getType(void) const { return m_type;}

    template<class ComponentType>
    weak_ptr<ComponentType> getComponent(ComponentId id);
    template<class ComponentType>
    weak_ptr<ComponentType> getComponent(const char *name);

    const ActorComponents* getComponents() { return &m_components;}
    void addComponent(StrongActorComponentPtr pComponent);
};

template<class ComponentType>
weak_ptr<ComponentType> Actor::getComponent(ComponentId id)
{
    ActorComponents::iterator findIt = m_components.find(id);
    if(findIt!=m_components.end())
    {
        StrongActorComponentPtr pBase(findIt->second);
        shared_ptr<ComponentType> pSub(static_pointer_cast<ComponentType>(pBase));
        weak_ptr<ComponentType> pWeakSub(pSub);
        return pWeakSub; // return the weak pointer
    }
    else
    {
        return weak_ptr<ComponentType>();
    }
}

template<class ComponentType>
weak_ptr<ComponentType> Actor::getComponent(const char *name)
{
    ComponentId id = ActorComponent::getIdFromName(name);
    ActorComponents::iterator findIt = m_components.find(id);
    if(findIt!=m_components.end())
    {
        StrongActorComponentPtr pBase(findIt->second);
        shared_ptr<ComponentType> pSub(static_pointer_cast<ComponentType>(pBase)); // cast to subclass version of the pointer
        weak_ptr<ComponentType> pWeakSub(pSub); // convert strong pointer to weak pointer
        return pWeakSub; //return the weak pointer
    }
    else{
        return weak_ptr<ComponentType>();
    }
}

#endif // ACTOR_H
