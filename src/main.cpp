#include <SDL3/SDL_main.h>
#include <SDL3/SDL_timer.h>

#include <fstream>
#include <iostream>
#include <iterator>

#include "context.h"
#include "cpu.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " [FILE]" << std::endl;
    return 1;
  }

  std::ifstream file(argv[1]);

  if (!file.good()) {
    std::cout << "File " << argv[1] << " could not be found" << std::endl;
    return 1;
  }

  std::string program(std::istreambuf_iterator<char>{file}, {});

  Context ctx;
  CPU cpu(program);

  while (ctx.is_valid()) {
    ctx.get_input(cpu.input);
    ctx.display(cpu.display);
    cpu.cycle();
  }
}
