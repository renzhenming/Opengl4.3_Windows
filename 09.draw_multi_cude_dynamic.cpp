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

#define numVAOs9 1
#define numVBOs9 2


GLuint vao9[numVAOs9];
GLuint vbo9[numVBOs9];

GLuint program9;
GLuint mvLoc9;
GLuint projLoc9;

float cameraX9, cameraY9, cameraZ9;

int width9;
int height9;
int displayLoopi;
float aspect9;
float tf;
glm::mat4 pMat9, vMat9, tMat9, rMat9, mMat9, mvMat9;

void setupVertices9(void) {
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

	glGenVertexArrays(numVAOs9, vao9);
	glBindVertexArray(vao9[0]);
	glGenBuffers(numVBOs9, vbo9);
	glBindBuffer(GL_ARRAY_BUFFER, vbo9[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}
void init9(GLFWwindow* window) {
	program9 = Utils::createShaderProgram("vertexShader.cube_colorfull", "fragShader.cube_colorfull");
	glfwGetFramebufferSize(window, &width9, &height9);
	aspect9 = (float)width9 / (float)height9;
	pMat9 = glm::perspective(glm::radians(60.0f), aspect9, 0.1f, 1000.0f);
	cameraX9 = 0.0f;
	cameraY9 = 0.0f;
	cameraZ9 = 32.0f;
	setupVertices9();
}


void display9(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program9);
	mvLoc9 = glGetUniformLocation(program9, "mv_matrix");
	projLoc9 = glGetUniformLocation(program9, "proj_matrix");

	vMat9 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX9, -cameraY9, -cameraZ9));
	glUniformMatrix4fv(projLoc9, 1, GL_FALSE, glm::value_ptr(pMat9));

	for (displayLoopi = 0; displayLoopi < 100; displayLoopi++) {
		tf = currentTime + displayLoopi;
		tMat9 = glm::translate(glm::mat4(1.0f), glm::vec3(sin(0.35f * tf) * 8.0f, cos(0.52f * tf) * 8.0f, sin(0.7f * tf) * 8.0f));
		rMat9 = glm::rotate(glm::mat4(1.0f), 2 * tf, glm::vec3(0.0f, 1.0f, 0.0f));
		rMat9 = glm::rotate(rMat9, 2 * tf, glm::vec3(1.0f, 0.0f, 0.0f));
		rMat9 = glm::rotate(rMat9, 2 * tf, glm::vec3(0.0f, 0.0f, 1.0f));

		mMat9 = tMat9 * rMat9;

		mvMat9 = vMat9 * mMat9;
		glUniformMatrix4fv(mvLoc9, 1, GL_FALSE, glm::value_ptr(mvMat9));


		glBindBuffer(GL_ARRAY_BUFFER, vbo9[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

}

int main9(void) {
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
	init9(window);
	while (!glfwWindowShouldClose(window)) {
		display9(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}