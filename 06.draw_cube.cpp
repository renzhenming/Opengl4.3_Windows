#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Utils.h"
using namespace std;

#define numVAOs6 1
#define numVBOs6 2


GLuint vao6[numVAOs6];
GLuint vbo6[numVBOs6];

GLuint program6;
GLuint mvLoc6;
GLuint projLoc6;

float cameraX6, cameraY6, cameraZ6;
float cubeX6, cubeY6, cubeZ6;

int width6;
int height6;
float aspect6;

glm::mat4 pMat6, vMat6, mMat6, mvMat6;

void setupVertices06(void) {
	float vertexPositions[108] = {
		-1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
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

	glGenVertexArrays(numVAOs6, vao6);
	glBindVertexArray(vao6[0]);
	glGenBuffers(numVBOs6, vbo6);
	glBindBuffer(GL_ARRAY_BUFFER, vbo6[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}
void init6(GLFWwindow* window) {
	program6 = Utils::createShaderProgram("vertexShader.cube", "fragShader.cube");
	cameraX6 = 0.0f;
	cameraY6 = 0.0f;
	cameraZ6 = 8.0f;
	cubeX6 = 0.0f;
	cubeY6 = -2.0f;
	cubeZ6 = 0.0f;
	setupVertices06();
}


void display6(GLFWwindow* window,double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(program6);
	mvLoc6 = glGetUniformLocation(program6, "mv_matrix");
	projLoc6 = glGetUniformLocation(program6, "proj_matrix");
	glfwGetFramebufferSize(window, &width6, &height6);
	aspect6 = (float)width6 / (float)height6;
	pMat6 = glm::perspective(1.0472f, aspect6, 0.1f, 1000.0f);

	vMat6 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX6, -cameraY6, -cameraZ6));
	mMat6 = glm::translate(glm::mat4(1.0f), glm::vec3(cubeX6, cubeY6, cubeZ6));
	mvMat6 = vMat6 * mMat6;

	glUniformMatrix4fv(mvLoc6, 1, GL_FALSE, glm::value_ptr(mvMat6));
	glUniformMatrix4fv(projLoc6, 1, GL_FALSE, glm::value_ptr(pMat6));

	glBindBuffer(GL_ARRAY_BUFFER, vbo6[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

int main6(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow *window = glfwCreateWindow(1000, 600, "rzm titie", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);
	init6(window);
	while (!glfwWindowShouldClose(window)) {
		display6(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}