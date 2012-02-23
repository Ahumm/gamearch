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
//#include "vec2.h"
//#include "vec3.h"
//#include "vec4.h"
//#include "mat4.h"
#include <iostream>
#include "morph.h"

#define TEST 0

int CurrentWidth = 800,
	CurrentHeight = 450,
	WindowHandle = 0;
double new_tick, last_tick, d_tick, starttime; //not floats!

bool exiting = false;
sf::Window* window = NULL;

//FOV, aspect ratio, near, far
glm::mat4 ProjectionMatrix = glm::perspective(60.0f, 16.0f / 9.0f, 0.1f, 100.f);
glm::mat4 ViewMatrix = glm::mat4(1.0f);
glm::mat4 ModelMatrix = glm::mat4(1.0f);

unsigned FrameCount = 0;
//static const double PI = 3.14159265358979323846;

GLuint
	ProjectionMatrixUniformLocation,
	ViewMatrixUniformLocation,
	ModelMatrixUniformLocation,
	samplerLoc,
	BufferIds[3] = { 0 },
	ShaderIds[3] = { 0 },
    TexId,
    VertCount,
    PolyCount;
	
double LastTime = 0;

void Initialize(int, char*[]);
void InitWindow(void);
void KeyboardFunction(sf::Event & event);
void RenderFunction(void);
void ResizeFunction(sf::Event & event);
void CreatePanda(void);
void TestPanda(void);
void DrawPanda(void);
void DestroyPanda(void);
void game_loop(void);
void checkShader(GLuint);
GLuint LoadShader(const char*, GLenum);
void OnGLError(const char*);
GLboolean LoadTexture( char* );
void Cleanup();

int main(int argc, char* argv[])
{
	Initialize(argc, argv);

    game_loop();
    
    DestroyPanda();
	
    Cleanup();
    
	exit(EXIT_SUCCESS);
}

void Initialize(int argc, char* argv[])
{
	InitWindow();
	OnGLError("Init window");

    GLenum GlewInitResult;
	glewExperimental = GL_TRUE;
    GlewInitResult = glewInit();
    OnGLError("GLEW init");
    
	if (GLEW_OK != GlewInitResult) {
		fprintf(
			stderr,
			"ERROR: %s\n",
			glewGetErrorString(GlewInitResult)
		);
		exit(EXIT_FAILURE);
	}
	OnGLError("GLEW string");
    
	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
	);
	OnGLError("Get string");
	
    fprintf(
		stdout,
		"INFO: GLEW Version: %s\n",
		glewGetString(GLEW_VERSION) 
	);
    
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	OnGLError("Clear color");
	glEnable(GL_DEPTH_TEST);
	OnGLError("Depth test");
	glDepthFunc(GL_LESS);
	OnGLError("Depth Func");
	glEnable(GL_CULL_FACE);
	OnGLError("Enable cull face");
	glCullFace(GL_BACK);
	OnGLError("Cull face");
	glFrontFace(GL_CCW);
	OnGLError("Front face");
    ViewMatrix = glm::lookAt(glm::vec3(0.0, 0.0, 15.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

    //ViewMatrix = mvp::lookat(mvp::vec3(0.0, 0.0, 15.0), mvp::vec3(0.0, 0.0, 0.0), mvp::vec3(0.0, 1.0, 0.0));

#if TEST == 1
    TestPanda();
#else
    CreatePanda();
#endif
}

void InitWindow(void)
{
    window = new sf::Window(sf::VideoMode(CurrentWidth, CurrentHeight, 32), "");
    window->SetActive();
    window->Display();
    window->SetFramerateLimit(60);
}

void KeyboardFunction(sf::Event & event)
{
    if (event.Type == sf::Event::KeyPressed)
    {
        switch(event.Key.Code)
        {
            case sf::Keyboard::Escape:
                window->Close();
                exiting = true;
                break;
            /*case sf::Keyboard::A:
                ViewMatrix = glm::rotate(ViewMatrix, 10.0f*(float)d_tick/1000.0f);
                break;
            case sf::Keyboard::D:
                ViewMatrix.rotateH(-10.0f*(float)d_tick/1000.0f);
                break;
            case sf::Keyboard::W:
                ViewMatrix.rotateP(10.0f*(float)d_tick/1000.0f);
                break;
            case sf::Keyboard::S:
                ViewMatrix.rotateP(-10.0f*(float)d_tick/1000.0f);
                break;*/
            default:
                break;
        }
    }
}

void game_loop(void){
    sf::Clock clock;
    last_tick = clock.GetElapsedTime().AsMilliseconds();
    while(window->IsOpen())
    {
        new_tick = clock.GetElapsedTime().AsMilliseconds();
        d_tick = new_tick - last_tick;
        sf::Event event;
        while(window->PollEvent(event))
        {
            if (event.Type == sf::Event::Closed)
            {
                window->Close();
                break;
            }
            else if(event.Type == sf::Event::KeyPressed)
            {
                KeyboardFunction(event);
            }
            else if(event.Type == sf::Event::Resized)
            {
                ResizeFunction(event);
            }
        }
        if(exiting)
            break;

        // Display
        RenderFunction();
        
        window->Display();
        last_tick = new_tick;
    }
}

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DrawPanda();
}

