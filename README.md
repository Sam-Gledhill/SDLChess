# SDLChess [WIP]

Mostly finished chess game - written in C++ using SDL2.

![image](https://github.com/user-attachments/assets/ea0f7c4c-7d94-49a4-a776-5bc30a168036)





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
