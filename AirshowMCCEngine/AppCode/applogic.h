#ifndef APPLOGIC_H
#define APPLOGIC_H

#include <Actors/actor.h>
#include <MainLoop/processmanager.h>

typedef std::map<ActorId, StrongActorPtr> ActorMap;


class ActorFactory;

class EarthProcess : public Process
{
    ActorId m_target;
    enum State
    {
        NORMAL,
        DECREASE,
        INCREASE
    };
    State m_state;

public:
    EarthProcess(ActorId target){m_target = target; m_state=NORMAL;}
    void vOnUpdate(float deltaMs) override;
};

class AppLogic
{
protected:
    ActorMap m_actors;
    ActorFactory * m_pActorFactory;
    ProcessManager * m_pProcessManager;
    ActorId m_LastActorId;
public:
    AppLogic();
    virtual ~AppLogic();
    bool init();
    virtual ActorFactory* vCreateActorFactory(void);
    virtual StrongActorPtr vCreateActor(const std::string &actorResource, TiXmlElement* overrides=nullptr,
                                       const Mat4x4* initialTransform=nullptr, const ActorId serversActorId=INVALID_ACTOR_ID);
    virtual WeakActorPtr vGetActor(const ActorId actorId);
    virtual void vDestroyActor(const ActorId actorId);
    virtual void vModifyActor(const ActorId actorId, TiXmlElement *overrides){}
    virtual void vMoveActor(const ActorId id, Mat4x4 const &mat) {}
    virtual void vOnUpdate(float time, float elapsedTime);
    ActorId GetNewActorID()
    {
        return ++m_LastActorId;
    }
};

#endif // APPLOGIC_H
