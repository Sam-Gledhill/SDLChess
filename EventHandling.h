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
    SDL_Rect grabTileUnderCursor(SDL_Point mousePosition);
    void handleMouseMotion();
    void handleEvent(SDL_Event &event);
    void handleMouseButtonDown();
    void handlePutDownPiece();
    void handlePickUpPiece();
};

#endif