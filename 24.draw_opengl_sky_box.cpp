
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

#define numVAOs24 1
#define numVBOs24 4

float cameraX24, cameraY24, cameraZ24;
float torLocX24, torLocY24, torLocZ24;

GLuint renderingProgram24;
GLuint renderingProgramCubeMap24;
GLuint vao24[numVAOs24];
GLuint vbo24[numVBOs24];

GLuint brickTexture24, skyboxTexture24;
GLuint mvLoc24, projLoc24,vLoc24;
int width24, height24;
float aspect24;

glm::mat4 pMat24, vMat24, mMat24, mvMat24;

Torus myTorus24(0.5f, 0.2f, 48);
int numTorusVertices24, numTorusIndices24;

void setupVertices24(void) {
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

	numTorusVertices24 = myTorus24.getNumVertices();
	numTorusIndices24 = myTorus24.getNumIndices();
	std::vector<int> ind = myTorus24.getIndices();
	std::vector<glm::vec3> vert = myTorus24.getVertices();
	std::vector<glm::vec2> tex = myTorus24.getTexCoords();
	std::vector<glm::vec3> norm = myTorus24.getNormals();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	for (int i = 0; i < numTorusVertices24; i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(numVAOs24, vao24);
	glBindVertexArray(vao24[0]);
	glGenBuffers(numVBOs24, vbo24);

	glBindBuffer(GL_ARRAY_BUFFER, vbo24[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPositions) * 4, cubeVertexPositions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo24[1]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo24[2]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * sizeof(float), &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo24[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);
}

void display24(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	vMat24 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX24, -cameraY24, -cameraZ24));

	// draw cube map
	glUseProgram(renderingProgramCubeMap24);

	vLoc24 = glGetUniformLocation(renderingProgramCubeMap24, "v_matrix");
	glUniformMatrix4fv(vLoc24, 1, GL_FALSE, glm::value_ptr(vMat24));

	projLoc24 = glGetUniformLocation(renderingProgramCubeMap24, "p_matrix");
	glUniformMatrix4fv(projLoc24, 1, GL_FALSE, glm::value_ptr(pMat24));

	glBindBuffer(GL_ARRAY_BUFFER, vbo24[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture24);
	Utils::checkOpenGLError();
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glEnable(GL_DEPTH_TEST);

	// draw scene (in this case it is just a torus
	glUseProgram(renderingProgram24);
	mvLoc24 = glGetUniformLocation(renderingProgram24, "mv_matrix");
	projLoc24 = glGetUniformLocation(renderingProgram24, "proj_matrix");


	mMat24 = glm::translate(glm::mat4(1.0f), glm::vec3(torLocX24, torLocY24, torLocZ24));
	mMat24 = glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
	mMat24 = glm::rotate(mMat24, (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));
	mMat24 = glm::rotate(mMat24, (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f));
	mvMat24 = vMat24 * mMat24;

	glUniformMatrix4fv(mvLoc24, 1, GL_FALSE, glm::value_ptr(mvMat24));
	glUniformMatrix4fv(projLoc24, 1, GL_FALSE, glm::value_ptr(pMat24));

	glBindBuffer(GL_ARRAY_BUFFER, vbo24[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo24[2]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brickTexture24);

	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glDepthFunc(GL_LEQUAL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo24[3]);
	glDrawElements(GL_TRIANGLES, numTorusIndices24, GL_UNSIGNED_INT, 0);
}

void init24(GLFWwindow* window) {
	renderingProgram24 = Utils::createShaderProgram("vertexShader.simple_sky_box", "fragShader.simple_sky_box");
	renderingProgramCubeMap24 = Utils::createShaderProgram("vertexShader.opengl_sky_box", "fragShader.opengl_sky_box");
	glfwGetFramebufferSize(window, &width24, &height24);
	aspect24 = (float)width24 / (float)height24;
	pMat24 = glm::perspective(glm::radians(60.0f), aspect24, 0.1f, 1000.0f);
	setupVertices24();
	brickTexture24 = Utils::loadTexture("brick1.jpg");
	skyboxTexture24 = Utils::loadCubeMap("cubeMap"); // expects a folder name
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	torLocX24 = 0.0f;
	torLocY24 = 0.0f;
	torLocZ24 = 0.0f;
	cameraX24 = 0.0f;
	cameraY24 = 0.0f;
	cameraZ24 = 5.0f;
	glBindTexture(GL_TEXTURE_2D, brickTexture24);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
}


void window_size_callback24(GLFWwindow* window, int width, int height) {
	aspect24 = (float)width / (float)height;
	glViewport(0, 0, width, height);
	pMat24 = glm::perspective(glm::radians(60.0f), aspect24, 0.1f, 1000.0f);
}

int main(void) {
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
	glfwSetWindowSizeCallback(window, window_size_callback24);
	init24(window);
	while (!glfwWindowShouldClose(window)) {
		display24(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}