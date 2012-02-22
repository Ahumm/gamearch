#ifndef _VEC2_H_
#define _VEC2_H_

#include <iostream>
#include <math.h>

namespace mvp
{
    class vec2
    {
    public:
        //////////////////
        // CONSTRUCTORS //
        //////////////////

        vec2()
        {
            v_data[0] = v_data[1] = 0;
        }

        vec2(const float& x, const float& y)
        {
            v_data[0] = x;
            v_data[1] = y;
        }

        vec2(const float& n)
        {
            v_data[0] = v_data[1] = n;
        }

        vec2(const vec2& v)
        {
            v_data[0] = v.v_data[0];
            v_data[1] = v.v_data[1];
        }

        ///////////
        // PRINT //
        ///////////

        // PRINT ALL ELEMENTS TO OUT
        void print(std::ostream& out) const
        {
            // MODIFY PRECISION
            std::streamsize tp = out.precision(5);
            out << '(' << v_data[0] << ", " << v_data[1] << ')' << std::endl;
            // RESTORE PRECISION
            out.precision(tp);
        }

        ///////////////
        // ACCESSORS //
        ///////////////

        // get vector as float[2] array
        void get_as_array2(float* arr)
        {
            arr[0] = v_data[0];
            arr[1] = v_data[1];
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

        float dot(const vec2& o) const
        {
            return v_data[0] * o.v_data[0] +
                   v_data[1] * o.v_data[1];
        }

        float sum() const
        {
            return v_data[0] + v_data[1];
        }

        float magnitude_squared() const
        {
            return v_data[0] * v_data[0] +
                   v_data[1] * v_data[1];
        }

        float magnitude() const
        {
            return sqrt(v_data[0] * v_data[0] +
                        v_data[1] * v_data[1]);
        }
        vec2& normalize()
        {
            float mag = magnitude_squared();
            if(mag == 0)
            {
                return *this; // PREVENT DIVIDE BY ZERO ERRORS
            }
            mag = magnitude();
            v_data[0] = v_data[0] / mag;
            v_data[1] = v_data[1] / mag;
            return *this;
        }

        vec2 normalized() const
        {
            if(magnitude_squared() == 0)
            {
                return vec2(); // PREVENT DIVIDE BY ZERO ERRORS
            }
            return vec2(*this) / magnitude();
        }

        vec2& set_magnitude(const float& n_mag)
        {
            normalize();
            return (*this *= n_mag);
        }

        ///////////////
        // OPERATORS //
        ///////////////

        vec2& operator=(const vec2 o)        
        {
            v_data[0] = o.v_data[0];
            v_data[1] = o.v_data[1];
            return *this;
        }
        vec2& set(const vec2 o)
        {
            v_data[0] = o.v_data[0];
            v_data[1] = o.v_data[1];
            return *this;
        }
        vec2& set(const float& x, const float& y, const float& z)
        {
            v_data[0] = x;
            v_data[1] = y;
            return *this;
        }

        vec2 operator+(const vec2& o) const
        {
            return vec2(v_data[0] + o.v_data[0],
                        v_data[1] + o.v_data[1]);
        }
        vec2& operator+=(const vec2& o)
        {
            v_data[0] += o.v_data[0];
            v_data[1] += o.v_data[1];
            return *this;
        }
        vec2 operator+(const float& o) const
        {
            return vec2(v_data[0] + o,
                        v_data[1] + o);
        }
        vec2& operator+=(const float& o)
        {
            v_data[0] += o;
            v_data[1] += o;
            return *this;
        }

        vec2 operator-()
        {
            return vec2(-v_data[0],
                        -v_data[1]);
        }
        vec2 operator-(const vec2& o) const
        {
            return vec2(v_data[0] - o.v_data[0],
                        v_data[1] - o.v_data[1]);
        }
        vec2& operator-=(const vec2& o)
        {
            v_data[0] -= o.v_data[0];
            v_data[1] -= o.v_data[1];
            return *this;
        }
        vec2 operator-(const float& o) const
        {
            return vec2(v_data[0] - o,
                        v_data[1] - o);
        }
        vec2& operator-=(const float& o)
        {
            v_data[0] -= o;
            v_data[1] -= o;
            return *this;
        }

        vec2 operator*(const vec2& o) const
        {
            return vec2(v_data[0] * o.v_data[0],
                        v_data[1] * o.v_data[1]);
        }
        vec2& operator*=(const vec2& o)
        {
            v_data[0] *= o.v_data[0];
            v_data[1] *= o.v_data[1];
            return *this;
        }
        vec2 operator*(const float& o) const
        {
            return vec2(v_data[0] * o,
                        v_data[1] * o);
        }
        vec2& operator*=(const float& o)
        {
            v_data[0] *= o;
            v_data[1] *= o;
            return *this;
        }

        vec2 operator/(const vec2& o) const
        {
            return vec2(v_data[0] / o.v_data[0],
                        v_data[1] / o.v_data[1]);
        }
        vec2& operator/=(const vec2& o)
        {
            v_data[0] /= o.v_data[0];
            v_data[1] /= o.v_data[1];
            return *this;
        }
        vec2 operator/(const float& o) const
        {
            return vec2(v_data[0] / o,
                        v_data[1] / o);
        }
        vec2& operator/=(const float& o)
        {
            v_data[0] /= o;
            v_data[1] /= o;
            return *this;
        }
        
    private:    
        float v_data[2];
    };
}


#endif
