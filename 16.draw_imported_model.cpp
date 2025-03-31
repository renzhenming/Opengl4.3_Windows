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
#include "ImportedModel.h"
#include "Utils.h"
using namespace std;

#define numVAOs16 1
#define numVBOs16 3

float cameraX16, cameraY16, cameraZ16;
float objLocX16, objLocY16, objLocZ16;
GLuint renderingProgram16;
GLuint vao16[numVAOs16];
GLuint vbo16[numVBOs16];
GLuint shuttleTexture16;

// variable allocation for display
GLuint mvLoc16, projLoc16;
int width16, height16;
float aspect16;
glm::mat4 pMat16, vMat16, mMat16, mvMat16;


ImportedModel myModel("shuttle.obj");

void setupVertices16(void) {
	std::vector<glm::vec3> vert = myModel.getVertices();
	std::vector<glm::vec2> tex = myModel.getTextureCoords();
	std::vector<glm::vec3> norm = myModel.getNormals();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	for (int i = 0; i < myModel.getNumVertices(); i++) {
		pvalues.push_back((vert[i]).x);
		pvalues.push_back((vert[i]).y);
		pvalues.push_back((vert[i]).z);
		tvalues.push_back((tex[i]).s);
		tvalues.push_back((tex[i]).t);
		nvalues.push_back((norm[i]).x);
		nvalues.push_back((norm[i]).y);
		nvalues.push_back((norm[i]).z);
	}

	glGenVertexArrays(1, vao16);
	glBindVertexArray(vao16[0]);
	glGenBuffers(numVBOs16, vbo16);

	glBindBuffer(GL_ARRAY_BUFFER, vbo16[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), & pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo16[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * sizeof(float), &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo16[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * sizeof(float), &nvalues[0], GL_STATIC_DRAW);
}

void init16(GLFWwindow* window) {
	renderingProgram16 = Utils::createShaderProgram("vertexShader.imported_model", "fragShader.imported_model");
	cameraX16 = 0.0f; cameraY16 = 0.0f; cameraZ16 = 1.5f;
	objLocX16 = 0.0f; objLocY16 = 0.0f; objLocZ16 = 0.0f;

	glfwGetFramebufferSize(window, &width16, &height16);
	aspect16 = (float)width16 / (float)height16;
	pMat16 = glm::perspective(glm::radians(60.0f), aspect16, 0.1f, 1000.0f);

	setupVertices16();
	shuttleTexture16 = Utils::loadTexture("spstob_1.jpg");
}

void window_size_callback16(GLFWwindow* win, int newWidth, int newHeight) {
	aspect16 = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat16 = glm::perspective(glm::radians(60.0f), aspect16, 0.1f, 1000.0f);
}


void display16(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram16);

	mvLoc16 = glGetUniformLocation(renderingProgram16, "mv_matrix");
	projLoc16 = glGetUniformLocation(renderingProgram16, "proj_matrix");

	vMat16 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX16, -cameraY16, -cameraZ16));
	mMat16 = glm::translate(glm::mat4(1.0f), glm::vec3(objLocX16, objLocY16, objLocZ16));

	mMat16 = glm::rotate(mMat16, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	mMat16 = glm::rotate(mMat16, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat16 = glm::rotate(mMat16, glm::radians(35.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	mvMat16 = vMat16 * mMat16;

	glUniformMatrix4fv(mvLoc16, 1, GL_FALSE, glm::value_ptr(mvMat16));
	glUniformMatrix4fv(projLoc16, 1, GL_FALSE, glm::value_ptr(pMat16));

	glBindBuffer(GL_ARRAY_BUFFER, vbo16[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo16[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shuttleTexture16);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, myModel.getNumVertices());
}


int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "rzm title", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	glfwSetWindowSizeCallback(window, window_size_callback16);

	init16(window);

	while (!glfwWindowShouldClose(window)) {
		display16(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}