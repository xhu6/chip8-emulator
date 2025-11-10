#include "cpu.h"

const uint16_t FONT_OFFSET = 0x50;
const uint16_t FONT_SIZE = 0x50;

CPU::CPU(string program) {
  // https://austinmorlan.com/posts/chip8_emulator/
  array<uint8_t, FONT_SIZE> fonts = {
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };

  std::copy(fonts.begin(), fonts.end(), memory.begin() + FONT_OFFSET);
  std::copy(program.begin(), program.end(), memory.begin() + 0x200);
}

void CPU::error() { std::cerr << std::hex << "Warning: Unknown instruction " << op << "at" << pc << std::endl; }

void CPU::calculate() {
  uint16_t tmp;

  switch (N) {
  case 0x0:
    V[X] = V[Y];
    break;

  case 0x1:
    V[X] |= V[Y];
    V[0xF] = 0;
    break;

  case 0x2:
    V[X] &= V[Y];
    V[0xF] = 0;
    break;

  case 0x3:
    V[X] ^= V[Y];
    V[0xF] = 0;
    break;

  case 0x4: {
    tmp = V[X] + V[Y];
    V[X] = tmp & 0xFF;
    V[0xF] = tmp > 255;
  } break;

  case 0x5:
    tmp = V[X] >= V[Y];
    V[X] -= V[Y];
    V[0xF] = tmp;
    break;

  case 0x6:
    tmp = V[X] & 0x1;
    V[X] >>= 1;
    V[0xF] = tmp;
    break;

  case 0x7:
    tmp = V[Y] >= V[X];
    V[X] = V[Y] - V[X];
    V[0xF] = tmp;
    break;

  case 0xE:
    tmp = V[X] >> 7;
    V[X] <<= 1;
    V[0xF] = tmp;
    break;

  default:
    error();
  }
}

void CPU::misc() {
  // Misc statements for the 0xF000 instructions
  switch (NN) {
  case 0x07:
    V[X] = delay_timer;
    break;

  case 0x0A: {
    bool flag = false;

    for (int i = 0; i < 0x10; i++) {
      if (input[i]) {
        V[X] = i;
        flag = true;
        break;
      }
    }

    // Redo same instruction to block
    if (!flag)
      pc -= 2;

    break;
  }

  case 0x15:
    delay_timer = V[X];
    break;

  case 0x18:
    delay_timer = V[X];
    break;

  case 0x1E:
    ir += V[X];
    break;

  case 0x29:
    ir = FONT_OFFSET + (V[X] & 0xF) * 5;
    break;

  case 0x33:
    memory[ir] = V[X] / 100;
    memory[ir + 1] = (V[X] / 10) % 10;
    memory[ir + 2] = V[X] % 10;
    break;

  case 0x55:
    for (int i = 0; i <= X; i++) {
      memory[ir + i] = V[i];
    }
    break;

  case 0x65:
    for (int i = 0; i <= X; i++) {
      V[i] = memory[ir + i];
    }
    break;

  default:
    error();
  }
}

void CPU::fetch_instruction() {
  op = uint16_t(memory[pc]) << 8 | memory[pc + 1];

  NNN = op & 0x0FFF;
  NN = op & 0x00FF;
  N = op & 0x000F;

  X = (op & 0x0F00) >> 8;
  Y = (op & 0x00F0) >> 4;
}

void CPU::run_instruction() {
  switch (op & 0xF000) {
  case 0x0000:
    if (op == 0x00E0) {
      for (auto &i : display) {
        std::fill(i.begin(), i.end(), 0);
      }

    } else if (op == 0x00EE) {
      pc = stack.back();
      stack.pop_back();

    } else {
      stack.push_back(pc + 2);
      pc = NNN;
    }

    break;

  case 0x1000:
    pc = NNN - 2; // Will add 2 later
    break;

  case 0x2000:
    stack.push_back(pc);
    pc = NNN - 2; // Will add 2 later
    break;

  case 0x3000:
    if (V[X] == NN)
      pc += 2;
    break;

  case 0x4000:
    if (V[X] != NN)
      pc += 2;
    break;

  case 0x5000:
    if (N != 0x0)
      error();

    if (V[X] == V[Y])
      pc += 2;
    break;

  case 0x6000:
    V[X] = NN;
    break;

  case 0x7000:
    V[X] += NN;
    break;

  case 0x8000:
    calculate();
    break;

  case 0x9000:
    if (N != 0x0)
      error();

    if (V[X] != V[Y])
      pc += 2;
    break;

  case 0xA000:
    ir = NNN;
    break;

  case 0xB000:
    pc = V[0] + NNN;
    break;

  case 0xC000:
    V[X] = rand() & NN;
    break;

  // https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#dxyn-display
  case 0xD000:
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < 8; j++) {
        uint8_t y = V[Y] % 32 + i;
        uint8_t x = V[X] % 64 + j;

        if (y >= 32) continue;
        if (x >= 64) continue;

        bool value = (memory[ir + i] >> (7 - j)) & 0x1;
        display[y][x] ^= value;
        V[0xF] |= value > display[y][x];
      }
    }

    break;

  case 0xE000:
    switch (NN) {
    case 0x9E:
      if (input[V[X] & 0xF])
        pc += 2;
      break;

    case 0xA1:
      if (!input[V[X] & 0xF])
        pc += 2;
      break;

    default:
      error();
    }

    break;

  case 0xF000:
    misc();
    break;

  default:
    error();
  }
}

void CPU::cycle() {
  fetch_instruction();
  run_instruction();
  pc += 2;

  if (delay_timer > 0)
    delay_timer--;

  if (sound_timer > 0)
    sound_timer--;
}
