
#define GLEW_STATIC

#include <iostream>
#include "glm/glm.hpp"//core glm functionality
#include "glm/gtc/matrix_transform.hpp"//glm extension for generating common transformation matrices
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include <string>
#include "Shader.hpp"
#include "Camera.hpp"
#include "SkyBox.hpp"
#define TINYOBJLOADER_IMPLEMENTATION

#include "Model3D.hpp"
#include "Mesh.hpp"

using namespace std;

int glWindowWidth = 1024;
int glWindowHeight = 800;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

const GLuint SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

glm::mat4 model;
GLuint modelLoc;
glm::mat4 view;
GLuint viewLoc;
glm::mat4 projection;
GLuint projectionLoc;
glm::mat3 normalMatrix;
GLuint normalMatrixLoc;
glm::mat3 lightDirMatrix;
GLuint lightDirMatrixLoc;
GLfloat angle_plane;
glm::vec3 lightDir;
GLuint lightDirLoc;
glm::vec3 lightColor;
GLuint lightColorLoc;

float lr1 = 20.0f;
float lrVehicle = 0.0f;
float lrAircraft = 0.0f;

//gps::Camera myCamera(glm::vec3(0.0f, -3.0f, 25.0f), glm::vec3(0.0f, 0.0f, 0.0f));
gps::Camera myCamera(glm::vec3(0.0f, 5.0f, 10.5f), glm::vec3(0.0f, 0.0f, 0.0f));
GLfloat cameraSpeed = 0.4f;

bool pressedKeys[1024];
GLfloat angle;
GLfloat lightAngle;

gps::Model3D myModel;
gps::Model3D lightCube;
gps::Model3D spaceship;
gps::Model3D city;
gps::Model3D monsterAdela;
gps::Model3D truck;
gps::Model3D army1;
gps::Model3D aircraftBad;
gps::Model3D muhammer;
gps::Model3D saveAircraft;
gps::Model3D rotatePlane;

gps::Shader myCustomShader;
gps::Shader lightShader;
gps::Shader depthMapShader;

GLuint shadowMapFBO;
GLuint depthMapTexture;

//variables for background
std::vector<const GLchar*> faces;

gps::SkyBox mySkyBox;
gps::Shader skyboxShader;

//for mouse view
float lastxpos;
float lastypos;

double xpos;
double ypos;

float characterX = 0.0f;
float carX = 0.0f;
float heliZ = 0.0f;
float heliX = 0.0f;
GLfloat heliAngle;

bool moveCharacter = false;
bool moveCar = false;
bool moveHeli = false;
bool gameOver = false;
bool muhammerOn = true;

float startTime = 0;

