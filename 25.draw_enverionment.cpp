
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

#define numVAOs25 1
#define numVBOs25 4

float cameraX25, cameraY25, cameraZ25;
float torLocX25, torLocY25, torLocZ25;

GLuint renderingProgram25;
GLuint renderingProgramCubeMap25;
GLuint vao25[numVAOs25];
GLuint vbo25[numVBOs25];

GLuint skyboxTexture25;
GLuint mvLoc25, projLoc25, vLoc25, nLoc25;
int width25, height25;
float aspect25;

glm::mat4 pMat25, vMat25, mMat25, mvMat25, invTrMat25;

Torus myTorus25(0.5f, 0.2f, 48);
int numTorusVertices25, numTorusIndices25;

void setupVertices25(void) {
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

	numTorusVertices25 = myTorus25.getNumVertices();
	numTorusIndices25 = myTorus25.getNumIndices();
	std::vector<int> ind = myTorus25.getIndices();
	std::vector<glm::vec3> vert = myTorus25.getVertices();
	std::vector<glm::vec2> tex = myTorus25.getTexCoords();
	std::vector<glm::vec3> norm = myTorus25.getNormals();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	for (int i = 0; i < numTorusVertices25; i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(numVAOs25, vao25);
	glBindVertexArray(vao25[0]);
	glGenBuffers(numVBOs25, vbo25);

	glBindBuffer(GL_ARRAY_BUFFER, vbo25[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPositions), cubeVertexPositions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo25[1]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo25[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * sizeof(float), &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo25[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(int), &ind[0], GL_STATIC_DRAW);
}

void display25(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	vMat25 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX25, -cameraY25, -cameraZ25));
	// draw cube map
	glUseProgram(renderingProgramCubeMap25);

	vLoc25 = glGetUniformLocation(renderingProgramCubeMap25, "v_matrix");
	glUniformMatrix4fv(vLoc25, 1, GL_FALSE, glm::value_ptr(vMat25));

	projLoc25 = glGetUniformLocation(renderingProgramCubeMap25, "proj_matrix");
	glUniformMatrix4fv(projLoc25, 1, GL_FALSE, glm::value_ptr(pMat25));
	glBindBuffer(GL_ARRAY_BUFFER, vbo25[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture25);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glEnable(GL_DEPTH_TEST);

	// draw scene (in this case it is just a torus
	glUseProgram(renderingProgram25);
	mvLoc25 = glGetUniformLocation(renderingProgram25, "mv_matrix");
	projLoc25 = glGetUniformLocation(renderingProgram25, "proj_matrix");
	nLoc25 = glGetUniformLocation(renderingProgram25, "norm_matrix");

	mMat25 = glm::translate(glm::mat4(1.0f), glm::vec3(torLocX25, torLocY25, torLocZ25));
	mMat25 = glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
	mMat25 = glm::rotate(mMat25, (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));
	mMat25 = glm::rotate(mMat25, (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f));
	mvMat25 = vMat25 * mMat25;

	invTrMat25 = glm::transpose(glm::inverse(mvMat25));

	glUniformMatrix4fv(mvLoc25, 1, GL_FALSE, glm::value_ptr(mvMat25));
	glUniformMatrix4fv(projLoc25, 1, GL_FALSE, glm::value_ptr(pMat25));
	glUniformMatrix4fv(nLoc25, 1, GL_FALSE, glm::value_ptr(invTrMat25));

	glBindBuffer(GL_ARRAY_BUFFER, vbo25[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo25[2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture25);

	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glDepthFunc(GL_LEQUAL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo25[3]);
	glDrawElements(GL_TRIANGLES, numTorusIndices25, GL_UNSIGNED_INT, 0);
}

void init25(GLFWwindow* window) {
	renderingProgram25 = Utils::createShaderProgram("vertexShader.environment", "fragShader.environment");
	renderingProgramCubeMap25 = Utils::createShaderProgram("vertexShader.environment_cube", "fragShader.environment_cube");
	glfwGetFramebufferSize(window, &width25, &height25);
	aspect25 = (float)width25 / (float)height25;
	pMat25 = glm::perspective(glm::radians(60.0f), aspect25, 0.1f, 1000.0f);
	setupVertices25();
	skyboxTexture25 = Utils::loadCubeMap("cubeMap2"); // expects a folder name
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	torLocX25 = 0.0f;
	torLocY25 = 0.0f;
	torLocZ25 = 0.0f;
	cameraX25 = 0.0f;
	cameraY25 = 0.0f;
	cameraZ25 = 5.0f;
}


void window_size_callback25(GLFWwindow* window, int width, int height) {
	aspect25 = (float)width / (float)height;
	glViewport(0, 0, width, height);
	pMat25 = glm::perspective(glm::radians(60.0f), aspect25, 0.1f, 1000.0f);
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
	glfwSetWindowSizeCallback(window, window_size_callback25);
	init25(window);
	while (!glfwWindowShouldClose(window)) {
		display25(window, glfwGetTime());
		Utils::checkOpenGLError(1);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}