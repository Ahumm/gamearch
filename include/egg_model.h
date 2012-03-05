#ifndef _EGG_MODEL_H_
#define _EGG_MODEL_H_

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

namespace mvp {
    typedef struct {
        float position[4];
        float uv[2];
        float normal[4];
    } vert;
    
    class egg_model
    {
    public:
        egg_model()
        {
            texture = "";
            vert_arr = NULL;
            poly_arr = NULL;
        }
        
        egg_model(const char * path)
        {
            texture = "";
            vert_arr = NULL;
            poly_arr = NULL;
            load_model(path);
            to_arr();
        }
        
        ~egg_model()
        {
            if(vert_arr) delete vert_arr;
            if(poly_arr) delete poly_arr;
        }
        
        vector<vert> vertices;
        vector<int> polygons;
        string texture;
        
        vert* vert_arr;
        int* poly_arr;
        
        void to_arr()
        {
            vert_arr = new vert[vertices.size()];
            poly_arr = new int[polygons.size()];
        }
        
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
                    texture = line.substr(s+1,e-s-1);
                    //cout << cmd << ": " << line.substr(s+1,e-s-1) << endl;
                }
                if(cmd == "<Vertex>")
                {
                    float posx,posy,posz, normx,normy,normz, u,v;
                    
                    file >> posx >> posy >> posz >> tmp >> tmp >> normx >> normy >> normz >> tmp >> tmp >> tmp >> u >> v;
                    //cout << cmd << ": " << posx << " , " << posy << " , " << posz << endl;
                    vert tmp_vertex = {posx,posy,posz,1.0f,u,v,normx,normy,normz,0.0f};
                    //tmp_vertex.position = vec4(posx,posy,posz,1.0f);
                    //tmp_vertex.uv = vec2(u,v);
                    //tmp_vertex.normal = vec4(normx,normy,normz,0.0f);
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
                    polygons.push_back(a-1);
                    polygons.push_back(b-1);
                    polygons.push_back(c-1);
                    //cout << cmd << ": " << a << " , " << b << " , " << c << endl;
                }
            }
            file.close();
            //assign_RGBA();
        }
        
    };
}


#endif