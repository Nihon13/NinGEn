#ifndef MAT4_H
#define MAT4_H

#include "Math/math.h"
#include <cstring>

namespace ningen {

    class Mat4
    {
        public:
            union
            {
                float elements[4 * 4];
                Vec4 rows[4];
            };

            Mat4(void);
            Mat4(float diagonal);
            
            static Mat4 identity(void);

            Mat4& multiply(const Mat4& other);
            friend Mat4 operator*(Mat4 left, const Mat4& right);
            Mat4& operator*=(const Mat4& other);

            static Mat4 orthographic(float left, float right, float bottom, float top, float near, float far);
            static Mat4 perspective(float fov, float aspect, float near, float far);

            static Mat4 translate(const Vec3& translation);
            static Mat4 rotate(float angle, const Vec3& axis);
            static Mat4 scale(const Vec3& scale);

            static Mat4 transpose(const Mat4& matrix);

            friend std::ostream& operator<<(std::ostream& out, const Mat4& matrix);
    };

}

#endif

/*
Order of a matrix // column-major order //

0   4   8   12
1   5   9   13
2   6   10  14
3   7   11  15

*/