#include "Scene.h"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

bool Scene::initialize(int width, int height) {
    framebufferWidth_ = std::max(width, 1);
    framebufferHeight_ = std::max(height, 1);

    if (!renderer_.initialize())
        return false;

    brainLoaded_ = brainModel_.loadOBJ(BRAIN_MODEL_PATH);

    if (!brainLoaded_)
        std::cerr << "Brain model could not be loaded.\n";

    virusLoaded_ = virusModel_.loadOBJ(VIRUS_MODEL_PATH);
    if (!virusLoaded_)
        std::cerr << "Virus model could not be loaded.\n";
    return true;
}


Scene::DefenseStats Scene::stats(Defense type) {
    if (type == Defense::Cortex)
        return {"Cortex", 30, 3.5f, 85.0f, {0.70f, 0.38f, 1.0f, 1.0f}};
    return {"Neuron", 20, 5.0f, 45.0f, {0.2f, 0.45f, 1.0f, 1.0f}};
}

glm::ivec2 Scene::playerTile() const {
    return {static_cast<int>(std::floor(playerPosition_.x)),
            static_cast<int>(std::floor(playerPosition_.z))};
}

bool Scene::buildable(glm::ivec2 tile) {
    // Six columns immediately in front of the brain.
    return tile.x >= -5 && tile.x <= 0 && tile.y >= -5 && tile.y <= 4
        && !(tile.x == -5 && (tile.y == -1 || tile.y == 0));
}

const char* Scene::placementProblem(glm::ivec2 tile) const {
    if (!buildable(tile)) return "Move onto a teal build tile";
    for (const Tower& tower : towers_)
        if (tower.tile == tile) return "Tile occupied";
    if (resources_ < stats(selected_).cost) return "Need more resources";
    return nullptr;
}

void Scene::processInput(GLFWwindow* window) {

    // 1. Handle ESC
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    // 2. Handle restarting
    bool restartPressed =
        glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;

    if (restartPressed && !restartKeyWasPressed_) {
        resetGame();
    }

    restartKeyWasPressed_ = restartPressed;

    // Read player movement input
    playerInput_ = glm::vec2(0.0f);

    if (!gameOver_) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            playerInput_.y -= 1.0f;

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            playerInput_.y += 1.0f;

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            playerInput_.x -= 1.0f;

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            playerInput_.x += 1.0f;
    }

    if (!gameOver_) {
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) selected_ = Defense::Neuron;
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) selected_ = Defense::Cortex;
    }
    bool buildPressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
    if (!gameOver_ && buildPressed && !buildKeyDown_ && !restartPressed) {
        glm::ivec2 tile = playerTile();
        if (const char* problem = placementProblem(tile)) {
            buildMessage_ = problem;
        } else {
            towers_.push_back({tile, selected_});
            resources_ -= stats(selected_).cost;
            buildMessage_ = std::string("Built ") + stats(selected_).name;
        }
        messageTime_ = 1.5f;
    }
    buildKeyDown_ = buildPressed;

    // 3. Update the window title
    std::string title =
        "Brain vs. Rot | Health: " +
        std::to_string(baseHealth_) +
        " | Score: " +
        std::to_string(score_) +
        " | Resources: " +
        std::to_string(resources_);

    if (gameWon_) {
        title += " | YOU WIN! - Press R to Restart";
    }
    else if (gameOver_) {
        title += " | GAME OVER - Press R to Restart";
    }

    if (!gameOver_) {
        title += " | WASD move | 1 Neuron (20), 2 Cortex (30) | E build ";
        title += stats(selected_).name;
        if (messageTime_ > 0.0f) title += " | " + buildMessage_;
        else if (const char* problem = placementProblem(playerTile()))
            title += std::string(" | ") + problem;
    }
    glfwSetWindowTitle(window, title.c_str());
}

