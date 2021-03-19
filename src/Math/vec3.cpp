#include "vec3.h"

namespace ningen {

    Vec3::Vec3(void)
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }

    Vec3::Vec3(const float x, const float y, const float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vec3& Vec3::add(const Vec3& other)
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;

        return *this;
    }

    Vec3& Vec3::sub(const Vec3& other)
    {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;

        return *this;
    }

    Vec3& Vec3::mul(const Vec3& other)
    {
        this->x *= other.x;
        this->y *= other.y;
        this->z *= other.z;

        return *this;
    }

    Vec3& Vec3::div(const Vec3& other)
    {
        this->x /= other.x;
        this->y /= other.y;
        this->z /= other.z;

        return *this;
    }

    Vec3 operator+(Vec3 left, const Vec3& right)
    {
        return left.add(right);
    }

    Vec3 operator-(Vec3 left, const Vec3& right)
    {
        return left.sub(right);
    }

    Vec3 operator*(Vec3 left, const Vec3& right)
    {
        return left.mul(right);
    }

    Vec3 operator/(Vec3 left, const Vec3& right)
    {
        return left.div(right);
    }

    Vec3& Vec3::operator+=(const Vec3& other)
    {
        return add(other);
    }

    Vec3& Vec3::operator-=(const Vec3& other)
    {
        return sub(other);
    }

    Vec3& Vec3::operator*=(const Vec3& other)
    {
        return mul(other);
    }

    Vec3& Vec3::operator/=(const Vec3& other)
    {
        return div(other);
    }

    bool Vec3::operator==(const Vec3& other) const
    {
        return ( (this->x == other.x) && (this->y == other.y) && (this->z == other.z) );
    }

    bool Vec3::operator!=(const Vec3& other) const
    {
        return ( (this->x != other.x) && (this->y != other.y) && (this->z != other.z) );
    }

    std::ostream& operator<<(std::ostream& out, const Vec3& vector)
    {
        out << "vec3(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
        return out;
    }
    
}