#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "my_camera.h"
#include "landscape.h"
#include <iostream>
#include "skybox.h"
#include "car.h"


#define SQURE_LENGTH 10
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, Car* );

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 60.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//car's physics
float dTime = 1;
float Time = 0;

#define MAX_LEFT -45
#define MAX_RIGHT 45
#define MAX_FORWARD 0.2
#define MAX_BACKWARD -0.1
#define K_TURNING 100
#define K_VELOCITY 200

float FogDensity = 0.0f;

enum lastKey {
	L,
	R,
	NONE,
};

lastKey last;
//MyCamera
MyCamera mycamera(glm::vec3(20.0f, 6.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));


enum cameraID {
	GENERAL,
	BEHIND_CAR,
	IN_CAR
};

cameraID camID = GENERAL;

// timing
float deltaTime = 0.1f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(10.2f, 10.0f, 10.0f);

float Rotate = 0.0f;
//car model handle
Car *carModel;

void creatingWalls(Shader shader, Model wall) {
	
	glm::mat4  model;
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	model = glm::translate(model, glm::vec3(-SQURE_LENGTH*4/2, 1.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	wall.Draw(shader);
	
	for (int i = 0; i < SQURE_LENGTH; i++) {
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		wall.Draw(shader);
	}

	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 2.0f));
	model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	wall.Draw(shader);

	for (int i = 0; i < SQURE_LENGTH; i++) {
		model = glm::translate(model, glm::vec3(-4.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		wall.Draw(shader);
	}

	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 2.0f));
	model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	wall.Draw(shader);

	for (int i = 0; i < SQURE_LENGTH; i++) {
		model = glm::translate(model, glm::vec3(-4.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		wall.Draw(shader);
	}

	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 2.0f));
	model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	wall.Draw(shader);

	for (int i = 0; i < SQURE_LENGTH; i++) {
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		shader.setMat4("model", model);
		wall.Draw(shader);
	}


}


int main()
{

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif



														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	//Shader ourShader("1.model_loading.vs", "1.model_loading.fs");
	Shader loadingShader("vs.vs", "fs.fs");

	// wszystko do skyboxa
	Shader skyboxShader("shaders/skybox/skybox.vs", "shaders/skybox/skybox.fs");
	Skybox sky(50);
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);
	
	// load models
	// -----------
	Model ourModel("resources/objects/landscape/plansza.obj");
	Model wall("resources/objects/wall/wall.obj");
	Model grass("resources/objects/graas/grass.obj");
	Model wheel("resources/objects/car/back_wheel.obj");
	Model tree("resources/objects/nanosuit/nanosuit.obj");
	
	
	carModel = new Car();
	carModel->moveTranslate(glm::vec3(0.0f, 2.45f, 0.0f));

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glm::mat4 mProjection;
	mProjection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window, carModel);

		loadingShader.use();
		loadingShader.setFloat("FogDensity", FogDensity);
		glm::mat4 model;

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glClearColor(0.6f, 0.6f, 0.6f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int iModelViewLoc = glGetUniformLocation(loadingShader.ID, "view");
		int iProjectionLoc = glGetUniformLocation(loadingShader.ID, "projection");
		glUniformMatrix4fv(iProjectionLoc, 1, GL_FALSE, glm::value_ptr(mProjection));
		glm::mat4 mModelView;
		glm::mat4 skybox;
		switch (camID) {
		case GENERAL:
			skybox = glm::translate(skybox, mycamera.getPosition());
			mModelView = mycamera.GetViewMatrix();
			break;
		case BEHIND_CAR:
			skybox = glm::translate(skybox, carModel->getBehindCameraPostion());
			mModelView = carModel->getBehindCamera();
			break;

		case IN_CAR:
			skybox = glm::translate(skybox, carModel->getInCameraPostion());
			mModelView = carModel->getInCamera();
			break;
		}
		

		glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mModelView));

		loadingShader.setFloat("isSkybox", 1);
		
		// draw skybox cube
		
		//skybox = glm::translate(skybox, mycamera.getPosition());
		skybox = glm::rotate(skybox, 0.0f, glm::vec3(0.0f, 0.5f, 0.0f));
		skybox = glm::scale(skybox, glm::vec3(50.0f, 50.0f, 50.0f));
		skyboxShader.setMat4("model", skybox);
		sky.Draw(skyboxShader);
		loadingShader.setFloat("isSkybox", 0);

		// draw flooor
		model = glm::mat4();
		
		lightPos = carModel->getLightPosition();

		loadingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		loadingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		loadingShader.setVec3("lightPos", lightPos);
		loadingShader.setMat4("model", model);


		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		grass.Draw(loadingShader);

		model = glm::mat4();
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 0.0f));
		loadingShader.setMat4("model", model);
		tree.Draw(loadingShader);

		// draw car
		loadingShader.setVec3("viewPos", mycamera.getPosition());
		model = glm::rotate(model, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		loadingShader.setMat4("model", model);
		carModel->Draw(loadingShader, mycamera);



		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, Car *car)
{
	float tmpTurning = car->getTurning();
	float tmpVelocity = car->getVelocity();
	bool noneTurning = false;
	bool noneVelocity = false;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		car->setVelocity(tmpVelocity + (MAX_FORWARD - tmpVelocity) / K_VELOCITY);
	}
	else noneVelocity = true;

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		car->setVelocity(tmpVelocity + (MAX_BACKWARD - tmpVelocity) / K_VELOCITY);
	}
	else if (noneVelocity) {
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			car->setVelocity(tmpVelocity - tmpVelocity / 50);
		}
		else
		{
			car->setVelocity(tmpVelocity - tmpVelocity / 1000);
		}
		
	}

	car->moveTranslate(glm::vec3(-car->getVelocity()*sin((car->getRotation() + car->getTurning())*M_PI / 180), 0.00f, car->getVelocity()*cos((car->getRotation() + car->getTurning())*M_PI/180)));

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		car->setTurning(tmpTurning + (MAX_RIGHT - tmpTurning) / K_TURNING);
	}
	else noneTurning = true;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		car->setTurning(tmpTurning + (MAX_LEFT - tmpTurning) / K_TURNING);
	}
	else if (noneTurning) {
		car->setTurning(tmpTurning - tmpTurning / K_TURNING);
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
		mycamera.MoveForward();
	}
		camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			camera.ProcessKeyboard(BACKWARD, deltaTime);
			mycamera.MoveBackward();
	}
		
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			camera.ProcessKeyboard(LEFT, deltaTime);
			mycamera.MoveLeft();
	}
		
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			camera.ProcessKeyboard(RIGHT, deltaTime);
			mycamera.MoveRight();
	}


		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
			Rotate += 0.1;
		}
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
			Rotate -= 0.1;
		}
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
			camID = GENERAL;
		}
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
			camID = BEHIND_CAR;
		}
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
			camID = IN_CAR;
		}

		if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
			FogDensity += 0.0001;
		}
		if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
			if (FogDensity > 0)
				FogDensity -= 0.0001;
		}
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
			car->incrementWindowTransparency();
		}
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
			car->decrementWindowTransparency();
		}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;


	if (camID == BEHIND_CAR) carModel->behindCamera->MoveMouse(xoffset, yoffset);
	if (camID == GENERAL)mycamera.MoveMouse(xoffset, yoffset);
	if (camID == IN_CAR) carModel->inCamera->MoveMouse(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}