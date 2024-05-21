#include "ChessPieces.h"

ChessPiece::ChessPiece(SDL_Renderer *renderer, const char *imagePath, int xPos, int yPos, int width, int height)
{

    SDL_Surface *surface;
    surface = IMG_Load(imagePath);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // clear surface texture

    SDL_QueryTexture(tex, NULL, NULL, &boundRect.w, &boundRect.h); // Connects texture to rect

    boundRect.x = xPos;
    boundRect.y = yPos;
    boundRect.w = width;
    boundRect.h = height;
}

SDL_bool ChessPiece::clickedInRect(SDL_Point &p)
{
}