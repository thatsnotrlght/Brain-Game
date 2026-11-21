#pragma once

#include <glad/glad.h>
#include <string>

class Model {
public:
	bool loadOBJ(const std::string& path);
	void draw() const;
	void shutdown();

private:
	GLuint vao_ = 0;
	GLuint vbo_ = 0;
	GLsizei vertexCount_ = 0;
};