GLenum glCheckError_(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
	fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
	//TODO
	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	myCustomShader.useShaderProgram();

	//set projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	//send matrix data to shader
	GLint projLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	
	lightShader.useShaderProgram();
	
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	//set Viewport transform
	glViewport(0, 0, retina_width, retina_height);
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			pressedKeys[key] = true;
		else if (action == GLFW_RELEASE)
			pressedKeys[key] = false;
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{

}

void processMovement()
{

	if (pressedKeys[GLFW_KEY_Q]) {
		angle += 0.5f;
		if (angle > 360.0f)
			angle -= 360.0f;
	}

	if (pressedKeys[GLFW_KEY_E]) {
		angle -= 0.5f;
		if (angle < 0.0f)
			angle += 360.0f;
	}

	if (pressedKeys[GLFW_KEY_W]) {
		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_S]) {
		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_A]) {
		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_D]) {
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_J]) {

		lightAngle += 0.3f;
		if (lightAngle > 360.0f)
			lightAngle -= 360.0f;
		glm::vec3 lightDirTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDir, 1.0f));
		myCustomShader.useShaderProgram();
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDirTr));
	}

	if (pressedKeys[GLFW_KEY_L]) {
		lightAngle -= 0.3f; 
		if (lightAngle < 0.0f)
			lightAngle += 360.0f;
		glm::vec3 lightDirTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDir, 1.0f));
		myCustomShader.useShaderProgram();
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDirTr));
	}
    
    if(pressedKeys[GLFW_KEY_0]) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    
    if(pressedKeys[GLFW_KEY_9]) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    
    if(pressedKeys[GLFW_KEY_LEFT]) {
        if (characterX > -7.4) {
            characterX -= 0.1f;
            moveCharacter = true;
            moveCar = false;
        } else {
            moveCharacter = false;
            moveCar = true;
        }
    }
    
    if(pressedKeys[GLFW_KEY_RIGHT]) {
        if(moveCar == false && gameOver == false) {
            characterX += 0.1f;
            moveCharacter = true;
        }
    }
    
    if(pressedKeys[GLFW_KEY_UP]) {
        if (moveCar == true && carX > -32 && gameOver == false) {
            muhammerOn = false;
            carX -= 0.12f;
        } else {
            moveHeli = true;
            moveCar = false;
        }
    }
    
    if (pressedKeys[GLFW_KEY_DOWN]) {
        if(moveHeli == false && moveCar == true && gameOver == false) {
            carX += 0.12f;
        }
    }
    
    if (pressedKeys[GLFW_KEY_Z]) {
        if(moveHeli == true) {
            heliZ += 0.14f;
        }
    }
    
    if(pressedKeys[GLFW_KEY_X]) {
        if(moveHeli == true && heliZ > 0.1f && gameOver == false) {
            heliZ -= 0.14f;
        }
    }
    
    if (pressedKeys[GLFW_KEY_T]) {
        if(moveHeli == true && heliX > -2.0f && gameOver == false) {
            heliX -= 0.14f;
        }
    }
    
    if(pressedKeys[GLFW_KEY_G]) {
        if (moveHeli == true && gameOver == false) {
            heliX += 0.14f;
        }
    }
    
    if (pressedKeys[GLFW_KEY_H]) {
        if (moveHeli == true && gameOver == false) {
            heliAngle += 0.5f;
            if (heliAngle > 360.0f)
                heliAngle -= 360.0f;
        }
    }
    
    if (pressedKeys[GLFW_KEY_F]) {
        if (moveHeli == true && gameOver == false) {
            heliAngle -= 0.5f;
            if (heliAngle > 360.0f)
            heliAngle += 360.0f;
        }
    }
}

bool initOpenGLWindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	//for Mac OS X
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwMakeContextCurrent(glWindow);

	glfwWindowHint(GLFW_SAMPLES, 4);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	glfwSetKeyCallback(glWindow, keyboardCallback);
	glfwSetCursorPosCallback(glWindow, mouseCallback);
    
    //glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
	return true;
}

void initOpenGLState()
{
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_BLEND); //blending of colors
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glViewport(0, 0, retina_width, retina_height);

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
}

void initFBOs()
{
	//generate FBO ID
	glGenFramebuffers(1, &shadowMapFBO);
    
	//create depth texture for FBO
	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//attach texture to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 computeLightSpaceTrMatrix()
{
	const GLfloat near_plane = 1.0f, far_plane = 10.0f;
	glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);

	glm::vec3 lightDirTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDir, 1.0f));
	glm::mat4 lightView = glm::lookAt(lightDirTr, myCamera.getCameraTarget(), glm::vec3(0.0f, 1.0f, 0.0f));

	return lightProjection * lightView;
}

void initModels()
{
    monsterAdela = gps::Model3D("objects/monster/Umber_Hulk_1.obj", "objects/monster/");
    spaceship = gps::Model3D("objects/spaceship/Wraith_Raider_Starship/starship.obj", "objects/spaceship/Wraith_Raider_Starship/");
    city = gps::Model3D("objects/scifiCity/scifiCity/scifiCity.obj", "objects/scifiCity/scifiCity/");
	lightCube = gps::Model3D("objects/cube/cube.obj", "objects/cube/");
    truck = gps::Model3D("objects/truck/truck.obj", "objects/truck/");
    army1 = gps::Model3D("objects/army/army1.obj", "objects/army/");
    aircraftBad = gps::Model3D("objects/spacecraftBad1/aircraftBad.obj", "objects/spacecraftBad1/");
    muhammer = gps::Model3D("objects/muhammer/Muhammer.obj", "objects/muhammer/");
    saveAircraft = gps::Model3D("objects/saveAircraft/SeaHawk.obj", "objects/saveAircraft/");
    rotatePlane = gps::Model3D("objects/rotatePlane/saber.obj", "objects/rotatePlane/");
}

