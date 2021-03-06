#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <GL/glew.h> //MUST come before GLFW!
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/Image.hpp>
#include <glm/glm.hpp> //feel free to use your own math library!
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "morph.h"
#include "egg_model.h"

int CurrentWidth = 800,
	CurrentHeight = 450,
	WindowHandle = 0;
    
sf::Window* window = NULL;
sf::Clock* wclock = NULL;

mvp::egg_model* panda = NULL;

double thistime, oldtime, dt, starttime; //not floats!

glm::vec3 campos = glm::vec3(0.0,-10.0,15.0);
glm::vec3 tarpos = glm::vec3(0.0,0.0,0.0);
glm::vec3 upworl = glm::vec3(0.0,1.0,0.0);

//FOV, aspect ratio, near, far
glm::mat4 ProjectionMatrix = glm::perspective(60.0f, 16.0f / 9.0f, 0.1f, 100.f);
glm::mat4 ViewMatrix = glm::mat4(1.0f);
glm::mat4 ModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));

unsigned FrameCount = 0;
static const double PI = 3.14159265358979323846;

int used_program = 0;

GLuint
	ProjectionMatrixUniformLocation,
	ViewMatrixUniformLocation,
	ModelMatrixUniformLocation,
    TimeLocation,
	samplerLoc,
	BufferIds[3] = { 0 },
    nolight = 0,
    uberlight = 0,
	ShaderIds[4] = { 0 },
    TexId,
    poly_count,
    vert_count,
    poly_size,
    vert_size,
    uv_offset,
    norm_offset;
    
// FOR UBERLIGHT
GLuint
    ul_time_loc,
    ul_vm_loc,
    ul_mm_loc,
    ul_pm_loc,
    wclightpos_loc,
    viewposition_loc,
    wctolc_loc,
    wctolcit_loc,
    mctowc_loc,
    mctowcit_loc,
    ul_sampler_loc,
    lightcolor_loc,
    lightweights_loc,
    surfaceweights_loc,
    surfaceroughness_loc,
    ambientclamping_loc,
    barnshaping_loc,
    sewidth_loc,
    seheight_loc,
    sewidthedge_loc,
    seheightedge_loc,
    seroundness_loc,
    dsnear_loc,
    dsfar_loc,
    dsnearedge_loc,
    dsfaredge_loc;
    
// START UBERLIGHT DATA STUFF

glm::vec3 wclightpos = campos;                       // Starting light pos (same as camera)
glm::vec4 viewposition = glm::vec4(campos, 1.0f);    // Starting camera position

glm::mat4 wctolc;
glm::mat4 wctolcit;
glm::mat4 mctowc;
glm::mat4 mctowcit;

glm::vec3 lightcolor = glm::vec3(1.0f,1.0f,1.0f);
glm::vec3 lightweights = glm::vec3(1.0f,1.0f,1.0f);

glm::vec3 surfaceweights = glm::vec3(1.0f,1.0f,1.0f);
float surfaceroughness = 1.0f;
bool ambientclamping = true;

bool barnshaping = true;
float sewidth = 5.0f;
float seheight = 5.0f;
float sewidthedge = 1.0f;
float seheightedge = 1.0f;
float seroundness = 4.0f;

float dsnear = 0.1f;
float dsfar = 10.0f;
float dsnearedge = 0.0f;
float dsfaredge = 0.1f;

// END UBERLIGHT DATA STUFF    
    
    
    
    
	
float PandaRotation = 0;
double LastTime = 0;

void Initialize(int, char*[]);
void InitWindow(void);
void DestroyWindow(void);
void RenderFunction(void);
void CreatePanda(void);
void DrawPanda(void);
void DestroyPanda(void);
void game_loop(void);
bool handlekeys(sf::Event event);
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

	glClearColor(0.0f, 1.0f, 1.0f, 0.0f);
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
    //ViewMatrix = glm::lookAt(glm::vec3(0.0, -10.0,15.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    ViewMatrix = glm::lookAt(campos, tarpos, upworl);
    //ViewMatrix = glm::translate(ViewMatrix, glm::vec3(0,0,-2));
    
    
    CreatePanda();
}

