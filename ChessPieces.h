#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <string>
#include <functional>

#ifndef ChessPieces
#define ChessPieces

class ChessPiece
{
public:
    static std::vector<ChessPiece> chessPieceVector;
    static std::vector<std::vector<SDL_Rect>> chessTiles2d;
    static void initialiseTiles(int START, int PIECE_SIZE, int WINDOW_HEIGHT);
    static void initialiseChessPieces(SDL_Renderer *rend, int START, int PIECE_SIZE, int WINDOW_HEIGHT);

    // Wanted these to be private variables but caused segfault
    std::string type;
    static bool pawnMoveValid(ChessPiece &Piece, SDL_Rect currentTile);
    static bool rookMoveValid(ChessPiece &Piece, SDL_Rect currentTile);

    std::function<bool(ChessPiece &, SDL_Rect)> moveValid;
    void setType(std::string _type);

    bool attacking;
    std::string team;
    bool firstTurn;
    SDL_Rect originalTile;
    bool isGrabbed;
    int *texturePointer;
    static int windowWidth;
    static int windowHeight;
    SDL_Texture *tex;
    SDL_Rect boundRect;
    ChessPiece(SDL_Renderer *renderer, const char *imagePath, int xPos, int yPos, int width, int height, std::string _team);
    ~ChessPiece(){};
    void updatePosition(int x, int y);
    void displace(int x, int y);
    SDL_bool clickedInRect(SDL_Point *p);
    size_t collidingWithOtherPiece(std::vector<ChessPiece> chessVector, size_t currentIndex);
};

#endif