#include <iostream>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include "ChessPieces.h"
#include <vector>
#include <algorithm>
#include "EventHandling.h"

void initialiseTiles(int START, int PIECE_SIZE, int WINDOW_HEIGHT)
{

    std::vector<SDL_Rect> tileColumn;
    for (int i = START; i <= START + PIECE_SIZE * 8; i += PIECE_SIZE)
    {
        for (int j = 0; j <= WINDOW_HEIGHT - PIECE_SIZE; j += PIECE_SIZE)
        {
            tileColumn.push_back(SDL_Rect{i, j, PIECE_SIZE, PIECE_SIZE});
        }
        ChessPiece::chessTiles2d.push_back(tileColumn);
        tileColumn = {};
    }
}

void drawTiles(SDL_Renderer *rend)
{
    bool flip_colour = false;
    for (std::vector<SDL_Rect> column : ChessPiece::chessTiles2d)
    {
        flip_colour = !flip_colour;
        for (SDL_Rect &tile : column)
        {
            if (flip_colour)
            {
                SDL_SetRenderDrawColor(rend, 115, 110, 117, 1);
            }

            else
            {
                SDL_SetRenderDrawColor(rend, 255, 255, 255, 1);
            }

            flip_colour = !flip_colour;

            SDL_RenderDrawRect(rend, &tile);
            SDL_RenderFillRect(rend, &tile);
        }
    }
}

int main(int argc, char *argv[])
{
    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    int WINDOW_WIDTH = 800,
        WINDOW_HEIGHT = 400;

    SDL_Window *win = SDL_CreateWindow("Chess", // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);

    // int SDL_CAPTUREMouse(SDL_ENABLE);
    // Do the same thing with the renderer etc? Stuff that would remain constant across chess pieces
    ChessPiece::windowWidth = WINDOW_WIDTH;
    ChessPiece::windowHeight = WINDOW_HEIGHT;

    int PIECE_SIZE = 50;
    int START = PIECE_SIZE * 3;

    for (int i = START; i <= START + PIECE_SIZE * 8; i += PIECE_SIZE)
    {
        // For some reason passing the renderer through a local function causes a segfault -so this cannot be abstracted for now.
        ChessPiece::chessPieceVector.push_back(
            ChessPiece(rend, "blackpiece.png", i, 0, PIECE_SIZE, PIECE_SIZE, "black"));
        ChessPiece::chessPieceVector.push_back(
            ChessPiece(rend, "blackpiece.png", i, PIECE_SIZE, PIECE_SIZE, PIECE_SIZE, "black"));

        ChessPiece::chessPieceVector.push_back(
            ChessPiece(rend, "whitepiece.jpg", i, WINDOW_HEIGHT - PIECE_SIZE, PIECE_SIZE, PIECE_SIZE, "white"));
        ChessPiece::chessPieceVector.push_back(
            ChessPiece(rend, "whitepiece.jpg", i, WINDOW_HEIGHT - (2 * PIECE_SIZE), PIECE_SIZE, PIECE_SIZE, "white"));
    }

    initialiseTiles(START, PIECE_SIZE, WINDOW_HEIGHT);

    bool anyPieceGrabbed = false;
    bool exit = false;

    SDL_Event event;

    while (!exit)
    {

        while (SDL_PollEvent(&event))
        {
            break;
            // eventHandler.handleEvent(event);
        }

        // Fills screen with black
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 1);
        SDL_RenderClear(rend);

        drawTiles(rend);

        // Renders all chess pieces
        for (ChessPiece &piece : ChessPiece::chessPieceVector)
        {
            SDL_RenderCopy(rend, piece.tex, NULL, &piece.boundRect);
        }

        // triggers the double buffers
        // for multiple rendering
        SDL_RenderPresent(rend);
    }

    SDL_DestroyWindow(win);
    SDL_Quit();
}