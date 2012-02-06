#ifndef _MVP_MATH_H_
#define _MVP_MATH_H_

#include <math.h>
#include "matrix4.h"
#include "quaternion.h"
#include "point.h"
#include "vector3.h"

namespace mvp
{
    // LINEAR INTERPOLATE TWO VECTOR3<FLOAT>
    vector3<float> lerp(const vector3<float>& v1, const vector3<float>& v2, const float& a)
    {
        return (v1 * (1.0 - a) + v2 * a);
    }

    // LINEAR INTERPOLATE TWO POINT<FLOAT>
    point<float> lerp(const point<float>& p1, const point<float>& p2, const float& a)
    {
        return (p1 * (1.0 - a) + p2 * a);
    }

    // SPHERICAL LINEAR INTERPOLATE TWO QUATERNIONS (RETURN AS MATRIX)
    matrix4<float> slerp(quaternion& q1, quaternion& q2, const float& a)
    {
        float angle = q1.dot(q2);

        if(angle < 0.0)
        {
            q1 *= -1.0;
            angle *= -1.0;
        }

        float theta = acos(angle);
        float wp = sin((1-a) * theta) / sin(theta);
        float wq = sin(a * theta) / sin(theta);

        return (q1 * wp + q2 * wq).to_matrix();
    }

    // SPHERICAL LINEAR INTERPOLATE TWO MATRICIES (RETURN AS MATRIX)
    matrix4<float> slerp(const matrix4<float> m1, const matrix4<float> m2, const float& a)
    {
        quaternion q1(m1);
        quaternion q2(m2);
        return slerp(q1,q2,a);
    }
}

#endif
