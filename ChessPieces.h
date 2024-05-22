#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#ifndef ChessPieces
#define ChessPieces

class ChessPiece
{
public:
    SDL_Rect originalTile;
    bool isGrabbed;
    int *texturePointer;
    static int windowWidth;
    static int windowHeight;
    SDL_Texture *tex;
    SDL_Rect boundRect;
    ChessPiece(SDL_Renderer *renderer, const char *imagePath, int xPos, int yPos, int width, int height);
    ChessPiece();

    void updatePosition(int x, int y);
    void displace(int x, int y);
    SDL_bool clickedInRect(SDL_Point *p);
    SDL_bool collidingWithOtherPiece(std::vector<ChessPiece> chessVector, size_t currentIndex);
};

#endif