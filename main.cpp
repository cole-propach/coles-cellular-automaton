#include "inc/SDL2/SDL.h"
//#include "inc/SDL2/SDL_image.h"
#include <emscripten/emscripten.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>

using namespace std;

#include "main.h"
#include "renderer.h"

#include <iostream>
#include <cassert>
#include <cmath>
#include <queue>
#include <limits>
#include <utility>
#include <algorithm>
#include <cassert>

SDL_Renderer* renderer = nullptr;
float deltaTime = 0.0;

bool quit = false;

void inputs(){
    SDL_Event event;
    
    //read all the inputs
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            quit = true;
        }
    }
}

void gameLoop() {
    // deltaTime calculation
    static Uint32 lastTime = SDL_GetTicks();
    Uint32 currentTime = SDL_GetTicks();
    deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    inputs();

    // render
    SDL_SetRenderDrawColor(renderer, 0, 0, 150, 255);
    SDL_RenderClear(renderer);

    int x, y;
    Uint32 buttons = SDL_GetMouseState(&x, &y);

    if(buttons & SDL_BUTTON(SDL_BUTTON_LEFT)){
        drawRect(x, y, 10, 10, 255, 0, 0);
    }

    SDL_RenderPresent(renderer);

    if (quit) {
        emscripten_cancel_main_loop();
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    SDL_Window* window = SDL_CreateWindow("CCA",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        GAME_WIDTH, GAME_HEIGHT, 0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Start the main loop (browser-friendly)
    emscripten_set_main_loop(gameLoop, 0, 1);

    // Cleanup happens inside gameLoop when quit = true
    return 0;
}

