#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stack>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Utils.h"
using namespace std;

#define numVAOs12 1
#define numVBOs12 2

float cameraX12, cameraY12, cameraZ12;
GLuint renderingProgram12;
GLuint vao12[numVAOs12];
GLuint vbo12[numVBOs12];

// variable allocation for display
GLuint mvLoc12, projLoc12;
int width12, height12;
float aspect12;
glm::mat4 pMat12, vMat12, mMat12, mvMat12;

stack<glm::mat4> mvStack12;

void setupVertices12(void) {
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
	glGenVertexArrays(numVAOs12, vao12);
	glBindVertexArray(vao12[0]);

	glGenBuffers(numVBOs12, vbo12);
	glBindBuffer(GL_ARRAY_BUFFER, vbo12[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo12[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);
}

void display12(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram12);

	mvLoc12 = glGetUniformLocation(renderingProgram12, "mv_matrix");
	projLoc12 = glGetUniformLocation(renderingProgram12, "proj_matrix");

	vMat12 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX12, -cameraY12, -cameraZ12));
	mvStack12.push(vMat12);

	glUniformMatrix4fv(projLoc12, 1, GL_FALSE, glm::value_ptr(pMat12));

	// ----------------------  pyramid == sun
	mvStack12.push(mvStack12.top());
	mvStack12.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	mvStack12.push(mvStack12.top());
	mvStack12.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(1.0, 0.0, 0.0));
	glUniformMatrix4fv(mvLoc12, 1, GL_FALSE, glm::value_ptr(mvStack12.top()));

	glBindBuffer(GL_ARRAY_BUFFER, vbo12[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 18);
	mvStack12.pop();

	//-----------------------  cube == planet  
	mvStack12.push(mvStack12.top());
	mvStack12.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime) * 4.0, 0.0f, cos((float)currentTime) * 4.0f));
	mvStack12.push(mvStack12.top());
	mvStack12.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(mvLoc12, 1, GL_FALSE, glm::value_ptr(mvStack12.top()));
	glBindBuffer(GL_ARRAY_BUFFER, vbo12[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	mvStack12.pop();


	//-----------------------  smaller cube == moon
	mvStack12.push(mvStack12.top());
	mvStack12.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, sin((float)currentTime) * 2.0, cos((float)currentTime) * 2.0));
	mvStack12.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 0.0, 1.0));
	mvStack12.top() *= scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f));
	glUniformMatrix4fv(mvLoc12, 1, GL_FALSE, glm::value_ptr(mvStack12.top()));
	glBindBuffer(GL_ARRAY_BUFFER, vbo12[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	mvStack12.pop(); mvStack12.pop(); mvStack12.pop();
	mvStack12.pop();  // the final pop is for the view matrix

}

void init12(GLFWwindow* window) {
	renderingProgram12 = Utils::createShaderProgram("vertexShader.matrix_stack", "fragShader.matrix_stack");
	glfwGetFramebufferSize(window, &width12, &height12);
	aspect12 = (float)width12 / (float)height12;
	pMat12 = glm::perspective(glm::radians(60.0f), aspect12, 0.1f, 1000.0f);
	cameraX12 = 0.0f;
	cameraY12 = 0.0f;
	cameraZ12 = 12.0f;
	setupVertices12();
}


void window_size_callback12(GLFWwindow* win, int newWidth, int newHeight) {
	aspect12 = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat12 = glm::perspective(1.0472f, aspect12, 0.1f, 1000.0f);
}


int main12(void) {
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
	glfwSetWindowSizeCallback(window, window_size_callback12);
	init12(window);
	while (!glfwWindowShouldClose(window)) {
		display12(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_FAILURE);
}