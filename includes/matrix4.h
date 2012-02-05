#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#include "vector3.h"
#include "point.h"
#include <math.h>

namespace mvp
{
    template<class T>
    class matrix4
    {
    public:
        matrix4()
        {
            for(size_t r = 0; r < 4; ++r)
                for(size_t c = 0; c < 4; ++c)
                    if(r == c)
                        m_data[4 * r + c] = T(1);
                    else
                        m_data[4 * r + c] = T(0);
        }
        matrix4(const T* arr)
        {
            for(size_t i = 0; i < 16; ++i)
                m_data[i] = arr[i];
        }
        matrix4(const matrix4<T>& m)
        {
            for(size_t i = 0; i < 16; ++i)
                m_data[i] = m.m_data[i];
        }

        ///////////
        // PRINT //
        ///////////

        // PRINT MATRIX TO OUT (| BORDER)
        void print(std::ostream& out){
            // MODIFY PRECISION
            std::streamsize tp = out.precision(5);
            for(int r = 0; r < 4; ++r){
                out<<"\t| ";
                for (size_t c = 0; c < 4; ++c){
                    if(m_data[4*r+c] >= 0) out << ' ';
                    out << std::fixed << m_data[4*r+c] << " ";
                }
                out<<"|"<<std::endl;
            }
            // RESTORE PRECISION
            out.precision(tp);
        }

        // PRINT MATRIX TO OUT (| BORDER), LINE PRE-FIX S
        void print(std::ostream& out, std::string s){
            // MODIFY PRECISION
            std::streamsize tp = out.precision(5);
            for(int r = 0; r < 4; ++r){
                out << s << "| ";
                for (size_t c = 0; c < 4; ++c){
                    if(m_data[4*r+c] >= 0) out << ' ';
                    out << std::fixed << m_data[4*r+c] << " ";
                }
                out << "|" << std::endl;
            }
            // RESTORE PRECISION
            out.precision(tp);
        }

        T& at(const size_t c, const size_t r) { return m_data[4 * r + c]; }
        const T& at(const size_t c, const size_t r) const { return m_data[4 * r + c]; }

        matrix4<T>& operator=(const matrix4<T>& o)
        {
            for(size_t i = 0; i < 16; ++i)
                m_data[i] = o.m_data[i];
        }
        matrix4<T>& operator=(const T* arr)
        {
            for(size_t i = 0; i < 16; ++i)
                m_data[i] = arr[i];
        }
        matrix4<T>& operator*=(const matrix4<T>& o)
        {
            T tmp[16];
            tmp[ 0] = at(0,0) * o.at(0,0) + at(1,0) * o.at(0,1) + at(2,0) * o.at(0,2) + at(3,0) * o.at(0,3);
            tmp[ 1] = at(0,0) * o.at(1,0) + at(1,0) * o.at(1,1) + at(2,0) * o.at(1,2) + at(3,0) * o.at(1,3);
            tmp[ 2] = at(0,0) * o.at(2,0) + at(1,0) * o.at(2,1) + at(2,0) * o.at(2,2) + at(3,0) * o.at(2,3);
            tmp[ 3] = at(0,0) * o.at(3,0) + at(1,0) * o.at(3,1) + at(2,0) * o.at(3,2) + at(3,0) * o.at(3,3);
            tmp[ 4] = at(0,1) * o.at(0,0) + at(1,1) * o.at(0,1) + at(2,1) * o.at(0,2) + at(3,1) * o.at(0,3);
            tmp[ 5] = at(0,1) * o.at(1,0) + at(1,1) * o.at(1,1) + at(2,1) * o.at(1,2) + at(3,1) * o.at(1,3);
            tmp[ 6] = at(0,1) * o.at(2,0) + at(1,1) * o.at(2,1) + at(2,1) * o.at(2,2) + at(3,1) * o.at(2,3);
            tmp[ 7] = at(0,1) * o.at(3,0) + at(1,1) * o.at(3,1) + at(2,1) * o.at(3,2) + at(3,1) * o.at(3,3);
            tmp[ 8] = at(0,2) * o.at(0,0) + at(1,2) * o.at(0,1) + at(2,2) * o.at(0,2) + at(3,2) * o.at(0,3);
            tmp[ 9] = at(0,2) * o.at(1,0) + at(1,2) * o.at(1,1) + at(2,2) * o.at(1,2) + at(3,2) * o.at(1,3);
            tmp[10] = at(0,2) * o.at(2,0) + at(1,2) * o.at(2,1) + at(2,2) * o.at(2,2) + at(3,2) * o.at(2,3);
            tmp[11] = at(0,2) * o.at(3,0) + at(1,2) * o.at(3,1) + at(2,2) * o.at(3,2) + at(3,2) * o.at(3,3);
            tmp[12] = at(0,3) * o.at(0,0) + at(1,3) * o.at(0,1) + at(2,3) * o.at(0,2) + at(3,3) * o.at(0,3);
            tmp[13] = at(0,3) * o.at(1,0) + at(1,3) * o.at(1,1) + at(2,3) * o.at(1,2) + at(3,3) * o.at(1,3);
            tmp[14] = at(0,3) * o.at(2,0) + at(1,3) * o.at(2,1) + at(2,3) * o.at(2,2) + at(3,3) * o.at(2,3);
            tmp[15] = at(0,3) * o.at(3,0) + at(1,3) * o.at(3,1) + at(2,3) * o.at(3,2) + at(3,3) * o.at(3,3);
            *this = matrix4<T>(tmp);
            return *this;
        }

