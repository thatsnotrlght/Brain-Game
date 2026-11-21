# Brain vs. Rot

**Course:** CAP4052 – Game Design and Development  

## Team Members

| Team Member | Panther ID |
|---|---|
| Samuel Artiste | 6538723 |

## Youtube Video Link
https://youtu.be/8oBfUV543iY

## Approved Game Concept

**Brain vs. Rot** is a 3D lane-based tower defense game inspired by *Plants vs. Zombies*.

The player must defend their base against approaching enemies representing diseases or infections. Enemies advance through three lanes, and the player strategically places defenses to eliminate them before they reach the base.

The game introduces a brain-themed interpretation of traditional tower defense gameplay, emphasizing strategic placement, resource management, and survival.

## Game Objective

The objective is to survive for **60 seconds** without allowing the base's health to reach zero.

Enemies spawn periodically and advance toward the player's base. The player must place automated defenses in the appropriate lanes to stop them.

Defeating enemies increases the player's score and earns resources that can be spent on additional defenses.

**Win Condition:** Survive 60 seconds with remaining base health.

**Lose Condition:** Base health reaches zero.

## Controls

| Key | Action |
|---|---|
| 1 | Purchase and place a defense in lane 1 |
| 2 | Purchase and place a defense in lane 2 |
| 3 | Purchase and place a defense in lane 3 |
| R | Restart the game |
| Esc | Exit the game |

Each defense costs 20 resources and can only be placed once per lane.

Game information, including health, score, resources, and win/loss status, is displayed in the window title.

## Implemented Features

- **3D Environment:** A three-dimensional playfield with a perspective camera, ground platform, and three lanes.
- **Enemy Spawning:** Enemies spawn at timed intervals and advance toward the base.
- **Enemy Movement:** Frame-rate-independent movement using delta time.
- **Defense Placement:** Keyboard-controlled placement of automated defenses across three lanes.
- **Automatic Combat:** Defenses damage enemies within their attack range in the corresponding lane.
- **Enemy Health:** Enemies have health values and are removed when defeated.
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
6. Use keys 1, 2, and 3 to purchase defenses, R to restart, and Esc to exit.

## Known Limitations

- Game objects currently use simple cube-based geometry rather than detailed 3D character models.
- Combat is automatic and does not include visible projectiles or attack animations.
- Defenses attack eligible enemies in range rather than selecting a single target.
- Each lane supports only one placed defense.
- Game statistics and instructions are displayed in the window title instead of a dedicated in-game HUD.
- The game currently uses one level with a fixed survival objective.
- Enemy behavior is limited to advancing along predefined lanes.
- Sound effects, advanced lighting, and detailed textures have not been implemented.

## External Assets and Credits

No external graphical models, textures, music, or sound effects are used in the current implementation as of yet.

## Generative AI Usage Statement

ChatGPT was used as a debugging assistant during development, as well as to assist in filling in information for the README.

**Representative prompts:**
- "This is what I have in my .cpp and my .h right now. Can you help identify any potential issues."
- "I'm not really sure where to add the return enemy.health. After that can you help me with the base health, game over condition and score counter."
