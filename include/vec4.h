#ifndef _VEC4_H_
#define _VEC4_H_

#include <iostream>
#include <math.h>

namespace mvp
{
    class vec4
    {
    public:
        //////////////////
        // CONSTRUCTORS //
        //////////////////

        vec4()
        {
            v_data[0] = v_data[1] = v_data[2] = v_data[3] = 0;
        }

        vec4(const float& x, const float& y, const float& z, const float& w)
        {
            v_data[0] = x;
            v_data[1] = y;
            v_data[2] = z;
            v_data[3] = w;
        }

        vec4(const float& n)
        {
            v_data[0] = v_data[1] = v_data[2] = v_data[3] = n;
        }

        vec4(const vec4& v)
        {
            v_data[0] = v.v_data[0];
            v_data[1] = v.v_data[1];
            v_data[2] = v.v_data[2];
            v_data[3] = v.v_data[3];
        }

        ///////////
        // PRINT //
        ///////////

        // PRINT ALL ELEMENTS TO OUT
        void print(std::ostream& out) const
        {
            // MODIFY PRECISION
            std::streamsize tp = out.precision(5);
            out << '(' << v_data[0] << ", " << v_data[1] << ", " << v_data[2] << ", " << v_data[3] << ')' << std::endl;
            // RESTORE PRECISION
            out.precision(tp);
        }

        ///////////////
        // ACCESSORS //
        ///////////////

        // get vector as float[4] array
        void get_as_array4(float* arr)
        {
            arr[0] = v_data[0];
            arr[1] = v_data[1];
            arr[2] = v_data[2];
            arr[3] = v_data[3];
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

        float dot(const vec4& o) const
        {
            return v_data[0] * o.v_data[0] +
                   v_data[1] * o.v_data[1] +
                   v_data[2] * o.v_data[2] +
                   v_data[3] * o.v_data[3];
        }

        float sum() const
        {
            return v_data[0] + v_data[1] + v_data[2] + v_data[3];
        }

        float magnitude_squared() const
        {
            return v_data[0] * v_data[0] +
                   v_data[1] * v_data[1] +
                   v_data[2] * v_data[2] +
                   v_data[3] * v_data[3];
        }

        float magnitude() const
        {
            return sqrt(v_data[0] * v_data[0] +
                        v_data[1] * v_data[1] +
                        v_data[2] * v_data[2] +
                        v_data[3] * v_data[3]);
        }
        vec4& normalize()
        {
            float mag = magnitude_squared();
            if(mag == 0)
            {
                return *this; // PREVENT DIVIDE BY ZERO ERRORS
            }
            mag = magnitude();
            v_data[0] = v_data[0] / mag;
            v_data[1] = v_data[1] / mag;
            v_data[2] = v_data[2] / mag;
            v_data[3] = v_data[3] / mag;
            return *this;
        }

        vec4 normalized() const
        {
            if(magnitude_squared() == 0)
            {
                return vec4(); // PREVENT DIVIDE BY ZERO ERRORS
            }
            return vec4(*this) / magnitude();
        }

        vec4& set_magnitude(const float& n_mag)
        {
            normalize();
            return (*this *= n_mag);
        }

        ///////////////
        // OPERATORS //
        ///////////////

        vec4& operator=(const vec4 o)        
        {
            v_data[0] = o.v_data[0];
            v_data[1] = o.v_data[1];
            v_data[2] = o.v_data[2];
            v_data[3] = o.v_data[3];
            return *this;
        }
        vec4& set(const vec4 o)
        {
            v_data[0] = o.v_data[0];
            v_data[1] = o.v_data[1];
            v_data[2] = o.v_data[2];
            v_data[3] = o.v_data[3];
            return *this;
        }
        vec4& set(const float& x, const float& y, const float& z, const float& w)
        {
            v_data[0] = x;
            v_data[1] = y;
            v_data[2] = z;
            v_data[3] = w;
            return *this;
        }

        vec4 operator+(const vec4& o) const
        {
            return vec4(v_data[0] + o.v_data[0],
                              v_data[1] + o.v_data[1],
                              v_data[2] + o.v_data[2],
                              v_data[3] + o.v_data[3]);
        }
        vec4& operator+=(const vec4& o)
        {
            v_data[0] += o.v_data[0];
            v_data[1] += o.v_data[1];
            v_data[2] += o.v_data[2];
            v_data[3] += o.v_data[3];
            return *this;
        }
        vec4 operator+(const float& o) const
        {
            return vec4(v_data[0] + o,
                        v_data[1] + o,
                        v_data[2] + o,
                        v_data[3] + o);
        }
        vec4& operator+=(const float& o)
        {
            v_data[0] += o;
            v_data[1] += o;
            v_data[2] += o;
            v_data[3] += o;
            return *this;
        }

        vec4 operator-()
        {
            return vec4(-v_data[0],
                        -v_data[1],
                        -v_data[2],
                        -v_data[3]);
        }
        vec4 operator-(const vec4& o) const
        {
            return vec4(v_data[0] - o.v_data[0],
                        v_data[1] - o.v_data[1],
                        v_data[2] - o.v_data[2],
                        v_data[3] - o.v_data[3]);
        }
        vec4& operator-=(const vec4& o)
        {
            v_data[0] -= o.v_data[0];
            v_data[1] -= o.v_data[1];
            v_data[2] -= o.v_data[2];
            v_data[3] -= o.v_data[3];
            return *this;
        }
        vec4 operator-(const float& o) const
        {
            return vec4(v_data[0] - o,
                        v_data[1] - o,
                        v_data[2] - o,
                        v_data[3] - o);
        }
        vec4& operator-=(const float& o)
        {
            v_data[0] -= o;
            v_data[1] -= o;
            v_data[2] -= o;
            v_data[3] -= o;
            return *this;
        }

        vec4 operator*(const vec4& o) const
        {
            return vec4(v_data[0] * o.v_data[0],
                        v_data[1] * o.v_data[1],
                        v_data[2] * o.v_data[2],
                        v_data[3] * o.v_data[3]);
        }
        vec4& operator*=(const vec4& o)
        {
            v_data[0] *= o.v_data[0];
            v_data[1] *= o.v_data[1];
            v_data[2] *= o.v_data[2];
            v_data[3] *= o.v_data[3];
            return *this;
        }
        vec4 operator*(const float& o) const
        {
            return vec4(v_data[0] * o,
                        v_data[1] * o,
                        v_data[2] * o,
                        v_data[3] * o);
        }
        vec4& operator*=(const float& o)
        {
            v_data[0] *= o;
            v_data[1] *= o;
            v_data[2] *= o;
            v_data[3] *= o;
            return *this;
        }

        vec4 operator/(const vec4& o) const
        {
            return vec4(v_data[0] / o.v_data[0],
                        v_data[1] / o.v_data[1],
                        v_data[2] / o.v_data[2],
                        v_data[3] / o.v_data[3]);
        }
        vec4& operator/=(const vec4& o)
        {
            v_data[0] /= o.v_data[0];
            v_data[1] /= o.v_data[1];
            v_data[2] /= o.v_data[2];
            v_data[3] /= o.v_data[3];
            return *this;
        }
        vec4 operator/(const float& o) const
        {
            return vec4(v_data[0] / o,
                        v_data[1] / o,
                        v_data[2] / o,
                        v_data[3] / o);
        }
        vec4& operator/=(const float& o)
        {
            v_data[0] /= o;
            v_data[1] /= o;
            v_data[2] /= o;
            v_data[3] /= o;
            return *this;
        }
    
    private:
        float v_data[4];
    };
}


#endif
