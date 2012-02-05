#include <iostream>
//#include <array>
#include "vector3.h"
#include "point.h"
#include "matrix4.h"

using namespace std;

class Cube{
 public:
    mvp::matrix4<float> transform;
    mvp::point<float> verts[8];

    Cube(){ reset(); }

    void reset(){
        verts[0].set( 1, 1, 1);
        verts[1].set( 1, 1,-1);
        verts[2].set( 1,-1, 1);
        verts[3].set( 1,-1,-1);
        verts[4].set(-1, 1, 1);
        verts[5].set(-1, 1,-1);
        verts[6].set(-1,-1, 1);
        verts[7].set(-1,-1,-1);
        transform.reset();
  }

  void print(ostream& out, int tabs = 0){
    string t(tabs,'\t');
    out << t << "Transformation Matrix:\n";
    transform.print(out, t+'\t');
    out << t << "Verticies:\n";
    for(size_t i = 0; i < 8; ++i){
      out<< t+'\t' << "V" << i << ": ";
      (transform * verts[i]).print(out);
    }
  }
};

int main(){
  Cube c;

  string input = "";
  float i[7] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};

  bool changed = true;

  cout << "Initial positioning and Transformation:\n";
  c.print(cout, 1);

  while(1){
    input = "";
    i[0] = i[1] = i[2] = i[3] = i[4] = i[5] = i[6] = 0.0;
    do{
      if(input != "") break;
      cout << "--> ";
      cin >> input;
    }while(1);

    changed = true;

    if(input == "exit"){
      cout << "Exiting..." << endl;
      break;
    }
    if(input == "translateX"){
      cin >> i[0];
      c.transform.translateX(i[0]);
    }
    else if(input == "translateY"){
      cin >> i[0];
      c.transform.translateY(i[0]);
    }
    else if(input == "translateZ"){
      cin >> i[0];
      c.transform.translateZ(i[0]);
    }
    else if(input == "rotateH"){
      cin >> i[0];
      c.transform.rotateH(i[0]);
    }
    else if(input == "rotateP"){
      cin >> i[0];
      c.transform.rotateP(i[0]);
    }
    else if(input == "rotateR"){
      cin >> i[0];
      c.transform.rotateR(i[0]);
    }
    else if(input == "scale"){
      cin >> i[0];
      c.transform.scale(i[0]);
    }
    else if(input == "translateXYZ"){
      cin >> i[0] >> i[1] >> i[2];
      c.transform.translateXYZ(i[0],i[1],i[2]);
    }
    else if(input == "translateXYZ-V"){
      cin >> i[0] >> i[1] >> i[2];
      c.transform.translateXYZ(mvp::vector3<float>(i[0],i[1],i[2]));
    }
    else if(input == "rotateHPR"){
      cin >> i[0] >> i[1] >> i[2];
      c.transform.rotateHPR(i[0],i[1],i[2]);
    }
    else if(input == "transformXYZHPRS"){
      cin >> i[0] >> i[1] >> i[2] >> i[3] >> i[4] >> i[5] >> i[6];
      c.transform.transformXYZHPRS(i[0],i[1],i[2],i[3],i[4],i[5],i[6]);
    }
    else if(input == "reset"){ c.reset(); }
    else if(input == "help"){
      cout<< "\tAvailable commands: (Note: Must match exactly)\n\t\treset :: exit :: help :: print ::\n\t\ttranslateX <float> :: translateY <float> :: translateZ <float> ::\n\t\ttranslateXYZ <float> <float> <float> ::\n\t\ttranslateXYZ-V <float> <float> <float ::\n\t\trotateH <float> :: rotateP <float> :: rotateR <float> ::\n\t\trotateHPR <float> <float> <float> :: scale <float> ::\n\t\ttransformXYZHPRS <float> <float> <float> <float> <float> <float> <float>"<<endl;
      changed = false;
    }
    else if(input == "print"){}
    else{
      changed = false;
      cout << "Input not recognized, enter \"help\" for options."<<endl;
    }
    if(changed) c.print(cout, 1);

    // FLUSH THE LINE
    getline(cin, input);

    cout<< endl;
  }
}
