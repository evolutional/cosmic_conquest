#ifndef _cc_platform_h_
#define _cc_platform_h_

/* platform */

#define INVERSVID 1
#define JAM1 2
#define JAM2 4

struct IntuiText {
  unsigned char FrontPen, BackPen;
  unsigned char DrawMode;
  char IText[128];
};

void SetAPen(int color);
void RectFill(int x1, int y1, int x2, int y2);
void ABOX(int x1, int y1, int x2, int y2);
void EBOX(int x1, int y1, int x2, int y2);
void PrintIText(struct IntuiText *dt, int x, int y);
size_t IntuiTextLength(struct IntuiText *dt);

void cc_draw_mouse();
void cc_puts(int x, int y, const char *str, int color);

void platform_init();
void platform_shutdown();

int cc_main(int argc, char *argv[]);

#endif