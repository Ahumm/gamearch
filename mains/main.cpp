#include <iostream>
#include <array>
#include "includes.h"

using namespace std;

class Cube{
 public:
  t_stuff::Matrix4 transform;
  array<t_stuff::Point,8> verts;

  Cube(){ reset(); }

  void reset(){
    verts[0] = { 1, 1, 1, 1};
    verts[1] = { 1, 1,-1, 1};
    verts[2] = { 1,-1, 1, 1};
    verts[3] = { 1,-1,-1, 1};
    verts[4] = {-1, 1, 1, 1};
    verts[5] = {-1, 1,-1, 1};
    verts[6] = {-1,-1, 1, 1};
    verts[7] = {-1,-1,-1, 1};
    transform.reset();
  }

  void print(ostream& out, int tabs = 0){
    string t(tabs,'\t');
    out << t << "Transformation Matrix:\n";
    transform.print(out, t+'\t');
    out << t << "Verticies:\n";
    for(int i = 0; i < 8; ++i){
      out<< t+'\t' << "V" << i << ": ";
      (transform * verts[i]).print(out,3);
      out << endl;
    }
  }
};

int main(){
  Cube c;

  string input = "";
  float i[7] = {0,0,0,0,0,0,0};

  bool changed = true;

  cout << "Initial positioning and Transformation:\n";
  c.print(cout, 1);

  while(1){
    input = "";
    i[0] = i[1] = i[2] = i[3] = i[4] = i[5] = i[6] = 0.0;
    do{
      if(input != "") break;
      cout << "--> ";
      getline(cin, input);
    }while(1);

    changed = true;

    if(input == "exit"){
      cout << "Exiting..." << endl;
      break;
    }
    if(input == "translateX"){
      cout << "Enter 1 number (X): ";
      scanf("%f", &i[0]);
      //cin >> i[0];
      c.transform.translateX(i[0]);
    }
    else if(input == "translateY"){
      cout << "Enter 1 number (Y): ";
      scanf("%f", &i[0]);
      //cin >> i[0];
      c.transform.translateY(i[0]);
    }
    else if(input == "translateZ"){
      cout << "Enter 1 number (Z): ";
      scanf("%f", &i[0]);
      //cin >> i[0];
      c.transform.translateZ(i[0]);
    }
    else if(input == "rotateH"){
      cout << "Enter 1 number (H): ";
      scanf("%f", &i[0]);
      //cin >> i[0];
      c.transform.rotateH(i[0]);
    }
    else if(input == "rotateP"){
      cout << "Enter 1 number (P): ";
      scanf("%f", &i[0]);
      //cin >> i[0];
      c.transform.rotateP(i[0]);
    }
    else if(input == "rotateR"){
      cout << "Enter 1 number (R): ";
      scanf("%f", &i[0]);
      //cin >> i[0];
      c.transform.rotateR(i[0]);
    }
    else if(input == "scale"){
      cout << "uniform or nonuniform? ";
      getline(cin, input);
      if(input == "uniform"){
        cout << "Enter 1 number (S): ";
        scanf("%f", i[0]);
        //cin >> i[0];
        c.transform.scale(i[0]);
      }
      else if(input == "nonuniform"){
        cout << "Enter 3 numbers (SX, SY, SZ): ";
        scanf("%f %f %f", i[0], i[1], i[2]);
        //cin >> i[0] >> i[1] >> i[2];
        c.transform.scale(i[0],i[1],i[2]);
      }
      else{
        cout << "Input not recognized, jumping back..." << endl;
        changed = false;
      }
    }
    else if(input == "translateXYZ"){
      cout << "floats or vector3? ";
      getline(cin, input);
      if(input == "floats"){
        cout << "Enter 3 numbers (X,Y,Z): ";
        scanf("%f %f %f", i[0], i[1], i[2]);
        //cin >> i[0] >> i[1] >> i[2];
        c.transform.translateXYZ(i[0],i[1],i[2]);
      }
      else if(input == "vector3"){
        cout << "Enter 3 numbers (X,Y,Z) (Vectored behind the scenes): ";
        scanf("%f %f %f", i[0], i[1], i[2]);
        //cin >> i[0] >> i[1] >> i[2];
        c.transform.translateXYZ(t_stuff::Vector3(i[0],i[1],i[2]));
      }
      else{
        cout << "Input not recognized, jumping back..." << endl;
        changed = false;
      }
    }
    else if(input == "rotateHPR"){
      cout << "Enter 3 numbers (H,P,R): ";
      scanf("%f %f %f", i[0], i[1], i[2]);
      //cin >> i[0] >> i[1] >> i[2];
      c.transform.rotateHPR(i[0],i[1],i[2]);
    }
    else if(input == "transformXYZHPRS"){
      cout << "Enter 7 numbers (X,Y,Z,H,P,R,S): ";
      scanf("%f %f %f %f %f %f %f", i[0], i[1], i[2], i[3], i[4], i[5], i[6]);
      //cin >> i[0] >> i[1] >> i[2] >> i[3] >> i[4] >> i[5] >> i[6];
      c.transform.transformXYZHPRS(i[0],i[1],i[2],i[3],i[4],i[5],i[6]);
    }
    else if(input == "reset"){ c.reset(); }
    else if(input == "help"){
      cout<< "Available commands: (Note: Must match exactly)\n\treset :: exit :: help :: print ::\n\ttranslateX :: translateY :: translateZ :: translateXYZ ::\n\trotateH :: rotateP :: rotateR :: rotateHPR ::\n\tscale ::\n\ttransformXYZHPRS"<<endl;
      changed = false;
    }
    else if(input == "print"){}
    else{
      changed = false;
      cout << "Input not recognized, enter \"help\" for options."<<endl;
    }
    if(changed) c.print(cout, 1);
    
    cout<< endl; /* << "\tinput: " << input << "\n\tchanged: " << changed
        << "\n\ti[0]: " << i[0]
        << "\n\ti[1]: " << i[1]
        << "\n\ti[2]: " << i[2]
        << "\n\ti[3]: " << i[3]
        << "\n\ti[4]: " << i[4]
        << "\n\ti[5]: " << i[5]
        << "\n\ti[6]: " << i[6] << endl;*/
  }
}
