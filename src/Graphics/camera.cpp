#include "camera.h"

namespace ningen {

    Camera::Camera(float fov, float aspect, float near, float far)
        : m_DistanceFromTarget(fov), m_Aspect(aspect), m_Near(near), m_Far(far)
    {
        m_ProjectionMatrix = glm::perspective(glm::radians(m_DistanceFromTarget), m_Aspect, m_Near, m_Far);

        MouseInput::getInstance().addScrollBinding(std::bind(&Camera::changeFoV, this, std::placeholders::_1));
        MouseInput::getInstance().addRightButtonBinding(MouseOffsets::offset_X, std::bind(&Camera::changeAngleAroundTarget, this, std::placeholders::_1));
        MouseInput::getInstance().addRightButtonBindingWithKeyModifier(GLFW_MOD_CONTROL, MouseOffsets::offset_Y, std::bind(&Camera::changePitch, this, std::placeholders::_1));
    }

    void Camera::changeFoV(float offset)
    {
        m_DistanceFromTarget -= offset * m_ZoomSpeed;
        if (m_DistanceFromTarget < MIN_FOV)
        {
            m_DistanceFromTarget = MIN_FOV;
        }
        else if (m_DistanceFromTarget > MAX_FOV)
        {
            m_DistanceFromTarget = MAX_FOV;
        }

        recalculateProjectionMatrix();
        recalculateViewMatrix();
    }

    void Camera::changeAngleAroundTarget(float angle)
    {
        m_AngleAroundTarget -= angle * m_Sensitivity;
        m_AngleAroundTarget = fmod(m_AngleAroundTarget, 360.0f);
 
        recalculateViewMatrix();
    }

    void Camera::changePitch(float pitch)
    {
        m_Pitch -= pitch * m_Sensitivity;
        
        if (m_Pitch < -89.0f)
        {
            m_Pitch = -89.0f;
        }
        if (m_Pitch > 89.0f)
        {
            m_Pitch = 89.0f;
        }

        recalculateViewMatrix();
    }

    void Camera::calculateCameraPosition(void)
    {
        float horizontalDist = m_DistanceFromTarget * cos(glm::radians(m_Pitch));
        float verticalDist = m_DistanceFromTarget * sin(glm::radians(m_Pitch));
        float offsetX = horizontalDist * sin(glm::radians(m_AngleAroundTarget));
        float offsetZ = horizontalDist * cos(glm::radians(m_AngleAroundTarget));
        m_CameraPosition.y = m_TargetPosition.y + verticalDist;
        m_CameraPosition.x = m_TargetPosition.x - offsetX;
        m_CameraPosition.z = m_TargetPosition.z - offsetZ;
        m_Yaw = 180.0f - m_AngleAroundTarget;
    }

    void Camera::recalculateProjectionMatrix(void)
    {
        m_ProjectionMatrix = glm::perspective(glm::radians(m_DistanceFromTarget), m_Aspect, m_Near, m_Far);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void Camera::recalculateViewMatrix(void)
    {
        calculateCameraPosition();

        m_ViewMatrix = Mat4(1.0f);
        m_ViewMatrix = glm::rotate(m_ViewMatrix, glm::radians(m_Pitch), Vec3(1.0f, 0.0f, 0.0f));
        m_ViewMatrix = glm::rotate(m_ViewMatrix, glm::radians(m_Yaw), Vec3(0.0f, 1.0f, 0.0f));
        m_ViewMatrix = glm::translate(m_ViewMatrix, -m_CameraPosition);

        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

}