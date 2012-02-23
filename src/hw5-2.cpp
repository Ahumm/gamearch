#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/Image.hpp>
#include "egg_model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define PI 3.14159265

int window_width  = 800,
    window_height = 480;
    
double last_tick, new_tick, tick_diff;

bool exiting = false;

sf::Window* window = NULL;

glm::mat4 projection_matrix = glm::perspective(60.0f, (float)window_width / (float)window_height, 0.1f, 100.f);
glm::mat4 view_matrix = glm::mat4(1.0f);
glm::mat4 model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f,0.01f,0.01f));

GLuint
    projection_matrix_uniform_loc,
    view_matrix_uniform_loc,
    model_matrix_uniform_loc,
    texture_uniform_loc,
    program_id,
    vertex_id,
    fragment_id,
    texture_id,
    vao, vbo, ibo,
    poly_count,
    vert_count,
    poly_size,
    vert_size,
    uv_offset;
    
void initialize();                          /////
void init_window();                         /////
void keyboard_function(sf::Event& event);   /////
void game_loop();                           /////
void render();                              /////
void create_panda();                        /////
void draw_panda();                          /////
void destroy_panda();                       /////
void create_shaders();                      /////
char* filetobuf(const string& file);        /////
void cleanup();                             /////
GLuint load_texture(const string& path);    /////

int main()
{
    init_window();
    
    initialize();
    
    create_shaders();
    
    create_panda();
    
    game_loop();
    
    destroy_panda();
    
    cleanup();
}

void game_loop()
{
    sf::Clock clock;
    last_tick = clock.GetElapsedTime().AsMilliseconds();
    while(window->IsOpen())
    {
        new_tick = clock.GetElapsedTime().AsMilliseconds();
        tick_diff = new_tick - last_tick;
        
        sf::Event event;
        while(window->PollEvent(event))
        {
            if(event.Type == sf::Event::Closed)
            {
                window->Close();
                break;
            }
            else if(event.Type == sf::Event::KeyPressed)
            {
                keyboard_function(event);
            }
        }
        if(exiting)
            break;
            
        
        
        render();
        
        window->Display();
        last_tick = new_tick;
    }
}

void keyboard_function(sf::Event& event)
{
    switch(event.Key.Code)
    {
        case sf::Keyboard::Escape:
            window->Close();
            exiting = true;
            break;
        default:
            break;
    }
}

void cleanup()
{
    glDetachShader(program_id,fragment_id);
    glDetachShader(program_id,vertex_id);
    glDeleteShader(fragment_id);
    glDeleteShader(vertex_id);
    glDeleteProgram(program_id);
    delete window;
}

void init_window()
{
    window = new sf::Window(sf::VideoMode(window_width, window_height, 32), "Fun Fun Fun");
    window->SetActive();
    window->Display();
    window->SetFramerateLimit(60);
}

void initialize()
{
    GLenum GlewInitResult;
	glewExperimental = GL_TRUE;
    GlewInitResult = glewInit();
    
    if (GLEW_OK != GlewInitResult) {
		fprintf(
			stderr,
			"ERROR: %s\n",
			glewGetErrorString(GlewInitResult)
		);
		exit(EXIT_FAILURE);
	}
    
    fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
	);
    
    fprintf(
		stdout,
		"INFO: GLEW Version: %s\n",
		glewGetString(GLEW_VERSION) 
	);
    
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
    
    view_matrix = glm::lookAt(glm::vec3(0.0f,0.0f,15.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program_id);
    
    //Uniforms
    glUniformMatrix4fv(model_matrix_uniform_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(view_matrix_uniform_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));
	glUniformMatrix4fv(projection_matrix_uniform_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
    glUniform1i(texture_uniform_loc, 0);
    
    draw_panda();
    
    glBindVertexArray(0);
    glUseProgram(0);
}

void create_shaders()
{
    program_id = glCreateProgram();
    GLuint fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint vertex_id = glCreateShader(GL_VERTEX_SHADER);
    
    char* fragment_source = filetobuf("shaders/panda.fs");
    char* vertex_source = filetobuf("shaders/panda.vs");
    
    glShaderSource(fragment_id, 1, (const GLchar**)&fragment_source,0);
    glCompileShader(fragment_id);
    
    glShaderSource(vertex_id, 1, (const GLchar**)&vertex_source,0);
    glCompileShader(vertex_id);
    
    glAttachShader(program_id,fragment_id);
    glAttachShader(program_id,vertex_id);
    
    glBindAttribLocation(program_id, 0, "in_Position");
    glBindAttribLocation(program_id, 1, "in_Tex");
    
    glLinkProgram(program_id);
    
    glUseProgram(program_id);
    
    projection_matrix_uniform_loc = glGetUniformLocation(program_id, "ModelMatrix");
    view_matrix_uniform_loc = glGetUniformLocation(program_id, "ViewMatrix");
    model_matrix_uniform_loc = glGetUniformLocation(program_id, "ProjectionMatrix");
    texture_uniform_loc = glGetUniformLocation(program_id, "s_tex");
    
    free(fragment_source);
    free(vertex_source);
}

void create_panda()
{
    mvp::egg_model panda("models/panda-model.egg");
    
    vert_count = (GLuint)panda.vertices.size();
    poly_count = (GLuint)(panda.polygons.size() / 3);
    
    vert_size = (GLuint)sizeof(panda.vertices[0]);
    uv_offset = (GLuint)sizeof(panda.vertices[0].position);
    poly_size = (GLuint)sizeof(panda.polygons[0]);
    
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    
    glGenBuffers(2,&vbo);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vert_size * vert_count, &panda.vertices[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vert_size, (GLvoid*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vert_size, (GLvoid*)uv_offset);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, poly_size * poly_count, &panda.polygons[0], GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    
    texture_id = load_texture("textures/" + panda.texture);
}

void draw_panda()
{
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, poly_count, GL_UNSIGNED_INT, (GLvoid*)0);
}

void destroy_panda()
{
    glDeleteBuffers(1,&vbo);
    glDeleteBuffers(1,&ibo);
    glDeleteVertexArrays(1,&vao);
}

GLuint load_texture(const string& path)
{
    sf::Image img;
    if(!img.LoadFromFile(path))
    {
        fprintf( stderr, "Failed to load texture" );
        throw 1;
    }
    
    GLuint ret;
    glGenTextures(1, &ret);
    glBindTexture(GL_TEXTURE_2D, ret);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.GetWidth(), img.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.GetPixelsPtr());
    
    return ret;
}






char* filetobuf(const string& file)
{
    FILE *fptr;
    long length;
    char *buf;
 
    fptr = fopen(file.c_str(), "rb"); /* Open file for reading */
    if (!fptr) /* Return NULL on failure */
        return NULL;
    fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
    length = ftell(fptr); /* Find out how many bytes into the file we are */
    buf = (char*)malloc(length+1); /* Allocate a buffer for the entire length of the file and a null terminator */
    fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
    fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
    fclose(fptr); /* Close the file */
    buf[length] = 0; /* Null terminator */
 
    return buf; /* Return the buffer */
}





