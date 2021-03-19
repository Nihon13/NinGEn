#include "vec4.h"

namespace ningen {

    Vec4::Vec4(void)
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
        this->w = 0;
    }

    Vec4::Vec4(const float x, const float y, const float z, const float w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    Vec4& Vec4::add(const Vec4& other)
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        this->w += other.w;

        return *this;
    }

    Vec4& Vec4::sub(const Vec4& other)
    {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        this->w -= other.w;

        return *this;
    }

    Vec4& Vec4::mul(const Vec4& other)
    {
        this->x *= other.x;
        this->y *= other.y;
        this->z *= other.z;
        this->w *= other.w;

        return *this;
    }

    Vec4& Vec4::div(const Vec4& other)
    {
        this->x /= other.x;
        this->y /= other.y;
        this->z /= other.z;
        this->w /= other.w;

        return *this;
    }

    Vec4 operator+(Vec4 left, const Vec4& right)
    {
        return left.add(right);
    }

    Vec4 operator-(Vec4 left, const Vec4& right)
    {
        return left.sub(right);
    }

    Vec4 operator*(Vec4 left, const Vec4& right)
    {
        return left.mul(right);
    }

    Vec4 operator/(Vec4 left, const Vec4& right)
    {
        return left.div(right);
    }

    Vec4& Vec4::operator+=(const Vec4& other)
    {
        return add(other);
    }

    Vec4& Vec4::operator-=(const Vec4& other)
    {
        return sub(other);
    }

    Vec4& Vec4::operator*=(const Vec4& other)
    {
        return mul(other);
    }

    Vec4& Vec4::operator/=(const Vec4& other)
    {
        return div(other);
    }

    bool Vec4::operator==(const Vec4& other) const
    {
        return ( (this->x == other.x) && (this->y == other.y) && (this->z == other.z) && (this->w == other.w) );
    }

    bool Vec4::operator!=(const Vec4& other) const
    {
        return ( (this->x != other.x) && (this->y != other.y) && (this->z != other.z) && (this->w != other.w) );
    }

    std::ostream& operator<<(std::ostream& out, const Vec4& vector)
    {
        out << "vec4(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";
        return out;
    }
    
}