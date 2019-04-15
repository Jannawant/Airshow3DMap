#ifndef ACTORFACTORY_H
#define ACTORFACTORY_H

#include "actorcomponent.h"
#include <string>
#include <Utilities/templates.h>

using std::string;
using std::map;


typedef ActorComponent* (*ActorComponentCreator)(void);
typedef std::map<std::string, ActorComponentCreator> ActorComponentCreatorMap;


class ActorFactory
{
    ActorId m_lastActorId;
protected:
    GenericObjectFactory<ActorComponent, ComponentId> m_componentFactory;
    //ActorComponentCreatorMap m_actorComponentCreators;
public:
    ActorFactory();
    virtual ~ActorFactory(){}
    StrongActorPtr createActor(const char* actorResource, TiXmlElement* overrides, const Mat4x4* initialTransform, const ActorId serversActorId);
    void modifyActor(StrongActorPtr pActor, TiXmlElement* overrides);
protected:
    virtual StrongActorComponentPtr vCreateComponent(TiXmlElement* pData);
private:
    ActorId getNextActorId(void) {++m_lastActorId; return m_lastActorId;}
};

#endif // ACTORFACTORY_H
