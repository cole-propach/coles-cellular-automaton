#include <iostream>
#include <cassert>
#include <cmath>
#include <queue>
#include <limits>
#include <utility>
#include <algorithm>
#include <cassert>
#include <emscripten/emscripten.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include "inc/SDL2/SDL.h"
#include "simulation.h"
#include "main.h"
#include <cstdlib>
#include <ctime>

using namespace std;

InputStatus* inputStatus;

void inputs(){
    SDL_Event event;

    inputStatus->mouseJustLeftClicked = false;
    
    //read all the inputs
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            quit = true;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                inputStatus->mouseJustLeftClicked = true;
            }
        }
        else if(event.type == SDL_KEYDOWN){
            if (event.key.keysym.sym == SDLK_e) {
                eraserOn = !eraserOn;
            }
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                clearGrid();
            }
            if (event.key.keysym.sym == SDLK_RIGHT) {
                doNextGen();
            }
            if (event.key.keysym.sym == SDLK_c) {
                clearGrid();
                insertChaos();
            }
            if (event.key.keysym.sym == SDLK_1) {
                loadPreset(1);
            }
            if (event.key.keysym.sym == SDLK_2) {
                loadPreset(2);
            }
            if (event.key.keysym.sym == SDLK_3) {
                loadPreset(3);
            }
            if (event.key.keysym.sym == SDLK_4) {
                loadPreset(4);
            }
            if (event.key.keysym.sym == SDLK_5) {
                loadPreset(5);
            }
            if (event.key.keysym.sym == SDLK_6) {
                loadPreset(6);
            }
            if (event.key.keysym.sym == SDLK_7) {
                loadPreset(7);
            }
            if (event.key.keysym.sym == SDLK_8) {
                loadPreset(8);
            }
            if (event.key.keysym.sym == SDLK_9) {
                loadPreset(9);
            }
        }
    }
}

void simulate(){
    int x, y;
    Uint32 buttons = SDL_GetMouseState(&x, &y);

    if(buttons & SDL_BUTTON(SDL_BUTTON_LEFT)){
        bitmap[gridPosToIndex(toGrid(x, y))] = !eraserOn;
    }
}

void doNextGen(){
    //outside the bounds of the game, assume every cell is dead and remains dead forever
    auto countSemiFully = [](const std::pair<int, int>& gridPos) {
        int gridX = gridPos.first;
        int gridY = gridPos.second;
        auto inBounds = [](int x, int y){
            const int NUM_COLS = GAME_WIDTH / CELL_SIZE;
            const int NUM_ROWS = GAME_HEIGHT / CELL_SIZE;
            return x >= 1 && x <= NUM_COLS && y >= 1 && y <= NUM_ROWS;
        };

        int semiCount = 0;
        int fullyCount = 0;
        int curLeafCount = 0;

        //helper to safely get cell value
        auto getCell = [&](int x, int y) -> bool {
            if (inBounds(x, y))
                return bitmap[gridPosToIndex({x, y})];
            return false; // dead if out of bounds
        };

        // top leaf
        curLeafCount = 0;
        curLeafCount += getCell(gridX, gridY - 1);
        curLeafCount += getCell(gridX, gridY - 2);
        if (curLeafCount == 1) semiCount++;
        else if (curLeafCount == 2) fullyCount++;

        // right leaf
        curLeafCount = 0;
        curLeafCount += getCell(gridX + 1, gridY);
        curLeafCount += getCell(gridX + 2, gridY);
        if (curLeafCount == 1) semiCount++;
        else if (curLeafCount == 2) fullyCount++;

        // bottom leaf
        curLeafCount = 0;
        curLeafCount += getCell(gridX, gridY + 1);
        curLeafCount += getCell(gridX, gridY + 2);
        if (curLeafCount == 1) semiCount++;
        else if (curLeafCount == 2) fullyCount++;

        // left leaf
        curLeafCount = 0;
        curLeafCount += getCell(gridX - 1, gridY);
        curLeafCount += getCell(gridX - 2, gridY);
        if (curLeafCount == 1) semiCount++;
        else if (curLeafCount == 2) fullyCount++;

        return std::make_pair(semiCount, fullyCount);
    };
    bool newbitmap[1250];
    //for every cell
    for(int i = 0; i < 1250; i++){
        //count number of semi populated leaves
        //count the number of fully populated leaves
        pair<int, int> counts = countSemiFully(indexToGridPos(i));
        //if that cell is dead
        if(!bitmap[i]){
            //if it has 2 semi populated leaves
            if(counts.first == 2){
                //make this cell alive
                newbitmap[i] = true;
            }
            else{
                newbitmap[i] = false;
            }
        }
        //else (cell is alive)
        else{
            //if it has any fully populated leaves
            if(counts.second){
                //kill this cell
                newbitmap[i] = false;
            }
            //else if it has 0 semi populated leaves
            else if(!counts.first){
                //kill this cell
                newbitmap[i] = false;
            }
            //else
            else{
                //cell stays alive
                newbitmap[i] = true;
            }
        }
    }
    memcpy(bitmap, newbitmap, sizeof(bool) * 1250);
}


