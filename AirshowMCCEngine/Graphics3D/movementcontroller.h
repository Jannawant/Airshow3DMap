#ifndef MOVEMENTCONTROLLER_H
#define MOVEMENTCONTROLLER_H

#include "string"
#include "geometry.h"
#include "scenenode.h"
#include "Utilities/types.h"

using std::string;

class IKeyboardHandler
{
public:
    virtual bool vOnKeyDown(const char c) = 0;
    virtual bool vOnKeyUp(const char c) = 0;
};

class IPointerHandler
{
public:
    virtual bool vOnPointerMove(const Point &pos, const int radius) = 0;
    virtual bool vOnPointerButtonDown(const Point& pos, const int radius, const std::string &buttonName) = 0;
    virtual bool vOnPointerButtonUp(const Point& pos, const int radius, const std::string &buttonName) = 0;
};

class MovementController : public IPointerHandler, public IKeyboardHandler
{
protected:
    Mat4x4 m_matFromWorld;
    Mat4x4 m_matToWorld;
    Mat4x4 m_matPosition;
    Point m_lastMousePos;
    bool m_bKey[256];   //which keys are up and down
    //orientation controls
    float m_fTargetYaw;
    float m_fTargetPitch;
    float m_fYaw;
    float m_fPitch;
    float m_fPitchOnDown;
    float m_fYawOnDown;
    float m_maxSpeed;
    float m_currentSpeed;
    //Added for Ch19/20 refactor
    bool m_mouseLButtonDown;
    bool m_bRotateWhenLButtonDown;
    shared_ptr<SceneNode> m_object;
public:
    MovementController(shared_ptr<SceneNode> object, float initialYaw, float initialPitch, bool rotateWhenLButtonDown);
    void setObject(shared_ptr<SceneNode> newObject);
    void onUpdate(DWORD const deltaMilliseconds);
    bool vOnPointerMove(const Point &mousePos, const int radius);
    bool vOnPointerButtonDown(const Point &mousePos, const int radius, const std::string &butotnName);
    bool vOnPointerButtonUp(const Point &mousePos, const int radius, const std::string &buttonName);
    bool vOnKeyDown(const char c) { m_bKey[c] = true; return true;}
    bool vOnKeyUp(const char c) { m_bKey[c] = false; return true;}
    const Mat4x4* getToWorld(){return &m_matToWorld;}
    const Mat4x4* getFromWorld(){return &m_matFromWorld;}
};



































































#endif // MOVEMENTCONTROLLER_H
