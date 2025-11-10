cmake -G "Ninja" -D CMAKE_CXX_COMPILER=clang++ -B build -S . && cmake --build build && ./build/chip
