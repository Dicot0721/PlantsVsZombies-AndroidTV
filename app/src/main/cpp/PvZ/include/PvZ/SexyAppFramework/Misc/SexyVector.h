#ifndef PVZ_SEXYAPPFRAMEWORK_MISC_SEXY_VECTOR_H
#define PVZ_SEXYAPPFRAMEWORK_MISC_SEXY_VECTOR_H

#include <math.h>

namespace Sexy {
class SexyVector2 {
public:
    float x, y;

    SexyVector2()
        : x(0)
        , y(0) {}
    SexyVector2(float theX, float theY)
        : x(theX)
        , y(theY) {}

    float Dot(const SexyVector2 &v) const {
        return x * v.x + y * v.y;
    }
    SexyVector2 operator+(const SexyVector2 &v) const {
        return SexyVector2(x + v.x, y + v.y);
    }
    SexyVector2 operator-(const SexyVector2 &v) const {
        return SexyVector2(x - v.x, y - v.y);
    }
    SexyVector2 operator-() const {
        return SexyVector2(-x, -y);
    }
    SexyVector2 operator*(float t) const {
        return SexyVector2(t * x, t * y);
    }
    SexyVector2 operator/(float t) const {
        return SexyVector2(x / t, y / t);
    }
    void operator+=(const SexyVector2 &v) {
        x += v.x;
        y += v.y;
    }
    void operator-=(const SexyVector2 &v) {
        x -= v.x;
        y -= v.y;
    }
    void operator*=(float t) {
        x *= t;
        y *= t;
    }
    void operator/=(float t) {
        x /= t;
        y /= t;
    }

    bool operator==(const SexyVector2 &v) {
        return x == v.x && y == v.y;
    }
    bool operator!=(const SexyVector2 &v) {
        return x != v.x || y != v.y;
    }

    float Magnitude() const {
        return sqrtf(x * x + y * y);
    }
    float MagnitudeSquared() const {
        return x * x + y * y;
    }
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_MISC_SEXY_VECTOR_H
