#ifndef CAMERA_H
#define CAMERA_H

#include "Math/math.h"
#include "Core/mouseinput.h"
#include <functional>

#define MAX_FOV 45.0f
#define MIN_FOV 1.0f

namespace ningen
{
    class Camera
    {
        private:
            float m_FoV = 45.0f;
            float m_Aspect = 16.0f/9.0f;
            float m_Near = 0.1f;
            float m_Far = 300.0f;

            float m_ZoomSpeed = 1.0f;

            Vec3 m_Position = Vec3(0.0f, 0.0f, 1.0f);

            Mat4 m_ProjectionMatrix = Mat4(1.0f);
            Mat4 m_ViewMatrix = Mat4(1.0f);
            Mat4 m_ViewProjectionMatrix = Mat4(1.0f);
        public:
            Camera(void) = delete;
            Camera(float fov, float aspect, float near, float far);

            void changeFoV(float offset);
            void recalculateProjectionMatrix(void);
            void recalculateViewMatrix(void);
            inline const Vec3& getPosition() const { return m_Position; }
            inline const Mat4& getProjectionMatrix() const { return m_ProjectionMatrix; }
            inline const Mat4& getViewMatrix() const { return m_ViewMatrix; }
            inline const Mat4& getViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
    };

}

#endif