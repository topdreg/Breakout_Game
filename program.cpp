#define GLEW_STATIC
#include <GL/glew.h> 
#include <GLFW/glfw3.h> 

#include "game.h" 
#include "resource_manager.h"

//GLFW function declarations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode); 

//Width and height of the screen 
const GLuint SCREEN_WIDTH = 800; 
const GLuint SCREEN_HEIGHT = 600; 

Game Invaders(SCREEN_WIDTH, SCREEN_HEIGHT); 

int main(int argc, char *argv[]) 
{
	glfwInit(); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders", nullptr, nullptr); 
	glfwMakeContextCurrent(window); 

	glewExperimental = GL_TRUE; 
	glewInit(); 
	glGetError(); //called once to catch glewInit() bug, all other errors are then from our application

	glfwSetKeyCallback(window, key_callback); 

	//OpenGL configuration
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); 
	glEnable(GL_CULL_FACE); 
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	//Initialize game
	Invaders.Init(); 

	//DeltaTime variables
	GLfloat deltaTime = 0.0f; 
	GLfloat lastFrame = 0.0f; 

	//Start game within menu state
	Invaders.State = GAME_ACTIVE; 
	
	while (!glfwWindowShouldClose(window))
	{
		//Calculate delta time 
		GLfloat currentFrame = glfwGetTime(); 
		deltaTime = currentFrame - lastFrame; 
		lastFrame = currentFrame; 
		glfwPollEvents(); 

		//Manage user input 
		Invaders.ProcessInput(deltaTime); 

		//Update game state
		Invaders.Update(deltaTime); 

		//Render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT); 
		Invaders.Render(); 

		glfwSwapBuffers(window); 
	}
	
	//delete all resources as loaded using the resource manager
	ResourceManager::Clear(); 

	glfwTerminate(); 
	return 0; 
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE); 
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS) 
			Invaders.Keys[key] = GL_TRUE; 
		else if (action == GLFW_RELEASE) 
			Invaders.Keys[key] = GL_FALSE; 
	}
}

