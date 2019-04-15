#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "Actors/actorcomponent.h"


class TransformComponent : public ActorComponent
{
    Mat4x4 m_transform;
public:
    static const char *g_Name;
    virtual const char * vGetName() const override { return g_Name;}
    TransformComponent() : m_transform(Mat4x4::g_Identity){}
    virtual bool vInit(TiXmlElement *pData) override;

    Mat4x4 getTransform()const { return m_transform;}
    void setTransform(const Mat4x4& newTransform) { m_transform = newTransform;}
    Vec3 getPosition() const { return m_transform.getPosition();}
    void setPosition(const Vec3& pos) { m_transform.setPosition(pos);}
    Vec3 getLookAt(void) const {return m_transform.getDirection();}
};

#endif // TRANSFORMCOMPONENT_H
