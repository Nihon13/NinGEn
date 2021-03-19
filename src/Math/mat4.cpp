#include "mat4.h"

namespace ningen {

    Mat4::Mat4(void)
    {
        memset(elements, 0, 4 * 4 * sizeof(float));
    }

    Mat4::Mat4(float diagonal)
    {
        memset(elements, 0, 4 * 4 * sizeof(float));
        elements[0 + 0 * 4] = diagonal;
        elements[1 + 1 * 4] = diagonal;
        elements[2 + 2 * 4] = diagonal;
        elements[3 + 3 * 4] = diagonal;
    }

    Mat4 Mat4::identity(void)
    {
        return Mat4(1.0f);
    }

    Mat4& Mat4::multiply(const Mat4& other)
    {
        float buffer[16];
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                float sum = 0.0f;
                for (int e = 0; e < 4; e++)
                {
                    sum += elements[e + row * 4] * other.elements[col + e * 4];
                }
                buffer[col + row * 4] = sum;
            }
        }
        memcpy(elements, buffer, 4 * 4 * sizeof(float));
        return *this;
    }

    Mat4 operator*(Mat4 left, const Mat4& right)
    {
        return left.multiply(right);
    }

    Mat4& Mat4::operator*=(const Mat4& other)
    {
        return multiply(other);
    }

    Mat4 Mat4::orthographic(float left, float right, float bottom, float top, float near, float far)
    {
        Mat4 result;

        result.elements[0 + 0 * 4] = 2.0f / (right - left);
        result.elements[1 + 1 * 4] = 2.0f / (top - bottom);
        result.elements[2 + 2 * 4] = 2.0f / (near - far);
        result.elements[3 + 3 * 4] = 1.0f;
  
        result.elements[3 + 0 * 4] = - (right + left) / (right - left);
        result.elements[3 + 1 * 4] = - (top + bottom) / (top - bottom); 
        result.elements[3 + 2 * 4] = - (far + near) / (far - near);
        result.elements[3 + 3 * 4] = 1;

        return result;
    }

    Mat4 Mat4::perspective(float fov, float aspect, float near, float far)
    {
        Mat4 result;

        float a = 1.0f / tan(toRadians(fov * 0.5f));

        result.elements[0 + 0 * 4] = a / aspect;
        result.elements[1 + 1 * 4] = a;
        result.elements[2 + 2 * 4] = - (far + near) / (far - near);
        result.elements[3 + 2 * 4] = - (2.0f * far * near) / (far - near);
        result.elements[2 + 3 * 4] = -1.0f;

        return result;
    }

    Mat4 Mat4::translate(const Vec3& translation)
    {
        Mat4 result(1.0f);

        result.elements[3 + 0 * 4] = translation.x;
        result.elements[3 + 1 * 4] = translation.y;
        result.elements[3 + 2 * 4] = translation.z;

        return result;
    }

    Mat4 Mat4::rotate(float angle, const Vec3& axis)
    {
        Mat4 result(1.0f);

        float r = toRadians(angle);
        float s = sin(r);
        float c = cos(r);

        float x = axis.x;
        float y = axis.y;
        float z = axis.z;

        float q = (1.0f - c);

        result.elements[0 + 0 * 4] = x * x * q + c;
        result.elements[1 + 0 * 4] = x * y * q + z * s;
        result.elements[2 + 0 * 4] = x * z * q - y * s;

        result.elements[0 + 1 * 4] = x * y * q - z * s;
        result.elements[1 + 1 * 4] = y * y * q + c;
        result.elements[2 + 1 * 4] = y * z * q + x * s;

        result.elements[0 + 2 * 4] = x * z * q + y * s;
        result.elements[1 + 2 * 4] = y * z * q - x * s;
        result.elements[2 + 2 * 4] = z * z * q + c;

        return result;
    }

    Mat4 Mat4::scale(const Vec3& scale)
    {
        Mat4 result;

        result.elements[0 + 0 * 4] = scale.x;
        result.elements[1 + 1 * 4] = scale.y;
        result.elements[2 + 2 * 4] = scale.z;
        result.elements[3 + 3 * 4] = 1.0f;

        return result;
    }

    Mat4 Mat4::transpose(const Mat4& matrix)
    {
        Mat4 result;

        result.elements[0 + 0 * 4] = matrix.elements[0 + 0 * 4];
        result.elements[1 + 0 * 4] = matrix.elements[0 + 1 * 4];
        result.elements[2 + 0 * 4] = matrix.elements[0 + 2 * 4];
        result.elements[3 + 0 * 4] = matrix.elements[0 + 3 * 4];

        result.elements[0 + 1 * 4] = matrix.elements[1 + 0 * 4];
        result.elements[1 + 1 * 4] = matrix.elements[1 + 1 * 4];
        result.elements[2 + 1 * 4] = matrix.elements[1 + 2 * 4];
        result.elements[3 + 1 * 4] = matrix.elements[1 + 3 * 4];

        result.elements[0 + 2 * 4] = matrix.elements[2 + 0 * 4];
        result.elements[1 + 2 * 4] = matrix.elements[2 + 1 * 4];
        result.elements[2 + 2 * 4] = matrix.elements[2 + 2 * 4];
        result.elements[3 + 2 * 4] = matrix.elements[2 + 3 * 4];

        result.elements[0 + 3 * 4] = matrix.elements[3 + 0 * 4];
        result.elements[1 + 3 * 4] = matrix.elements[3 + 1 * 4];
        result.elements[2 + 3 * 4] = matrix.elements[3 + 2 * 4];
        result.elements[3 + 3 * 4] = matrix.elements[3 + 3 * 4];

        return result;
    }

    std::ostream& operator<<(std::ostream& out, const Mat4& matrix)
    {
        out << matrix.elements[0] << " | " << matrix.elements[1] << " | " << matrix.elements[2] << " | " <<matrix.elements[3] << "\n"
            <<  matrix.elements[4] << " | " << matrix.elements[5] << " | " << matrix.elements[6] << " | " << matrix.elements[7] << "\n"
            <<  matrix.elements[8] << " | " << matrix.elements[9] << " | " << matrix.elements[10] << " | " << matrix.elements[11] << "\n"
            <<  matrix.elements[12] << " | " << matrix.elements[13] << " | " << matrix.elements[14] << " | " << matrix.elements[15];

        return out;
    }
}