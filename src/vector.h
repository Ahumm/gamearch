#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <array>
#include <iostream>
#include <math.h>

namespace t_stuff {
    // GENERIC VECTOR OF N MANY T'S
    template <class T, size_t N>
        class Vector {
    public:
        // DEFAULT CONSTRUCTOR: {0,0, ... ,0}
        Vector(){ v_data.fill(0); }
        // STD::INITIALIZER_LIST<T> CONSTRUCTOR (PAD: 0)
        Vector(const std::initializer_list<T> s){
            auto p = s.begin();
            size_t i;
            for(i = 0; i < N && p != s.end(); ++i){
                v_data[i] = *p;
                ++p;
            }
            // PAD WITH ZEROS IF NEEDED
            for(; i < N; ++i){
                v_data[i] = T(0);
            }
        }

        // STD::ARRAY CONSTRUCTOR
        Vector(const std::array<T,N> arr){ v_data = arr; }

        // COPY CONSTRUCTOR
        Vector(const Vector<T,N>& o){ v_data = o.v_data; }
        
        // PRINT ALL ELEMENTS TO OUT
        void print(std::ostream& out){
            // MODIFY PRECISION
            std::streamsize tp = out.precision(5);
            out << '(';
            for(size_t i = 0; i < N; ++i){
                if(v_data[i] >= 0) out << ' ';
                out << std::fixed << v_data[i];
                if(i != N - 1) out << ", ";
                else out << ')';
            }
            // RESTORE PRECISION
            out.precision(tp);
        }

        // PRINT FIRST NOUT ELEMENTS TO OUT
        void print(std::ostream& out, int nout){
            // MODIFY PRECISION
            std::streamsize tp = out.precision(5);
            out << '(';
            for(size_t i = 0; i < N && i < nout; ++i){
                if(v_data[i] >= 0) out << ' ';
                out << std::fixed << v_data[i];
                if(i != N - 1 && i != nout - 1) out << ", ";
                else out << ')';
            }
            // RESTORE PRECISION
            out.precision(tp);
        }
        
        // ASSIGNMENT: STD::INITIALIZER_LIST<T> -> VECTOR<T,N>
        Vector<T,N>& operator=(const std::initializer_list<T>& s){
            auto p = s.begin();
            for(size_t i = 0; i < N && p != s.end(); ++i){
                v_data[i] = *p;
                ++p;
            }
            return *this;
        }
        
        // ASSIGNMENT: VECTOR<T,N> -> VECTOR<T,N>
        Vector<T,N>& operator=(const Vector<T,N>& o){
            if(this == &o)
                return *this;
            v_data = o.v_data;
            return *this;
        }

        // VECTOR<T,N> + VECTOR<T,N> = VECTOR<T,N>
        Vector<T,N> operator+(const Vector<T,N>& o){
            std::array<T,N> tmp;
            for(size_t i = 0; i < N; ++i)
                tmp[i] = v_data[i] + o.v_data[i];
            return Vector<T,N>(tmp);
        }

        // VECTOR<T,N> - VECTOR<T,N> = VECTOR<T,N>
        Vector<T,N> operator-(const Vector<T,N>& o){
            std::array<T,N> tmp;
            for(size_t i = 0; i < N; ++i)
                tmp[i] = v_data[i] - o.v_data[i];
            return Vector<T,N>(tmp);
        }
        
        // ACCESSORS
        T& operator[](const size_t i){ return v_data[i]; }
        const T& operator[](const size_t i) const { return v_data[i]; }
        T& at(const size_t i){ return v_data[i]; }
        const T& at(const size_t i) const { return v_data[i]; }
        
        // COMPUTE DOT PRODUCT
        T dot(const Vector<T,N> o) const {
            T sum = 0;
            for(size_t i = 0; i < N; ++i){
                sum = sum + (v_data[i] * o.v_data[i]);
            }
            return sum;
        }
        
    protected:
        // STL ARRAY, WOO
        std::array<T,N> v_data;
    };
    
    // EXPLICIT VECTOR<FLOAT,4> (X,Y,Z,W) (VECTOR IN 3D SPACE)
    class Vector3 : public Vector<float,4> {
     public:
        // DEFAULT CONSTRUCTOR: {0,0,0,0}
        Vector3() : Vector<float,4>() {}
        
        // STD::INITIALIZER_LIST CONSTRUCTOR
        Vector3(const std::initializer_list<float>& s) : Vector<float,4>(s) {
            if(s.size() == 3)
                this->v_data[3] = 0.0;
        }

        // MANUAL CONSTRUCTION, LIMITED
        Vector3(const float x, const float y, const float z) : Vector<float,4>(){
            this->v_data[0] = x;
            this->v_data[1] = y;
            this->v_data[2] = z;
            this->v_data[3] = 0.0;
        }

        // MANUAL CONSTRUCTION, FULL
        Vector3(const float x, const float y, const float z, const float w) : Vector<float,4>(){
            this->v_data[0] = x;
            this->v_data[1] = y;
            this->v_data[2] = z;
            this->v_data[3] = w;
        }

