#include "ChessPieces.h"

ChessPiece::ChessPiece(SDL_Renderer *renderer, const char *imagePath, int xPos, int yPos, int width, int height, std::string team)
{
    SDL_Surface *surface;
    surface = IMG_Load(imagePath);
    tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // clear surface texture
    isGrabbed = false;

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

void ChessPiece::displace(int x, int y)
{
    updatePosition(boundRect.x + x, boundRect.y + y);
}

SDL_bool ChessPiece::collidingWithOtherPiece(std::vector<ChessPiece> chessVector, size_t currentIndex)
{

    for (size_t i = 0; i < chessVector.size(); i++)
    {
        // Don't compare piece against itself
        if (i == currentIndex)
        {
            continue;
        }

        // todo: build default constructor so can declare this outside the function
        ChessPiece otherPiece = chessVector[i];

        if (SDL_HasIntersection(&boundRect, &otherPiece.boundRect))
        {
            return SDL_TRUE;
        }
    }

    return SDL_FALSE;
}

int ChessPiece::windowWidth;
int ChessPiece::windowHeight;