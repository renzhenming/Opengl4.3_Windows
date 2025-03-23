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

#define numVAOs8 1
#define numVBOs8 2


GLuint vao8[numVAOs8];
GLuint vbo8[numVBOs8];

GLuint program8;
GLuint mvLoc8;
GLuint projLoc8;

float cameraX8, cameraY8, cameraZ8;

int width8;
int height8;
float aspect8;

glm::mat4 pMat8, vMat8, tMat8, rMat8, mMat8, mvMat8;

void setupVertices8(void) {
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

	glGenVertexArrays(numVAOs8, vao8);
	glBindVertexArray(vao8[0]);
	glGenBuffers(numVBOs8, vbo8);
	glBindBuffer(GL_ARRAY_BUFFER, vbo8[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}
void init8(GLFWwindow* window) {
	program8 = Utils::createShaderProgram("vertexShader.cube_colorfull", "fragShader.cube_colorfull");
	glfwGetFramebufferSize(window, &width8, &height8);
	aspect8 = (float)width8 / (float)height8;
	pMat8 = glm::perspective(glm::radians(60.0f), aspect8, 0.1f, 1000.0f);
	cameraX8 = 0.0f;
	cameraY8 = 0.0f;
	cameraZ8 = 8.0f;
	setupVertices8();
}


void display8(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program8);
	mvLoc8 = glGetUniformLocation(program8, "mv_matrix");
	projLoc8 = glGetUniformLocation(program8, "proj_matrix");

	vMat8 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX8, -cameraY8, -cameraZ8));
	std::cout << "currentTime:" << currentTime << std::endl;

	tMat8 = glm::translate(glm::mat4(1.0f), glm::vec3(sin(currentTime), cos(currentTime), sin(currentTime)));
	std::cout << "sin(currentTime):" << sin(currentTime) << std::endl;
	std::cout << "cos(currentTime):" << cos(currentTime) << std::endl;
	std::cout << "sin(currentTime):" << sin(currentTime) << std::endl;
	rMat8 = glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
	rMat8 = glm::rotate(rMat8, (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));
	rMat8 = glm::rotate(rMat8, (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f));

	mMat8 = tMat8 * rMat8;

	mvMat8 = vMat8 * mMat8;

	glUniformMatrix4fv(mvLoc8, 1, GL_FALSE, glm::value_ptr(mvMat8));
	glUniformMatrix4fv(projLoc8, 1, GL_FALSE, glm::value_ptr(pMat8));

	glBindBuffer(GL_ARRAY_BUFFER, vbo8[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

int main8(void) {
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
	init8(window);
	while (!glfwWindowShouldClose(window)) {
		display8(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}