#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

int main(int argc, char *argv[])
{
    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window *win = SDL_CreateWindow("GAME", // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       600, 400, 0);

    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;

    // creates a renderer to render our images
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);

    ///
    /// Section 2: SDL image loader
    ///
    SDL_Surface *surface;
    surface = IMG_Load("capybara.png");

    SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surface);

    SDL_FreeSurface(surface); // clear surface texture?

    SDL_Rect dest;
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h); // connects dest with texture

    // dest.w /= 6;
    // dest.h /= 6;

    dest.x = (600 - dest.w) / 2;
    dest.y = (400 - dest.h) / 2;

    ///
    /// Section 4: SDL ttf and rendering text
    ///

    ///
    /// Section 3: Game Loop and Basic Controls
    ///

    // We add a delay in order to see that our window
    // has successfully popped up.

    bool exit = false;

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
