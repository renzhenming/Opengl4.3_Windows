
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

#define numVAOs23 1
#define numVBOs23 5

float cameraX23, cameraY23, cameraZ23;
float torLocX23, torLocY23, torLocZ23;

GLuint renderingProgram23;
GLuint vao23[numVAOs23];
GLuint vbo23[numVBOs23];

GLuint brickTexture23, skyboxTexture23;
GLuint mvLoc23, projLoc23;
int width23, height23;
float aspect23;

glm::mat4 pMat23, vMat23, mMat23, mvMat23;

Torus myTorus23(0.5f, 0.2f, 48);
int numTorusVertices23, numTorusIndices23;

void setupVertices23(void) {
	float cubeVertexPositions[108] =
	{ -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
	};
	float cubeTextureCoord[72] =
	{ 1.00f, 0.6666666f, 1.00f, 0.3333333f, 0.75f, 0.3333333f,	// back face lower right
		0.75f, 0.3333333f, 0.75f, 0.6666666f, 1.00f, 0.6666666f,	// back face upper left
		0.75f, 0.3333333f, 0.50f, 0.3333333f, 0.75f, 0.6666666f,	// right face lower right
		0.50f, 0.3333333f, 0.50f, 0.6666666f, 0.75f, 0.6666666f,	// right face upper left
		0.50f, 0.3333333f, 0.25f, 0.3333333f, 0.50f, 0.6666666f,	// front face lower right
		0.25f, 0.3333333f, 0.25f, 0.6666666f, 0.50f, 0.6666666f,	// front face upper left
		0.25f, 0.3333333f, 0.00f, 0.3333333f, 0.25f, 0.6666666f,	// left face lower right
		0.00f, 0.3333333f, 0.00f, 0.6666666f, 0.25f, 0.6666666f,	// left face upper left
		0.25f, 0.3333333f, 0.50f, 0.3333333f, 0.50f, 0.0000000f,	// bottom face upper right
		0.50f, 0.0000000f, 0.25f, 0.0000000f, 0.25f, 0.3333333f,	// bottom face lower left
		0.25f, 1.0000000f, 0.50f, 1.0000000f, 0.50f, 0.6666666f,	// top face upper right
		0.50f, 0.6666666f, 0.25f, 0.6666666f, 0.25f, 1.0000000f		// top face lower left
	};
	numTorusVertices23 = myTorus23.getNumVertices();
	numTorusIndices23 = myTorus23.getNumIndices();
	std::vector<int> ind = myTorus23.getIndices();
	std::vector<glm::vec3> vert = myTorus23.getVertices();
	std::vector<glm::vec2> tex = myTorus23.getTexCoords();
	std::vector<glm::vec3> norm = myTorus23.getNormals();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	for (int i = 0; i < numTorusVertices23; i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(numVAOs23, vao23);
	glBindVertexArray(vao23[0]);
	glGenBuffers(numVBOs23, vbo23);

	glBindBuffer(GL_ARRAY_BUFFER, vbo23[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPositions) * 4, cubeVertexPositions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo23[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTextureCoord) * 4, cubeTextureCoord, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo23[2]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo23[3]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * sizeof(float), &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo23[4]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);
}

void display23(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	vMat23 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX23, -cameraY23, -cameraZ23));

	// draw cube map
	glUseProgram(renderingProgram23);

	mMat23 = glm::translate(glm::mat4(1.0f), glm::vec3(cameraX23, cameraY23, cameraZ23));
	mvMat23 = vMat23 * mMat23;

	mvLoc23 = glGetUniformLocation(renderingProgram23, "mv_matrix");
	projLoc23 = glGetUniformLocation(renderingProgram23, "proj_matrix");
	glUniformMatrix4fv(mvLoc23, 1, GL_FALSE, glm::value_ptr(mvMat23));
	glUniformMatrix4fv(projLoc23, 1, GL_FALSE, glm::value_ptr(pMat23));

	glBindBuffer(GL_ARRAY_BUFFER, vbo23[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo23[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skyboxTexture23);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glEnable(GL_DEPTH_TEST);

	// draw scene (in this case it is just a torus
	glUseProgram(renderingProgram23);
	mvLoc23 = glGetUniformLocation(renderingProgram23, "mv_matrix");
	projLoc23 = glGetUniformLocation(renderingProgram23, "proj_matrix");


	mMat23 = glm::translate(glm::mat4(1.0f), glm::vec3(torLocX23, torLocY23, torLocZ23));
	mMat23 = glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
	mMat23 = glm::rotate(mMat23, (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));
	mMat23 = glm::rotate(mMat23, (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f));
	mvMat23 = vMat23 * mMat23;

	glUniformMatrix4fv(mvLoc23, 1, GL_FALSE, glm::value_ptr(mvMat23));
	glUniformMatrix4fv(projLoc23, 1, GL_FALSE, glm::value_ptr(pMat23));

	glBindBuffer(GL_ARRAY_BUFFER, vbo23[2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo23[3]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brickTexture23);

	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glDepthFunc(GL_LEQUAL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo23[4]);
	glDrawElements(GL_TRIANGLES, numTorusIndices23, GL_UNSIGNED_INT, 0);
}

void init23(GLFWwindow* window) {
	renderingProgram23 = Utils::createShaderProgram("vertexShader.simple_sky_box", "fragShader.simple_sky_box");
	glfwGetFramebufferSize(window, &width23, &height23);
	aspect23 = (float)width23 / (float)height23;
	pMat23 = glm::perspective(glm::radians(60.0f), aspect23, 0.1f, 1000.0f);
	setupVertices23();
	brickTexture23 = Utils::loadTexture("brick1.jpg");
	skyboxTexture23 = Utils::loadTexture("alien.jpg");
	torLocX23 = 0.0f;
	torLocY23 = -0.75f;
	torLocZ23 = 0.0f;
	cameraX23 = 0.0f;
	cameraY23 = 0.0f;
	cameraZ23 = 5.0f;
	glBindTexture(GL_TEXTURE_2D, brickTexture23);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
}


void window_size_callback23(GLFWwindow* window, int width, int height) {
	aspect23 = (float)width / (float)height;
	glViewport(0, 0, width, height);
	pMat23 = glm::perspective(glm::radians(60.0f), aspect23, 0.1f, 1000.0f);
}

int main23(void) {
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
	glfwSetWindowSizeCallback(window, window_size_callback23);
	init23(window);
	while (!glfwWindowShouldClose(window)) {
		display23(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}