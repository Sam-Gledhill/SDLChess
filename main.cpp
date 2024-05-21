#include <iostream>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>

int main(int argc, char *argv[])
{
    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    int WINDOW_WIDTH = 600, WINDOW_HEIGHT = 400;

    SDL_Window *win = SDL_CreateWindow("GAME", // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;

    int SDL_CAPTUREMouse(SDL_ENABLE);

    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);

    SDL_Surface *surface;
    surface = IMG_Load("capybara.png");
    SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface); // clear surface texture

    SDL_Rect dest;
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h); // connects dest with texture

    dest.x = 0;
    dest.y = 0;

    dest.w = 50;
    dest.h = 50;

    bool exit = false;

    SDL_Point mousePos;

    bool pieceCaptured = false;

    while (!exit)
    {

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {

            case SDL_QUIT:
                exit = true;
                break;

            case SDL_MOUSEMOTION:
                if (!pieceCaptured)
                {
                    break;
                }

                SDL_GetMouseState(&mousePos.x, &mousePos.y);

                dest.x = mousePos.x - (dest.w / 2);
                dest.y = mousePos.y - (dest.h / 2);
                break;

            case SDL_MOUSEBUTTONDOWN:

                SDL_GetMouseState(&mousePos.x, &mousePos.y);

                if (SDL_PointInRect(&mousePos, &dest))
                {
                    std::cout << "Point in rect" << std::endl;
                    pieceCaptured = !pieceCaptured;
                    dest.x = mousePos.x - (dest.w / 2);
                    dest.y = mousePos.y - (dest.h / 2);
                }

                break;

            case SDL_KEYDOWN:

                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:
                    exit = true;
                    break;

                case SDL_SCANCODE_W:
                    dest.y -= 5;
                    break;

                case SDL_SCANCODE_S:
                    dest.y += 5;
                    break;

                case SDL_SCANCODE_A:
                    dest.x -= 5;
                    break;

                case SDL_SCANCODE_D:
                    dest.x += 5;
                    break;
                }
            }
        }

        // clears the screen
        SDL_RenderClear(rend);
        SDL_RenderCopy(rend, tex, NULL, &dest);

        // triggers the double buffers
        // for multiple rendering
        SDL_RenderPresent(rend);
    }

    ///
    /// Section 5: Freeing resources
    ///

    // We destroy our window. We are passing in the pointer
    // that points to the memory allocated by the
    // 'SDL_CreateWindow' function. Remember, this is
    // a 'C-style' API, we don't have destructors.
    SDL_DestroyWindow(win);

    // We safely uninitialize SDL2, that is, we are
    // taking down the subsystems here before we exit
    // our program.
    SDL_Quit();
}
