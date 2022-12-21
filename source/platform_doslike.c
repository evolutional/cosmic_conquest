#define DOS_IMPLEMENTATION
#include "dos.h"

#include "ZX Eurostile.h"

#include "defs.h"
#include "platform.h"
#include "proto.h"

extern int fline;
extern int screen1;
extern int screen2;

/* Amiga Sprite Info from original game*/
static unsigned short cc_pointer_gfx[] = {
    0x0000, 0x0000, 0x8080, 0x0000, 0x4100, 0x0000, 0x2200, 0x0000,
    0x1400, 0x0000, 0x0000, 0x0000, 0x1400, 0x0000, 0x2200, 0x0000,
    0x4100, 0x0000, 0x8080, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};

struct {
  unsigned char *pointer_spr;
} _cc_doslike = {0};

void SetAPen(int color) { setcolor(color); }

#define _BOX(x1, y1, x2, y2)                                                   \
  {                                                                            \
    int p[] = {10 * x1,      5 * y1 + 5,  10 * x2 + 10, 5 * y1 + 5,            \
               10 * x2 + 10, 5 * y2 + 10, 10 * x1,      5 * y2 + 10};          \
    fillpoly(p, 4);                                                            \
  }

void ABOX(x1, y1, x2, y2) {
  SetAPen(GREENPEN);
  RectFill((long)10 * x1, (long)5 * y1 + 5, (long)10 * x2 + 9,
           (long)5 * y2 + 9);
}
void EBOX(x1, y1, x2, y2) {
  SetAPen(BLACKPEN);
  RectFill((long)10 * x1, (long)5 * y1 + 5, (long)10 * x2 + 9,
           (long)5 * y2 + 9);
}

void RectFill(x1, y1, x2, y2) {
  int p[] = {x1, y1, x2 + 1, y1, x2 + 1, y2 + 1, x1, y2 + 1};
  fillpoly(p, 4);
}

void cc_putc_ex(int x, int y, char c, int color, int drawmode) {
  c -= 32;
  const uint8_t *chptr = &FONT_ZX_EUROSTILE_BITMAP[c * 8];
  uint8_t *scr = screenbuffer() + (y * screenwidth()) + x;
  uint8_t fg = (uint8_t)color;
  uint8_t bg = (uint8_t)BLACKPEN;
  if (drawmode & INVERSVID) {
    uint8_t t = fg;
    fg = bg;
    bg = t;
  }

  for (int row = 0; row < 8; row++) {
    uint8_t ch = *chptr++;
    uint8_t col[8] = {
        ch & 0b10000000 ? fg : bg, ch & 0b1000000 ? fg : bg,
        ch & 0b100000 ? fg : bg,   ch & 0b10000 ? fg : bg,
        ch & 0b1000 ? fg : bg,     ch & 0b100 ? fg : bg,
        ch & 0b10 ? fg : bg,       ch & 0b1 ? fg : bg,
    };
    memcpy(scr, col, 8);
    scr += screenwidth();
  }
}

void cc_putc(int x, int y, char c, int color) { cc_putc_ex(x, y, c, color, 0); }

void cc_puts_ex(int x, int y, const char *c, int color, int drawmode) {
  if (!c)
    return;
  while (*c) {
    cc_putc_ex(x, y, *c, color, drawmode);
    c++;
    x += 8;
  }
}

void cc_puts(int x, int y, const char *str, int color) {
  cc_puts_ex(x, y, str, color, 0);
}

void PrintIText(struct IntuiText *dt, int x, int y) {
  if (!dt)
    return;
  if (dt->IText)
    cc_puts_ex(x, y, dt->IText, dt->FrontPen, dt->DrawMode);
}

size_t IntuiTextLength(struct IntuiText *dt) {
  if (!dt)
    return 0;
  if (dt->IText)
    return strlen(dt->IText) * 8;
  return 0;
}

int clearall(int pla) {
  int i, j;
  if (!display_check(pla))
    return 0;
  fline = 0;
  setcolor(BLACKPEN);
  clearscreen();
  for (i = 4; i < XMAX * 16 + 4; i += 16)
    for (j = 4; j < YMAX * 8 + 4; j += 8)
      putpixel(i, j, WHITEPEN);
  screen1 = CLEAR;
  screen2 = CLEAR;
  return 0;
}

int _cc_keyget() {
  int back = 0;
  int ret = 0;

  enum keycode_t *keys = readkeys();
  while (*keys) {
    enum keycode_t k = *keys;

    if (k == KEY_RETURN) {
      ret = 1;
    } else if (k == KEY_BACK) {
      back = 1;
    }

    ++keys;
  }

  if (back)
    return '\b';

  if (ret)
    return 0x0d;

  const char *c = readchars();
  if (*c && *c != '\r' && *c != '\n') {
    return *c;
  }
  return 0;
}

int cget(int pla) {
  while (1) {
    waitvbl();

    int c = _cc_keyget();
    if (c) {
      return c;
    }
    if (keystate(KEY_LBUTTON)) {
      process_mouse(pla, (int)mousex() / 8, (int)mousey() / 8);
    }
    if (keystate(KEY_RBUTTON)) {
      showstatus(pla);
    }
  }
  return 0;
}

int coget() {
  while (1) {
    waitvbl();

    int c = _cc_keyget();
    if (c) {
      return c;
    }
  }
  return 0;
}

void setpal16(int color, int r, int g, int b) {
  setpal(color, r * 4, g * 4, b * 4);
}

void platform_init() {
  setvideomode(videomode_640x200);
  // set palette
  setpal16(BLACKPEN, 0L, 0L, 0L);        /* Black  */
  setpal16(WHITEPEN, 15L, 15L, 15L);     /* White  */
  setpal16(GREENPEN, 0L, 9L, 0L);        /* Green  */
  setpal16(63, 0L, 9L, 0L);              /* Green  */
  setpal16(OTHERPEN, 15L, 12L, 0L);      /* Orange */
  setpal16(PLAYERPEN(0), 15L, 2L, 0L);   /* Red    */
  setpal16(PLAYERPEN(1), 7L, 9L, 15L);   /* Blue   */
  setpal16(PLAYERPEN(2), 15L, 15L, 2L);  /* Yellow */
  setpal16(PLAYERPEN(3), 0L, 15L, 2L);   /* Full Green */
  setpal16(SPRITEPEN(0), 0L, 0L, 0L);    /* Black  */
  setpal16(SPRITEPEN(1), 5L, 13L, 0L);   /* Green  */
  setpal16(SPRITEPEN(2), 15L, 15L, 15L); /* White  */
  setpal16(SPRITEPEN(3), 15L, 12L, 0L);  /* Orange */

  _cc_doslike.pointer_spr = malloc(16 * 16);
  unsigned char *dst = _cc_doslike.pointer_spr;
  const unsigned short *ptr = cc_pointer_gfx;
  for (int y = 0; y < 16; y++) {
    unsigned short a = *ptr++; // bp0
    unsigned short b = *ptr++; // bp1

    for (int x = 0; x < 16; x++) {
      unsigned char c = ((a >> (7 - x)) & 0x1) | ((b >> (7 - x)) & 0x1);
      *dst = c;
      ++dst;
    }
  }
}

void cc_draw_mouse() {
  if (_cc_doslike.pointer_spr) {
    maskblit(mousex(), mousey(), _cc_doslike.pointer_spr, 16, 16, 0, 0, 16, 16,
             0);
  }
}

void platform_shutdown() { free(_cc_doslike.pointer_spr); }

int main(int argc, char *argv[]) { return cc_main(argc, argv); }