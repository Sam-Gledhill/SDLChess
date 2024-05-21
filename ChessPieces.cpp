#include "ChessPieces.h"

ChessPiece::ChessPiece(SDL_Renderer *renderer, const char *imagePath, int xPos, int yPos, int width, int height)
{
    SDL_Surface *surface;
    surface = IMG_Load(imagePath);
    tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // clear surface texture

    SDL_QueryTexture(tex, NULL, texturePointer, &boundRect.w, &boundRect.h); // Connects texture to rect

    boundRect.x = xPos;
    boundRect.y = yPos;
    boundRect.w = width;
    boundRect.h = height;
}

void ChessPiece::updatePosition(int x, int y)
{

    if (x && x >= 0 && x <= ChessPiece::windowWidth - boundRect.w)
    {
        boundRect.x = x;
    }

    if (y && y >= 0 && y <= ChessPiece::windowHeight - boundRect.h)
    {
        boundRect.y = y;
    }
}

SDL_bool ChessPiece::clickedInRect(SDL_Point *p)
{
    return SDL_PointInRect(p, &boundRect);
}

int ChessPiece::windowWidth;
int ChessPiece::windowHeight;