void Scene::update(float deltaTime) {

    // Stop gameplay updates when the game is over
    if (gameOver_) {
        return;
    }

    messageTime_ -= deltaTime;

    // Move player
    if (glm::length(playerInput_) > 0.0f) {

        glm::vec2 direction =
            glm::normalize(playerInput_);

        playerPosition_.x +=
            direction.x * playerSpeed_ * deltaTime;

        playerPosition_.z +=
            direction.y * playerSpeed_ * deltaTime;
    }

    // Keep player inside the arena
    playerPosition_.x = std::clamp(
        playerPosition_.x, -9.5f, 9.5f
    );

    playerPosition_.z = std::clamp(
        playerPosition_.z, -5.5f, 5.5f
    );

    survivalTime_ += deltaTime;

    if (survivalTime_ >= 60.0f) {
        gameWon_ = true;
        gameOver_ = true;
        return;
    }

    // Spawn enemies
    spawnTimer_ += deltaTime;

    if (spawnTimer_ >= 2.0f) {
        spawnTimer_ = 0.0f;

        std::uniform_int_distribution<int> pickRow(-7, 6);
        float spawnZ = static_cast<float>(pickRow(spawnRandom_)) + 0.5f;

        enemies_.push_back(Enemy{ 9.5f, spawnZ, 1.5f, 100.0f });
    }

    // Move enemies
    for (Enemy& enemy : enemies_) {
        enemy.x -= enemy.speed * deltaTime;
    }

    // Measure range from each tower center in the same X/Z plane as the tiles.
    const float shotInterval = 0.8f;

    for (Tower& tower : towers_) {
        tower.shotCooldown -= deltaTime;
        if (tower.shotCooldown > 0.0f) continue;

        const auto defense = stats(tower.type);
        for (Enemy& enemy : enemies_) {
            float dx = enemy.x - (tower.tile.x + 0.5f);
            float dz = enemy.z - (tower.tile.y + 0.5f);
            if (enemy.health <= 0.0f || dx * dx + dz * dz > defense.range * defense.range)
                continue;

            enemy.health -= defense.damagePerSecond * shotInterval;
            tower.shotCooldown = shotInterval;
            break;
        }
    }

    // 4. Count defeated enemies and base damage
    for (const Enemy& enemy : enemies_) {

        if (enemy.health <= 0.0f) {
            score_ += 10;
            resources_ += 10;
        }
        else if (enemy.x < -5.0f) {
            baseHealth_ -= 1;
        }
    }

    // 5. Remove defeated enemies and enemies past the base
    enemies_.erase(
        std::remove_if(
            enemies_.begin(),
            enemies_.end(),
            [](const Enemy& enemy) {
                return enemy.health <= 0.0f ||
                    enemy.x < -5.0f;
            }
        ),
        enemies_.end()
    );

    // 6. Check if the player has lost
    if (baseHealth_ <= 0) {
        baseHealth_ = 0;
        gameOver_ = true;
    }
}


glm::mat4 Scene::buildViewMatrix() const {
    glm::mat4 view = glm::lookAt(
        camera_.eye, camera_.center, camera_.up
    );

    return view;

}

glm::mat4 Scene::buildProjectionMatrix() const {
    const float aspect = static_cast<float>(framebufferWidth_) /
                         static_cast<float>(framebufferHeight_);

    return glm::perspective(
        glm::radians(60.0f), //How wide the camera's vertical viewing angle is
        aspect, //Keeps the image from stretching
        0.1f, 
        100.0f
    );
}

