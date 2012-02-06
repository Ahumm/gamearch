#ifndef _SQT_H_
#define _SQT_H_

#include "vector3.h"
#include "mvp_math.h"
#include "quaternion.h"
#include <iostream>

namespace mvp
{
    class sqt
    {
    public:
        // CONSTRUCT BASE SQT
        sqt()
        {
            s_s = 1.0;
            s_q = quaternion();
            s_t = vector3<float>();
        }
        // BUILD MORE COMPLEX SQT
        sqt(const float& s, const quaternion& q, const vector3<float>& t)
        {
            s_s = s;
            s_q = q;
            s_t = t;
        }
        // COPY CONSTRUCTOR
        sqt(const sqt& s)
        {
            s_s = s.s_s;
            s_q = s.s_q;
            s_t = s.s_t;
        }

        // RESET SQT TO BASE VALUES
        void reset()
        {
            sqt t;
            *this = t;
        }

        // PRINT (FOR DEBUG AND DISPLAY)
        void print(std::ostream& out, std::string t) const
        {
            std::streamsize tp = out.precision(5);
            out << std::fixed << t+"Scale:      " << s_s << std::endl;
            out << t+"Quaternion: ";
            s_q.print(out);
            out << std::endl;
            out << t+"Translate:  ";
            s_t.print(out);
            out << std::endl;
            out.precision(tp);
        } 

        // ASSINGMENT OPERATOR
        sqt& operator=(const sqt& s)
        {
            s_s = s.s_s;
            s_q = s.s_q;
            s_t = s.s_t;
            return *this;
        }

        // ALLPY TRANSFORMATION TO A POINT
        point<float> operator*(const point<float>& p) const
        {
            point<float> tmp;
            tmp.at(0) = s_t.at(0) + p.at(0);
            tmp.at(1) = s_t.at(1) + p.at(1);
            tmp.at(2) = s_t.at(2) + p.at(2);
            tmp = s_q.to_matrix() * tmp;
            tmp *= s_s;
            return tmp;
        }

        // SCALE UNIFORMLY
        void scale(const float& s) { s_s *= s; }

        // TRANSLATE ALONG AXES
        void translateX(const float& x){ s_t += vector3<float>(  x,0.0,0.0); }
        void translateY(const float& y){ s_t += vector3<float>(0.0,  y,0.0); }
        void translateZ(const float& z){ s_t += vector3<float>(0.0,0.0,  z); }
        void translateXYZ(const float& x, const float& y, const float& z){ s_t += vector3<float>(x,y,z); }
        void translateXYZ(const vector3<float>& v) { s_t *= v; }

        // ROTATE ABOUT AXES
        void rotateH(const float& ht)
        {
            s_q *= quaternion(0.0, sin(ht / 2.0), 0.0, cos(ht / 2.0));
        }
        void rotateP(const float& pt)
        {
            s_q *= quaternion(sin(pt / 2.0), 0.0, 0.0, cos(pt / 2.0));
        }
        void rotateR(const float& rt)
        {
            s_q *= quaternion(0.0, 0.0, sin(rt / 2.0), cos(rt / 2.0));
        }
        void rotateHPR(const float& ht, const float& pt, const float& rt)
        {
            rotateP(pt);
            rotateH(ht);
            rotateR(rt);
        }

        // TRANSFORM EVERYTHING
        void transformXYZHPRS(const float& x, const float& y, const float & z, const float& ht, const float& pt, const float& rt, const float& s)
        {
            translateXYZ(x,y,z);
            rotateHPR(ht,pt,rt);
            scale(s);
        }

    private:
        vector3<float> s_t;
        float s_s;
        quaternion s_q;
    };
}

#endif
