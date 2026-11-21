#pragma once

#include "Renderer3D.h"
#include "Model.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <random>
#include <vector>

class Scene {
public:
    bool initialize(int framebufferWidth, int framebufferHeight);
    void processInput(GLFWwindow* window);
    void update(float deltaTime);
    void render();
    void onFramebufferResized(int width, int height);
    void shutdown();

private:
    //void updateCamera();
    //void updateWindowTitle(GLFWwindow* window) const;

    glm::mat4 buildViewMatrix() const;
    glm::mat4 buildProjectionMatrix() const;

    struct Camera {
        glm::vec3 eye{ -6.0f, 13.0f, 15.0f };
        glm::vec3 center{ 0.0f, -0.5f, 0.0f };
        glm::vec3 up{ 0.0f, 1.0f, 0.0f };
    };

    Camera camera_;

    Renderer3D renderer_;


    // Models: Define model path in cmakelist 
    // -> Add model and loaded 
    // -> load at startup and release at shutdown (scene::initialize)
    // -> Draw in scene::render
    Model brainModel_;
    bool brainLoaded_ = false;

    Model virusModel_;
    bool virusLoaded_ = false;

    int framebufferWidth_ = 1000;
    int framebufferHeight_ = 700;

    // Player movement
    glm::vec3 playerPosition_{ -6.0f, -0.55f, 0.0f };
    glm::vec2 playerInput_{ 0.0f, 0.0f };

    float playerSpeed_ = 5.0f;

    struct Enemy {
        float x;
        float z;
        float speed;
        float health;
    };

    std::vector<Enemy> enemies_;
    float spawnTimer_ = 0.0f;

    enum class Defense { Neuron, Cortex };
    struct Tower {
        glm::ivec2 tile;
        Defense type;
        float shotCooldown = 0.0f;
    };
    struct DefenseStats {
        const char* name;
        int cost;
        float range;
        float damagePerSecond;
        glm::vec4 color;
    };
    static DefenseStats stats(Defense type);
    static bool buildable(glm::ivec2 tile);
    glm::ivec2 playerTile() const;
    const char* placementProblem(glm::ivec2 tile) const;

    std::vector<Tower> towers_;
    Defense selected_ = Defense::Neuron;
    bool buildKeyDown_ = false;
    std::string buildMessage_;
    float messageTime_ = 0.0f;

    int baseHealth_ = 5;
    int score_ = 0;

    std::mt19937 spawnRandom_{ std::random_device{}() };

    bool gameOver_ = false;
    bool restartKeyWasPressed_ = false;
    float survivalTime_ = 0.0f;
    bool gameWon_ = false;

    int resources_ = 30;

    void resetGame();
};