void InitWindow(void)
{
	// Create the window and select it as active
    window = new sf::Window(sf::VideoMode(CurrentWidth,CurrentHeight, 32), "Fun Fun Fun");
    wclock = new sf::Clock();
    window->SetActive();
    window->Display();
    window->SetFramerateLimit(60);
}

void DestroyWindow(void)
{
    delete window;
    delete wclock;
}

void game_loop(void){
    bool exiting = 0;
    oldtime = wclock->GetElapsedTime().AsMilliseconds();
    while(window->IsOpen())
    {
        thistime = wclock->GetElapsedTime().AsMilliseconds();
        dt = thistime - oldtime;
        
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
                exiting = handlekeys(event);
            }
            if(exiting)
                break;
        }
        if(exiting)
            break;
        
       
        // Display
        RenderFunction();
        window->Display();
        oldtime = thistime;
    }
}

bool handlekeys(sf::Event event)
{
    if(event.Key.Code == sf::Keyboard::Escape)
    {
        window->Close();
        return true;
    }
    if(event.Key.Code == sf::Keyboard::S)
    {
        used_program = (used_program + 1) % 2;
        if(used_program == 1) {
            glUseProgram(uberlight);
        }
        else {
            glUseProgram(nolight);
        }
        OnGLError("ERROR: Could not switch shaders");
    }
    
    return false;
}

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DrawPanda();
}

