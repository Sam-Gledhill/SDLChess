# SDLChess [WIP]

Mostly finished chess game - written in C++ using SDL2.

![Demo Image](https://github.com/user-attachments/assets/a7714546-bae6-4f80-8f20-fff87b71cd31)



To do:
*   Fix bug where moves are being falsely invalidated when moving multiple tiles near pawns.
*   Detect check/checkmate situations.
*   Limit FPS.
*   Add turns - alternate black and white moves.
*   Add UI - timer, resign and turn indicators.

To compile:

```
g++ -fdiagnostics-color=always -g main.cpp ChessPieces.cpp EventHandling.cpp -o main -lSDL2 -lSDL2_image
```
