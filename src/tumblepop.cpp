#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace sf;
using namespace std;



// ======================================================
// FUNCTION DECLARATIONS
// ======================================================
int clampInt(int v, int lo, int hi);
int loadLeaderboardFromScoreFile(string scoreLevel[], string scoreResult[],
                                 int scoreValue[], float scoreTime[]);
void sortScoresByDescendingScore(string scoreLevel[], string scoreResult[],
                                 int scoreValue[], float scoreTime[], int count);

void runMainMenu(RenderWindow& window, int& state, int& nextLevel, int screenX, int screenY);
void runPlayerSelect(RenderWindow& window, int& state, int nextLevel, int& chosenPlayer,
                     int screenX, int screenY);
void runControlsScreen(RenderWindow& window, int& state, int screenX, int screenY);
void runLeaderboardScreen(RenderWindow& window, int& state, int screenX, int screenY);

void runLevel1(RenderWindow& window, int& state, int chosenPlayer, int screenX, int screenY);
void runLevel2(RenderWindow& window, int& state, int chosenPlayer, int screenX, int screenY);
void runLevel3(RenderWindow& window, int& state, int chosenPlayer, int screenX, int screenY);

const int STATE_MAIN_MENU      = 0;
const int STATE_PLAYER_SELECT  = 1;
const int STATE_PLAY_LEVEL1    = 2;
const int STATE_PLAY_LEVEL2    = 3;
const int STATE_PLAY_LEVEL3    = 4;
const int STATE_SHOW_CONTROLS  = 5;
const int STATE_SHOW_LEADERBOARD = 6;

const int STATE_EXIT_GAME      = 7;
const int MAX_SCORE_ENTRIES = 100;

int main()
{
    srand((unsigned)time(NULL));

    const int SCREEN_X = 16 * 48;
    const int SCREEN_Y = 14 * 48;

    RenderWindow window(VideoMode(SCREEN_X, SCREEN_Y), "Tumble-POP");
    window.setFramerateLimit(60);

    Music menuMusic;
    if (menuMusic.openFromFile("Data/mus.ogg"))
    {
        menuMusic.setLoop(true);
        menuMusic.setVolume(60.f);
        menuMusic.play();
    }

    int state        = STATE_MAIN_MENU;
    int nextLevel    = 0;
    int chosenPlayer = 0;

    while (window.isOpen())
    {
        Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == Event::Closed)
                window.close();
        }

        if (!window.isOpen()) break;

        // ========== STATE CONTROL ==========
        if (state == STATE_MAIN_MENU)
        {
            runMainMenu(window, state, nextLevel, SCREEN_X, SCREEN_Y);
        }
        else if (state == STATE_PLAYER_SELECT)
        {
            runPlayerSelect(window, state, nextLevel, chosenPlayer, SCREEN_X, SCREEN_Y);
        }
        else if (state == STATE_PLAY_LEVEL1)
        {
            runLevel1(window, state, chosenPlayer, SCREEN_X, SCREEN_Y);
        }
        else if (state == STATE_PLAY_LEVEL2)
        {
            runLevel2(window, state, chosenPlayer, SCREEN_X, SCREEN_Y);
        }
        else if (state == STATE_PLAY_LEVEL3)
        {
            runLevel3(window, state, chosenPlayer, SCREEN_X, SCREEN_Y);
        }
        else if (state == STATE_SHOW_CONTROLS)
        {
            runControlsScreen(window, state, SCREEN_X, SCREEN_Y);
        }
        else if (state == STATE_SHOW_LEADERBOARD)
        {
            runLeaderboardScreen(window, state, SCREEN_X, SCREEN_Y);
        }
        else if (state == STATE_EXIT_GAME)
        {
            window.close();
        }
    }

    return 0;
}

// ======================================================
// HELPER FUNCTIONS
// ======================================================

int clampInt(int v, int lo, int hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

int loadLeaderboardFromScoreFile(string scoreLevel[], string scoreResult[],
                                 int scoreValue[], float scoreTime[])
{
    ifstream in("Data/levelscore.txt");
    if (!in)
    {
        cout << "Cannot open Data/levelscore.txt\n";
        return 0;
    }

    int count = 0;
    while (count < MAX_SCORE_ENTRIES)
    {
        string level;
        int score;

        if (!(in >> level >> score))
            break; 

        scoreLevel[count]  = level;      // e.g. LEVEL1 or LEVEL2
        scoreResult[count] = "";        
        scoreValue[count]  = score;    
        count++;
    }

    cout << "Loaded " << count << " score entries from levelscore.txt\n";
    return count;
}


void sortScoresByDescendingScore(string scoreLevel[], string scoreResult[],
                                 int scoreValue[], float scoreTime[], int count)
{
    for (int i = 0; i < count - 1; ++i)
    {
        for (int j = i + 1; j < count; ++j)
        {
            if (scoreValue[j] > scoreValue[i])
            {
                int tmpScore = scoreValue[i];
                scoreValue[i] = scoreValue[j];
                scoreValue[j] = tmpScore;

                string tmpLevel = scoreLevel[i];
                scoreLevel[i] = scoreLevel[j];
                scoreLevel[j] = tmpLevel;

                string tmpResult = scoreResult[i];
                scoreResult[i] = scoreResult[j];
                scoreResult[j] = tmpResult;

                float tmpTime = scoreTime[i];
                scoreTime[i] = scoreTime[j];
                scoreTime[j] = tmpTime;
            }
        }
    }
}

// ======================================================
// MAIN MENU
// ======================================================
void runMainMenu(RenderWindow& window, int& state, int& nextLevel, int screenX, int screenY)
{
    Texture bgTex, level1Tex, level2Tex, level3Tex;
    Texture controlsTex, boardTex, exitTex, titleTex;

    bgTex.loadFromFile("Data/menu_bg.png");
    level1Tex.loadFromFile("Data/menu_level1.png");
    level2Tex.loadFromFile("Data/menu_level2.png");
    level3Tex.loadFromFile("Data/menu_level3.png");
    controlsTex.loadFromFile("Data/menu_controls.png");
    boardTex.loadFromFile("Data/menu_leaderboard.png");
    exitTex.loadFromFile("Data/menu_exit.png");
    titleTex.loadFromFile("Data/title_menu.png");

    Sprite bg(bgTex);
    bg.setScale((float)screenX / bgTex.getSize().x,
                (float)screenY / bgTex.getSize().y);

    Sprite title(titleTex);
    Vector2u titleSz = titleTex.getSize();
    float titleScale = (float)screenX * 0.95 / titleSz.x;
    title.setScale(titleScale, titleScale);
    float titleX = (screenX - titleSz.x * titleScale) / 2.0f;
    float titleY = 10.0f;
    title.setPosition(titleX, titleY);

    const int ITEM_COUNT = 6;
    Sprite items[ITEM_COUNT];

    items[0].setTexture(level1Tex);
    items[1].setTexture(level2Tex);
    items[2].setTexture(level3Tex);
    items[3].setTexture(controlsTex);
    items[4].setTexture(boardTex);
    items[5].setTexture(exitTex);

    int selected = 0;
    bool upHeld = false, downHeld = false, enterHeld = false;

    cout << "Main Menu: Up/Down choose, Enter select\n";

    while (window.isOpen() && state == STATE_MAIN_MENU)
    {
        Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == Event::Closed)
            {
                window.close();
                return;
            }
        }

        bool upNow   = Keyboard::isKeyPressed(Keyboard::Up);
        bool downNow = Keyboard::isKeyPressed(Keyboard::Down);
        bool enterNow= Keyboard::isKeyPressed(Keyboard::Enter);

        if (upNow && !upHeld)
            selected = (selected + ITEM_COUNT - 1) % ITEM_COUNT;
        if (downNow && !downHeld)
            selected = (selected + 1) % ITEM_COUNT;

        upHeld   = upNow;
        downHeld = downNow;

        if (enterNow && !enterHeld)
        {
            if (selected == 0) { nextLevel = 1; state = STATE_PLAYER_SELECT; }
            else if (selected == 1) { nextLevel = 2; state = STATE_PLAYER_SELECT; }
            else if (selected == 2) { nextLevel = 3; state = STATE_PLAYER_SELECT; }
            else if (selected == 3) { state = STATE_SHOW_CONTROLS; }
            else if (selected == 4) { state = STATE_SHOW_LEADERBOARD; }
            else if (selected == 5) { state = STATE_EXIT_GAME; }
        }
        enterHeld = enterNow;

        window.clear();
        window.draw(bg);
        window.draw(title);

        float startY = 140.0;
        float stepY  = 80.0;

        for (int i = 0; i < ITEM_COUNT; ++i)
        {
            float scale = (i == selected) ? 1.08 : 1.0;
            items[i].setScale(scale, scale);

            Vector2u sz = items[i].getTexture()->getSize();
            float width = sz.x * scale;
            float x     = (screenX - width) / 2.0;
            float y     = startY + i * stepY;

            items[i].setPosition(x, y);
            window.draw(items[i]);
        }

        window.display();
    }
}

