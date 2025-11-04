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
#include "renderer.h"
#include "main.h"

void drawRect(int x, int y, int w, int h, int r, int g, int b){
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_Rect fillRect = { x, y, w, h };
    SDL_RenderFillRect(renderer, &fillRect);
}