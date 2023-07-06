#include "minesweeper.hpp"
#include "assets.hpp"
#include <cstring>

using namespace blit;

#define SCREEN_HEIGHT 240
#define SCREEN_WEIGHT 320
#define TILE_SIZE 24

#define MAP_SIZE 8
#define BOMBS_NUMBER 10
#define SHOW_FPS

struct tiles {
    Rect CLOSED = Rect(0, 0, 3, 3);

    Rect EMPTY = Rect(6, 0, 3, 3);
    Rect ONE   = Rect(12, 0, 3, 3);
    Rect TWO   = Rect(15, 0, 3, 3);
    Rect THREE = Rect(18, 0, 3, 3);
    Rect FOUR  = Rect(21, 0, 3, 3);
    Rect FIVE  = Rect(24, 0, 3, 3);
    Rect SIX   = Rect(27, 0, 3, 3);
    Rect SEVEN = Rect(30, 0, 3, 3);
    Rect EIGHT = Rect(33, 0, 3, 3);

    Rect FLAG  = Rect(3, 0, 3, 3);
    Rect QUESTION = Rect(9, 0, 3, 3);
    Rect BOMB = Rect(36, 0, 3, 3);
    Rect BOMB_RED = Rect(39, 0, 3, 3);
};

tiles GAME_TILES;

GameManager GM;

void clearScreen(){
    screen.clear();
    screen.pen = Pen(0, 0, 0);
    screen.rectangle(Rect(0, 0, SCREEN_WEIGHT, SCREEN_HEIGHT));
}

void Tile::bKeyHandler() {
    if (isOpened){
        return; 
    }
    if (! isFlagged && ! isQuestionMarked){
        isFlagged = true;
        return;
    }
    if (isFlagged && ! isQuestionMarked){
        isFlagged = false;
        isQuestionMarked = true;
        return;
    }
    if (! isFlagged && isQuestionMarked) {
        isQuestionMarked = false;
        return;
    }
}

void GameManager::newGame(){
    GAME = Minesweeper();
    inGame = true;
}

void GameManager::render(){
    if (inGame || inGameOver || inWin){
        GAME.render();
    }
    if (inGameOver){
        screen.pen = Pen(0, 255, 0);
        screen.text("GAME OVER", font, Point(120, 120));
    }
    if (inWin){
        screen.pen = Pen(0, 255, 0);
        screen.text("WIN!!!", font, Point(120, 120));
    }
}

void GameManager::update(){
    if (inGameOver){
        if (buttons.released & Button::B){
            inGameOver = false;
            newGame();
        }
        return;
    }
    if (inWin){
        if (buttons.released & Button::B){
            inWin = false;
            newGame();
        }
        return;
    }
    if (inGame){
        GAME.update();
        if (GAME.gameOver){
            inGameOver = true;
        }
        if (GAME.win){
            inWin = true;
        }
        return;
    }
    
}

void Minesweeper::checkTile(Point location){
    int bombsCounter = 0;

    if (location.x < 1 || location.y < 1 || location.x > MAP_SIZE || location.y > MAP_SIZE){
        return;
    }
    if (MAP[location.y][location.x].isOpened || MAP[location.y][location.x].isFlagged 
            || MAP[location.y][location.x].isQuestionMarked){
        return;
    }
    if (MAP[location.y][location.x].isBomb){
        MAP[location.y][location.x].isOpened = true;
        return;
    }
    if (MAP[location.y + 1][location.x - 1].isBomb){
        bombsCounter++;
    }
    if (MAP[location.y + 1][location.x].isBomb){
        bombsCounter++;
    }
    if (MAP[location.y + 1][location.x + 1].isBomb){
        bombsCounter++;
    }
    if (MAP[location.y][location.x - 1].isBomb){
        bombsCounter++;
    }
    if (MAP[location.y][location.x + 1].isBomb){
        bombsCounter++;
    }
    if (MAP[location.y - 1][location.x - 1].isBomb){
        bombsCounter++;
    }
    if (MAP[location.y - 1][location.x].isBomb){
        bombsCounter++;
    }
    if (MAP[location.y - 1][location.x + 1].isBomb){
        bombsCounter++;
    }

    tilesOpened++;
    MAP[location.y][location.x].isOpened = true;
    MAP[location.y][location.x].bombsNearby = bombsCounter;
    // flood-fill recursion calls to open the entire area
    if (bombsCounter == 0){
        checkTile(location + Point(1, -1));
        checkTile(location + Point(1, 0));
        checkTile(location + Point(1, 1));
        checkTile(location + Point(0, -1));
        checkTile(location + Point(0, 1));
        checkTile(location + Point(-1, -1));
        checkTile(location + Point(-1, 0));
        checkTile(location + Point(-1, 1));
    }

    if (tilesOpened == tilesToOpen && ! gameOver){
        win = true;
    }
}

