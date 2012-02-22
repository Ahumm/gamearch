#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include "egg_model.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"
#include <iostream>

int CurrentWidth = 800,
	CurrentHeight = 450,
	WindowHandle = 0;
double thistime, oldtime, dt, starttime; //not floats!

//FOV, aspect ratio, near, far
//mvp::mat4 ProjectionMatrix = glm::perspective(60.0f, 16.0f / 9.0f, 0.1f, 100.f);
mvp::mat4 ViewMatrix;// = glm::mat4(1.0f);
mvp::mat4 ModelMatrix;// = glm::mat4(1.0f);

unsigned FrameCount = 0;
static const double PI = 3.14159265358979323846;

GLuint
	ProjectionMatrixUniformLocation,
	ViewMatrixUniformLocation,
	ModelMatrixUniformLocation,
	samplerLoc,
	BufferIds[3] = { 0 },
	ShaderIds[3] = { 0 },
    TexId,
    VertCount;
	
double LastTime = 0;

void Initialize(int, char*[]);
void InitWindow(void);
void RenderFunction(void);
void CreatePanda(void);
void DrawPanda(void);
void DestroyPanda(void);
void game_loop(void);
void checkShader(GLuint);
GLuint LoadShader(const char*, GLenum);
void OnGLError(const char*);
GLboolean LoadTexture( char* );

int main(int argc, char* argv[])
{
	Initialize(argc, argv);

    game_loop();
    
    DestroyPanda();
	
	exit(EXIT_SUCCESS);
}

void Initialize(int argc, char* argv[])
{
	GLenum GlewInitResult;

	InitWindow();
	OnGLError("Init window");

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

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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
    //ViewMatrix = glm::lookAt(glm::vec3(0.0, 0.0, 4.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    //ViewMatrix = glm::translate(ViewMatrix, glm::vec3(0,0,-2));
    
    ////////////////////////////////////////////////////////////////////////
    // DO MATH HERE
    ////////////////////////////////////////////////////////////////////////
    
    CreatePanda();
}

void InitWindow(void)
{
	// Initialize GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit( EXIT_FAILURE );
    }
    
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 0);
    glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	if( !glfwOpenWindow( CurrentWidth, CurrentHeight, 0,0,0,0, 16,0, GLFW_WINDOW ) )
    {
        fprintf( stderr, "Failed to open GLFW window\n" );
        glfwTerminate();
        exit( EXIT_FAILURE );
    }
    
    fprintf(stderr, "GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void game_loop(void){
    int playing;
    playing = GL_TRUE;
    while( playing && glfwGetWindowParam( GLFW_OPENED ) )
    {
        glfwEnable(GLFW_STICKY_KEYS);
        // Frame timer
        oldtime = thistime;
        thistime = glfwGetTime();
        dt = thistime - oldtime;
        
        //Key events
        // Did the user press ESC?
        if( glfwGetKey( GLFW_KEY_ESC ) )
        {
            playing = GL_FALSE;
        }
       
        // Display
        RenderFunction();
        glfwSwapBuffers();
        glfwDisable(GLFW_STICKY_KEYS);
    }
}

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DrawPanda();
}

void CreateCube()
{
    printf("Create\n");

    const mvp::egg_model panda("panda_model.egg");

    VertCount = panda.vertices.size();
    
    ShaderIds[0] = glCreateProgram();
    printf("%d\n",ShaderIds[0]);
    OnGLError("ERROR: Could not create the shader program");
	
	ShaderIds[1] = LoadShader("panda.fs", GL_FRAGMENT_SHADER);
    checkShader(ShaderIds[1]);
	ShaderIds[2] = LoadShader("panda.vs", GL_VERTEX_SHADER);
	checkShader(ShaderIds[2]);
	glAttachShader(ShaderIds[0], ShaderIds[1]);
	glAttachShader(ShaderIds[0], ShaderIds[2]);
	
	glLinkProgram(ShaderIds[0]);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(panda.vertices[0]), &panda.vertices[0], GL_STATIC_DRAW);
    OnGLError("ERROR: Could not bind the VBO to the VAO");

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(panda.vertices[0]), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(panda.vertices[0]), (GLvoid*)sizeof(panda.vertices[0].position));
    OnGLError("ERROR: Could not set VAO attributes");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIds[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(panda.polygons[0]), &panda.polygons[0], GL_STATIC_DRAW);
    OnGLError("ERROR: Could not bind the IBO to the VAO");

	glBindVertexArray(0);
	
    // Generate texture objects
    glGenTextures( 1, &TexId );

    // Make texture object active
    glBindTexture( GL_TEXTURE_2D, TexId );

    // Set texture parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    // Upload texture from file to texture memory, autouses glTexImage2D, needs TGA
    if( !glfwLoadTexture2D( panda.texture, 0 ) )
        fprintf( stderr, "Failed to load texture" );
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
	float CubeAngle;
	double Now = glfwGetTime();

	if (LastTime == 0)
		LastTime = Now;

	CubeRotation += 450.0f * ((float)(Now - LastTime));
    //fprintf(stderr, "Now: %f, Last: %f, CR: %f\n",Now,LastTime, (45.0f * ((float)(Now - LastTime) / CLOCKS_PER_SEC)));
	CubeAngle = CubeRotation * (float)(PI / 180); //radians
    //fprintf(stderr, "cube angle: %f\n",CubeAngle);
	LastTime = Now;

	ModelMatrix = mvp::mat4;
    ModelMatrix.rotateHPR(CubeAngle,CubeAngle,0.0f);
    //ModelMatrix = glm::rotate(ModelMatrix, CubeAngle, glm::vec3(0, 1, 0)); //rotateH
    //ModelMatrix = glm::rotate(ModelMatrix, CubeAngle, glm::vec3(1, 0, 0)); //rotateP
    
	glUseProgram(ShaderIds[0]);
    OnGLError("DRAW_ERROR: Could not use the shader program");

	glUniformMatrix4fv(ModelMatrixUniformLocation, 1, GL_FALSE, ModelMatrix.value_ptr());
	glUniformMatrix4fv(ViewMatrixUniformLocation, 1, GL_FALSE, ViewMatrix.value_ptr());
	glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE, ProjectionMatrix.value_ptr());
    OnGLError("ERROR: Could not set the shader uniforms");

	glBindVertexArray(BufferIds[0]);
    OnGLError("ERROR: Could not bind the VAO for drawing purposes");

	glDrawElements(GL_TRIANGLES, VertCount, GL_UNSIGNED_INT, (GLvoid*)0);
	OnGLError("ERROR: Could not draw the cube");

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
		fprintf(stderr, "ERROR: Could not open file %s\n", filename);

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

		fprintf(stderr, display_message, gluErrorString(ErrorValue));

		free(display_message);
	}
}