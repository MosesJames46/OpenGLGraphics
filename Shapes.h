#pragma once
#include <glad/glad.h>
#include <vector>

class Shape {
public:
	unsigned int generate_VAO(unsigned int uninitialized_VAO) {
		glGenVertexArrays(1, &uninitialized_VAO);
		return uninitialized_VAO;
	}

	unsigned int generate_VBO(unsigned int uninitialized_VBO) {
		glGenBuffers(1, &uninitialized_VBO);
		return uninitialized_VBO;
	}
	void bind_VAO(unsigned int initialized_VAO) {
		glBindVertexArray(initialized_VAO);
	}
	void bind_VBO(unsigned int initialized_VBO) {
		glBindBuffer(GL_ARRAY_BUFFER, initialized_VBO);
	}

	void generate_and_bind_VAO(unsigned int VAO) {
		bind_VAO(generate_VAO(VAO));
	}

	void generate_and_bind_VBO(unsigned int VBO) {
		bind_VBO(generate_VBO(VBO));
	}

	void format_buffer(int size_of_array, std::vector<float>& vertex_data, GLenum draw_type) {
		glBufferData(GL_ARRAY_BUFFER, size_of_array * sizeof(float), vertex_data.data(), draw_type);
	}
	void set_attributes(int count, int starting_position, int attribute_size, GLenum numerical_type, GLboolean normalized, int vertex_size, int offset) {
		glVertexAttribPointer(starting_position, attribute_size, numerical_type, normalized, vertex_size * sizeof(float), reinterpret_cast<void*>(offset * sizeof(float)));
		glEnableVertexAttribArray(count);
	}
	void set_attributes(int count, int starting_position, int attribute_size, int vertex_size, int offset) {
		glVertexAttribPointer(starting_position, attribute_size, GL_FLOAT, GL_FALSE, vertex_size * sizeof(float), reinterpret_cast<void*>(offset * sizeof(float)));
		glEnableVertexAttribArray(count);
	}

	void gen_bind_format(std::vector<float> vertices, unsigned int VAO, unsigned int VBO) {
		generate_and_bind_VAO(VAO);
		generate_and_bind_VBO(VBO);
		format_buffer(vertices.size(), vertices, GL_STATIC_DRAW);
	}
	void unbind_vertex_attribute_pointer() {
		glEnableVertexAttribArray(0);
	}

	template <typename T>
	void print_data(T data) {
		std::cout << data << "\n";
	}
protected:
	struct Vertex {
		std::vector<float> coordinates;
		std::vector<float> colors;

		Vertex(float coordinates, float colors) : coordinates(coordinates), colors(colors) {};
	};
};

class Triangle : public Shape {
public:
	unsigned int triangle_VAO, triangle_VBO;

	

	Triangle(std::vector<Vertex> vertices)  {
		size_t j = 0;
		size_t total_size_of_vectos = vertices.size();
		for (int i = 0; i < total_size_of_vectos; i++) {
			
		}
	}

	template <typename Shader>
	void draw(Shader shader, int start, int vertex_count) {
		shader.use();
		bind_triangle_VAO(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
private:
	std::vector<Vertex> vertex_data;
};