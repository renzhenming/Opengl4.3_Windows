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

#define numVAOs17 1
#define numVBOs17 4

float cameraX17, cameraY17, cameraZ17;
float torLocX17, torLocY17, torLocZ17;
GLuint renderingProgram17;
GLuint vao17[numVAOs17];
GLuint vbo17[numVBOs17];

Torus myTorus17(0.5f, 0.2f, 48);
int numTorusVertices17 = myTorus17.getNumVertices();
int numTorusIndices17 = myTorus17.getNumIndices();


glm::vec3 initialLightLoc17 = glm::vec3(10.0f, 0.0f, 2.0f);
float amt = 0.0f;

GLuint mvLoc17, projLoc17, nLoc17;
GLuint globalAmbLoc17, ambLoc17, diffLoc17, specLoc17, posLoc17, mambLoc17, mdiffLoc17, mspecLoc17, mshiLoc17;
int width17, height17;
float aspect17;
glm::mat4 pMat17, vMat17, mMat17, mvMat17, invTrMat17, rMat17;
glm::vec3 currentLightPos17, transformed17;
float lightPos17[3];

// white light
float globalAmbient17[4] = { 0.7f,0.7f,0.7f,1.0f };
float lightAmbient17[4] = { 0.0f,0.0f,0.0f,1.0f };
float lightDiffuse17[4] = { 1.0f,1.0f,1.0f,1.0f };
float lightSpecular17[4] = { 1.0f,1.0f,1.0f,1.0f };

// gold material
float* matAmb17 = Utils::goldAmbient();
float* matDif17 = Utils::goldDiffuse();
float* matSpe17 = Utils::goldSpecular();
float matShi17 = Utils::goldShininess();

void installLigth17(glm::mat4 vMatrix) {
	transformed17 = glm::vec3(vMatrix * glm::vec4(currentLightPos17, 1.0));
	lightPos17[0] = transformed17.x;
	lightPos17[1] = transformed17.y;
	lightPos17[2] = transformed17.z;

	// get the locations of the light and material fields in the shader
	globalAmbLoc17 = glGetUniformLocation(renderingProgram17, "globalAmbient");
	ambLoc17 = glGetUniformLocation(renderingProgram17, "light.ambient");
	diffLoc17 = glGetUniformLocation(renderingProgram17, "light.diffuse");
	specLoc17 = glGetUniformLocation(renderingProgram17, "light.specular");
	posLoc17 = glGetUniformLocation(renderingProgram17, "light.position");
	mambLoc17 = glGetUniformLocation(renderingProgram17, "material.ambient");
	mdiffLoc17 = glGetUniformLocation(renderingProgram17, "material.diffuse");
	mspecLoc17 = glGetUniformLocation(renderingProgram17, "material.specular");
	mshiLoc17 = glGetUniformLocation(renderingProgram17, "material.shininess");

	//  set the uniform light and material values in the shader
	glProgramUniform4fv(renderingProgram17, globalAmbLoc17, 1, globalAmbient17);
	glProgramUniform4fv(renderingProgram17, ambLoc17, 1, lightAmbient17);
	glProgramUniform4fv(renderingProgram17, diffLoc17, 1, lightDiffuse17);
	glProgramUniform4fv(renderingProgram17, specLoc17, 1, lightSpecular17);
	glProgramUniform3fv(renderingProgram17, posLoc17, 1, lightPos17);
	glProgramUniform4fv(renderingProgram17, mambLoc17, 1, matAmb17);
	glProgramUniform4fv(renderingProgram17, mdiffLoc17, 1, matDif17);
	glProgramUniform4fv(renderingProgram17, mspecLoc17, 1, matSpe17);
	glProgramUniform1f(renderingProgram17, mshiLoc17, matShi17);
}

void setupVertex17() {
	std::vector<int> ind = myTorus17.getIndices();
	std::vector<glm::vec3> vert = myTorus17.getVertices();
	std::vector<glm::vec2> tex = myTorus17.getTexCoords();
	std::vector<glm::vec3> norm = myTorus17.getNormals();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	for (int i = 0; i < myTorus17.getNumVertices();i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}
	glGenVertexArrays(numVAOs17, vao17);
	glBindVertexArray(vao17[0]);
	glGenBuffers(numVBOs17, vbo17);

	glBindBuffer(GL_ARRAY_BUFFER, vbo17[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo17[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * sizeof(float), &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo17[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * sizeof(float), &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo17[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(float), &ind[0], GL_STATIC_DRAW);
}

void init17(GLFWwindow* window) {
	//切换不同的光照模型
	renderingProgram17 = Utils::createShaderProgram("vertexShader.light_blinn_phong", "fragShader.light_blinn_phong");
	//renderingProgram17 = Utils::createShaderProgram("vertexShader.light_phong", "fragShader.light_phong");
	//renderingProgram17 = Utils::createShaderProgram("vertexShader.light_gouraud", "fragShader.light_gouraud");
	cameraX17 = 0.0f;
	cameraY17 = 0.0f;
	cameraZ17 = 1.0f;
	torLocX17 = 0.0f;
	torLocY17 = 0.0f;
	torLocZ17 = -1.0f;
	glfwGetFramebufferSize(window, &width17, &height17);
	aspect17 = (float)width17 / (float)height17;
	pMat17 = glm::perspective(glm::radians(60.0f), aspect17, 0.1f, 1000.0f);
	setupVertex17();
}

void window_size_callback17(GLFWwindow* window, int width, int height) {
	aspect17 = (float)width / (float)height;
	pMat17 = glm::perspective(glm::radians(60.0f), aspect17, 0.1f, 1000.0f);
	glViewport(0, 0, width, height);
}

void display17(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram17);

	mvLoc17 = glGetUniformLocation(renderingProgram17, "mv_matrix");
	projLoc17 = glGetUniformLocation(renderingProgram17, "proj_matrix");
	nLoc17 = glGetUniformLocation(renderingProgram17, "norm_matrix");

	vMat17 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX17, -cameraY17, -cameraZ17));
	mMat17 = glm::translate(glm::mat4(1.0f), glm::vec3(torLocX17, torLocY17, torLocZ17));
	mMat17 *= glm::rotate(mMat17, glm::radians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	currentLightPos17 = glm::vec3(initialLightLoc17.x, initialLightLoc17.y, initialLightLoc17.z);
	installLigth17(vMat17);

	mvMat17 = vMat17 * mMat17;
	invTrMat17 = glm::transpose(glm::inverse(mvMat17));

	glUniformMatrix4fv(mvLoc17, 1, GL_FALSE, glm::value_ptr(mvMat17));
	glUniformMatrix4fv(projLoc17, 1, GL_FALSE, glm::value_ptr(pMat17));
	glUniformMatrix4fv(nLoc17, 1, GL_FALSE, glm::value_ptr(invTrMat17));

	glBindBuffer(GL_ARRAY_BUFFER, vbo17[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo17[2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo17[3]);
	glDrawElements(GL_TRIANGLES, numTorusIndices17, GL_UNSIGNED_INT, 0);
}

int main17(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow * window = glfwCreateWindow(600, 600, "rzm title",NULL,NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);
	glfwSetWindowSizeCallback(window, window_size_callback17);
	init17(window);
	while (!glfwWindowShouldClose(window)) {
		display17(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}