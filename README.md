# SDLChess [WIP]

Mostly finished chess game - written in C++ using SDL2.

To do:
*   Detect check/checkmate situations.
*   Limit FPS.
*   Add turns - alternate black and white moves.
*   Add UI - timer, resign and turn indicators.

To compile:

```
%g++ -fdiagnostics-color=always -g main.cpp ChessPieces.cpp EventHandling.cpp -o main -lSDL2 -lSDL2_image
```
