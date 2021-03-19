#ifndef VEC2_H
#define VEC2_H

#include <iostream>

namespace ningen {

    class Vec2
    {
        public:
            float x, y;

            Vec2(void);
            Vec2(const float x, const float y);

            friend Vec2 operator+(Vec2 left, const Vec2& right);
            friend Vec2 operator-(Vec2 left, const Vec2& right);
            friend Vec2 operator*(Vec2 left, const Vec2& right);
            friend Vec2 operator/(Vec2 left, const Vec2& right);

            Vec2& operator+=(const Vec2& other);
            Vec2& operator-=(const Vec2& other);
            Vec2& operator*=(const Vec2& other);
            Vec2& operator/=(const Vec2& other);

            bool operator==(const Vec2& other) const;
            bool operator!=(const Vec2& other) const;
        
            friend std::ostream& operator<<(std::ostream& out, const Vec2& vec);
        private:
            Vec2& add(const Vec2& other);
            Vec2& sub(const Vec2& other);
            Vec2& mul(const Vec2& other);
            Vec2& div(const Vec2& other);
    };

}

#endif