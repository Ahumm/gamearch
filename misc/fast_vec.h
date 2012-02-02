#ifndef _FAST_VEC_H_
#define _FAST_VEC_H

#include <iostream>

namespace fast_vec{
    class Base{
    public:
        Base();
        Base(const float x, const float y, const float z, const float w);
        Base(const Base& b);

        float& at(const size_t i);
        const float& at(const size_t i) const;
        float& operator[](const size_t i);
        const float& operator[](const size_t i) const;

        void print(std::ostream& out);
        void print3(std::ostream& out);

    protected:
        float b_data[4];
    };
    
    class Vector3 : public Base{
    public:
        Vector3();
        Vector3(const float x, const float y, const float z);
        Vector3(const Vector3& o);

        float dot(const Vector3& o) const;
        Vector3 cross(const Vector3& o) const;
        float magnitude() const;
        Vector3 normalized() const;
        void normalize();

        Vector3& operator=(const Vector3& o);
    };

    class Point : public Base {
    public:
        Point();
        Point(const float x, const float y, const float z);
        Point(const Point& o);

        Point& operator=(const Point& o);
    };

    Vector3 operator+(const Vector3& a, const Vector3& b);
    Vector3 operator-(const Vector3& a, const Vector3& b);
    Point operator+(const Point& a, const Vector3& b);
    Point operator-(const Point& a, const Vector3& b);
    Vector3 operator-(const Point& v1, const Point& b);

    
}

#endif
