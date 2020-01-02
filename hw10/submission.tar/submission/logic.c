#include "logic.h"

void initializeAppState(AppState* appState) {
    // TA-TODO: Initialize everything that's part of this AppState struct here.
    // Suppose the struct contains random values, make sure everything gets
    // the value it should have when the app begins.
    appState -> gameOver = 0;
    appState -> victory = 0;
    appState -> lives = 5;

    appState -> player.row = 70;
    appState -> player.col = 5;
    appState -> player.width = 10;
    appState -> player.height = 10;
    appState -> player.color = YELLOW;

    for (int i = 0; i < 9; i++) {
        appState -> enemies[i].row = 10 + i*10;
        appState -> enemies[i].col = 20 + i*24;
        appState -> enemies[i].width = 10;
        appState -> enemies[i].height = 10;
        appState -> enemies[i].direction = 0;
        appState -> enemies[i].color = RED;
    }

}

// TA-TODO: Add any process functions for sub-elements of your app here.
// For example, for a snake game, you could have a processSnake function
// or a createRandomFood function or a processFoods function.
//
// e.g.:
// static Snake processSnake(Snake* currentSnake);
// static void generateRandomFoods(AppState* currentAppState, AppState* nextAppState);

// This function processes your current app state and returns the new (i.e. next)
// state of your application.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    /* TA-TODO: Do all of your app processing here. This function gets called
     * every frame.
     *
     * To check for key presses, use the KEY_JUST_PRESSED macro for cases where
     * you want to detect each key press once, or the KEY_DOWN macro for checking
     * if a button is still down.
     *
     * To count time, suppose that the GameBoy runs at a fixed FPS (60fps) and
     * that VBlank is processed once per frame. Use the vBlankCounter variable
     * and the modulus % operator to do things once every (n) frames. Note that
     * you want to process button every frame regardless (otherwise you will
     * miss inputs.)
     *
     * Do not do any drawing here.
     *
     * TA-TODO: VERY IMPORTANT! READ THIS PART.
     * You need to perform all calculations on the currentAppState passed to you,
     * and perform all state updates on the nextAppState state which we define below
     * and return at the end of the function. YOU SHOULD NOT MODIFY THE CURRENTSTATE.
     * Modifying the currentAppState will mean the undraw function will not be able
     * to undraw it later.
     */

    AppState nextAppState = *currentAppState;

    //Set up player movement
    if (KEY_DOWN(BUTTON_RIGHT, keysPressedNow) && nextAppState.player.col < 230) {
        nextAppState.player.col++;
    }
    if (KEY_DOWN(BUTTON_LEFT, keysPressedNow) && nextAppState.player.col > 0) {
        nextAppState.player.col--;
    }
    if (KEY_DOWN(BUTTON_UP, keysPressedNow) && nextAppState.player.row > 0) {
        nextAppState.player.row--;
    }
    if (KEY_DOWN(BUTTON_DOWN, keysPressedNow) && nextAppState.player.row < 150) {
        nextAppState.player.row++;
    }

    //Setup vertical movement for enemy blocks
    for (int i = 0; i < 9; i++)
    {
        if (nextAppState.enemies[i].row < 139 && !nextAppState.enemies[i].direction) {
            nextAppState.enemies[i].row+=2;
        }
        if (nextAppState.enemies[i].row > 21 && nextAppState.enemies[i].direction) {
            nextAppState.enemies[i].row-=2;

        }
        if (nextAppState.enemies[i].row == 140) {
            nextAppState.enemies[i].row-=2;
            nextAppState.enemies[i].direction = 1;
        }
        if (nextAppState.enemies[i].row == 20) {
            nextAppState.enemies[i].row+=2;
            nextAppState.enemies[i].direction = 0;
        }
        int colDiff = nextAppState.player.col - nextAppState.enemies[i].col;
        int rowDiff = nextAppState.player.row - nextAppState.enemies[i].row;

        //Check collision between player and enemy
        if (colDiff <= 10 && colDiff >= -10) {
            if (rowDiff <= 10 && rowDiff >= -10) {
                nextAppState.lives--;
                nextAppState.player.col = 5;
                nextAppState.player.row = 70;
            }
        }
    }

    //Set gameOver to true if no lives left
    if (nextAppState.lives == 0) {
        nextAppState.gameOver = 1;
    }

    //Check if player reached end of level
    int xDiff = 230 - nextAppState.player.col;
    int yDiff = 60 - nextAppState.player.row;
    if (xDiff >= 0 && xDiff <= 10) {
        if (yDiff <= 10 && yDiff >= -30) {
            nextAppState.victory = 1;
        }
    }


    UNUSED(keysPressedBefore);

    return nextAppState;
}
