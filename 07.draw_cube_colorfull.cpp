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

#define numVAOs7 1
#define numVBOs7 2


GLuint vao7[numVAOs7];
GLuint vbo7[numVBOs7];

GLuint program7;
GLuint mvLoc7;
GLuint projLoc7;

float cameraX7, cameraY7, cameraZ7;
float cubeX7, cubeY7, cubeZ7;

int width7;
int height7;
float aspect7;

glm::mat4 pMat7, vMat7, mMat7, mvMat7;

void setupVertices7(void) {
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

	glGenVertexArrays(numVAOs7, vao7);
	glBindVertexArray(vao7[0]);
	glGenBuffers(numVBOs7, vbo7);
	glBindBuffer(GL_ARRAY_BUFFER, vbo7[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}
void init7(GLFWwindow* window) {
	program7 = Utils::createShaderProgram("vertexShader.cube_colorfull", "fragShader.cube_colorfull");
	cameraX7 = 0.0f;
	cameraY7 = 0.0f;
	cameraZ7 = 8.0f;
	cubeX7 = 0.0f;
	cubeY7 = -2.0f;
	cubeZ7 = 0.0f;
	setupVertices7();
}


void display7(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(program7);
	mvLoc7 = glGetUniformLocation(program7, "mv_matrix");
	projLoc7 = glGetUniformLocation(program7, "proj_matrix");
	glfwGetFramebufferSize(window, &width7, &height7);
	aspect7 = (float)width7 / (float)height7;
	pMat7 = glm::perspective(1.0472f, aspect7, 0.1f, 1000.0f);

	vMat7 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX7, -cameraY7, -cameraZ7));
	mMat7 = glm::translate(glm::mat4(1.0f), glm::vec3(cubeX7, cubeY7, cubeZ7));
	mvMat7 = vMat7 * mMat7;

	glUniformMatrix4fv(mvLoc7, 1, GL_FALSE, glm::value_ptr(mvMat7));
	glUniformMatrix4fv(projLoc7, 1, GL_FALSE, glm::value_ptr(pMat7));

	glBindBuffer(GL_ARRAY_BUFFER, vbo7[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

int main7(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(1000, 600, "rzm titie", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);
	init7(window);
	while (!glfwWindowShouldClose(window)) {
		display7(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}