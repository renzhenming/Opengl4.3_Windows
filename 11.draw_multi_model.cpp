#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Utils.h"
using namespace std;

#define numVAOs11 1
#define numVBOs11 2

float cameraX11, cameraY11, cameraZ11;
float cubeLocX11, cubeLocY11, cubeLocZ11;
float pyrLocX11, pyrLocY11, pyrLocZ11;
GLuint renderingProgram11;
GLuint vao11[numVAOs11];
GLuint vbo11[numVBOs11];

// variable allocation for display
GLuint mvLoc11, projLoc11;
int width11, height11;
float aspect11;
glm::mat4 pMat11, vMat11, mMat11, mvMat11;


void window_size_callback11(GLFWwindow* win, int newWidth, int newHeight) {
	aspect11 = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat11 = glm::perspective(glm::radians(60.0f), aspect11, 0.1f, 1000.0f);
}

void setupVertices11(void) {
	float vertexPositions[108] =
	{ -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
	};
	float pyramidPositions[54] =
	{ -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,    //front
		1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,    //right
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  //back
		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,  //left
		-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, //LF
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f  //RR
	};
	glGenVertexArrays(1, vao11);
	glBindVertexArray(vao11[0]);
	glGenBuffers(numVBOs11, vbo11);

	glBindBuffer(GL_ARRAY_BUFFER, vbo11[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo11[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);
}

void init11(GLFWwindow* window) {
	renderingProgram11 = Utils::createShaderProgram("vertexShader.multi_model", "fragShader.multi_model");

	glfwGetFramebufferSize(window, &width11, &height11);
	aspect11 = (float)width11 / (float)height11;
	pMat11 = glm::perspective(glm::radians(60.0f), aspect11, 0.1f, 1000.0f);

	cameraX11 = 0.0f; cameraY11 = 0.0f; cameraZ11 = 8.0f;
	cubeLocX11 = 0.0f; cubeLocY11 = -2.0f; cubeLocZ11 = 0.0f;
	pyrLocX11 = 2.0f; pyrLocY11 = 2.0f; pyrLocZ11 = 0.0f;
	setupVertices11();
}


void display11(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(renderingProgram11);

	mvLoc11 = glGetUniformLocation(renderingProgram11, "mv_matrix");
	projLoc11 = glGetUniformLocation(renderingProgram11, "proj_matrix");

	vMat11 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX11, -cameraY11, -cameraZ11));

	// draw the cube using buffer #0
	mMat11 = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX11, cubeLocY11, cubeLocZ11));
	mvMat11 = vMat11 * mMat11;

	glUniformMatrix4fv(mvLoc11, 1, GL_FALSE, glm::value_ptr(mvMat11));
	glUniformMatrix4fv(projLoc11, 1, GL_FALSE, glm::value_ptr(pMat11));

	glBindBuffer(GL_ARRAY_BUFFER, vbo11[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	// draw the pyramid using buffer #1
	mMat11 = glm::translate(glm::mat4(1.0f), glm::vec3(pyrLocX11, pyrLocY11, pyrLocZ11));
	mvMat11 = vMat11* mMat11;

	glUniformMatrix4fv(mvLoc11, 1, GL_FALSE, glm::value_ptr(mvMat11));
	glUniformMatrix4fv(projLoc11, 1, GL_FALSE, glm::value_ptr(pMat11));

	glBindBuffer(GL_ARRAY_BUFFER, vbo11[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, 18);
}

int main11(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter 4 - program 3", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	glfwSetWindowSizeCallback(window, window_size_callback11);

	init11(window);

	while (!glfwWindowShouldClose(window)) {
		display11(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}