void Scene::render() {
    glm::mat4 view = buildViewMatrix();
    glm::mat4 projection = buildProjectionMatrix();

    renderer_.beginFrame(view, projection);

    // tiled battlefield
    for (int x = -10; x < 10; ++x) {
        for (int z = -7; z < 7; ++z) {

            glm::mat4 tileModel(1.0f);

            tileModel = glm::translate(
                tileModel,
                glm::vec3(x + 0.5f, -1.0f, z + 0.5f)
            );

            tileModel = glm::scale(
                tileModel,
                glm::vec3(0.97f, 0.2f, 0.97f)
            );

            // Alternate tile colors
            bool alternate = (std::abs(x + z) % 2 == 0);

            glm::vec4 tileColor = alternate
                ? glm::vec4(0.62f, 0.65f, 0.68f, 1.0f)
                : glm::vec4(0.48f, 0.52f, 0.56f, 1.0f);

            glm::ivec2 tile{x, z};
            if (buildable(tile))
                tileColor = {0.27f, 0.55f, 0.52f, 1.0f};
            if (!gameOver_ && tile == playerTile())
                tileColor = placementProblem(tile) ? glm::vec4(0.95f, 0.55f, 0.18f, 1.0f)
                                                   : glm::vec4(0.53f, 0.95f, 0.42f, 1.0f);
            renderer_.drawCube(tileModel, tileColor);
        }
    }

    // Draw two lane dividers
    for (int i = 0; i < 2; ++i) {

        float dividerZ = -1.0f + i * 2.0f;

        glm::mat4 dividerModel(1.0f);

        dividerModel = glm::translate(
            dividerModel,
            glm::vec3(0.0f, -0.88f, dividerZ)
        );

        dividerModel = glm::scale(
            dividerModel,
            glm::vec3(12.0f, 0.04f, 0.06f)
        );

        renderer_.drawCube(
            dividerModel,
            glm::vec4(0.8f, 0.9f, 0.6f, 1.0f)
        );
    }


    // Render the brain at the defended end of the battlefield.
    if (brainLoaded_) {
        glm::mat4 brainTransform(1.0f);

        brainTransform = glm::translate(
            brainTransform,
            glm::vec3(-5.3f, 0.0f, 0.0f)
        );

        brainTransform = glm::scale(
            brainTransform,
            glm::vec3(1.6f)
        );

        renderer_.drawModel(
            brainModel_,
            brainTransform,
            glm::vec4(0.95f, 0.48f, 0.65f, 1.0f)
        );

        // brain health
        const float fullWidth = 1.8f;
        const float width = fullWidth * (baseHealth_ / 5.0f);
        const float leftEdge = -5.3f - fullWidth * 0.5f;

        if (baseHealth_ > 0) {
            glm::mat4 barModel(1.0f);
            barModel = glm::translate(barModel,
                glm::vec3(leftEdge + width * 0.5f, 1.15f, 0.0f));
            barModel = glm::scale(barModel, glm::vec3(width, 0.14f, 0.14f));
            renderer_.drawCube(barModel, glm::vec4(0.25f, 1.0f, 0.35f, 1.0f));
        }
    }

    // Draw controllable player
    glm::mat4 playerModel(1.0f);

    playerModel = glm::translate(
        playerModel,
        playerPosition_
    );

    playerModel = glm::scale(
        playerModel,
        glm::vec3(0.7f, 0.7f, 0.7f)
    );

    renderer_.drawCube(
        playerModel,
        glm::vec4(0.1f, 0.9f, 0.95f, 1.0f)
    );


    // Draw enemies
    for (const Enemy& enemy : enemies_) {

        glm::mat4 enemyModel(1.0f);

        enemyModel = glm::translate(
            enemyModel,
            glm::vec3(enemy.x, -0.55f, enemy.z)
        );

        enemyModel = glm::scale(
            enemyModel,
            glm::vec3(0.7f, 0.7f, 0.7f)
        );

        const glm::vec4 virusColor(0.85f, 0.15f, 0.15f, 1.0f);

        if (virusLoaded_) {
            renderer_.drawModel(virusModel_, enemyModel, virusColor);
        }
        else {
            renderer_.drawCube(enemyModel, virusColor);
        }

        const float healthFraction = std::clamp(enemy.health / 100.0f, 0.0f, 1.0f);
        const float fullWidth = 0.9f;
        const float width = fullWidth * healthFraction;
        const float leftEdge = enemy.x - fullWidth * 0.5f;

        glm::mat4 barModel(1.0f);
        barModel = glm::translate(barModel,
            glm::vec3(leftEdge + width * 0.5f, 0.10f, enemy.z));
        barModel = glm::scale(barModel, glm::vec3(width, 0.12f, 0.12f));

        renderer_.drawCube(barModel, glm::vec4(0.25f, 1.0f, 0.35f, 1.0f));
    }

    for (const Tower& tower : towers_) {
        bool cortex = tower.type == Defense::Cortex;
        glm::vec3 size = cortex ? glm::vec3(0.86f, 1.1f, 0.86f)
                                : glm::vec3(0.50f, 1.35f, 0.50f);
        glm::vec3 center(tower.tile.x + 0.5f, -0.9f + size.y / 2, tower.tile.y + 0.5f);
        auto model = glm::translate(glm::mat4(1.0f), center);
        renderer_.drawCube(glm::scale(model, size), stats(tower.type).color);
    }
}

void Scene::onFramebufferResized(int width, int height) {
    framebufferWidth_ = std::max(width, 1);
    framebufferHeight_ = std::max(height, 1);
    glViewport(0, 0, framebufferWidth_, framebufferHeight_);
}

void Scene::shutdown() {
    virusModel_.shutdown();
    brainModel_.shutdown();
    renderer_.shutdown();
}

void Scene::resetGame() {
    enemies_.clear();
    spawnTimer_ = 0.0f;

    playerPosition_ = glm::vec3(-6.0f, -0.55f, 0.0f);
    playerInput_ = glm::vec2(0.0f);

    baseHealth_ = 5;
    score_ = 0;
    gameOver_ = false;
    survivalTime_ = 0.0f;
    gameWon_ = false;
    resources_ = 30;

    towers_.clear();
    selected_ = Defense::Neuron;
    buildMessage_.clear();
    messageTime_ = 0.0f;
}
