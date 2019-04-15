#include "transformcomponent.h"

const char *TransformComponent::g_Name = "TransformComponent";

bool TransformComponent::vInit(TiXmlElement *pData)
{
    GCC_ASSERT(pData);
    Vec3 yawPitchRoll = m_transform.getYawPitchRoll();
    std::cout<<"YawPitchRoll "<<yawPitchRoll.getVec3().x << " "<<yawPitchRoll.getVec3().y << " "<<yawPitchRoll.getVec3().z <<std::endl;
    yawPitchRoll = yawPitchRoll.radiansToDegrees();
    std::cout<<"In degrees YawPitchRoll "<<yawPitchRoll.getVec3().x << " "<<yawPitchRoll.getVec3().y << " "<<yawPitchRoll.getVec3().z <<std::endl;
    Vec3 position = m_transform.getPosition();
    TiXmlElement* pPositionElement = pData->FirstChildElement("Position");
    if(pPositionElement)
    {
        double x = 0;
        double y = 0;
        double z = 0;
        pPositionElement->Attribute("x", &x);
        pPositionElement->Attribute("y", &y);
        pPositionElement->Attribute("z", &z);
        position = Vec3(x,y,z);
    }
    else
    {
        std::cout<< "No pPositionElement" << std::endl;
    }

    TiXmlElement *pOrientationElement = pData->FirstChildElement("YawPitchRoll");
    if(pOrientationElement)
    {
        double yaw = 0;
        double pitch = 0;
        double roll = 0;
        //Yaw pitch roll in degrees from xml?
        pOrientationElement->Attribute("x",&yaw);
        pOrientationElement->Attribute("y",&pitch);
        pOrientationElement->Attribute("z",&roll);
        yawPitchRoll = Vec3(yaw,pitch,roll);
    }
    else {
        std::cout<<"No pOrientationElement"<<std::endl;
    }
    Mat4x4 translation;
    translation.buildTranslation(position);
    std::cout<<"Translation "<< translation <<endl;
    Mat4x4 rotation;
    rotation.buildYawPitchRoll(yawPitchRoll);
    std::cout<<"Rotation"<<rotation<<endl;

    /**
    // This is not supported yet.
    TiXmlElement* pLookAtElement = pData->FirstChildElement("LookAt");
    if (pLookAtElement)
    {
        double x = 0;
        double y = 0;
        double z = 0;
        pLookAtElement->Attribute("x", &x);
        pLookAtElement->Attribute("y", &y);
        pLookAtElement->Attribute("z", &z);

        Vec3 lookAt((float)x, (float)y, (float)z);
        rotation.BuildRotationLookAt(translation.GetPosition(), lookAt, g_Up);
    }

    TiXmlElement* pScaleElement = pData->FirstChildElement("Scale");
    if (pScaleElement)
    {
        double x = 0;
        double y = 0;
        double z = 0;
        pScaleElement->Attribute("x", &x);
        pScaleElement->Attribute("y", &y);
        pScaleElement->Attribute("z", &z);
        m_scale = Vec3((float)x, (float)y, (float)z);
    }
    **/

    //m_transform = rotation * translation;
    m_transform = Mat4x4::g_Identity * translation;
    return true;
}



































