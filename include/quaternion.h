#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "vector3.h"
#include "matrix4.h"
#include <iostream>

namespace mvp
{
    class quaternion
    {
    public:
        // CONSTRUCT BASE QUATERNION
        quaternion() : q_x(0.0), q_y(0.0), q_z(0.0), q_w(1.0) {}
        // BUILD MORE COMPLEX QUATERNIONS
        quaternion(const float& x, const float& y, const float& z, const float& w) : q_x(x), q_y(y), q_z(z), q_w(w) {}
        quaternion(const float& x, const float& y, const float& z) : q_x(x), q_y(y), q_z(z), q_w(1.0) {}
        quaternion(const vector3<float>& v) : q_x(v[0]), q_y(v[1]), q_z(v[2]), q_w(1.0) {}
        quaternion(const matrix4<float>& m) { *this = m; }
        // COPY CONSTRUCTION
        quaternion(const quaternion& q)
        {
            q_x = q.q_x;
            q_y = q.q_y;
            q_z = q.q_z;
            q_w = q.q_w;
        }

        // PRINT (FOR DEBUG AND DISPLAY)
        void print(std::ostream& out) const
        {
            // MODIFY PRECISION
            std::streamsize tp = out.precision(5);
            out << std::fixed << '(' << q_x << ", " << q_y << ", " << q_z << ", " << q_w << ')';
            // RESTORE PRECISION
            out.precision(tp);
        }

        // ACCESSORS
        float x() const { return q_x; }
        float y() const { return q_y; }
        float z() const { return q_z; }
        float w() const { return q_w; }

        // RETURN THE CONJUGATE
        quaternion conj() const
        {
            quaternion c(q_x,q_y,q_z,q_w);
            c.q_x *= -1;
            c.q_y *= -1;
            c.q_z *= -1;
            return c;
        }

        // ASSIGNMENT: VECTOR3<FLOAT> -> QUATERNION
        quaternion& operator=(const vector3<float>& v)
        {
            q_x = v[0];
            q_y = v[1];
            q_z = v[2];
            q_w = 1.0;
            return *this;
        }
        // ASSIGNMENT: QUATERNION -> QUATERNION
        quaternion& operator=(const quaternion& q)
        {
            q_x = q.q_x;
            q_y = q.q_y;
            q_z = q.q_z;
            q_w = q.q_w;
            return *this;
        }
        // GENERATE QUATERNION FROM MATRX4<FLOAT>
        quaternion& operator=(const matrix4<float>& m)
        {
            float diag = m.at(0,0) + m.at(1,1) + m.at(2,2);

            if(diag > 0.0)
            {
                float scale = sqrt(diag + 1.0);
                q_w = scale / 2.0;
                scale = 0.5 / scale;
                q_x = (m.at(2,1) - m.at(1,2)) * scale;
                q_y = (m.at(0,2) - m.at(2,0)) * scale;
                q_z = (m.at(1,0) - m.at(0,1)) * scale;
                return *this;
            }
            else
            {
                size_t i = 0;
                size_t nxt[3] = {1, 2, 0};
                float q[4] = {0.0, 0.0, 0.0, 0.0};
                if(m.at(1,1) > m.at(0,0))
                    i = 1;
                if(m.at(2,2) > m.at(i,i))
                    i = 2;
                size_t j = nxt[i];
                size_t k = nxt[j];
                float s = sqrt(m.at(i,i) - m.at(j,j) + m.at(k,k) + 1);
                q[i] = s / 2.0;
                if(s != 0)
                    s = 0.5 / s;
                q_w = (m.at(j,k) - m.at(k,j)) * s;
                q[j] = (m.at(i,j) + m.at(j,i)) * s;
                q[k] = (m.at(i,k) + m.at(k,i)) * s;
                q_x = q[0];
                q_y = q[1];
                q_z = q[2];
                return *this;
            }
        }

