#include "context.h"
#include <SDL3/SDL_keycode.h>

const int PIXEL_SIZE = 20;

Context::Context() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw std::runtime_error(SDL_GetError());
  }

  window = SDL_CreateWindow("chip", 64 * PIXEL_SIZE, 32 * PIXEL_SIZE, 0);
  if (window == nullptr) {
    throw std::runtime_error(SDL_GetError());
  }

  renderer = SDL_CreateRenderer(window, NULL);
  if (renderer == nullptr) {
    throw std::runtime_error(SDL_GetError());
  }
}

bool Context::is_valid() { return valid; }

uint8_t to_index(SDL_Keycode key) {
  // 1234       123C
  // qwer   ->  456D
  // asdf       789E
  // zxvc       A0BF

  switch (key) {
  case SDLK_1:
    return 0x1;

  case SDLK_2:
    return 0x2;

  case SDLK_3:
    return 0x3;

  case SDLK_4:
    return 0xC;

  case SDLK_Q:
    return 0x4;

  case SDLK_W:
    return 0x5;

  case SDLK_E:
    return 0x6;

  case SDLK_R:
    return 0xD;

  case SDLK_A:
    return 0x7;

  case SDLK_S:
    return 0x8;

  case SDLK_D:
    return 0x9;

  case SDLK_F:
    return 0xE;

  case SDLK_Z:
    return 0xA;

  case SDLK_X:
    return 0x0;

  case SDLK_C:
    return 0xB;

  case SDLK_V:
    return 0xF;
  }

  return 0xFF;
}

void Context::get_input(array<bool, 16> &input) {
  SDL_Event event;
  // std::fill(input.begin(), input.end(), 0);

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_QUIT:
      valid = false;
      break;

    case SDL_EVENT_KEY_DOWN:
      if (event.key.key == SDLK_ESCAPE) {
        valid = false;
      } else {
        uint8_t index = to_index(event.key.key);
        if (index == 0xFF)
          continue;

        input[index] = true;
      }

      break;

    case SDL_EVENT_KEY_UP: {
      uint8_t index = to_index(event.key.key);
      if (index == 0xFF)
        continue;

      input[index] = false;

    } break;
    }
  }
}

void Context::display(const array<array<bool, 64>, 32> &pixels) {
  // Clear screen to black
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  std::vector<SDL_FRect> rects;

  for (int i = 0; i < pixels.size(); i++) {
    for (int j = 0; j < pixels[i].size(); j++) {
      if (pixels[i][j] == false) {
        continue;
      }

      rects.push_back({float(j * PIXEL_SIZE), float(i * PIXEL_SIZE), PIXEL_SIZE, PIXEL_SIZE});
    }
  }

  // White
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRects(renderer, rects.data(), rects.size());
  SDL_RenderPresent(renderer);
}

Context::~Context() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