// ======================================================
// PLAYER SELECT
// ======================================================
void runPlayerSelect(RenderWindow& window, int& state, int nextLevel, int& chosenPlayer,
                     int screenX, int screenY)
{
    Texture bgTex, yellowTex, greenTex, headerTex;
    bgTex.loadFromFile("Data/menu_bg.png");
    yellowTex.loadFromFile("Data/yellow.png");
    greenTex.loadFromFile("Data/green.png");
    headerTex.loadFromFile("Data/player_select_header.png");

    Sprite bg(bgTex);
    bg.setScale((float)screenX / bgTex.getSize().x,
                (float)screenY / bgTex.getSize().y);

    Sprite header(headerTex);
    Vector2u headerSz = headerTex.getSize();
    float headerScale = (float)screenX * 0.9/ headerSz.x;
    header.setScale(headerScale, headerScale);
    float headerX = (screenX - headerSz.x * headerScale) / 2.0;
    float headerY = 30.0f;
    header.setPosition(headerX, headerY);

    Sprite yellowSprite(yellowTex);
    Sprite greenSprite(greenTex);

    float desiredHeight = 2.0 * 48.0;
    float scaleY = desiredHeight / (float)yellowTex.getSize().y;
    float scaleX = scaleY;
    yellowSprite.setScale(scaleX, scaleY);
    greenSprite.setScale(scaleX, scaleY);

    float centerX = screenX / 2.0;
    float baseY   = screenY / 2.0 + 80.0;
    float gapX    = 220.0;

    int selected = 0;
    bool leftHeld = false, rightHeld = false, enterHeld = false;

    cout << "Player Select: Left/Right choose, Enter confirm, Esc back.\n";

    while (window.isOpen() && state == STATE_PLAYER_SELECT)
    {
        Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == Event::Closed)
            {
                window.close();
                return;
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            state = STATE_MAIN_MENU;
            return;
        }

        bool leftNow  = Keyboard::isKeyPressed(Keyboard::Left);
        bool rightNow = Keyboard::isKeyPressed(Keyboard::Right);
        bool enterNow = Keyboard::isKeyPressed(Keyboard::Enter);

        if (leftNow && !leftHeld)  { selected = 0; cout << "Selected YELLOW\n"; }
        if (rightNow && !rightHeld){ selected = 1; cout << "Selected GREEN\n"; }
        leftHeld  = leftNow;
        rightHeld = rightNow;

        if (enterNow && !enterHeld)
        {
            chosenPlayer = selected;
            cout << "Starting Level " << nextLevel << " with player "
                 << (selected == 0 ? "YELLOW" : "GREEN") << "\n";
            if (nextLevel == 1)      state = STATE_PLAY_LEVEL1;
            else if (nextLevel == 2) state = STATE_PLAY_LEVEL2;
            else if (nextLevel == 3) state = STATE_PLAY_LEVEL3;
            return;
        }
        enterHeld = enterNow;

        window.clear();
        window.draw(bg);
        window.draw(header);

        float yelX = centerX - gapX;
        float yelY = baseY;
        float grnX = centerX + gapX;
        float grnY = baseY;

        if (selected == 0)
        {
            yellowSprite.setScale(scaleX * 1.2, scaleY * 1.2);
            greenSprite.setScale(scaleX, scaleY);
            yellowSprite.setPosition(yelX, yelY - 16.0);
            greenSprite.setPosition(grnX, grnY);
        }
        else
        {
            yellowSprite.setScale(scaleX, scaleY);
            greenSprite.setScale(scaleX * 1.2, scaleY * 1.2);
            yellowSprite.setPosition(yelX, yelY);
            greenSprite.setPosition(grnX, grnY - 16.0);
        }

        window.draw(yellowSprite);
        window.draw(greenSprite);
        window.display();
    }
}

// ======================================================
// CONTROLS SCREEN
// ======================================================
void runControlsScreen(RenderWindow& window, int& state, int screenX, int screenY)
{
    Texture controlsTex;
    controlsTex.loadFromFile("Data/controls_page.png");

    Sprite img(controlsTex);
    img.setScale((float)screenX / controlsTex.getSize().x,
                 (float)screenY / controlsTex.getSize().y);

    bool enterHeld = false;
    bool escHeld   = false;

    cout << "Controls Screen: Press Enter or Esc to go back.\n";

    while (window.isOpen() && state == STATE_SHOW_CONTROLS)
    {
        Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == Event::Closed)
            {
                window.close();
                return;
            }
        }

        bool enterNow = Keyboard::isKeyPressed(Keyboard::Enter);
        bool escNow   = Keyboard::isKeyPressed(Keyboard::Escape);

        if ((enterNow && !enterHeld) || (escNow && !escHeld))
        {
            cout << "Returning to MAIN_MENU from Controls\n";
            state = STATE_MAIN_MENU;
            return;
        }

        enterHeld = enterNow;
        escHeld   = escNow;

        window.clear();
        window.draw(img);
        window.display();
    }
}

// ======================================================
// LEADERBOARD SCREEN
// ======================================================
void runLeaderboardScreen(RenderWindow& window, int& state, int screenX, int screenY)
{
    Texture boardBgTex;
    boardBgTex.loadFromFile("Data/leaderboard_bg.png");

    Sprite bg(boardBgTex);
    bg.setScale((float)screenX / boardBgTex.getSize().x,
                (float)screenY / boardBgTex.getSize().y);

    string scoreLevel[MAX_SCORE_ENTRIES];
    string scoreResult[MAX_SCORE_ENTRIES];
    int    scoreValue[MAX_SCORE_ENTRIES];
    float  scoreTime[MAX_SCORE_ENTRIES];

    int count = loadLeaderboardFromScoreFile(scoreLevel, scoreResult, scoreValue, scoreTime);
    sortScoresByDescendingScore(scoreLevel, scoreResult, scoreValue, scoreTime, count);

    int top = (count < 7 ? count : 7);

    cout << "\n";
    cout << "===== LEADERBOARD (Top 7 from levelscore.txt) =====\n";

    if (top == 0)
    {
        cout << "No scores found.\n";
    }
    else
    {
        for (int i = 0; i < top; ++i)
        {
            cout << (i + 1) << ". "
                 << scoreLevel[i] << " " << scoreResult[i]
                 << "  Score: " << scoreValue[i]
                 << "  Time: "  << scoreTime[i] << "s\n";
        }
    }

    cout << "====================================================\n";
    cout << "Press Enter or Esc to go back to menu.\n\n";

    bool enterHeld = false;
    bool escHeld   = false;

    while (window.isOpen() && state == STATE_SHOW_LEADERBOARD)
    {
        Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == Event::Closed)
            {
                window.close();
                return;
            }
        }

        bool enterNow = Keyboard::isKeyPressed(Keyboard::Enter);
        bool escNow   = Keyboard::isKeyPressed(Keyboard::Escape);

        if ((enterNow && !enterHeld) || (escNow && !escHeld))
        {
            cout << "Returning to MAIN_MENU from Leaderboard\n";
            state = STATE_MAIN_MENU;
            return;
        }

        enterHeld = enterNow;
        escHeld   = escNow;

        window.clear();
        window.draw(bg);
        window.display();
    }
}

//----- LEVEL 1 ---- 

