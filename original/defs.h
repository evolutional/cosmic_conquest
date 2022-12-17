#define STARNO 16
#define TASKNO 26
#define PLANETNO 8
#define PLAYERNO 4
#define UNINHABITED PLAYERNO+1
#define FIGHTING PLAYERNO+2
#define XMAX 25
#define YMAX 25
#define SHIPNO 31
#define INVESTING SHIPNO
#define RESEARCHING SHIPNO+1
#define DO_NOTHING SHIPNO+3
#define MINING SHIPNO+2
#define STRLENGTH 20
#define SHIPNAMELENGTH 15
#define ATSTAR(pla,tas,sta) (task[pla][tas].headed==sta && task[pla][tas].arriving<=general.time)
#define HEADEDSTAR(pla,tas,sta) (task[pla][tas].headed==sta && task[pla][tas].arriving>general.time)
#define INTASK(pla,tas,shi) (task[pla][tas].ship_free[shi]+task[pla][tas].ship_load[shi])
#define MCOST 15
#define ICOST 15
#define RCOST 10
#define MAXSPEED 10
#define OREUNIT 29
#define POPUNIT 30
#define BUFLENGTH 50

#define BLACKPEN 0L
#define GREENPEN 1L
#define WHITEPEN 2L
#define OTHERPEN 3L
#define PLAYERPEN(n) (4L+n)
#define SPRITEPEN(n) (16L+n)

struct NPlanet
  {
  unsigned char ruler;
  unsigned char size;
  unsigned char population;
  unsigned char poptype;
  unsigned char industry;
  unsigned char producing;
  unsigned int ipoints;
  unsigned char mineable;
  };
struct NStar
  {
  int x;
  int y;
  struct NPlanet planet[PLANETNO];
  };
struct NTask
  {
  int headed;
  int arriving;
  unsigned char ship_free[SHIPNO];
  unsigned char ship_load[SHIPNO];
  };
struct NShip
  {
  char name[SHIPNAMELENGTH];
  int size;
  int capacity;
  int hyprange;
  int hypspeed;
  int speed;
  int attack;
  int attackno;
  int shields;
  int techcost;
  int techlevel;
  int special;
  };
struct StarData
  {
  int ruler;
  int totsize;
  int totpop;
  int totind;
  int danger;
  int when;
  };
struct NCommData
  {
  struct StarData star[STARNO];
  };
#define NOSPEC     0
#define STARKILLER 1
#define KAMIKAZE   2
#define INVISIBLE  4
#define FASTEJECTER 8
#define NOTPROD   16
#define STEAL     32
#define FASTEJECTABLE 64
struct NGeneral
  {
  int time;
  char pname[PLAYERNO][STRLENGTH];
  int techlevel[PLAYERNO];
  int relation[PLAYERNO][PLAYERNO];
  };
#define RELMAX 2
#define RELMIN 0
#define ALLIANCE 2
#define WAR 1
#define PEACE 0
struct NCommand
  {
  int com;
  int par[SHIPNO+10];
  };

#define DUMMY 0
#define CARRY 1
#define DESTINATION 2
#define EMIGRATION 3
#define LAND 4
#define JOIN 5
#define KILL 6
#define SPLIT 7
#define PRODUCE 8
#define DECLARE 9
#define POSSESS 10
#define ERROR 11

#define CLEAR 0
#define STARSUMMARY 1
#define TASKSUMMARY 2
#define TASKOVERVIEW 3
#define SHIPSUMMARY 4
#define STAROVERVIEW 5
#define PLAYERSUMMARY 6
#define SHIPOVERVIEW 7
#define PLANETSUMMARY 8

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define ABS(x) ((x<0)?(-(x)):(x))
#define RANDOM(x) ((int) ((1L*(x)*(rand()&0xfff))/0x1000))
#define RSGN() ((int) (rand()&0x02)-1)
#define SECSET ('z'-'a'+1)
#define TASKCHAR(tas) (((tas)<SECSET) ? (tas)+'a' : (tas)+'A'-SECSET)
#include "proto.h"
