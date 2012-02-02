#include "fast_vec.h"
#include <iostream>
#include <math.h>

using namespace fast_vec;

Base::Base(){
    b_data[0] = b_data[1] = b_data[2] = b_data[3] = 0;
}
Base::Base(const float x, const float y, const float z, const float w){
    b_data[0] = x;
    b_data[1] = y;
    b_data[2] = z;
    b_data[3] = w; 
}
Base::Base(const Base& b){
    b_data[0] = b.b_data[0];
    b_data[1] = b.b_data[1];
    b_data[2] = b.b_data[2];
    b_data[3] = b.b_data[3]; 
}

float& Base::at(const size_t i){ return b_data[i]; }
const float& Base::at(const size_t i) const { return b_data[i]; }
float& Base::operator[](const size_t i) { return b_data[i]; }
const float& Base::operator[](const size_t i) const { return b_data[i]; }

void Base::print(std::ostream& out){
    // MODIFY PRECISION
    std::streamsize tp = out.precision(5);
    out << std::fixed << '('
        << b_data[0] << " , " << b_data[1] << " , "
        << b_data[2] << " , " << b_data[3] <<  ')' << std::endl;
    // RESTORE PRECISION
    out.precision(tp);
}
void Base::print3(std::ostream& out){
    // MODIFY PRECISION
    std::streamsize tp = out.precision(5);
    out << std::fixed << '(' 
        << b_data[0] << " , "
        << b_data[1] << " , "
        << b_data[2] <<  ')'
        << std::endl;
    // RESTORE PRECISION
    out.precision(tp);
}


Vector3::Vector3() : Base() {}
Vector3::Vector3(const float x, const float y, const float z)
    : Base(x,y,z,0.0) {}
Vector3::Vector3(const Vector3& o) : Base(o) {}

float Vector3::dot(const Vector3& o) const {
    return this->b_data[0] * o.b_data[0] +
           this->b_data[1] * o.b_data[1] +
           this->b_data[2] * o.b_data[2];
}

Vector3 Vector3::cross(const Vector3& o) const {
    return Vector3((this->b_data[1] * o.b_data[2]) - (this->b_data[2] * o.b_data[1]),
                   (this->b_data[2] * o.b_data[0]) - (this->b_data[0] * o.b_data[2]),
                   (this->b_data[0] * o.b_data[1]) - (this->b_data[1] * o.b_data[0]));
}

float Vector3::magnitude() const {
    return sqrt(this->dot(*this));
}

Vector3 Vector3::normalized() const {
    float mag = magnitude();
    return Vector3(this->b_data[0] / mag,
                   this->b_data[1] / mag,
                   this->b_data[2] / mag);
}

void Vector3::normalize(){
    float mag = magnitude();
    this->b_data[0] = this->b_data[0] / mag;
    this->b_data[1] = this->b_data[1] / mag;
    this->b_data[2] = this->b_data[2] / mag;
}

Vector3& Vector3::operator=(const Vector3& o){
    if(this == &o)
        return *this;
    this->b_data[0] = o.b_data[0];
    this->b_data[1] = o.b_data[1];
    this->b_data[2] = o.b_data[2];
    this->b_data[3] = o.b_data[3];
    return *this;
}


Point::Point() : Base() { this->b_data[3] = 1.0; }
Point::Point(const float x, const float y, const float z) : Base(x,y,z,1.0) {}
Point::Point(const Point& o) : Base(o) {}

Point& Point::operator=(const Point& o){
    if(this == &o)
        return *this;
    this->b_data[0] = o.b_data[0];
    this->b_data[1] = o.b_data[1];
    this->b_data[2] = o.b_data[2];
    this->b_data[3] = o.b_data[3];
    return *this;
}

Vector3 operator+(const Vector3& a, const Vector3& b){
    return Vector3(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}
Vector3 operator-(const Vector3& a, const Vector3& b){
    return Vector3(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}
Point operator+(const Point& a, const Vector3& b){
    return Point(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}
Point operator-(const Point& a, const Vector3& b){
    return Point(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}
Vector3 operator-(const Point& a, const Point& b){
    return Vector3(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}
