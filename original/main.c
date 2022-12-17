#include <functions.h>
#include <exec/memory.h>
#include <graphics/gfxbase.h>
#include <graphics/rastport.h>
#include <graphics/text.h>
#include <intuition/intuitionbase.h>
#include <stdio.h>
#include <ctype.h>
#include "defs.h"

#define ABOX(x1,y1,x2,y2) SetAPen(ConPort,GREENPEN);RectFill(ConPort,(long) 10*x1,(long) 5*y1+5,(long) 10*x2+9,(long) 5*y2+9)
#define EBOX(x1,y1,x2,y2) SetAPen(ConPort,BLACKPEN);RectFill(ConPort,(long) 10*x1,(long) 5*y1+5,(long) 10*x2+9,(long) 5*y2+9)

struct NewScreen NScreen =
  {
  0,0,640,200,
  3,
  GREENPEN,WHITEPEN,
  HIRES,
  CUSTOMSCREEN,
  NULL,
  (UBYTE *) "Cosmic Conquest by (PD) Carl Edman",
  NULL,
  NULL
  };

struct NewWindow NWindow =
  {
  0,0,
  640,200,
  GREENPEN,WHITEPEN,
  VANILLAKEY|MOUSEBUTTONS,
  BORDERLESS|BACKDROP|SIMPLE_REFRESH|NOCAREREFRESH|RMBTRAP|ACTIVATE,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  640,200,
  640,200,
  CUSTOMSCREEN
  };

struct TextAttr NFont =
  {
  (STRPTR) "ruby.font",
  (UWORD) 8,
  (UBYTE) FS_NORMAL,
  (UBYTE) FPF_DISKFONT
  };

USHORT CrossPointer[] =
  {
  0x0000,0x0000,
  0x8080,0x0000,
  0x4100,0x0000,
  0x2200,0x0000,
  0x1400,0x0000,
  0x0000,0x0000,
  0x1400,0x0000,
  0x2200,0x0000,
  0x4100,0x0000,
  0x8080,0x0000,
  0x0000,0x0000,
  0x0000,0x0000
  };

struct NStar star[STARNO];
struct NTask task[PLAYERNO][TASKNO+1];
struct NCommData CommData[PLAYERNO];
struct NGeneral general;
struct Screen *ConScreen=NULL;
struct Window *ConWindow=NULL;
struct IntuitionBase *IntuitionBase;
struct GfxBase *GfxBase;
struct RastPort *ConPort;

int growth[PLANETNO]={3,5,7,8,7,6,4,2};
int fline;
int mode=-1,mode2=0,mode3=0;
int file=-1;
int waittime=-1;

main()
  {
  int i;
  /* Opening Libs */
  IntuitionBase = (struct IntuitionBase *) OpenLibrary("intuition.library",LIBRARY_VERSION);
  if(IntuitionBase==NULL) cleanup("Unable to open intuition.library");
  GfxBase = (struct GfxBase *) OpenLibrary("graphics.library",LIBRARY_VERSION);
  if (GfxBase==NULL) cleanup("Unable to open graphics.library");
  /* Opening Screen */
  NScreen.Font=&NFont;
  ConScreen=(struct Screen *) OpenScreen(&NScreen);
  if (ConScreen==NULL) cleanup("Unable to open conscreen");
  ScreenToFront(ConScreen);
  ShowTitle(ConScreen,FALSE);
  SetRGB4(&(ConScreen->ViewPort),BLACKPEN, 0L, 0L, 0L);    /* Black  */
  SetRGB4(&(ConScreen->ViewPort),WHITEPEN,15L,15L,15L);    /* White  */
  SetRGB4(&(ConScreen->ViewPort),GREENPEN, 0L, 9L, 0L);    /* Green  */
  SetRGB4(&(ConScreen->ViewPort),OTHERPEN,15L,12L, 0L);    /* Orange */
  SetRGB4(&(ConScreen->ViewPort),PLAYERPEN(0),15L, 2L, 0L);/* Red    */
  SetRGB4(&(ConScreen->ViewPort),PLAYERPEN(1), 7L, 9L,15L);/* Blue   */
  SetRGB4(&(ConScreen->ViewPort),PLAYERPEN(2),15L,15L, 2L);/* Yellow */
  SetRGB4(&(ConScreen->ViewPort),PLAYERPEN(3), 0L,15L, 2L);/* Full Green */
  SetRGB4(&(ConScreen->ViewPort),SPRITEPEN(0), 0L, 0L, 0L);/* Black  */
  SetRGB4(&(ConScreen->ViewPort),SPRITEPEN(1), 5L,13L, 0L);/* Green  */
  SetRGB4(&(ConScreen->ViewPort),SPRITEPEN(2),15L,15L,15L);/* White  */
  SetRGB4(&(ConScreen->ViewPort),SPRITEPEN(3),15L,12L, 0L);/* Orange */
  NWindow.Screen=ConScreen;
  ConWindow=(struct Window *) OpenWindow(&NWindow);
  if(ConWindow==NULL) cleanup("Unable to open ConWindow");
  ConPort=ConWindow->RPort;
  SetPointer(ConWindow,CrossPointer,9L,9L,-4L,-4L);
  title();
  conquest();
  cleanup("OK.");
  }
cleanup(str)
char *str;
  {
  puts(str);
  if (file!=-1) close(file);
  if (ConWindow) CloseWindow(ConWindow);
  if (ConScreen) CloseScreen(ConScreen);
  if (GfxBase) CloseLibrary(GfxBase);
  if (IntuitionBase) CloseLibrary(IntuitionBase);
  exit(1);
  }
title()
  {
  Move(ConPort,232L,10L);
  Text(ConPort,"by",2L);
  ABOX( 8, 0,15,15);
  EBOX(10, 2,15,13);
  ABOX(18, 8,25,15);
  EBOX(20,10,23,13);
  ABOX(24,16,25,16);
  ABOX(28, 7,29,15);
  ABOX(30, 8,33, 9);
  ABOX(36, 2,37,15);
  ABOX(38,14,39,15);
  ABOX( 0,20, 7,35);
  EBOX( 2,22, 7,33);
  ABOX( 2,26, 5,27);
  ABOX(10,28,17,35);
  EBOX(12,30,15,33);
  ABOX(16,22,17,27);
  ABOX(20,27,21,27);
  ABOX(20,28,29,35);
  EBOX(22,30,23,35);
  EBOX(26,30,27,35);
  ABOX(32,28,39,35);
  EBOX(34,30,37,33);
  ABOX(38,36,39,36);
  ABOX(42,28,47,35);
  EBOX(44,30,45,35);
  ABOX(42,27,43,27);
  }
#include "human.c"
