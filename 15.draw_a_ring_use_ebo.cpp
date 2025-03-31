#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/soil2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Torus.h"
#include "Utils.h"
using namespace std;

#define numVAOs15 1
#define numVBOs15 4

float cameraX15, cameraY15, cameraZ15;
float torLocX15, torLocY15, torLocZ15;
GLuint renderingProgram15;
GLuint vao15[numVAOs15];
GLuint vbo15[numVBOs15];
GLuint brickTexture15;
float rotAmt15 = 0.0f;

// variable allocation for display
GLuint mvLoc15, projLoc15;
int width15, height15;
float aspect15;
glm::mat4 pMat15, vMat15, mMat15, mvMat15;

Torus myTorus(0.5f, 0.2f, 48);

void setupVertices15(void) {
	std::vector<int> ind = myTorus.getIndices();
	std::vector<glm::vec3> vert = myTorus.getVertices();
	std::vector<glm::vec2> tex = myTorus.getTexCoords();
	std::vector<glm::vec3> norm = myTorus.getNormals();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	for (int i = 0; i < myTorus.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}
	glGenVertexArrays(1, vao15);
	glBindVertexArray(vao15[0]);
	glGenBuffers(numVBOs15, vbo15);

	glBindBuffer(GL_ARRAY_BUFFER, vbo15[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo15[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * sizeof(float), &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo15[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * sizeof(float), &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo15[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(float), &ind[0], GL_STATIC_DRAW);
}


void init15(GLFWwindow* window) {
	renderingProgram15 = Utils::createShaderProgram("vertexShader.ebo", "fragShader.ebo");
	cameraX15 = 0.0f; cameraY15 = 0.0f; cameraZ15 = 2.0f;
	torLocX15 = 0.0f; torLocY15 = 0.0f; torLocZ15 = -0.5f;

	glfwGetFramebufferSize(window, &width15, &height15);
	aspect15 = (float)width15 / (float)height15;
	pMat15 = glm::perspective(glm::radians(60.0f), aspect15, 0.1f, 1000.0f);

	setupVertices15();

	brickTexture15 = Utils::loadTexture("brick1.jpg");
	glBindTexture(GL_TEXTURE_2D, brickTexture15);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
}

void window_size_callback15(GLFWwindow* win, int newWidth, int newHeight) {
	aspect15 = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat15 = glm::perspective(glm::radians(60.0f), aspect15, 0.1f, 1000.0f);
}


void display15(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram15);

	mvLoc15 = glGetUniformLocation(renderingProgram15, "mv_matrix");
	projLoc15 = glGetUniformLocation(renderingProgram15, "proj_matrix");

	vMat15 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX15, -cameraY15, -cameraZ15));
	mMat15 = glm::translate(glm::mat4(1.0f), glm::vec3(torLocX15, torLocY15, torLocZ15));
	//mMat *= glm::eulerAngleXYZ( glm::radians(30.0f), 0.0f, 0.0f);
	mMat15 = glm::rotate(mMat15, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	mvMat15 = vMat15 * mMat15;

	glUniformMatrix4fv(mvLoc15, 1, GL_FALSE, glm::value_ptr(mvMat15));
	glUniformMatrix4fv(projLoc15, 1, GL_FALSE, glm::value_ptr(pMat15));

	glBindBuffer(GL_ARRAY_BUFFER, vbo15[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo15[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brickTexture15);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo15[3]);
	glDrawElements(GL_TRIANGLES, myTorus.getIndices().size(), GL_UNSIGNED_INT, 0);
}

int main15(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(800, 800, "rzm title", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	glfwSetWindowSizeCallback(window, window_size_callback15);

	init15(window);

	while (!glfwWindowShouldClose(window)) {
		display15(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}