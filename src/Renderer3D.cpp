#include "Renderer3D.h"
#include "Model.h"

#include <glm/gtc/type_ptr.hpp>

namespace {

    const char* vertexShaderSource = R"GLSL(
    #version 330 core

    layout(location = 0) in vec3 aPosition;

    uniform mat4 uModel;
    uniform mat4 uView;
    uniform mat4 uProjection;

    void main() {
        gl_Position =
            uProjection * uView * uModel *
            vec4(aPosition, 1.0);
    }
)GLSL";

    const char* fragmentShaderSource = R"GLSL(
    #version 330 core

    out vec4 FragColor;

    uniform vec4 uColor;

    void main() {
        FragColor = uColor;
    }
)GLSL";
}

bool Renderer3D::initialize() {


    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    shaderProgram_ = glCreateProgram();

    glAttachShader(shaderProgram_, vertexShader);
    glAttachShader(shaderProgram_, fragmentShader);

    glLinkProgram(shaderProgram_);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // vertices store positions of cubes corners
    const float vertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f
    };

    //indices tells OpenGL which corners should connect to form triangles
    const unsigned int indices[] = {
    0, 2, 1, 0, 3, 2,
    4, 5, 6, 4, 6, 7,
    0, 4, 7, 0, 7, 3,
    1, 2, 6, 1, 6, 5,
    3, 7, 6, 3, 6, 2,
    0, 1, 5, 0, 5, 4
    };

    //VBO to store vertex data in GPU accessible buffer, like coordinates for exmaple (X,Y,Z)

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(float), nullptr
    );

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);


    return true;
}

void Renderer3D::beginFrame(const glm::mat4& view,
    const glm::mat4& projection) {

    glUseProgram(shaderProgram_);

    GLint viewLocation = glGetUniformLocation(shaderProgram_, "uView");

    GLint projectionLocation =
        glGetUniformLocation(shaderProgram_, "uProjection");

    glUniformMatrix4fv(
        viewLocation,
        1,
        GL_FALSE,
        glm::value_ptr(view)
    );

    glUniformMatrix4fv(
        projectionLocation,
        1,
        GL_FALSE,
        glm::value_ptr(projection)
    );

}

void Renderer3D::drawCube(
    const glm::mat4& model,
    const glm::vec4& color
) {
    glUseProgram(shaderProgram_);

    GLint modelLocation =
        glGetUniformLocation(shaderProgram_, "uModel");

    GLint colorLocation =
        glGetUniformLocation(shaderProgram_, "uColor");

    glUniformMatrix4fv(
        modelLocation,
        1,
        GL_FALSE,
        glm::value_ptr(model)
    );

    glUniform4fv(
        colorLocation,
        1,
        glm::value_ptr(color)
    );

    glBindVertexArray(vao_);

    glDrawElements(
        GL_TRIANGLES,
        36,
        GL_UNSIGNED_INT,
        nullptr
    );

    glBindVertexArray(0);
}


void Renderer3D::drawModel(
    const Model& model,
    const glm::mat4& transform,
    const glm::vec4& color
) {
    glUseProgram(shaderProgram_);

    GLint modelLocation =
        glGetUniformLocation(shaderProgram_, "uModel");

    GLint colorLocation =
        glGetUniformLocation(shaderProgram_, "uColor");

    glUniformMatrix4fv(
        modelLocation,
        1,
        GL_FALSE,
        glm::value_ptr(transform)
    );

    glUniform4fv(
        colorLocation,
        1,
        glm::value_ptr(color)
    );

    model.draw();
}



void Renderer3D::shutdown() {
    if (ebo_ != 0)
        glDeleteBuffers(1, &ebo_);

    if (vbo_ != 0)
        glDeleteBuffers(1, &vbo_);

    if (vao_ != 0)
        glDeleteVertexArrays(1, &vao_);

    if (shaderProgram_ != 0)
        glDeleteProgram(shaderProgram_);

    ebo_ = 0;
    vbo_ = 0;
    vao_ = 0;
    shaderProgram_ = 0;
}