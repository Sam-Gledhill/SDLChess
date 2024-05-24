#include "EventHandling.h"
#include "ChessPieces.h"

EventHandler::EventHandler()
{
    anyPieceGrabbed = false;
    exit = false;
}

SDL_Rect EventHandler::grabTileUnderCursor(SDL_Point mousePosition)
{
    for (auto &column : ChessPiece::chessTiles2d)
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

void EventHandler::handleEvent(SDL_Event &event)
{

    // todo :: abstract the separate parts of this into private functions
    switch (event.type)
    {

    case SDL_QUIT:
        exit = true;
        break;

    case SDL_MOUSEMOTION:
        EventHandler::handleMouseMotion();
        break;

    case SDL_MOUSEBUTTONDOWN:
        EventHandler::handleMouseButtonDown();
        break;

    case SDL_KEYDOWN:
    {
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
        {
            exit = true;
        }
    }
    }
}

void EventHandler::handleMouseButtonDown()
{
    SDL_GetMouseState(&mousePos.x, &mousePos.y);

    if (anyPieceGrabbed)
    {
        // Grab final piece in stack. Would've been put there to be drawn on top.
        ChessPiece &piece = ChessPiece::chessPieceVector.back();

        // Pass in pointer instead of passing in whole vector
        SDL_Rect tile = grabTileUnderCursor(mousePos);

        // If piece is dragged somewhere that is not a tile.
        if (SDL_RectEmpty(&tile))
        {
            piece.updatePosition(piece.originalTile.x, piece.originalTile.y);
        }

        else
        {
            piece.updatePosition(tile.x, tile.y);
            // Bit of a janky way to do it but it works
            size_t index = piece.collidingWithOtherPiece(ChessPiece::chessPieceVector, ChessPiece::chessPieceVector.size() - 1);

            int MULT = 1 + piece.firstTurn;

            std::cout << MULT << "" << piece.firstTurn << std::endl;

            bool validBlackY = tile.y == (MULT * 50) + piece.originalTile.y || tile.y == 1 * 50 + piece.originalTile.y;
            bool validWhiteY = tile.y == (-MULT * 50) + piece.originalTile.y || tile.y == -1 * 50 + piece.originalTile.y;

            if (piece.team == "black" && !(validBlackY && tile.x == piece.originalTile.x))
            {
                piece.updatePosition(piece.originalTile.x, piece.originalTile.y);
            }

            else if (piece.team == "white" && (!validWhiteY && tile.x == piece.originalTile.x))
            {
                piece.updatePosition(piece.originalTile.x, piece.originalTile.y);
            }

            else
            {
                piece.firstTurn = false;
            }

            if (index != -1)
            {
                if (piece.team == ChessPiece::chessPieceVector[index].team)
                {
                    std::cout << "Invalid move" << std::endl;
                    piece.updatePosition(piece.originalTile.x, piece.originalTile.y);
                }
                else
                {
                    std::cout << "Piece Captured" << std::endl;
                    piece.updatePosition(tile.x, tile.y);
                    ChessPiece::chessPieceVector.erase(ChessPiece::chessPieceVector.begin() + index);
                    piece.originalTile = tile;
                }
            }
        }

        // Bandaid fix for multiple pieces having .isGrabbed true, find out why.
        for (ChessPiece &piece : ChessPiece::chessPieceVector)
        {
            piece.isGrabbed = false;
        }
        anyPieceGrabbed = false;
    }

    else
    {
        for (size_t i = 0; i < ChessPiece::chessPieceVector.size(); i++)
        {

            // Use references or piece information doesn't update properly.
            ChessPiece &piece = ChessPiece::chessPieceVector[i];

            if (piece.clickedInRect(&mousePos))
            {
                piece.isGrabbed = true;
                piece.originalTile = grabTileUnderCursor(mousePos);
                anyPieceGrabbed = true;
                piece.updatePosition(mousePos.x - (piece.boundRect.w / 2), mousePos.y - (piece.boundRect.h / 2));

                // Draws the currently grabbed piece on top of all the others so collision detection works properly
                std::iter_swap(ChessPiece::chessPieceVector.begin() + i, ChessPiece::chessPieceVector.end() - 1);

                break;
            }
        }
    }
}

void EventHandler::handleMouseMotion()
{
    if (!anyPieceGrabbed)
    {
        return;
    }

    for (ChessPiece &piece : ChessPiece::chessPieceVector)
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
}