#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

void init2(GLFWwindow* window) {

}

void display2(GLFWwindow* window, double currentTime) {
	glClearColor(0.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

int main(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow *window = glfwCreateWindow(600, 600, "title.....", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);
	init2(window);
	while (!glfwWindowShouldClose(window)) {
		display2(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}