void ResizeFunction(sf::Event & event)
{
    if (event.Type == sf::Event::Resized)
    {
        CurrentWidth = event.Size.Width;
        CurrentHeight = event.Size.Height;
        glViewport(0, 0, CurrentWidth, CurrentHeight);
    }
}

void CreatePanda()
{
    printf("Create\n");
    const mvp::egg_model panda("models/panda-model.egg");
//ModelMatrix.scale(0.01);

    VertCount = panda.vertices.size();
    PolyCount = panda.polygons.size();

    ShaderIds[0] = glCreateProgram();
    printf("%d\n",ShaderIds[0]);
    OnGLError("ERROR: Could not create the shader program");

	ShaderIds[1] = LoadShader("shaders/panda.fs", GL_FRAGMENT_SHADER);
    checkShader(ShaderIds[1]);
	ShaderIds[2] = LoadShader("shaders/panda.vs", GL_VERTEX_SHADER);
	checkShader(ShaderIds[2]);

	glAttachShader(ShaderIds[0], ShaderIds[1]);
	glAttachShader(ShaderIds[0], ShaderIds[2]);
   
    glBindAttribLocation(ShaderIds[0], 0, "in_Position");
    glBindAttribLocation(ShaderIds[0], 1, "in_Tex");
   
	glLinkProgram(ShaderIds[0]);
    glUseProgram(ShaderIds[0]);
    OnGLError("ERROR: Could not link the shader program");

    ModelMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ModelMatrix");
    OnGLError("ERROR: Could not get Model uniform locations");
    ViewMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ViewMatrix");
    OnGLError("ERROR: Could not get View uniform locations");
    ProjectionMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ProjectionMatrix");
    OnGLError("ERROR: Could not get Projection uniform locations");

	glGenVertexArrays(1, &BufferIds[0]);
    OnGLError("ERROR: Could not generate the VAO");
	glBindVertexArray(BufferIds[0]);
    OnGLError("ERROR: Could not bind the VAO");

	glGenBuffers(2, &BufferIds[1]);
    OnGLError("ERROR: Could not generate the buffer objects");

	glBindBuffer(GL_ARRAY_BUFFER, BufferIds[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(panda.vertices[0])*VertCount, &(panda.vertices[0]), GL_STATIC_DRAW);
    OnGLError("ERROR: Could not bind the VBO to the VAO");

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(panda.vertices[0]), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(panda.vertices[0]), (GLvoid*)sizeof(panda.vertices[0].position));
    OnGLError("ERROR: Could not set VAO attributes");
    
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
    OnGLError("ERROR: Could not enable vertex attributes");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIds[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(panda.polygons[0])*PolyCount, &(panda.polygons[0]), GL_STATIC_DRAW);
    OnGLError("ERROR: Could not bind the IBO to the VAO");

	glBindVertexArray(0);
	
    // Load Texture
    ///sf::Image img;
    ///if(!img.LoadFromFile("textures/" + panda.texture))
    ///{
    ///    fprintf( stderr, "Failed to load texture" );
    ///    throw 1;
    ///}

    // Generate texture objects
    ///glGenTextures( 1, &TexId );

    // Make texture object active
    ///glBindTexture( GL_TEXTURE_2D, TexId );
    
    // Set texture parameters
    ///glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    ///glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    ///glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    ///glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    ///glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.GetWidth(), img.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.GetPixelsPtr());
    
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2048, 1536, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels?);
    ///samplerLoc = glGetUniformLocation(ShaderIds[0], "s_tex");
    //glActiveTexture(GL_TEXTURE0);
    ///glUniform1i(samplerLoc, 0);
}

