
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Torus.h"
#include "Utils.h"
using namespace std;

#define numVBOs20 3

float cameraX20, cameraY20, cameraZ20;
float torLocX20, torLocY20, torLocZ20;
GLuint renderingProgram20;
GLuint vbo20[numVBOs20];
GLuint vao20[1];

Torus myTorus20(0.5f, 0.2f, 48);
int numTorusVertices20 = myTorus20.getNumVertices();
int numTorusIndices20 = myTorus20.getNumIndices();

GLuint modelLoc20, projLoc20, viewLoc20, lightPosLoc20;
GLuint lightColorLoc20, objectColorLoc20;
int width20, height20;
float aspect20;
glm::mat4 pMat20, vMat20, mMat20;

float lightColor20[3] = { 1.0f, 1.0f, 1.0f };
float objectColor20[3] = { 0.2473f, 0.2095f, 0.0745f };
float initialLightLoc20[4] = { 10.0f, 0.0f, 2.0f};

void installLigth20(glm::mat4 vMatrix) {
	lightColorLoc20 = glGetUniformLocation(renderingProgram20, "lightColor");
	objectColorLoc20 = glGetUniformLocation(renderingProgram20, "objectColor");
	lightPosLoc20 = glGetUniformLocation(renderingProgram20, "lightPos");

	glProgramUniform3fv(renderingProgram20, lightColorLoc20, 1, lightColor20);
	glProgramUniform3fv(renderingProgram20, objectColorLoc20, 1, objectColor20);
	glProgramUniform3fv(renderingProgram20, lightPosLoc20, 1, initialLightLoc20);
}

void setupVertex20() {
	std::vector<int> ind = myTorus20.getIndices();
	std::vector<glm::vec3> vert = myTorus20.getVertices();
	std::vector<glm::vec3> normals = myTorus20.getNormals();

	std::vector<float> pvalues;
	std::vector<float> nvalues;

	for (int i = 0; i < myTorus20.getNumVertices();i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		nvalues.push_back(normals[i].x);
		nvalues.push_back(normals[i].y);
		nvalues.push_back(normals[i].z);
	}
	glGenVertexArrays(1, vao20);
	glBindVertexArray(vao20[0]);
	glGenBuffers(numVBOs20, vbo20);

	glBindBuffer(GL_ARRAY_BUFFER, vbo20[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), &pvalues[0], GL_STATIC_DRAW);
	Utils::checkOpenGLError();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo20[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(float), &ind[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo20[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * sizeof(float), &nvalues[0], GL_STATIC_DRAW);
}

void init20(GLFWwindow* window) {
	renderingProgram20 = Utils::createShaderProgram("vertexShader.ambient_diffuse_light", "fragShader.ambient_diffuse_light");
	cameraX20 = 0.0f;
	cameraY20 = 0.0f;
	cameraZ20 = 1.0f;
	torLocX20 = 0.0f;
	torLocY20 = 0.0f;
	torLocZ20 = -1.0f;
	glfwGetFramebufferSize(window, &width20, &height20);
	aspect20 = (float)width20 / (float)height20;
	pMat20 = glm::perspective(glm::radians(60.0f), aspect20, 0.1f, 1000.0f);
	setupVertex20();
}

void window_size_callback20(GLFWwindow* window, int width, int height) {
	aspect20 = (float)width / (float)height;
	pMat20 = glm::perspective(glm::radians(60.0f), aspect20, 0.1f, 1000.0f);
	glViewport(0, 0, width, height);
}

void display20(GLFWwindow* window, double currentTime) {

	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram20);

	modelLoc20 = glGetUniformLocation(renderingProgram20, "model");
	viewLoc20 = glGetUniformLocation(renderingProgram20, "view");
	projLoc20 = glGetUniformLocation(renderingProgram20, "projection");

	vMat20 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX20, -cameraY20, -cameraZ20));
	mMat20 = glm::translate(glm::mat4(1.0f), glm::vec3(torLocX20, torLocY20, torLocZ20));
	mMat20 *= glm::rotate(mMat20, glm::radians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	installLigth20(vMat20);

	glUniformMatrix4fv(modelLoc20, 1, GL_FALSE, glm::value_ptr(mMat20));
	glUniformMatrix4fv(projLoc20, 1, GL_FALSE, glm::value_ptr(pMat20));
	glUniformMatrix4fv(viewLoc20, 1, GL_FALSE, glm::value_ptr(vMat20));

	glBindBuffer(GL_ARRAY_BUFFER, vbo20[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo20[2]);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo20[1]);
	glDrawElements(GL_TRIANGLES, numTorusIndices20, GL_UNSIGNED_INT, 0);
}

int main20(void) {
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
	glfwSetWindowSizeCallback(window, window_size_callback20);
	init20(window);
	while (!glfwWindowShouldClose(window)) {
		display20(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}