#include "graphics.h"
#include "gba.h"
#include <stdio.h>
// TA-TODO: Include any header files for images generated by nin10kit.
// Example for the provided garbage image:
#include "images/izaan.h"

// TA-TODO: Add any draw/undraw functions for sub-elements of your app here.
// For example, for a snake game, you could have a drawSnake function
// or a drawFood function
//
// e.g.:
// static void drawSnake(Snake* snake);
// static void drawFood(Food* food);

// This function will be used to draw everything about the app
// including the background and whatnot.
void fullDrawAppState(AppState *state) {
    // TA-TODO: IMPLEMENT.
    Player square = state -> player;
    //Enemy enemies[10] = state -> enemies;
    fillScreenDMA(CYAN);
    drawImageDMA(85, 40, IZAAN_WIDTH, IZAAN_HEIGHT, izaan);
    drawRectDMA(square.col, square.row, square.width, square.height, square.color);
    drawRectDMA(230, 60, 10, 30, MAGENTA);
    for (int i = 0; i < 9; i++)
    {
        drawRectDMA(state -> enemies[i].col, state -> enemies[i].row, state -> enemies[i].width, state -> enemies[i].height, state -> enemies[i].color);
    }


    char buffer[8];
    sprintf(buffer, "Lives: %d", state -> lives);
    drawString(3, 150, buffer, MAGENTA);

}

// This function will be used to undraw (i.e. erase) things that might
// move in a frame. E.g. in a Snake game, erase the Snake, the food & the score.
void undrawAppState(AppState *state) {
    // TA-TODO: IMPLEMENT.
    Player square = state -> player;
    drawRectDMA(square.col, square.row, square.width, square.height, CYAN);
    for (int i = 0; i < 9; i++)
    {
        drawRectDMA(state -> enemies[i].col, state -> enemies[i].row, state -> enemies[i].width, state -> enemies[i].height, CYAN);
    }
    char buffer[8];
    sprintf(buffer, "Lives: %d", state -> lives);
    drawString(3, 150, buffer, CYAN);
}

// This function will be used to draw things that might have moved in a frame.
// For example, in a Snake game, draw the snake, the food, the score.
void drawAppState(AppState *state) {
    // TA-TODO: IMPLEMENT.
    Player square = state -> player;
    drawImageDMA(85, 40, IZAAN_WIDTH, IZAAN_HEIGHT, izaan);
    drawRectDMA(square.col, square.row, square.width, square.height, square.color);
    for (int i = 0; i < 9; i++)
    {
        drawRectDMA(state -> enemies[i].col, state -> enemies[i].row, state -> enemies[i].width, state -> enemies[i].height, state -> enemies[i].color);
    }
    char buffer[8];
    sprintf(buffer, "Lives: %d", state -> lives);
    drawString(3, 150, buffer, MAGENTA);
}