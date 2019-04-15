#ifndef ACTORCOMPONENT_H
#define ACTORCOMPONENT_H

#include "codestd.h"
//#include "Utilities/templates.h"
#include "Utilities/stringUtility.h"

class TiXmlElement;

class ActorComponent
{
    friend class ActorFactory;
protected:
    StrongActorPtr m_pOwner;
public:
    virtual ~ActorComponent(void){m_pOwner.reset();}
    virtual bool vInit(TiXmlElement* pData) = 0;
    virtual void vPostInit(void){}
    virtual void vUpdate(int deltaMs){}
    virtual void vOnChanged(void){}
    //for the editor
    //virtual TiXmlElement* vGenerateXml(void) = 0;
    virtual ComponentId vGetId(void) const;
    virtual const char *vGetName() const = 0;
    static ComponentId getIdFromName(const char * componentStr);
private:
    void setOwner(StrongActorPtr pOwner){m_pOwner = pOwner;}
};




























#endif // ACTORCOMPONENT_H
