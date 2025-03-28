#pragma once
#include <glad/glad.h>
#include <glm-master/glm/glm.hpp>
#include <glm-master/glm/gtc/type_ptr.hpp>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>


class Shader {
public:
	unsigned int programShaderID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void useProgram();
	void transfer_from_file_to_stream(const char* vertex_file_path, const char* fragment_file_path, std::string& vertex_string,
		std::string& fragment_string, std::ifstream& vertex_file, std::ifstream& fragment_file);
	unsigned int compile_shader(GLenum shader_type, const char* source_file_path);
	std::vector<unsigned int> get_compiled_vertex_and_fragment_shader(const char* vertex_file_path, const char* fragment_file_path);
	std::vector<unsigned int> compile_and_check_frag_vertex_shader(const char* vertex_shader_file_path, const char* fragment_shader_file_path, int success_value);
	void get_shader_iv(unsigned int shader, GLenum compilation_type, int success_value);
	void link_program_shader(unsigned int vertex_shader, unsigned int fragment_shader);
	void delete_shader(unsigned int shader);
	void setBool(const std::string& name, bool value);
	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);
	void setMat4(const std::string& name, glm::mat4 matrix);
	void setVec3(const std::string& name, glm::vec3 vector);
	void setVec4(const std::string& name, glm::vec4 vector);

	void setPointLightStruct(
		std::string& name, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse,
		glm::vec3 specular, float constant, float linear, float quadratic
	);
};
