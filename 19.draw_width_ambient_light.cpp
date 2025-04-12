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

#define numVBOs19 2

float cameraX19, cameraY19, cameraZ19;
float torLocX19, torLocY19, torLocZ19;
GLuint renderingProgram19;
GLuint vbo19[numVBOs19];

Torus myTorus19(0.5f, 0.2f, 48);
int numTorusVertices19 = myTorus19.getNumVertices();
int numTorusIndices19 = myTorus19.getNumIndices();

GLuint modelLoc19, projLoc19, viewLoc19;
GLuint lightColorLoc19, objectColorLoc19;
int width19, height19;
float aspect19;
glm::mat4 pMat19, vMat19, mMat19;

float lightColor19[4] = { 1.0f, 1.0f, 1.0f,1.0f };
float objectColor19[4] = { 1.0f, 1.0f, 1.0f,1.0f };

void installLigth19(glm::mat4 vMatrix) {
    lightColorLoc19 = glGetUniformLocation(renderingProgram19, "lightColor");
    objectColorLoc19 = glGetUniformLocation(renderingProgram19, "objectColor");

    glProgramUniform4fv(renderingProgram19, lightColorLoc19, 1, lightColor19);
    glProgramUniform4fv(renderingProgram19, objectColorLoc19, 1, objectColor19);
}

void setupVertex19() {
    std::vector<int> ind = myTorus19.getIndices();
    std::vector<glm::vec3> vert = myTorus19.getVertices();

    std::vector<float> pvalues;

    for (int i = 0; i < myTorus19.getNumVertices();i++) {
        pvalues.push_back(vert[i].x);
        pvalues.push_back(vert[i].y);
        pvalues.push_back(vert[i].z);
    }
    glGenBuffers(numVBOs19, vbo19);

    glBindBuffer(GL_ARRAY_BUFFER, vbo19[0]);
    glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), &pvalues[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo19[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(float), &ind[0], GL_STATIC_DRAW);
}

void init19(GLFWwindow* window) {
    renderingProgram19 = Utils::createShaderProgram("vertexShader.ambient_light", "fragShader.ambient_light");
    cameraX19 = 0.0f;
    cameraY19 = 0.0f;
    cameraZ19 = 1.0f;
    torLocX19 = 0.0f;
    torLocY19 = 0.0f;
    torLocZ19 = -1.0f;
    glfwGetFramebufferSize(window, &width19, &height19);
    aspect19 = (float)width19 / (float)height19;
    pMat19 = glm::perspective(glm::radians(60.0f), aspect19, 0.1f, 1000.0f);
    setupVertex19();
}

void window_size_callback19(GLFWwindow* window, int width, int height) {
    aspect19 = (float)width / (float)height;
    pMat19 = glm::perspective(glm::radians(60.0f), aspect19, 0.1f, 1000.0f);
    glViewport(0, 0, width, height);
}