void runLevel1(RenderWindow& window, int& state, int chosenPlayer, int screenX, int screenY)
{
    const int CELL_SIZE    = 48;
    const int LEVEL_WIDTH  = 14;
    const int LEVEL_HEIGHT = 14;

    const int SCREEN_X = screenX;
    const int SCREEN_Y = screenY;

    const float speedScale = 1.0; //  enemies  speed

    float playerSpeed  = 2.0;
    float vacuumPower  = 1.5; 

    // =================  LEVEL GRID =================
    const char* levelRows[LEVEL_HEIGHT] =
    {
        "##############",
        "#............#",
        "#............#",
        "#.##########.#",
        "#.....##.....#",
        "###..####..###",
        "#....#..#....#",
        "#..###..###..#",
        "#....#..#....#",
        "###..####..###",
        "#.....##.....#",
        "#.##########.#",
        "#............#",
        "##############"
    };

    char lvl[LEVEL_HEIGHT][LEVEL_WIDTH];
    for (int i = 0; i < LEVEL_HEIGHT; i++)
        for (int j = 0; j < LEVEL_WIDTH; j++)
            lvl[i][j] = (levelRows[i][j] == '#') ? '#' : ' ';

    // =================  GRAPHICS =================
    Texture bgTex;    bgTex.loadFromFile("Data/bg.png");
    Sprite  bgSprite; bgSprite.setTexture(bgTex);
    bgSprite.setScale(
        (float)SCREEN_X / bgTex.getSize().x,
        (float)SCREEN_Y / bgTex.getSize().y
    );

    Texture blockTexture; blockTexture.loadFromFile("Data/block1.png");
    Sprite  blockSprite;  blockSprite.setTexture(blockTexture);
    blockSprite.setScale(
        (float)CELL_SIZE / blockTexture.getSize().x,
        (float)CELL_SIZE / blockTexture.getSize().y
    );

    Texture ghostTex;    ghostTex.loadFromFile("Data/ghost.png");
    Texture skeletonTex; skeletonTex.loadFromFile("Data/skeleton.png");

    // 4 rotated flame textures
    Texture flameRightTex; flameRightTex.loadFromFile("Data/flame_right.png");
    Texture flameLeftTex;  flameLeftTex .loadFromFile("Data/flame_left.png");
    Texture flameUpTex;    flameUpTex   .loadFromFile("Data/flame_up.png");
    Texture flameDownTex;  flameDownTex .loadFromFile("Data/flame_down.png");
    Sprite  flameSprite;
    int flameW = (int)flameRightTex.getSize().x;
    int flameH = (int)flameRightTex.getSize().y;

    Texture playerTexture;
    if (chosenPlayer == 0)
        playerTexture.loadFromFile("Data/yellow.png");
    else
        playerTexture.loadFromFile("Data/green.png");

    Sprite playerSprite; playerSprite.setTexture(playerTexture);
    float pScaleX = (float)CELL_SIZE / playerTexture.getSize().x;
    float pScaleY = (float)CELL_SIZE / playerTexture.getSize().y;
    int   PlayerWidth  = CELL_SIZE;
    int   PlayerHeight = CELL_SIZE;

    float gScaleX = (float)CELL_SIZE / ghostTex.getSize().x;
    float gScaleY = (float)CELL_SIZE / ghostTex.getSize().y;
    float sScaleX = (float)CELL_SIZE / skeletonTex.getSize().x;
    float sScaleY = (float)CELL_SIZE / skeletonTex.getSize().y;

    // End‑screen pictures
    Texture gameOverTex, levelClearTex;
    bool hasGameOver   = gameOverTex.loadFromFile("Data/gameover.png");
    bool hasLevelClear = levelClearTex.loadFromFile("Data/levelclear.png");
    Sprite endSprite;

    // =================  MUSIC =================
    Music music;
    if (music.openFromFile("Data/mus.ogg")) {
        music.setLoop(true);
        music.setVolume(60.0);
        music.play();
    }

    // ================= PLAYER PHYSICS =================
    float player_x = CELL_SIZE * 2;
    float player_y = CELL_SIZE * 10;

    const float jumpStrength      = -8.0;
    const float gravity           = 0.2;
    float       velocityY         = 0.0;
    float       terminal_Velocity = 0.8;

    bool onGround    = false;
    bool facingRight = true;
    bool zHeld       = false;
    bool xHeld       = false;
    bool upHeld      = false;

    int  shotDir   = 0; // 0=right,1=left,2=up,3=down
    int  aimDir    = 0;
    int  invFrames = 0;
    const int INV_FRAMES = 60;

    // ================= 5) ENEMIES =================
    const int NUM_GHOSTS    = 8;
    const int NUM_SKELETONS = 4;
    const int NUM_ENEMIES   = NUM_GHOSTS + NUM_SKELETONS;

    float enemyX[NUM_ENEMIES];
    float enemyY[NUM_ENEMIES];
    int   enemyType[NUM_ENEMIES];
    float enemyDir[NUM_ENEMIES];
    bool  enemyAlive[NUM_ENEMIES];

    // Ghosts
    enemyType[0] = 0; enemyAlive[0] = true; enemyDir[0] =  1; enemyX[0] = 3  * CELL_SIZE; enemyY[0] = 2  * CELL_SIZE;
    enemyType[1] = 0; enemyAlive[1] = true; enemyDir[1] = -1; enemyX[1] = 9  * CELL_SIZE; enemyY[1] = 2  * CELL_SIZE;
    enemyType[2] = 0; enemyAlive[2] = true; enemyDir[2] =  1; enemyX[2] = 2  * CELL_SIZE; enemyY[2] = 4  * CELL_SIZE;
    enemyType[3] = 0; enemyAlive[3] = true; enemyDir[3] = -1; enemyX[3] = 11 * CELL_SIZE; enemyY[3] = 4  * CELL_SIZE;
    enemyType[4] = 0; enemyAlive[4] = true; enemyDir[4] =  1; enemyX[4] = 2  * CELL_SIZE; enemyY[4] = 8  * CELL_SIZE;
    enemyType[5] = 0; enemyAlive[5] = true; enemyDir[5] = -1; enemyX[5] = 11 * CELL_SIZE; enemyY[5] = 8  * CELL_SIZE;
    enemyType[6] = 0; enemyAlive[6] = true; enemyDir[6] =  1; enemyX[6] = 3  * CELL_SIZE; enemyY[6] =10  * CELL_SIZE;
    enemyType[7] = 0; enemyAlive[7] = true; enemyDir[7] = -1; enemyX[7] =10 * CELL_SIZE; enemyY[7] =10  * CELL_SIZE;

    // Skeletons
    enemyType[8]  = 1; enemyAlive[8]  = true; enemyDir[8]  =  1; enemyX[8]  = 3  * CELL_SIZE; enemyY[8]  = 6  * CELL_SIZE;
    enemyType[9]  = 1; enemyAlive[9]  = true; enemyDir[9]  = -1; enemyX[9]  =10 * CELL_SIZE; enemyY[9]  = 6  * CELL_SIZE;
    enemyType[10] = 1; enemyAlive[10] = true; enemyDir[10] =  1; enemyX[10] = 3  * CELL_SIZE; enemyY[10] =12 * CELL_SIZE;
    enemyType[11] = 1; enemyAlive[11] = true; enemyDir[11] = -1; enemyX[11] =10 * CELL_SIZE; enemyY[11] =12 * CELL_SIZE;

    float ghostSpeed    = 0.6 * speedScale;
    float skeletonSpeed = 0.6 * speedScale;

    // ================= 6) CAPTURE / RELEASE & SCORE =================
    const int MAX_CAPTURE = 3;
    int  capturedType[MAX_CAPTURE];
    int  capturedCount = 0;

    const int MAX_RELEASE = 3;
    bool  releaseActive[MAX_RELEASE];
    int   releaseType[MAX_RELEASE];
    float releaseX[MAX_RELEASE];
    float releaseY[MAX_RELEASE];
    int   releaseTimer[MAX_RELEASE];
    for (int i = 0; i < MAX_RELEASE; i++) releaseActive[i] = false;

    int  score         = 0;
    int  health        = 3;
    bool tookDamage    = false;
    int  streak        = 0;
    int  frameCount    = 0;
    bool levelFinished = false;
    bool gameOver      = false;
    bool bonusesApplied= false;

    // =================  BULLETS for Z / X =================
    const int MAX_BULLETS = 10;
    bool  bulletActive[MAX_BULLETS];
    float bulletX[MAX_BULLETS];
    float bulletY[MAX_BULLETS];
    int   bulletDir[MAX_BULLETS];   // 0 right,1 left,2 up,3 down
    int   bulletType[MAX_BULLETS];  // 0 = ghost, 1 = skeleton
    float bulletSpeed = 3.0;       // visible, not instant
    for (int i = 0; i < MAX_BULLETS; ++i) {
        bulletActive[i] = false;
        bulletType[i]   = -1;
    }

    cout << "===== LEVEL 1 STARTED =====\n";
    cout << "Player: " << (chosenPlayer == 0 ? "YELLOW" : "GREEN") << "\n";
    cout << "Screen: " << SCREEN_X << "x" << SCREEN_Y << "\n";

    // ================= MAIN LOOP (STATE VERSION) =================
    while (window.isOpen() && state == STATE_PLAY_LEVEL1)
    {
        Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == Event::Closed)
            {
                window.close();
                return;
            }
        }

        // ESC -> back to main menu, not exit program
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            cout << "Returning to MAIN_MENU from Level 1\n";
            state = STATE_MAIN_MENU;
            return;
        }

        if (!levelFinished && !gameOver)
            frameCount++;

        if (invFrames > 0)
            invFrames--;

        // ---------- UPDATE (only while playing) ----------
        if (!levelFinished && !gameOver)
        {
            // ---- horizontal movement ----
            float moveX = 0.0;
            bool leftDown  = Keyboard::isKeyPressed(Keyboard::Left);
            bool rightDown = Keyboard::isKeyPressed(Keyboard::Right);
            if (leftDown && !rightDown)
            {
                moveX -= playerSpeed;
                facingRight = false;
            }
            else if (rightDown && !leftDown)
            {
                moveX += playerSpeed;
                facingRight = true;
            }

            float newPlayerX = player_x + moveX;
            int midRow  = (int)(player_y + PlayerHeight / 2) / CELL_SIZE;
            int leftCol = (int)(newPlayerX) / CELL_SIZE;
            int rightCol= (int)(newPlayerX + PlayerWidth - 1) / CELL_SIZE;
            midRow  = clampInt(midRow,  0, LEVEL_HEIGHT - 1);
            leftCol = clampInt(leftCol, 0, LEVEL_WIDTH  - 1);
            rightCol= clampInt(rightCol,0, LEVEL_WIDTH  - 1);
            bool collideHoriz =
                (lvl[midRow][leftCol] == '#') ||
                (lvl[midRow][rightCol]== '#');
            if (!collideHoriz)
                player_x = newPlayerX;

            // ---- aim direction ----
            aimDir = facingRight ? 0 : 1;
            bool wDown     = Keyboard::isKeyPressed(Keyboard::W);
            bool sDown     = Keyboard::isKeyPressed(Keyboard::S);
            bool spaceDown = Keyboard::isKeyPressed(Keyboard::Space);

            if (Keyboard::isKeyPressed(Keyboard::A)) aimDir = 1;
            if (Keyboard::isKeyPressed(Keyboard::D)) aimDir = 0;
            if (wDown && spaceDown)                 aimDir = 2;
            else if (sDown)                         aimDir = 3;

            shotDir = aimDir;

            // ---- canJump ----
            bool canJump = false;
            int footRow  = (int)(player_y + PlayerHeight + 1) / CELL_SIZE;
            int footLeft = (int)(player_x) / CELL_SIZE;
            int footRight= (int)(player_x + PlayerWidth - 1) / CELL_SIZE;
            int footMid  = (int)(player_x + PlayerWidth / 2) / CELL_SIZE;
            if (footRow >= 0 && footRow < LEVEL_HEIGHT)
            {
                footLeft  = clampInt(footLeft,  0, LEVEL_WIDTH - 1);
                footRight = clampInt(footRight, 0, LEVEL_WIDTH - 1);
                footMid   = clampInt(footMid,   0, LEVEL_WIDTH - 1);
                char fl = lvl[footRow][footLeft];
                char fm = lvl[footRow][footMid];
                char fr = lvl[footRow][footRight];
                if (fl == '#' || fm == '#' || fr == '#')
                    canJump = true;
            }

            // ---- jump start ----
            bool upNow = Keyboard::isKeyPressed(Keyboard::Up);
            if (upNow && !upHeld && canJump)
            {
                int headRow   = (int)(player_y - 1) / CELL_SIZE;
                int headLeft  = (int)(player_x) / CELL_SIZE;
                int headRight = (int)(player_x + PlayerWidth - 1) / CELL_SIZE;
                bool ceiling = false;
                if (headRow >= 0 && headRow < LEVEL_HEIGHT)
                {
                    headLeft  = clampInt(headLeft,  0, LEVEL_WIDTH - 1);
                    headRight = clampInt(headRight, 0, LEVEL_WIDTH - 1);
                    if (lvl[headRow][headLeft] == '#' ||
                        lvl[headRow][headRight]== '#')
                        ceiling = true;
                }
                if (!ceiling)
                {
                    velocityY = jumpStrength;
                    onGround  = false;
                }
            }
            upHeld = upNow;

            // ---- vertical movement ----
            float offset_y = player_y + velocityY;
            if (velocityY < 0) // up
            {
                int topRow   = (int)(offset_y) / CELL_SIZE;
                int topLeft  = (int)(player_x) / CELL_SIZE;
                int topRight = (int)(player_x + PlayerWidth - 1) / CELL_SIZE;
                topRow   = clampInt(topRow,   0, LEVEL_HEIGHT - 1);
                topLeft  = clampInt(topLeft,  0, LEVEL_WIDTH  - 1);
                topRight = clampInt(topRight, 0, LEVEL_WIDTH  - 1);
                bool hitCeiling =
                    (lvl[topRow][topLeft] == '#') ||
                    (lvl[topRow][topRight]== '#');
                if (hitCeiling)
                {
                    player_y  = (topRow + 1) * CELL_SIZE;
                    velocityY = 0.0;
                }
                else
                {
                    player_y  = offset_y;
                    velocityY += gravity;
                }
            }
            else // down / stand
            {
                int bottomRow = (int)(offset_y + PlayerHeight) / CELL_SIZE;
                int left2  = (int)(player_x) / CELL_SIZE;
                int right2 = (int)(player_x + PlayerWidth - 1) / CELL_SIZE;
                int mid2   = (int)(player_x + PlayerWidth / 2) / CELL_SIZE;
                bottomRow = clampInt(bottomRow, 0, LEVEL_HEIGHT - 1);
                left2     = clampInt(left2,     0, LEVEL_WIDTH  - 1);
                right2    = clampInt(right2,    0, LEVEL_WIDTH  - 1);
                mid2      = clampInt(mid2,      0, LEVEL_WIDTH  - 1);
                char bl = lvl[bottomRow][left2];
                char br = lvl[bottomRow][right2];
                char bm = lvl[bottomRow][mid2];
                if (bl == '#' || bm == '#' || br == '#')
                {
                    onGround  = true;
                    velocityY = 0.0f;
                    player_y  = bottomRow * CELL_SIZE - PlayerHeight - 1;
                }
                else
                {
                    player_y  = offset_y;
                    onGround  = false;
                    velocityY += gravity;
                    if (velocityY > terminal_Velocity)
                        velocityY = terminal_Velocity;
                }
            }

            // ---- enemy patrol ----
            for (int i = 0; i < NUM_ENEMIES; i++)
            {
                if (!enemyAlive[i]) continue;
                float speed = (enemyType[i] == 0) ? ghostSpeed : skeletonSpeed;
                float dir   = enemyDir[i];
                float exNew = enemyX[i] + dir * speed;
                int erow    = (int)(enemyY[i] + PlayerHeight / 2) / CELL_SIZE;
                int ecolL   = (int)(exNew) / CELL_SIZE;
                int ecolR   = (int)(exNew + PlayerWidth - 1) / CELL_SIZE;
                erow  = clampInt(erow,  0, LEVEL_HEIGHT - 1);
                ecolL = clampInt(ecolL, 0, LEVEL_WIDTH  - 1);
                ecolR = clampInt(ecolR, 0, LEVEL_WIDTH  - 1);
                bool collideEnemy =
                    (lvl[erow][ecolL] == '#') ||
                    (lvl[erow][ecolR] == '#');
                if (collideEnemy)
                    enemyDir[i] = -dir;
                else
                    enemyX[i] = exNew;
            }

            // ---- player vs enemy collision ----
            if (invFrames == 0)
            {
                for (int i = 0; i < NUM_ENEMIES; i++)
                {
                    if (!enemyAlive[i]) continue;
                    int pRow = (int)(player_y + PlayerHeight / 2) / CELL_SIZE;
                    int pCol = (int)(player_x + PlayerWidth  / 2) / CELL_SIZE;
                    int eRow = (int)(enemyY[i] + CELL_SIZE / 2) / CELL_SIZE;
                    int eCol = (int)(enemyX[i] + CELL_SIZE / 2) / CELL_SIZE;
                    if (pRow != eRow || pCol != eCol) continue;

                    health--;
                    invFrames   = INV_FRAMES;
                    tookDamage  = true;
                    streak      = 0;
                    if (health > 0) score -= 50;
                    else            score -= 200;
                    break;
                }
            }

            if (health <= 0)
                gameOver = true;

            // ---- vacuum capture with 4 flame textures ----
            bool flameOnLogic = Keyboard::isKeyPressed(Keyboard::Space);
            float vacX = 0.0, vacY = 0.0, vacW = 0.0, vacH = 0.0;

            if (flameOnLogic)
            {
                if      (aimDir == 0) flameSprite.setTexture(flameRightTex);
                else if (aimDir == 1) flameSprite.setTexture(flameLeftTex);
                else if (aimDir == 2) flameSprite.setTexture(flameUpTex);
                else                  flameSprite.setTexture(flameDownTex);

                vacW = (float)flameW;
                vacH = (float)flameH;

                if (aimDir == 0){
                    vacX = player_x + PlayerWidth;
                    vacY = player_y + PlayerHeight / 2 - vacH / 2;
                }else if (aimDir == 1){
                    vacX = player_x - vacW;
                    vacY = player_y + PlayerHeight / 2 - vacH / 2;
                }else if (aimDir == 2){
                    vacX = player_x + PlayerWidth / 2 - vacW / 2;
                    vacY = player_y - vacH;
                }else{
                    vacX = player_x + PlayerWidth / 2 - vacW / 2;
                    vacY = player_y + PlayerHeight;
                }

                flameSprite.setPosition(vacX, vacY);

                if (capturedCount < MAX_CAPTURE)
                {
                    for (int i = 0; i < NUM_ENEMIES; i++)
                    {
                        if (!enemyAlive[i]) continue;
                        if (capturedCount >= MAX_CAPTURE) break;

                        float ex = enemyX[i];
                        float ey = enemyY[i];
                        bool overlap =
                            ex < vacX + vacW &&
                            ex + CELL_SIZE > vacX &&
                            ey < vacY + vacH &&
                            ey + CELL_SIZE > vacY;

                        if (overlap)
                        {
                            enemyAlive[i] = false;
                            capturedType[capturedCount] = enemyType[i];
                            capturedCount++;
                            int baseCap = (enemyType[i] == 0) ? 50 : 75;
                            score += baseCap;
                        }
                    }
                }
            }

            // ---- Bullets movement + enemy hit ----
            for (int b = 0; b < MAX_BULLETS; ++b)
            {
                if (!bulletActive[b]) continue;
                float dx = 0.0f, dy = 0.0f;
                if      (bulletDir[b] == 0) dx =  bulletSpeed;
                else if (bulletDir[b] == 1) dx = -bulletSpeed;
                else if (bulletDir[b] == 2) dy = -bulletSpeed;
                else                        dy =  bulletSpeed;

                float newX = bulletX[b] + dx;
                float newY = bulletY[b] + dy;

                if (newX < 0 || newX > SCREEN_X || newY < 0 || newY > SCREEN_Y)
                {
                    bulletActive[b] = false;
                    continue;
                }

                int ccol = (int)(newX) / CELL_SIZE;
                int crow = (int)(newY) / CELL_SIZE;
                crow = clampInt(crow, 0, LEVEL_HEIGHT - 1);
                ccol = clampInt(ccol, 0, LEVEL_WIDTH  - 1);
                if (lvl[crow][ccol] == '#')
                {
                    bulletActive[b] = false;
                    continue;
                }

                bulletX[b] = newX;
                bulletY[b] = newY;

                for (int i = 0; i < NUM_ENEMIES; i++)
                {
                    if (!enemyAlive[i]) continue;
                    float ex = enemyX[i];
                    float ey = enemyY[i];
                    bool hit =
                        bulletX[b] > ex &&
                        bulletX[b] < ex + CELL_SIZE &&
                        bulletY[b] > ey &&
                        bulletY[b] < ey + CELL_SIZE;

                    if (hit)
                    {
                        enemyAlive[i] = false;
                        bulletActive[b] = false;
                        int baseCap = (enemyType[i] == 0) ? 50 : 75;
                        score += baseCap * 2; // shot kill bonus
                        break;
                    }
                }
            }

            // ---- Single Shot (Z) ----
            bool zNow = Keyboard::isKeyPressed(Keyboard::Z);
            if (zNow && !zHeld && capturedCount > 0)
            {
                capturedCount--;
                int type = capturedType[capturedCount]; // 0 ghost, 1 skeleton

                for (int b = 0; b < MAX_BULLETS; ++b)
                {
                    if (!bulletActive[b])
                    {
                        bulletActive[b] = true;
                        bulletDir[b]    = shotDir;
                        bulletType[b]   = type;
                        bulletX[b]      = player_x + PlayerWidth / 2;
                        bulletY[b]      = player_y + PlayerHeight / 2;
                        break;
                    }
                }
            }
            zHeld = zNow;

            // ---- Burst (X) ----
            bool xNow = Keyboard::isKeyPressed(Keyboard::X);
            if (xNow && !xHeld && capturedCount > 0)
            {
                int toShoot = capturedCount;
                if (toShoot > 3) toShoot = 3;

                for (int n = 0; n < toShoot; ++n)
                {
                    capturedCount--;
                    int type = capturedType[capturedCount];

                    for (int b = 0; b < MAX_BULLETS; ++b)
                    {
                        if (!bulletActive[b])
                        {
                            bulletActive[b] = true;
                            bulletDir[b]    = shotDir;
                            bulletType[b]   = type;
                            bulletX[b]      = player_x + PlayerWidth / 2;
                            bulletY[b]      = player_y + PlayerHeight / 2;
                            break;
                        }
                    }
                }
                // if more captured remain beyond 3, they stay stored
            }
            xHeld = xNow;

            // ----  release visuals ----
            for (int r = 0; r < MAX_RELEASE; r++)
            {
                if (releaseActive[r])
                {
                    releaseTimer[r]--;
                    if (releaseTimer[r] <= 0)
                        releaseActive[r] = false;
                }
            }

            // ---- level clear check ----
            bool anyAlive = false;
            for (int i = 0; i < NUM_ENEMIES; i++)
                if (enemyAlive[i]) anyAlive = true;
            if (!anyAlive)
                levelFinished = true;

            // ----scores and save to file at end ----
            if ((levelFinished || gameOver) && !bonusesApplied)
            {
                bonusesApplied = true;
                float elapsed = frameCount / 60.0;

                if (levelFinished)
                    score += 1000;
                if (!tookDamage)
                    score += 1500;
                if (elapsed <= 30.0f)       score += 2000;
                else if (elapsed <= 45.0f)  score += 1000;
                else if (elapsed <= 60.0f)  score += 500;

                // create levelscore file
              ofstream out("Data/levelscore.txt", ios::app);
          if (out)
         {
         // Only level name and score
         out << "LEVEL1 " << score << "\n";
         out.close();
        }

                cout << "FINAL RESULT\n";
                cout << "Score : "  << score   << "\n";
                cout << "Health : " << health  << "\n";
                cout << "Time : "   << elapsed << " seconds\n";
                cout << (gameOver ? "GAME OVER\n" : "LEVEL 1 CLEAR\n");
            }
        }

        // ================= DRAW =================
        if (!levelFinished && !gameOver)
        {
            window.clear();
            window.draw(bgSprite);

            for (int i = 0; i < LEVEL_HEIGHT; i++)
                for (int j = 0; j < LEVEL_WIDTH; j++)
                    if (lvl[i][j] == '#')
                    {
                        blockSprite.setPosition(j * CELL_SIZE, i * CELL_SIZE);
                        window.draw(blockSprite);
                    }

            for (int i = 0; i < NUM_ENEMIES; i++)
            {
                if (!enemyAlive[i]) continue;
                Sprite eSprite;
                if (enemyType[i] == 0)
                {
                    eSprite.setTexture(ghostTex);
                    eSprite.setScale(gScaleX, gScaleY);
                }
                else
                {
                    eSprite.setTexture(skeletonTex);
                    eSprite.setScale(sScaleX, sScaleY);
                }
                eSprite.setPosition(enemyX[i], enemyY[i]);
                window.draw(eSprite);
            }

            for (int r = 0; r < MAX_RELEASE; r++)
            {
                if (!releaseActive[r]) continue;
                Sprite rSprite;
                if (releaseType[r] == 0)
                {
                    rSprite.setTexture(ghostTex);
                    rSprite.setScale(gScaleX, gScaleY);
                }
                else
                {
                    rSprite.setTexture(skeletonTex);
                    rSprite.setScale(sScaleX, sScaleY);
                }
                rSprite.setPosition(releaseX[r], releaseY[r]);
                window.draw(rSprite);
            }

            if (Keyboard::isKeyPressed(Keyboard::Space))
                window.draw(flameSprite);

            // Draw bullets as small enemy sprites
            for (int b = 0; b < MAX_BULLETS; ++b)
            {
                if (!bulletActive[b]) continue;
                Sprite bulletSprite;
                if (bulletType[b] == 0) // ghost
                {
                    bulletSprite.setTexture(ghostTex);
                    bulletSprite.setScale(gScaleX * 0.8f, gScaleY * 0.8f);
                }
                else                     // skeleton or unknown
                {
                    bulletSprite.setTexture(skeletonTex);
                    bulletSprite.setScale(sScaleX * 0.8f, sScaleY * 0.8f);
                }
                bulletSprite.setPosition(bulletX[b], bulletY[b]);
                window.draw(bulletSprite);
            }

            if (!facingRight)
            {
                playerSprite.setScale(pScaleX, pScaleY);
                playerSprite.setPosition(player_x, player_y);
            }
            else
            {
                playerSprite.setScale(-pScaleX, pScaleY);
                playerSprite.setPosition(player_x + PlayerWidth, player_y);
            }
            window.draw(playerSprite);

            window.display();
        }
        else
        {
            // End screen with game over / level clear picture
            window.clear(Color::Black);

            if (gameOver && hasGameOver)
            {
                endSprite.setTexture(gameOverTex);
                Vector2u sz = gameOverTex.getSize();
                endSprite.setPosition((SCREEN_X - sz.x) / 2.f,
                                      (SCREEN_Y - sz.y) / 2.f);
                window.draw(endSprite);
            }
            else if (levelFinished && hasLevelClear)
            {
                endSprite.setTexture(levelClearTex);
                Vector2u sz = levelClearTex.getSize();
                endSprite.setPosition((SCREEN_X - sz.x) / 2.f,
                                      (SCREEN_Y - sz.y) / 2.f);
                window.draw(endSprite);
            }

            window.display();
        }
    }

    cout << "===== LEVEL 1 COMPLETED =====\n";
    cout << "Returning to MAIN_MENU\n";
    state = STATE_MAIN_MENU;
}

