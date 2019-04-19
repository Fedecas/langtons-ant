#include <SDL.h>
#include <stdio.h>

#include "matrix.h"

#define WIDTH   1280                // in pixels
#define HEIGHT  720                 // in pixels
#define TITLE   "Langton's ant"

#define FRAMES_PER_SECOND   30
#define DELAY               (1000 / FRAMES_PER_SECOND)

#define STEPS_PER_FRAME     50

#define CELL_SIZE    5                      // cell size in pixels
#define CELL_COUNTX  (WIDTH / CELL_SIZE)    // width of grid of cells
#define CELL_COUNTY  (HEIGHT / CELL_SIZE)   // height of grid of cells

#define UP      0
#define LEFT    1
#define DOWN    2
#define RIGHT   3

SDL_Window* WindowCreate(int width, int height, char* title)
{
    // Initialize SDL with video resources
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        printf("[error] SDL video subsystem could not initialize! SDL_Error: %s\n", SDL_GetError());

    // Create SDL window
    SDL_Window* context = SDL_CreateWindow(title,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height, SDL_WINDOW_SHOWN);

    if (context == NULL)
        printf("[error] Window could not be created! SDL_Error: %s\n", SDL_GetError());

    return context;
}

SDL_Surface* SurfaceCreate(SDL_Window* context)
{
    // Get the surface of the current window
    SDL_Surface* surface = SDL_GetWindowSurface(context);

    if (surface == NULL)
        printf("[error] Surface could not be created! SDL_Error: %s\n", SDL_GetError());

    return surface;
}

void WindowUpdate(SDL_Window* context)
{
    // Update the pixels surface
    if (SDL_UpdateWindowSurface(context) == -1)
        printf("[error] Surface could not be updated! SDL_Error: %s\n", SDL_GetError());
}

void Paint(SDL_Surface* surface, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
    // Paint the (x, y) cell
    SDL_Rect cell = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
    SDL_FillRect(surface, &cell, SDL_MapRGB(surface->format, r, g, b));
}

void Draw(SDL_Surface* surface, int** grid, int rows, int columns)
{
    // Paint each cell of the grid
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            Uint8 color = grid[i][j] * 0xFF;

            Paint(surface, i, j, color, color, color);
        }
    }
}

int InputEvent()
{
    SDL_Event event;
    int quit = 0;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: quit = 1; break; // if close the window
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: quit = 1; break; // if press escape key
                    default: break;
                } break;
                default: break;
        }
    }

    return quit;
}

int ant_move(int* ant_x, int* ant_y, int direction)
{
    int edge = 0;

    switch (direction) {
        case UP: (*ant_y)--; break;
        case LEFT: (*ant_x)--; break;
        case DOWN: (*ant_y)++; break;
        case RIGHT: (*ant_x)++; break;
        default: break;
    }

    if (*ant_x == 0 || *ant_x == CELL_COUNTX - 1 ||
        *ant_y == 0 || *ant_y == CELL_COUNTY - 1) {

        edge = 1;
    }

    return edge;
}

int ant_react(int** grid, int* ant_x, int* ant_y, int* direction)
{
    if (grid[*ant_x][*ant_y] == 0) {
        grid[*ant_x][*ant_y] = 1;
        (*direction)--;
    }
    else {
        grid[*ant_x][*ant_y] = 0;
        (*direction)++;
    }

    if (*direction < 0) {
        *direction = 3;
    }
    else if (*direction > 3) {
        *direction = 0;
    }

    return ant_move(ant_x, ant_y, *direction);
}

int main(int argc, char *argv[])
{
    SDL_Window* context = WindowCreate(WIDTH, HEIGHT, TITLE);
    SDL_Surface* surface = SurfaceCreate(context);

    int** grid = create_matrix(CELL_COUNTX, CELL_COUNTY);

    int ant_x = CELL_COUNTX / 2;
    int ant_y = CELL_COUNTY / 2;

    int direction = 0;

    int quit = 0;
    int edge = 0;

    int steps_count = 0;
    char title[32];

    while(!quit) { 
        quit = InputEvent();
        
        if (!edge) {
            for (int i = 0; i < STEPS_PER_FRAME; i++) {
                edge = ant_react(grid, &ant_x, &ant_y, &direction);

                steps_count++;

                if (edge) break;
            }

            Draw(surface, grid, CELL_COUNTX, CELL_COUNTY);

            Paint(surface, ant_x, ant_y, 0xFF, 0, 0); // paint the ant in red

            WindowUpdate(context);

            sprintf_s(title, 32, "%s | %d steps", TITLE, steps_count);
            SDL_SetWindowTitle(context, title);
        }

        SDL_Delay(DELAY);
    }

    grid = destroy_matrix(grid, CELL_COUNTX, CELL_COUNTY);

    SDL_FreeSurface(surface);
    SDL_DestroyWindow(context);
    SDL_Quit();

    return 0;
}