void Minesweeper::openAllTiles(){
    for (int y = 0; y < MAP_SIZE; y++){
        for (int x = 0; x < MAP_SIZE; x++){
            MAP[y + 1][x + 1].isQuestionMarked = false;
            MAP[y + 1][x + 1].isFlagged = false;
            checkTile(Point(x + 1, y + 1));
        }
    } 
}

void Minesweeper::render(){
    clearScreen();
    for (int y = 0; y < MAP_SIZE; y++){
        for (int x = 0; x < MAP_SIZE; x++){
            if(! MAP[y + 1][x + 1].isOpened){
                screen.sprite(GAME_TILES.CLOSED, Point(10 + x * TILE_SIZE, 10 + y * TILE_SIZE));
            }
            if(MAP[y + 1][x + 1].bombsNearby == 0){
                screen.sprite(GAME_TILES.EMPTY, Point(10 + x * TILE_SIZE, 10 + y * TILE_SIZE));
            }
            if(MAP[y + 1][x + 1].bombsNearby == 1){
                screen.sprite(GAME_TILES.ONE, Point(10 + x * TILE_SIZE, 10 + y * TILE_SIZE));
            }
            if(MAP[y + 1][x + 1].bombsNearby == 2){
                screen.sprite(GAME_TILES.TWO, Point(10 + x * TILE_SIZE, 10 + y * TILE_SIZE));
            }
            if(MAP[y + 1][x + 1].bombsNearby == 3){
                screen.sprite(GAME_TILES.THREE, Point(10 + x * TILE_SIZE, 10 + y * TILE_SIZE));
            }
            if(MAP[y + 1][x + 1].bombsNearby == 4){
                screen.sprite(GAME_TILES.FOUR, Point(10 + x * TILE_SIZE, 10 + y * TILE_SIZE));
            }
            if(MAP[y + 1][x + 1].bombsNearby == 5){
                screen.sprite(GAME_TILES.FIVE, Point(10 + x * TILE_SIZE, 10 + y * TILE_SIZE));
            }
            if(MAP[y + 1][x + 1].bombsNearby == 6){
                screen.sprite(GAME_TILES.SIX, Point(10 + x * TILE_SIZE, 10 + y * TILE_SIZE));
            }
            if(MAP[y + 1][x + 1].bombsNearby == 7){
                screen.sprite(GAME_TILES.SEVEN, Point(10 + x * TILE_SIZE, 10 + y * TILE_SIZE));
            }
            if(MAP[y + 1][x + 1].bombsNearby == 8){
                screen.sprite(GAME_TILES.EIGHT, Point(10 + x * TILE_SIZE, 10 + y * TILE_SIZE));
            }
            if(MAP[y + 1][x + 1].isFlagged){
                screen.sprite(GAME_TILES.FLAG, Point(10 + x * TILE_SIZE, 10 + y * TILE_SIZE));
            }
            if(MAP[y + 1][x + 1].isQuestionMarked){
                screen.sprite(GAME_TILES.QUESTION, Point(10 + x * TILE_SIZE, 10 + y * TILE_SIZE));
            }
            if(MAP[y + 1][x + 1].isBomb && MAP[y + 1][x + 1].isOpened){
                screen.sprite(GAME_TILES.BOMB, Point(10 + x * TILE_SIZE, 10 + y * TILE_SIZE));
            }
            if(MAP[y + 1][x + 1].isBomb && MAP[y + 1][x + 1].isOpened && Point(x, y) == CursorLocation){
                screen.sprite(GAME_TILES.BOMB_RED, Point(10 + x * TILE_SIZE, 10 + y * TILE_SIZE));
            }
        }
    }
    screen.pen = Pen(0, 255, 0);
    screen.circle(Point(10 + 12 + CursorLocation.x * TILE_SIZE, 10 + 12 + CursorLocation.y * TILE_SIZE), 2);
    // TODO: draw normal sprite for the cursor.
    //screen.sprite(GAME_TILES.CURSOR, Point(10 + 12 + CursorLocation.x * TILE_SIZE, 10 + CursorLocation.y * TILE_SIZE));
}