void CreatePanda()
{
    printf("Create\n");
    panda = new mvp::egg_model("models/panda-model.egg");

//Get some vertex and polygon info
    vert_count = (GLuint)panda->vertices.size();
    poly_count = (GLuint)panda->polygons.size();
    
    vert_size = (GLuint)sizeof(panda->vertices[0]);
    uv_offset = (GLuint)sizeof(panda->vertices[0].position);
    norm_offset = (GLuint)(sizeof(panda->vertices[0].position) + sizeof(panda->vertices[0].uv));
    poly_size = (GLuint)sizeof(panda->polygons[0]);
    
    fprintf(stdout, "is: %u\nfs: %u\nvc: %u\npc: %u\nvs: %u\nuvo: %u\nno: %u\nps: %u\n", sizeof(int),sizeof(float),vert_count, poly_count, vert_size, uv_offset, norm_offset, poly_size);
    
    // NORMAL SHADERS
    nolight = glCreateProgram();
    OnGLError("ERROR: Could not create the shader program");
	
	ShaderIds[0] = LoadShader("shaders/panda.fs", GL_FRAGMENT_SHADER);
    checkShader(ShaderIds[0]);
	ShaderIds[1] = LoadShader("shaders/panda.vs", GL_VERTEX_SHADER);
	checkShader(ShaderIds[1]);
	glAttachShader(nolight, ShaderIds[0]);
	glAttachShader(nolight, ShaderIds[1]);
	
    //if not using "location" in shader
	glBindAttribLocation(nolight, 0, "in_Position");
	glBindAttribLocation(nolight, 1, "in_Tex");
    //glBindAttribLocation(ShaderIds[0], 2, "in_Normal");
	
	glLinkProgram(nolight);
    OnGLError("ERROR: Could not link the shader program");

    //glUseProgram(nolight);
    OnGLError("ERROR: Could use shader program (Create)");
    
    TimeLocation = glGetUniformLocation(nolight, "time");
    OnGLError("ERROR: Could not get time uniform locations");
    ModelMatrixUniformLocation = glGetUniformLocation(nolight, "ModelMatrix");
    OnGLError("ERROR: Could not get Model uniform locations");
    ViewMatrixUniformLocation = glGetUniformLocation(nolight, "ViewMatrix");
    OnGLError("ERROR: Could not get View uniform locations");
    ProjectionMatrixUniformLocation = glGetUniformLocation(nolight, "ProjectionMatrix");
    OnGLError("ERROR: Could not get Projection uniform locations");
    
    // END NORMAL SHADER
    
    // UNSET ACTIVE PROGRAM
    glUseProgram(0);
    
    // UBERLIGHT SHADERS
    
    
    uberlight = glCreateProgram();
    printf("%d\n",uberlight);
    OnGLError("ERROR: Could not create the shader program");
	
	ShaderIds[2] = LoadShader("shaders/uberlight.fs.bak", GL_FRAGMENT_SHADER);
    checkShader(ShaderIds[2]);
	ShaderIds[3] = LoadShader("shaders/uberlight.vs.bak", GL_VERTEX_SHADER);
	checkShader(ShaderIds[3]);
	glAttachShader(uberlight, ShaderIds[2]);
	glAttachShader(uberlight, ShaderIds[3]);
    
    printf("%d\n", ShaderIds[3]);
	
    //if not using "location" in shader
	glBindAttribLocation(uberlight, 0, "in_Position");
	glBindAttribLocation(uberlight, 1, "in_Tex");
    glBindAttribLocation(uberlight, 2, "in_Normal");
	
	glLinkProgram(uberlight);
    OnGLError("ERROR: Could not link the shader program");

    //glUseProgram(uberlight);
    OnGLError("ERROR: Could use shader program (Create)");
    
    ul_time_loc = glGetUniformLocation(uberlight, "time");
    printf("%d\n", ul_time_loc);
    OnGLError("ERROR: Could not get time uniform locations");
    ul_mm_loc = glGetUniformLocation(uberlight, "ModelMatrix");
    OnGLError("ERROR: Could not get Model uniform locations");
    ul_vm_loc = glGetUniformLocation(uberlight, "ViewMatrix");
    OnGLError("ERROR: Could not get View uniform locations");
    ul_pm_loc = glGetUniformLocation(uberlight, "ProjectionMatrix");
    OnGLError("ERROR: Could not get Projection uniform locations");
    wclightpos_loc = glGetUniformLocation(uberlight, "WCLightPos");
    viewposition_loc = glGetUniformLocation(uberlight, "ViewPosition");
    wctolc_loc = glGetUniformLocation(uberlight, "WCtoLC");
    wctolcit_loc = glGetUniformLocation(uberlight, "WCtoLCit");
    mctowc_loc = glGetUniformLocation(uberlight, "MCtoWC");
    mctowcit_loc = glGetUniformLocation(uberlight, "MCtoWCit");
    ul_sampler_loc = glGetUniformLocation(uberlight, "s_tex");
    lightcolor_loc = glGetUniformLocation(uberlight, "LightColor");
    lightweights_loc = glGetUniformLocation(uberlight, "LightWeights");
    surfaceweights_loc = glGetUniformLocation(uberlight, "SurfaceWeights");
    surfaceroughness_loc = glGetUniformLocation(uberlight, "SurfaceRoughness");
    ambientclamping_loc = glGetUniformLocation(uberlight, "AmbientClamping");
    barnshaping_loc = glGetUniformLocation(uberlight, "BarnShaping");
    sewidth_loc = glGetUniformLocation(uberlight, "SeWidth");
    seheight_loc = glGetUniformLocation(uberlight, "SeHeight");
    sewidthedge_loc = glGetUniformLocation(uberlight, "SeWidthEdge");
    seheightedge_loc = glGetUniformLocation(uberlight, "SeHeightEdge");
    seroundness_loc = glGetUniformLocation(uberlight, "SeRoundness");
    dsnear_loc = glGetUniformLocation(uberlight, "DsNear");
    dsfar_loc = glGetUniformLocation(uberlight, "DsFar");
    dsnearedge_loc = glGetUniformLocation(uberlight, "DsNearEdge");
    dsfaredge_loc = glGetUniformLocation(uberlight, "DsFarEdge");
    OnGLError("ERROR: Could not get some uniform locations");
    
    // END UBERLIGHT SHADERS

    used_program = 0;
    glUseProgram(nolight);
    
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
	glBufferData(GL_ARRAY_BUFFER, vert_size * vert_count, &(panda->vertices[0]), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);
    OnGLError("ERROR: Could not bind the VBO to the VAO");

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vert_size, (GLvoid*)0);
	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vert_size, (GLvoid*)uv_offset);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), (GLvoid*)sizeof(VERTICES[0].Position));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vert_size, (GLvoid*)norm_offset);
    OnGLError("ERROR: Could not set VAO attributes");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIds[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, poly_size * poly_count, &(panda->polygons[0]), GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);
    OnGLError("ERROR: Could not bind the IBO to the VAO");

	glBindVertexArray(0);
    
    // Generate texture objects
    glGenTextures( 1, &TexId );

    // Make texture object active
    glBindTexture( GL_TEXTURE_2D, TexId );
    OnGLError("ERROR: Could not bind Texture");
    
    // Set texture parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    OnGLError("ERROR: Could not set Texture parameters");
    
    
    // Upload texture from file to texture memory, autouses glTexImage2D, needs TGA
    sf::Image img;
    if(!img.LoadFromFile("textures/" + panda->texture))
    {
        fprintf( stderr, "Failed to load texture" );
        throw 1;
    }
    
    img.FlipVertically();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.GetWidth(), img.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.GetPixelsPtr());
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2048, 1536, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    //samplerLoc = glGetUniformLocation(nolight, "s_tex");
    OnGLError("ERROR: Could not get Texture location");
    //glActiveTexture(GL_TEXTURE0);
    //glUniform1i(samplerLoc, 0);
    OnGLError("ERROR: Could not load Texture");
}