std::pair<int, int> toGrid(int x, int y) {
    int gridX = x / CELL_SIZE;
    int gridY = y / CELL_SIZE;
    return std::make_pair(gridX+1, gridY+1);
}

int gridPosToIndex(const std::pair<int, int>& gridPos) {
    int NUM_COLS = GAME_WIDTH / CELL_SIZE;
    int x = gridPos.first;
    int y = gridPos.second;

    int index = (y - 1) * NUM_COLS + (x - 1);
    return index;
}

std::pair<int, int> indexToGridPos(int index) {
    int NUM_COLS = GAME_WIDTH / CELL_SIZE;

    int y = (index / NUM_COLS) + 1;
    int x = (index % NUM_COLS) + 1;

    return {x, y};
}

void clearGrid(){
    for(int i = 0; i < 1250; i++){
        bitmap[i] = false;
    }
}

std::vector<std::vector<int>> cycle81 = {
    {1,1,0,1,1,0,1,1},
    {1,1,0,1,1,0,1,1},
    {0,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,1,1},
    {0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1},
    {0,0,0,1,1,0,1,1},
    {0,0,0,0,0,0,0,0},
    {1,1,0,1,1,0,1,1},
    {1,1,0,1,1,0,1,1},
};

std::vector<std::vector<int>> rocket = {
    {0,1,0},
    {0,1,0},
    {0,0,0},
    {1,1,1},
    {1,0,1}
};
std::vector<std::vector<int>> garden = {
    {1,1,0,0,0,0,0,0,0,0,0,0,1,1},
    {1,1,0,1,0,1,0,0,1,0,1,0,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,0,1,1,0,1,1,0,1,1,0,1,1},
    {1,1,0,1,1,0,1,1,0,1,1,0,1,1}
};
std::vector<std::vector<int>> snowflake = {
    {1,1,0,0,1,0,0,1,1},
    {1,1,0,0,1,0,0,1,1},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,1,0,0,0,0},
    {1,1,0,1,0,1,0,1,1},
    {0,0,0,0,1,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {1,1,0,0,1,0,0,1,1},
    {1,1,0,0,1,0,0,1,1}
};
std::vector<std::vector<int>> flower = {
    {1,1,0,1,1,0,1,1,0,1,1},
    {1,1,0,1,1,0,1,1,0,1,1},
    {0,0,0,0,0,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0,0,1,1},
    {1,1,0,0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0,0,1,1},
    {1,1,0,0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,0,0,0,0,0},
    {1,1,0,1,1,0,1,1,0,1,1},
    {1,1,0,1,1,0,1,1,0,1,1},
};

void placePatternCentered(const std::vector<std::vector<int>>& pattern) {
    const int NUM_COLS = GAME_WIDTH / CELL_SIZE;
    const int NUM_ROWS = GAME_HEIGHT / CELL_SIZE;

    int patRows = pattern.size();
    int patCols = pattern[0].size();

    // Compute top-left position to center the pattern
    int startRow = (NUM_ROWS - patRows) / 2 + 1; // +1 because 1-based
    int startCol = (NUM_COLS - patCols) / 2 + 1;

    for (int r = 0; r < patRows; ++r) {
        for (int c = 0; c < patCols; ++c) {
            int gridX = startCol + c;
            int gridY = startRow + r;
            if (gridX >= 1 && gridX <= NUM_COLS && gridY >= 1 && gridY <= NUM_ROWS) {
                bitmap[gridPosToIndex({gridX, gridY})] = pattern[r][c] != 0;
            }
        }
    }
}

