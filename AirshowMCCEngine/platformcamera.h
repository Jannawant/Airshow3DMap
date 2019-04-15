#ifndef PLATFORMCAMERA_H
#define PLATFORMCAMERA_H

#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Graphics3D/geometry.h"
#include "preamble.h"
#include "codestd.h"

class platformCamera
{
    platformCamera(){}
    glm::mat4x4 m_world;
public:
    static platformCamera * cameraInstance;
    static platformCamera * glCamera()
    {
        if(cameraInstance == nullptr)
            cameraInstance = new platformCamera();
        return cameraInstance;
    }
    void setTransform(const Mat4x4 &mat)
    {
        m_world = mat.getMat4x4();
    }
    glm::mat4x4 view() const
    {
        return m_world;
    }
};

#endif // PLATFORMCAMERA_H
