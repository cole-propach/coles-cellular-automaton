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

std::pair<int, int> toGrid(int x, int y) {
    int gridX = x / CELL_SIZE;
    int gridY = y / CELL_SIZE;
    return std::make_pair(gridX+1, gridY+1);
}

int gridPosToIndex(const std::pair<int, int>& gridPos) {
    int NUM_COLS = GAME_WIDTH / CELL_SIZE;
    int x = gridPos.first;  // column, starts at 1
    int y = gridPos.second; // row, starts at 1

    // Convert to 0-based index
    int index = (y - 1) * NUM_COLS + (x - 1);
    return index;
}

void clearGrid(){
    for(int i = 0; i < 1250; i++){
        bitmap[i] = false;
    }
}