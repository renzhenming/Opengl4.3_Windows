#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/soil2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Torus.h"
#include "Utils.h"
#include "ImportedModel.h"
using namespace std;

void passOne(void);
void passTwo(void);

#define numVAOs18 1
#define numVBOs18 5

GLuint renderingProgram118, renderingProgram218;
GLuint vao18[numVAOs18];
GLuint vbo18[numVBOs18];

ImportedModel pyramid18("pyr.obj");

Torus myTorus18(0.6f, 0.4f, 48);

int numPyramidVertices18, numTorusVertices18, numTorusIndices18;

glm::vec3 torusLoc18(1.6f, 0.0f, -0.3f);
glm::vec3 pyrLoc18(-1.0f, 0.1f, 0.3f);
glm::vec3 cameraLoc18(0.0f, 0.2f, 6.0f);
glm::vec3 lightLoc18(-3.8f, 2.2f, 1.1f);

// white light
float globalAmbient18[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
float lightAmbient18[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse18[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular18[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

// gold material
float* gMatAmb18 = Utils::goldAmbient();
float* gMatDif18 = Utils::goldDiffuse();
float* gMatSpe18 = Utils::goldSpecular();
float gMatShi18 = Utils::goldShininess();

// bronze material
float* bMatAmb18 = Utils::bronzeAmbient();
float* bMatDif18 = Utils::bronzeDiffuse();
float* bMatSpe18 = Utils::bronzeSpecular();
float bMatShi18 = Utils::bronzeShininess();

float thisAmb18[4], thisDif18[4], thisSpe18[4], matAmb18[4], matDif18[4], matSpe18[4];
float thisShi18, matShi18;

// shadow stuff
int scSizeX18, scSizeY18;
GLuint shadowTex18, shadowBuffer18;
glm::mat4 lightVmatrix18;
glm::mat4 lightPmatrix18;
glm::mat4 shadowMVP118;
glm::mat4 shadowMVP218;
glm::mat4 b18;


// variable allocation for display
GLuint mvLoc18, projLoc18, nLoc18, sLoc18;
int width18, height18;
float aspect18;
glm::mat4 pMat18, vMat18, mMat18, mvMat18, invTrMat18;
glm::vec3 currentLightPos18, transformed18;
float lightPos18[3];
GLuint globalAmbLoc18, ambLoc18, diffLoc18, specLoc18, posLoc18, mambLoc18, mdiffLoc18, mspecLoc18, mshiLoc18;
glm::vec3 origin18(0.0f, 0.0f, 0.0f);
glm::vec3 up18(0.0f, 1.0f, 0.0f);

void setupShadowBuffers18(GLFWwindow* window) {
	glfwGetFramebufferSize(window, &width18, &height18);
	scSizeX18 = width18;
	scSizeY18 = height18;
	glGenFramebuffers(1, &shadowBuffer18);
	glGenTextures(1, &shadowTex18);
	glBindTexture(GL_TEXTURE_2D, shadowTex18);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX18, scSizeY18, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void setupVertices18(void) {
	numPyramidVertices18 = pyramid18.getNumVertices();
	std::vector<glm::vec3> vert = pyramid18.getVertices();
	std::vector<glm::vec3> norm = pyramid18.getNormals();
	std::vector<float> pyramidPvalues;
	std::vector<float> pyramidNvalues;
	for (int i = 0; i < numPyramidVertices18;i++) {
		pyramidPvalues.push_back(vert[i].x);
		pyramidPvalues.push_back(vert[i].y);
		pyramidPvalues.push_back(vert[i].z);
		pyramidNvalues.push_back((norm[i]).x);
		pyramidNvalues.push_back((norm[i]).y);
		pyramidNvalues.push_back((norm[i]).z);
	}

	numTorusVertices18 = myTorus18.getNumVertices();
	numTorusIndices18 = myTorus18.getNumIndices();
	std::vector<int> ind = myTorus18.getIndices();
	vert = myTorus18.getVertices();
	norm = myTorus18.getNormals();
	std::vector<float> torusPvalues;
	std::vector<float> torusNvalues;
	for (int i = 0; i < numTorusVertices18; i++) {
		torusPvalues.push_back(vert[i].x);
		torusPvalues.push_back(vert[i].y);
		torusPvalues.push_back(vert[i].z);
		torusNvalues.push_back(norm[i].x);
		torusNvalues.push_back(norm[i].y);
		torusNvalues.push_back(norm[i].z);
	}
	glGenVertexArrays(numVAOs18, vao18);
	glBindVertexArray(vao18[0]);
	glGenBuffers(numVBOs18, vbo18);

	glBindBuffer(GL_ARRAY_BUFFER, vbo18[0]);
	glBufferData(GL_ARRAY_BUFFER, torusPvalues.size() * sizeof(float), &torusPvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo18[1]);
	glBufferData(GL_ARRAY_BUFFER, pyramidPvalues.size() * sizeof(float), &pyramidPvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo18[2]);
	glBufferData(GL_ARRAY_BUFFER, torusNvalues.size() * sizeof(float), &torusNvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo18[3]);
	glBufferData(GL_ARRAY_BUFFER, pyramidNvalues.size() * sizeof(float), &pyramidNvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo18[4]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(float), &ind[0], GL_STATIC_DRAW);
}

void init18(GLFWwindow* window) {
	renderingProgram118 = Utils::createShaderProgram("vertexShader.shadow1", "fragShader.shadow1");
	renderingProgram218 = Utils::createShaderProgram("vertexShader.shadow2", "fragShader.shadow2");
	glfwGetFramebufferSize(window, &width18, &height18);
	aspect18 = (float)width18 / (float)height18;
	pMat18 = glm::perspective(glm::radians(60.0f), aspect18, 0.1f, 1000.0f);
	setupVertices18();
	setupShadowBuffers18(window);
	b18 = glm::mat4(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);
}

void display18(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	currentLightPos18 = glm::vec3(lightLoc18);

	lightVmatrix18 = glm::lookAt(currentLightPos18, origin18, up18);
	lightPmatrix18 = glm::perspective(glm::radians(60.0f), aspect18, 0.1f, 1000.0f);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer18);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowTex18, 0);

	glDrawBuffer(GL_NONE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2.0f, 4.0f);

	passOne();

	glDisable(GL_POLYGON_OFFSET_FILL);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowTex18);

	glDrawBuffer(GL_FRONT);
	passTwo();
}

void installLights18(int renderingProgram, glm::mat4 vMatrix) {
	transformed18 = glm::vec3(vMatrix * glm::vec4(currentLightPos18, 1.0));
	lightPos18[0] = transformed18.x;
	lightPos18[1] = transformed18.y;
	lightPos18[2] = transformed18.z;

	matAmb18[0] = thisAmb18[0]; matAmb18[1] = thisAmb18[1]; matAmb18[2] = thisAmb18[2]; matAmb18[3] = thisAmb18[3];
	matDif18[0] = thisDif18[0]; matDif18[1] = thisDif18[1]; matDif18[2] = thisDif18[2]; matDif18[3] = thisDif18[3];
	matSpe18[0] = thisSpe18[0]; matSpe18[1] = thisSpe18[1]; matSpe18[2] = thisSpe18[2]; matSpe18[3] = thisSpe18[3];
	matShi18 = thisShi18;

	// get the locations of the light and material fields in the shader
	globalAmbLoc18 = glGetUniformLocation(renderingProgram, "globalAmbient");
	ambLoc18 = glGetUniformLocation(renderingProgram, "light.ambient");
	diffLoc18 = glGetUniformLocation(renderingProgram, "light.diffuse");
	specLoc18 = glGetUniformLocation(renderingProgram, "light.specular");
	posLoc18 = glGetUniformLocation(renderingProgram, "light.position");
	mambLoc18 = glGetUniformLocation(renderingProgram, "material.ambient");
	mdiffLoc18 = glGetUniformLocation(renderingProgram, "material.diffuse");
	mspecLoc18 = glGetUniformLocation(renderingProgram, "material.specular");
	mshiLoc18 = glGetUniformLocation(renderingProgram, "material.shininess");

	//  set the uniform light and material values in the shader
	glProgramUniform4fv(renderingProgram, globalAmbLoc18, 1, globalAmbient18);
	glProgramUniform4fv(renderingProgram, ambLoc18, 1, lightAmbient18);
	glProgramUniform4fv(renderingProgram, diffLoc18, 1, lightDiffuse18);
	glProgramUniform4fv(renderingProgram, specLoc18, 1, lightSpecular18);
	glProgramUniform3fv(renderingProgram, posLoc18, 1, lightPos18);
	glProgramUniform4fv(renderingProgram, mambLoc18, 1, matAmb18);
	glProgramUniform4fv(renderingProgram, mdiffLoc18, 1, matDif18);
	glProgramUniform4fv(renderingProgram, mspecLoc18, 1, matSpe18);
	glProgramUniform1f(renderingProgram, mshiLoc18, matShi18);
}

void passTwo(void) {
	glUseProgram(renderingProgram218);

	mvLoc18 = glGetUniformLocation(renderingProgram218, "mv_matrix");
	projLoc18 = glGetUniformLocation(renderingProgram218, "proj_matrix");
	nLoc18 = glGetUniformLocation(renderingProgram218, "norm_matrix");
	sLoc18 = glGetUniformLocation(renderingProgram218, "shadowMVP");

	// draw the torus

	thisAmb18[0] = bMatAmb18[0];
	thisAmb18[1] = bMatAmb18[1];
	thisAmb18[2] = bMatAmb18[2];

	thisDif18[0] = bMatDif18[0];
	thisDif18[1] = bMatDif18[1];
	thisDif18[2] = bMatDif18[2];

	thisSpe18[0] = bMatSpe18[0];
	thisSpe18[1] = bMatSpe18[1];
	thisSpe18[2] = bMatSpe18[2];

	thisShi18 = bMatShi18;

	vMat18 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraLoc18.x, -cameraLoc18.y, -cameraLoc18.z));
	mMat18 = glm::translate(glm::mat4(1.0f), torusLoc18);
	mMat18 = glm::rotate(mMat18, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	currentLightPos18 = glm::vec3(lightLoc18);
	installLights18(renderingProgram218, vMat18);

	mvMat18 = vMat18 * mMat18;
	invTrMat18 = glm::transpose(glm::inverse(mvMat18));
	shadowMVP218 = b18 * lightPmatrix18 * lightVmatrix18 * mMat18;

	glUniformMatrix4fv(mvLoc18, 1, GL_FALSE, glm::value_ptr(mvMat18));
	glUniformMatrix4fv(projLoc18, 1, GL_FALSE, glm::value_ptr(pMat18));
	glUniformMatrix4fv(nLoc18, 1, GL_FALSE, glm::value_ptr(invTrMat18));
	glUniformMatrix4fv(sLoc18, 1, GL_FALSE, glm::value_ptr(shadowMVP218));

	glBindBuffer(GL_ARRAY_BUFFER, vbo18[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo18[2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo18[4]);
	glDrawElements(GL_TRIANGLES, numTorusIndices18, GL_UNSIGNED_INT, 0);

	// draw the pyramid

	thisAmb18[0] = gMatAmb18[0];
	thisAmb18[1] = gMatAmb18[1];
	thisAmb18[2] = gMatAmb18[2];

	thisDif18[0] = gMatDif18[0];
	thisDif18[1] = gMatDif18[1];
	thisDif18[2] = gMatDif18[2];

	thisSpe18[0] = gMatSpe18[0];
	thisSpe18[1] = gMatSpe18[1];
	thisSpe18[2] = gMatSpe18[2];

	thisShi18 = gMatShi18;

	mMat18 = glm::translate(glm::mat4(1.0f), pyrLoc18);
	mMat18 = glm::rotate(mMat18, glm::radians(30.0f),glm::vec3(1.0f, 0.0f, 0.0f));
	mMat18 = glm::rotate(mMat18, glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	currentLightPos18 = glm::vec3(lightLoc18);
	installLights18(renderingProgram218, vMat18);

	mvMat18 = vMat18 * mMat18;
	invTrMat18 = glm::transpose(glm::inverse(mvMat18));
	shadowMVP218 = b18 * lightPmatrix18 * lightVmatrix18 * mMat18;

	glUniformMatrix4fv(mvLoc18, 1, GL_FALSE, glm::value_ptr(mvMat18));
	glUniformMatrix4fv(projLoc18, 1, GL_FALSE, glm::value_ptr(pMat18));
	glUniformMatrix4fv(nLoc18, 1, GL_FALSE, glm::value_ptr(invTrMat18));
	glUniformMatrix4fv(sLoc18, 1, GL_FALSE, glm::value_ptr(shadowMVP218));

	glBindBuffer(GL_ARRAY_BUFFER, vbo18[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo18[3]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, numPyramidVertices18);
}

void passOne(void) {

	glUseProgram(renderingProgram118);
	// draw the torus
	mMat18 = glm::translate(glm::mat4(1.0f), torusLoc18);
	mMat18 = glm::rotate(mMat18, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	shadowMVP118 = lightPmatrix18 * lightVmatrix18 * mMat18;
	sLoc18 = glGetUniformLocation(renderingProgram118, "shadowMVP");
	glUniformMatrix4fv(sLoc18, 1, GL_FALSE, glm::value_ptr(shadowMVP118));

	glBindBuffer(GL_ARRAY_BUFFER, vbo18[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo18[4]);
	glDrawElements(GL_TRIANGLES, numTorusIndices18, GL_UNSIGNED_INT, 0);

	// draw the pyramid
	mMat18 = glm::translate(glm::mat4(1.0f), pyrLoc18);
	mMat18 = glm::rotate(mMat18, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mMat18 = glm::rotate(mMat18, glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	shadowMVP118 = lightPmatrix18 * lightVmatrix18 * mMat18;
	glUniformMatrix4fv(sLoc18, 1, GL_FALSE, glm::value_ptr(shadowMVP118));

	glBindBuffer(GL_ARRAY_BUFFER, vbo18[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, numPyramidVertices18);
}

void window_size_callback18(GLFWwindow* window, int width, int height) {
	aspect18 = (float)width / (float)height;
	glViewport(0, 0, width, height);
	pMat18 = glm::perspective(glm::radians(60.0f), aspect18, 0.1f, 1000.0f);
}

int main18(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "rzm_title", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);
	glfwSetWindowSizeCallback(window, window_size_callback18);
	init18(window);
	while (!glfwWindowShouldClose(window)) {
		display18(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}