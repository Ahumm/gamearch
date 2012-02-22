#ifndef _VEC3_H_
#define _VEC3_H_

#include <iostream>
#include <math.h>

namespace mvp
{
    class vec3
    {
    public:
        //////////////////
        // CONSTRUCTORS //
        //////////////////

        vec3()
        {
            v_data[0] = v_data[1] = v_data[2] = 0;
        }

        vec3(const float& x, const float& y, const float& z)
        {
            v_data[0] = x;
            v_data[1] = y;
            v_data[2] = z;
        }

        vec3(const float& n)
        {
            v_data[0] = v_data[1] = v_data[2] = n;
        }

        vec3(const vec3& v)
        {
            v_data[0] = v.v_data[0];
            v_data[1] = v.v_data[1];
            v_data[2] = v.v_data[2];
        }

        ///////////
        // PRINT //
        ///////////

        // PRINT ALL ELEMENTS TO OUT
        void print(std::ostream& out) const
        {
            // MODIFY PRECISION
            std::streamsize tp = out.precision(5);
            out << '(' << v_data[0] << ", " << v_data[1] << ", " << v_data[2] << ')' << std::endl;
            // RESTORE PRECISION
            out.precision(tp);
        }

        ///////////////
        // ACCESSORS //
        ///////////////

        // get vector as float[3] array
        void get_as_array3(float* arr)
        {
            arr[0] = v_data[0];
            arr[1] = v_data[1];
            arr[2] = v_data[2];
        }
        
        // Get data access
        const float* value_ptr() const
        {
            return v_data;
        }
        
        float& operator[](const size_t& i){ return v_data[i]; }
        const float& operator[](const size_t& i) const { return v_data[i]; }
        float& at(const size_t& i){ return v_data[i]; }
        const float& at(const size_t& i) const {return v_data[i]; }

        ////////////////////
        // Misc Functions //
        ////////////////////

        float dot(const vec3& o) const
        {
            return v_data[0] * o.v_data[0] +
                   v_data[1] * o.v_data[1] +
                   v_data[2] * o.v_data[2];
        }

        vec3 cross(const vec3& o) const
        {
            return vec3(v_data[1] * o.v_data[2] - v_data[2] * o.v_data[1],
                        v_data[2] * o.v_data[0] - v_data[0] * o.v_data[2],
                        v_data[0] * o.v_data[1] - v_data[0] * o.v_data[1]);
        }

        float sum() const
        {
            return v_data[0] + v_data[1] + v_data[2];
        }

        float magnitude_squared() const
        {
            return v_data[0] * v_data[0] +
                   v_data[1] * v_data[1] +
                   v_data[2] * v_data[2];
        }

        float magnitude() const
        {
            return sqrt(at(0) * at(0) +
                        at(1) * at(1) +
                        at(2) * at(2));
        }
        vec3& normalize()
        {
            float mag = magnitude_squared();
            if(mag == 0)
            {
                return *this; // PREVENT DIVIDE BY ZERO ERRORS
            }
            mag = magnitude();
            at(0) = at(0) / mag;
            at(1) = at(1) / mag;
            at(2) = at(2) / mag;
            return *this;
        }

        vec3 normalized() const
        {
            if(magnitude_squared() == 0)
            {
                return vec3(); // PREVENT DIVIDE BY ZERO ERRORS
            }
            return vec3(*this) / magnitude();
        }

        vec3& set_magnitude(const float& n_mag)
        {
            normalize();
            return (*this *= n_mag);
        }

        ///////////////
        // OPERATORS //
        ///////////////

        vec3& operator=(const vec3 o)        
        {
            v_data[0] = o.v_data[0];
            v_data[1] = o.v_data[1];
            v_data[2] = o.v_data[2];
            return *this;
        }
        vec3& set(const vec3 o)
        {
            v_data[0] = o.v_data[0];
            v_data[1] = o.v_data[1];
            v_data[2] = o.v_data[2];
            return *this;
        }
        vec3& set(const float& x, const float& y, const float& z)
        {
            v_data[0] = x;
            v_data[1] = y;
            v_data[2] = z;
            return *this;
        }

        vec3 operator+(const vec3& o) const
        {
            return vec3(v_data[0] + o.v_data[0],
                        v_data[1] + o.v_data[1],
                        v_data[2] + o.v_data[2]);
        }
        vec3& operator+=(const vec3& o)
        {
            v_data[0] += o.v_data[0];
            v_data[1] += o.v_data[1];
            v_data[2] += o.v_data[2];
            return *this;
        }
        vec3 operator+(const float& o) const
        {
            return vec3(v_data[0] + o,
                        v_data[1] + o,
                        v_data[2] + o);
        }
        vec3& operator+=(const float& o)
        {
            v_data[0] += o;
            v_data[1] += o;
            v_data[2] += o;
            return *this;
        }

        vec3 operator-()
        {
            return vec3(-v_data[0],
                        -v_data[1],
                        -v_data[2]);
        }
        vec3 operator-(const vec3& o) const
        {
            return vec3(v_data[0] - o.v_data[0],
                        v_data[1] - o.v_data[1],
                        v_data[2] - o.v_data[2]);
        }
        vec3& operator-=(const vec3& o)
        {
            v_data[0] -= o.v_data[0];
            v_data[1] -= o.v_data[1];
            v_data[2] -= o.v_data[2];
            return *this;
        }
        vec3 operator-(const float& o) const
        {
            return vec3(v_data[0] - o,
                        v_data[1] - o,
                        v_data[2] - o);
        }
        vec3& operator-=(const float& o)
        {
            v_data[0] -= o;
            v_data[1] -= o;
            v_data[2] -= o;
            return *this;
        }

        vec3 operator*(const vec3& o) const
        {
            return vec3(v_data[0] * o.v_data[0],
                        v_data[1] * o.v_data[1],
                        v_data[2] * o.v_data[2]);
        }
        vec3& operator*=(const vec3& o)
        {
            v_data[0] *= o.v_data[0];
            v_data[1] *= o.v_data[1];
            v_data[2] *= o.v_data[2];
            return *this;
        }
        vec3 operator*(const float& o) const
        {
            return vec3(v_data[0] * o,
                        v_data[1] * o,
                        v_data[2] * o);
        }
        vec3& operator*=(const float& o)
        {
            v_data[0] *= o;
            v_data[1] *= o;
            v_data[2] *= o;
            return *this;
        }

        vec3 operator/(const vec3& o) const
        {
            return vec3(v_data[0] / o.v_data[0],
                        v_data[1] / o.v_data[1],
                        v_data[2] / o.v_data[2]);
        }
        vec3& operator/=(const vec3& o)
        {
            v_data[0] /= o.v_data[0];
            v_data[1] /= o.v_data[1];
            v_data[2] /= o.v_data[2];
            return *this;
        }
        vec3 operator/(const float& o) const
        {
            return vec3(v_data[0] / o,
                        v_data[1] / o,
                        v_data[2] / o);
        }
        vec3& operator/=(const float& o)
        {
            v_data[0] /= o;
            v_data[1] /= o;
            v_data[2] /= o;
            return *this;
        }
        
    private:
        float v_data[3];
    };
}


#endif