        // SET VALUES
        void set(const float& x, const float& y, const float& z, const float& w)
        {
            q_x = x;
            q_y = y;
            q_z = z;
            q_w = w;
        }
        // SET VALUES FROM VECTOR3<FLOAT>
        void set(const vector3<float>& v)
        {
            q_x = v[0];
            q_y = v[1];
            q_z = v[2];
            q_w = 1.0;
        }
        // SET VALUES FROM MATRIX4<FLOAT>
        void set(const matrix4<float>& m) { *this = m; }
        // SET VALUES FROM QUATERNION
        void set(const quaternion& q)
        {
            q_x = q.q_x;
            q_y = q.q_y;
            q_z = q.q_z;
            q_w = q.q_w;
        }

        // MULTIPLICATION ASSIGNMENT: QUATERNION
        quaternion& operator*=(const quaternion& q)
        {
            *this = *this * q;
            return *this;
        }
        // MULTIPLICATION ASSIGNMENT: SCALAR
        quaternion& operator*=(const float& s)
        {
            *this = *this * s;
            return *this;
        }
        // MULTIPLICATION: QUATERNION
        quaternion operator*(const quaternion& q) const
        {
            quaternion tmp;
            tmp.q_w = (q.q_w * q_w) - (q.q_x * q_x) - (q.q_y * q_y) - (q.q_z * q_z);
            tmp.q_x = (q.q_w * q_x) + (q.q_x * q_w) + (q.q_z * q_y) - (q.q_y * q_z);
            tmp.q_y = (q.q_w * q_y) + (q.q_y * q_w) + (q.q_x * q_z) - (q.q_z * q_x);
            tmp.q_z = (q.q_w * q_z) + (q.q_z * q_w) + (q.q_y * q_x) - (q.q_x * q_y);
            return tmp;
        }
        // MULTIPLICATION: SCALAR
        quaternion operator*(const float& s) const 
        {
            return quaternion(s * q_x, s * q_y, s * q_z, s * q_w);
        }

        // ADDITION: QUATERNION
        quaternion operator+(const quaternion& q) const
        {
            return quaternion(q_x + q.q_x, q_y + q.q_y, q_z + q.q_z, q_w + q.q_w);
        }

        // QUATERNION DOT PRODUCT
        float dot(const quaternion& q) const
        {
            return (q_x * q.q_x) + (q_y * q.q_y) + (q_z * q.q_z) + (q_w * q.q_w);
        }

        // NORMALIZE THIS QUATERNION
        void normalize()
        {
            float n = q_x * q_x + q_y * q_y + q_z * q_z + q_w * q_w;
            if(n == 1) return;
            *this *= (1.0 / sqrt(n));
        }
        // RETURN NORMALIZED VERSION OF THIS QUATERNION
        quaternion normalized() const
        {
            quaternion tmp = *this;
            tmp.normalize();
            return tmp;
        }

        // CONVERT QUATERNION TO MATRIX
        matrix4<float> to_matrix() const
        {
            matrix4<float> m;
            m.at(0,0) = 1.0 - 2.0 * q_y * q_y - 2.0 * q_z * q_z;
            m.at(1,0) = 2.0 * q_x * q_y - 2.0 * q_z * q_w;
            m.at(2,0) = 2.0 * q_x * q_z + 2.0 * q_y * q_w;
            m.at(3,0) = 0.0;
            m.at(0,1) = 2.0* q_x * q_y + 2.0 * q_z * q_w;
            m.at(1,1) = 1.0 - 2.0 * q_x * q_x - 2.0 * q_z * q_z;
            m.at(2,1) = 2.0 * q_z * q_y - 2.0 * q_x * q_w;
            m.at(3,1) = 0.0;
            m.at(0,2) = 2.0 * q_x * q_z - 2.0 * q_y * q_w;
            m.at(1,2) = 2.0 * q_z * q_y + 2.0 * q_x * q_w;
            m.at(2,2) = 1.0 - 2.0 * q_x * q_x - 2.0 * q_y * q_y;
            m.at(3,2) = 0.0;
            m.at(0,3) = 0.0;
            m.at(1,3) = 0.0;
            m.at(2,3) = 0.0;
            m.at(3,3) = 1.0;

            return m;
        }

    private:
        float q_x;
        float q_y;
        float q_z;
        float q_w;
    };
}

#endif
