#include "camera.h"
#include <GLES2/gl2.h>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
    :m_Viewport(0)
    ,m_Position(0)
    ,m_Rotation()
    ,m_ProectionMatrix(1)
    ,m_ViewMatrix(1)
    ,m_ViewDirty(false)
{
}

Camera::Camera(int screenWidth, int screenHeight)
    :m_Viewport(0,0,screenWidth, screenHeight)
    ,m_Position(0)
    ,m_Rotation()
    ,m_ProectionMatrix(1)
    ,m_ViewMatrix(1)
    ,m_ViewDirty(false)
{}

void Camera::SetViewPort(int x, int y, int width, int height)
{
    m_Viewport = glm::vec4(x,y,width,height);
    glViewport(x,y,width,height);
}

glm::vec4 Camera::GetViewPort() const
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

void Camera::SetPosition(const glm::vec3 &pos)
{
    m_Position = pos;
    m_ViewDirty = true;
}

glm::vec3 Camera::GetPosition() const
{
    return m_Position;
}

void Camera::Translate(const glm::vec3 &delta, bool local /* = true */)
{
    if(local)
    {
        m_Position += m_Rotation * delta;
    }
    else
    {
        m_Position += delta;
    }
    m_ViewDirty = true;
}

void Camera::SetRotation(const glm::quat &rot)
{
    m_Rotation = rot;
    m_ViewDirty = true;
}

glm::quat Camera::GetRotation() const
{
    return m_Rotation;
}

void Camera::SetEulerAngles(const glm::vec3 &eulerAngles)
{
    m_Rotation = glm::quat(glm::radians(eulerAngles));
}

glm::vec3 Camera::GetEulerAngles() const
{
    return glm::degrees(glm::eulerAngles(m_Rotation));
}

void Camera::Rotate(const glm::quat &rot)
{
    m_Rotation = m_Rotation*rot;
    m_ViewDirty = true;
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    return m_ProectionMatrix;
}

glm::mat4 Camera::GetViewMatrix()
{
    UpdateViewMatrix();
    return m_ViewMatrix;
}

void Camera::UpdateViewMatrix()
{
    if(m_ViewDirty)
    {
        glm::mat4 translate = glm::translate(glm::mat4(1.0), -m_Position);
//        //Since we know the rotation matrix is orthonormalized , we can simply
//        // transpose the rotation matrix instead of inversing
        glm::mat4 rotate = glm::transpose(glm::toMat4(m_Rotation));
//        glm::mat4 View = glm::lookAt(
//            glm::vec3(3,0,0), // Камера находится в мировых координатах (4,3,3) z,y
//            glm::vec3(0,0,0), // И направлена в начало координат
//            glm::vec3(0,1,0)  // "Голова" находится сверху
//        );
        m_ViewMatrix = rotate * translate;
        //m_ViewMatrix =  View;
        m_ViewDirty = false;
    }
}

