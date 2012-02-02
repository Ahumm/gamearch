#include "fast_vec.h"
#include <iostream>

using namespace std;

int main(){
    fast_vec::Vector3 v0,v1,v2,v3,v4,v5,v6,v7,v8,v9;
    v0.print(cout);
    v1 = fast_vec::Vector3(1,2,3);
    v2 = fast_vec::Vector3(4,5,6);
    v2 = v1.cross(v2);
    cout << v1.dot(v2) <<endl;
    v1.print(cout);
    v2.print(cout);
}
