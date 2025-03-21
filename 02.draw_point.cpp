#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define numVAOs 1

using namespace std;
GLuint renderingProgram;
GLuint vao[numVAOs];

void printShaderLog(GLuint shader) {
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

void printProgramLog(int prog) {
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

bool checkOpenGLError() {
	bool foundError = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR) {
		cout << "glError: " << glErr << endl;
		glErr = glGetError();
	}
	return foundError;
}

GLuint createShaderProgram() {
	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;
	const char* vshaderScource =
		"#version 430    \n"
		"void main(void) \n"
		"{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";

	const char* fshaderSource =
		"#version 430 \n"
		"out vec4 color; \n"
		"void main(void) \n"
		"{color = vec4(1.0,0.3,0.4,0.0);} ";

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vShader, 1, &vshaderScource, NULL);
	glShaderSource(fShader, 1, &fshaderSource, NULL);
	glCompileShader(vShader);
	checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		cout << "vertex compilation failed" << endl;
		printShaderLog(vShader);
	}

	glCompileShader(fShader);
	checkOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		cout << "fragment compilation failed" << endl;
		printShaderLog(fShader);
	}

	GLuint vfprogram = glCreateProgram();
	glAttachShader(vfprogram, vShader);
	glAttachShader(vfprogram, fShader);
	glLinkProgram(vfprogram);
	checkOpenGLError();
	glGetProgramiv(vfprogram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		cout << "linking failed" << endl;
		printProgramLog(vfprogram);
	}

	return vfprogram;
}

void init2(GLFWwindow* window) {
	renderingProgram = createShaderProgram();
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

void display2(GLFWwindow* window, double currentTime) {
	glUseProgram(renderingProgram);
	glPointSize(30.0f);
	glDrawArrays(GL_POINTS, 0, 1);
}

int main(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "rzm window", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
	/*
	glfwSwapInterval �����Ĳ�����һ���������京�����£�
	0������رմ�ֱͬ���������Կ����Ծ����ܸߵ�֡�ʽ�����Ⱦ��Ҳ�����������˺�ѵ������
	1����ʾ������ֱͬ�����Կ�������ʾ����ˢ����ͬ�����Դ˷�ֹ����˺�ѣ��������ܻ����֡�ʵ���΢�½���
	2����ζ�ſ���˫�ػ��崹ֱͬ�����Կ�������ʾ��ˢ������֮���ٽ���һ�ν������壬����ܻ�ʹ֡�ʼ��룬���ܱ�֤�����ƽ����
	*/
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
