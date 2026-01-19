# 🎮 SFML Multi-Level Game Project

## C++ Game Development Project using SFML Framework

A **3-level 2D platformer / capture-shooter game** developed using **C++ and the SFML framework**.  
This project demonstrates **game states, physics, collision detection, enemy AI, scoring system, and menu handling** in a complete desktop game.

---

## 🎮 Game Overview

- **Genre:** 2D Platformer / Capture-Shooter  
- **Platform:** Desktop (Windows / Linux / macOS)  
- **Engine:** SFML 2.6+  
- **Language:** C++17  

---

## 🧠 Core Mechanics

- **Vacuum Capture**  
  Use `Space + WASD` to capture enemies with a flame effect  
  - Ghost → **50 points**  
  - Skeleton → **75 points**

- **Bullet Release**  
  Release captured enemies as projectiles  
  - `Z` → Single shot (1 captured enemy)  
  - `X` → Burst shot (up to 3 captured enemies)  
  - Bullet kills give **double points**:
    - Ghost → **100 points**
    - Skeleton → **150 points**

- **Three Levels** with unique layouts and enemy behavior  
- **Physics System:** Gravity, jumping, tile-based collision  
- **Lives System:** 3 hearts with invincibility frames after damage  
- **Scoring System:** Capture points, bullet kills, time bonus, no-damage bonus  

---

## 🎮 Controls

### Menus
↑ / ↓ Navigate
Enter Select

shell
Copy code

### Gameplay
WASD + Space Aim & Vacuum Capture
Z Single Shot
X Burst Shot
ESC Pause / Menu

yaml
Copy code

---

## 🏗️ Technical Architecture

### 🔁 Game States
STATE_MAIN_MENU → STATE_PLAYER_SELECT → STATE_PLAY_LEVEL[1–3]
↓
STATE_SHOW_CONTROLS / LEADERBOARD → STATE_EXIT_GAME

yaml
Copy code

---

### 📦 Data Structures

- **Level Grid**

char lvl[HEIGHT][WIDTH];
Loaded from ASCII level patterns

Enemies
Parallel arrays storing position, type, direction, and alive state

Bullets
Object pool system (no runtime memory allocation)

Captured Enemies
Stack-based storage for vacuum-captured enemies

⚙️ Key Systems
System	Implementation	Purpose
Tile Rendering	ASCII grid → sprite positioning	Level layout
Player Physics	Gravity & velocity clamping	Jump & fall
Enemy AI	Patrol + random pause/turn	Enemy behavior
Collision	AABB tile lookup	Player/enemy/bullet
Input	Edge-trigger key handling	Prevent key hold
Scoring	Frame counter → time calculation	Bonuses

📁 Project Structure
css
Copy code
Data/
├── block1.png
├── ghost.png
├── skeleton.png
├── player_yellow.png
├── player_green.png
├── flame.png
├── level_clear.png
├── game_over.png
└── menu assets...

src/
└── temblepop.cpp    # Complete game (single-file prototype)
🎯 Level Design
Level 1 – Basic Capture
makefile
Copy code
Ghosts:     8  (50 / 100 points)
Skeletons:  4  (75 / 150 points)
Grid:       14 x 14
Level 2 – Random Ghosts
css
Copy code
Ghosts with random pauses & direction changes
Grid: 16 x 14
Level 3 – Advanced Enemies
mathematica
Copy code
Invisible movement & Chelnov-style patterns
📊 Scoring System
Action	Ghost	Skeleton
Flame Capture	+50	+75
Bullet Kill	+100	+150
Damage Taken	-50	-50
Game Over	-200	-200

🎁 Level Clear Bonuses
Level Clear → +1000

No Damage → +1500

Time Bonus:

≤ 30s → +2000

≤ 45s → +1000

≤ 60s → +500

🔧 Build Instructions
Prerequisites
css
Copy code
g++ (GCC 9+)
SFML 2.6 development libraries
Install SFML
bash
Copy code
sudo apt install libsfml-dev     # Ubuntu / Debian
brew install sfml                # macOS
Compile & Run
bash
Copy code
g++ -o game main.cpp -lsfml-graphics -lsfml-window -lsfml-system
./game
🐛 Known Issues & TODO
Audio system

Particle effects

Smooth sprite animations

Mobile controls

Online leaderboard

Additional enemy types

📈 Performance
Fixed 60 FPS

Array-based object pooling

No runtime memory allocations

Single-pass rendering

🎓 Learning Outcomes
SFML fundamentals (sprites, textures, events)

Game loop & state machine design

2D physics and collision detection

Efficient data structures

Scoring balance & gameplay logic

Debugging with frame counters

👤 Author
Muhammmad Tasawer Sadiq
Islamabad, Pakistan

Learning C++ & SFML through iterative game development.

⭐ Star this repository if it helps your SFML learning
🐛 Issues & Pull Requests are welcome

Last Updated: January 2026
