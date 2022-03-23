#include "camera.h"

namespace ningen {

    Camera::Camera(float fov, float aspect, float near, float far)
        : m_DistanceFromTarget(fov), m_Aspect(aspect), m_Near(near), m_Far(far)
    {
        m_ProjectionMatrix = glm::perspective(glm::radians(m_DistanceFromTarget), m_Aspect, m_Near, m_Far);

        MouseInput::getInstance().addScrollBinding(std::bind(&Camera::changeFoV, this, std::placeholders::_1));
        MouseInput::getInstance().addRightButtonBinding(std::bind(&Camera::changeAngleAroundTarget, this, std::placeholders::_1));
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
        m_AngleAroundTarget -= angle * m_HorizontalSpeed;
        m_AngleAroundTarget = fmod(m_AngleAroundTarget, 360.0f);
        m_Yaw = -m_AngleAroundTarget;
 
        recalculateViewMatrix();
    }

    void Camera::recalculateProjectionMatrix(void)
    {
        m_ProjectionMatrix = glm::perspective(glm::radians(m_DistanceFromTarget), m_Aspect, m_Near, m_Far);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void Camera::recalculateViewMatrix(void)
    {
        m_CameraPosition.x = m_TargetPosition.x + sin(glm::radians(m_AngleAroundTarget)) * m_DistanceFromTarget;
        m_CameraPosition.z = m_TargetPosition.z + cos(glm::radians(m_AngleAroundTarget)) * m_DistanceFromTarget;

        m_ViewMatrix = Mat4(1.0f);
        m_ViewMatrix = glm::rotate(m_ViewMatrix, glm::radians(m_Yaw), Vec3(0.0f, 1.0f, 0.0f));
        m_ViewMatrix = glm::translate(m_ViewMatrix, -m_CameraPosition);

        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

}