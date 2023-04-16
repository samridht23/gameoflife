#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <iostream>

const Uint32 window_width = 1000;
const Uint32 window_height = 800;
const Uint32 grid_size = 100;
int rect_x = window_width / 2;
int rect_y = window_height / 2;

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
  for (int i = 0; i <= window_width; i += grid_size) {
    SDL_RenderDrawLine(renderer, i, 0, i, window_height);
  }
  for (int i = 0; i <= window_height; i += grid_size) {
    SDL_RenderDrawLine(renderer, 0, i, window_width, i);
  }
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_Rect cell = {rect_x, rect_y, grid_size, grid_size};
  SDL_RenderFillRect(renderer, &cell);
  // Update the screen with any rendering performed since the previous call
  SDL_RenderPresent(renderer);
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
  while (SDL_WaitEvent(&event)) {
    if (event.type == SDL_QUIT) {
      break;
    }
    drawGrid();
    if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
      case SDLK_w:
        rect_y -= grid_size;
        break;
      case SDLK_s:
        rect_y += grid_size;
        break;
      case SDLK_a:
        rect_x -= grid_size;
        break;
      case SDLK_d:
        rect_x += grid_size;
        break;
      default:
        break;
      }
    }
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