void TestPanda()
{
    printf("Create\n");
PolyCount = 5412;
VertCount = 1339;
    ShaderIds[0] = glCreateProgram();
    printf("%d\n",ShaderIds[0]);
    OnGLError("ERROR: Could not create the shader program");

	ShaderIds[1] = LoadShader("shaders/panda.fs", GL_FRAGMENT_SHADER);
    checkShader(ShaderIds[1]);
	ShaderIds[2] = LoadShader("shaders/panda.vs", GL_VERTEX_SHADER);
	checkShader(ShaderIds[2]);

	glAttachShader(ShaderIds[0], ShaderIds[1]);
	glAttachShader(ShaderIds[0], ShaderIds[2]);
    
    glBindAttribLocation(ShaderIds[0], 0, "in_Position");
    glBindAttribLocation(ShaderIds[0], 1, "in_Tex");

	glLinkProgram(ShaderIds[0]);
    glUseProgram(ShaderIds[0]);
    OnGLError("ERROR: Could not link the shader program");

    ModelMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ModelMatrix");
    OnGLError("ERROR: Could not get Model uniform locations");
    ViewMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ViewMatrix");
    OnGLError("ERROR: Could not get View uniform locations");
    ProjectionMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ProjectionMatrix");
    OnGLError("ERROR: Could not get Projection uniform locations");

	glGenVertexArrays(1, &BufferIds[0]);
    OnGLError("ERROR: Could not generate the VAO");
	glBindVertexArray(BufferIds[0]);
    OnGLError("ERROR: Could not bind the VAO");

    glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
    OnGLError("ERROR: Could not enable vertex attributes");
    
	glGenBuffers(2, &BufferIds[1]);
    OnGLError("ERROR: Could not generate the buffer objects");

	glBindBuffer(GL_ARRAY_BUFFER, BufferIds[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);
    OnGLError("ERROR: Could not bind the VBO to the VAO");

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), (GLvoid*)sizeof(VERTICES[0].Position));
    OnGLError("ERROR: Could not set VAO attributes");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIds[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);
    OnGLError("ERROR: Could not bind the IBO to the VAO");

	glBindVertexArray(0);

    // Load Texture
    sf::Image img;
    if(!img.LoadFromFile("panda-model.png"))
    {
        fprintf( stderr, "Failed to load texture" );
        throw 1;
    }

    // Generate texture objects
    glGenTextures( 1, &TexId );

    // Make texture object active
    glBindTexture( GL_TEXTURE_2D, TexId );

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.GetWidth(), img.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.GetPixelsPtr());
    
    // Set texture parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2048, 1536, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels?);
    samplerLoc = glGetUniformLocation(ShaderIds[0], "s_tex");
    //glActiveTexture(GL_TEXTURE0);
    glUniform1i(samplerLoc, 0);
}

