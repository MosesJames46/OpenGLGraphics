#include "libs.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;
void processInput(GLFWwindow* window);

const char* vertexShaderSource = "#version 330 core\n"

	"layout (location = 0) in vec3 aPos;\n"

	"void main() {\n"
	"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
	"}\0";

const char* fragmentShaderSource = " #version 330 core\n"
	"out vec4 FragColor;\n"

	"void main() {\n"
	"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";


int main() {

	//Initialize, get context version, and profile
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Initializing our window with the proper parameters and make window context
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Window failed to initialize." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//Telling GLAD that we need it to load our function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialized GLAD." << std::endl;
		return -1;
	}

	//Telling GLFW we want to be able to resize our window see framebuffer_size_callback function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Shaders: vertex then fragment shaders both created, attached to their types and sources, and compiled.
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//Both shaders use this format of checking the shaderiv's compile status using the infoLog
	int success;
	char infoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "FAILED::VERTEX::SHADER::INITIALIZATION\n" << infoLog << std::endl;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "FAILED::FRAGMENT::SHADER::INITIALIZATION\n" << infoLog << std::endl;
	}

	//Shader Program: create a shader program, attach the vertex and fragment shaders then link.
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Programiv: attach the shaderProgram, request the link status, and attach the success.
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "FAILED::PROGRAM::INITIALIZATION\n" << infoLog << std::endl;
	}
	//Delete both shaders.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float triangleOne[] = {
		-0.75f, 0.5f, 0.0f,  // top left
		-0.75f, 0.25f, 0.0f,  // bottom left
		-0.65f, 0.25f, 0.0f,   // bottom right 

	};

	float triangleTwo[] = {
		-.45f, 0.5f, 0.0f, // top left
		-.45f, 0.25f, 0.0f, //bottom left
		-.35f, 0.25f, 0.0f, //bottom right
	};



	//VAO and VBO Generation
	unsigned int VAOs[2], VBOs[2];

	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	//VAO one
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleOne), triangleOne, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//VAO two
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleTwo), triangleTwo, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	//Render Loop
	while (!glfwWindowShouldClose(window)) {
		//process inputs
		processInput(window);

		//Render Options
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shaderProgram);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		//Poll Events
		glfwPollEvents();

		//Swap and Clear Buffers
		glfwSwapBuffers(window);
	}
	//De-allocating is optional, but still good.
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
	


}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}