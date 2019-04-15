#ifndef RENDERCOMPONENTINTERFACE_H
#define RENDERCOMPONENTINTERFACE_H

#include "actorcomponent.h"
#include "Graphics3D/scenenode.h"

class RenderComponentInterface : public ActorComponent
{
public:
    virtual shared_ptr<SceneNode> vGetSceneNode(void)=0;
};

#endif // RENDERCOMPONENTINTERFACE_H
