//std
	#include <iostream>
	#include <fstream>
	#include <windows.h>
	#include <cmath>
	#include <vector>
//opengl and libs
	#include <glad/glad.h>
	#include <GLFW/glfw3.h>
//glm
	#include <glm/vec3.hpp>
	#include <glm/vec4.hpp>
	#include <glm/mat4x4.hpp>
	#include <glm/ext/matrix_transform.hpp>
	#include <glm/ext/matrix_clip_space.hpp>
	#include <glm/ext/scalar_constants.hpp>
	#include <glm/glm.hpp>
	#include <glm/gtc/type_ptr.hpp>
//ms
	#include <conio.h>
//project
	#include "Class/Program/program.hpp"
	#include "Class/MyColoredPolyModel/MyColoredPolyModel.hpp"
	#include "Class/myWorldThing/myWorldThing.hpp"
	#include "main.hpp"

using namespace std;

char moveX = 0, moveY = 0, moveZ = 0, rot = 0;
unsigned char axis = 0;

int main()
{
	//initialize glfw
		if (!glfwInit())
		{
			OutputDebugStringW(L"FATAL: failed to initialize glfw\n");
			exit(1);
		}
	//initialize window
		GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
		if (window == NULL)
		{
			OutputDebugStringW(L"FATAL: failed to initialize glfw window\n");
			exit(1);
		}
		glfwMakeContextCurrent(window);
		glfwSetKeyCallback(window, key_callback);
		glfwSwapInterval(1);
	//initialize glad
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			OutputDebugStringW(L"FATAL: failed to initialize glad\n");
			exit(1);
		}
	//init depth buffer
		glad_glEnable(GL_DEPTH_TEST);
	//set clear colour
		glClearColor(1, 1, 1, 1);
	//set up VAO(s)
		GLuint VAOs[1];
		glad_glGenVertexArrays(1, VAOs);
		glad_glBindVertexArray(VAOs[0]);
	//shader setup
		//compile
			Program myFirstTransform(
				"Shaders/MyFirstTransform/myFirstTransform.frag",
				"Shaders/MyFirstTransform/myFirstTransform.vert"
			);
		//get uniform locations
			myFirstTransform.Use();
			GLuint
				colorLocation		= glad_glGetUniformLocation(myFirstTransform.handle, "color"),
				projectionLocation	= glad_glGetUniformLocation(myFirstTransform.handle, "projection"),
				viewLocation		= glad_glGetUniformLocation(myFirstTransform.handle, "view"),
				modelLocation		= glad_glGetUniformLocation(myFirstTransform.handle, "model");
			//check for failure
			if (projectionLocation == -1 || viewLocation == -1 || modelLocation == -1)
			{
				OutputDebugStringW(L"FATAL: couldn't get uniform location\n");
				exit(1);
			}
	//set up world stuff
		glad_glPointSize(10);
		vector<MyWorldThing*> worldThings{
			new MyWorldThing(
				new MyColoredPolyModel(
					6,
					new float[] {
						//front
						-0.1f, -0.1f,  0.1f, 1.0f,
						 0.1f, -0.1f,  0.1f, 1.0f,
						 0.1f,  0.1f,  0.1f, 1.0f,
						-0.1f,  0.1f,  0.1f, 1.0f,
						-0.1f, -0.1f,  0.1f, 1.0f,
						//right
						 0.1f, -0.1f,  0.1f, 1.0f,
						 0.1f, -0.1f, -0.1f, 1.0f,
						 0.1f,  0.1f, -0.1f, 1.0f,
						 0.1f,  0.1f,  0.1f, 1.0f,
						 0.1f, -0.1f,  0.1f, 1.0f,
						//back
						-0.1f, -0.1f,  -0.1f, 1.0f,
						 0.1f, -0.1f,  -0.1f, 1.0f,
						 0.1f,  0.1f,  -0.1f, 1.0f,
						-0.1f,  0.1f,  -0.1f, 1.0f,
						-0.1f, -0.1f,  -0.1f, 1.0f,
						//left
						-0.1f, -0.1f,  0.1f, 1.0f,
						-0.1f, -0.1f, -0.1f, 1.0f,
						-0.1f,  0.1f, -0.1f, 1.0f,
						-0.1f,  0.1f,  0.1f, 1.0f,
						-0.1f, -0.1f,  0.1f, 1.0f,
						//top
						-0.1f, -0.1f,  0.1f, 1.0f,
						 0.1f, -0.1f,  0.1f, 1.0f,
						 0.1f, -0.1f, -0.1f, 1.0f,
						-0.1f, -0.1f, -0.1f, 1.0f,
						-0.1f, -0.1f,  0.1f, 1.0f,
						//bottom
						-0.1f, 0.1f,  0.1f, 1.0f,
						 0.1f, 0.1f,  0.1f, 1.0f,
						 0.1f, 0.1f, -0.1f, 1.0f,
						-0.1f, 0.1f, -0.1f, 1.0f,
						-0.1f, 0.1f,  0.1f, 1.0f,
						},
					new glm::vec3[] {
						glm::vec3(1.0f, 0.0f, 0.0f),
						glm::vec3(0.0f, 1.0f, 0.0f),
						glm::vec3(0.0f, 0.0f, 1.0f),
						glm::vec3(1.0f, 0.5f, 0.0f),
						glm::vec3(1.0f, 0.0f, 1.0f),
						glm::vec3(0.0f, 1.0f, 1.0f)
					},
					new unsigned char[] {5, 5, 5, 5, 5, 5},
					colorLocation,
					VAOs[0]
				),
				glm::vec3(-0.2f, 0.2f,-2.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				projectionLocation,
				viewLocation,
				modelLocation
			),
			new MyWorldThing(
				/*size_t nPolygons, 
				float* verts, 
				glm::vec3* polygonColors, 
				unsigned char* polygonBoundaries,
				GLuint colorLocation, 
				GLuint VAO*/
				new MyColoredPolyModel(
					1,
					new float[] {
						0.0f, 0.0f, 0.0f, 1.0f,
						0.2f, 0.0f, 0.0f, 1.0f,
						0.1f, 0.1f, 0.0f, 1.0f,
						0.0f, 0.0f, 0.0f, 1.0f
					},
					new glm::vec3[] {
						glm::vec3(1.0f, 0.0f, 0.0f)
					},
					new unsigned char[] {4},
					colorLocation,
					VAOs[0]
				),
				glm::vec3(0.0f, 0.0f, -1.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				projectionLocation,
				viewLocation,
				modelLocation
			)
		};
		MyWorldThing* first = worldThings[0];
	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//print debug to console
			system("cls");
			cout << "triangle:"
				<< "\n\tposition"
					<< "\n\t\tx: "		<< first->position.x
					<< "\n\t\ty: "		<< first->position.y
					<< "\n\t\tz: "		<< first->position.z
				<< "\n\trotation: "
					<< "\n\t\tcur: "	<< (int)axis
					<< "\n\t\tx: "		<< first->angle.x
					<< "\n\t\ty: "		<< first->angle.y
					<< "\n\t\tz: "		<< first->angle.z
			;
		//update world state
			first->position.x += moveX * MOVE_RATE;
			first->position.y += moveY * MOVE_RATE;
			first->position.z += moveZ * MOVE_RATE;
			switch (axis) {
				case 0:
					first->angle.x += rot * ROTATE_RATE;
				break;
				case 1:
					first->angle.y += rot * ROTATE_RATE;
				break;
				case 2:
					first->angle.z += rot * ROTATE_RATE;
				break;
			}
			moveX = 0;
			moveY = 0;
			moveZ = 0;
			rot = 0;
		//clear framebuffers
			glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//draw all objects
			for (auto& thing : worldThings)
				thing->Draw();
		//glfw stuff
			glfwSwapBuffers(window);
			glfwPollEvents();
		Sleep(FRAME_DELAY);
	}

	//free resources
		//glfw
		glfwDestroyWindow(window);
		glfwTerminate();
		//vertex buffer
		//vertex array
}

/// <summary>
/// called whenever a key is pressed
/// </summary>
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
		case GLFW_KEY_W:
			moveY = -1;
		break;
		case GLFW_KEY_A:
			moveX = 1;
		break;
		case GLFW_KEY_S:
			moveY = 1;
		break;
		case GLFW_KEY_D:
			moveX = -1;
		break;
		case GLFW_KEY_R:
			moveZ = -1;
		break;
		case GLFW_KEY_F:
			moveZ = 1;
		break;
		case GLFW_KEY_Q:
			rot = 1;
		break;
		case GLFW_KEY_E:
			rot = -1;
		break;
		case GLFW_KEY_SPACE:
			if(axis<2)
				axis++;
			else
				axis = 0;
		break;
		default:
		break;
	}
}