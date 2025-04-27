
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/soil2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Sphere.h"
#include "Utils.h"
using namespace std;

#define numVAOs30 1
#define numVBOs30 4

float cameraX30, cameraY30, cameraZ30;
float sphLocX30, sphLocY30, sphLocZ30;
GLuint renderingProgram30;
GLuint vao30[numVAOs30];
GLuint vbo30[numVBOs30];

GLuint mvLoc30, projLoc30, vLoc30, nLoc30;
GLuint heightTexture30, colorTexture30,normalTexture30;
int width30, height30;
float aspect30;
float rotAmt30 = 0.0f;
glm::mat4 pMat30, vMat30, mMat30, mvMat30, invTrMat30;
glm::vec3 currentLightPos30;

Sphere mySphere30(96);
int numSphereVertices;
float lightPos30[3];
float lightLocX30, lightLocY30, lightLocZ30;
GLuint globalAmbLoc30, ambLoc30, diffLoc30, specLoc30, posLoc30, mambLoc30, mdiffLoc30, mspecLoc30, mshiLoc30;

// white light
float globalAmbient30[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
float lightAmbient30[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse30[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular30[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

// silver material
float* matAmb30 = Utils::silverAmbient();
float* matDif30 = Utils::silverDiffuse();
float* matSpe30 = Utils::silverSpecular();
float matShi30 = Utils::silverShininess();

void setupVertices30(void) {
	numSphereVertices = mySphere30.getNumIndices();
	std::vector<int> ind = mySphere30.getIndices();
	std::vector<glm::vec3> vert = mySphere30.getVertices();
	std::vector<glm::vec2> tex = mySphere30.getTexCoords();
	std::vector<glm::vec3> norm = mySphere30.getNormals();
	std::vector<glm::vec3> tang = mySphere30.getTangents();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;
	std::vector<float> tanvalues;

	for (int i = 0; i < mySphere30.getNumIndices(); i++) {
		pvalues.push_back((vert[ind[i]]).x);
		pvalues.push_back((vert[ind[i]]).y);
		pvalues.push_back((vert[ind[i]]).z);
		tvalues.push_back((tex[ind[i]]).s);
		tvalues.push_back((tex[ind[i]]).t);
		nvalues.push_back((norm[ind[i]]).x);
		nvalues.push_back((norm[ind[i]]).y);
		nvalues.push_back((norm[ind[i]]).z);
		tanvalues.push_back((tang[ind[i]]).x);
		tanvalues.push_back((tang[ind[i]]).y);
		tanvalues.push_back((tang[ind[i]]).z);
	}

	glGenVertexArrays(numVAOs30, vao30);
	glBindVertexArray(vao30[0]);
	glGenBuffers(numVBOs30, vbo30);

	glBindBuffer(GL_ARRAY_BUFFER, vbo30[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo30[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * sizeof(float), &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo30[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * sizeof(float), &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo30[3]);
	glBufferData(GL_ARRAY_BUFFER, tanvalues.size() * sizeof(float), &tanvalues[0], GL_STATIC_DRAW);

}

void installLights30(glm::mat4 vMatrix) {
	glm::vec3 transformed = glm::vec3(vMatrix * glm::vec4(currentLightPos30, 1.0));
	lightPos30[0] = transformed.x;
	lightPos30[1] = transformed.y;
	lightPos30[2] = transformed.z;

	// get the locations of the light and material fields in the shader
	globalAmbLoc30 = glGetUniformLocation(renderingProgram30, "globalAmbient");
	ambLoc30 = glGetUniformLocation(renderingProgram30, "light.ambient");
	diffLoc30 = glGetUniformLocation(renderingProgram30, "light.diffuse");
	specLoc30 = glGetUniformLocation(renderingProgram30, "light.specular");
	posLoc30 = glGetUniformLocation(renderingProgram30, "light.position");
	mambLoc30 = glGetUniformLocation(renderingProgram30, "material.ambient");
	mdiffLoc30 = glGetUniformLocation(renderingProgram30, "material.diffuse");
	mspecLoc30 = glGetUniformLocation(renderingProgram30, "material.specular");
	mshiLoc30 = glGetUniformLocation(renderingProgram30, "material.shininess");

	//  set the uniform light and material values in the shader
	glProgramUniform4fv(renderingProgram30, globalAmbLoc30, 1, globalAmbient30);
	glProgramUniform4fv(renderingProgram30, ambLoc30, 1, lightAmbient30);
	glProgramUniform4fv(renderingProgram30, diffLoc30, 1, lightDiffuse30);
	glProgramUniform4fv(renderingProgram30, specLoc30, 1, lightSpecular30);
	glProgramUniform3fv(renderingProgram30, posLoc30, 1, lightPos30);
	glProgramUniform4fv(renderingProgram30, mambLoc30, 1, matAmb30);
	glProgramUniform4fv(renderingProgram30, mdiffLoc30, 1, matDif30);
	glProgramUniform4fv(renderingProgram30, mspecLoc30, 1, matSpe30);
	glProgramUniform1f(renderingProgram30, mshiLoc30, matShi30);
}

void display30(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram30);
	mvLoc30 = glGetUniformLocation(renderingProgram30, "mv_matrix");
	projLoc30 = glGetUniformLocation(renderingProgram30, "proj_matrix");
	nLoc30 = glGetUniformLocation(renderingProgram30, "norm_matrix");

	vMat30 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX30, -cameraY30, -cameraZ30));
	mMat30 = glm::translate(glm::mat4(1.0f), glm::vec3(sphLocX30, sphLocY30, sphLocZ30));
	mMat30 = glm::rotate(mMat30, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mMat30 = glm::rotate(mMat30, rotAmt30, glm::vec3(0.0f, 1.0f, 0.0f));
	rotAmt30 = currentTime * 0.2f;
	mvMat30 = vMat30 * mMat30;
	invTrMat30 = glm::transpose(glm::inverse(mvMat30));

	currentLightPos30 = glm::vec3(lightLocX30, lightLocY30, lightLocZ30);
	installLights30(vMat30);

	glUniformMatrix4fv(mvLoc30, 1, GL_FALSE, glm::value_ptr(mvMat30));
	glUniformMatrix4fv(projLoc30, 1, GL_FALSE, glm::value_ptr(pMat30));
	glUniformMatrix4fv(nLoc30, 1, GL_FALSE, glm::value_ptr(invTrMat30));

	glBindBuffer(GL_ARRAY_BUFFER, vbo30[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo30[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo30[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo30[3]);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorTexture30);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalTexture30);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, heightTexture30);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glDrawArrays(GL_TRIANGLES, 0, numSphereVertices);
}

void init30(GLFWwindow* window) {
	renderingProgram30 = Utils::createShaderProgram("vertexShader.height_mapping2", "fragShader.height_mapping2");
	glfwGetFramebufferSize(window, &width30, &height30);
	aspect30 = (float)width30 / (float)height30;
	pMat30 = glm::perspective(glm::radians(60.0f), aspect30, 0.1f, 1000.0f);
	setupVertices30();
	sphLocX30 = 0.0f;
	sphLocY30 = 0.0f;
	sphLocZ30 = -1.0f;
	cameraX30 = 0.0f;
	cameraY30 = 0.0f;
	cameraZ30 = 2.0f;
	lightLocX30 = 3.0f;
	lightLocY30 = 2.0f;
	lightLocZ30 = 3.0f;

	colorTexture30 = Utils::loadTexture("earthspec1kBLUE.jpg");
	normalTexture30 = Utils::loadTexture("earthspec1kNORMAL.jpg");
	heightTexture30 = Utils::loadTexture("earthspec1kNEG.jpg");
}


void window_size_callback30(GLFWwindow* window, int width, int height) {
	aspect30 = (float)width / (float)height;
	glViewport(0, 0, width, height);
	pMat30 = glm::perspective(glm::radians(60.0f), aspect30, 0.1f, 1000.0f);
}

int main30(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(800, 800, "rzm-title,,,", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);
	glfwSetWindowSizeCallback(window, window_size_callback30);
	init30(window);
	while (!glfwWindowShouldClose(window)) {
		display30(window, glfwGetTime());
		Utils::checkOpenGLError(1);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}