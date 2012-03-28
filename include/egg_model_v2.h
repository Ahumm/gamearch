#ifndef _EGG_MODEL_H_
#define _EGG_MODEL_H_

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

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
            name = "JOINT_TREE_ROOT";
            local = glm::mat4(1.0);
            world = glm::mat4(1.0);
            transform = glm::mat4(1.0);
            h = h_anim.begin();
            p = p_anim.begin();
            r = r_anim.begin();
            x = x_anim.begin();
            y = y_anim.begin();
            z = z_anim.begin();
        }
        
        ~joint()
        {
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
        
        bool add_joint(joint* n_joint)
        {
            if(name == "JOINT_TREE_ROOT")
            {
                children.push_back(n_joint);
                return true;
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
        
        void print_joint(string tabs = "") {
            cout << tabs << "Name: " << name << endl;
            cout << tabs << "Matrix:" << endl;
            float* m = glm::value_ptr(transform);
            cout << tabs << "\t" << m[0] << " " << m[4] << " " << m[8 ] << " " << m[12] << endl;
            cout << tabs << "\t" << m[1] << " " << m[5] << " " << m[9 ] << " " << m[13] << endl;
            cout << tabs << "\t" << m[2] << " " << m[6] << " " << m[10] << " " << m[14] << endl;
            cout << tabs << "\t" << m[3] << " " << m[7] << " " << m[11] << " " << m[15] << endl;
            
            cout << tabs << "H_anim: { ";
            for(size_t i = 0; i < h_anim.size(); i++) cout << h_anim[i] << " ";
            cout << "}" << endl;
            cout << tabs << "P_anim: { ";
            for(size_t i = 0; i < p_anim.size(); i++) cout << p_anim[i] << " ";
            cout << "}" << endl;
            cout << tabs << "R_anim: { ";
            for(size_t i = 0; i < r_anim.size(); i++) cout << r_anim[i] << " ";
            cout << "}" << endl;
            cout << tabs << "X_anim: { ";
            for(size_t i = 0; i < x_anim.size(); i++) cout << x_anim[i] << " ";
            cout << "}" << endl;
            cout << tabs << "Y_anim: { ";
            for(size_t i = 0; i < y_anim.size(); i++) cout << y_anim[i] << " ";
            cout << "}" << endl;
            cout << tabs << "Z_anim: { ";
            for(size_t i = 0; i < z_anim.size(); i++) cout << z_anim[i] << " ";
            cout << "}" << endl;
            
            if(children.size() > 0) cout << tabs << "Children" << endl;
            for(size_t i = 0; i < children.size(); ++i) {
                children[i]->print_joint(tabs + "\t");
            }
            
        }
        
        void update() {
            for(size_t i = 0; i < children.size(); ++i) {
                children[i]->update(world);
            }
        }
        
        void update(glm::mat4 p_world) {
            local = transform;
            if(h == h_anim.end()) h = h_anim.begin();
            if(p == p_anim.end()) p = p_anim.begin();
            if(r == r_anim.end()) r = r_anim.begin();
            if(x == x_anim.end()) x = x_anim.begin();
            if(y == y_anim.end()) y = y_anim.begin();
            if(z == z_anim.end()) z = z_anim.begin();
            
            float ht = (h == h_anim.end())? 0.0f :(*h);
            float pt = (p == p_anim.end())? 0.0f :(*p);
            float rt = (r == r_anim.end())? 0.0f :(*r);
            float xt = (x == x_anim.end())? 0.0f :(*x);
            float yt = (y == y_anim.end())? 0.0f :(*y);
            float zt = (z == z_anim.end())? 0.0f :(*z);
            
            local = glm::rotate(local, ht, glm::vec3(0.0,1.0,0.0));
            local = glm::rotate(local, pt, glm::vec3(1.0,0.0,0.0));
            local = glm::rotate(local, rt, glm::vec3(0.0,0.0,1.0));
            local = glm::translate(local, glm::vec3(xt,yt,zt));
            
            world = p_world * local;
            
            if(h != h_anim.end()) ++h;
            if(p != p_anim.end()) ++p;
            if(r != r_anim.end()) ++r;
            if(x != x_anim.end()) ++x;
            if(y != y_anim.end()) ++y;
            if(z != z_anim.end()) ++z;

            
            for(size_t i = 0; i < children.size(); ++i) {
                children[i]->update(world);
            }
        }
        
        void reset_animation() {
            h = h_anim.begin();
            p = p_anim.begin();
            r = r_anim.begin();
            x = x_anim.begin();
            y = y_anim.begin();
            z = z_anim.begin();
            
            for(size_t i = 0; i < children.size(); ++i) {
                children[i]->reset_animation();
            }
        }
    
        string name;
        glm::mat4 transform;
        glm::mat4 local;
        glm::mat4 world;
        vector<joint*> children;
        
        vector<float> h_anim;
        vector<float>::iterator h;
        vector<float> p_anim;
        vector<float>::iterator p;
        vector<float> r_anim;
        vector<float>::iterator r;
        vector<float> x_anim;
        vector<float>::iterator x;
        vector<float> y_anim;
        vector<float>::iterator y;
        vector<float> z_anim;
        vector<float>::iterator z;
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
            is_animated = false;
        }
        
        egg_model(const char * path)
        {
            texture = "";
            vert_arr = NULL;
            poly_arr = NULL;
            j_root = NULL;
            is_tex = false;
            is_animated = false;
            load_model(path);
            
            compute_weights();

            //if(j_root) j_root->print_joint("");
            
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
        
        vector<float> s_joint_vec;
        std::map<string,int> joint_holder;
        
        joint* j_root;
        
        vert* vert_arr;
        poly* poly_arr;
        
        bool is_tex;
        bool is_animated;
        
        void update() {
            if(!j_root) return;
            j_root->update();
            
            serialize_joints();
        }
        
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
                    joint* t_root = munch_joint(file);
                    if(!j_root) {
                        j_root = new joint();
                    }
                    j_root->add_joint(t_root);
                }
            }
            file.close();
        }
        
        joint* munch_joint(ifstream &file)
        {
            joint* t_joint = new joint;
            string munch = "";
            
            file >> t_joint->name;
            cout << t_joint->name << endl;
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
                            //glm::mat4 t_mat4 = glm::transpose(glm::make_mat4(arr));
                            glm::mat4 t_mat4 = glm::make_mat4(arr);
                            t_joint->transform = t_mat4;
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
                            string tmp_str;
                            file >> tmp_str;
                            stringstream ss(tmp_str);
                            ss >> weight;
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
                else if(s_tag == "<Scalar>"){
                    // Munch until we're out of whatever hole we landed in
                    file >> munch >> munch >> munch >> munch;
                }
                    
            
                file >> s_tag;
            }
            
            return t_joint;
        }
        
        void compute_weights()
        {
            if(!j_root) return;
            
            joint_holder.clear();
            
            int c = 0;
            weight_helper(joint_holder, j_root, c);
            
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
        
        void weight_helper(std::map<string,int> &holder, joint* root, int &c)
        {
            // Add root joint
            holder.insert(pair<string,int>(root->name, c));
            c++;

            // Add all children joints;
            for(int i = 0; i < root->children.size(); ++i) {
                weight_helper(holder, root->children[i], c);
            }
        }
        
        void serialize_joints() {
            if(!j_root) return;
            
            s_joint_vec.clear();
            
            map<int, glm::mat4> tmp_holder;
            
            map<string,int>::iterator m_it = joint_holder.begin();
            while(m_it != joint_holder.end()) {
                joint* t_joint = j_root->find_joint((*m_it).first);
                tmp_holder.insert(pair<int,glm::mat4>((*m_it).second, t_joint->world));
                ++m_it;
            }
            
            map<int, glm::mat4>::iterator t_it = tmp_holder.begin();
            while(t_it != tmp_holder.end()) {
                float* v_ptr = glm::value_ptr(t_it->second);
                for(int i = 0; i < 16; ++i) {
                    s_joint_vec.push_back(v_ptr[i]);
                }
                ++t_it;
            }
        }
        
        void load_animation(const char* anim_filename) {
            is_animated = true;
            
            string munch = "";
        
            ifstream file;
            file.open(anim_filename);
            if(!file.is_open()){
                cout << "File " << anim_filename << " could not be opened" << endl;
                return;
            }
            while(!file.eof())
            {
                string n_tag = "";
                file >> n_tag;
                
                if(n_tag == "<Table>") {
                    string tmp_str = "";
                    file >> tmp_str;
                    if(tmp_str == "{") continue;
                    joint* t_joint = j_root->find_joint(tmp_str);
                    if(!t_joint) continue;
                    
                    munch = "";
                    string s_tag = "";
                    while(s_tag != "}") {
                        file >> s_tag;
                        if(s_tag == "<Xfm$Anim_S$>") {
                            file >> munch >> munch;
                        }
                        else if( s_tag == "<Scalar>") {
                            file >> munch >> munch >> munch >> munch;
                        }
                        else if(s_tag == "<Char*>") {
                            file >> munch >> munch >> munch >> munch;
                        }
                        else if(s_tag == "<S$Anim>") {
                            char dof = 0;
                            vector<float> v_vals;
                            
                            file >> dof;
                            file >> munch >> munch >> munch >> tmp_str;
                            
                            // Grab all numbers
                            while(tmp_str != "}") {
                                stringstream ss(tmp_str);
                                float f;
                                ss >> f;
                                v_vals.push_back(f);
                                file >> tmp_str;
                            }
                            
                            file >> munch;
                            
                            switch(dof) {
                                case 'h':
                                    t_joint->h_anim = v_vals;
                                    break;
                                case 'p':
                                    t_joint->p_anim = v_vals;
                                    break;
                                case 'r':
                                    t_joint->r_anim = v_vals;
                                    break;
                                case 'x':
                                    t_joint->x_anim = v_vals;
                                    break;
                                case 'y':
                                    t_joint->y_anim = v_vals;
                                    break;
                                case 'z':
                                    t_joint->z_anim = v_vals;
                                    break;
                            }
                        }
                    }
                }
            }
            file.close();
            j_root->print_joint("");
            j_root->reset_animation();
        }
    };
}


#endif