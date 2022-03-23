#ifndef CAMERA_H
#define CAMERA_H

#include "Math/math.h"
#include "Core/mouseinput.h"
#include <functional>

#define MAX_FOV 90.0f
#define MIN_FOV 1.0f

namespace ningen {

    class Camera
    {
        private:
            float m_Aspect = 16.0f/9.0f;
            float m_Near = 0.1f;
            float m_Far = 300.0f;

            Vec3 m_CameraPosition = Vec3(0.0f, 0.0f, 50.0f);
            Vec3 m_TargetPosition = Vec3(0.0f, 0.0f, 0.0f);

            float m_DistanceFromTarget = 50.0f;
            float m_AngleAroundTarget = 0.0f;

            float m_Pitch = 0.0f;
            float m_Yaw = 0.0f;
            float m_Roll = 0.0f;
            
            const float m_ZoomSpeed = 1.0f;
            const float m_HorizontalSpeed = 0.15f;

            Mat4 m_ProjectionMatrix = Mat4(1.0f);
            Mat4 m_ViewMatrix = Mat4(1.0f);
            Mat4 m_ViewProjectionMatrix = Mat4(1.0f);
        public:
            Camera(void) = delete;
            Camera(float fov, float aspect, float near, float far);

            void changeFoV(float offset);
            void changeAngleAroundTarget(float angle);
            void recalculateProjectionMatrix(void);
            void recalculateViewMatrix(void);
            inline const Vec3& getCameraPosition() const { return m_CameraPosition; }
            inline const Mat4& getProjectionMatrix() const { return m_ProjectionMatrix; }
            inline const Mat4& getViewMatrix() const { return m_ViewMatrix; }
            inline const Mat4& getViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
    };

}

#endif