// ======================================================
// LEVEL 2 - PLACEHOLDER
// ======================================================
void runLevel2(RenderWindow& window, int& state, int chosenPlayer, int screenX, int screenY)
{
    srand((unsigned)time(NULL));

    // ---------- constants ----------
    const int CELL  = 48;
    const int W     = 16;
    const int H     = 14;

    const int SCR_X = screenX;
    const int SCR_Y = screenY;

    const float PLAYER_SPEED = 2.0;
    const float JUMP_V       = -7.0;
    const float GRAVITY      = 0.12;
    const float TERM_V       = 0.8;

    // ---------- map ----------
    const char* rows[H] = {
        "################",
        "#              #",
        "#              #",
        "####   ######  #",
        "#  ##          #",
        "##  ##  ###  ###",
        "#    ##        #",
        "###   ##  #### #",
        "#      ##      #",
        "####    ##  ####",
        "#        ##    #",
        "######    #### #",
        "#              #",
        "################"
    };

    char lvl[H][W];
    for(int i = 0; i < H; i++)
        for(int j = 0; j < W; j++)
            lvl[i][j] = (rows[i][j] == '#') ? '#' : ' ';

    // ---------- textures / sprites ----------
    Texture bgTex;         bgTex.loadFromFile("Data/bg.png");
    Texture blockTex;      blockTex.loadFromFile("Data/block1.png");
    Texture playerTex;
    if (chosenPlayer == 0)
        playerTex.loadFromFile("Data/yellow.png");
    else
        playerTex.loadFromFile("Data/green.png");

    Texture ghostTex;      ghostTex.loadFromFile("Data/ghost.png");
    Texture skeletonTex;   skeletonTex.loadFromFile("Data/skeleton.png");
    Texture invisibleTex;  invisibleTex.loadFromFile("Data/invisible.png");
    Texture chelnovTex;    chelnovTex.loadFromFile("Data/chelnov.png");
    Texture flamR; flamR.loadFromFile("Data/flame_right.png");
    Texture flamL; flamL.loadFromFile("Data/flame_left.png");
    Texture flamU; flamU.loadFromFile("Data/flame_up.png");
    Texture flamD; flamD.loadFromFile("Data/flame_down.png");
    Texture ballTex;       ballTex.loadFromFile("Data/ball.png");

    // end‑screen pictures
    Texture gameOverTex, levelClearTex;
    bool hasGameOver   = gameOverTex.loadFromFile("Data/gameover.png");
    bool hasLevelClear = levelClearTex.loadFromFile("Data/levelclear.png");
    Sprite endSprite;

    Sprite bg; bg.setTexture(bgTex);
    bg.setScale((float)SCR_X / bgTex.getSize().x,
                (float)SCR_Y / bgTex.getSize().y);

    Sprite tile; tile.setTexture(blockTex);
    tile.setScale((float)CELL / blockTex.getSize().x,
                  (float)CELL / blockTex.getSize().y);

    Sprite player; player.setTexture(playerTex);
    float pSX = (float)CELL / playerTex.getSize().x;
    float pSY = (float)CELL / playerTex.getSize().y;

    float gSX = (float)CELL / ghostTex.getSize().x;
    float gSY = (float)CELL / ghostTex.getSize().y;
    float sSX = (float)CELL / skeletonTex.getSize().x;
    float sSY = (float)CELL / skeletonTex.getSize().y;

    Sprite flame; int fW = (int)flamR.getSize().x, fH = (int)flamR.getSize().y;

    // ---------- music ----------
    Music music;
    if (music.openFromFile("Data/mus.ogg"))
    {
        music.setLoop(true);
        music.setVolume(60.f);
        music.play();
    }

    // ---------- player state ----------
    float px = 2 * CELL, py = 11 * CELL;
    float vy = 0.0;
    bool onGround = false, facingRight = true, upHeld = false;
    int  aimDir = 0;
    int  health = 3, score = 0;
    int  frames = 0;
    bool gameOver = false, levelClear = false;
    bool resultPrinted = false;   // print/write once

    // ---------- capture / release ----------
    const int CAP_MAX = 5;
    int  capType[CAP_MAX], capCount = 0;
    const int MAX_BUL = 15;
    bool  bulA[MAX_BUL];
    float bulX[MAX_BUL], bulY[MAX_BUL];
    int   bulDir[MAX_BUL], bulType[MAX_BUL];
    for(int i = 0; i < MAX_BUL; i++)
    {
        bulA[i] = false;
        bulType[i] = -1;
    }
    const float BUL_SPEED = 3.0;
    bool xHeld = false, zHeld = false;

    // ---------- enemies ----------
    const int EN = 20;
    float ex[EN], ey[EN];
    int   et[EN];      // 0=Ghost,1=Skeleton,2=Invisible,3=Chelnov
    int   edir[EN];    // -1/+1
    bool  alive[EN];
    int   gPause[EN];
    int   invT[EN];
    bool  invVis[EN];
    int   cheShotT[EN];
    int   cheNoCap[EN];

    int idx = 0;

    // Ghosts on row 2
    et[idx]=0; edir[idx]= 1; ex[idx]= 1*CELL; ey[idx]=2*CELL;  gPause[idx]=0; invT[idx]=0; invVis[idx]=true; cheShotT[idx]=0; cheNoCap[idx]=0; idx++;
    et[idx]=0; edir[idx]=-1; ex[idx]= 3*CELL; ey[idx]=2*CELL;  gPause[idx]=0; invT[idx]=0; invVis[idx]=true; cheShotT[idx]=0; cheNoCap[idx]=0; idx++;
    et[idx]=0; edir[idx]= 1; ex[idx]= 8*CELL; ey[idx]=2*CELL;  gPause[idx]=0; invT[idx]=0; invVis[idx]=true; cheShotT[idx]=0; cheNoCap[idx]=0; idx++;
    et[idx]=0; edir[idx]=-1; ex[idx]=11*CELL; ey[idx]=2*CELL;  gPause[idx]=0; invT[idx]=0; invVis[idx]=true; cheShotT[idx]=0; cheNoCap[idx]=0; idx++;

    // Skeletons
    et[idx]=1; edir[idx]= 1; ex[idx]= 8*CELL; ey[idx]=4*CELL;  gPause[idx]=0; invT[idx]=0; invVis[idx]=true; cheShotT[idx]=0; cheNoCap[idx]=0; idx++;
    et[idx]=1; edir[idx]=-1; ex[idx]=10*CELL; ey[idx]=4*CELL;  gPause[idx]=0; invT[idx]=0; invVis[idx]=true; cheShotT[idx]=0; cheNoCap[idx]=0; idx++;
    et[idx]=1; edir[idx]= 1; ex[idx]=13*CELL; ey[idx]=4*CELL;  gPause[idx]=0; invT[idx]=0; invVis[idx]=true; cheShotT[idx]=0; cheNoCap[idx]=0; idx++;

    et[idx]=1; edir[idx]= 1; ex[idx]= 1*CELL; ey[idx]=6*CELL;  gPause[idx]=0; invT[idx]=0; invVis[idx]=true; cheShotT[idx]=0; cheNoCap[idx]=0; idx++;
    et[idx]=1; edir[idx]=-1; ex[idx]=11*CELL; ey[idx]=6*CELL;  gPause[idx]=0; invT[idx]=0; invVis[idx]=true; cheShotT[idx]=0; cheNoCap[idx]=0; idx++;

    et[idx]=1; edir[idx]= 1; ex[idx]= 2*CELL; ey[idx]=8*CELL;  gPause[idx]=0; invT[idx]=0; invVis[idx]=true; cheShotT[idx]=0; cheNoCap[idx]=0; idx++;
    et[idx]=1; edir[idx]=-1; ex[idx]=13*CELL; ey[idx]=8*CELL;  gPause[idx]=0; invT[idx]=0; invVis[idx]=true; cheShotT[idx]=0; cheNoCap[idx]=0; idx++;

    et[idx]=1; edir[idx]= 1; ex[idx]= 2*CELL; ey[idx]=10*CELL; gPause[idx]=0; invT[idx]=0; invVis[idx]=true; cheShotT[idx]=0; cheNoCap[idx]=0; idx++;
    et[idx]=1; edir[idx]=-1; ex[idx]= 5*CELL; ey[idx]=12*CELL; gPause[idx]=0; invT[idx]=0; invVis[idx]=true; cheShotT[idx]=0; cheNoCap[idx]=0; idx++;

    // Invisible Men
    et[idx]=2; edir[idx]= 1; ex[idx]=12*CELL; ey[idx]=6*CELL;
    gPause[idx]=0; invT[idx]=240+rand()%180; invVis[idx]=true; cheShotT[idx]=0; cheNoCap[idx]=0; idx++;
    et[idx]=2; edir[idx]=-1; ex[idx]=12*CELL; ey[idx]=8*CELL;
    gPause[idx]=0; invT[idx]=240+rand()%180; invVis[idx]=true; cheShotT[idx]=0; cheNoCap[idx]=0; idx++;
    et[idx]=2; edir[idx]= 1; ex[idx]=11*CELL; ey[idx]=10*CELL;
    gPause[idx]=0; invT[idx]=240+rand()%180; invVis[idx]=true; cheShotT[idx]=0; cheNoCap[idx]=0; idx++;

    // Chelnov
    et[idx]=3; edir[idx]= 1; ex[idx]= 2*CELL; ey[idx]=8*CELL;   gPause[idx]=0; invT[idx]=0; invVis[idx]=true; cheShotT[idx]=0; cheNoCap[idx]=0; idx++;
    et[idx]=3; edir[idx]=-1; ex[idx]=13*CELL; ey[idx]=8*CELL;   gPause[idx]=0; invT[idx]=0; invVis[idx]=true; cheShotT[idx]=0; cheNoCap[idx]=0; idx++;
    et[idx]=3; edir[idx]= 1; ex[idx]= 3*CELL; ey[idx]=10*CELL;  gPause[idx]=0; invT[idx]=0; invVis[idx]=true; cheShotT[idx]=0; cheNoCap[idx]=0; idx++;
    et[idx]=3; edir[idx]=-1; ex[idx]=10*CELL; ey[idx]=12*CELL;  gPause[idx]=0; invT[idx]=0; invVis[idx]=true; cheShotT[idx]=0; cheNoCap[idx]=0; idx++;

    for(int i = 0; i < EN; i++) alive[i] = true;

    // Chelnov bullets
    const int MAX_EB = 10;
    bool  eA[MAX_EB];
    float eX[MAX_EB], eY[MAX_EB], eDX[MAX_EB], eDY[MAX_EB];
    for(int i = 0; i < MAX_EB; i++) eA[i] = false;
    const float EB_SPEED = 2.0f;

    cout << "===== LEVEL 2 STARTED =====\n";
    cout << "Player: " << (chosenPlayer == 0 ? "YELLOW" : "GREEN") << "\n";

    while(window.isOpen() && state == STATE_PLAY_LEVEL2)
    {
        Event ev;
        while(window.pollEvent(ev))
        {
            if(ev.type == Event::Closed)
            {
                window.close();
                return;
            }
        }

        // ESC = back to main menu (do NOT close whole program)
        if(Keyboard::isKeyPressed(Keyboard::Escape))
        {
            cout << "Returning to MAIN_MENU from Level 2\n";
            state = STATE_MAIN_MENU;
            return;
        }

        if(!gameOver && !levelClear) frames++;

        // ===== PLAYER =====
        if(!gameOver && !levelClear)
        {
            float moveX = 0.f;
            bool L = Keyboard::isKeyPressed(Keyboard::Left);
            bool R = Keyboard::isKeyPressed(Keyboard::Right);
            if(L && !R) { moveX -= PLAYER_SPEED; facingRight = false; }
            else if(R && !L) { moveX += PLAYER_SPEED; facingRight = true; }

            float npx = px + moveX;
            int mrow = (int)(py + CELL/2) / CELL;
            int lcol = (int)npx / CELL;
            int rcol = (int)(npx + CELL - 1) / CELL;
            mrow = clampInt(mrow, 0, H-1);
            lcol = clampInt(lcol, 0, W-1);
            rcol = clampInt(rcol, 0, W-1);
            bool block = (lvl[mrow][lcol]=='#' || lvl[mrow][rcol]=='#');
            if(!block) px = npx;

            aimDir = facingRight ? 0 : 1;
            if      (Keyboard::isKeyPressed(Keyboard::W)) aimDir = 2;
            else if (Keyboard::isKeyPressed(Keyboard::S)) aimDir = 3;
            else if (Keyboard::isKeyPressed(Keyboard::A)) aimDir = 1;
            else if (Keyboard::isKeyPressed(Keyboard::D)) aimDir = 0;

            bool canJ = false;
            int frow = (int)(py + CELL + 1) / CELL;
            int fl = (int)px / CELL, fr = (int)(px + CELL - 1) / CELL, fm = (int)(px + CELL/2) / CELL;
            frow = clampInt(frow, 0, H-1);
            fl   = clampInt(fl,   0, W-1);
            fr   = clampInt(fr,   0, W-1);
            fm   = clampInt(fm,   0, W-1);
            if(lvl[frow][fl]=='#' || lvl[frow][fm]=='#' || lvl[frow][fr]=='#')
                canJ = true;

            bool upNow = Keyboard::isKeyPressed(Keyboard::Up);
            if(upNow && !upHeld && canJ)
            {
                int hrow = (int)(py - 1) / CELL;
                int hl = (int)px / CELL, hr = (int)(px + CELL - 1) / CELL;
                hrow = clampInt(hrow, 0, H-1);
                hl   = clampInt(hl,   0, W-1);
                hr   = clampInt(hr,   0, W-1);
                bool ceil = (lvl[hrow][hl]=='#' || lvl[hrow][hr]=='#');
                if(!ceil)
                {
                    vy = JUMP_V;
                    onGround = false;
                }
            }
            upHeld = upNow;

            float npy = py + vy;
            if(vy < 0)
            {
                int trow = (int)npy / CELL;
                int tl = (int)px / CELL, tr = (int)(px + CELL - 1) / CELL;
                trow = clampInt(trow, 0, H-1);
                tl   = clampInt(tl,   0, W-1);
                tr   = clampInt(tr,   0, W-1);
                bool ceil = (lvl[trow][tl]=='#' || lvl[trow][tr]=='#');
                if(ceil)
                {
                    py = (trow + 1) * CELL;
                    vy = 0;
                }
                else
                {
                    py = npy;
                    vy += GRAVITY;
                }
            }
            else
            {
                int brow = (int)(npy + CELL) / CELL;
                int blc = (int)px / CELL, brc = (int)(px + CELL - 1) / CELL, bmc = (int)(px + CELL/2) / CELL;
                brow = clampInt(brow, 0, H-1);
                blc  = clampInt(blc,  0, W-1);
                brc  = clampInt(brc,  0, W-1);
                bmc  = clampInt(bmc,  0, W-1);
                bool floor = (lvl[brow][blc]=='#' || lvl[brow][bmc]=='#' || lvl[brow][brc]=='#');
                if(floor)
                {
                    onGround = true;
                    vy = 0;
                    py = brow * CELL - CELL - 1;
                }
                else
                {
                    py = npy;
                    onGround = false;
                    vy += GRAVITY;
                    if(vy > TERM_V) vy = TERM_V;
                }
            }
        }

        // ===== ENEMIES =====
        if(!gameOver && !levelClear)
        {
            for(int i = 0; i < EN; i++)
            {
                if(!alive[i]) continue;

                float sp;
                if      (et[i]==0) sp = 0.35;
                else if (et[i]==1) sp = 0.4;
                else if (et[i]==2) sp = 0.45;
                else               sp = 0.35;

                // Ghost
                if(et[i]==0)
                {
                    if(gPause[i]>0){ gPause[i]--; sp=0; }
                    else{
                        if(rand()%240==0) gPause[i]=30;
                        if(rand()%300==0) edir[i] = -edir[i];
                    }
                }

                // Invisible Man
                if(et[i]==2)
                {
                    invT[i]--;
                    if(invT[i] <= 0)
                    {
                        invVis[i] = !invVis[i];
                        if(invVis[i])
                        {
                            invT[i] = 240 + rand()%180;
                        }
                        else
                        {
                            invT[i] = 60 + rand()%60;
                            for(int t=0; t<40; t++)
                            {
                                int r = 2 + rand()%(H-4);
                                int c = 1 + rand()%(W-2);
                                if(lvl[r][c] != ' ') continue;
                                int br = clampInt(r+1,0,H-1);
                                if(lvl[br][c] != '#') continue;
                                ex[i] = c*CELL; ey[i] = r*CELL; break;
                            }
                        }
                    }
                }

                float nex = ex[i] + edir[i]*sp;
                int row   = (int)(ey[i] + CELL/2) / CELL;
                int front = (int)((edir[i]>0)?(nex+CELL-1):nex) / CELL;
                row   = clampInt(row,0,H-1);
                front = clampInt(front,0,W-1);

                char frontTile = lvl[row][front];
                int  belowRow  = clampInt(row+1,0,H-1);
                char belowFront= lvl[belowRow][front];

                if(frontTile=='#' || belowFront!='#')
                    edir[i] = -edir[i];
                else
                    ex[i] = nex;
            }

            // Chelnov shooting
            for(int i=0; i<EN; i++)
            {
                if(!alive[i] || et[i]!=3) continue;
                cheShotT[i]++;
                if(cheShotT[i] >= 240)
                {
                    cheShotT[i] = 0;
                    cheNoCap[i] = 60;
                    for(int b=0; b<MAX_EB; b++) if(!eA[b])
                    {
                        eA[b] = true;
                        eX[b] = ex[i] + CELL/2; eY[b] = ey[i] + CELL/2;
                        eDX[b] = (edir[i]>0) ? EB_SPEED : -EB_SPEED;
                        eDY[b] = 0.0f;
                        break;
                    }
                }
                if(cheNoCap[i] > 0) cheNoCap[i]--;
            }
        }

        // Chelnov bullets
        if(!gameOver && !levelClear)
        {
            for(int b=0; b<MAX_EB; b++)
            {
                if(!eA[b]) continue;
                float nx = eX[b] + eDX[b], ny = eY[b] + eDY[b];
                if(nx<0 || nx>SCR_X || ny<0 || ny>SCR_Y)
                { eA[b]=false; continue; }
                int c=(int)nx/CELL, r=(int)ny/CELL;
                c = clampInt(c,0,W-1); r = clampInt(r,0,H-1);
                if(lvl[r][c]=='#'){ eA[b]=false; continue; }
                eX[b]=nx; eY[b]=ny;
                if(nx>px && nx<px+CELL && ny>py && ny<py+CELL)
                {
                    health--; eA[b]=false;
                    if(health<=0) gameOver=true;
                }
            }
        }

        // player vs enemy touch
        if(!gameOver && !levelClear)
        {
            for(int i=0; i<EN; i++)
            {
                if(!alive[i]) continue;
                if(et[i]==2 && !invVis[i]) continue;
                bool hit = (px<ex[i]+CELL && px+CELL>ex[i] &&
                            py<ey[i]+CELL && py+CELL>ey[i]);
                if(hit)
                {
                    health--;
                    if(health<=0) gameOver=true;
                    break;
                }
            }
        }

        // ===== VACUUM CAPTURE & RELEASE =====
        if(!gameOver && !levelClear)
        {
            bool sucking = Keyboard::isKeyPressed(Keyboard::Space);
            float vacX = 0, vacY = 0; int vW = fW, vH = fH;
            if(sucking)
            {
                if      (aimDir==0) flame.setTexture(flamR);
                else if (aimDir==1) flame.setTexture(flamL);
                else if (aimDir==2) flame.setTexture(flamU);
                else                flame.setTexture(flamD);

                if(aimDir==0){ vacX=px+CELL;          vacY=py+CELL/2 - vH/2; }
                else if(aimDir==1){ vacX=px-vW;       vacY=py+CELL/2 - vH/2; }
                else if(aimDir==2){ vacX=px+CELL/2-vW/2; vacY=py-vH; }
                else { vacX=px+CELL/2-vW/2; vacY=py+CELL; }
                flame.setPosition(vacX,vacY);

                if(capCount < CAP_MAX)
                {
                    for(int i=0; i<EN; i++)
                    {
                        if(!alive[i]) continue;
                        if(et[i]==2 && !invVis[i]) continue;
                        if(et[i]==3 && cheNoCap[i]>0) continue;
                        bool over = (ex[i]<vacX+vW && ex[i]+CELL>vacX &&
                                     ey[i]<vacY+vH && ey[i]+CELL>vacY);
                        if(over)
                        {
                            alive[i] = false;
                            capType[capCount++] = et[i];
                            if(et[i]==0)      score+=50;
                            else if(et[i]==1) score+=75;
                            else if(et[i]==2) score+=150;
                            else              score+=200;
                            if(capCount>=CAP_MAX) break;
                        }
                    }
                }
            }

            // Space + X : shoot ONE stored enemy
            bool xNow = Keyboard::isKeyPressed(Keyboard::X);
            if(sucking && xNow && !xHeld && capCount>0)
            {
                capCount--;
                int t = capType[capCount];
                for(int b=0; b<MAX_BUL; b++) if(!bulA[b])
                {
                    bulA[b]  = true;
                    bulType[b] = t;
                    bulDir[b]  = aimDir;
                    bulX[b]    = px + CELL/2;
                    bulY[b]    = py + CELL/2;
                    break;
                }
            }
            xHeld = xNow;

            // Space + Z : shoot ALL stored enemies
            bool zNow = Keyboard::isKeyPressed(Keyboard::Z);
            if(sucking && zNow && !zHeld && capCount>0)
            {
                while(capCount>0)
                {
                    capCount--;
                    int t = capType[capCount];
                    for(int b=0; b<MAX_BUL; b++) if(!bulA[b])
                    {
                        bulA[b]  = true;
                        bulType[b] = t;
                        bulDir[b]  = aimDir;
                        bulX[b]    = px + CELL/2;
                        bulY[b]    = py + CELL/2;
                        break;
                    }
                }
            }
            zHeld = zNow;
        }

        // bullets from released enemies
        if(!gameOver && !levelClear)
        {
            for(int b=0; b<MAX_BUL; b++)
            {
                if(!bulA[b]) continue;
                float dx=0, dy=0;
                if      (bulDir[b]==0) dx=BUL_SPEED;
                else if (bulDir[b]==1) dx=-BUL_SPEED;
                else if (bulDir[b]==2) dy=-BUL_SPEED;
                else                   dy=BUL_SPEED;
                float nx=bulX[b]+dx, ny=bulY[b]+dy;
                if(nx<0||nx>SCR_X||ny<0||ny>SCR_Y)
                { bulA[b]=false; continue; }
                int c=(int)nx/CELL, r=(int)ny/CELL;
                c=clampInt(c,0,W-1); r=clampInt(r,0,H-1);
                if(lvl[r][c]=='#'){ bulA[b]=false; continue; }
                bulX[b]=nx; bulY[b]=ny;

                for(int i=0; i<EN; i++)
                {
                    if(!alive[i]) continue;
                    bool hit = nx>ex[i] && nx<ex[i]+CELL &&
                               ny>ey[i] && ny<ey[i]+CELL;
                    if(hit)
                    {
                        alive[i]=false; bulA[b]=false;
                        if(et[i]==0)      score+=100;
                        else if(et[i]==1) score+=150;
                        else if(et[i]==2) score+=300;
                        else              score+=400;
                        break;
                    }
                }
            }
        }

        // level clear
        if(!gameOver && !levelClear)
        {
            bool any=false;
            for(int i=0; i<EN; i++) if(alive[i]) any=true;
            if(!any) levelClear=true;
        }

        // ===== DRAW OR RESULT SCREEN =====
        if(!gameOver && !levelClear)
        {
            window.clear();
            window.draw(bg);

            for(int i=0; i<H; i++)
                for(int j=0; j<W; j++)
                    if(lvl[i][j]=='#')
                    {
                        tile.setPosition(j*CELL,i*CELL);
                        window.draw(tile);
                    }

            for(int i=0; i<EN; i++)
            {
                if(!alive[i]) continue;
                if(et[i]==2 && !invVis[i]) continue;
                Sprite e;
                if(et[i]==0){ e.setTexture(ghostTex);      e.setScale(gSX,gSY); }
                else if(et[i]==1){ e.setTexture(skeletonTex); e.setScale(sSX,sSY); }
                else if(et[i]==2){ e.setTexture(invisibleTex);e.setScale(gSX,gSY); }
                else             { e.setTexture(chelnovTex);  e.setScale(sSX,sSY); }
                e.setPosition(ex[i],ey[i]);
                window.draw(e);
            }

            for(int b=0; b<MAX_EB; b++)
                if(eA[b])
                {
                    Sprite s; s.setTexture(ballTex);
                    s.setPosition(eX[b],eY[b]); window.draw(s);
                }

            for(int b=0; b<MAX_BUL; b++)
                if(bulA[b])
                {
                    Sprite p;
                    if(bulType[b]==0){
                        p.setTexture(ghostTex);
                        p.setScale(gSX*0.8f,gSY*0.8f);
                    }
                    else if(bulType[b]==1){
                        p.setTexture(skeletonTex);
                        p.setScale(sSX*0.8f,sSY*0.8f);
                    }
                    else if(bulType[b]==2){
                        p.setTexture(invisibleTex);
                        p.setScale(gSX*0.8f,gSY*0.8f);
                    }
                    else{
                        p.setTexture(chelnovTex);
                        p.setScale(sSX*0.8f,sSY*0.8f);
                    }
                    p.setPosition(bulX[b],bulY[b]);
                    window.draw(p);
                }

            if(Keyboard::isKeyPressed(Keyboard::Space))
                window.draw(flame);

            if(!facingRight)
            {
                player.setScale(pSX,pSY);
                player.setPosition(px,py);
            }
            else
            {
                player.setScale(-pSX,pSY);
                player.setPosition(px+CELL,py);
            }
            window.draw(player);

            window.display();
        }
        else
        {
            window.clear(Color::Black);

            if(levelClear && hasLevelClear)
            {
                endSprite.setTexture(levelClearTex);
                Vector2u sz = levelClearTex.getSize();
                endSprite.setPosition((SCR_X - sz.x)/2.f,
                                      (SCR_Y - sz.y)/2.f);
                window.draw(endSprite);
            }
            else if(gameOver && hasGameOver)
            {
                endSprite.setTexture(gameOverTex);
                Vector2u sz = gameOverTex.getSize();
                endSprite.setPosition((SCR_X - sz.x)/2.f,
                                      (SCR_Y - sz.y)/2.f);
                window.draw(endSprite);
            }

            window.display();

            if(!resultPrinted)
            {
                float t = frames / 60.0f;

                ofstream out("Data/levelscore.txt", ios::app);
                if(out){
                 out << "LEVEL2 " << score << "\n";
                out.close();
                }
 

                cout << "\n===== LEVEL 2 RESULT =====\n";
                cout << (levelClear ? "LEVEL CLEARED\n" : "GAME OVER\n");
                cout << "Score  : " << score  << "\n";
                cout << "Health : " << health << "\n";
                cout << "Time   : " << t      << " seconds\n";
                cout << "==========================\n";
                cout << "Press ESC to return to menu.\n";
                resultPrinted = true;
            }
        }
    }

    cout << "===== LEVEL 2 COMPLETED =====\n";
    cout << "Returning to MAIN_MENU\n";
    state = STATE_MAIN_MENU;
}


