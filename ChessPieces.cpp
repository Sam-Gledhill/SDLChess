#include "ChessPieces.h"

ChessPiece::ChessPiece(SDL_Renderer *renderer, const char *imagePath, int xPos, int yPos, int width, int height, std::string _team)
{
    SDL_Surface *surface;
    team = _team;
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

    boundRect.y = y;
}

SDL_bool ChessPiece::clickedInRect(SDL_Point *p)
{
    return SDL_PointInRect(p, &boundRect);
}

void ChessPiece::displace(int x, int y)
{
    updatePosition(boundRect.x + x, boundRect.y + y);
}

size_t ChessPiece::collidingWithOtherPiece(std::vector<ChessPiece> chessVector, size_t currentIndex)
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
            return i;
        }
    }

    return -1;
}

void ChessPiece::initialiseTiles(int START, int PIECE_SIZE, int WINDOW_HEIGHT)
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

void ChessPiece::initialiseChessPieces(SDL_Renderer *rend, int START, int PIECE_SIZE, int WINDOW_HEIGHT)
{
    for (int i = START; i <= START + PIECE_SIZE * 8; i += PIECE_SIZE)
    {
        // For some reason passing the renderer through a local function causes a segfault here -so this cannot be abstracted for now.
        ChessPiece::chessPieceVector.push_back(
            ChessPiece(rend, "blackpiece.png", i, 0, PIECE_SIZE, PIECE_SIZE, "black"));
        ChessPiece::chessPieceVector.push_back(
            ChessPiece(rend, "blackpiece.png", i, PIECE_SIZE, PIECE_SIZE, PIECE_SIZE, "black"));

        ChessPiece::chessPieceVector.push_back(
            ChessPiece(rend, "whitepiece.jpg", i, WINDOW_HEIGHT - PIECE_SIZE, PIECE_SIZE, PIECE_SIZE, "white"));
        ChessPiece::chessPieceVector.push_back(
            ChessPiece(rend, "whitepiece.jpg", i, WINDOW_HEIGHT - (2 * PIECE_SIZE), PIECE_SIZE, PIECE_SIZE, "white"));
    }
}

int ChessPiece::windowWidth;
int ChessPiece::windowHeight;

std::vector<ChessPiece> ChessPiece::chessPieceVector{};
std::vector<std::vector<SDL_Rect>> ChessPiece::chessTiles2d{};
