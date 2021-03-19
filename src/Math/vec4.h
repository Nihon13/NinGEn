#ifndef VEC4_H
#define VEC4_H

#include <iostream>

namespace ningen {

    class Vec4
    {
        public:
            float x, y, z, w;

            Vec4(void);
            Vec4(const float x, const float y, const float z, const float w);

            friend Vec4 operator+(Vec4 left, const Vec4& right);
            friend Vec4 operator-(Vec4 left, const Vec4& right);
            friend Vec4 operator*(Vec4 left, const Vec4& right);
            friend Vec4 operator/(Vec4 left, const Vec4& right);

            Vec4& operator+=(const Vec4& other);
            Vec4& operator-=(const Vec4& other);
            Vec4& operator*=(const Vec4& other);
            Vec4& operator/=(const Vec4& other);

            bool operator==(const Vec4& other) const;
            bool operator!=(const Vec4& other) const;
        
            friend std::ostream& operator<<(std::ostream& out, const Vec4& vec);
        private:
            Vec4& add(const Vec4& other);
            Vec4& sub(const Vec4& other);
            Vec4& mul(const Vec4& other);
            Vec4& div(const Vec4& other);
    };

}

#endif