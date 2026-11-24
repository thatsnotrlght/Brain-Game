# Brain vs. Rot

**Course:** CAP4052 – Game Design and Development  

## Team Members

| Team Member | Panther ID |
|---|---|
| Samuel Artiste | 6538723 |

## Individual Contribution - Samuel Artiste
I developed Brain vs. Rot from a three-lane defense prototype into a playable 3D arena. I chose how the player moves and builds on the battlefield, defined the tradeoff between the two turrets, and chose enemy size as a visual cue for speed and health. I also found and used brain, virus, and neuron models and implemented that into the game and worked on the HUD, health bars, and turret defense dotted range to know when enemies are in your turrets range. I tested these systems in the running game and used the results to identify some problems I may have had.

## Youtube Video Link
https://youtu.be/Mq4nFGgK75cs

## Approved Game Concept

**Brain vs. Rot** is a 3D lane-based tower defense game inspired by *Plants vs. Zombies*.

The original approved concept was for the player to defend their base against approaching enemies representing diseases or infections. Enemies advance through three lanes, and the player strategically places defenses to eliminate them before they reach the base. In HW#4, the player controls a character in a tiled arena. Viruses ender from random rows on the right, and the player walks to the elgibile tiles near the brain to construct defense turrets. The game introduces a brain-themed interpretation of traditional tower defense gameplay, emphasizing strategic placement, resource management, and survival.

## Game Objective

The objective is to defend the brain for 60 seconds without allowing the base's health to reach zero. You start with 5 brain health and 30 resources. Each living virus that corsses the defense line costs the brain 1 health, even if it crosses on an outer row. Each defeated virus earns 10 score and 10 resources. The objective is to survive until the countdown expires to win, with a timer you can see in the top left corner. If brain health reaches zero, the round ends in defeat. Then the player would press R to play again.

**Win Condition:** Survive 60 seconds with remaining base health.

**Lose Condition:** Base health reaches zero.

## Controls

| Key | Action |
|---|---|
| W,A,S,D | Moves the player around the arena |
| 1 | Select Neuron Blaster (20 resources) |
| 2 | Select Visual Cortex (30 resources) |
| E | Builds the selected turret on the tile under the player |
| R | Restart the game |
| Esc | Exit the game |

Game information, including health, score, resources, and win/loss status, is displayed in the window title.

## Implemented Features

- **3D Environment:** A three-dimensional playfield with a perspective camera, ground platform, and three lanes.
- **Two defenses with a placement tradeoff**. The Neuron Blaster costs 20, reaches 5 tiles, and deals 45 nominal damage per second. The Visual Cortex costs 30, reaches 3.5 tiles, and deals 85 nominal damage per second. Each attacks one living enemy in range per firing interval. A dotted ring shows the tower’s actual reach.
- **Enemy progression**. Viruses spawn about once per second on a random row. The first 15 seconds introduce medium viruses. Afterward, small fast viruses and large high-health viruses can also spawn.
- **Combat feedback**. Each virus has a health bar scaled to its own maximum health. The brain has a separate bar that drops when a virus gets through.
- **On-screen HUD**. The board shows the selected turret and cost, score, available resources, survival countdown, and end-of-round restart prompt. The window title also provides controls and build feedback.
- **3D presentation**. The arena uses a perspective camera, a marked build zone, and imported brain, virus, and neuron models. Both turrets share the neuron mesh and use different colors.
Restartable round. R clears enemies and towers and resets the player position, timer, score, resources, and brain health.
- **Resource Management:** Players earn resources from defeated enemies and spend them to purchase defenses.
- **Scoring:** Players earn points by defeating enemies.
- **Base Health:** Enemies that reach the base reduce its health.
- **Game States:** Win condition, game-over condition, and restart functionality.
- **3D Transformations:** Model, view, and projection matrices are used to position, scale, and render game objects.

## Technology Stack

- **Language:** C++
- **Graphics API:** OpenGL 3.3 Core Profile
- **Windowing and Input:** GLFW
- **OpenGL Function Loading:** GLAD
- **Mathematics:** GLM
- **Build System:** CMake
- **Development Environment:** Visual Studio on Windows

## Dependencies

The project requires:

- A C++ compiler supporting the project configuration
- CMake
- OpenGL
- GLFW
- GLAD
- GLM

## Build and Run Instructions

1. Open the project folder containing `CMakeLists.txt` in Visual Studio or another CMake-compatible development environment.
2. Ensure GLFW, GLAD, and GLM are available through the configured dependencies.
3. Configure the project using CMake.
4. Build the executable using the selected compiler and build configuration.
5. Run the generated executable.
6. Use keys 1 and 2 to purchase defenses, R to restart, and Esc to exit.

## Known Limitations

Contains one 60 second arena round. Viruses travel straight along their spawn row. Towers attack automatically, with range rings and health bars providing combat feedback. Both turrets share one neuron mesh. The current build uses flat colors and has no sound effects or projectile animations.

## External Assets and Credits

| Asset | Use in game | Creator, original source, and license |
|---|---|---|
| `assets/models/Brain.obj` | Defended brain | Creator: isabelle123; Name: The Brain; Source: https://3dwarehouse.sketchup.com/model/b869d976fc60e9ace4b68d3b17c43658/The-Brain  |
| `assets/models/virus.obj` | Virus enemies | Creator: Ashkan Alidi; Name: Virus; Source: https://3dwarehouse.sketchup.com/model/bacb4d26f185b2d2af7693d85e58498c/Virus |
| `assets/models/neuron.obj` | Shared mesh for both turrets | Creator: best-lemming; Name: brain nuron; Source: https://3dwarehouse.sketchup.com/model/a7aad01fef4c6b05664147e6dca8a559/brain-nuron |

- **tinyobjloader** was used for OBJ loading
- **stb_easy_font** was used for HUD lettering

## Generative AI Usage Statement

I used ChatGPT as a C++ and OpenGL coding assistant, helping to explain unfamiliar code, and help with implementation examples. It also was used to assist me with drafting the README. I made gameplay decisions and checked behavior in the running prototype. For example, to verify the enemy health bar, I placed a turret and compared an enemy moving through its attack range with one outside of it. I observed the enemy's health bar shrink. Verification was done through playtesting, running the prototype, & trial and error.

**Representative prompts:**
- "Yes its good, now can you assist me in adding a radius around the turrets instead of projectiles to show their range and such. I already added the models of the neuron, defined the model path in cmakelist, and loaded at startup and release at shutdown. I want them both to reference the neuron obj for now"
- "I'm not really sure where to add the return enemy.health. After that can you help me with the base health, game over condition and score counter."
