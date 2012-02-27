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
#include "morph.h"

#define PI 3.14159265

int window_width  = 800,
    window_height = 480;
    
double last_tick, new_tick, tick_diff;

bool exiting = false;

sf::Window* window = NULL;

glm::mat4 projection_matrix = glm::perspective(60.0f, (float)window_width / (float)window_height, 0.1f, 100.f);
glm::mat4 view_matrix = glm::mat4(1.0f);
glm::mat4 model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.0001f));

GLuint
    projection_matrix_uniform_loc,
    view_matrix_uniform_loc,
    model_matrix_uniform_loc,
    texture_uniform_loc,
    program_id,
    vertex_id,
    fragment_id,
    texture_id,
    vert_arr, vert_buf, index_buf,
    poly_count,
    vert_count,
    poly_size,
    vert_size,
    uv_offset;
    
void initialize();                          /////
void init_window();                         /////
void keyboard_function(sf::Event& event);   /////
void resize_function(sf::Event& event);     /////
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
    
    exit(EXIT_SUCCESS);
}

void game_loop()
{
    sf::Clock clock;
    last_tick = clock.GetElapsedTime().AsMilliseconds();
    while(window->IsOpen())
    {
        new_tick = clock.GetElapsedTime().AsMilliseconds();
        tick_diff = new_tick - last_tick;
        
        // Handle events
        sf::Event event;
        while(window->PollEvent(event))
        {
            if(event.Type == sf::Event::Closed)
            {
                window->Close();
                exiting = true;
            }
            else if(event.Type == sf::Event::KeyPressed)
            {
                keyboard_function(event);
            }
            else if(event.Type == sf::Event::Resized)
            {
                resize_function(event);
            }
            if(exiting)
                break;
        }
        if(exiting)
            break;
            
        
        // Draw what needs to be drawn
        render();
        
        // Display what was drawn
        window->Display();
        last_tick = new_tick;
    }
}

void keyboard_function(sf::Event& event)
{
    float rotation;
    switch(event.Key.Code)
    {
        //Exit on Escape
        case sf::Keyboard::Escape:
            window->Close();
            exiting = true;
            break;
        case sf::Keyboard::A:
            rotation = 1;//450.0f * ((float)tick_diff);
            model_matrix = glm::rotate(model_matrix, rotation, glm::vec3(0,1,0));
            break;
        case sf::Keyboard::D:
            rotation = -1;//-450.0f * ((float)tick_diff);
            model_matrix = glm::rotate(model_matrix, rotation, glm::vec3(0,1,0));
            break;
        case sf::Keyboard::W:
            rotation = 1;//450.0f * ((float)tick_diff);
            model_matrix = glm::rotate(model_matrix, rotation, glm::vec3(1,0,0));
            break;
        case sf::Keyboard::S:
            rotation = -1;//-450.0f * ((float)tick_diff);
            model_matrix = glm::rotate(model_matrix, rotation, glm::vec3(1,0,0));
            break;
        default:
            break;
    }
}
void resize_function(sf::Event & event)
{
    // Resize the window
    if (event.Type == sf::Event::Resized)
    {
        window_width = event.Size.Width;
        window_height = event.Size.Height;
        glViewport(0, 0, window_width, window_height);
    }
}

void cleanup()
{
    // Clean before exiting
    glDetachShader(program_id,fragment_id);
    glDetachShader(program_id,vertex_id);
    glDeleteShader(fragment_id);
    glDeleteShader(vertex_id);
    glDeleteProgram(program_id);
    delete window;
}

void init_window()
{
    // Create the window and select it as active
    window = new sf::Window(sf::VideoMode(window_width, window_height, 32), "Fun Fun Fun");
    window->SetActive();
    window->Display();
    window->SetFramerateLimit(60);
}

