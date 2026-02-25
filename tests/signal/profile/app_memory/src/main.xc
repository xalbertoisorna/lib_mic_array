// Copyright 2022-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>

#include <xs1.h>
#include <platform.h>

extern "C"{
  int main_tile_0();
  int main_tile_1();
}

int main() {
  par {
    on tile[0]: main_tile_0();
    on tile[1]: main_tile_1();
  }
  return 0;
}
