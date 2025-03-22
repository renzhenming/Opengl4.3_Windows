#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include "Utils.h"

using namespace std;

#define numVAOs 1
GLuint renderingProgram5;
GLuint vao5[numVAOs];
GLuint offsetLoc;

float x = 0.0f;
float inc = 0.01f;

void init5(GLFWwindow* window) {
	renderingProgram5 = Utils::createShaderProgram("vertexShader.anim", "fragShader.anim");
	glGenVertexArrays(numVAOs, vao5);
	glBindVertexArray(vao5[0]);
}

void display5(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram5);

	x += inc;
	if (x > 1.0f) {
		inc = -0.01f;
	}
	if (x < -1.0f) {
		inc = 0.01f;
	}
	offsetLoc = glGetUniformLocation(renderingProgram5, "offset");
	glProgramUniform1f(renderingProgram5, offsetLoc, x);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main() {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(400, 200, "rzm title", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);
	init5(window);
	while (!glfwWindowShouldClose(window)) {
		display5(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}