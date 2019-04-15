#ifndef CAMERA_H
#define CAMERA_H

#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/quaternion.hpp>
#include <Graphics3D/scenenode.h>
#include "Graphics3D/geometry.h"

class Camera
{
public:
    Camera();
    Camera(int screenWidth, int screenHeight);
    void init(Scene *scene);
    void SetViewPort(int x, int y, int width, int height);
    Vec4 GetViewPort() const;

    void SetProjectionRH(float fov, float aspectRatio, float zNear, float zFar);
    void ApplyViewMatrix();
    void SetPosition(const Vec3& pos);
    Vec3 GetPosition() const;

    //Translate the camera by some amount. If local is TRUE(default) then the translation should
    //be applied int local-space of the camera. If local is FALSE, then the translation is
    //applied in world-space.
    void Translate(const Vec3 & delta, bool local = true);

    void SetRotation(const Quaternion& rot);
    Quaternion GetRotation() const;

    void SetEulerAngles(const Vec3& eulerAngles);
    Vec3 GetEulerAngles() const;

    //Rotate the camera by some amount
    void Rotate(const Quaternion& rot);

    Mat4x4 GetProjectionMatrix() const;
    Mat4x4 GetViewMatrix();

    void setMoveSpeed(double speed){m_moveSpeed = speed; }
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

protected:
    void UpdateViewMatrix();
    Vec4 m_Viewport;
    Vec3 m_Position;
    Quaternion m_Rotation;
    Mat4x4 m_ProectionMatrix;
    Mat4x4 m_ViewMatrix;
private:
    shared_ptr<CameraNode> m_pCameraNode;
    int m_width;
    int m_height;
    bool m_ViewDirty;
    bool m_isFree;
    double m_moveSpeed;
};

#endif // CAMERA_H
