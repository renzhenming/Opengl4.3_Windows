
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

#define numVBOs21 3

float cameraX21, cameraY21, cameraZ21;
float torLocX21, torLocY21, torLocZ21;
GLuint renderingProgram21;
GLuint vbo21[numVBOs21];
GLuint vao21[1];

Torus myTorus21(0.5f, 0.2f, 48);
int numTorusVertices21 = myTorus21.getNumVertices();
int numTorusIndices21 = myTorus21.getNumIndices();

GLuint modelLoc21, projLoc21, viewLoc21, lightPosLoc21, viewPosLoc21;
GLuint lightColorLoc21, objectColorLoc21;
int width21, height21;
float aspect21;
glm::mat4 pMat21, vMat21, mMat21;

float lightColor21[3] = { 1.0f, 1.0f, 1.0f };
float objectColor21[3] = { 1.0f, 0.5f, 0.31f };
float initialLightLoc21[3] = { 10.0f, 0.0f, 2.0f };
float viewPos[3] = { 0.0f, 0.0f, 3.0f };

void installLigth21(glm::mat4 vMatrix) {
	lightColorLoc21 = glGetUniformLocation(renderingProgram21, "lightColor");
	objectColorLoc21 = glGetUniformLocation(renderingProgram21, "objectColor");
	lightPosLoc21 = glGetUniformLocation(renderingProgram21, "lightPos");
	viewPosLoc21 = glGetUniformLocation(renderingProgram21, "viewPos");

	glProgramUniform3fv(renderingProgram21, lightColorLoc21, 1, lightColor21);
	glProgramUniform3fv(renderingProgram21, objectColorLoc21, 1, objectColor21);
	glProgramUniform3fv(renderingProgram21, lightPosLoc21, 1, initialLightLoc21);
	glProgramUniform3fv(renderingProgram21, viewPosLoc21, 1, viewPos);
}

void setupVertex21() {
	std::vector<int> ind = myTorus21.getIndices();
	std::vector<glm::vec3> vert = myTorus21.getVertices();
	std::vector<glm::vec3> normals = myTorus21.getNormals();

	std::vector<float> pvalues;
	std::vector<float> nvalues;

	for (int i = 0; i < myTorus21.getNumVertices();i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		nvalues.push_back(normals[i].x);
		nvalues.push_back(normals[i].y);
		nvalues.push_back(normals[i].z);
	}
	glGenVertexArrays(1, vao21);
	glBindVertexArray(vao21[0]);
	glGenBuffers(numVBOs21, vbo21);

	glBindBuffer(GL_ARRAY_BUFFER, vbo21[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), &pvalues[0], GL_STATIC_DRAW);
	Utils::checkOpenGLError();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo21[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(float), &ind[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo21[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * sizeof(float), &nvalues[0], GL_STATIC_DRAW);
}

void init21(GLFWwindow* window) {
	renderingProgram21 = Utils::createShaderProgram("vertexShader.ambient_diffuse_specular_light", "fragShader.ambient_diffuse_specular_light");
	cameraX21 = 0.0f;
	cameraY21 = 0.0f;
	cameraZ21 = 1.0f;
	torLocX21 = 0.0f;
	torLocY21 = 0.0f;
	torLocZ21 = -1.0f;
	glfwGetFramebufferSize(window, &width21, &height21);
	aspect21 = (float)width21 / (float)height21;
	pMat21 = glm::perspective(glm::radians(60.0f), aspect21, 0.1f, 1000.0f);
	setupVertex21();
}

void window_size_callback21(GLFWwindow* window, int width, int height) {
	aspect21 = (float)width / (float)height;
	pMat21 = glm::perspective(glm::radians(60.0f), aspect21, 0.1f, 1000.0f);
	glViewport(0, 0, width, height);
}

void display21(GLFWwindow* window, double currentTime) {

	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram21);

	modelLoc21 = glGetUniformLocation(renderingProgram21, "model");
	viewLoc21 = glGetUniformLocation(renderingProgram21, "view");
	projLoc21 = glGetUniformLocation(renderingProgram21, "projection");

	vMat21 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX21, -cameraY21, -cameraZ21));
	mMat21 = glm::translate(glm::mat4(1.0f), glm::vec3(torLocX21, torLocY21, torLocZ21));
	mMat21 *= glm::rotate(mMat21, glm::radians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	installLigth21(vMat21);

	glUniformMatrix4fv(modelLoc21, 1, GL_FALSE, glm::value_ptr(mMat21));
	glUniformMatrix4fv(projLoc21, 1, GL_FALSE, glm::value_ptr(pMat21));
	glUniformMatrix4fv(viewLoc21, 1, GL_FALSE, glm::value_ptr(vMat21));

	glBindBuffer(GL_ARRAY_BUFFER, vbo21[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo21[2]);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo21[1]);
	glDrawElements(GL_TRIANGLES, numTorusIndices21, GL_UNSIGNED_INT, 0);
}

int main21(void) {
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
	glfwSetWindowSizeCallback(window, window_size_callback21);
	init21(window);
	while (!glfwWindowShouldClose(window)) {
		display21(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}