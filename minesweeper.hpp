#include "32blit.hpp"

#define MAP_SIZE 8
#define BOMBS_NUMBER 10

// to describe 1 Tile on the battlefield
class Tile {
    public:
        bool isBomb = false;
        bool isFlagged = false;
        bool isQuestionMarked = false;
        bool isOpened = false;
        int  bombsNearby = -1;

        void bKeyHandler();
};

class  Minesweeper {
    public:
        Tile MAP[MAP_SIZE + 2][MAP_SIZE + 2] {};
        blit::Point CursorLocation = blit::Point(0, 0);
        int BOMBS = BOMBS_NUMBER;
        bool firstMove = true;
        int  tilesOpened = 0;
        int  tilesToOpen = MAP_SIZE * MAP_SIZE - BOMBS;
        bool gameOver = false;
        bool win = false;
        void newGame();
        void render();
        void plantBombs(blit::Point);
        void update();
        void checkTile(blit::Point);
        void openAllTiles();
};

// game manager
class GameManager {
    public:
        bool inMenu = false;
        bool inGame = false;
        bool inGameOver = false;
        bool inWin = false;
        Minesweeper GAME;
        void newGame();
        void update();
        void render();
};