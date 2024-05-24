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
        break;

    case SDL_MOUSEBUTTONDOWN:

        SDL_GetMouseState(&mousePos.x, &mousePos.y);

        if (anyPieceGrabbed)
        {
            ChessPiece &piece = ChessPiece::chessPieceVector.back();

            // Pass in pointer instead of passing in whole vector
            SDL_Rect tile = EventHandler::grabTileUnderCursor(mousePos);

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

                if (index != -1)
                {
                    std::cout << index << std::endl;
                    std::cout << ChessPiece::chessPieceVector[index].team << piece.team << std::endl;
                    if (piece.team == ChessPiece::chessPieceVector[index].team)
                    {
                        std::cout << "Same team" << std::endl;
                        piece.updatePosition(piece.originalTile.x, piece.originalTile.y);
                    }
                    else
                    {
                        std::cout << "Different team" << std::endl;
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
            std::cout << piece.isGrabbed << anyPieceGrabbed << std::endl;
        }

        else
        {
            for (size_t i = 0; i < ChessPiece::chessPieceVector.size(); i++)
            {

                // Use references or piece information doesn't update properly.
                ChessPiece &piece = ChessPiece::chessPieceVector[i];

                if (piece.clickedInRect(&mousePos))
                {
                    std::cout << "Piece picked up / Put Down" << std::endl;

                    piece.isGrabbed = true;
                    piece.originalTile = grabTileUnderCursor(mousePos);
                    anyPieceGrabbed = true;
                    std::cout << piece.team << std::endl;
                    piece.updatePosition(mousePos.x - (piece.boundRect.w / 2), mousePos.y - (piece.boundRect.h / 2));

                    // Draws the currently grabbed piece on top of all the others so collision detection works properly
                    std::iter_swap(ChessPiece::chessPieceVector.begin() + i, ChessPiece::chessPieceVector.end() - 1);

                    break;
                }
            }
        }
        break;

    case SDL_KEYDOWN:
    {
        switch (event.key.keysym.scancode)
        {
        case SDL_SCANCODE_ESCAPE:
            exit = true;
            break;
        }
    }
    }
}