void DestroyPanda()
{
	glDetachShader(ShaderIds[0], ShaderIds[1]);
	glDetachShader(ShaderIds[0], ShaderIds[2]);
	glDeleteShader(ShaderIds[1]);
	glDeleteShader(ShaderIds[2]);
	glDeleteProgram(ShaderIds[0]);
	OnGLError("ERROR: Could not destroy the shaders");

	glDeleteBuffers(2, &BufferIds[1]);
	glDeleteVertexArrays(1, &BufferIds[0]);
	OnGLError("ERROR: Could not destroy the buffer objects");
}

void DrawPanda(void)
{
	glUseProgram(ShaderIds[0]);
    OnGLError("DRAW_ERROR: Could not use the shader program");
    
	glUniformMatrix4fv(ModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	glUniformMatrix4fv(ViewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
    OnGLError("ERROR: Could not set the shader uniforms");
    
    ///glBindTexture(GL_TEXTURE_2D, TexId);
    
	glBindVertexArray(BufferIds[0]);
    OnGLError("ERROR: Could not bind the VAO for drawing purposes");

	glDrawElements(GL_TRIANGLES, PolyCount, GL_UNSIGNED_INT, (GLvoid*)0);
	OnGLError("ERROR: Could not draw the panda");

	glBindVertexArray(0);
	glUseProgram(0);
}

void checkShader(GLuint shader)
{
	GLint status;
	//lGetShaderiv gets a particular parameter of the shader
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		int loglen;
		char logbuffer[1000];
		//there's also a corresponding glGetProgramInfoLog function for the linked program object
		glGetShaderInfoLog(shader, sizeof(logbuffer), &loglen, logbuffer);
		fprintf(stderr, "OpenGL Shader Compile Error:\n%.*s", loglen, logbuffer);
	} else {
		int loglen;
		char logbuffer[1000];
		glGetShaderInfoLog(shader, sizeof(logbuffer), &loglen, logbuffer);
		fprintf(stderr, "OpenGL Shader Compile OK:\n%.*s", loglen, logbuffer);
	}
}

GLuint LoadShader(const char* filename, GLenum shader_type)
{
	GLuint shader_id = 0;
	FILE* file;
	long file_size = -1;
	GLchar* glsl_source;
    fprintf(stderr, "name: %s\n",filename);

	if (NULL != (file = fopen(filename, "rb")) &&
		0 == fseek(file, 0, SEEK_END) &&
		-1 != (file_size = ftell(file)))
	{
		rewind(file);
		
		if (NULL != (glsl_source = (GLchar*)malloc(file_size + 1)))
		{
			if (file_size == (long)fread(glsl_source, sizeof(GLchar), file_size, file))
			{
				glsl_source[file_size] = '\0';
                const GLchar* glsl_source_c = glsl_source;
				fprintf(stderr, "Source: %s\n", glsl_source_c);

				if (0 != (shader_id = glCreateShader(shader_type)))
				{
					glShaderSource(shader_id, 1, &glsl_source_c, NULL);
					glCompileShader(shader_id);
					OnGLError("Could not compile a shader");
				}
				else
					fprintf(stderr, "ERROR: Could not create a shader.\n");
			}
			else
				fprintf(stderr, "ERROR: Could not read file %s\n", filename);

			free(glsl_source);
		}
		else
			fprintf(stderr, "ERROR: Could not allocate %li bytes.\n", file_size);

		fclose(file);
	}
	else
    {
		fprintf(stderr, "ERROR: Could not open file %s\n", filename);
    }

	return shader_id;
}

void OnGLError(const char* error_message)
{
	const GLenum ErrorValue = glGetError();
    
	if (ErrorValue != GL_NO_ERROR)
	{
		const char* APPEND_DETAIL_STRING = ": %s\n";
		const size_t APPEND_LENGTH = strlen(APPEND_DETAIL_STRING) + 1;
		const size_t message_length = strlen(error_message);
		char* display_message = (char*)malloc(message_length + APPEND_LENGTH);

		memcpy(display_message, error_message, message_length);
		memcpy(&display_message[message_length], APPEND_DETAIL_STRING, APPEND_LENGTH);

		fprintf(stderr, display_message, ErrorValue);

		free(display_message);
	} 
}

void Cleanup()
{
    delete window;
}