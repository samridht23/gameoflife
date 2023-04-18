#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include <vector>

const int window_width = 1000;
const int window_height = 800;
const int grid_size = 10;

bool grid[window_width][window_height];

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

void initPos() {
  grid[20][20] = true;
  grid[20][30] = true;
  grid[20][40] = true;
}

void updateState() {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);
  for (int i = 0; i <= window_width; i += grid_size) {
    for (int j = 0; j <= window_height; j += grid_size) {
      if (grid[i][j]) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect cell = {i, j, grid_size, grid_size};
        SDL_RenderFillRect(renderer, &cell);
      }
    }
  }
  SDL_RenderPresent(renderer);
}

int neighbourCount(int x, int y) {
  int count = 0;
  int rowOffsets[] = {-10, -10, -10, 0, 0, 10, 10, 10};
  int colOffsets[] = {-10, 0, 10, -10, 10, -10, 0, 10};
  for (int i = 0; i < 8; i++) {
    int newRow = x + rowOffsets[i];
    int newCol = y + colOffsets[i];
    // Check if the new row and column are within bounds
    if (newRow >= 0 && newRow < window_width && newCol >= 0 &&
        newCol < window_height) {
      // Check if the neighboring cell is live (true)
      if (grid[newRow][newCol]) {
        count++;
      }
    }
  }
  return count;
}
void calculateState() {
  for (int i = 0; i <= window_width; i += grid_size) {
    for (int j = 0; j <= window_height; j += grid_size) {
      int count = neighbourCount(i, j);
      if (count < 2 && grid[i][j]) {
        grid[i][j] = false;
      } else if ((count == 2 || count == 3) && grid[i][j]) {
        grid[i][j] = true;
      } else if (count > 3 && grid[i][j]) {
        grid[i][j] = false;
      } else if (!grid[i][j] && count == 3) {
        grid[i][j] = true;
      }
    }
  }
}

bool initSDL() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::cout << SDL_GetError() << "\n";
    return false;
  }
  return true;
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
  SDL_Event event;
  bool quit = false;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
      }
    }
    //initPos();
    calculateState();
    updateState();
    SDL_Delay(1000);
  }

  // Code cleanup
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
