#ifndef _MATRIX4_H_
#define _MATRIX4_H_

//#include <array>
#include <iostream>
#include <math.h>

namespace mvp {
    // GENERIC NxM MATRIX<T>
    template <class T>
        class Matrix{
    public:
        // NxM CONSTRUCTOR
        Matrix(size_t c, size_t r){
            rows = r;
            cols = c;
            // CREATE ARRAY AND INITIALIZE TO ALL ZEROS
            m_data = new T[rows*cols];
            for(size_t i = 0; i < rows*cols; ++i){
                m_data[i] = 0;
            }
        }

        // COPY CONSTRUCTOR
        Matrix(const Matrix<T>& o){
            rows = o.rows;
            cols = o.cols;
            // CREATE ARRAY AND COPY FROM O
            m_data = new T[rows*cols];
            for(size_t i = 0; i < rows*cols; ++i){
                m_data[i] = o.m_data[i];
            }
        }

        // DESTRUCTOR
        ~Matrix(){
            if(m_data){
                delete m_data;
                m_data = 0;
            }
        }
        
        // PRINT MATRIX TO OUT (| BORDER)
        void print(std::ostream& out){
            // MODIFY PRECISION
            std::streamsize tp = out.precision(5);
            for(int r = 0; r < rows; ++r){
                out<<"\t| ";
                for (size_t c = 0; c < cols; ++c){
                    if(m_data[rows*r+c] >= 0) out << ' ';
                    out << std::fixed << m_data[rows*r+c] << " ";
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
            for(int r = 0; r < rows; ++r){
                out << s << "| ";
                for (size_t c = 0; c < cols; ++c){
                    if(m_data[rows*r+c] >= 0) out << ' ';
                    out << std::fixed << m_data[rows*r+c] << " ";
                }
                out << "|" << std::endl;
            }
            // RESTORE PRECISION
            out.precision(tp);
        }
        
        // ASSIGNMENT: NxM MATRIX<T> -> NxM MATRIX<T>
        Matrix<T>& operator=(const Matrix<T>& o){
            if(this == &o)
                return *this;
            rows = o.rows;
            cols = o.cols;
            // CLEAN UP
            if(m_data){
                delete m_data;
                m_data = 0;
            }
            // ALLOCATE NEW ARRAY
            m_data = new T[rows*cols];
            // COPY FROM O
            for(size_t i = 0; i < rows*cols; ++i){
                m_data[i] = o.m_data[i];
            }
            return *this;
        }
        
        // ACCESSORS
        T& at(const int c, const int r){ return m_data[cols*r+c]; }
        const T& at(const int c, const int r) const { return m_data[cols*r+c]; }
        
        // MULTIPLICATION ASSIGNMENT: MULTIPLY NxM MATRIX<T> BY OxN MATRIX<T>, STORE RESULT MxO MATRIX<T> AS THIS
        Matrix<T>& operator*=(const Matrix<T>& o){
            size_t r1 = rows;
            size_t c1 = cols;
            size_t c2 = o.cols;

            //            if(c1 != o.rows){
                
            // TMP HOLDER
            Matrix<T> tmp(r1,c2);
            // START OF THIS
            const T* p1 = &m_data[0];
            // START OF O
            const T* p2 = &o.m_data[0];
            // START OF TMP
            T* ptmp = &tmp.m_data[0];

            for(size_t r = 0; r < r1; ++r){
                for(size_t c = 0; c < c2; ++c){
                    // INITIALIZE TO 0
                    ptmp[r*c2+c] = T(0);
                    // ESSENTIALLY DOT ROW R FROM THIS WITH COL C OF O
                    for(size_t tc = 0; tc < c1; ++tc){
                        ptmp[r*c2+c] += p1[r*c1+tc] * p2[tc*c2+c];
                    }
                }
            }
            *this = tmp;
            return *this;
        }
        
    protected:
        T* m_data;
        size_t rows;
        size_t cols;
    };

    // MULTIPLICATION OPERATOR: NxM MATRIX<T> * OxP MATRIX<T> = MxO MATRIX<T>
    template<class T>
    Matrix<T>& operator*(const Matrix<T>& o1, const Matrix<T>& o2){
        // COPY SO AS NOT TO OVERWRITE
        Matrix<T> m = o1;
        // WRAP OVER MULTIPLICATION ASSIGNMENT
        return m *= 02;
    }


    // EXPLICIT 4x4 MATRIX<FLOAT>
    class Matrix4 : public Matrix<float>{
     public:
        // DEFAULT CONSTRUCTOR
        Matrix4() : Matrix<float>(4,4) {
            for(size_t i = 0; i < 4; ++i){
                this->m_data[5*i] = 1;
            }
        }
        // STD::INITIALIZER_LIST CONSTRUCTOR (PAD: 0)
        Matrix4(const float* s) : Matrix<float>(4,4) {
            size_t i;
            for(i = 0; i < 16; ++i)
                this->m_data[i] = s[i];
        }
        // COPY CONSTRUCTOR
        Matrix4(const Matrix4& o) : Matrix<float>(4,4) {
            this->rows = 4;
            this->cols = 4;
            if(this->m_data){
                delete this->m_data;
                this->m_data = 0;
            }
            this->m_data = new float[16];
            for(size_t i = 0; i < 16; ++i)
                this->m_data[i] = o.m_data[i];
        }

        // DESTRUCTOR
        ~Matrix4(){
            delete this->m_data;
            this->m_data = 0;
        }

        // MULTIPLY MATRIX4 THIS BY MATRIX4 O, RETURNS MATRIX4. SEE MATRIX<T>::OPERATOR*(MATRIX<T>) FOR COMMENTS
        Matrix4 operator*(const Matrix4& o){
            Matrix4 tmp;
            const float* p1 = &this->m_data[0];
            const float* p2 = &o.m_data[0];
            float* ptmp = &tmp.m_data[0];

            for(size_t r = 0; r < 4; ++r){
                for(size_t c = 0; c < 4; ++c){
                    ptmp[r*4+c] = 0.0;
                    for(size_t tc = 0; tc < 4; ++tc){
                        ptmp[r*4+c] += p1[r*4+tc] * p2[tc*4+c];
                    }
                }
            }
            return tmp;
        }

        // MULTIPLY MATRIX4 THIS BY VECTOR3 O, RETURNS VECTOR3
        Vector3 operator*(const Vector3& o){
            // INITIALIZE
            Vector3 tmp(0,0,0,0);

            // COMPUTE ROW BY ROW CROSS PRODUCT WITH VECTOR3 O
            for(size_t r = 0; r < 4; ++r){
                for(size_t c = 0; c < 4; ++c){
                    tmp[r] += o[c] * this->m_data[4*r+c];
                }
            }
            return tmp;
        }

        // MULTIPLY MATRIX4 THIS BY POINT 0, RETURNS POINT
        Point operator*(const Point& o){
            // INITIALIZE
            Point tmp(0,0,0,1);
            
            // COMPUTE ROW BY ROW CROSS PRODUCT WITH POINT O
            for(size_t r = 0; r < 4; ++r){
                for(size_t c = 0; c < 4; ++c){
                    tmp[r] += o[c] * this->m_data[4*r+c];
                }
            }
            return tmp;
        }

        // ASSINGMENT: STD::INITIALIZERLIST<FLOAT> -> MATRIX4 (PAD: 0)
        Matrix4& operator=(const float* s){
            // CLEAN UP
            if(this->m_data){
                delete this->m_data;
                this->m_data = 0;
            }
            // ALLOCATE NEW ARRAY
            this->m_data = new float[16];
            // COPY IN
            size_t i;
            for(i = 0; i < 16; ++i)
                this->m_data[i] = s[i];
            return *this;
        }
        
        // ASSIGNMENT: MATRIX4 -> MATRIX4
        Matrix4& operator=(const Matrix4& o){
            if(this == &o)
                return *this;
            // SET LEGACY VARIABLES
            this->rows = 4;
            this->cols = 4;
            // CLEAN UP
            if(this->m_data){
                delete this->m_data;
                this->m_data = 0;
            }
            // ALLOCATE NEW ARRAY
            this->m_data = new float[16];
            // COPY IN
            for(size_t i = 0; i < 16; ++i)
                this->m_data[i] = o.m_data[i];
            return *this;
        }

        // ACCESSORS
        float& at(const size_t c, const size_t r){ return this->m_data[cols*r+c]; }
        const float& at(const size_t c, const size_t r) const { return this->m_data[cols*r+c]; }
      


        /********************************
         * T  R  A  N  S  F  O  R  M  S *
         ********************************/



        // ADD TRANSLATE ALONG X
        void translateX(const float nx){
            Matrix4 tmp;
            // GENERATE TRANSLATIONX MATRIX, THEN APPLY
            tmp.at(3,0) = nx;
            copyin(*this * tmp);
        }
        // ADD TRANSLATE ALONG Y
        void translateY(const float ny){
            Matrix4 tmp;
            // GENERATE TRANSLATIONY MATRIX, THEN APPLY
            tmp.at(3,1) = ny;
            copyin(*this * tmp);
        }
        // ADD TRANSLATE ALONG Z
        void translateZ(const float nz){
            Matrix4 tmp;
            // GENERATE TRANSLATIONZ MATRIX, THEN APPLY
            tmp.at(3,2) = nz;
            copyin(*this * tmp);
        }
        // ADD TRANSLATE ALONG X, Y, AND Z (FLOATS)
        void translateXYZ(const float nx, const float ny, const float nz){
            Matrix4 tmp;
            // GENERATE TRANSLATEXYZ MATRIX, THEN APPLY
            tmp.at(3,0) = nx;
            tmp.at(3,1) = ny;
            tmp.at(3,2) = nz;
            copyin(*this * tmp);
        }
        // ADD TRANSLATE ALONG X, Y, Z (VECTOR3)
        void translateXYZ(const Vector3& o){
            // WRAP OVER TRANSLATEXYZ(FLOAT, FLOAT, FLOAT)
            translateXYZ(o[0],o[1],o[2]);
        }
        
        // ADD ROTATION IN HEADING (AROUND Y AXIS)
        void rotateH(const float ht){
            Matrix4 tmp;
            // GENERATE HEADING ROTATION MATRIX
            tmp.at(0,0) = cos(ht);
            tmp.at(2,0) = sin(ht);
            tmp.at(0,2) = -sin(ht);
            tmp.at(2,2) = cos(ht);
            // APPLY
            copyin(*this * tmp);
        }
        // ADD ROTATION IN PITCH (AROUND X AXIS)
        void rotateP(const float pt){
            Matrix4 tmp;
            // GENERATE PITCH ROTATION MATRIX
            tmp.at(1,1) = cos(pt);
            tmp.at(2,1) = -sin(pt);
            tmp.at(1,2) = sin(pt);
            tmp.at(2,2) = cos(pt);
            // APPLY
            copyin(*this * tmp);
        }
        // ADD ROTATION IN ROLL (AROUND Z AXIS)
        void rotateR(const float rt){
            Matrix4 tmp;
            // GENERATE ROLL MATRIX
            tmp.at(0,0) = cos(rt);
            tmp.at(0,1) = sin(rt);
            tmp.at(1,0) = -sin(rt);
            tmp.at(1,1) = cos(rt);
            // APPLY
            copyin(*this * tmp);
        }
        // ADD ROTATION IN HEADING, PITCH, ROLL (AROUND Y, X, Z AXES, RESPECTIVELY)
        void rotateHPR(const float ht,const float pt,const float rt){
            Matrix4 tmp1, tmp2, tmp3;
            // GENERATE HEADING ROTATION MATRIX
            tmp1.at(0,0) = cos(ht);
            tmp1.at(2,0) = sin(ht);
            tmp1.at(0,2) = -sin(ht);
            tmp1.at(2,2) = cos(ht);
            // GENERATE PITCH ROTATION MATRIX
            tmp2.at(1,1) = cos(pt);
            tmp2.at(2,1) = -sin(pt);
            tmp2.at(1,2) = sin(pt);
            tmp2.at(2,2) = cos(pt);
            // GENERATE ROLL ROTATION MATRIX
            tmp3.at(0,0) = cos(rt);
            tmp3.at(0,1) = sin(rt);
            tmp3.at(1,0) = -sin(rt);
            tmp3.at(1,1) = cos(rt);
            // APPLY TMP1, TMP2, AND TMP3 TO EXISTING MATRIX4
            copyin(*this * tmp1 * tmp2 * tmp3);
        }

        // SCALE UNIFORMLY
        void scale(const float s){
            // WRAPPER OVER SCALE(FLOAT, FLOAT, FLOAT)
            scale(s,s,s);
        }
        // SCALE DIRECTLY
        void scale(const float xs, const float ys, const float zs){
            Matrix4 tmp;
            // GENERATE SCALAR MATRIX
            tmp.at(0,0) = xs;
            tmp.at(1,1) = ys;
            tmp.at(2,2) = zs;
            // APPLY
            copyin(*this * tmp);
        }
        
        // TRANSFORM ALL FIELDS (TRANSLATEXYZ -> ROTATEHPR -> SCALE (UNIFORM))
        void transformXYZHPRS(const float nx, const float ny, const float nz, const float ht, const float pt, const float rt, const float s){
            Matrix4 tmp1, tmp2, tmp3;
            // GENERATE TRANSLATION MATRIX
            tmp1.at(3,0) = nx;       // X TRANSLATION
            tmp1.at(3,1) = ny;       // Y TRANSLATION
            tmp1.at(3,2) = nx;       // Z TRANSLATION
            // APPLY TMP1 THEN CLEAR TMP1
            copyin(*this * tmp1);
            tmp1.reset();
            // GENERATE ROTATION MATRICIES
            tmp1.at(0,0) = cos(ht);  // HEADING
            tmp1.at(2,0) = sin(ht);  // HEADING
            tmp1.at(0,2) = -sin(ht); // HEADING
            tmp1.at(2,2) = cos(ht);  // HEADING
            tmp2.at(1,1) = cos(pt);  // PITCH
            tmp2.at(2,1) = -sin(pt); // PITCH
            tmp2.at(1,2) = sin(pt);  // PITCH
            tmp2.at(2,2) = cos(pt);  // PITCH
            tmp3.at(0,0) = cos(rt);  // ROLL
            tmp3.at(0,1) = sin(rt);  // ROLL
            tmp3.at(1,0) = -sin(rt); // ROLL
            tmp3.at(1,1) = cos(rt);  // ROLL
            // APPLY TMP1, TMP2, AND TMP3 THEN CLEAR TMP1
            copyin(*this * tmp1 * tmp2 * tmp3);
            tmp1.reset();
            // GENERATE SCALAR MATRIX
            tmp1.at(0,0) = s;       // X SCALAR
            tmp1.at(1,1) = s;       // Y SCALAR
            tmp1.at(2,2) = s;       // Z SCALAR
            // APPLY TMP1
            copyin(*this * tmp1);
        }

        // RESET THIS TO IDENTITY MATRIX4
        void reset(){
            // CLEAN UP
            if(this->m_data){
                delete this->m_data;
                this->m_data = 0;
            }
            // ALLOCATE NEW ARRAY
            this->m_data = new float[16];
            // INITIALIZE TO IDENTITY MATRIX4
            for(size_t r = 0; r < 4; ++r)
                for(size_t c = 0; c< 4; ++c)
                    if(r==c) this->m_data[4*r+c] = 1;
                    else this->m_data[4*r+c] = 0;
        }
        
        // COPY WITH NO ALLOCATION
        void copyin(const Matrix4& o){
            for(size_t i = 0; i < 16; ++i)
                this->m_data[i] = o.m_data[i];
        }
    };
}

#endif
