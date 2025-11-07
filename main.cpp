/* Cole's Cellular Automaton

A cell's neighborhood is composed of 4 leaves (labeled with 1, 2, 3, and 4). 0s are ignored cells, and C is the cell whose neighborhood we are looking at.

0 0 1 0 0
0 0 1 0 0
4 4 C 2 2
0 0 3 0 0
0 0 3 0 0

Leaves can be in 1 of 3 states, depending on the number of live cells it contains:
    - Unpopulated: The leaf has exactly 0 live cells
    - Semi Populated: The leaf has exactly 1 live cell
    - Fully Populated: The lead has exactly 2 live cells

Rules (inspired Conway's Game of Life):
    - Live Cells
        - Overpopulation:
            A live cell with any fully populated leaves dies.
        - Underpopulation:
            A live cell with 0 semi populated leaves dies.
        - Survival:
            A live cell with any semi populated leaves stays alive.
    - Dead Cells
        - Reproduction:
            A dead cell with exactly 2 semi populated leaves becomes alive.
*/

#include "inc/SDL2/SDL.h"
#include <emscripten/emscripten.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>

using namespace std;

#include "main.h"
#include "renderer.h"
#include "simulation.h"

#include <iostream>
#include <cassert>
#include <cmath>
#include <queue>
#include <limits>
#include <utility>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>


SDL_Renderer* renderer = nullptr;
float deltaTime = 0.0;
bool bitmap[1250] = {false};
bool eraserOn = false;

void render(){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    drawGrid();
    drawCells();

    SDL_RenderPresent(renderer);
}

bool quit = false;
void gameLoop() {
    static Uint32 lastTime = SDL_GetTicks();
    Uint32 currentTime = SDL_GetTicks();
    deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    inputs();
    simulate();
    render();

    if (quit) {
        emscripten_cancel_main_loop();
    }
}

int main(int argc, char* argv[]) {
    srand(static_cast<unsigned int>(time(nullptr)));
    SDL_Init(SDL_INIT_VIDEO);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    SDL_Window* window = SDL_CreateWindow("CCA",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        GAME_WIDTH, GAME_HEIGHT, 0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    emscripten_set_main_loop(gameLoop, 0, 1);

    return 0;
}

