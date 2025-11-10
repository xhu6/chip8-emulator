#include <SDL3/SDL_main.h>
#include <SDL3/SDL_timer.h>

#include <fstream>
#include <iterator>
#include <iostream>

#include "context.h"
#include "cpu.h"

int main(int argc, char *argv[]) {
  std::ifstream file("./games/pong.rom");
  std::string program(std::istreambuf_iterator<char>{file}, {});

  std::cout << program.size() << std::endl;

  Context ctx;
  CPU cpu(program);

  while (ctx.is_valid()) {
    ctx.get_input(cpu.input);
    ctx.display(cpu.display);
    cpu.cycle();
  }
}
