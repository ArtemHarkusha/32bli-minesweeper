#include "minesweeper.hpp"
#include "assets.hpp"
#include <cstring>

using namespace blit;

#define SCREEN_HEIGHT 240
#define SCREEN_WEIGHT 320
#define TILE_SIZE 24

#define MAP_SIZE 8
#define SHOW_FPS

struct tiles {
    Rect CLOSED = Rect(0, 0, 3, 3);
};

tiles GAME_TILES;

// to describe 1 Tile on the battlefield
class Tile {
    public:
        bool isBomb = false;
        bool isFlagged = false;
        bool isQuestionMarked = false;
        bool isOpened = false;
        int  bombsNearby = -1;
};


class  Minesweeper {
    public:
        Tile MAP[MAP_SIZE + 2][MAP_SIZE + 2] {};
        void render();
        void plantBombs(int);
};

void Minesweeper::render(){
    screen.pen = Pen(0, 255, 0);
    for (int y = 0; y < MAP_SIZE; y++){
        for (int x = 0; x < MAP_SIZE; x++){
            if(! MAP[y + 1][x + 1].isOpened && ! MAP[y + 1][x + 1].isBomb){
                screen.sprite(GAME_TILES.CLOSED, Point(10 + x * TILE_SIZE, 10 + y * TILE_SIZE));
            }
        }
    }
    
}

void Minesweeper::plantBombs(int number){
    int x, y;
    while (number != 0){
        x = blit::random() % MAP_SIZE;
        y = blit::random() % MAP_SIZE;
        if (! MAP[x + 1][y + 1].isBomb){
            MAP[x + 1][y + 1].isBomb = true;
            number--;
        }
    }
}

Minesweeper test;

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
    test.plantBombs(10);

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

    
    test.render();

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
    
}