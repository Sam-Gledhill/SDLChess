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

    SDL_Window *win = SDL_CreateWindow("Capybara Chess", // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;

    int SDL_CAPTUREMouse(SDL_ENABLE);

    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);

    // Do the same thing with the renderer etc? Stuff that would remain constant across chess pieces
    ChessPiece::windowWidth = WINDOW_WIDTH;
    ChessPiece::windowHeight = WINDOW_HEIGHT;

    std::vector<ChessPiece> chessPieceList;
    std::vector<std::vector<SDL_Rect>> chessTileList;

    int PIECE_SIZE = 50;
    int START = PIECE_SIZE * 3;
    for (int i = START; i <= START + PIECE_SIZE * 8; i += PIECE_SIZE)
    {
        chessPieceList.push_back(
            ChessPiece(rend, "capybara.png", i, 0, PIECE_SIZE, PIECE_SIZE));
        chessPieceList.push_back(
            ChessPiece(rend, "capybara.png", i, PIECE_SIZE, PIECE_SIZE, PIECE_SIZE));

        chessPieceList.push_back(
            ChessPiece(rend, "capybara.png", i, WINDOW_HEIGHT - PIECE_SIZE, PIECE_SIZE, PIECE_SIZE));
        chessPieceList.push_back(
            ChessPiece(rend, "capybara.png", i, WINDOW_HEIGHT - (2 * PIECE_SIZE), PIECE_SIZE, PIECE_SIZE));
    }

    std::vector<SDL_Rect> tileColumn;
    for (int i = START; i <= START + PIECE_SIZE * 8; i += PIECE_SIZE)
    {
        for (int j = 0; j <= WINDOW_HEIGHT - PIECE_SIZE; j += PIECE_SIZE)
        {
            tileColumn.push_back(SDL_Rect{i, j, PIECE_SIZE, PIECE_SIZE});
        }
        chessTileList.push_back(tileColumn);
        tileColumn = {};
    }

    bool _currentPieceClicked = false;
    bool anyPieceGrabbed = false;
    bool exit = false;

    SDL_Point mousePos;

    bool pieceCaptured = false;

    while (!exit)
    {

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {

            case SDL_QUIT:
                exit = true;
                break;

            case SDL_MOUSEMOTION:
                if (!anyPieceGrabbed)
                {
                    break;
                }

                for (ChessPiece &piece : chessPieceList)
                {
                    if (!piece.isGrabbed)
                    {
                        continue;
                    }

                    SDL_GetMouseState(&mousePos.x, &mousePos.y);

                    // Set midpoint of image to mouse position
                    piece.updatePosition(mousePos.x - (piece.boundRect.w / 2), mousePos.y - (piece.boundRect.h / 2));

                    // When grabbed piece updated - break
                    break;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:

                SDL_GetMouseState(&mousePos.x, &mousePos.y);

                for (size_t i = 0; i < chessPieceList.size(); i++)
                {

                    // Use references or piece information doesn't update properly.
                    ChessPiece &piece = chessPieceList[i];
                    _currentPieceClicked = piece.clickedInRect(&mousePos);
                    // Can only grab one piece at a time
                    if (anyPieceGrabbed && !piece.isGrabbed && (_currentPieceClicked || piece.collidingWithOtherPiece(chessPieceList, i)))
                    {
                        std::cout << "Not this piece" << std::endl;
                        break;
                    }

                    if (_currentPieceClicked)
                    {
                        std::cout << "Piece picked up / Put Down" << std::endl;

                        piece.isGrabbed = !piece.isGrabbed;
                        anyPieceGrabbed = bool(piece.isGrabbed);

                        piece.updatePosition(mousePos.x - (piece.boundRect.w / 2), mousePos.y - (piece.boundRect.h / 2));

                        // Draws the currently grabbed piece on top of all the others so collision detection works properly
                        std::iter_swap(chessPieceList.begin() + i, chessPieceList.end() - 1);

                        break;
                    }
                }
                break;

            case SDL_KEYDOWN:

                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:
                    exit = true;
                    break;
                }
            }
        }

        // Fills screen with black
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 1);
        SDL_RenderClear(rend);

        bool flip_colour = false;

        for (std::vector<SDL_Rect> column : chessTileList)
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

        for (ChessPiece &piece : chessPieceList)
        {
            SDL_RenderCopy(rend, piece.tex, NULL, &piece.boundRect);
        }

        // triggers the double buffers
        // for multiple rendering
        SDL_RenderPresent(rend);
    }

    // We destroy our window. We are passing in the pointer
    // that points to the memory allocated by the
    // 'SDL_CreateWindow' function. Remember, this is
    // a 'C-style' API, we don't have destructors.
    SDL_DestroyWindow(win);

    // We safely uninitialize SDL2, that is, we are
    // taking down the subsystems here before we exit
    // our program.
    SDL_Quit();
}