void makeFence() {
    const int NUM_COLS = GAME_WIDTH / CELL_SIZE;
    const int NUM_ROWS = GAME_HEIGHT / CELL_SIZE;

    // Clear the bitmap first
    for(int i = 0; i < NUM_COLS * NUM_ROWS; ++i) {
        bitmap[i] = false;
    }

    int squareWidth = 2;   // 2x2 squares
    int squareHeight = 2;
    int rowSpacing = 1;    // 1 empty cell between rows
    int colSpacing = 1;    // 1 empty cell between columns

    int numSquareRows = (NUM_ROWS + rowSpacing) / (squareHeight + rowSpacing);
    int numSquareCols = (NUM_COLS + colSpacing) / (squareWidth + colSpacing);

    int totalGridHeight = numSquareRows * squareHeight + (numSquareRows - 1) * rowSpacing;
    int totalGridWidth  = numSquareCols * squareWidth + (numSquareCols - 1) * colSpacing;

    int startRow = (NUM_ROWS - totalGridHeight) / 2 + 1;
    int startCol = (NUM_COLS - totalGridWidth) / 2 + 1;

    // Draw the border squares
    for (int r = 0; r < numSquareRows; ++r) {
        for (int c = 0; c < numSquareCols; ++c) {
            // Skip interior squares, except for the border
            if (r != 0 && r != numSquareRows - 1 && c != 0 && c != numSquareCols - 1) continue;

            int squareStartRow = startRow + r * (squareHeight + rowSpacing);
            int squareStartCol = startCol + c * (squareWidth + colSpacing);

            // Fill the 2x2 square
            for (int dr = 0; dr < squareHeight; ++dr) {
                for (int dc = 0; dc < squareWidth; ++dc) {
                    int gridX = squareStartCol + dc;
                    int gridY = squareStartRow + dr;

                    if (gridX >= 1 && gridX <= NUM_COLS && gridY >= 1 && gridY <= NUM_ROWS) {
                        bitmap[gridPosToIndex({gridX, gridY})] = true;
                    }
                }
            }
        }
    }
}

void insertChaos(){
    double aliveProbability = 0.4;
    int topMargin = 4;
    int leftMargin = 3;
    int NUM_COLS = GAME_WIDTH / CELL_SIZE;
    int NUM_ROWS = GAME_HEIGHT / CELL_SIZE;

    // Fill board with random live/dead cells, ignoring margins
    for (int y = 1; y <= NUM_ROWS; ++y) {
        for (int x = 1; x <= NUM_COLS; ++x) {
            // Skip if within any of the margins
            if (y <= topMargin || y > NUM_ROWS - topMargin ||
                x <= leftMargin || x > NUM_COLS - leftMargin) {
                //bitmap[gridPosToIndex({x, y})] = false;
                continue;
            }

            // Random alive/dead assignment
            double r = static_cast<double>(rand()) / RAND_MAX;
            bitmap[gridPosToIndex({x, y})] = (r < aliveProbability);
        }
    }
}

void drawFlowerAt(int x, int y) {
    bitmap[gridPosToIndex({x,     y - 1})] = true; // top
    bitmap[gridPosToIndex({x - 1, y    })] = true; // left
    bitmap[gridPosToIndex({x + 1, y    })] = true; // right
    bitmap[gridPosToIndex({x,     y + 1})] = true; // bottom
}

void makeCheckeredDiamond() {
    std::pair<int,int> center = { (GAME_WIDTH / CELL_SIZE + 1) / 2,
                                  (GAME_HEIGHT / CELL_SIZE + 1) / 2 };
    int cx = center.first;
    int cy = center.second;

    int radius = 6; // Manhattan distance from center; ensures 7 cells along each diagonal

    for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
            if (std::abs(dx) + std::abs(dy) <= radius) {
                bool alive = ((cx + dx + cy + dy) % 2 == 0); // checker pattern
                int index = gridPosToIndex({cx + dx, cy + dy});
                bitmap[index] = alive;
            }
        }
    }
}

void loadPreset(int number){
    if(number < 1 || number > 9) return;

    clearGrid();
    switch(number){
        case 1:
            placePatternCentered(cycle81);
        break;
        case 2:
            makeFence();
        break;
        case 3:
            makeFence();
            insertChaos();
        break;
        case 4:
            placePatternCentered(rocket);
        break;
        case 5:
            placePatternCentered(garden);
        break;
        case 6:
            //make Snowflake machine
            makeCheckeredDiamond();
        break;
        case 7:
            placePatternCentered(snowflake);
        break;
        case 8:
            placePatternCentered(flower);
        break;
        case 9:
            //make 4 flowers
            drawFlowerAt(21, 13);
            drawFlowerAt(29, 13);
            drawFlowerAt(25, 17);
            drawFlowerAt(25, 9);
        break;
    }

}