void initialize()
{
    // Initialize GLEW
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
    
    // Set out background color
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

    // Set up some GL values
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
    
    // Look at the panda
    view_matrix = glm::lookAt(glm::vec3(0.0f,0.0f,15.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
}

void render()
{
    // Clear the buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Load the shader program
    glUseProgram(program_id);
    
    // Set the Uniforms
    glUniformMatrix4fv(model_matrix_uniform_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(view_matrix_uniform_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));
	glUniformMatrix4fv(projection_matrix_uniform_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
    glUniform1i(texture_uniform_loc, 0);
    
    // Draw our panda
    draw_panda();
    
    // Clean up
    glBindVertexArray(0);
    glUseProgram(0);
}

void create_shaders()
{
    // Create the program
    program_id = glCreateProgram();
    // Create the shaders
    GLuint fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint vertex_id = glCreateShader(GL_VERTEX_SHADER);
    
    // Get the shader source code
    char* fragment_source = filetobuf("shaders/panda.fs");
    char* vertex_source = filetobuf("shaders/panda.vs");
    
    // Make the fragment shader
    glShaderSource(fragment_id, 1, (const GLchar**)&fragment_source,0);
    glCompileShader(fragment_id);
    
    // Make the vertex shader
    glShaderSource(vertex_id, 1, (const GLchar**)&vertex_source,0);
    glCompileShader(vertex_id);
    
    // Attach the shaders to the program
    glAttachShader(program_id,fragment_id);
    glAttachShader(program_id,vertex_id);
    
    // Bind the shader variables
    glBindAttribLocation(program_id, 0, "in_Position");
    glBindAttribLocation(program_id, 1, "in_Tex");
    
    // Link the program
    glLinkProgram(program_id);
    
    // Set the program ass active
    glUseProgram(program_id);
    
    // Get the shader Uniform locations
    projection_matrix_uniform_loc = glGetUniformLocation(program_id, "ModelMatrix");
    view_matrix_uniform_loc = glGetUniformLocation(program_id, "ViewMatrix");
    model_matrix_uniform_loc = glGetUniformLocation(program_id, "ProjectionMatrix");
    texture_uniform_loc = glGetUniformLocation(program_id, "s_tex");
    
    // Clean up
    free(fragment_source);
    free(vertex_source);
}

void create_panda()
{
    // Load the model
    mvp::egg_model panda("models/panda-model.egg");
    
    //Get some vertex and polygon info
    vert_count = (GLuint)panda.vertices.size();
    poly_count = (GLuint)panda.polygons.size();
    
    vert_size = (GLuint)sizeof(panda.vertices[0]);
    uv_offset = (GLuint)sizeof(panda.vertices[0].position);
    poly_size = (GLuint)sizeof(panda.polygons[0]);
    
    // Make and bind a vertex array object
    glGenVertexArrays(1,&vert_arr);
    glBindVertexArray(vert_arr);
    
    // Generate and bind a vertex buffer object
    glGenBuffers(1,&vert_buf);
    glBindBuffer(GL_ARRAY_BUFFER, vert_buf);
    
    // generate and bind the index buffer
    glGenBuffers(1,&index_buf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buf);
    
    // Set the VBO's data to the vertices
    //glBufferData(GL_ARRAY_BUFFER, vert_size * vert_count, &panda.vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);
    
    // Set the VAO's data to the position and the vertex position and uv
    //glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vert_size, (GLvoid*)0);
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vert_size, (GLvoid*)uv_offset);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), (GLvoid*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), (GLvoid*)sizeof(VERTICES[0].Position));
    
    // Enable the vertex array's attributes
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    // Set the index buffer to the polygons
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, poly_size * poly_count, &panda.polygons[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);
    
    // Load the texture
    texture_id = load_texture("textures/" + panda.texture);
    
    // Clean up
    glBindVertexArray(0);
}

void draw_panda()
{
    // Set the texture as active
    glBindTexture(GL_TEXTURE_2D, texture_id);
    // Set the vertex array as active
    glBindVertexArray(vert_arr);
    // Draw the triangles
    glDrawElements(GL_TRIANGLES, poly_count, GL_UNSIGNED_INT, (GLvoid*)0);
}

void destroy_panda()
{
    // Delete the VBOs and VAOs
    glDeleteBuffers(1,&vert_buf);
    glDeleteBuffers(1,&index_buf);
    glDeleteVertexArrays(1,&vert_arr);
}

GLuint load_texture(const string& path)
{
    // Load and image from a file
    sf::Image img;
    if(!img.LoadFromFile(path))
    {
        fprintf( stderr, "Failed to load texture" );
        throw 1;
    }
    
    GLuint ret;
    // Generate and bind the texture
    glGenTextures(1, &ret);
    glBindTexture(GL_TEXTURE_2D, ret);
    
    // Set some GL values
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // specify the image to use as the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.GetWidth(), img.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.GetPixelsPtr());
    
    return ret;
}


char* filetobuf(const string& file)
{
    FILE *fptr;
    long length;
    char *buf;
 
    // Open the file
    fptr = fopen(file.c_str(), "rb");
    // Check for failure
    if (!fptr)
        return NULL;
    // Get the length of the file
    fseek(fptr, 0, SEEK_END); 
    length = ftell(fptr);
    // Allocate some space (with a null terminator)
    buf = (char*)malloc(length+1); 
    fseek(fptr, 0, SEEK_SET); 
    // Read the file
    fread(buf, length, 1, fptr);
    // Close
    fclose(fptr);
    // Null terminate
    buf[length] = 0; 
 
    return buf; 
}





