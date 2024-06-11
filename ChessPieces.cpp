#include "ChessPieces.h"

ChessPiece::ChessPiece(SDL_Renderer *renderer, const char *imagePath, int xPos, int yPos, int width, int height, std::string _team)
{
    attacking = false;
    SDL_Surface *surface;
    firstTurn = true;
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

ChessPiece::ChessPiece()
{
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

size_t ChessPiece::collidingWithOtherPiece(std::vector<ChessPiece> chessVector, size_t currentIndex)
{
    // This should be refactored to get rid of the chessVector passthrough

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

void ChessPiece::setOriginalTile()
{
    for (std::vector<SDL_Rect> &column : chessTiles2d)
    {
        for (SDL_Rect &tile : column)
        {
            if (SDL_HasIntersection(&boundRect, &tile))
            {
                originalTile = tile;
            }
        }
    }
}

void ChessPiece::initialiseTiles(int START, int PIECE_SIZE, int WINDOW_HEIGHT)
{

    std::vector<SDL_Rect> tileColumn;
    for (int i = START; i <= START + PIECE_SIZE * 7; i += PIECE_SIZE)
    {
        for (int j = 0; j <= WINDOW_HEIGHT - PIECE_SIZE; j += PIECE_SIZE)
        {
            tileColumn.push_back(SDL_Rect{i, j, PIECE_SIZE, PIECE_SIZE});
        }
        ChessPiece::chessTiles2d.push_back(tileColumn);
        tileColumn = {};
    }
}

void ChessPiece::setType(std::string _type)
{
    type = _type;
    if (type == "pawn")
    {
        // Instead of having checks for every piece type - just call the relevant piece.moveFunction()
        moveValid = pawnMoveValid;
    }

    else if (type == "rook")
    {
        moveValid = rookMoveValid;
    }

    else if (type == "bishop")
    {
        moveValid = bishopMoveValid;
    }

    else if (type == "knight")
    {
        moveValid = knightMoveValid;
    }

    else if (type == "queen")
    {
        moveValid = queenMoveValid;
    }

    else if (type == "king")
    {
        moveValid = kingMoveValid;
    }
}

// Note for king, simulate moving every enemy piece on the board to the king

// Can probably abstract the move logic to another cpp file as passing in piece anyway.
bool ChessPiece::pawnMoveValid(ChessPiece &piece, SDL_Rect currentTile)
{
    if (piece.originalTile.x == currentTile.x && piece.originalTile.y == currentTile.y)
    {
        return false;
    }

    int MULT = 1 + piece.firstTurn;
    bool validMove;

    // move without being on top of another tile
    if (!piece.attacking)
    {

        if (piece.team == "black")
        {
            bool validBlackY = currentTile.y == (MULT * 50) + piece.originalTile.y || currentTile.y == 1 * 50 + piece.originalTile.y;
            bool validBlackX = currentTile.x == piece.originalTile.x;

            validMove = validBlackX && validBlackY;
        }

        else if (piece.team == "white")
        {
            bool validWhiteY = currentTile.y == -(MULT * 50) + piece.originalTile.y || currentTile.y == -1 * 50 + piece.originalTile.y;
            bool validWhitex = currentTile.x == piece.originalTile.x;

            validMove = validWhitex && validWhiteY;
        }
    }

    else
    {
        if (piece.team == "black")
        {
            // Not doing en passant for now

            bool validBlackY = currentTile.y == piece.originalTile.y + 50;
            bool validBlackx = abs(piece.originalTile.x - currentTile.x) == 50;

            validMove = validBlackx && validBlackY;
        }
        if (piece.team == "white")
        {
            // Not doing en passant for now
            bool validWhiteY = currentTile.y == piece.originalTile.y - 50;
            bool validWhiteX = abs(piece.originalTile.x - currentTile.x) == 50;
            validMove = validWhiteX && validWhiteY;

            // std::cout << validWhiteX << validWhiteY << std::flush;
        }
    }

    return validMove;
}

bool ChessPiece::rookMoveValid(ChessPiece &piece, SDL_Rect currentTile)
{
    // Have to check if any pieces are between the start and end points
    return currentTile.x == piece.originalTile.x || currentTile.y == piece.originalTile.y;
}

bool ChessPiece::bishopMoveValid(ChessPiece &piece, SDL_Rect currentTile)
{
    int denom = currentTile.x - piece.originalTile.x;

    // Avoid divide by zero errors.
    if (denom == 0)
    {
        return false;
    }

    return abs((currentTile.y - piece.originalTile.y) / denom) == 1;
}

bool ChessPiece::knightMoveValid(ChessPiece &piece, SDL_Rect currentTile)
{
    int dx = currentTile.x - piece.originalTile.x;
    int dy = currentTile.y - piece.originalTile.y;

    return (abs(dx) == 2 * 50 && abs(dy) == 1 * 50) || (abs(dx) == 1 * 50 && abs(dy) == 2 * 50);
}

bool ChessPiece::queenMoveValid(ChessPiece &piece, SDL_Rect currentTile)
{
    int denom = currentTile.x - piece.originalTile.x;
    int num = currentTile.y - piece.originalTile.y;
    bool diagonalValid;

    // Avoid divide by zero errors.
    if (denom == 0)
    {
        diagonalValid = false;
    }

    else
    {
        diagonalValid = abs(num / denom) == 1;
    }

    bool straightValid = currentTile.x == piece.originalTile.x || currentTile.y == piece.originalTile.y;

    return diagonalValid || straightValid;
}

bool ChessPiece::kingMoveValid(ChessPiece &piece, SDL_Rect currentTile)
{
    int dx = abs(currentTile.x - piece.originalTile.x);
    int dy = abs(currentTile.y - piece.originalTile.y);

    return (dx == 0 && dy == 1 * 50) || (dx == 1 * 50 && dy == 0) || (dx == 1 * 50 && dy == 1 * 50);
}

void ChessPiece::addPieceToBoard(ChessPiece &piece, std::string pieceType)
{
    piece.setType(pieceType);
    ChessPiece::chessPieceVector.push_back(piece);
}

void ChessPiece::initialiseChessPieces(SDL_Renderer *rend, int START, int PIECE_SIZE, int WINDOW_HEIGHT)
{

    // This function is awful - desperate need of refactor.

    // Setup pawns
    for (int i = START; i <= START + PIECE_SIZE * 7; i += PIECE_SIZE)
    {
        ChessPiece::chessPieceVector.push_back(
            ChessPiece(rend, "Assets/PawnBlack.png", i, PIECE_SIZE, PIECE_SIZE, PIECE_SIZE, "black"));

        ChessPiece::chessPieceVector.push_back(
            ChessPiece(rend, "Assets/PawnWhite.png", i, WINDOW_HEIGHT - (2 * PIECE_SIZE), PIECE_SIZE, PIECE_SIZE, "white"));
    }

    // Have to do it like this otherwise it causes a segfault for some reason
    for (ChessPiece &piece : ChessPiece::chessPieceVector)
    {
        piece.setType("pawn");
    }

    ChessPiece _piece;

    // Adding rooks
    _piece = ChessPiece(rend, "Assets/RookBlack.png", START, 0, PIECE_SIZE, PIECE_SIZE, "black");
    addPieceToBoard(_piece, "rook");

    _piece = ChessPiece(rend, "Assets/RookBlack.png", START + 7 * PIECE_SIZE, 0, PIECE_SIZE, PIECE_SIZE, "black");
    addPieceToBoard(_piece, "rook");

    _piece = ChessPiece(rend, "Assets/RookWhite.png", START, WINDOW_HEIGHT - PIECE_SIZE, PIECE_SIZE, PIECE_SIZE, "white");
    addPieceToBoard(_piece, "rook");

    _piece = ChessPiece(rend, "Assets/RookWhite.png", START + 7 * PIECE_SIZE, WINDOW_HEIGHT - PIECE_SIZE, PIECE_SIZE, PIECE_SIZE, "white");
    addPieceToBoard(_piece, "rook");

    // Adding Knights
    _piece = ChessPiece(rend, "Assets/KnightBlack.png", START + PIECE_SIZE, 0, PIECE_SIZE, PIECE_SIZE, "black");
    addPieceToBoard(_piece, "knight");

    _piece = ChessPiece(rend, "Assets/KnightBlack.png", START + 6 * PIECE_SIZE, 0, PIECE_SIZE, PIECE_SIZE, "black");
    addPieceToBoard(_piece, "knight");

    _piece = ChessPiece(rend, "Assets/KnightWhite.png", START + PIECE_SIZE, WINDOW_HEIGHT - PIECE_SIZE, PIECE_SIZE, PIECE_SIZE, "white");
    addPieceToBoard(_piece, "knight");

    _piece = ChessPiece(rend, "Assets/KnightWhite.png", START + 6 * PIECE_SIZE, WINDOW_HEIGHT - PIECE_SIZE, PIECE_SIZE, PIECE_SIZE, "white");
    addPieceToBoard(_piece, "knight");

    // Adding bishops
    _piece = ChessPiece(rend, "Assets/BishopBlack.png", START + 2 * PIECE_SIZE, 0, PIECE_SIZE, PIECE_SIZE, "black");
    addPieceToBoard(_piece, "bishop");

    _piece = ChessPiece(rend, "Assets/BishopBlack.png", START + 5 * PIECE_SIZE, 0, PIECE_SIZE, PIECE_SIZE, "black");
    addPieceToBoard(_piece, "bishop");

    _piece = ChessPiece(rend, "Assets/BishopWhite.png", START + 2 * PIECE_SIZE, WINDOW_HEIGHT - PIECE_SIZE, PIECE_SIZE, PIECE_SIZE, "white");
    addPieceToBoard(_piece, "bishop");

    _piece = ChessPiece(rend, "Assets/BishopWhite.png", START + 5 * PIECE_SIZE, WINDOW_HEIGHT - PIECE_SIZE, PIECE_SIZE, PIECE_SIZE, "white");
    addPieceToBoard(_piece, "bishop");

    // Adding queens
    _piece = ChessPiece(rend, "Assets/QueenBlack.png", START + 3 * PIECE_SIZE, 0, PIECE_SIZE, PIECE_SIZE, "black");
    addPieceToBoard(_piece, "queen");

    _piece = ChessPiece(rend, "Assets/QueenWhite.png", START + 3 * PIECE_SIZE, WINDOW_HEIGHT - PIECE_SIZE, PIECE_SIZE, PIECE_SIZE, "white");
    addPieceToBoard(_piece, "queen");

    // Adding kings
    _piece = ChessPiece(rend, "Assets/KingBlack.png", START + 4 * PIECE_SIZE, 0, PIECE_SIZE, PIECE_SIZE, "black");
    addPieceToBoard(_piece, "king");

    _piece = ChessPiece(rend, "Assets/KingWhite.png", START + 4 * PIECE_SIZE, WINDOW_HEIGHT - PIECE_SIZE, PIECE_SIZE, PIECE_SIZE, "white");
    addPieceToBoard(_piece, "king");

    for (ChessPiece &piece : ChessPiece::chessPieceVector)
    {
        piece.setOriginalTile();
    }
}

int ChessPiece::windowWidth;
int ChessPiece::windowHeight;

std::vector<ChessPiece> ChessPiece::chessPieceVector{};
std::vector<std::vector<SDL_Rect>> ChessPiece::chessTiles2d{};
