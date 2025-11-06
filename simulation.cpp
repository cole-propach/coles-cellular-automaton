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