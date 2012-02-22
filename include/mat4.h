#ifndef _MAT4_H_
#define _MAT4_H_

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include <math.h>

namespace mvp
{
    class mat4
    {
    public:
        
        //////////////////
        // CONSTRUCTORS //
        //////////////////
        
        mat4()
        {
            for(size_t r = 0; r < 4; ++r)
                for(size_t c = 0; c < 4; ++c)
                    if(r == c)
                        m_data[4 * r + c] = 1.0f;
                    else
                        m_data[4 * r + c] = 0.0f;
        }
        mat4(const float* arr)
        {
            for(size_t i = 0; i < 16; ++i)
                m_data[i] = arr[i];
        }
        mat4(const mat4& m)
        {
            for(size_t i = 0; i < 16; ++i)
                m_data[i] = m.m_data[i];
        }

        ///////////
        // PRINT //
        ///////////

        // PRINT MATRIX TO OUT (| BORDER)
        void print(std::ostream& out) const
        {
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
        void print(std::ostream& out, std::string s) const
        {
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

        ///////////////
        // ACCESSORS //
        ///////////////

        const float* value_ptr() const
        {
            return m_data;
        }
        
        float& at(const size_t c, const size_t r) { return m_data[4 * r + c]; }
        const float& at(const size_t c, const size_t r) const { return m_data[4 * r + c]; }

        ///////////////
        // OPERATORS //
        ///////////////

        mat4& operator=(const mat4& o)
        {
            for(size_t i = 0; i < 16; ++i)
                m_data[i] = o.m_data[i];
        }
        mat4& operator=(const float* arr)
        {
            for(size_t i = 0; i < 16; ++i)
                m_data[i] = arr[i];
        }
        mat4& operator*=(const mat4& o)
        {
            float tmp[16];
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
            *this = mat4(tmp);
            return *this;
        }
        
        vec4 operator*(const vec4& o) const
        {
            vec4 tmp;
            tmp[0] = o.at(0) * at(0,0) + o.at(1) * at(1,0) + o.at(2) * at(2,0) + o.at(3) * at(3,0);
            tmp[1] = o.at(0) * at(0,1) + o.at(1) * at(1,1) + o.at(2) * at(2,1) + o.at(3) * at(3,1);
            tmp[2] = o.at(0) * at(0,2) + o.at(1) * at(1,2) + o.at(2) * at(2,2) + o.at(3) * at(3,2);
            tmp[3] = o.at(0) * at(0,3) + o.at(1) * at(1,3) + o.at(2) * at(2,3) + o.at(3) * at(3,3);
            return tmp;
        }

        /////////////////////
        // TRANSFORMATIONS //
        /////////////////////

        // ADD TRANSLATE ALONG X
        void translateX(const float& nx){
            mat4 tmp;
            // GENERATE TRANSLATIONX MATRIX, THEN APPLY
            tmp.at(3,0) = nx;
            *this *= tmp;
        }
        // ADD TRANSLATE ALONG Y
        void translateY(const float& ny){
            mat4 tmp;
            // GENERATE TRANSLATIONY MATRIX, THEN APPLY
            tmp.at(3,1) = ny;
            *this *= tmp;
        }
        // ADD TRANSLATE ALONG Z
        void translateZ(const float& nz){
            mat4 tmp;
            // GENERATE TRANSLATIONZ MATRIX, THEN APPLY
            tmp.at(3,2) = nz;
            *this *= tmp;
        }
        // ADD TRANSLATE ALONG X, Y, AND Z (FLOATS)
        void translateXYZ(const float& nx, const float& ny, const float& nz){
            mat4 tmp;
            // GENERATE TRANSLATEXYZ MATRIX, THEN APPLY
            tmp.at(3,0) = nx;
            tmp.at(3,1) = ny;
            tmp.at(3,2) = nz;
            *this *= tmp;
        }
        // ADD TRANSLATE ALONG X, Y, Z (VECTOR3)
        void translateXYZ(const vec3& o){
            // WRAP OVER TRANSLATEXYZ(FLOAT, FLOAT, FLOAT)
            translateXYZ(o[0],o[1],o[2]);
        }
        
        // ADD ROTATION IN HEADING (AROUND Y AXIS)
        void rotateH(const float& ht){
            mat4 tmp;
            // GENERATE HEADING ROTATION MATRIX
            tmp.at(0,0) = cos(ht);
            tmp.at(2,0) = sin(ht);
            tmp.at(0,2) = -sin(ht);
            tmp.at(2,2) = cos(ht);
            // APPLY
            *this *= tmp;
        }
        // ADD ROTATION IN PITCH (AROUND X AXIS)
        void rotateP(const float& pt){
            mat4 tmp;
            // GENERATE PITCH ROTATION MATRIX
            tmp.at(1,1) = cos(pt);
            tmp.at(2,1) = -sin(pt);
            tmp.at(1,2) = sin(pt);
            tmp.at(2,2) = cos(pt);
            // APPLY
            *this *= tmp;
        }
        // ADD ROTATION IN ROLL (AROUND Z AXIS)
        void rotateR(const float& rt){
            mat4 tmp;
            // GENERATE ROLL MATRIX
            tmp.at(0,0) = cos(rt);
            tmp.at(0,1) = sin(rt);
            tmp.at(1,0) = -sin(rt);
            tmp.at(1,1) = cos(rt);
            // APPLY
            *this *= tmp;
        }
        // ADD ROTATION IN HEADING, PITCH, ROLL (AROUND Y, X, Z AXES, RESPECTIVELY)
        void rotateHPR(const float& ht,const float& pt,const float& rt){
            // PITCH CHANGE
            rotateP(pt);
            // HEADING CHANGE
            rotateH(ht);
            // ROLL CHANGE
            rotateR(rt);
        }

        // SCALE UNIFORMLY
        void scale(const float& s){
            // WRAPPER OVER SCALE(FLOAT, FLOAT, FLOAT)
            scale(s,s,s);
        }
        // SCALE DIRECTLY
        void scale(const float& xs, const float& ys, const float& zs){
            mat4 tmp;
            // GENERATE SCALAR MATRIX
            tmp.at(0,0) = xs;
            tmp.at(1,1) = ys;
            tmp.at(2,2) = zs;
            // APPLY
            *this *= tmp;
        }
        
        // TRANSFORM ALL FIELDS (TRANSLATEXYZ -> ROTATEHPR -> SCALE (UNIFORM))
        void transformXYZHPRS(const float& nx, const float& ny, const float& nz, const float& ht, const float& pt, const float& rt, const float& s){
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

        // RESET TO IDENTITY MATRIX
        void reset()
        {
            for(size_t r = 0; r < 4; ++r)
                for(size_t c = 0; c < 4; ++c)
                    if(r == c)
                        m_data[4 * r + c] = 1.0f;
                    else
                        m_data[4 * r + c] = 0.0f;
        }

    private:
        float m_data[16];
    };
    
    // MULTIPLICATION OF TWO MATRICIES
    mat4 operator*(const mat4& m, const mat4& o)
    {
        mat4 tmp = o;
        return tmp *= m;
    }
}

#endif
