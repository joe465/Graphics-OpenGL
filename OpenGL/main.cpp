#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// window dimensions
const GLint WIDTH = 600;
const GLint HEIGHT = 600;

// Vertex Shader
const char* vertex_shader_source = R"(
	#version 330 core
	layout (location = 0) in vec3 aPos;
	void main() {
		gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	}
)";

// Fragment Shader
const char* fragment_shader_source = R"(
	#version 330 core
	out vec4 FragColor;
	void main() {
		FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // Orange color
	}
)";

int main() {

	// initialize GLFW
	if (!glfwInit()) {
		std::cout << "GLFW Initialization Failed!" << std::endl;
		glfwTerminate();
		return 1;
	}

	// setup GLFW window property
	// openGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// core profile means no backward compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Main Window", NULL, NULL);
	if (!mainWindow) {
		std::cerr << "Window Creation Failed!" << std::endl;
		glfwTerminate();
		return -1; // return non-zero value to indicate an error
	}
	// get buffer size information
	int bufferWidth;
	int bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// allow modern extension features
	glewExperimental = GL_TRUE;

	// initialize GLEW
	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW Initialization Failed!" << std::endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return -1;
	}

	// setup viewport size
	glViewport(0, 0, bufferHeight, bufferWidth);

	// Create and compile the vertex shader
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	// Check for vertex shader compilation errors / success
	GLint success;
	GLchar info_log[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		std::cerr << "Vertex shader compilation failed : " << info_log << std::endl;
	}
	else {
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		std::cout << "Vertex shader compilation succeeded" << info_log << std::endl;
	}

	// Create and compile the fragment shader
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	// Check for fragment shader compilation errors / success
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		std::cerr << "Fragment shader compilation failed : " << info_log << std::endl;
	}
	else {
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		std::cout << "Fragment shader compilation succeeded : " << info_log << std::endl;
	}

	// Create and link the shader program
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	// Check for shader program linking errors / success
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, info_log);
		std::cerr << "Shader program linking failed : " << info_log << std::endl;
	}
	else {
		glGetProgramInfoLog(shader_program, 512, NULL, info_log);
		std::cout << "Shader program linking succeeded : " << info_log << std::endl;
	}

	// Delete the vertex shader and fragment shader after successfully linked into a shader program
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Define the vertices of a triangle
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, // Vertex 1 (x, y, z)
		 0.5f, -0.5f, 0.0f, // Vertex 2 (x, y, z)
		 0.0f,  0.5f, 0.0f, // Vertex 3 (x, y, z)
	};

	// Set up the Vertex Array Object (VAO)
	GLuint VAO;
	GLuint VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Define the layout of the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// main loop until window close
	while (!glfwWindowShouldClose(mainWindow)) {
		// get & handle user input events
		glfwPollEvents();

		// clear window
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_program);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3); // Draw a triangle
		
		// swap front and back buffer
		glfwSwapBuffers(mainWindow);
	}

	// Cleanup and exit
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shader_program);

	glfwTerminate();

	return 0;
}