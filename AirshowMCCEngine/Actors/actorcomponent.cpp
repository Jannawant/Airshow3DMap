#include "actorcomponent.h"
#include <iostream>
using std::cout; using std::endl;
ComponentId ActorComponent::vGetId() const { return getIdFromName(vGetName());}

ComponentId ActorComponent::getIdFromName(const char *componentStr)
{
    void * rawId = HashedString::hash_name(componentStr);
    return reinterpret_cast<ComponentId>(rawId);
}
