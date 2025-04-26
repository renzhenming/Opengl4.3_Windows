
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

#define numVAOs28 1
#define numVBOs28 5

float cameraX28, cameraY28, cameraZ28;
float sphLocX28, sphLocY28, sphLocZ28;
GLuint renderingProgram28;
GLuint vao28[numVAOs28];
GLuint vbo28[numVBOs28];

GLuint mvLoc28, projLoc28, vLoc28, nLoc28;
GLuint moonTexture28, moonNormalMap28;
int width28, height28;
float aspect28;

glm::mat4 pMat28, vMat28, mMat28, mvMat28, invTrMat28;

Sphere mySphere28(48);
int numSphereVertices28;
int numSphereIndices28;
float lightLocX28, lightLocY28, lightLocZ28;
glm::vec3 currentLightPos28;
float lightPos28[3];
GLuint globalAmbLoc28, ambLoc28, diffLoc28, specLoc28, posLoc28, mambLoc28, mdiffLoc28, mspecLoc28, mshiLoc28;
// white light
float globalAmbient28[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
float lightAmbient28[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse28[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular28[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

// gold material
float* matAmb28 = Utils::goldAmbient();
float* matDif28 = Utils::goldDiffuse();
float* matSpe28 = Utils::goldSpecular();
float matShi28 = Utils::goldShininess();

float rotAmt28 = 0.0f;

void setupVertices28(void) {
	numSphereVertices28 = mySphere28.getNumVertices();
	numSphereIndices28 = mySphere28.getNumIndices();
	std::vector<int> ind = mySphere28.getIndices();
	std::vector<glm::vec3> vert = mySphere28.getVertices();
	std::vector<glm::vec2> tex = mySphere28.getTexCoords();
	std::vector<glm::vec3> norm = mySphere28.getNormals();
	std::vector<glm::vec3> tang = mySphere28.getTangents();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;
	std::vector<float> tanvalues;

	for (int i = 0; i < numSphereVertices28; i++) {
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

	glGenVertexArrays(numVAOs28, vao28);
	glBindVertexArray(vao28[0]);
	glGenBuffers(numVBOs28, vbo28);

	glBindBuffer(GL_ARRAY_BUFFER, vbo28[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo28[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * sizeof(float), &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo28[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * sizeof(float), &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo28[3]);
	glBufferData(GL_ARRAY_BUFFER, tanvalues.size() * sizeof(float), &tanvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo28[4]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(float), &ind[0], GL_STATIC_DRAW);
}

void installLights28(glm::mat4 vMatrix) {
	glm::vec3 transformed = glm::vec3(vMatrix * glm::vec4(currentLightPos28, 1.0));
	lightPos28[0] = transformed.x;
	lightPos28[1] = transformed.y;
	lightPos28[2] = transformed.z;

	// get the locations of the light and material fields in the shader
	globalAmbLoc28 = glGetUniformLocation(renderingProgram28, "globalAmbient");
	ambLoc28 = glGetUniformLocation(renderingProgram28, "light.ambient");
	diffLoc28 = glGetUniformLocation(renderingProgram28, "light.diffuse");
	specLoc28 = glGetUniformLocation(renderingProgram28, "light.specular");
	posLoc28 = glGetUniformLocation(renderingProgram28, "light.position");
	mambLoc28 = glGetUniformLocation(renderingProgram28, "material.ambient");
	mdiffLoc28 = glGetUniformLocation(renderingProgram28, "material.diffuse");
	mspecLoc28 = glGetUniformLocation(renderingProgram28, "material.specular");
	mshiLoc28 = glGetUniformLocation(renderingProgram28, "material.shininess");

	//  set the uniform light and material values in the shader
	glProgramUniform4fv(renderingProgram28, globalAmbLoc28, 1, globalAmbient28);
	glProgramUniform4fv(renderingProgram28, ambLoc28, 1, lightAmbient28);
	glProgramUniform4fv(renderingProgram28, diffLoc28, 1, lightDiffuse28);
	glProgramUniform4fv(renderingProgram28, specLoc28, 1, lightSpecular28);
	glProgramUniform3fv(renderingProgram28, posLoc28, 1, lightPos28);
	glProgramUniform4fv(renderingProgram28, mambLoc28, 1, matAmb28);
	glProgramUniform4fv(renderingProgram28, mdiffLoc28, 1, matDif28);
	glProgramUniform4fv(renderingProgram28, mspecLoc28, 1, matSpe28);
	glProgramUniform1f(renderingProgram28, mshiLoc28, matShi28);
}

void display28(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram28);
	mvLoc28 = glGetUniformLocation(renderingProgram28, "mv_matrix");
	projLoc28 = glGetUniformLocation(renderingProgram28, "proj_matrix");
	nLoc28 = glGetUniformLocation(renderingProgram28, "norm_matrix");

	vMat28 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX28, -cameraY28, -cameraZ28));
	mMat28 = glm::translate(glm::mat4(1.0f), glm::vec3(sphLocX28, sphLocY28, sphLocZ28));
	mMat28 = glm::rotate(mMat28, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mMat28 = glm::rotate(mMat28, rotAmt28, glm::vec3(0.0f, 1.0f, 0.0f));
	rotAmt28 = currentTime * 0.2f - 2.5f;
	mvMat28 = vMat28 * mMat28;

	invTrMat28 = glm::transpose(glm::inverse(mvMat28));

	currentLightPos28 = glm::vec3(lightLocX28, lightLocY28, lightLocZ28);
	installLights28(vMat28);

	glUniformMatrix4fv(mvLoc28, 1, GL_FALSE, glm::value_ptr(mvMat28));
	glUniformMatrix4fv(projLoc28, 1, GL_FALSE, glm::value_ptr(pMat28));
	glUniformMatrix4fv(nLoc28, 1, GL_FALSE, glm::value_ptr(invTrMat28));

	glBindBuffer(GL_ARRAY_BUFFER, vbo28[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo28[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo28[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo28[3]);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, moonNormalMap28);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, moonTexture28);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo28[4]);
	glDrawElements(GL_TRIANGLES, numSphereIndices28, GL_UNSIGNED_INT, 0);
}

void init28(GLFWwindow* window) {
	renderingProgram28 = Utils::createShaderProgram("vertexShader.texture_norm_mapping", "fragShader.texture_norm_mapping");
	glfwGetFramebufferSize(window, &width28, &height28);
	aspect28 = (float)width28 / (float)height28;
	pMat28 = glm::perspective(glm::radians(60.0f), aspect28, 0.1f, 1000.0f);
	setupVertices28();
	sphLocX28 = 0.0f;
	sphLocY28 = 0.0f;
	sphLocZ28 = -1.0f;
	cameraX28 = 0.0f;
	cameraY28 = 0.0f;
	cameraZ28 = 1.7f;
	lightLocX28 = 3.0f;
	lightLocY28 = 2.0f;
	lightLocZ28 = 3.0f;

	moonTexture28 = Utils::loadTexture("moon.jpg");
	moonNormalMap28 = Utils::loadTexture("moonNORMAL.jpg");
}


void window_size_callback28(GLFWwindow* window, int width, int height) {
	aspect28 = (float)width / (float)height;
	glViewport(0, 0, width, height);
	pMat28 = glm::perspective(glm::radians(60.0f), aspect28, 0.1f, 1000.0f);
}

int main28(void) {
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
	glfwSetWindowSizeCallback(window, window_size_callback28);
	init28(window);
	while (!glfwWindowShouldClose(window)) {
		display28(window, glfwGetTime());
		Utils::checkOpenGLError(1);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}