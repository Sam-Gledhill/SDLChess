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

    if (anyPieceGrabbed)
    {
        handlePutDownPiece();
            }

    else if (!anyPieceGrabbed)
    {
        handlePickUpPiece();
    }
}

void EventHandler::handlePutDownPiece(){
    SDL_GetMouseState(&mousePos.x, &mousePos.y);
    // Grab final piece in stack. Would've been put there to be drawn on top.
    ChessPiece &piece = ChessPiece::chessPieceVector.back();

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

        //Index >=0 when piece collision found
        if (index != -1)
        {
            // Don't allow capturing of own pieces
            if (piece.team == ChessPiece::chessPieceVector[index].team)
            {
                // The piece being placed in its original tile is considered an invalid move
                piece.updatePosition(piece.originalTile.x, piece.originalTile.y);
            }

            else
            {
                piece.attacking = true;
            }
        }

        if (piece.moveValid(piece, tile))
        {
            bool validPath = false;

            if (piece.type == "knight"){
                validPath = true;
            }
            else{
                validPath = checkValidPathToDest(piece.originalTile,tile);
            } 
            

            if (!validPath){
                piece.attacking = false;
                piece.updatePosition(piece.originalTile.x, piece.originalTile.y);
            }

            else{
                piece.firstTurn = false;
                piece.originalTile = tile;
            }
            
            if (piece.attacking)
            {
                piece.attacking = false;
                ChessPiece::chessPieceVector.erase(ChessPiece::chessPieceVector.begin() + index);
            }
        }
        else
        {
            piece.updatePosition(piece.originalTile.x, piece.originalTile.y);
        }
    }

    // Bandaid fix for multiple pieces having .isGrabbed true, find out why.
    for (ChessPiece &piece : ChessPiece::chessPieceVector)
    {
        piece.isGrabbed = false;
    }
    anyPieceGrabbed = false;

}

bool EventHandler::checkValidPathToDest(SDL_Rect originalTile, SDL_Rect destinationTile){
   
    int dx = destinationTile.x - originalTile.x;
    int dy = destinationTile.y - originalTile.y;

    int dxSign = dx/abs(dx);
    int dySign = dy/abs(dy);

    int TILE_SIZE = 50;

    SDL_Point p;

    //Checks the space between original piece position and desired destination. Sets validPath to true if a piece has been jumped over.
    //Gives invalid move if moving > 1 tile behind a friendly piece
    for(int i=1; i< std::max(abs(dx)/TILE_SIZE,abs(dy)/TILE_SIZE); i++){

        p.x = originalTile.x + dxSign*(TILE_SIZE*i);
        p.y = originalTile.y + dySign*(TILE_SIZE*i);

        for(ChessPiece& otherPiece: ChessPiece::chessPieceVector){
            if(otherPiece.clickedInRect(&p)){
                return false;
            }
        }
    }

    return true;

}

void EventHandler::handlePickUpPiece(){
    SDL_GetMouseState(&mousePos.x, &mousePos.y);
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