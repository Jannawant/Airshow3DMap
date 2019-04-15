#include "movementcontroller.h"
#include "AppCode/mainapp.h"

#define MAX(a,b) ((a) >= (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

MovementController::MovementController(shared_ptr<SceneNode> object, float initialYaw, float initialPitch, bool rotateWhenLButtonDown)
{
    m_object->vGet()->transform(&m_matToWorld, &m_matFromWorld);
    m_fTargetYaw = m_fYaw = RADIANS_TO_DEGREES(-initialYaw);
    m_fTargetPitch = m_fPitch = RADIANS_TO_DEGREES(initialPitch);
    m_maxSpeed = 30.0f; //30 meters per second
    m_currentSpeed = 0.0f;
    Vec3 pos = m_matToWorld.getPosition();
    m_matPosition.buildTranslation(pos);
    Point ptCursor;
    g_pApp->getScreen()->getCursor(ptCursor);
    m_lastMousePos = ptCursor;
    memset(m_bKey, 0x00, sizeof(m_bKey));
    m_mouseLButtonDown = false;
    m_bRotateWhenLButtonDown = rotateWhenLButtonDown;
}

void MovementController::onUpdate(DWORD const deltaMilliseconds)
{
    //if (m_bKey['Q'])
    //{
    //	// This code is a cheat to position the camera exactly in a given
    //	// spot so I can take screen shots!

    //	Mat4x4 camTranslate;
    //	D3DXMatrixTranslation(&m_matPosition, 8.847f, 7.055f, 11.618f);

    //	m_fTargetYaw = m_fYaw += -64.35f;
    //	m_fTargetPitch = m_fPitch = 28.57f;

    //	// Calculate the new rotation matrix from the camera
    //	// yaw and pitch.
    //	Mat4x4 matRot;
    //	D3DXMatrixRotationYawPitchRoll(&matRot, DEGREES_TO_RADIANS(m_fYaw), DEGREES_TO_RADIANS(m_fPitch), 0);

    //	// Create the new object-to-world matrix, and the
    //	// new world-to-object matrix.
    //	D3DXMatrixMultiply(&m_matToWorld, &matRot, &m_matPosition);
    //	D3DXMatrixInverse(&m_matFromWorld, NULL, &m_matToWorld);

    //	m_object->VSetTransform(&m_matToWorld, &m_matFromWorld);
    //	return;
    //}

    bool bTranslating = false;
    Vec4 atWorld(0,0,0,0);
    Vec4 rightWorld(0,0,0,0);
    Vec4 upWorld(0,0,0,0);

    if (m_bKey['W'] || m_bKey['S'])
    {
        // In D3D, the "look at" default is always
        // the positive Z axis.
        Vec4 at = g_Forward4;
        if (m_bKey['S'])
            at *= -1;

        // This will give us the "look at" vector
        // in world space - we'll use that to move
        // the camera.
        atWorld = m_matToWorld.xForm(at);
        bTranslating = true;
    }

    if (m_bKey['A'] || m_bKey['D'])
    {
        // In D3D, the "right" default is always
        // the positive X axis.
        Vec4 right = g_Right4;
        if (m_bKey['A'])
            right *= -1;

        // This will give us the "right" vector
        // in world space - we'll use that to move
        // the camera
        rightWorld = m_matToWorld.xForm(right);
        bTranslating = true;
    }

    if (m_bKey[' '] || m_bKey['C'] || m_bKey['X'])
    {
        // In D3D, the "up" default is always
        // the positive Y axis.
        Vec4 up = g_Right4;
        if (!m_bKey[' '])
            up *= -1;

        //Unlike strafing, Up is always up no matter
        //which way you are looking
        upWorld = up;
        bTranslating = true;
    }

    //Handling rotation as a result of mouse position
    {
        // The secret formula!!! Don't give it away!
        //If you are seeing this now, then you must be some kind of elite hacker!
        m_fYaw += (m_fTargetYaw - m_fYaw) * ( .35f );
        m_fTargetPitch = MAX(-90, MIN(90, m_fTargetPitch));
        m_fPitch += (m_fTargetPitch - m_fPitch) * ( .35f );

        // Calculate the new rotation matrix from the camera
        // yaw and pitch.
        Mat4x4 matRot;
        matRot.buildYawPitchRoll(DEGREES_TO_RADIANS(-m_fYaw), DEGREES_TO_RADIANS(m_fPitch), 0);

        // Create the new object-to-world matrix, and the
        // new world-to-object matrix.

        m_matToWorld = matRot * m_matPosition;
        m_matFromWorld = m_matToWorld.inverse();
        m_object->vSetTransform(&m_matToWorld, &m_matFromWorld);
    }

    if (bTranslating)
    {
        float elapsedTime = (float)deltaMilliseconds / 1000.0f;

        Vec3 direction = Vec3(atWorld + rightWorld + upWorld);
        direction.normalize();

        // Ramp the acceleration by the elapsed time.
        float numberOfSeconds = 5.f;
        m_currentSpeed += m_maxSpeed * ( (elapsedTime*elapsedTime) / numberOfSeconds);
        if (m_currentSpeed > m_maxSpeed)
            m_currentSpeed = m_maxSpeed;

        direction *= m_currentSpeed;

        Vec3 pos = m_matPosition.getPosition() + direction;
        m_matPosition.setPosition(pos);
        m_matToWorld.setPosition(pos);

        m_matFromWorld = m_matToWorld.inverse();
        m_object->vSetTransform(&m_matToWorld, &m_matFromWorld);
    }
    else
    {
        m_currentSpeed = 0.0f;
    }
}

bool MovementController::vOnPointerMove(const Point &mousePos, const int radius)
{
    if(m_bRotateWhenLButtonDown)
    {
        if(m_lastMousePos != mousePos && m_mouseLButtonDown)
        {
            m_fTargetYaw = m_fTargetYaw + (m_lastMousePos.x - mousePos.x);
            m_fTargetPitch = m_fTargetPitch + (mousePos.y - m_lastMousePos.y);
            m_lastMousePos = mousePos;
        }
    }
    else if(m_lastMousePos!= mousePos)
    {
        m_fTargetYaw = m_fTargetYaw + (m_lastMousePos.x - mousePos.x);
        m_fTargetPitch = m_fTargetPitch + (mousePos.y - m_lastMousePos.y);
        m_lastMousePos = mousePos;
    }
    return true;
}

bool MovementController::vOnPointerButtonDown(const Point &mousePos, const int radius, const string &buttonName)
{
    if(buttonName == "PointerLeft")
    {
        m_mouseLButtonDown = true;
        m_lastMousePos = mousePos;
        return true;
    }
    return false;
}

bool MovementController::vOnPointerButtonUp(const Point &mousePos, const int radius, const string &buttonName)
{
    if(buttonName == "PointerLeft")
    {
        m_mouseLButtonDown = false;
        return true;
    }
    return false;
}



