// ======================================================
// LEVEL 3 - PLACEHOLDER
// ======================================================
void runLevel3(RenderWindow& window, int& state, int chosenPlayer, int screenX, int screenY)
{
    const int CELL = 48;
    const int W = 16;  // 16x16 grid ✓
    const int H = 16;
    const int SCR_X = screenX;
    const int SCR_Y = screenY;

    // === BEST 16x16 GRID - Perfect player + octopus flow ===
    const char* rows[H] = {
        "################",  // 0: Top
        "#..............#",  // 1: Player spawn ✓
        "#....##....##..#",  // 2: Octopus paths
        "#..............#",  // 3: Open corridor ✓
        "##....##....##.#",  // 4: Maze zones
        "#..............#",  // 5: Main path ✓
        "#..##......##..#",  // 6: Cross paths
        "#..............#",  // 7: Central arena ✓
        "#..##......##..#",  // 8: Octopus highways
        "#..............#",  // 9: Open movement ✓
        "##....##....##.#",  // 10: Maze return
        "#..............#",  // 11: Safe zones ✓
        "#....##....##..#",  // 12: Final paths
        "#..............#",  // 13: Player return
        "################",  // 14: Bottom
        "################"   // 15: Border
    };
    
    char lvl[H][W];
    for (int i = 0; i < H; ++i) for (int j = 0; j < W; ++j)
        lvl[i][j] = (rows[i][j] == '#') ? '#' : ' ';

    // Graphics
    Texture bgTex; bgTex.loadFromFile("Data/bg.png");
    Texture blockTex; blockTex.loadFromFile("Data/block1.png");
    Texture minionTex; minionTex.loadFromFile("Data/min1.png");
    Texture flamR; flamR.loadFromFile("Data/flame_right.png");
    Texture flamL; flamL.loadFromFile("Data/flame_left.png");
    Texture flamU; flamU.loadFromFile("Data/flame_up.png");
    Texture flamD; flamD.loadFromFile("Data/flame_down.png");

    Texture playerTex;
    if (chosenPlayer == 0) playerTex.loadFromFile("Data/yellow.png");
    else playerTex.loadFromFile("Data/green.png");

    Sprite bg(bgTex); bg.setScale((float)SCR_X/bgTex.getSize().x, (float)SCR_Y/bgTex.getSize().y);
    Sprite block(blockTex); block.setScale((float)CELL/blockTex.getSize().x, (float)CELL/blockTex.getSize().y);
    Sprite player(playerTex);
    Sprite flame;
    
    float pScaleX = (float)CELL/playerTex.getSize().x;
    float pScaleY = (float)CELL/playerTex.getSize().y;
    float fScale = 1.8f;
    int fW = flamR.getSize().x;
    int fH = flamR.getSize().y;

    // Player setup
    float px = 1*CELL, py = 1*CELL;
    float vx = 0, vy = 0;
    bool facingRight = true;
    bool upHeld = false;
    int captured = 0, score = 0;
    bool levelDone = false;
    int aimDir = 0;  // 0=right, 1=left, 2=up, 3=down

    // Octopuses
    const int MAX_OCTOPUS = 12;
    float octopusX[MAX_OCTOPUS], octopusY[MAX_OCTOPUS];
    bool octopusAlive[MAX_OCTOPUS];
    float octopusVX[MAX_OCTOPUS];

    srand(time(0));
    for (int i = 0; i < MAX_OCTOPUS; i++) {
        octopusAlive[i] = true;
        octopusVX[i] = (rand() % 2) ? 0.25f : -0.25f;
        int rx = 2 + rand() % 13;
        int ry = 3 + rand() % 10;
        octopusX[i] = rx * CELL;
        octopusY[i] = ry * CELL;
    }

    // Main loop
    while (window.isOpen() && state == STATE_PLAY_LEVEL3) {
        Event ev; while (window.pollEvent(ev))
            if (ev.type == Event::Closed) { window.close(); return; }
        
        if (Keyboard::isKeyPressed(Keyboard::Escape)) { 
            state = STATE_MAIN_MENU; return; 
        }

        // Win check
        int aliveCount = 0;
        for (int i = 0; i < MAX_OCTOPUS; i++) if (octopusAlive[i]) aliveCount++;
        if (aliveCount == 0) {
            levelDone = true;
            score += 3000;
            ofstream out("Data/levelscore.txt", ios::app);
            if (out) out << "LEVEL3 " << score << "\n";
            state = STATE_MAIN_MENU;
            return;
        }

        // === PERFECT AIM DIRECTION ===
        aimDir = facingRight ? 0 : 1;  // Default to facing direction
        if (Keyboard::isKeyPressed(Keyboard::W)) aimDir = 2;  // Up
        if (Keyboard::isKeyPressed(Keyboard::S)) aimDir = 3;  // Down

        // === PLAYER MOVEMENT ===
        vx = 0;
        if (Keyboard::isKeyPressed(Keyboard::Left)) { vx = -2.0f; facingRight = false; }
        if (Keyboard::isKeyPressed(Keyboard::Right)) { vx = 2.0f; facingRight = true; }

        // Horizontal collision
        float newPX = px + vx;
        int pRow = clampInt((int)(py + CELL/2) / CELL, 0, H-1);
        int pLeft = clampInt((int)newPX / CELL, 0, W-1);
        int pRight = clampInt((int)(newPX + CELL-1) / CELL, 0, W-1);
        if (lvl[pRow][pLeft] != '#' && lvl[pRow][pRight] != '#') px = newPX;

        // Jump + gravity
        bool onGround = false;
        int groundRow = clampInt((int)(py + CELL + 3) / CELL, 0, H-1);
        int groundCol = clampInt((int)(px + CELL/2) / CELL, 0, W-1);
        if (lvl[groundRow][groundCol] == '#') onGround = true;

        bool jumpNow = Keyboard::isKeyPressed(Keyboard::Up);
        if (jumpNow && !upHeld && onGround) vy = -7.0f;
        upHeld = jumpNow;

        vy += 0.22f;
        if (vy > 1.0f) vy = 1.0f;

        // Vertical collision
        float newPY = py + vy;
        if (vy < 0) {
            int headRow = clampInt((int)newPY / CELL, 0, H-1);
            int headLeft = clampInt((int)px / CELL, 0, W-1);
            int headRight = clampInt((int)(px + CELL-1) / CELL, 0, W-1);
            if (lvl[headRow][headLeft] == '#' || lvl[headRow][headRight] == '#') {
                py = (headRow + 1) * CELL;
                vy = 0;
            } else {
                py = newPY;
            }
        } else {
            int footRow = clampInt((int)(newPY + CELL) / CELL, 0, H-1);
            int footLeft = clampInt((int)px / CELL, 0, W-1);
            int footRight = clampInt((int)(px + CELL-1) / CELL, 0, W-1);
            if (lvl[footRow][footLeft] == '#' || lvl[footRow][footRight] == '#') {
                py = footRow * CELL - CELL;
                vy = 0;
            } else {
                py = newPY;
            }
        }

        // === PERFECT DIRECTIONAL FLAMES ===
        bool sucking = Keyboard::isKeyPressed(Keyboard::Space);
        float flameX = 0, flameY = 0;
        if (sucking && captured < 6) {
            if (aimDir == 0) {  // RIGHT
                flame.setTexture(flamR);
                flameX = px + CELL;
                flameY = py + CELL/2 - fH/2;
            } else if (aimDir == 1) {  // LEFT
                flame.setTexture(flamL);
                flameX = px - fW;
                flameY = py + CELL/2 - fH/2;
            } else if (aimDir == 2) {  // UP
                flame.setTexture(flamU);
                flameX = px + CELL/2 - fW/2;
                flameY = py - fH;
            } else {  // DOWN
                flame.setTexture(flamD);
                flameX = px + CELL/2 - fW/2;
                flameY = py + CELL;
            }
            
            flame.setPosition(flameX, flameY);
            flame.setScale(fScale, fScale);

            // Capture logic (4-cell range)
            for (int i = 0; i < MAX_OCTOPUS; i++) {
                if (!octopusAlive[i]) continue;
                float dx = abs(px - octopusX[i]);
                float dy = abs(py - octopusY[i]);
                if (dx < 4*CELL && dy < 4*CELL) {
                    octopusAlive[i] = false;
                    captured++;
                    score += 200;
                    break;
                }
            }
        }

        // === OCTOPUS MOVEMENT ===
        for (int i = 0; i < MAX_OCTOPUS; i++) {
            if (!octopusAlive[i]) continue;
            
            float newOX = octopusX[i] + octopusVX[i];
            int oRow = clampInt((int)(octopusY[i] + CELL/2) / CELL, 0, H-1);
            int oCol = clampInt((int)newOX / CELL, 0, W-1);
            
            if (lvl[oRow][oCol] == '#') {
                octopusVX[i] = -octopusVX[i];
            } else {
                octopusX[i] = newOX;
            }
        }

        // === DRAW ===
        window.clear();
        window.draw(bg);

        // Perfect 16x16 grid
        for (int i = 0; i < H; i++) for (int j = 0; j < W; j++)
            if (lvl[i][j] == '#') {
                block.setPosition(j*CELL, i*CELL);
                window.draw(block);
            }

        // Octopuses
        for (int i = 0; i < MAX_OCTOPUS; i++) {
            if (octopusAlive[i]) {
                Sprite octo(minionTex);
                octo.setScale(0.7f, 0.7f);
                octo.setPosition(octopusX[i], octopusY[i]);
                window.draw(octo);
            }
        }

        // Perfect directional flames
        if (sucking) window.draw(flame);

        // Player
        player.setScale(facingRight ? -pScaleX : pScaleX, pScaleY);
        player.setPosition(facingRight ? px + CELL : px, py);
        window.draw(player);

        window.display();
    }
}










