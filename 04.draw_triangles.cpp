#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

#define numVAOs4 1

GLuint renderingProgram4;
GLuint vao4[numVAOs4];

string readFile4(const char* file) {
	string content;
	string line = "";
	ifstream fileStream(file,ios::in);
	while (!fileStream.eof()) {
		getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

void printShaderLog4(GLuint shader) {
	char* log;
	int len = 0;
	int chWrittn = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWrittn, log);
		cout << "Shader Info Log: " << log << endl;
		free(log);
	}
}

void printProgramLog4(int prog) {
	char* log;
	int len = 0;
	int chWrittn = 0;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetProgramInfoLog(prog, len, &chWrittn, log);
		cout << "Program Info Log: " << log << endl;
		free(log);
	}
}

bool checkOpenGLError4() {
	bool foundError = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR) {
		cout << "glError: " << glErr << endl;
		glErr = glGetError();
	}
	return foundError;
}


GLuint createShaderProgram4() {
	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint vfprogram = glCreateProgram();

	string vertexShaderStr = readFile4("vertexShader.triangle");
	string fragmentShaderStr = readFile4("fragShader.triangle");
	const char* verShaderSrc = vertexShaderStr.c_str();
	const char* fragShaderSrc = fragmentShaderStr.c_str();
	glShaderSource(vShader, 1, &verShaderSrc, NULL);
	glShaderSource(fShader, 1, &fragShaderSrc, NULL);
	glCompileShader(vShader);
	checkOpenGLError4();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		cout << "vertex compilation failed" << endl;
		printShaderLog4(vShader);
	}


	glCompileShader(fShader);
	checkOpenGLError4();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		cout << "fragment compilation failed" << endl;
		printShaderLog4(fShader);
	}

	glAttachShader(vfprogram, vShader);
	glAttachShader(vfprogram, fShader);
	glLinkProgram(vfprogram);
	checkOpenGLError4();
	glGetProgramiv(vfprogram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		cout << "linking failed" << endl;
		printProgramLog4(vfprogram);
	}
	return vfprogram;
}

void display4(GLFWwindow* window, double currentTime) {
	glUseProgram(renderingProgram4);
	glPointSize(30.0f);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void init4(GLFWwindow* window) {
	renderingProgram4 = createShaderProgram4();
	glGenVertexArrays(numVAOs4, vao4);
	glBindVertexArray(vao4[0]);
}

int main4(void) {
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
	init4(window);

	while (!glfwWindowShouldClose(window)) {
		display4(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}