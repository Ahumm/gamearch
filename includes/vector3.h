#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <iostream>

namespace mvp
{
    template<class T>
    class vector3
    {
    public:
        //////////////////
        // CONSTRUCTORS //
        //////////////////

        vector3()
        {
            v_data[0] = v_data[1] = v_data[2] = T(0);
        }

        vector3(const T& x, const T& y, const T& z)
        {
            v_data[0] = x;
            v_data[1] = y;
            v_data[2] = z;
        }

        vector3(const T& n)
        {
            v_data[0] = v_data[1] = v_data[2] = n;
        }

        vector3(const vector3& v)
        {
            v_data[0] = v.v_data[0];
            v_data[1] = v.v_data[1];
            v_data[2] = v.v_data[2];
        }

        ///////////
        // PRINT //
        ///////////

        // PRINT ALL ELEMENTS TO OUT
        void print(std::ostream& out){
            // MODIFY PRECISION
            std::streamsize tp = out.precision(5);
            out << '(' << v_data[0] << ", " << v_data[1] << ", " << v_data[2] << ')' << std::endl;
            // RESTORE PRECISION
            out.precision(tp);
        }

        ///////////////
        // ACCESSORS //
        ///////////////

        // get vector as T[4] or T[3] array
        void get_as_array4(T* arr)
        {
            arr[0] = v_data[0];
            arr[1] = v_data[1];
            arr[2] = v_data[2];
            arr[3] = T(0);
        }
        void get_ass_array3(T* arr)
        {
            arr[0] = v_data[0];
            arr[1] = v_data[1];
            arr[2] = v_data[2];
        }

        T& operator[](const size_t& i){ return v_data[i]; }
        const T& operator[](const size_t& i) const { return v_data[i]; }
        T& at(const size_t& i){ return v_data[i]; }
        const T& at(const size_t& i) const {return v_data[i]; }

        ////////////////////
        // Misc Functions //
        ////////////////////

        T dot(const vector3<T>& o) const
        {
            return v_data[0] * o.v_data[0] +
                v_data[1] * o.v_data[1] +
                v_data[2] * o.v_data[2];
        }

        T cross(const vector3<T>& o) const
        {
            return vector3<T>(v_data[1] * o.v_data[2] - v_data[2] * o.v_data[1],
                              v_data[2] * o.v_data[0] - v_data[0] * o.v_data[2],
                              v_data[0] * o.v_data[1] - v_data[0] * o.v_data[1]);
        }

        T sum() const
        {
            return v_data[0] + v_data[1] + v_data[2];
        }

        T magnitude_squared() const
        {
            return v_data[0] * v_data[0]
                + v_data[1] * v_data[1]
                + v_data[2] * v_data[2];
        }

        ///////////////
        // OPERATORS //
        ///////////////

        vector3<T>& operator=(const vector3<T> o)        
        {
            v_data[0] = o.v_data[0];
            v_data[1] = o.v_data[1];
            v_data[2] = o.v_data[2];
            return *this;
        }
        vector3<T>& set(const vector3<T> o)
        {
            v_data[0] = o.v_data[0];
            v_data[1] = o.v_data[1];
            v_data[2] = o.v_data[2];
            return *this;
        }
        vector3<T>& set(const T& x, const T& y, const T& z)
        {
            v_data[0] = x;
            v_data[1] = y;
            v_data[2] = z;
            return *this;
        }

        vector3<T> operator+(const vector3<T>& o) const
        {
            return vector3<T>(v_data[0] + o.v_data[0],
                              v_data[1] + o.v_data[1],
                              v_data[2] + o.v_data[2]);
        }
        vector3<T>& operator+=(const vector3<T>& o)
        {
            v_data[0] += o.v_data[0];
            v_data[1] += o.v_data[1];
            v_data[2] += o.v_data[2];
            return *this;
        }
        vector3<T> operator+(const T& o) const
        {
            return vector3<T>(v_data[0] + o,
                              v_data[1] + o,
                              v_data[2] + o);
        }
        vector3<T>& operator+=(const T& o)
        {
            v_data[0] += o;
            v_data[1] += o;
            v_data[2] += o;
            return *this;
        }

        vector3<T> operator-()
        {
            return vector3<T>(-v_data[0],
                              -v_data[1],
                              -v_data[2]);
        }
        vector3<T> operator-(const vector3<T>& o) const
        {
            return vector3<T>(v_data[0] - o.v_data[0],
                              v_data[1] - o.v_data[1],
                              v_data[2] - o.v_data[2]);
        }
        vector3<T>& operator-=(const vector3<T>& o)
        {
            v_data[0] -= o.v_data[0];
            v_data[1] -= o.v_data[1];
            v_data[2] -= o.v_data[2];
            return *this;
        }
        vector3<T> operator-(const T& o) const
        {
            return vector3<T>(v_data[0] - o,
                              v_data[1] - o,
                              v_data[2] - o);
        }
        vector3<T>& operator-=(const T& o)
        {
            v_data[0] -= o;
            v_data[1] -= o;
            v_data[2] -= o;
            return *this;
        }

        vector3<T> operator*(const vector3<T>& o) const
        {
            return vector3<T>(v_data[0] * o.v_data[0],
                              v_data[1] * o.v_data[1],
                              v_data[2] * o.v_data[2]);
        }
        vector3<T>& operator*=(const vector3<T>& o)
        {
            v_data[0] *= o.v_data[0];
            v_data[1] *= o.v_data[1];
            v_data[2] *= o.v_data[2];
            return *this;
        }
        vector3<T> operator*(const T& o) const
        {
            return vector3<T>(v_data[0] * o,
                              v_data[1] * o,
                              v_data[2] * o);
        }
        vector3<T>& operator*=(const T& o)
        {
            v_data[0] *= o;
            v_data[1] *= o;
            v_data[2] *= o;
            return *this;
        }

        vector3<T> operator/(const vector3<T>& o) const
        {
            return vector3<T>(v_data[0] / o.v_data[0],
                              v_data[1] / o.v_data[1],
                              v_data[2] / o.v_data[2]);
        }
        vector3<T>& operator/=(const vector3<T>& o)
        {
            v_data[0] /= o.v_data[0];
            v_data[1] /= o.v_data[1];
            v_data[2] /= o.v_data[2];
            return *this;
        }
        vector3<T> operator/(const T& o) const
        {
            return vector3<T>(v_data[0] / o,
                              v_data[1] / o,
                              v_data[2] / o);
        }
        vector3<T>& operator/=(const T& o)
        {
            v_data[0] /= o;
            v_data[1] /= o;
            v_data[2] /= o;
            return *this;
        }

        T magnitude() const
        {
            return sqrt(at(0) * at(0) +
                        at(1) * at(1) +
                        at(2) * at(2));
        }
        vector3<T>& normalize()
        {
            T mag = magnitude_squared();
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

        vector3<T> normalized() const
        {
            if(magnitude_squared() == 0)
            {
                return vector3<T>(); // PREVENT DIVIDE BY ZERO ERRORS
            }
            return vector3<T>(*this) / magnitude;
        }

        vector3<T>& set_magnitude(const T& n_mag)
        {
            normalize();
            return (*this *= n_mag);
        }

    protected:
        T v_data[3];
    };
}


#endif