void initShaders()
{
	myCustomShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
	lightShader.loadShader("shaders/lightCube.vert", "shaders/lightCube.frag");
	depthMapShader.loadShader("shaders/simpleDepthMap.vert", "shaders/simpleDepthMap.frag");
}

void initUniforms()
{
	myCustomShader.useShaderProgram();

	modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");

	viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	
	normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
	
	lightDirMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDirMatrix");

	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	projectionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
	//set the light direction (direction towards the light)
    lightDir = glm::vec3(0.0f, 1.0f, 2.0f); //LA NICU: bate spre directia aia
	lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));

	//set light color
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	lightShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

//function for creating background
void initBackground() {
    
    //create background
    faces.push_back("textures/darkskies/darkskies_rt.tga");
    faces.push_back("textures/darkskies/darkskies_lf.tga");
    faces.push_back("textures/darkskies/darkskies_up.tga");
    faces.push_back("textures/darkskies/darkskies_dn.tga");
    faces.push_back("textures/darkskies/darkskies_bk.tga");
    faces.push_back("textures/darkskies/darkskies_ft.tga");
    
    mySkyBox.Load(faces);
    skyboxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
    skyboxShader.useShaderProgram();
    
    view = myCamera.getViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    
    projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

//for viewing the scene using the mouse
static void cursor_position_callback(GLFWwindow* glWindow, double xpos, double ypos)
{
    if (lastxpos < xpos)
        myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
    if (lastxpos > xpos)
        myCamera.move(gps::MOVE_LEFT, cameraSpeed);
    if (lastypos > ypos)
        myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
    if (lastypos < ypos)
        myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
    lastxpos = xpos;
    lastypos = ypos;
    
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (yoffset > 0) {
        myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
    } else {
        myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
    }
}

void renderScene()
{
    bool startedMoving = false;
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	processMovement();	

    //enemies chasing
    if ((moveCharacter == true || moveCar == true) && gameOver == false)
    {
        lr1 -= 0.1f;
        lrVehicle -= 0.05f;
        lrAircraft -= 0.1f;
        
        //detect of enemies caught our guy
        if (moveCharacter == true) {
            if ((lrAircraft + 21.0f - characterX <= 0.0f) || (lrVehicle + 13.0f - characterX <= 0.0f) ||
                (lr1 + 16.0f - characterX <= 0.0f)) {
                gameOver = true;
            }
        } else if (moveCar == true) {
            if ((lrAircraft + 35.4f - carX <= 0.0f) || (lrVehicle + 29.4f - carX <= 0.0f)
                || (lr1 + 32.4f - carX <= 0.0f)) {
                gameOver = true;
            }
        }
    }
    
	//render the scene to the depth buffer (first pass)
	depthMapShader.useShaderProgram();

	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightSpaceTrMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(computeLightSpaceTrMatrix()));
		
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	//create shadow for muhammer
	model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
    model = glm::translate(model, glm::vec3(characterX, 0.0f, 0.0f));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));

    if(muhammerOn == true) {
        muhammer.Draw(depthMapShader);
    }
    
    //shadow for truck
    model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -1.5f, 0.0f));
    model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
    model = glm::translate(model, glm::vec3(carX, 0.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
                       1,
                       GL_FALSE,
                       glm::value_ptr(model));
    truck.Draw(depthMapShader);
    
    
    //shadows for the city
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
                       1,
                       GL_FALSE,
                       glm::value_ptr(model));
    city.Draw(depthMapShader);
    
    //shadow for rotate plane
    angle_plane += 0.5f;
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -10.0f, 0.0f));
    model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
    model = glm::rotate(model, glm::radians(angle_plane), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
                       1,
                       GL_FALSE,
                       glm::value_ptr(model));
    rotatePlane.Draw(depthMapShader);
    
    //Save aircraft shadow
    model = glm::translate(glm::mat4(1.0f), glm::vec3(heliX, heliZ, 0.0f));
    model = glm::rotate(model, glm::radians(heliAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
                       1,
                       GL_FALSE,
                       glm::value_ptr(model));
    saveAircraft.Draw(depthMapShader);
    
    //ARMY1 shadow
    model = glm::translate(glm::mat4(1.0f), glm::vec3(lrVehicle, 0.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
                       1,
                       GL_FALSE,
                       glm::value_ptr(model));
    army1.Draw(depthMapShader);

    
    //shadow for monster
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.6f, 0.0f));
    model = glm::translate(model, glm::vec3(3.0f, -3.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
                       1,
                       GL_FALSE,
                       glm::value_ptr(model));
    monsterAdela.Draw(depthMapShader);
    
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//render the scene (second pass)

    //daca vreau numa obiectul fara umbre, incep de aici
	myCustomShader.useShaderProgram();

	//send lightSpace matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "lightSpaceTrMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(computeLightSpaceTrMatrix()));

	//send view matrix to shader
	view = myCamera.getViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "view"),
		1,
		GL_FALSE,
		glm::value_ptr(view));	

	//compute light direction transformation matrix
	lightDirMatrix = glm::mat3(glm::inverseTranspose(view));
	//send lightDir matrix data to shader
	glUniformMatrix3fv(lightDirMatrixLoc, 1, GL_FALSE, glm::value_ptr(lightDirMatrix));

	glViewport(0, 0, retina_width, retina_height);
	myCustomShader.useShaderProgram();

	//bind the depth map
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "shadowMap"), 3);
	

    //for spaceship
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(lr1, 10.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
		
    spaceship.Draw(myCustomShader);
    
    //for aircraft bad1
    model = glm::translate(glm::mat4(1.0f), glm::vec3(lrAircraft, 0.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    
    aircraftBad.Draw(myCustomShader);
    
    //for monster
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.6f, 0.0f));
    model = glm::translate(model, glm::vec3(3.0f, -3.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    
    monsterAdela.Draw(myCustomShader);

    
    //SCIFI CITY
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    city.Draw(myCustomShader);

    //TRUCK
    model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -1.5f, 0.0f));
    model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
    model = glm::translate(model, glm::vec3(carX, 0.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    
    truck.Draw(myCustomShader);
    
    //ARMY1
    model = glm::translate(glm::mat4(1.0f), glm::vec3(lrVehicle, 0.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    
    army1.Draw(myCustomShader);
    
    //MUHAMMER
    model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
    model = glm::translate(model, glm::vec3(characterX, 0.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    if (muhammerOn == true) {
        muhammer.Draw(myCustomShader);
    }
    
    //Save aircraft
    model = glm::translate(glm::mat4(1.0f), glm::vec3(heliX, heliZ, 0.0f));
    model = glm::rotate(model, glm::radians(heliAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    
    saveAircraft.Draw(myCustomShader);
    
    //Rotate Plane
    angle_plane += 0.5f;
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -10.0f, 0.0f));
    model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
    model = glm::rotate(model, glm::radians(angle_plane), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    
    rotatePlane.Draw(myCustomShader);

	//draw a white cube around the light

	lightShader.useShaderProgram();

	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

	model = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, lightDir);
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	lightCube.Draw(lightShader);
    
    //for background
    mySkyBox.Draw(skyboxShader, view, projection);
}

int main(int argc, const char * argv[]) {

	initOpenGLWindow();
	initOpenGLState();
	initFBOs();
	initModels();
	initShaders();
	initUniforms();	
	glCheckError();

    //for background
    initBackground();
    
    //for viewing using mouse
    glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwGetCursorPos(glWindow, &xpos, &ypos);
    glfwSetCursorPosCallback(glWindow, cursor_position_callback);
    
	while (!glfwWindowShouldClose(glWindow)) {
		renderScene();

		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}

	//close GL context and any other GLFW resources
	glfwTerminate();

	return 0;
}
