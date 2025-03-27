#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/soil2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Utils.h"


#define numVAOs13 1
#define numVBOs13 2

float cameraX13, cameraY13, cameraZ13;
float pyrLocX13, pyrLocY13, pyrLocZ13;
GLuint renderingProgram13;
GLuint vao13[numVAOs13];
GLuint vbo13[numVBOs13];

// variable allocation for display
GLuint mvLoc13, projLoc13;
int width13, height13;
float aspect13;
glm::mat4 pMat13, vMat13, mMat13, mvMat13;

GLuint brickTexture13;

void setupVertex13() {
	float pyramidPositions[54] =
	{ -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,    //front
		1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,    //right
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  //back
		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,  //left
		-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, //LF
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f  //RR
	};
	float textureCoordinates[36] =
	{ 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(numVAOs13, vao13);
	glBindVertexArray(vao13[0]);
	glGenBuffers(numVBOs13, vbo13);

	glBindBuffer(GL_ARRAY_BUFFER, vbo13[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo13[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinates), textureCoordinates, GL_STATIC_DRAW);
}

void init13(GLFWwindow* window) {
	renderingProgram13 = Utils::createShaderProgram("vertexShader.texture", "fragShader.texture");
	cameraX13 = 0.0f;
	cameraY13 = 0.0f;
	cameraZ13 = 4.0f;
	pyrLocX13 = 0.0f;
	pyrLocY13 = 0.0f;
	pyrLocZ13 = 0.0f;
	setupVertex13();
	glfwGetFramebufferSize(window, &width13, &height13);
	aspect13 = (float)width13 / (float)height13;
	pMat13 = glm::perspective(glm::radians(60.0f), aspect13, 0.1f, 1000.0f);
	brickTexture13 = Utils::loadTexture("brick1.jpg");
}

void display13(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram13);

	mvLoc13 = glGetUniformLocation(renderingProgram13, "mv_matrix");
	projLoc13 = glGetUniformLocation(renderingProgram13, "proj_matrix");

	vMat13 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX13, -cameraY13, -cameraZ13));
	mMat13 = glm::translate(glm::mat4(1.0f), glm::vec3(pyrLocX13, pyrLocY13, pyrLocZ13));
	mMat13 = glm::rotate(mMat13, -0.45f, glm::vec3(1.0f, 0.0f, 0.0f));
	mMat13 = glm::rotate(mMat13, 0.61f, glm::vec3(0.0f, 1.0f, 0.0f));
	mMat13 = glm::rotate(mMat13, 0.00f, glm::vec3(0.0f, 0.0f,1.0f));

	mvMat13 = vMat13 * mMat13;

	glUniformMatrix4fv(mvLoc13, 1, GL_FALSE, glm::value_ptr(mvMat13));
	glUniformMatrix4fv(projLoc13, 1, GL_FALSE, glm::value_ptr(pMat13));

	glBindBuffer(GL_ARRAY_BUFFER, vbo13[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo13[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brickTexture13);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 18);
}

void window_size_callback13(GLFWwindow* window, int width, int height) {
	aspect13 = (float)width / (float)height;
	glViewport(0, 0, width, height);
	pMat13 = glm::perspective(glm::radians(60.0f), aspect13, 0.1f, 1000.0f);
}

int main(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow *window = glfwCreateWindow(600, 600, "rzm title", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);
	glfwSetWindowSizeCallback(window, window_size_callback13);
	init13(window);
	while (!glfwWindowShouldClose(window)) {
		display13(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}