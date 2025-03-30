#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/soil2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Sphere.h"
#include "Utils.h"
using namespace std;

#define numVAOs14 1
#define numVBOs14 3

float cameraX14, cameraY14, cameraZ14;
float sphLocX14, sphLocY14, sphLocZ14;
GLuint renderingProgram14;
GLuint vao14[numVAOs14];
GLuint vbo14[numVBOs14];
GLuint earthTexture14;
float rotAmt14 = 0.0f;

// variable allocation for display
GLuint mvLoc14, projLoc14;
int width14, height14;
float aspect14;
glm::mat4 pMat14, vMat14, mMat14, mvMat14;

Sphere mySphere = Sphere(48);

/*
注意这里并没有使用索引缓冲区，所以顶点的顺序根据索引重新进行了排列
*/
void setupVertices14(void) {
	std::vector<int> ind = mySphere.getIndices();
	std::vector<glm::vec3> vert = mySphere.getVertices();
	std::vector<glm::vec2> tex = mySphere.getTexCoords();
	std::vector<glm::vec3> norm = mySphere.getNormals();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	int numIndices = mySphere.getNumIndices();
	for (int i = 0; i < numIndices; i++) {
		pvalues.push_back((vert[ind[i]]).x);
		pvalues.push_back((vert[ind[i]]).y);
		pvalues.push_back((vert[ind[i]]).z);
		tvalues.push_back((tex[ind[i]]).s);
		tvalues.push_back((tex[ind[i]]).t);
		nvalues.push_back((norm[ind[i]]).x);
		nvalues.push_back((norm[ind[i]]).y);
		nvalues.push_back((norm[ind[i]]).z);
	}

	glGenVertexArrays(numVAOs14, vao14);
	glBindVertexArray(vao14[0]);

	glGenBuffers(numVBOs14, vbo14);

	glBindBuffer(GL_ARRAY_BUFFER, vbo14[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo14[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * sizeof(float), &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo14[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * sizeof(float), &nvalues[0], GL_STATIC_DRAW);
}

void init14(GLFWwindow* window) {
	renderingProgram14 = Utils::createShaderProgram("vertexShader.boll", "fragShader.boll");
	cameraX14 = 0.0f;
	cameraY14 = 0.0f;
	cameraZ14 = 12.0f;
	sphLocX14 = 0.0f;
	sphLocY14 = 0.0f;
	sphLocZ14 = -1.0f;
	glfwGetFramebufferSize(window, &width14, &height14);
	aspect14 = (float)width14 / (float)height14;
	pMat14 = glm::perspective(glm::radians(60.0f), aspect14, 0.f, 100.0f);
	setupVertices14();
	earthTexture14 = Utils::loadTexture("ice.jpg");
}

void window_size_callback14(GLFWwindow* win, int newWidth, int newHeight) {
	aspect14 = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat14 = glm::perspective(glm::radians(60.0f), aspect14, 0.1f, 1000.0f);
	pMat14 = glm::perspective(glm::radians(60.0f), aspect14, 0.1f, 1000.0f);
}

void display14(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram14);

	mvLoc14 = glGetUniformLocation(renderingProgram14, "mv_matrix");
	projLoc14 = glGetUniformLocation(renderingProgram14, "proj_matrix");

	vMat14 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX14, -cameraY14, -cameraZ14));

	glm::mat4 translateM = glm::translate(glm::mat4(1.0f), glm::vec3(sin(currentTime), cos(currentTime), sin(currentTime)));
	glm::mat4 rotateM = glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));
	rotateM = glm::rotate(rotateM, (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
	rotateM = glm::rotate(rotateM, (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f));

	mMat14 = translateM * rotateM;
	mvMat14 = vMat14 * mMat14;

	glUniformMatrix4fv(mvLoc14, 1, GL_FALSE, glm::value_ptr(mvMat14));
	glUniformMatrix4fv(projLoc14, 1, GL_FALSE, glm::value_ptr(pMat14));

	glBindBuffer(GL_ARRAY_BUFFER, vbo14[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo14[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earthTexture14);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
}

int main14(void) {
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
	glfwSetWindowSizeCallback(window, window_size_callback14);
	init14(window);
	while (!glfwWindowShouldClose(window)) {
		display14(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

















