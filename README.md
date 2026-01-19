# tumblepop-game
A Temple Run–style game developed as a university project.
SFML Multi-Level Game Project
C++ Game Development Project using SFML Framework

A 3-level 2D platformer game built with C++ and SFML. Features sprite rendering, collision detection, scoring system, enemy AI, player physics, and a complete menu system.

🎮 Game Overview
Genre: 2D Platformer / Capture-Shooter
Platform: Desktop (Windows/Linux/Mac)
Engine: SFML 2.6+
Language: C++17

Core Mechanics
Vacuum Capture: Use Space + WASD to capture ghosts (50pts) and skeletons (75pts) with a flame effect

Bullet Release: Press Z (single) or X (burst) to shoot captured enemies as projectiles (double points: 100/150pts)

3 Levels with unique enemy patterns and layouts

Physics: Gravity, jumping, collision with tile-based blocks

Scoring: Base capture, bullet kills, time bonuses, no-damage bonus

Lives: 3 health hearts, invincibility frames after damage

Controls
text
Main Menu: ↑↓ select, Enter choose
Player Select: ↑↓ choose, Enter start
Gameplay:
  WASD + Space = aim & vacuum capture
  Z = single shot (1 captured enemy)
  X = burst shot (up to 3 enemies)
  ESC = menu
🏗️ Technical Architecture
Game States
text
STATE_MAIN_MENU → STATE_PLAYER_SELECT → STATE_PLAY_LEVEL[1-3]
                  ↓
              STATE_SHOW_CONTROLS / LEADERBOARD → STATE_EXIT_GAME
Data Structures
Level Grid: char lvl[HEIGHT][WIDTH] from ASCII patterns

Enemies: Parallel arrays enemyX/Y[type][alive][dir][NUM_ENEMIES]

Bullets: Object pool bulletActive[X/Y/dir/type][MAX_BULLETS]

Captured: capturedType[count] stack for vacuum storage

Key Systems
System	Implementation	Purpose
Tile Rendering	ASCII → 2D grid → blockSprite.setPosition(j*CELL, i*CELL)	Block layout from patterns
Player Physics	velocityY += gravity, terminalVelocity clamp	Jump/fall mechanics
Enemy AI	Patrol + random pause/turn (rand()%240==0)	Ghost/skeleton behavior
Collision	AABB: (int)(pos/CELL_SIZE) tile lookup	Player/enemy/bullet/wall
Input	isKeyPressed() && !keyHeld edge trigger	Single-press detection
Scoring	Frame counter → elapsed = frameCount/60.0f	Time bonuses + capture points
📁 Project Structure
text
Data/
├── block1.png          # Wall tile (48x48)
├── ghost.png           # Enemy type 0
├── skeleton.png        # Enemy type 1
├── player_yellow.png   # Player sprites
├── player_green.png
├── flame.png           # Vacuum effect
├── level_clear.png
├── game_over.png
└── menu assets...
templepop.cpp           # Complete game
🎯 Level Design
Level 1: Basic Capture
text
Ghosts: 8  (50pts capture / 100pts bullet)
Skeletons: 4 (75pts / 150pts)
Grid: 14x14
Level 2: Random Ghosts
text
Ghosts with rand() pauses and direction flips
16x14 grid
Level 3: Advanced Enemies
text
Invisible + Chelnov patterns
📊 Scoring System
Action	Ghost	Skeleton
Flame Capture	+50	+75
Bullet Kill	+100	+150
Damage Taken	-50	-50
Game Over	-200	-200
Level Clear Bonuses:

Clear: +1000

No Damage: +1500

Time ≤30s: +2000 / ≤45s: +1000 / ≤60s: +500

🔧 Build Instructions
Prerequisites
text
g++ (GCC 9+)
SFML 2.6 development libraries
text
sudo apt install libsfml-dev  # Ubuntu/Debian
brew install sfml             # macOS
Compile
bash
g++ -o game main.cpp -lsfml-graphics -lsfml-window -lsfml-system
./game
🐛 Known Issues & TODO
 Audio system

 Particle effects

 Smooth sprite animations

 Mobile controls

 Online leaderboard

 More enemy types

📈 Performance
Fixed 60 FPS (setFramerateLimit(60))

Array-based pooling (no allocations during gameplay)

Single-pass rendering

🎓 Learning Outcomes
✅ SFML Fundamentals: Sprites, textures, events, window management
✅ Game Loops: State machines, fixed timestep
✅ 2D Physics: Gravity, collision, velocity
✅ Data Structures: 2D grids, parallel arrays, object pools
✅ Game Design: Capture → release mechanics, scoring balance
✅ Debugging: Console output, frame counters

Author
Muhammad Tasawer Sadiq
Islamabad, Pakistan
Learning C++ & SFML through iterative development

⭐ Star this repo if it helps your SFML learning!
🐛 Issues/PRs welcome for improvements

Last Updated: January 2026
