#include "Shader.h"

#include "Shader.h"
Shader::Shader(const char* vertex_file_path, const char* fragment_file_path) {
	//These will eventually hold the source code for the vert/frag shaders
	std::string vertex_string;
	std::string fragment_string;
	std::ifstream vertex_file_stream;
	std::ifstream fragment_file_stream;

	//Enable the exceptions for failbit and bad bit
	vertex_file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragment_file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	transfer_from_file_to_stream(vertex_file_path, fragment_file_path, vertex_string, fragment_string, vertex_file_stream, fragment_file_stream);

	const char* vertex_shader_file_path = vertex_string.c_str();
	const char* fragment_shader_file_path = fragment_string.c_str();
	int success = 0;
	char infoLog[512];

	std::vector<unsigned int> compiled_shaders = compile_and_check_frag_vertex_shader(vertex_file_path, fragment_file_path, success);

	link_program_shader(compiled_shaders[0], compiled_shaders[1]);

	//Checking if program linked properly
	glGetProgramiv(programShaderID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programShaderID, 512, NULL, infoLog);
		std::cout << "FAILED::SHADER::PROGRAM::LINK\n" << infoLog << std::endl;
	}
	//Finsihed with Shaders so delete
	delete_shader(compiled_shaders[0]);
	delete_shader(compiled_shaders[1]);
}

std::vector<unsigned int> Shader::get_compiled_vertex_and_fragment_shader(const char* vertex_file_path, const char* fragment_file_path) {
	unsigned int vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_file_path);
	unsigned int fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_file_path);
	return { vertex_shader, fragment_shader };
}

void Shader::delete_shader(unsigned int shader) {
	glDeleteShader(shader);
}

void Shader::get_shader_iv(unsigned int shader,GLenum compile_type, int success_value) {
	char info[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success_value);
	if (!success_value) {
		glGetShaderInfoLog(shader, 512, NULL, info);
		std::cout << "Failed to obtain shader.\n" << info << std::endl;
	}
}

std::vector<unsigned int> Shader::compile_and_check_frag_vertex_shader(const char* vertex_shader_file_path, const char* fragment_shader_file_path, int success) {
	unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader_file_path);
	unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_file_path);
	get_shader_iv(vs, GL_COMPILE_STATUS, success);
	get_shader_iv(fs, GL_COMPILE_STATUS, success);
	return { vs, fs };
}

void Shader::link_program_shader(unsigned int vertex_shader, unsigned int fragment_shader) {
	programShaderID = glCreateProgram();
	glAttachShader(programShaderID, vertex_shader);
	glAttachShader(programShaderID, fragment_shader);
	glLinkProgram(programShaderID);
}

void Shader::transfer_from_file_to_stream(const char* vertex_file_path, const char* fragment_file_path, std::string& vertex_string, std::string& fragment_string,
	std::ifstream& vertex_file_stream, std::ifstream& fragment_file_stream) {
	/*_path
	Converts the vertex and fragment file information to a string stream for easier transfer into a string.
	*/
	try {
		//Open files and transfer data
		vertex_file_stream.open(vertex_file_path);
		fragment_file_stream.open(fragment_file_path);
		std::stringstream vertex_string_stream, fragment_string_stream;
		vertex_string_stream << vertex_file_stream.rdbuf();
		fragment_string_stream << fragment_file_stream.rdbuf();
		//Close files
		vertex_file_stream.close();
		fragment_file_stream.close();
		//Assign the data to the strings for vert/frag
		vertex_string = vertex_string_stream.str();
		fragment_string = fragment_string_stream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "Failed to transfer file information to a string." << std::endl;
	}
}

unsigned int Shader::compile_shader(GLenum shader_type, const char* file_path) {
	unsigned int shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &file_path, NULL);
	glCompileShader(shader);
	return shader;
}

void Shader::useProgram() {
	glUseProgram(programShaderID);
}

void Shader::setBool(const std::string& name, bool value) {
	glUniform1i(glGetUniformLocation(programShaderID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) {
	glUniform1i(glGetUniformLocation(programShaderID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) {
	glUniform1f(glGetUniformLocation(programShaderID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, glm::mat4 matrix) {
	glUniformMatrix4fv(glGetUniformLocation(programShaderID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVec3(const std::string& name, glm::vec3 vector) {
	glUniform3fv(glGetUniformLocation(programShaderID, name.c_str()), 1, glm::value_ptr(vector));
}

void Shader::setVec4(const std::string& name, glm::vec4 vector) {
	glUniform3fv(glGetUniformLocation(programShaderID, name.c_str()), 1, glm::value_ptr(vector));
}

void Shader::setPointLightStruct(
	std::string& name, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse,
	glm::vec3 specular, float constant, float linear, float quadratic
) {
	setVec3(name + ".position", position);
	setVec3(name + ".ambient", ambient);
	setVec3(name + ".diffuse", diffuse);
	setVec3(name + ".specular", specular);
	setFloat(name + ".constant", constant);
	setFloat(name + ".linear", linear);
	setFloat(name + ".quadratic", quadratic);
}
