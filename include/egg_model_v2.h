#ifndef _EGG_MODEL_H_
#define _EGG_MODEL_H_

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>

using namespace std;

namespace mvp {
    typedef struct {
        float position[4];
        float rgba[4];
        float uv[2];
        float normal[4];
        int j_index[3];
        float j_weight[3];
    } vert;
    
    typedef struct {
        string joints[3];
        float weights[3];
    } vert_weight;
    
    typedef struct {
        int v_refs[3];
        float rgba[4];
    } poly;
    
    const vert BASIC_VERT = {0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,-1,-1,-1,0.0f,0.0f,0.0f};
    const poly BASIC_POLY = {0,0,0,0.0f,0.0f,0.0f,1.0f};
    
    class joint {
    public:
        joint()
        {
            name = "";
            localmatrix = glm::mat4(1.0);
        }
        
        ~joint()
        {
        }
        
        glm::mat4 to_model_matrix(string j_name)
        {
            if(name == j_name)
            {
                return localmatrix;
            }
            glm::mat4 cmp = glm::mat4(0.0);
            for(int i = 0; i < children.size(); ++i)
            {
                glm::mat4 c_tmp = children[i]->to_model_matrix_helper(j_name, localmatrix);
                if(c_tmp != cmp) return c_tmp;
            }
            return cmp;
        }
        
        glm::mat4 to_model_matrix_helper(string j_name, glm::mat4 parentmat)
        {
            if(name == j_name)
            {
                return parentmat * localmatrix;
            }
            glm::mat4 cmp = glm::mat4(0.0);
            glm::mat4 l_to_p = parentmat * localmatrix;
            for(int i = 0; i < children.size(); ++i)
            {
                glm::mat4 c_tmp = children[i]->to_model_matrix_helper(j_name, l_to_p);
                if(c_tmp != cmp) return c_tmp;
            }
            return cmp;
        }
        
        bool add_joint(string p_name, joint* n_joint)
        {
            if(name == p_name)
            {
                children.push_back(n_joint);
                return true;
            }
            for(int i = 0; i < children.size(); ++i)
            {
                bool c_tmp = children[i]->add_joint(p_name, n_joint);
                if(c_tmp) return c_tmp;
            }
            return false;
        }
        
        joint* find_joint(string j_name)
        {
            if(name == j_name)
            {
                return this;
            }
            for(int i = 0; i < children.size(); ++i)
            {
                joint* c_tmp = children[i]->find_joint(j_name);
                if(c_tmp) return c_tmp;
            }
            return NULL;
        }
        
        
    
        string name;
        glm::mat4 localmatrix;
        vector<joint*> children;
    };
    
    class egg_model
    {
    public:
        egg_model()
        {
            texture = "";
            vert_arr = NULL;
            poly_arr = NULL;
            j_root = NULL;
            is_tex = false;
        }
        
        egg_model(const char * path)
        {
            texture = "";
            vert_arr = NULL;
            poly_arr = NULL;
            j_root = NULL;
            is_tex = false;
            load_model(path);
            to_arr();
        }
        
        ~egg_model()
        {
            if(vert_arr) delete vert_arr;
            if(poly_arr) delete poly_arr;
            if(j_root) delete j_root;
        }
        
        vector<vert> vertices;
        vector<vert_weight> vertex_weights;
        vector<poly> polygons;
        string texture;
        
        joint* j_root;
        
        vert* vert_arr;
        poly* poly_arr;
        
        bool is_tex;
        
        void to_arr()
        {
            vert_arr = new vert[vertices.size()];
            poly_arr = new poly[polygons.size()];
        }
        