void Minesweeper::update(){
    if (buttons.pressed & Button::DPAD_RIGHT){
        CursorLocation += Point(1, 0);
        if (CursorLocation.x >= MAP_SIZE){
           CursorLocation.x = 0;
        }
    }
    if (buttons.pressed & Button::DPAD_LEFT){
        CursorLocation -= Point(1, 0);
        if (CursorLocation.x < 0){
            CursorLocation.x = MAP_SIZE - 1;
        }
    }
    if (buttons.pressed & Button::DPAD_DOWN){
        CursorLocation += Point(0, 1);
        if (CursorLocation.y >= MAP_SIZE){
            CursorLocation.y = 0;
        }
    }
    if (buttons.pressed & Button::DPAD_UP){
        CursorLocation -= Point(0, 1);
        if (CursorLocation.y < 0){
            CursorLocation.y = MAP_SIZE - 1;
        }
    }
    if (buttons.pressed & Button::A){
        if (firstMove) {
            plantBombs(Point(CursorLocation.y + 1, CursorLocation.x + 1));
            firstMove = false;
        }
        checkTile(Point(CursorLocation.x + 1, CursorLocation.y + 1));
    }
    if (buttons.pressed & Button::B){
        MAP[CursorLocation.y + 1][CursorLocation.x + 1].bKeyHandler();
    }
    
    if (MAP[CursorLocation.y + 1][CursorLocation.x + 1].isBomb && MAP[CursorLocation.y + 1][CursorLocation.x + 1].isOpened){
        gameOver = true;
    }
    if (win || gameOver){
        openAllTiles();
    }
}

void Minesweeper::plantBombs(Point safeSpot){
    int toPlant = BOMBS;
    Point bombLocation;
    while (toPlant != 0){

        bombLocation.x = blit::random() % MAP_SIZE + 1;
        bombLocation.y = blit::random() % MAP_SIZE + 1;

        if ((bombLocation == safeSpot) || (bombLocation == safeSpot + Point(1, -1)) || 
            (bombLocation == safeSpot + Point(1, 0)) || (bombLocation == safeSpot + Point(1, 1)) ||
            (bombLocation == safeSpot + Point(0, -1)) || (bombLocation == safeSpot + Point(0, 1)) || 
            (bombLocation == safeSpot + Point(-1, -1)) || (bombLocation == safeSpot + Point(-1, 1)) ||
            (bombLocation == safeSpot + Point(-1, 0)) || (MAP[bombLocation.x][bombLocation.y].isBomb) )
        {
            continue;
        }
 
        MAP[bombLocation.x][bombLocation.y].isBomb = true;
        toPlant--;
        
    }
}

#ifdef SHOW_FPS
    void draw_fps(uint32_t &ms_start, uint32_t &ms_end) {

        //Draw fps box
        screen.alpha = 255;
        screen.pen = Pen(0, 0, 0);
        //screen.rectangle(Rect(0, screen.bounds.h - 5, 20, 9));
        screen.pen = Pen(0, 255, 0);

        //Calculate frames per second (fps)
        float time_difference_in_sec = static_cast<float>(ms_end - ms_start) / 1000;
        if (time_difference_in_sec == 0) {
            time_difference_in_sec = 1;
        }
        int fps = static_cast<int>(1 / time_difference_in_sec);

        //Cap max shown fps
        if (fps > 999) {
            fps = 999;
        }

        //Draw fps counter
        std::string fps_string = std::to_string(fps);
        screen.text(fps_string, minimal_font, Rect(1, screen.bounds.h - 7, 10, 10));

        //Draw frame time boxes
        int block_size = 4;
        for (int i = 0; i < static_cast<int>(ms_end - ms_start); i++) {
            screen.pen = Pen(i * 5, 255 - (i * 5), 0);
            screen.rectangle(Rect(i * (block_size + 1) + 1 + 21, screen.bounds.h - block_size - 1, block_size, block_size));
        }
    }
#endif


void init() {
    set_screen_mode(ScreenMode::hires);
    screen.sprites = Surface::load(spritesheet);
    screen.pen = Pen(0, 0, 0);
    screen.rectangle(Rect(0, 0, SCREEN_WEIGHT, SCREEN_HEIGHT));
    GM.newGame();
}

///////////////////////////////////////////////////////////////////////////
//
// render(time)
//
// This function is called to perform rendering of the game. time is the 
// amount if milliseconds elapsed since the start of your game
//


void render(uint32_t time) {

    #ifdef SHOW_FPS
        uint32_t ms_start = now();
    #endif

    
    GM.render();

    #ifdef SHOW_FPS
        uint32_t ms_end = now();
        draw_fps(ms_start, ms_end);
    #endif
}

    
///////////////////////////////////////////////////////////////////////////
//
// update(time)
//
// This is called to update your game state. time is the 
// amount if milliseconds elapsed since the start of your game
//
void update(uint32_t time) {
    GM.update();
}