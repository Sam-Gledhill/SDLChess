#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

#ifndef EventHandling
#define EventHandling

class EventHandler
{
public:
    bool exit;
    bool anyPieceGrabbed;
    SDL_Point mousePos;
    EventHandler();
    SDL_Rect getTileUnderCursor(SDL_Point mousePosition);
    bool checkValidPathToDest(SDL_Rect originalTile, SDL_Rect destinationTile);
    void handleMouseMotion();
    void handleEvent(SDL_Event &event);
    void handleMouseButtonDown();
    void handlePutDownPiece();
    void handlePickUpPiece();
};

#endif