        void load_model(const char * path)
        {
            string munch = "";
        
            ifstream file;
            file.open(path);
            if(!file.is_open()){
                cout << "File " << path << " could not be opened" << endl;
                return;
            }
            while(!file.eof())
            {
                string next_tag;
                
                file >> next_tag;
            
                if(next_tag == "<Texture>") {
                    file >> munch >> munch;
                    file >> texture;
                    // Eat our way out
                    int depth = 1;
                    while(depth > 0) {
                        file >> munch;
                        if(munch == "{") ++depth;
                        if(munch == "}") --depth;
                    }
                }
                else if(next_tag == "<Vertex>") {
                    vert tmp_vert;
                    memcpy(&tmp_vert, &BASIC_VERT, sizeof(BASIC_VERT));
                    vert_weight tmp_vert_weight = {"","","",-1,-1,-1};
                    file >> munch >> munch;
                    file >> tmp_vert.position[0] >> tmp_vert.position[1] >> tmp_vert.position[2];
                    
                    string s_tag;
                    file >> s_tag;
                    while(s_tag != "}")
                    {
                        if(s_tag == "<Normal>") {
                            file >> munch;
                            file >> tmp_vert.normal[0] >> tmp_vert.normal[1] >> tmp_vert.normal[2];
                            file >> munch;
                        }
                        else if(s_tag == "<UV>") {
                            is_tex = true;
                            file >> munch;
                            file >> tmp_vert.uv[0] >> tmp_vert.uv[1];
                            file >> munch;
                        }
                        else if(s_tag == "<RGBA>") {
                            file >> munch;
                            file >> tmp_vert.rgba[0] >> tmp_vert.rgba[1] >> tmp_vert.rgba[2] >> tmp_vert.rgba[3];
                            file >> munch;
                        }
                        file >> s_tag;
                    }
                    vertices.push_back(tmp_vert);
                    vertex_weights.push_back(tmp_vert_weight);
                }
                else if(next_tag == "<Polygon>") {
                    poly t_poly;
                    memcpy(&t_poly, &BASIC_POLY, sizeof(BASIC_POLY));
                    
                    string s_tag;
                    file >> s_tag;
                    while(s_tag != "}") {
                        if(s_tag == "<RGBA>") {
                            file >> munch;
                            file >> t_poly.rgba[0] >> t_poly.rgba[1] >> t_poly.rgba[2] >> t_poly.rgba[3];
                            file >> munch;
                        }
                        else if(s_tag == "<VertexRef>") {
                            file >> munch;
                            file >> t_poly.v_refs[0] >> t_poly.v_refs[1] >> t_poly.v_refs[2];
                            // Eat our way out
                            int depth = 1;
                            while(depth > 0) {
                                file >> munch;
                                if(munch == "{") ++depth;
                                if(munch == "}") --depth;
                            }
                        }
                        file >> s_tag;
                    }
                    polygons.push_back(t_poly);
                }
                else if(next_tag == "<Joint>") {
                    j_root = munch_joint(file);
                }
            }
            file.close();
        }
        
        joint* munch_joint(ifstream &file)
        {
            joint* t_joint = new joint;
            string munch = "";
            
            file >> t_joint->name;
            file >> munch;
            string s_tag;
            file >> s_tag;
            while(s_tag != "}") {
                if(s_tag == "<Transform>") {
                    string ss_tag;
                    file >> munch;
                    file >> ss_tag;
                    while(ss_tag != "}") {
                        if(ss_tag == "<Matrix4>") {
                            file >> munch;
                            float arr[16];
                            
                            for(int i = 0; i < 16; ++i) {
                                float t;
                                file >> t;
                                arr[i] = t;
                            }
                            glm::mat4 t_mat4 = glm::transpose(glm::make_mat4(arr));
                            t_joint->localmatrix = t_mat4;
                            file >> munch;
                        }
                        file >> ss_tag;
                    }
                }
                else if(s_tag == "<Joint>") {
                    t_joint->children.push_back(munch_joint(file));
                }
                else if(s_tag == "<VertexRef>") {
                    file >> munch;
                    string ss_tag;
                    vector<int> v;
                    float weight;
                    file >> ss_tag;
                    while(ss_tag != "}") {
                        if(ss_tag == "<Scalar>") {
                            file >> munch >> munch;
                            file >> weight;
                            file >> munch;
                        }
                        else if(ss_tag == "<Ref>") {
                            file >> munch >> munch >> munch;
                        }
                        else {
                            v.push_back(atoi(ss_tag.c_str()));
                        }
                        file >> ss_tag;
                    }
                    for(int i = 0; i < v.size(); ++i) {
                        for(int j = 0; j < 3; ++j) {
                            if(vertex_weights[v[i]].joints[j] == "") {
                                vertex_weights[v[i]].joints[j] = t_joint->name;
                                vertex_weights[v[i]].weights[j] = weight;
                                break;
                            }
                        }
                    }
                }
            
                file >> s_tag;
            }
        }
        
        void compute_weights()
        {
            if(!j_root) return;
            
            std::map<string,int> joint_holder;
            
            int s = weight_helper(joint_holder, j_root, 0);
            
            vector<vert>::iterator it;
            vector<vert_weight>::iterator w_it;
            for(it = vertices.begin(), w_it = vertex_weights.begin(); it != vertices.end(); it++, w_it++)
            {
                for(int j = 0; j < 3; ++j) {
                    if((*w_it).joints[j] != "") {
                        map<string,int>::iterator m_it = joint_holder.find((*w_it).joints[j]);
                        if(m_it != joint_holder.end()) {
                            (*it).j_index[j] = m_it->second;
                            (*it).j_weight[j] = w_it->weights[j];
                        }
                    }
                }
            }
        }
        
        int weight_helper(std::map<string,int> &holder, joint* root, int c)
        {
            // Add root joint
            holder.insert(pair<string,int>(root->name, c));
            c++;
            
            // Add all children joints;
            for(int i = 0; i < root->children.size(); ++i) {
                c = weight_helper(holder, root->children[i], c);
            }
            
            return c;
        }
    };
}


#endif