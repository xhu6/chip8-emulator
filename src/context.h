#pragma once

#include <algorithm>
#include <array>
#include <stdexcept>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

using std::array;

class Context {
private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  bool valid = true;

public:
  Context();

  // Disable copy because of destructor
  Context(const Context &) = delete;

  bool is_valid();

  void get_input(array<bool, 16> &input);
  void display(const array<array<bool, 64>, 32> &pixels);

  ~Context();
};
