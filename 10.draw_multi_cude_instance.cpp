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

#define numVAOs10 1
#define numVBOs10 1

float cameraX10, cameraY10, cameraZ10;
GLuint renderingProgram10;
GLuint vao10[numVAOs10];
GLuint vbo10[numVBOs10];

// variable allocation for display
GLuint mLoc10, vLoc10, projLoc10, tfLoc10;
int width10, height10;
float aspect10, timeFactor10;
glm::mat4 pMat10, vMat10, mMat10, mvMat10;

void setupVertices10(void) {
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
	glGenVertexArrays(1, vao10);
	glBindVertexArray(vao10[0]);
	glGenBuffers(1, vbo10);
	glBindBuffer(GL_ARRAY_BUFFER, vbo10[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}

void init10(GLFWwindow* window) {
	renderingProgram10 = Utils::createShaderProgram("vertexShader.cube_instance", "fragShader.cube_instance");
	glfwGetFramebufferSize(window, &width10, &height10);
	aspect10 = (float)width10 / (float)height10;
	pMat10 = glm::perspective(glm::radians(60.0f), aspect10, 0.1f, 1000.0f);
	// Z=32.0f when 24 instances, 420.0f when 100000 instances
	cameraX10 = 0.0f;cameraY10 = 0.0f;cameraZ10 = 420;
	setupVertices10();
}

void window_size_callback10(GLFWwindow* window, int width, int height) {
	aspect10 = (float)width / (float)height;
	glViewport(0, 0, width, height);
	pMat10 = glm::perspective(glm::radians(60.0f), aspect10, 0.1f, 1000.0f);
}

void display10(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram10);

	vLoc10 = glGetUniformLocation(renderingProgram10, "v_matrix");
	projLoc10 = glGetUniformLocation(renderingProgram10, "proj_matrix");

	vMat10 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX10, -cameraY10, -cameraZ10));

	glUniformMatrix4fv(vLoc10, 1, GL_FALSE, glm::value_ptr(vMat10));
	glUniformMatrix4fv(projLoc10, 1, GL_FALSE, glm::value_ptr(pMat10));

	timeFactor10 = (float)currentTime;
	tfLoc10 = glGetUniformLocation(renderingProgram10, "tf");
	glUniform1f(tfLoc10, timeFactor10);

	glBindBuffer(GL_ARRAY_BUFFER, vbo10[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 100000);	// 0, 36, 24  (or 100000)
}

int main10(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "rzm title", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);
	glfwSetWindowSizeCallback(window, window_size_callback10);
	init10(window);
	while (!glfwWindowShouldClose(window)) {
		display10(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}