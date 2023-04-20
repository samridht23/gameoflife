#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

const int window_width = 600;
const int window_height = 400;
const int cell_size = 6;

bool grid[window_width][window_height];
int neighbourCount(int x, int y) {
  int count = 0;
  int rowOffsets[] = {-cell_size, -cell_size, -cell_size, 0,
                      0,          cell_size,  cell_size,  cell_size};
  int colOffsets[] = {-cell_size, 0,          cell_size, -cell_size,
                      cell_size,  -cell_size, 0,         cell_size};
  for (int i = 0; i < 8; i++) {
    int row = x + rowOffsets[i];
    int col = y + colOffsets[i];
    if (row >= 0 && row < window_width && col >= 0 && col < window_height) {
      if (grid[row][col]) {
        count += 1;
      }
    }
  }
  return count;
}

void calculateState() {
  bool tempGrid[window_width][window_height];
  for (int i = 0; i < window_width; i += cell_size) {
    for (int j = 0; j < window_height; j += cell_size) {
      int count = neighbourCount(i, j);
      if (grid[i][j]) {
        if (count < 2)
          tempGrid[i][j] = false;
        else if (count == 2 || count == 3)
          tempGrid[i][j] = grid[i][j];
        else if (count > 3)
          tempGrid[i][j] = false;
      } else if (!grid[i][j]) {
        if (count == 3)
          tempGrid[i][j] = true;
      }
    }
  }
  for (int i = 0; i < window_width; i += cell_size) {
    for (int j = 0; j < window_height; j += cell_size) {
      grid[i][j] = tempGrid[i][j];
    }
  }
}

void seed(int cellNum) {
  for (int i = 0; i < cellNum; i++) {
    int max_row = window_width / cell_size;
    int rand_row = (rand() % max_row) * cell_size;
    int max_col = window_height / cell_size;
    int rand_col = (rand() % max_col) * cell_size;
    grid[rand_row][rand_col] = true;
  }
}

void updateState(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  for (int i = 0; i < window_width; i += cell_size) {
    for (int j = 0; j < window_height; j += cell_size) {
      if (grid[i][j]) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_Rect cell = {i, j, cell_size - 1, cell_size - 1};
        SDL_RenderFillRect(renderer, &cell);
      }
    }
  }
  SDL_RenderPresent(renderer);
}

int main() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::cout << SDL_GetError() << std::endl;
    return 1;
  }
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, window_width, window_height,
                            SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    std::cout << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr) {
    std::cout << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 1;
  }
  seed(1000);
  SDL_Event event;
  bool quit = false;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
      }
    }
    updateState(renderer);
    calculateState();
    SDL_Delay(50);
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
