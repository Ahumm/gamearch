#ifndef _POINT_H_
#define _POINT_H_

#include "vector3.h"

namespace mvp
{
    template<class T>
    class point
    {
    public:
        //////////////////
        // CONSTRUCTORS //
        //////////////////

        point()
        {
            v_data[0] = v_data[1] = v_data[2] = T(0);
        }

        point(const T& x, const T& y, const T& z)
        {
            v_data[0] = x;
            v_data[1] = y;
            v_data[2] = z;
        }

        point(const T& n)
        {
            v_data[0] = v_data[1] = v_data[2] = n;
        }

        point(const point<T>& v)
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

        // get point as T[4] or T[3] array
        void get_as_array4(T* arr)
        {
            arr[0] = v_data[0];
            arr[1] = v_data[1];
            arr[2] = v_data[2];
            arr[3] = T(1);
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

        T sum() const
        {
            return v_data[0] + v_data[1] + v_data[2];
        }

        ///////////////
        // OPERATORS //
        ///////////////

        point<T>& operator=(const point<T> o)        
        {
            v_data[0] = o.v_data[0];
            v_data[1] = o.v_data[1];
            v_data[2] = o.v_data[2];
            return *this;
        }
        point<T>& set(const point<T> o)
        {
            v_data[0] = o.v_data[0];
            v_data[1] = o.v_data[1];
            v_data[2] = o.v_data[2];
            return *this;
        }
        point<T>& set(const T& x, const T& y, const T& z)
        {
            v_data[0] = x;
            v_data[1] = y;
            v_data[2] = z;
            return *this;
        }

        point<T> operator+(const point<T>& o) const
        {
            return point<T>(v_data[0] + o.v_data[0],
                              v_data[1] + o.v_data[1],
                              v_data[2] + o.v_data[2]);
        }
        point<T>& operator+=(const point<T>& o)
        {
            v_data[0] += o.v_data[0];
            v_data[1] += o.v_data[1];
            v_data[2] += o.v_data[2];
            return *this;
        }
        point<T> operator+(const T& o) const
        {
            return point<T>(v_data[0] + o,
                              v_data[1] + o,
                              v_data[2] + o);
        }
        point<T>& operator+=(const T& o)
        {
            v_data[0] += o;
            v_data[1] += o;
            v_data[2] += o;
            return *this;
        }

        point<T> operator-()
        {
            return point<T>(-v_data[0],
                              -v_data[1],
                              -v_data[2]);
        }
        point<T> operator-(const point<T>& o) const
        {
            return point<T>(v_data[0] - o.v_data[0],
                              v_data[1] - o.v_data[1],
                              v_data[2] - o.v_data[2]);
        }
        point<T>& operator-=(const point<T>& o)
        {
            v_data[0] -= o.v_data[0];
            v_data[1] -= o.v_data[1];
            v_data[2] -= o.v_data[2];
            return *this;
        }
        point<T> operator-(const T& o) const
        {
            return point<T>(v_data[0] - o,
                              v_data[1] - o,
                              v_data[2] - o);
        }
        point<T>& operator-=(const T& o)
        {
            v_data[0] -= o;
            v_data[1] -= o;
            v_data[2] -= o;
            return *this;
        }

        point<T> operator*(const point<T>& o) const
        {
            return point<T>(v_data[0] * o.v_data[0],
                              v_data[1] * o.v_data[1],
                              v_data[2] * o.v_data[2]);
        }
        point<T>& operator*=(const point<T>& o)
        {
            v_data[0] *= o.v_data[0];
            v_data[1] *= o.v_data[1];
            v_data[2] *= o.v_data[2];
            return *this;
        }
        point<T> operator*(const T& o) const
        {
            return point<T>(v_data[0] * o,
                              v_data[1] * o,
                              v_data[2] * o);
        }
        point<T>& operator*=(const T& o)
        {
            v_data[0] *= o;
            v_data[1] *= o;
            v_data[2] *= o;
            return *this;
        }

        point<T> operator/(const point<T>& o) const
        {
            return point<T>(v_data[0] / o.v_data[0],
                              v_data[1] / o.v_data[1],
                              v_data[2] / o.v_data[2]);
        }
        point<T>& operator/=(const point<T>& o)
        {
            v_data[0] /= o.v_data[0];
            v_data[1] /= o.v_data[1];
            v_data[2] /= o.v_data[2];
            return *this;
        }
        point<T> operator/(const T& o) const
        {
            return point<T>(v_data[0] / o,
                              v_data[1] / o,
                              v_data[2] / o);
        }
        point<T>& operator/=(const T& o)
        {
            v_data[0] /= o;
            v_data[1] /= o;
            v_data[2] /= o;
            return *this;
        }


    protected:
        T v_data[3];
    };

    //////////////////////////
    // POINT & VECTOR3 MATH //
    //////////////////////////

    // VECTOR3 + VECTOR3 = VECTOR3
    template<class T>
    vector3<T> operator+(const vector3<T>& v1, const vector3<T>& v2)
    {
        return vector3<T>(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
    }

    // VECTOR3 - VECTOR3 = VECTOR3
    template<class T>
    vector3<T> operator-(const vector3<T>& v1, const vector3<T>& v2)
    {
        return vector3<T>(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
    }

    // POINT + VECTOR3 = POINT
    template<class T>
    point<T> operator+(const point<T>& p, const vector3<T>& v)
    {
        return point<T>(p[0] + v[0], p[1] + v[1], p[2] + v[2]);
    }

    // POINT - VECTOR3 = POINT
    template<class T>
    point<T> operator-(const point<T>& p, const vector3<T>& v)
    {
        return point<T>(p[0] - v[0], p[1] - v[1], p[2] - v[2]);
    }

    // POINT - POINT = VECTOR3
    template<class T>
    vector3<T> operator-(const point<T>& p1, const point<T>& p2)
    {
        return vector3<T>(p1[0] - p2[0], p1[1] - p2[1], p1[2] - p2[2]);
    }


}

#endif
