
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

#define numVAOs27 1
#define numVBOs27 5

float cameraX27, cameraY27, cameraZ27;
float sphLocX27, sphLocY27, sphLocZ27;
GLuint renderingProgram27;
GLuint vao27[numVAOs27];
GLuint vbo27[numVBOs27];

GLuint mvLoc27, projLoc27, vLoc27, nLoc27;
int width27, height27;
float aspect27;

glm::mat4 pMat27, vMat27, mMat27, mvMat27, invTrMat27;

GLuint roofTexture27;
Sphere mySphere27(48);
int numSphereVertices27;
int numSphereIndices27;
float lightLocX27, lightLocY27, lightLocZ27;
glm::vec3 currentLightPos27;
float lightPos27[3];
GLuint globalAmbLoc27, ambLoc27, diffLoc27, specLoc27, posLoc27, mambLoc27, mdiffLoc27, mspecLoc27, mshiLoc27;
// white light
float globalAmbient27[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
float lightAmbient27[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse27[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular27[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

// gold material
float* matAmb27 = Utils::goldAmbient();
float* matDif27 = Utils::goldDiffuse();
float* matSpe27 = Utils::goldSpecular();
float matShi27 = Utils::goldShininess();

float rotAmt27 = 0.0f;

void setupVertices27(void) {
	numSphereVertices27 = mySphere27.getNumVertices();
	numSphereIndices27 = mySphere27.getNumIndices();
	std::vector<int> ind = mySphere27.getIndices();
	std::vector<glm::vec3> vert = mySphere27.getVertices();
	std::vector<glm::vec2> tex = mySphere27.getTexCoords();
	std::vector<glm::vec3> norm = mySphere27.getNormals();
	std::vector<glm::vec3> tang = mySphere27.getTangents();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;
	std::vector<float> tanvalues;

	for (int i = 0; i < numSphereVertices27; i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
		tanvalues.push_back(tang[i].x);
		tanvalues.push_back(tang[i].y);
		tanvalues.push_back(tang[i].z);
	}

	glGenVertexArrays(numVAOs27, vao27);
	glBindVertexArray(vao27[0]);
	glGenBuffers(numVBOs27, vbo27);

	glBindBuffer(GL_ARRAY_BUFFER, vbo27[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo27[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * sizeof(float), &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo27[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * sizeof(float), &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo27[3]);
	glBufferData(GL_ARRAY_BUFFER, tanvalues.size() * sizeof(float), &tanvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo27[4]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(float), &ind[0], GL_STATIC_DRAW);
}

void installLights27(glm::mat4 vMatrix) {
	glm::vec3 transformed = glm::vec3(vMatrix * glm::vec4(currentLightPos27, 1.0));
	lightPos27[0] = transformed.x;
	lightPos27[1] = transformed.y;
	lightPos27[2] = transformed.z;

	// get the locations of the light and material fields in the shader
	globalAmbLoc27 = glGetUniformLocation(renderingProgram27, "globalAmbient");
	ambLoc27 = glGetUniformLocation(renderingProgram27, "light.ambient");
	diffLoc27 = glGetUniformLocation(renderingProgram27, "light.diffuse");
	specLoc27 = glGetUniformLocation(renderingProgram27, "light.specular");
	posLoc27 = glGetUniformLocation(renderingProgram27, "light.position");
	mambLoc27 = glGetUniformLocation(renderingProgram27, "material.ambient");
	mdiffLoc27 = glGetUniformLocation(renderingProgram27, "material.diffuse");
	mspecLoc27 = glGetUniformLocation(renderingProgram27, "material.specular");
	mshiLoc27 = glGetUniformLocation(renderingProgram27, "material.shininess");

	//  set the uniform light and material values in the shader
	glProgramUniform4fv(renderingProgram27, globalAmbLoc27, 1, globalAmbient27);
	glProgramUniform4fv(renderingProgram27, ambLoc27, 1, lightAmbient27);
	glProgramUniform4fv(renderingProgram27, diffLoc27, 1, lightDiffuse27);
	glProgramUniform4fv(renderingProgram27, specLoc27, 1, lightSpecular27);
	glProgramUniform3fv(renderingProgram27, posLoc27, 1, lightPos27);
	glProgramUniform4fv(renderingProgram27, mambLoc27, 1, matAmb27);
	glProgramUniform4fv(renderingProgram27, mdiffLoc27, 1, matDif27);
	glProgramUniform4fv(renderingProgram27, mspecLoc27, 1, matSpe27);
	glProgramUniform1f(renderingProgram27, mshiLoc27, matShi27);
}

void display27(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram27);
	mvLoc27 = glGetUniformLocation(renderingProgram27, "mv_matrix");
	projLoc27 = glGetUniformLocation(renderingProgram27, "proj_matrix");
	nLoc27 = glGetUniformLocation(renderingProgram27, "norm_matrix");

	vMat27 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX27, -cameraY27, -cameraZ27));
	mMat27 = glm::translate(glm::mat4(1.0f), glm::vec3(sphLocX27, sphLocY27, sphLocZ27));
	mMat27 = glm::rotate(mMat27, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mMat27 = glm::rotate(mMat27, rotAmt27, glm::vec3(0.0f, 1.0f, 0.0f));
	rotAmt27 = currentTime * 0.2f;
	mvMat27 = vMat27 * mMat27;

	invTrMat27 = glm::transpose(glm::inverse(mvMat27));

	currentLightPos27 = glm::vec3(lightLocX27, lightLocY27, lightLocZ27);
	installLights27(vMat27);

	glUniformMatrix4fv(mvLoc27, 1, GL_FALSE, glm::value_ptr(mvMat27));
	glUniformMatrix4fv(projLoc27, 1, GL_FALSE, glm::value_ptr(pMat27));
	glUniformMatrix4fv(nLoc27, 1, GL_FALSE, glm::value_ptr(invTrMat27));

	glBindBuffer(GL_ARRAY_BUFFER, vbo27[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo27[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo27[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo27[3]);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, roofTexture27);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo27[4]);
	glDrawElements(GL_TRIANGLES, numSphereIndices27, GL_UNSIGNED_INT, 0);
}

void init27(GLFWwindow* window) {
	renderingProgram27 = Utils::createShaderProgram("vertexShader.norm_mapping", "fragShader.norm_mapping");
	glfwGetFramebufferSize(window, &width27, &height27);
	aspect27 = (float)width27 / (float)height27;
	pMat27 = glm::perspective(glm::radians(60.0f), aspect27, 0.1f, 1000.0f);
	roofTexture27 = Utils::loadTexture("castleroofNORMAL.jpg");
	setupVertices27();
	sphLocX27 = 0.0f; 
	sphLocY27 = 0.0f;
	sphLocZ27 = -1.0f;
	cameraX27 = 0.0f;
	cameraY27 = 0.0f;
	cameraZ27 = 3.0f;
	lightLocX27 = -5.0f;
	lightLocY27 = 2.0f;
	lightLocZ27 = 5.0f;
}


void window_size_callback27(GLFWwindow* window, int width, int height) {
	aspect27 = (float)width / (float)height;
	glViewport(0, 0, width, height);
	pMat27 = glm::perspective(glm::radians(60.0f), aspect27, 0.1f, 1000.0f);
}

int main27(void) {
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
	glfwSetWindowSizeCallback(window, window_size_callback27);
	init27(window);
	while (!glfwWindowShouldClose(window)) {
		display27(window, glfwGetTime());
		Utils::checkOpenGLError(1);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}