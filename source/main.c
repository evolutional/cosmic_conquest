
#include <stdint.h>
#include <stdio.h>

#include "defs.h"
#include "platform.h"
#include "proto.h"

void title_screen() {
  cc_puts(232, 10, "by", GREENPEN);
  ABOX(8, 0, 15, 15);
  EBOX(10, 2, 15, 13);
  ABOX(18, 8, 25, 15);
  EBOX(20, 10, 23, 13);
  ABOX(24, 16, 25, 16);
  ABOX(28, 7, 29, 15);
  ABOX(30, 8, 33, 9);
  ABOX(36, 2, 37, 15);
  ABOX(38, 14, 39, 15);
  ABOX(0, 20, 7, 35);
  EBOX(2, 22, 7, 33);
  ABOX(2, 26, 5, 27);
  ABOX(10, 28, 17, 35);
  EBOX(12, 30, 15, 33);
  ABOX(16, 22, 17, 27);
  ABOX(20, 27, 21, 27);
  ABOX(20, 28, 29, 35);
  EBOX(22, 30, 23, 35);
  EBOX(26, 30, 27, 35);
  ABOX(32, 28, 39, 35);
  EBOX(34, 30, 37, 33);
  ABOX(38, 36, 39, 36);
  ABOX(42, 28, 47, 35);
  EBOX(44, 30, 45, 35);
  ABOX(42, 27, 43, 27);
}

int cc_main(int argc, char *argv[]) {
  platform_init();

  title_screen();
  conquest();

  platform_shutdown();
  return 0;
}