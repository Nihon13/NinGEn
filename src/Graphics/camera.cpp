#include "camera.h"

namespace ningen
{

    Camera::Camera(float fov, float aspect, float near, float far)
        : m_FoV(fov), m_Aspect(aspect), m_Near(near), m_Far(far)
    {
        m_ProjectionMatrix = glm::perspective(glm::radians(m_FoV), m_Aspect, m_Near, m_Far);

        MouseInput::getInstance().addScrollBinding(std::bind(&Camera::changeFoV, this, std::placeholders::_1));
    }

    void Camera::changeFoV(float offset)
    {
        m_FoV -= offset * m_ZoomSpeed;
        if (m_FoV < MIN_FOV)
        {
            m_FoV = MIN_FOV;
        }
        else if (m_FoV > MAX_FOV)
        {
            m_FoV = MAX_FOV;
        }

        recalculateProjectionMatrix();
    }

    void Camera::recalculateProjectionMatrix(void)
    {
        m_ProjectionMatrix = glm::perspective(glm::radians(m_FoV), m_Aspect, m_Near, m_Far);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void Camera::recalculateViewMatrix(void)
    {
        // m_ViewMatrix = ?
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

}