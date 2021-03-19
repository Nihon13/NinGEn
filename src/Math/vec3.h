#ifndef VEC3_H
#define VEC3_H

#include <iostream>

namespace ningen {

    class Vec3
    {
        public:
            float x, y, z;

            Vec3(void);
            Vec3(const float x, const float y, const float z);

            friend Vec3 operator+(Vec3 left, const Vec3& right);
            friend Vec3 operator-(Vec3 left, const Vec3& right);
            friend Vec3 operator*(Vec3 left, const Vec3& right);
            friend Vec3 operator/(Vec3 left, const Vec3& right);

            Vec3& operator+=(const Vec3& other);
            Vec3& operator-=(const Vec3& other);
            Vec3& operator*=(const Vec3& other);
            Vec3& operator/=(const Vec3& other);

            bool operator==(const Vec3& other) const;
            bool operator!=(const Vec3& other) const;
        
            friend std::ostream& operator<<(std::ostream& out, const Vec3& vec);
        private:
            Vec3& add(const Vec3& other);
            Vec3& sub(const Vec3& other);
            Vec3& mul(const Vec3& other);
            Vec3& div(const Vec3& other);
    };

}

#endif