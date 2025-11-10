#pragma once

#include <algorithm>
#include <array>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>

using std::uint8_t, std::uint16_t, std::array, std::string;

class CPU {
  uint16_t ir = 0;
  uint16_t pc = 0x200;

  uint8_t delay_timer = 0;
  uint8_t sound_timer = 0;

  // registers and memory
  array<uint8_t, 16> V = {};
  array<uint8_t, 4096> memory = {};

  // variables for decoding ops
  uint16_t op;
  uint16_t NNN, NN, N;
  uint16_t X, Y;

  std::vector<uint16_t> stack;

  void error();
  void calculate();
  void misc();
  void fetch_instruction();
  void run_instruction();

public:
  array<bool, 16> input = {};
  array<array<bool, 64>, 32> display = {};

  CPU(string program);
  void cycle();
};
