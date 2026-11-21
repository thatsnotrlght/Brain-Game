#include "Scene.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <cmath>
#include <cstdlib>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace {
Scene* gScene = nullptr;

void framebufferSizeCallback(GLFWwindow*, int width, int height) {
    if (gScene != nullptr) gScene->onFramebufferResized(width, height);
}
}

int main() {
    if (glfwInit() != GLFW_TRUE) {
        std::cerr << "glfwInit failed.\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(
        1000, 700, "CAP4052 HW3 - Brain vs. Rot", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "glfwCreateWindow failed.\n";
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "GLAD initialization failed.\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    Scene scene;
    gScene = &scene;

    int framebufferWidth = 0;
    int framebufferHeight = 0;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);


    if (!scene.initialize(framebufferWidth, framebufferHeight)) {
        scene.shutdown();

        gScene = nullptr;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    float lastTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window)) {
        const float now = static_cast<float>(glfwGetTime());
        float deltaTime = now - lastTime;
        lastTime = now;

        // Prevent a long debugger pause/window drag from causing a huge catch-up step.
        if (deltaTime > 0.10f) deltaTime = 0.10f;

        glfwPollEvents();
        scene.processInput(window);
        scene.update(deltaTime);

        glClearColor(0.10f, 0.12f, 0.16f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene.render();

        glfwSwapBuffers(window);
    }

    scene.shutdown();

    gScene = nullptr;
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
