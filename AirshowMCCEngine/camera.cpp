#include "camera.h"
#include <GLES2/gl2.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Graphics3D/scene.h"

Camera::Camera():
    m_ViewDirty(false)
    ,m_isFree(true)
{
}

Camera::Camera(int screenWidth, int screenHeight)
    :m_Viewport(0,0,screenWidth, screenHeight)
    ,m_width(screenWidth)
    ,m_height(screenHeight)
    ,m_ViewDirty(false)
    ,m_isFree(true)
{}

void Camera::init(Scene *scene)
{
    m_Position = Vec3(0.0, 2.0, 2.0);
    static Frustum frustum;
    frustum.init(40.0f, m_width/static_cast<float>(m_height), 0.1f, 200.0f);
    m_pCameraNode.reset(GCC_NEW CameraNode(&Mat4x4::g_Identity,frustum));
    scene->addChild(INVALID_ACTOR_ID, m_pCameraNode);
    scene->setCamera(m_pCameraNode);
    m_pCameraNode->vOnRestore(scene);
}

void Camera::SetViewPort(int x, int y, int width, int height)
{
    m_Viewport = Vec4(x,y,width,height);
    glViewport(x,y,width,height);
}

Vec4 Camera::GetViewPort() const
{
    return m_Viewport;
}

void Camera::SetProjectionRH(float fov, float aspectRatio, float zNear, float zFar)
{
    m_ProectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
}

void Camera::ApplyViewMatrix()
{
    UpdateViewMatrix();
}

void Camera::SetPosition(const Vec3 &pos)
{
    m_Position = pos;
    m_ViewDirty = true;
}

Vec3 Camera::GetPosition() const
{
    return m_Position;
}

void Camera::Translate(const Vec3 &delta, bool local /* = true */)
{
    if(local)
    {
//        glm::quat c = m_Rotation.getQuat();
//        c *= delta;
//        glm::vec3 v;
//        v += c;
//        m_Position = Vec3(v);
        //m_pos += m_rot * delta;
    }
    else
    {
        m_Position += delta;
    }
    m_ViewDirty = true;
}

void Camera::SetRotation(const Quaternion &rot)
{
    m_Rotation = rot;
    m_ViewDirty = true;
}

Quaternion Camera::GetRotation() const
{
    return m_Rotation;
}

void Camera::SetEulerAngles(const Vec3 &eulerAngles)
{
    //m_Rotation = glm::quat( glm::radians(eulerAngles));
    m_Rotation = Quaternion(eulerAngles);
}

Vec3 Camera::GetEulerAngles() const
{
    return m_Rotation.eulerAngles();
}

void Camera::Rotate(const Quaternion &rot)
{
    m_Rotation = m_Rotation*rot;
    m_ViewDirty = true;
}

Mat4x4 Camera::GetProjectionMatrix() const
{
    return m_ProectionMatrix;
}

Mat4x4 Camera::GetViewMatrix()
{
    UpdateViewMatrix();
    return m_ViewMatrix;
}

void Camera::moveUp()
{
    Vec3 curRot = m_Rotation.eulerAngles();
    Vec3 newRot = Vec3(curRot.getVec3().x, curRot.getVec3().y+2.0, curRot.getVec3().z);
    SetEulerAngles(Vec3(curRot.getVec3().x, curRot.getVec3().y+2.0, curRot.getVec3().z));
//    Vec3 camPos = m_pCameraNode->getPosition();
//    float xLookAt = camPos.getVec3().x + sin(1.0);
//    float yLookAt = camPos.getVec3().y + cos(1.0);
//    float zLookAt = camPos.getVec3().z;

    Mat4x4 translate;
    translate.buildTranslation(m_Position);
    Mat4x4 rotate = m_Rotation.toMat4x4().transpose();
    m_ViewMatrix =  rotate * translate;
    m_ViewMatrix = m_ViewMatrix.inverse();
    m_pCameraNode->vSetTransform(&m_ViewMatrix);
    //m_pCameraNode->setViewMatrix(m_ViewMatrix);
    //m_pCameraNode->setLookAt();
}

void Camera::moveDown()
{

}

void Camera::moveLeft()
{

}

void Camera::moveRight()
{

}

void Camera::UpdateViewMatrix()
{
    if(m_ViewDirty)
    {
        Mat4x4 translate;
        translate.buildTranslation(-m_Position);
        //glm::mat4 translate = glm::translate(glm::mat4(1.0), -m_Position);
//        //Since we know the rotation matrix is orthonormalized , we can simply
//        // transpose the rotation matrix instead of inversing
        //glm::mat4 rotate = glm::transpose(glm::toMat4(m_Rotation));
        //glm::mat4 rotate = glm::transpose(glm::toMat4(glm::quat(0.5,0.5,0.5,0.5)));
        Mat4x4 rotate = m_Rotation.toMat4x4().transpose();

//        glm::mat4 View = glm::lookAt(
//            glm::vec3(3,0,0), // Камера находится в мировых координатах (4,3,3) z,y
//            glm::vec3(0,0,0), // И направлена в начало координат
//            glm::vec3(0,1,0)  // "Голова" находится сверху
//        );
        m_ViewMatrix =  rotate * translate;
        m_ViewDirty = false;
    }
    //exit(0);
}

