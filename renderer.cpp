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

void drawLine(int x1, int y1, int x2, int y2, int thickness, int r, int g, int b) {
    // Calculate the distance and angle between the points
    float dx = x2 - x1;
    float dy = y2 - y1;
    float length = std::sqrt(dx*dx + dy*dy);

    if (length == 0) return; // avoid division by zero

    // Unit vector along the line
    float ux = dx / length;
    float uy = dy / length;

    // Draw small rectangles along the line
    for (float i = 0; i <= length; i += 1.0f) {
        int cx = int(x1 + ux * i);
        int cy = int(y1 + uy * i);
        drawRect(cx - thickness/2, cy - thickness/2, thickness, thickness, r, g, b);
    }
}

void drawGrid() {
    // Draw vertical lines
    for (int x = 0; x < GAME_WIDTH; x += CELL_SIZE) {
        drawLine(x, 0, x, GAME_HEIGHT, 2, 0, 0, 0); // inner lines
    }
    drawLine(0, 0, 0, GAME_HEIGHT, 1, 0, 0, 0);                 // left edge
    drawLine(GAME_WIDTH - 1, 0, GAME_WIDTH - 1, GAME_HEIGHT, 1, 0, 0, 0); // right edge

    // Draw horizontal lines
    for (int y = 0; y < GAME_HEIGHT; y += CELL_SIZE) {
        drawLine(0, y, GAME_WIDTH, y, 2, 0, 0, 0); // inner lines
    }
    drawLine(0, 0, GAME_WIDTH, 0, 1, 0, 0, 0);                  // top edge
    drawLine(0, GAME_HEIGHT - 1, GAME_WIDTH, GAME_HEIGHT - 1, 1, 0, 0, 0); // bottom edge
}

void drawCells() {
    const int NUM_COLS = GAME_WIDTH / CELL_SIZE;
    const int NUM_ROWS = GAME_HEIGHT / CELL_SIZE;
    for (int y = 1; y <= NUM_ROWS; ++y) {         // 1-based row
        for (int x = 1; x <= NUM_COLS; ++x) {     // 1-based column
            int index = (y - 1) * NUM_COLS + (x - 1);
            if (bitmap[index]) {
                int px = (x - 1) * CELL_SIZE;    // pixel x
                int py = (y - 1) * CELL_SIZE;    // pixel y
                drawRect(px, py, CELL_SIZE, CELL_SIZE, 0, 0, 0); // black rectangle
            }
        }
    }
}