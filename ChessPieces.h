#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef ChessPieces
#define ChessPieces

class ChessPiece
{
public:
    bool isGrabbed;
    SDL_Rect boundRect;
    ChessPiece(SDL_Renderer *renderer, const char *imagePath, int xPos, int yPos, int width, int height);

    SDL_bool clickedInRect(SDL_Point &p);
};

#endif