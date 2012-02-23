#ifndef _EGG_MODEL_H_
#define _EGG_MODEL_H_

#include "vec3.h"
#include "vec2.h"
#include "vec4.h"
#include "mat4.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

namespace mvp {
    typedef struct {
        vec4 position;
        vec2 uv;
        vec4 normal;
    } vert;
    
    class egg_model
    {
    public:
        egg_model()
        {
            texture = "";
        }
        
        egg_model(const char * path)
        {
            texture = "";
            load_model(path);
            cout << "Verts: " << verticies.size() << endl;
            cout << "Polys: " << polygons.size() << endl;
            cout << "vec2 Size: " << sizeof(vec2) << endl;
            cout << "vec3 Size: " << sizeof(vec3) << endl;
            cout << "vec4 Size: " << sizeof(vec4) << endl;
            cout << "mat4 Size: " << sizeof(mat4) << endl;
        }
        
        ~egg_model(){}
        
        vector<vert> vertices;
        //vector<mvp::vec3> verticies;
        //vector<mvp::vec3> normals;
        //vector<mvp::vec2> uvs;
        vector<int> polygons;
        string texture;
        
        void load_model(const char * path)
        {
            ifstream file;
            file.open(path);
            if(!file.is_open()){
                cout << "File " << path << " could not be opened" << endl;
                return;
            }
            while(!file.eof())
            {
                string line;
                string tmp;
                string cmd;
                
                getline(file,line);
                size_t s = line.find("<"), e = line.find(">");
                if(s == string::npos || e == string::npos) continue;
                
                cmd = line.substr(s,e-s+1);
                
                if(cmd == "<Texture>")
                {
                    getline(file,line);
                    size_t s = line.find("\""), e = line.rfind("\"");
                    cout << cmd << ": " << line.substr(s+1,e-s-1) << endl;
                }
                if(cmd == "<Vertex>")
                {
                    float posx,posy,posz, normx,normy,normz, u,v;
                    
                    file >> posx >> posy >> posz >> tmp >> tmp >> normx >> normy >> normz >> tmp >> tmp >> tmp >> u >> v;
                    cout << cmd << ": " << posx << " , " << posy << " , " << posz << endl;
                    vert tmp_vertex;
                    tmp_vertex.position = vec4(posx,posy,posz,1.0f);
                    tmp_vertex.uv = vec2(u,v);
                    tmp_vertex.normal = vec4(normx,normy,normz,0.0f);
                    vertices.push_back(tmp_vertex);
                    //verticies.push_back(vec3(posx,posy,posz));
                    //normals.push_back(vec3(normx,normy,normz));
                    //uvs.push_back(vec2(u,v));
                }
                if(cmd == "<Polygon>")
                {
                    int a,b,c;
                    getline(file,line);
                    getline(file,line);
                    file >> tmp >> tmp >> a >> b >> c;
                    polygons.push_back(a);
                    polygons.push_back(b);
                    polygons.push_back(c);
                    cout << cmd << ": " << a << " , " << b << " , " << c << endl;
                }
            }
            file.close();
        }
        
    };
}


#endif