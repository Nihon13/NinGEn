#ifndef MATH_H
#define MATH_H

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"

#include <cmath>

#define MATH_PI 3.1415926535897f

namespace ningen {

    inline float toRadians(float degrees)
    {
        return (float)(degrees * (MATH_PI / 180.0f));
    }

    inline float toDegrees(float radians)
    {
        return (float)(radians * (180.0f / MATH_PI));
    }

}

#endif