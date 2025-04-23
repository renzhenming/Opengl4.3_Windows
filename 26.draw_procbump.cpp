
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

#define numVAOs26 1
#define numVBOs26 4

float cameraX26, cameraY26, cameraZ26;
float torLocX26, torLocY26, torLocZ26;

GLuint renderingProgram26;
GLuint vao26[numVAOs26];
GLuint vbo26[numVBOs26];

GLuint mvLoc26, projLoc26, vLoc26, nLoc26;
int width26, height26;
float aspect26;

glm::mat4 pMat26, vMat26, mMat26, mvMat26, invTrMat26;

Torus myTorus26(0.5f, 0.2f, 48);
int numTorusVertices26, numTorusIndices26;
float lightLocX26, lightLocY26, lightLocZ26;
glm::vec3 currentLightPos26;
float lightPos26[3];
GLuint globalAmbLoc26, ambLoc26, diffLoc26, specLoc26, posLoc26, mambLoc26, mdiffLoc26, mspecLoc26, mshiLoc26;
// white light
float globalAmbient26[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
float lightAmbient26[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse26[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular26[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

// gold material
float* matAmb26 = Utils::goldAmbient();
float* matDif26 = Utils::goldDiffuse();
float* matSpe26 = Utils::goldSpecular();
float matShi26 = Utils::goldShininess();

void setupVertices26(void) {

	numTorusVertices26 = myTorus26.getNumVertices();
	numTorusIndices26 = myTorus26.getNumIndices();
	std::vector<int> ind = myTorus26.getIndices();
	std::vector<glm::vec3> vert = myTorus26.getVertices();
	std::vector<glm::vec2> tex = myTorus26.getTexCoords();
	std::vector<glm::vec3> norm = myTorus26.getNormals();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	for (int i = 0; i < numTorusVertices26; i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(numVAOs26, vao26);
	glBindVertexArray(vao26[0]);
	glGenBuffers(numVBOs26, vbo26);


	glBindBuffer(GL_ARRAY_BUFFER, vbo26[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo26[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * sizeof(float), &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo26[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * sizeof(float), &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo26[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(int), &ind[0], GL_STATIC_DRAW);
}

void installLights26(glm::mat4 vMatrix) {
	glm::vec3 transformed = glm::vec3(vMatrix * glm::vec4(currentLightPos26, 1.0));
	lightPos26[0] = transformed.x;
	lightPos26[1] = transformed.y;
	lightPos26[2] = transformed.z;

	// get the locations of the light and material fields in the shader
	globalAmbLoc26 = glGetUniformLocation(renderingProgram26, "globalAmbient");
	ambLoc26 = glGetUniformLocation(renderingProgram26, "light.ambient");
	diffLoc26 = glGetUniformLocation(renderingProgram26, "light.diffuse");
	specLoc26 = glGetUniformLocation(renderingProgram26, "light.specular");
	posLoc26 = glGetUniformLocation(renderingProgram26, "light.position");
	mambLoc26 = glGetUniformLocation(renderingProgram26, "material.ambient");
	mdiffLoc26 = glGetUniformLocation(renderingProgram26, "material.diffuse");
	mspecLoc26 = glGetUniformLocation(renderingProgram26, "material.specular");
	mshiLoc26 = glGetUniformLocation(renderingProgram26, "material.shininess");

	//  set the uniform light and material values in the shader
	glProgramUniform4fv(renderingProgram26, globalAmbLoc26, 1, globalAmbient26);
	glProgramUniform4fv(renderingProgram26, ambLoc26, 1, lightAmbient26);
	glProgramUniform4fv(renderingProgram26, diffLoc26, 1, lightDiffuse26);
	glProgramUniform4fv(renderingProgram26, specLoc26, 1, lightSpecular26);
	glProgramUniform3fv(renderingProgram26, posLoc26, 1, lightPos26);
	glProgramUniform4fv(renderingProgram26, mambLoc26, 1, matAmb26);
	glProgramUniform4fv(renderingProgram26, mdiffLoc26, 1, matDif26);
	glProgramUniform4fv(renderingProgram26, mspecLoc26, 1, matSpe26);
	glProgramUniform1f(renderingProgram26, mshiLoc26, matShi26);
}

void display26(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram26);
	mvLoc26 = glGetUniformLocation(renderingProgram26, "mv_matrix");
	projLoc26 = glGetUniformLocation(renderingProgram26, "proj_matrix");
	nLoc26 = glGetUniformLocation(renderingProgram26, "norm_matrix");

	vMat26 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX26, -cameraY26, -cameraZ26));
	mMat26 = glm::translate(glm::mat4(1.0f), glm::vec3(torLocX26, torLocY26, torLocZ26));
	mMat26 = glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
	mMat26 = glm::rotate(mMat26, (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));
	mMat26 = glm::rotate(mMat26, (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f));
	mvMat26 = vMat26 * mMat26;

	invTrMat26 = glm::transpose(glm::inverse(mvMat26));

	currentLightPos26 = glm::vec3(lightLocX26, lightLocY26, lightLocZ26);
	installLights26(vMat26);

	glUniformMatrix4fv(mvLoc26, 1, GL_FALSE, glm::value_ptr(mvMat26));
	glUniformMatrix4fv(projLoc26, 1, GL_FALSE, glm::value_ptr(pMat26));
	glUniformMatrix4fv(nLoc26, 1, GL_FALSE, glm::value_ptr(invTrMat26));

	glBindBuffer(GL_ARRAY_BUFFER, vbo26[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo26[2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo26[3]);
	glDrawElements(GL_TRIANGLES, numTorusIndices26, GL_UNSIGNED_INT, 0);
}

void init26(GLFWwindow* window) {
	renderingProgram26 = Utils::createShaderProgram("vertexShader.procbump", "fragShader.procbump");
	glfwGetFramebufferSize(window, &width26, &height26);
	aspect26 = (float)width26 / (float)height26;
	pMat26 = glm::perspective(glm::radians(60.0f), aspect26, 0.1f, 1000.0f);
	setupVertices26();
	torLocX26 = 0.0f;
	torLocY26 = 0.0f;
	torLocZ26 = -1.0f;
	cameraX26 = 0.0f;
	cameraY26 = 0.0f;
	cameraZ26 = 3.0f;
	lightLocX26 = 5.0f;
	lightLocY26 = 2.0f;
	lightLocZ26 = 2.0f;
}


void window_size_callback26(GLFWwindow* window, int width, int height) {
	aspect26 = (float)width / (float)height;
	glViewport(0, 0, width, height);
	pMat26 = glm::perspective(glm::radians(60.0f), aspect26, 0.1f, 1000.0f);
}

int main26(void) {
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
	glfwSetWindowSizeCallback(window, window_size_callback26);
	init26(window);
	while (!glfwWindowShouldClose(window)) {
		display26(window, glfwGetTime());
		Utils::checkOpenGLError(1);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}