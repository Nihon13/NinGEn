#include "vec2.h"

namespace ningen {

    Vec2::Vec2(void)
    {
        this->x = 0;
        this->y = 0;
    }

    Vec2::Vec2(const float x, const float y)
    {
        this->x = x;
        this->y = y;
    }

    Vec2& Vec2::add(const Vec2& other)
    {
        this->x += other.x;
        this->y += other.y;

        return *this;
    }

    Vec2& Vec2::sub(const Vec2& other)
    {
        this->x -= other.x;
        this->y -= other.y;

        return *this;
    }

    Vec2& Vec2::mul(const Vec2& other)
    {
        this->x *= other.x;
        this->y *= other.y;

        return *this;
    }

    Vec2& Vec2::div(const Vec2& other)
    {
        this->x /= other.x;
        this->y /= other.y;

        return *this;
    }

    Vec2 operator+(Vec2 left, const Vec2& right)
    {
        return left.add(right);
    }

    Vec2 operator-(Vec2 left, const Vec2& right)
    {
        return left.sub(right);
    }

    Vec2 operator*(Vec2 left, const Vec2& right)
    {
        return left.mul(right);
    }

    Vec2 operator/(Vec2 left, const Vec2& right)
    {
        return left.div(right);
    }

    Vec2& Vec2::operator+=(const Vec2& other)
    {
        return add(other);
    }

    Vec2& Vec2::operator-=(const Vec2& other)
    {
        return sub(other);
    }

    Vec2& Vec2::operator*=(const Vec2& other)
    {
        return mul(other);
    }

    Vec2& Vec2::operator/=(const Vec2& other)
    {
        return div(other);
    }

    bool Vec2::operator==(const Vec2& other) const
    {
        return ( (this->x == other.x) && (this->y == other.y) );
    }

    bool Vec2::operator!=(const Vec2& other) const
    {
        return ( (this->x != other.x) && (this->y != other.y) );
    }

    std::ostream& operator<<(std::ostream& out, const Vec2& vector)
    {
        out << "vec2(" << vector.x << ", " << vector.y << ")";
        return out;
    }
    
}