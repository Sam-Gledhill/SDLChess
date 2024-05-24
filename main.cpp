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

std::vector<std::vector<SDL_Rect>> initialiseTiles(int START, int PIECE_SIZE, int WINDOW_HEIGHT)
{

    std::vector<std::vector<SDL_Rect>> tilevector2d;

    std::vector<SDL_Rect> tileColumn;
    for (int i = START; i <= START + PIECE_SIZE * 8; i += PIECE_SIZE)
    {
        for (int j = 0; j <= WINDOW_HEIGHT - PIECE_SIZE; j += PIECE_SIZE)
        {
            tileColumn.push_back(SDL_Rect{i, j, PIECE_SIZE, PIECE_SIZE});
        }
        tilevector2d.push_back(tileColumn);
        tileColumn = {};
    }

    return tilevector2d;
}

SDL_Rect grabTileUnderCursor(SDL_Point mousePosition, std::vector<std::vector<SDL_Rect>> tileVector2d)
{
    for (auto &column : tileVector2d)
    {
        for (auto tile : column)
        {
            if (SDL_PointInRect(&mousePosition, &tile))
            {
                return tile;
            }
        }
    }

    return SDL_Rect{};
}

void drawTiles(SDL_Renderer *rend, std::vector<std::vector<SDL_Rect>> tilevector2d)
{
    bool flip_colour = false;
    for (std::vector<SDL_Rect> column : tilevector2d)
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

    // Can maybe have this as a class variable?
    std::vector<ChessPiece> chessPieceList;

    for (int i = START; i <= START + PIECE_SIZE * 8; i += PIECE_SIZE)
    {
        // For some reason passing the renderer through a local function causes a segfault -so this cannot be abstracted for now.
        chessPieceList.push_back(
            ChessPiece(rend, "blackpiece.png", i, 0, PIECE_SIZE, PIECE_SIZE, "black"));
        chessPieceList.push_back(
            ChessPiece(rend, "blackpiece.png", i, PIECE_SIZE, PIECE_SIZE, PIECE_SIZE, "black"));

        chessPieceList.push_back(
            ChessPiece(rend, "whitepiece.jpg", i, WINDOW_HEIGHT - PIECE_SIZE, PIECE_SIZE, PIECE_SIZE, "white"));
        chessPieceList.push_back(
            ChessPiece(rend, "whitepiece.jpg", i, WINDOW_HEIGHT - (2 * PIECE_SIZE), PIECE_SIZE, PIECE_SIZE, "white"));
    }

    std::vector<std::vector<SDL_Rect>> chessTileList = initialiseTiles(START, PIECE_SIZE, WINDOW_HEIGHT);

    bool _currentPieceClicked = false;
    bool anyPieceGrabbed = false;
    bool exit = false;

    SDL_Point mousePos;
    SDL_Event event;
    // bool pieceCaptured = false;
    // Uint32 before = SDL_GetTicks();
    // Uint32 after = SDL_GetTicks();
    // Uint32 delta;

    // float FRAMERATE = 30.0;

    while (!exit)
    {

        // after = SDL_GetTicks();
        // delta = after - before;
        // if (delta < 1000 / FRAMERATE)
        // {
        //     continue;
        // }
        // // std::cout << "escaped at: " << 1000 / delta << std::endl;
        // before = after;

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

                if (anyPieceGrabbed)
                {
                    ChessPiece &piece = chessPieceList.back();

                    // Pass in pointer instead of passing in whole vector
                    SDL_Rect tile = grabTileUnderCursor(mousePos, chessTileList);

                    // If piece is dragged somewhere that is not a tile.
                    if (SDL_RectEmpty(&tile))
                    {
                        piece.updatePosition(piece.originalTile.x, piece.originalTile.y);
                    }

                    else
                    {
                        piece.updatePosition(tile.x, tile.y);

                        // Bit of a janky way to do it but it works
                        size_t index = piece.collidingWithOtherPiece(chessPieceList, chessPieceList.size() - 1);

                        if (index != -1)
                        {
                            std::cout << index << std::endl;
                            std::cout << chessPieceList[index].team << piece.team << std::endl;
                            if (piece.team == chessPieceList[index].team)
                            {
                                std::cout << "Same team" << std::endl;
                                piece.updatePosition(piece.originalTile.x, piece.originalTile.y);
                            }
                            else
                            {
                                std::cout << "Different team" << std::endl;
                                piece.updatePosition(tile.x, tile.y);
                                chessPieceList.erase(chessPieceList.begin() + index);
                                piece.originalTile = tile;
                            }
                        }
                    }

                    piece.isGrabbed = false;
                    anyPieceGrabbed = false;
                    std::cout << piece.isGrabbed << anyPieceGrabbed << std::endl;
                }

                else
                {
                    for (size_t i = 0; i < chessPieceList.size(); i++)
                    {

                        // Use references or piece information doesn't update properly.
                        ChessPiece &piece = chessPieceList[i];

                        if (piece.clickedInRect(&mousePos))
                        {
                            std::cout << "Piece picked up / Put Down" << std::endl;

                            piece.isGrabbed = true;
                            piece.originalTile = grabTileUnderCursor(mousePos, chessTileList);
                            anyPieceGrabbed = true;
                            std::cout << piece.team << std::endl;
                            piece.updatePosition(mousePos.x - (piece.boundRect.w / 2), mousePos.y - (piece.boundRect.h / 2));

                            // Draws the currently grabbed piece on top of all the others so collision detection works properly
                            std::iter_swap(chessPieceList.begin() + i, chessPieceList.end() - 1);

                            break;
                        }
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

        // Refactor this to pass in a pointer to the list
        drawTiles(rend, chessTileList);

        // Renders all chess pieces
        for (ChessPiece &piece : chessPieceList)
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