void DestroyPanda()
{
    glUseProgram(0);
    
    OnGLError("ERROR: pre-destroy");
    
	glDetachShader(nolight, ShaderIds[0]);
	glDetachShader(nolight, ShaderIds[1]);
    OnGLError("ERROR: Could not detach nolight shaders");
	glDeleteShader(ShaderIds[0]);
	glDeleteShader(ShaderIds[1]);
    OnGLError("ERROR: Could not destroy nolight shaders");
	glDeleteProgram(nolight);
    OnGLError("ERROR: Could not destroy nolight program");
    
    glDetachShader(uberlight, ShaderIds[2]);
	glDetachShader(uberlight, ShaderIds[3]);
	glDeleteShader(ShaderIds[2]);
	glDeleteShader(ShaderIds[3]);
	glDeleteProgram(uberlight);
	OnGLError("ERROR: Could not destroy uberlight shaders");

	glDeleteBuffers(2, &BufferIds[1]);
	glDeleteVertexArrays(1, &BufferIds[0]);
	OnGLError("ERROR: Could not destroy the buffer objects");
    
    delete panda;
}

void DrawPanda(void)
{
	float PandaAngle;
	double Now = wclock->GetElapsedTime().AsMilliseconds();

	if (LastTime == 0)
		LastTime = Now;

	PandaRotation += 5.0f * ((float)(Now - LastTime));
    //fprintf(stderr, "Now: %f, Last: %f, CR: %f\n",Now,LastTime, (45.0f * ((float)(Now - LastTime) / CLOCKS_PER_SEC)));
	PandaAngle = PandaRotation * (float)(PI / 180); //radians
    //fprintf(stderr, "cube angle: %f\n",PandaAngle);
	LastTime = Now;

	//ModelMatrix = glm::mat4(1.0f);
    ModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));
    ModelMatrix = glm::rotate(ModelMatrix, PandaAngle, glm::vec3(0, 1, 0)); //rotateH
    ModelMatrix = glm::rotate(ModelMatrix, PandaAngle, glm::vec3(1, 0, 0)); //rotateP

    switch(used_program)
    {
        case 0:
            glUniformMatrix4fv(ModelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
            glUniformMatrix4fv(ViewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
            glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
            glUniform1f(TimeLocation, (float)Now);
            OnGLError("ERROR: Could not set the shader uniforms");
            break;
        case 1:
            // COMPUTATIONS
            
            wctolc = glm::lookAt(campos, tarpos, upworl);
            wctolcit = glm::transpose(glm::inverse(wctolc));
            mctowc = ViewMatrix * ModelMatrix;
            mctowcit = glm::transpose(glm::inverse(mctowc));
        
            // UNIFORM SETTING
            
            glUniformMatrix4fv(ul_mm_loc, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
            glUniformMatrix4fv(ul_vm_loc, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
            glUniformMatrix4fv(ul_pm_loc, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
            glUniform1f(ul_time_loc, (float)Now);
            glUniform1i(ul_sampler_loc, 0);
            
            glUniform3fv(wclightpos_loc, 1, glm::value_ptr(wclightpos));             // WCLIGHTTPOS
            glUniform4fv(viewposition_loc, 1, glm::value_ptr(viewposition));         // VIEWPOSITION
            
            glUniformMatrix4fv(wctolc_loc, 1, GL_FALSE, glm::value_ptr(wctolc));     // WCTOLC
            glUniformMatrix4fv(wctolcit_loc, 1, GL_FALSE, glm::value_ptr(wctolcit)); // WCTOLCIT
            glUniformMatrix4fv(mctowc_loc, 1, GL_FALSE, glm::value_ptr(mctowc));     // MCTOWC
            glUniformMatrix4fv(mctowcit_loc, 1, GL_FALSE, glm::value_ptr(mctowcit)); // MCTOWCIT
            
            glUniform3fv(lightcolor_loc, 1, glm::value_ptr(lightcolor));             // LIGHTCOLOR
            glUniform3fv(lightweights_loc, 1, glm::value_ptr(lightweights));         // LIGHTWEIGHTS
            
            glUniform3fv(surfaceweights_loc, 1, glm::value_ptr(surfaceweights));     // SURFACEWEIGHTS
            glUniform1f(surfaceroughness_loc, surfaceroughness);                     // SURFACEROUGHNESS
            glUniform1i(ambientclamping_loc, (ambientclamping) ? 1 : 0);             // AMBIENTCLAMPING
            
            glUniform1i(barnshaping_loc, (barnshaping) ? 1 : 0);                     // BARNSHAPING
            glUniform1f(sewidth_loc, sewidth);                                       // SEWIDTH
            glUniform1f(seheight_loc, seheight);                                     // SEHEIGHT
            glUniform1f(sewidthedge_loc, sewidthedge);                               // SEWIDTHEDGE
            glUniform1f(seheightedge_loc, seheightedge);                             // SEHEIGHTEDGE
            glUniform1f(seroundness_loc, seroundness);                               // SEROUNDNESS
            
            glUniform1f(dsnear_loc, dsnear);                                         // DSNEAR
            glUniform1f(dsfar_loc, dsfar);                                           // DSFAR
            glUniform1f(dsnearedge_loc, dsnearedge);                                 // DSNEAREDGE
            glUniform1f(dsfaredge_loc, dsfaredge);                                   // DSFAREDGE
            
            OnGLError("ERROR: Could not set the shader uniforms");
            break;
    }

	glBindVertexArray(BufferIds[0]);
    OnGLError("ERROR: Could not bind the VAO for drawing purposes");

	glDrawElements(GL_TRIANGLES, poly_count, GL_UNSIGNED_INT, (GLvoid*)0);
	OnGLError("ERROR: Could not draw the cube");

	glBindVertexArray(0);
	//glUseProgram(0);
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
		fprintf(stdout, "OpenGL Shader Compile Error:\n%.*s", loglen, logbuffer);
	} else {
		int loglen;
		char logbuffer[1000];
		glGetShaderInfoLog(shader, sizeof(logbuffer), &loglen, logbuffer);
		fprintf(stderr, "OpenGL Shader Compile OK:\n%.*s", loglen, logbuffer);
		fprintf(stdout, "OpenGL Shader Compile OK:\n%.*s", loglen, logbuffer);
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

		fprintf(stderr, display_message, "");//gluErrorString(ErrorValue));
		fprintf(stdout, display_message, "");//gluErrorString(ErrorValue));

		free(display_message);
	}
}