void display19(GLFWwindow* window, double currentTime) {

    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    Utils::checkOpenGLError();
    glUseProgram(renderingProgram19);

    modelLoc19 = glGetUniformLocation(renderingProgram19, "model");
    viewLoc19 = glGetUniformLocation(renderingProgram19, "view");
    projLoc19 = glGetUniformLocation(renderingProgram19, "projection");
    vMat19 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX19, -cameraY19, -cameraZ19));
    mMat19 = glm::translate(glm::mat4(1.0f), glm::vec3(torLocX19, torLocY19, torLocZ19));
    mMat19 *= glm::rotate(mMat19, glm::radians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    installLigth19(vMat19);

    glUniformMatrix4fv(modelLoc19, 1, GL_FALSE, glm::value_ptr(mMat19));
    glUniformMatrix4fv(projLoc19, 1, GL_FALSE, glm::value_ptr(pMat19));
    glUniformMatrix4fv(viewLoc19, 1, GL_FALSE, glm::value_ptr(vMat19));

    glBindBuffer(GL_ARRAY_BUFFER, vbo19[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo19[1]);
    glDrawElements(GL_TRIANGLES, numTorusIndices19, GL_UNSIGNED_INT, 0);
}

int main(void) {
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
    glfwSetWindowSizeCallback(window, window_size_callback19);
    init19(window);
    Utils::checkOpenGLError();
    while (!glfwWindowShouldClose(window)) {
        display19(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}











//
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <string>
//#include <iostream>
//#include <fstream>
//#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
//#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
//#include "Torus.h"
//#include "Utils.h"
//using namespace std;
//
//#define numVBOs19 2
//
//float cameraX19, cameraY19, cameraZ19;
//float torLocX19, torLocY19, torLocZ19;
//GLuint renderingProgram19;
//GLuint vbo19[numVBOs19];
//GLuint vao19[1];
//
//Torus myTorus19(0.5f, 0.2f, 48);
//int numTorusVertices19 = myTorus19.getNumVertices();
//int numTorusIndices19 = myTorus19.getNumIndices();
//
//GLuint modelLoc19, projLoc19, viewLoc19;
//GLuint lightColorLoc19, objectColorLoc19;
//int width19, height19;
//float aspect19;
//glm::mat4 pMat19, vMat19, mMat19;
//
//float lightColor19[4] = { 1.0f, 1.0f, 1.0f,1.0f };
//float objectColor19[4] = { 1.0f, 1.0f, 1.0f,1.0f };
//
//void installLigth19(glm::mat4 vMatrix) {
//	lightColorLoc19 = glGetUniformLocation(renderingProgram19, "lightColor");
//	objectColorLoc19 = glGetUniformLocation(renderingProgram19, "objectColor");
//
//	glProgramUniform4fv(renderingProgram19, lightColorLoc19, 1, lightColor19);
//	glProgramUniform4fv(renderingProgram19, objectColorLoc19, 1, objectColor19);
//}
//
//void setupVertex19() {
//	std::vector<int> ind = myTorus19.getIndices();
//	std::vector<glm::vec3> vert = myTorus19.getVertices();
//
//	std::vector<float> pvalues;
//
//	for (int i = 0; i < myTorus19.getNumVertices();i++) {
//		pvalues.push_back(vert[i].x);
//		pvalues.push_back(vert[i].y);
//		pvalues.push_back(vert[i].z);
//	}
//	glGenVertexArrays(1, vao19);
//	glBindVertexArray(vao19[0]);
//	glGenBuffers(numVBOs19, vbo19);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo19[0]);
//	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), &pvalues[0], GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo19[1]);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(float), &ind[0], GL_STATIC_DRAW);
//}
//
//void init19(GLFWwindow* window) {
//	renderingProgram19 = Utils::createShaderProgram("vertexShader.ambient_light", "fragShader.ambient_light");
//	cameraX19 = 0.0f;
//	cameraY19 = 0.0f;
//	cameraZ19 = 1.0f;
//	torLocX19 = 0.0f;
//	torLocY19 = 0.0f;
//	torLocZ19 = -1.0f;
//	glfwGetFramebufferSize(window, &width19, &height19);
//	aspect19 = (float)width19 / (float)height19;
//	pMat19 = glm::perspective(glm::radians(60.0f), aspect19, 0.1f, 1000.0f);
//	setupVertex19();
//}
//
//void window_size_callback19(GLFWwindow* window, int width, int height) {
//	aspect19 = (float)width / (float)height;
//	pMat19 = glm::perspective(glm::radians(60.0f), aspect19, 0.1f, 1000.0f);
//	glViewport(0, 0, width, height);
//}
//
//void display19(GLFWwindow* window, double currentTime) {
//
//	glClear(GL_DEPTH_BUFFER_BIT);
//	glClear(GL_COLOR_BUFFER_BIT);
//	Utils::checkOpenGLError();
//	glUseProgram(renderingProgram19);
//
//	modelLoc19 = glGetUniformLocation(renderingProgram19, "model");
//	viewLoc19 = glGetUniformLocation(renderingProgram19, "view");
//	projLoc19 = glGetUniformLocation(renderingProgram19, "projection");
//	vMat19 = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX19, -cameraY19, -cameraZ19));
//	mMat19 = glm::translate(glm::mat4(1.0f), glm::vec3(torLocX19, torLocY19, torLocZ19));
//	mMat19 *= glm::rotate(mMat19, glm::radians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//
//	installLigth19(vMat19);
//
//	glUniformMatrix4fv(modelLoc19, 1, GL_FALSE, glm::value_ptr(mMat19));
//	glUniformMatrix4fv(projLoc19, 1, GL_FALSE, glm::value_ptr(pMat19));
//	glUniformMatrix4fv(viewLoc19, 1, GL_FALSE, glm::value_ptr(vMat19));
//	glBindBuffer(GL_ARRAY_BUFFER, vbo19[0]);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	glEnableVertexAttribArray(0);
//	glEnable(GL_CULL_FACE);
//	glFrontFace(GL_CCW);
//	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LEQUAL);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo19[1]);
//	glDrawElements(GL_TRIANGLES, numTorusIndices19, GL_UNSIGNED_INT, 0);
//}
//
//int main(void) {
//	if (!glfwInit()) {
//		exit(EXIT_FAILURE);
//	}
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	GLFWwindow* window = glfwCreateWindow(600, 600, "rzm title", NULL, NULL);
//	glfwMakeContextCurrent(window);
//	if (glewInit() != GLEW_OK) {
//		exit(EXIT_FAILURE);
//	}
//	glfwSwapInterval(1);
//	glfwSetWindowSizeCallback(window, window_size_callback19);
//	init19(window);
//	Utils::checkOpenGLError();
//	while (!glfwWindowShouldClose(window)) {
//		display19(window, glfwGetTime());
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//	glfwDestroyWindow(window);
//	glfwTerminate();
//	exit(EXIT_SUCCESS);
//}