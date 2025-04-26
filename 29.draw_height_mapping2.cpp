
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/soil2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "ImportedModel.h"
#include "Utils.h"
using namespace std;

#define numVAOs29 1
#define numVBOs29 3

float cameraX29, cameraY29, cameraZ29;
float gndLocX29, gndLocY29, gndLocZ29;
GLuint renderingProgram29;
GLuint vao29[numVAOs29];
GLuint vbo29[numVBOs29];

GLuint mvLoc29, projLoc29, vLoc29, nLoc29;
GLuint heightTexture29, heightMap29;
int width29, height29;
float aspect29;

glm::mat4 pMat29, vMat29, mMat29, mvMat29;

ImportedModel ground29("grid.obj");
int numGroundVertices29;

void setupVertices29(void) {
	numGroundVertices29 = ground29.getNumVertices();
	std::vector<glm::vec3> vert = ground29.getVertices();
	std::vector<glm::vec2> tex = ground29.getTextureCoords();
	std::vector<glm::vec3> norm = ground29.getNormals();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	for (int i = 0; i < numGroundVertices29; i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(numVAOs29, vao29);
	glBindVertexArray(vao29[0]);
	glGenBuffers(numVBOs29, vbo29);

	glBindBuffer(GL_ARRAY_BUFFER, vbo29[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo29[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * sizeof(float), &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo29[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * sizeof(float), &nvalues[0], GL_STATIC_DRAW);

}

void display29(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram29);
	mvLoc29 = glGetUniformLocation(renderingProgram29, "mv_matrix");
	projLoc29 = glGetUniformLocation(renderingProgram29, "proj_matrix");

	vMat29 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX29, -cameraY29, -cameraZ29));
	mMat29 = glm::translate(glm::mat4(1.0f), glm::vec3(gndLocX29, gndLocY29, gndLocZ29));
	mMat29 = glm::rotate(mMat29, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mvMat29 = vMat29 * mMat29;

	glUniformMatrix4fv(mvLoc29, 1, GL_FALSE, glm::value_ptr(mvMat29));
	glUniformMatrix4fv(projLoc29, 1, GL_FALSE, glm::value_ptr(pMat29));

	glBindBuffer(GL_ARRAY_BUFFER, vbo29[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo29[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo29[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, heightTexture29);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, heightMap29);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glDrawArrays(GL_TRIANGLES, 0, numGroundVertices29);
}

void init29(GLFWwindow* window) {
	renderingProgram29 = Utils::createShaderProgram("vertexShader.height_mapping", "fragShader.height_mapping");
	glfwGetFramebufferSize(window, &width29, &height29);
	aspect29 = (float)width29 / (float)height29;
	pMat29 = glm::perspective(glm::radians(60.0f), aspect29, 0.1f, 1000.0f);
	setupVertices29();
	gndLocX29 = 0.0f;
	gndLocY29 = 0.0f;
	gndLocZ29 = 0.0f;
	cameraX29 = 0.03f;
	cameraY29 = 0.03f;
	cameraZ29 = 0.8f;

	heightTexture29 = Utils::loadTexture("heightTexture.jpg");
	heightMap29 = Utils::loadTexture("height.jpg");
}


void window_size_callback29(GLFWwindow* window, int width, int height) {
	aspect29 = (float)width / (float)height;
	glViewport(0, 0, width, height);
	pMat29 = glm::perspective(glm::radians(60.0f), aspect29, 0.1f, 1000.0f);
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
	glfwSetWindowSizeCallback(window, window_size_callback29);
	init29(window);
	while (!glfwWindowShouldClose(window)) {
		display29(window, glfwGetTime());
		Utils::checkOpenGLError(1);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}