        // COPY CONSTRUCTOR
        Vector3(const Vector3& o){ this->v_data = o.v_data; }
        

        // OBTAIN CROSS PRODUCT OF THIS AND VECTOR3 O
        Vector3 cross(const Vector3& o){
            Vector3 ret;
            ret.v_data[0] = (this->v_data[1] * o.v_data[2]) - (this->v_data[2] * o.v_data[1]);
            ret.v_data[1] = (this->v_data[2] * o.v_data[0]) - (this->v_data[0] * o.v_data[2]);
            ret.v_data[2] = (this->v_data[0] * o.v_data[1]) - (this->v_data[1] * o.v_data[0]);
            return ret;
        }
        
        // ASSIGNMENT: STD::INITIALIZER_LIST -> VECTOR3
        Vector3& operator=(const std::initializer_list<float>& s){
            auto p = s.begin();
            for(size_t i = 0; i < 4 && p != s.end(); ++i){
                this->v_data[i] = *p;
                ++p;
            }
            if(s.size() == 3)
                this->v_data[3] = 0.0;
            return *this;
        }
        
        // ASSIGNMENT: VECTOR3 -> VECTOR3
        Vector3& operator=(const Vector3& o){
            if(this == &o)
                return *this;
            this->v_data = o.v_data;
            return *this;
        }

        // OBTAIN VECTOR'S MAGNITUDE
        float magnitude(){
            float tmp = 0;
            for(size_t i = 0; i < 3; ++i){
                tmp += this->v_data[i] * this->v_data[i];
            }
            return sqrt(tmp);
        }
        
        // NORMALIZE VECTOR3 AND RETURN
        Vector3 normalized(){
            float mag = magnitude();
            return Vector3(this->v_data[0] / mag,
                           this->v_data[1] / mag,
                           this->v_data[2] / mag);
        }

        // NORMALIZE VECTOR3 IN PLACE
        void normalize(){
            float mag = magnitude();
            this->v_data[0] = this->v_data[0] / mag;
            this->v_data[1] = this->v_data[1] / mag;
            this->v_data[2] = this->v_data[2] / mag;
        }
    };

    // EXPLICIT VECTOR<FLOAT,4> (X,Y,Z,W) (POINT IN 3D SPACE)
    class Point : public Vector<float,4> {
     public:
        // DEFAULT CONSTRUCTOR: {0,0,0,1}
        Point() : Vector<float,4>() { this->v_data[3] = 1; }
        Point(const std::initializer_list<float>& s) : Vector<float,4>(s) {
            this->v_data[3] = 1.0;
        }
        // MANUAL CONSTRUCTION, LIMITED
        Point(const float x, const float y, const float z) : Vector<float,4>(){
            this->v_data[0] = x;
            this->v_data[1] = y;
            this->v_data[2] = z;
            this->v_data[3] = 1.0;
        }
        // MANUAL CONSTRUCTION, FULL
        Point(const float x, const float y, const float z, const float w) : Vector<float,4>(){
            this->v_data[0] = x;
            this->v_data[1] = y;
            this->v_data[2] = z;
            this->v_data[3] = w;
        }
        // COPY CONSTRUCTOR
        Point(const Point& o){ this->v_data = o.v_data; }
        
        // ASSIGNMENT: STD::INITIALIZER_LIST<FLOAT> -> POINT
        Point& operator=(const std::initializer_list<float>& s){
            auto p = s.begin();
            for(size_t i = 0; i < 4 && p != s.end(); i++){
                this->v_data[i] = *p;
                ++p;
            }
            if(s.size() == 3)
                this->v_data[3] = 1.0;
            return *this;
        }
        
        // ASSIGNEMENT: POINT -> POINT
        Point& operator=(const Point& o){
            if(this == &o)
                return *this;
            this->v_data = o.v_data;
            return *this;
        }
    };


    //////////////////////////
    // NON-MEMBER FUNCTIONS //
    //////////////////////////

    // VECTOR3 + VECTOR3 = VECTOR3
    Vector3 operator+(const Vector3& v1, const Vector3& v2){
        return Vector3(v1[0] + v1[0], v1[1] + v2[1], v1[2] + v2[2]);
    }

    // VECTOR3 - VECTOR3 = VECTOR3
    Vector3 operator-(const Vector3& v1, const Vector3& v2){
        return Vector3(v1[0] - v1[0], v1[1] - v2[1], v1[2] - v2[2]);
    }

    // POINT + VECTOR3 = POINT
    Point operator+(const Point& p, const Vector3& v){
        return Point(p[0] + v[0], p[1] + v[1], p[2] + v[2]);
    }

    // POINT - VECTOR3 = POINT
    Point operator-(const Point& p, const Vector3& v){
        return Point(p[0] - v[0], p[1] - v[1], p[2] - v[2]);
    }

    // POINT - POINT = VECTOR3
    Vector3 operator-(const Point& p1, const Point & p2){
        return Vector3(p1[0] - p2[0], p1[1] - p2[1], p1[2] - p2[2]);
    }
        
        
        
}
#endif