        vector3<T> operator*(const vector3<T>& o) const
        {
            vector3<T> tmp;
            tmp[0] = o.at(0) * at(0,0) + o.at(1) * at(1,0) + o.at(2) * at(2,0) + T(0) * at(3,0);
            tmp[1] = o.at(0) * at(0,1) + o.at(1) * at(1,1) + o.at(2) * at(2,1) + T(0) * at(3,1);
            tmp[2] = o.at(0) * at(0,2) + o.at(1) * at(1,2) + o.at(2) * at(2,2) + T(0) * at(3,2);
            tmp[3] = o.at(0) * at(0,3) + o.at(1) * at(1,3) + o.at(2) * at(2,3) + T(0) * at(3,3);
            return tmp;
        }

        point<T> operator*(const point<T>& o) const
        {
            point<T> tmp;
            tmp[0] = o.at(0) * at(0,0) + o.at(1) * at(1,0) + o.at(2) * at(2,0) + T(1) * at(3,0);
            tmp[1] = o.at(0) * at(0,1) + o.at(1) * at(1,1) + o.at(2) * at(2,1) + T(1) * at(3,1);
            tmp[2] = o.at(0) * at(0,2) + o.at(1) * at(1,2) + o.at(2) * at(2,2) + T(1) * at(3,2);
            tmp[3] = o.at(0) * at(0,3) + o.at(1) * at(1,3) + o.at(2) * at(2,3) + T(1) * at(3,3);
            return tmp;
        }

        // ADD TRANSLATE ALONG X
        void translateX(const T& nx){
            matrix4<T> tmp;
            // GENERATE TRANSLATIONX MATRIX, THEN APPLY
            tmp.at(3,0) = nx;
            *this *= tmp;
        }
        // ADD TRANSLATE ALONG Y
        void translateY(const T& ny){
            matrix4<T> tmp;
            // GENERATE TRANSLATIONY MATRIX, THEN APPLY
            tmp.at(3,1) = ny;
            *this *= tmp;
        }
        // ADD TRANSLATE ALONG Z
        void translateZ(const T& nz){
            matrix4<T> tmp;
            // GENERATE TRANSLATIONZ MATRIX, THEN APPLY
            tmp.at(3,2) = nz;
            *this *= tmp;
        }
        // ADD TRANSLATE ALONG X, Y, AND Z (FLOATS)
        void translateXYZ(const T& nx, const T& ny, const T& nz){
            matrix4<T> tmp;
            // GENERATE TRANSLATEXYZ MATRIX, THEN APPLY
            tmp.at(3,0) = nx;
            tmp.at(3,1) = ny;
            tmp.at(3,2) = nz;
            *this *= tmp;
        }
        // ADD TRANSLATE ALONG X, Y, Z (VECTOR3)
        void translateXYZ(const vector3<T>& o){
            // WRAP OVER TRANSLATEXYZ(FLOAT, FLOAT, FLOAT)
            translateXYZ(o[0],o[1],o[2]);
        }
        
