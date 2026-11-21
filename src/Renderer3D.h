#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Model;

class Renderer3D {
public:
	bool initialize(); // prepared OpenGL resources (like compiling shaders and buffers)
	void shutdown();

	void beginFrame(const glm::mat4& view,
					const glm::mat4& projection); // view and projection matrices to shader

	void drawCube(
		const glm::mat4& model,
		const glm::vec4& color); //render cube using model matrix

	void drawModel(
		const Model& model,
		const glm::mat4& transform,
		const glm::vec4& color
	);
				
private:
	GLuint vao_ = 0;
	GLuint vbo_ = 0;
	GLuint ebo_ = 0;
	GLuint shaderProgram_ = 0;
};