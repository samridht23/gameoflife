#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <iostream>

const Uint32 window_width = 1000;
const Uint32 window_height = 800;
const Uint32 grid_size = 10;

bool grid[window_width][window_height];

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

bool initSDL() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::cout << SDL_GetError() << "\n";
    return false;
  }
  return true;
}

void drawGrid() {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  for (int x = 0; x <= window_width; x+=grid_size) {
    for (int y = 0; y <= window_height; y+=grid_size) {
      if (grid[x][y] == true) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect cell = {x, y, grid_size, grid_size};
        SDL_RenderFillRect(renderer, &cell);
      }
    }
  }
  SDL_RenderPresent(renderer);
}
// initial state
void initPos() {
  grid[60][50] = true;
  grid[60][60] = true;
  grid[60][70] = true;
  grid[10][10] = true;
}

// do bound checking
int countNeighbour(int x, int y) {
  int count = 0;
  if (grid[x - 1][y - 1] == true) {
    count += 1;
  } else if (grid[x][y - 1] == true) {
    count += 1;
  } else if (grid[x + 1][y - 1] == true) {
    count += 1;
  } else if (grid[x - 1][y] == true) {
    count += 1;
  } else if (grid[x + 1][y] == true) {
    count += 1;
  } else if (grid[x - 1][y + 1] == true) {
    count += 1;
  } else if (grid[x][y - 1] == true) {
    count += 1;
  } else if (grid[x + 1][y + 1] == true) {
    count += 1;
  }
  return count;
}

void generateCells() {
  for (int i = 0; i <= window_width; i += grid_size) {
    for (int j = 0; j <= window_height; j += grid_size) {
      int x = i / 10 - 1;
      int y = j / 10 - 1;
      if (countNeighbour(x, y) < 2) {
        grid[x][y] = false;
      } else if (countNeighbour(x, y) == 2 || countNeighbour(x, y) == 3) {
        grid[y][y] = true;
      } else if (countNeighbour(x, y) > 3) {
        grid[x][y] = false;
      } else if (countNeighbour(x, y) == 3 && grid[x][y] == false)
        grid[x][y] = true;
    }
  }
}

int main(int argc, char *argv[]) {
  // SDL Init
  if (!initSDL())
    return 1;

  // Window Init
  window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, window_width, window_height,
                            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if (window == NULL) {
    std::cout << SDL_GetError() << "\n";
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }
  // Renderer Init
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    std::cout << SDL_GetError() << "\n";
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  // Main event loop
  // fix this even loop use two while implementation in docs
  SDL_Event event;
  bool quit = false;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
      } else if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
          int windowWidth, windowHeight;
          SDL_GetWindowSize(window, &windowWidth, &windowHeight);
          printf("Window size changed to: %d x %d\n", windowWidth,
                 windowHeight);
        }
      }
    }
    initPos();
    //generateCells();
    drawGrid();
    SDL_Delay(1000);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