        // ADD ROTATION IN HEADING (AROUND Y AXIS)
        void rotateH(const T& ht){
            matrix4<T> tmp;
            // GENERATE HEADING ROTATION MATRIX
            tmp.at(0,0) = cos(ht);
            tmp.at(2,0) = sin(ht);
            tmp.at(0,2) = -sin(ht);
            tmp.at(2,2) = cos(ht);
            // APPLY
            *this *= tmp;
        }
        // ADD ROTATION IN PITCH (AROUND X AXIS)
        void rotateP(const T& pt){
            matrix4<T> tmp;
            // GENERATE PITCH ROTATION MATRIX
            tmp.at(1,1) = cos(pt);
            tmp.at(2,1) = -sin(pt);
            tmp.at(1,2) = sin(pt);
            tmp.at(2,2) = cos(pt);
            // APPLY
            *this *= tmp;
        }
        // ADD ROTATION IN ROLL (AROUND Z AXIS)
        void rotateR(const T& rt){
            matrix4<T> tmp;
            // GENERATE ROLL MATRIX
            tmp.at(0,0) = cos(rt);
            tmp.at(0,1) = sin(rt);
            tmp.at(1,0) = -sin(rt);
            tmp.at(1,1) = cos(rt);
            // APPLY
            *this *= tmp;
        }
        // ADD ROTATION IN HEADING, PITCH, ROLL (AROUND Y, X, Z AXES, RESPECTIVELY)
        void rotateHPR(const T& ht,const T& pt,const T& rt){
            // PITCH CHANGE
            rotateP(pt);
            // HEADING CHANGE
            rotateH(ht);
            // ROLL CHANGE
            rotateR(rt);
        }

        // SCALE UNIFORMLY
        void scale(const T& s){
            // WRAPPER OVER SCALE(FLOAT, FLOAT, FLOAT)
            scale(s,s,s);
        }
        // SCALE DIRECTLY
        void scale(const T& xs, const T& ys, const T& zs){
            matrix4<T> tmp;
            // GENERATE SCALAR MATRIX
            tmp.at(0,0) = xs;
            tmp.at(1,1) = ys;
            tmp.at(2,2) = zs;
            // APPLY
            *this *= tmp;
        }
        
        // TRANSFORM ALL FIELDS (TRANSLATEXYZ -> ROTATEHPR -> SCALE (UNIFORM))
        void transformXYZHPRS(const T& nx, const T& ny, const T& nz, const T& ht, const T& pt, const T& rt, const T& s){
            // TRANSLATE
            translateX(nx);
            translateY(ny);
            translateZ(nz);
            // ROTATE
            rotateP(pt);
            rotateH(ht);
            rotateR(rt);
            // SCALE
            scale(s);
        }

        void reset()
        {
            for(size_t r = 0; r < 4; ++r)
                for(size_t c = 0; c < 4; ++c)
                    if(r == c)
                        m_data[4 * r + c] = T(1);
                    else
                        m_data[4 * r + c] = T(0);
        }

    private:
        T m_data[16];
    };
    
    template<class T>
    matrix4<T> operator*(const matrix4<T>& m, const matrix4<T>& o)
    {
        matrix4<T> tmp = o;
        return tmp *= m;
    }
}

#endif
