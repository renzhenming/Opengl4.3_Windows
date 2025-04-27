#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/soil2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Utils.h"
using namespace std;

#define numVAOs 1

float camerX31, cameraY31, cameraZ31;
float terLocX31, terLocY31, terLocZ31;
GLuint renderingProgram31;
GLuint vao31[numVAOs];

GLuint mvpLoc31;
int width31, height31;
float aspect31;
glm::mat4 pMat31, vMat31, mMat31, mvpMat31;

void init31(GLFWwindow* window) {
	renderingProgram31 = Utils::createShaderProgram("vertexShader.tessellation", 
		"tessCShader.tessellation", "tessEShader.tessellation", "fragShader.tessellation");
	camerX31 = 0.5f;
	cameraY31 = -0.5f;
	cameraZ31 = 2.0f;
	terLocX31 = 0.0f;
	terLocY31 = 0.0f;
	terLocZ31 = 0.0f;

	glfwGetFramebufferSize(window, &width31, &height31);
	aspect31 = (float)width31 / (float)height31;
	pMat31 = glm::perspective(glm::radians(60.0f), aspect31, 0.1f, 1000.0f);
	glGenVertexArrays(numVAOs, vao31);
	glBindVertexArray(vao31[0]);
}

void display31(GLFWwindow* window, float currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram31);

	vMat31 = glm::translate(glm::mat4(1.0f), glm::vec3(-camerX31, -cameraY31, -cameraZ31));
	mMat31 = glm::translate(glm::mat4(1.0f), glm::vec3(terLocX31, terLocY31, terLocZ31));
	mMat31 = glm::rotate(mMat31, glm::radians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mvpMat31 = pMat31 * vMat31 * mMat31;

	mvpLoc31 = glGetUniformLocation(renderingProgram31, "mvp_matrix");
	glUniformMatrix4fv(mvpLoc31, 1, GL_FALSE, glm::value_ptr(mvpMat31));

	glFrontFace(GL_CCW);
	glPatchParameteri(GL_PATCH_VERTICES, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_PATCHES, 0, 1);
}

void window_size_callback31(GLFWwindow* window,int width,int height) {
	aspect31 = (float)width / (float)height;
	glViewport(0, 0, width, height);
	pMat31 = glm::perspective(glm::radians(60.0f), aspect31, 0.1f, 1000.0f);
}
int main31(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow * window = glfwCreateWindow(800, 800, "rzm title", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);
	glfwSetWindowSizeCallback(window, window_size_callback31);
	init31(window);
	while (!glfwWindowShouldClose(window)) {
		display31(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}