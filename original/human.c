extern struct NShip ship[SHIPNO];
struct IntuiText DText=
  {
  GREENPEN,BLACKPEN,
  JAM2,
  0,0,
  NULL,
  NULL,
  NULL
  };
struct IntuiMessage *mes;
int screen1,screen2;
int show1,show2;
int showpla1,showpla2;
int sort[STARNO],dis[STARNO],sortsta;

human(pla)
int pla;
  {
  int i;
  struct NCommand c;
  c.com=DUMMY;
  screen1=PLAYERSUMMARY;
  screen2=CLEAR;
  sortsta=-1;
  if (waittime>general.time) return();
  make_displ(pla);
  while((c.com=input_action(0,pla))>=0)
    {
    switch(c.com)
      {
      case POSSESS:
        c.par[0]=input_star(1,pla);
        c.par[1]=input_planet(2,pla);
        break;
      case LAND:
        c.par[0]=input_task(1,pla);
        c.par[1]=input_planet(2,pla);
        break;
      case CARRY:
        c.par[0]=input_task(1,pla);
        input_ships_task(2,pla,&c);
        break;
      case DESTINATION:
        c.par[0]=input_task(1,pla);
        c.par[1]=input_star(2,pla);
        break;
      case EMIGRATION:
        c.par[0]=input_star(1,pla);
        c.par[1]=input_planet(2,pla);
        c.par[2]=input_number(3,pla,star[c.par[0]].planet[c.par[1]].population);
        break;
      case JOIN:
        input_many_tasks(1,pla,&c);
        break;
      case KILL:
        c.par[0]=input_star(1,pla);
        c.par[1]=input_planet_or_ret(2,pla);
        break;
      case SPLIT:
        c.par[0]=input_task(1,pla);
        input_ships_task(2,pla,&c);
        break;
      case PRODUCE:
        c.par[0]=input_star(1,pla);
        c.par[1]=input_planet(2,pla);
        c.par[2]=input_product(3,pla,general.techlevel[pla]);
        break;
      case DECLARE:
        c.par[0]=input_player(1,pla);
        c.par[1]=input_status(2,pla,general.relation[pla][c.par[0]]);
        break;
      default:break;/*ERROR*/
      }
    do_command(pla,&c);
    make_displ(pla);
    }
  }
make_displ(pla)
int pla;
  {
  mapdisplay(pla);
  if (screen1==STARSUMMARY) starsummary(pla,show1);
  if (screen2==TASKSUMMARY) tasksummary(pla,showpla2,show2);
  if (screen1==PLAYERSUMMARY) showstatus(pla);
  if (screen2==TASKOVERVIEW) showtasks(pla,showpla2,show2);
  if (screen2==SHIPOVERVIEW) tshipsummary(pla,showpla2,show2);
  if (screen1==STAROVERVIEW) showstars(pla);
  if (screen2==SHIPSUMMARY) shipsummary(pla,show2);
  if (screen2==PLANETSUMMARY) planetsummary(pla,show2,showpla2);
  }
input_action(line,pla)
int line;
int pla;
  {
  int c,i,j,p;
  char *str;
  switch(cget(pla))
    {
#include "debug.c" /* remove this line when recompiling */
    case 'p':
      str="Possess planet";
      c=POSSESS;
      break;
    case 'G':
      return(-1);
    case 'c':
      str="Carry ships";
      c=CARRY;
      break;
    case 'h':
      str="Hyperspace Tf";
      c=DESTINATION;
      break;
    case 'e':
      str="Emigration";
      c=EMIGRATION;
      break;
    case 'l':
      str="Land on planet";
      c=LAND;
      break;
    case 'j':
      str="Join taskforces";
      c=JOIN;
      break;
    case 'k':
      str="Kill";
      c=KILL;
      break;
    case 'n':
      str="New Taskforce from";
      c=SPLIT;
      break;
    case 'b':
      str="Build";
      c=PRODUCE;
      break;
    case 'd':
      str="Diplomacy";
      c=DECLARE;
      break;
    case '?':
      showhelp();
      return(DUMMY);
    case 's':
      SetAPen(ConPort,BLACKPEN);
      RectFill(ConPort,400L,0L,639L,39L);
      DText.FrontPen=GREENPEN;
      DText.IText=(UBYTE *) "SHOW  >Starsummary";
      PrintIText(ConPort,&DText,400L,8L*line);
      i=input_star_or_ret(1,pla);
      if(i==STARNO)
        {
        screen1=STAROVERVIEW;
        return(DUMMY);
        }
      j=input_planet_or_ret(2,pla);
      if(j==PLANETNO)
        {
        screen1=STARSUMMARY;
        show1=i;
        showpla1=j;
        return(DUMMY);
        }
      screen2=PLANETSUMMARY;
      show2=i;
      showpla2=j;
      return(DUMMY);
    case 't':
      SetAPen(ConPort,BLACKPEN);
      RectFill(ConPort,400L,0L,639L,39L);
      DText.FrontPen=GREENPEN;
      DText.IText=(UBYTE *) "SHOW  >Taskforcesummary";
      PrintIText(ConPort,&DText,400L,8L*line);
      showpla2=input_player_pref(1,pla);
      show2=input_task_or_ret(2,pla);
      if(show2==TASKNO) screen2=TASKOVERVIEW;
                   else screen2=TASKSUMMARY;
      return(DUMMY);
    case 'z':
      SetAPen(ConPort,BLACKPEN);
      RectFill(ConPort,400L,0L,639L,39L);
      DText.FrontPen=GREENPEN;
      DText.IText=(UBYTE *) "SHOW  >Status";
      PrintIText(ConPort,&DText,400L,8L*line);
      screen1=PLAYERSUMMARY;
      return(DUMMY);
    case 'f':
      SetAPen(ConPort,BLACKPEN);
      RectFill(ConPort,400L,0L,639L,39L);
      DText.FrontPen=GREENPEN;
      DText.IText=(UBYTE *) "SHOW  >Shipsummary";
      PrintIText(ConPort,&DText,400L,8L*line);
      showpla2=input_player_or_ret(1,pla);
      show2=input_ship(2,pla);
      if(showpla2==PLAYERNO) screen2=SHIPSUMMARY;
                        else screen2=SHIPOVERVIEW;
      return(DUMMY);
    default:
      DisplayBeep(ConScreen);
      showhelp();
      return(DUMMY);
    }
  SetAPen(ConPort,BLACKPEN);
  RectFill(ConPort,400L,0L,639L,39L);
  DText.FrontPen=GREENPEN;
  DText.IText=(UBYTE *) "ACTION>";
  PrintIText(ConPort,&DText,400L,8L*line);
  DText.IText=(UBYTE *)str;
  PrintIText(ConPort,&DText,456L,8L*line);
  return(c);
  }
input_task(line,pla)
int line;
int pla;
  {
  int c,tas;
  DText.FrontPen=GREENPEN;
  DText.IText=(UBYTE *) "TF    >";
  PrintIText(ConPort,&DText,400L,8L*line);
  tas=TASKNO+1;
  while(tas>=TASKNO)
    {
    c=cget(pla);
    if(c>='a' && c<='z') tas=c-'a';
    if(c>='A' && c<='Z') tas=c-'A'+SECSET;
    }
  DText.IText=(UBYTE *)" ";
  DText.IText[0]=(UBYTE) c;
  PrintIText(ConPort,&DText,456L,8L*line);
  return(tas);
  }
input_task_or_ret(line,pla)
int line;
int pla;
  {
  int c,tas;
  DText.FrontPen=GREENPEN;
  DText.IText=(UBYTE *) "TF    >";
  PrintIText(ConPort,&DText,400L,8L*line);
  tas=TASKNO+1;
  while(tas>TASKNO)
    {
    c=cget(pla);
    if(c>='a' && c<='z') tas=c-'a';
    if(c>='A' && c<='Z') tas=c-'A'+SECSET;
    if(c==0x0d) tas=TASKNO;
    }
  if(tas==TASKNO) return(TASKNO);
  DText.IText=(UBYTE *)" ";
  DText.IText[0]=(UBYTE) c;
  PrintIText(ConPort,&DText,456L,8L*line);
  return(tas);
  }
input_ships_task(line,pla,co)
int line;
int pla;
struct NCommand *co;
  {
  int i,tas;
  tas=co->par[0];
  for(i=0;i<SHIPNO;i++)
    {
    co->par[i+1]=0;
    if(INTASK(pla,tas,i)==0 || task[pla][tas].headed==-1) continue;
    SetAPen(ConPort,BLACKPEN);
    RectFill(ConPort,400L,line*8L,639L,line*8L+15);
    SetAPen(ConPort,GREENPEN);
    DText.IText=(UBYTE *)ship[i].name;
    PrintIText(ConPort,&DText,400L,line*8L);
    co->par[i+1]=input_number(line+1,pla,INTASK(pla,tas,i));
    }
  return();
  }
input_many_tasks(line,pla,co)
int line;
int pla;
struct NCommand *co;
  {
  int c,i;
  i=0;
  DText.FrontPen=GREENPEN;
  DText.IText=(UBYTE *) "TFS   >";
  PrintIText(ConPort,&DText,400L,8L*line);
  DText.IText=(UBYTE *)" ";
  while(1)
    {
    c=cget(pla);
    if(c==0x0d)
      {
      co->par[i]=TASKNO;
      break;
      }
    if(c>='a' && c<='z' && c-'a'<TASKNO)
      {
      co->par[i]=c-'a';
      DText.IText[0]=(UBYTE) c;
      PrintIText(ConPort,&DText,456L+8L*i,8L*line);
      i++;
      }
    if(c>='A' && c<='Z' && c-'A'+SECSET<TASKNO)
      {
      co->par[i]=c-'A'+SECSET;
      DText.IText[0]=(UBYTE) c;
      PrintIText(ConPort,&DText,456L+8L*i,8L*line);
      i++;
      }
    if(i>22)
      {
      co->par[i]=TASKNO;
      break;
      }
    }
  }
input_star(line,pla)
int line;
int pla;
  {
  int sta;
  DText.FrontPen=GREENPEN;
  DText.IText=(UBYTE *) "STAR  >";
  PrintIText(ConPort,&DText,400L,8L*line);
  while( (sta=cget(pla)-'a') <0 || sta>=STARNO );
  DText.IText=(UBYTE *)" ";
  DText.IText[0]=(UBYTE) sta+'A';
  PrintIText(ConPort,&DText,456L,8L*line);
  return(sta);
  }
input_star_or_ret(line,pla)
int line;
int pla;
  {
  int sta;
  DText.FrontPen=GREENPEN;
  DText.IText=(UBYTE *) "STAR  >";
  PrintIText(ConPort,&DText,400L,8L*line);
  while( ((sta=cget(pla)-'a') <0 || sta>=STARNO) && sta!=0x0d-'a' );
  if(sta==0x0d-'a') return(STARNO);
  DText.IText=(UBYTE *)" ";
  DText.IText[0]=(UBYTE) sta+'A';
  PrintIText(ConPort,&DText,456L,8L*line);
  return(sta);
  }
input_player_or_ret(line,pla)
int line;
int pla;
  {
  int c,d=PLAYERNO;
  DText.FrontPen=GREENPEN;
  DText.IText=(UBYTE *) "POWER >None";
  PrintIText(ConPort,&DText,400L,8L*line);
  while(1)
    {
    c=cget(pla);
    if(c>='a' && (c-'a')<PLAYERNO)
      {
      d=c-'a';
      c=0x0d;
      }
    if(c==',' && d>0)
      {
      d--;
      }
    if(c=='.' && d<PLAYERNO)
      {
      d++;
      }
    SetAPen(ConPort,BLACKPEN);
    RectFill(ConPort,456L,line*8L,639L,line*8L+7L);
    SetAPen(ConPort,GREENPEN);
    if(d<PLAYERNO)
      {
      DText.FrontPen=PLAYERPEN(d);
      DText.IText=(UBYTE *)general.pname[d];
      PrintIText(ConPort,&DText,456L,8L*line);
      }
    else
      {
      DText.FrontPen=GREENPEN;
      DText.IText=(UBYTE *)"None";
      PrintIText(ConPort,&DText,456L,8L*line);
      }
    if(c==0x0d)
      {
      DText.FrontPen=GREENPEN;
      return(d);
      }
    }
  }
input_planet(line,pla)
int line;
int pla;
  {
  int c;
  DText.FrontPen=GREENPEN;
  DText.IText=(UBYTE *) "PLANET>";
  PrintIText(ConPort,&DText,400L,8L*line);
  while((c=cget(pla))<'0' || c>'9');
  DText.IText=(UBYTE *)" ";
  DText.IText[0]=(UBYTE) c;
  PrintIText(ConPort,&DText,456L,8L*line);
  return(c-'0');
  }
input_planet_or_ret(line,pla)
int line;
int pla;
  {
  int c;
  DText.FrontPen=GREENPEN;
  DText.IText=(UBYTE *) "PLANET>";
  PrintIText(ConPort,&DText,400L,8L*line);
  while(((c=cget(pla))<'0' || c>'9') && c!=0x0d);
  if(c==0x0d) return(PLANETNO);
  DText.IText=(UBYTE *)" ";
  DText.IText[0]=(UBYTE) c;
  PrintIText(ConPort,&DText,456L,8L*line);
  return(c-'0');
  }
input_ship_in_task(line,pla,tas)
int line;
int pla;
int tas;
  {
  int c,d,e;
  DText.FrontPen=GREENPEN;
  for(d=0;d<SHIPNO;d++) if(INTASK(pla,tas,d)) break;
  DText.IText=(UBYTE *) "SHIP  >";
  PrintIText(ConPort,&DText,400L,8L*line);
  if (d==SHIPNO) return(0);
  DText.IText=(UBYTE *)ship[d].name;
  PrintIText(ConPort,&DText,456L,8L*line);
  while(1)
    {
    c=cget(pla);
    if(c>='a' && (c-'a')<SHIPNO && INTASK(pla,tas,c-'a'))
      {
      d=c-'a';
      c=0x0d;
      }
    if(c==',')
      {
      e=d-1;
      for(;e>=0;e--) if(INTASK(pla,tas,e)) break;
      if(e>=0) d=e;
      }
    if(c=='.')
      {
      e=d+1;
      for(;e<SHIPNO;e++) if(INTASK(pla,tas,e)) break;
      if(e<SHIPNO) d=e;
      }
    SetAPen(ConPort,BLACKPEN);
    RectFill(ConPort,456L,line*8L,639L,line*8L+7L);
    SetAPen(ConPort,GREENPEN);
    DText.IText=(UBYTE *)ship[d].name;
    PrintIText(ConPort,&DText,456L,8L*line);
    if(c==0x0d) return(d);
    }
  }
input_product(line,pla,tech)
int line;
int pla;
int tech;
  {
  int c,d=RESEARCHING,e;
  DText.FrontPen=GREENPEN;
  DText.IText=(UBYTE *) "BUILD >";
  PrintIText(ConPort,&DText,400L,8L*line);
  DText.IText=(UBYTE *)"Researching";
  PrintIText(ConPort,&DText,456L,8L*line);
  while(1)
    {
    c=cget(pla);
    if(c>='a' && (c-'a')<SHIPNO && ship[c-'a'].techlevel<=tech)
      {
      d=c-'a';
      c=0x0d;
      }
    if(c=='M') {d=MINING;c=0x0d;}
    if(c=='I') {d=INVESTING;c=0x0d;}
    if(c=='R') {d=RESEARCHING;c=0x0d;}
    if(c==',')
      {
      e=d-1;
      if(e<SHIPNO)
        {
        for(;e>=0;e--) if(ship[e].techlevel<=tech&&!(ship[e].special&NOTPROD)) break;
        if(e>=0) d=e;
        }
      else d=e;
      }
    if(c=='.')
      {
      e=d+1;
      if(e>=SHIPNO)
        {
        if(e<DO_NOTHING) d=e;
        }
      else
        {
        for(;e<SHIPNO;e++) if(ship[e].techlevel<=tech&&!(ship[e].special&NOTPROD)) break;
        d=e;
        }
      }
    SetAPen(ConPort,BLACKPEN);
    RectFill(ConPort,456L,line*8L,639L,line*8L+7L);
    SetAPen(ConPort,GREENPEN);
    if(d<SHIPNO && !(ship[d].special&NOTPROD)) DText.IText=(UBYTE *)ship[d].name;
    if(d==RESEARCHING) DText.IText=(UBYTE *)"Researching";
    if(d==MINING) DText.IText=(UBYTE *)"Mining";
    if(d==INVESTING) DText.IText=(UBYTE *)"Investing";
    PrintIText(ConPort,&DText,456L,8L*line);
    if(c==0x0d) return(d);
    }
  }
input_ship(line,pla)
int line;
int pla;
  {
  int c,d=SHIPNO-1,e;
  DText.FrontPen=GREENPEN;
  DText.IText=(UBYTE *) "SHIP  >";
  PrintIText(ConPort,&DText,400L,8L*line);
  DText.IText=(UBYTE *)ship[d].name;
  PrintIText(ConPort,&DText,456L,8L*line);
  while(1)
    {
    c=cget(pla);
    if(c>='a' && (c-'a')<SHIPNO)
      {
      d=c-'a';
      c=0x0d;
      }
    if(c==',')
      {
      for(e=d-1;e>=0;e--) if(ship[e].name[0]!='\0') break;
      if(e>=0) d=e;
      }
    if(c=='.')
      {
      for(e=d+1;e<SHIPNO;e++) if(ship[e].name[0]!='\0')break;
      if(e<SHIPNO) d=e;
      }
    SetAPen(ConPort,BLACKPEN);
    RectFill(ConPort,456L,line*8L,639L,line*8L+7L);
    SetAPen(ConPort,GREENPEN);
    DText.IText=(UBYTE *)ship[d].name;
    PrintIText(ConPort,&DText,456L,8L*line);
    if(c==0x0d) return(d);
    }
  }
input_status(line,pla,status)
int line;
int pla;
int status;
  {
  int c;
  DText.FrontPen=GREENPEN;
  DText.IText=(UBYTE *) "STATUS>";
  PrintIText(ConPort,&DText,400L,8L*line);
  DText.IText=(UBYTE *) "        ";
  pstr(status,DText.IText);
  PrintIText(ConPort,&DText,456L,8L*line);
  while(1)
    {
    c=cget(pla);
    if(c==',' && status>RELMIN) status--;
    if(c=='.' && status<RELMAX) status++;
    if(c=='w') {status=WAR;c=0x0d;}
    if(c=='p') {status=PEACE;c=0x0d;}
    if(c=='a') {status=ALLIANCE;c=0x0d;}
    pstr(status,DText.IText);
    PrintIText(ConPort,&DText,456L,8L*line);
    if(c==0x0d) return(status);
    }
  }
input_number(line,pla,max)
int line;
int pla;
int max;
  {
  int pos,c,d=0;
  DText.FrontPen=GREENPEN;
  DText.IText=(UBYTE *)"NUMBER>000(000)";
  mstr(max,DText.IText+11);
  PrintIText(ConPort,&DText,400L,line*8L);
  if(max<=0) return(0);
  DText.IText=(UBYTE *)"000";
  while(1)
    {
    c=coget();
    if(c=='.' && d<max) d++;
    if(c==',' && d>0) d--;
    if(c>='0' && c<='9' && (c-'a')<=max)
      {
      d=c-'0';
      c=0x0d;
      }
    if(c=='>') d=max;
    if(c=='<') d=0;
    mstr(d,DText.IText);
    PrintIText(ConPort,&DText,456L,line*8L);
    if(c==0x0d) return(d);
    }
  }
input_player(line,pla)
int line;
int pla;
  {
  int c,d=0;
  DText.FrontPen=GREENPEN;
  if (pla==0) d++;
  DText.IText=(UBYTE *) "POWER >";
  PrintIText(ConPort,&DText,400L,8L*line);
  DText.FrontPen=PLAYERPEN(d);
  DText.IText=(UBYTE *)general.pname[d];
  PrintIText(ConPort,&DText,456L,8L*line);
  while(1)
    {
    c=cget(pla);
    if(c>='a' && (c-'a')<PLAYERNO && (c-'a')!=pla)
      {
      d=c-'a';
      c=0x0d;
      }
    if(c==',' && d>0)
      {
      d--;
      if(d==pla)
        {
        if(d==0) d=1;
        else d--;
        }
      }
    if(c=='.' && d+1<PLAYERNO)
      {
      d++;
      if(d==pla)
        {
        if(d==PLAYERNO-1) d--;
        else d++;
        }
      }
    SetAPen(ConPort,BLACKPEN);
    RectFill(ConPort,456L,line*8L,639L,line*8L+7L);
    SetAPen(ConPort,GREENPEN);
    DText.FrontPen=PLAYERPEN(d);
    DText.IText=(UBYTE *)general.pname[d];
    PrintIText(ConPort,&DText,456L,8L*line);
    if(c==0x0d)
      {
      DText.FrontPen=GREENPEN;
      return(d);
      }
    }
  }
input_player_pref(line,pla)
int line;
int pla;
  {
  int c,d;
  DText.FrontPen=GREENPEN;
  d=pla;
  DText.IText=(UBYTE *) "POWER >";
  PrintIText(ConPort,&DText,400L,8L*line);
  DText.FrontPen=PLAYERPEN(d);
  DText.IText=(UBYTE *)general.pname[d];
  PrintIText(ConPort,&DText,456L,8L*line);
  while(1)
    {
    c=cget(pla);
    if(c>='a' && (c-'a')<PLAYERNO)
      {
      d=c-'a';
      c=0x0d;
      }
    if(c==',' && d>0)
      {
      d--;
      }
    if(c=='.' && d+1<PLAYERNO)
      {
      d++;
      }
    SetAPen(ConPort,BLACKPEN);
    RectFill(ConPort,456L,line*8L,639L,line*8L+7L);
    SetAPen(ConPort,GREENPEN);
    DText.FrontPen=PLAYERPEN(d);
    DText.IText=(UBYTE *)general.pname[d];
    PrintIText(ConPort,&DText,456L,8L*line);
    if(c==0x0d)
      {
      DText.FrontPen=GREENPEN;
      return(d);
      }
    }
  }
cprint(pla,str1,str2,str3,str4,str5)
int pla;
char *str1;
char *str2;
char *str3;
char *str4;
char *str5;
  {
  if(!display_check(pla)) return();
  SetAPen(ConPort,BLACKPEN);
  RectFill(ConPort,400L,0L,639L,39L);
  DText.FrontPen=GREENPEN;
  DText.IText=(UBYTE *) str1;
  PrintIText(ConPort,&DText,520L-IntuiTextLength(&DText)/2,0L);
  DText.IText=(UBYTE *) str2;
  PrintIText(ConPort,&DText,520L-IntuiTextLength(&DText)/2,8L);
  DText.IText=(UBYTE *) str3;
  PrintIText(ConPort,&DText,520L-IntuiTextLength(&DText)/2,16L);
  DText.IText=(UBYTE *) str4;
  PrintIText(ConPort,&DText,520L-IntuiTextLength(&DText)/2,24L);
  DText.IText=(UBYTE *) str5;
  PrintIText(ConPort,&DText,520L-IntuiTextLength(&DText)/2,32L);
  }
nprint(pla,str)
int pla;
char *str;
  {
  int i;
  if(!display_check(pla)) return();
  DText.FrontPen=GREENPEN;
  DText.IText=(UBYTE *)"                              ";
  for(i=0;str[i]!='\0';i++) DText.IText[i]=(UBYTE) str[i];
  for(;i<30;i++) DText.IText[i]=(UBYTE) ' ';
  PrintIText(ConPort,&DText,400L,32L);
  }
randomize()
  {
  ULONG Seconds,Micros;
  CurrentTime(&Seconds,&Micros);
  srand((int)(Seconds+Micros));
  }
cget(pla)
int pla;
  {
  long Code,Class,X,Y;
  while(1)
    {
    WaitPort(ConWindow->UserPort);
    mes=(struct IntuiMessage *) GetMsg(ConWindow->UserPort);
    Class=mes->Class;
    Code=mes->Code;
    X=mes->MouseX;
    Y=mes->MouseY;
    ReplyMsg(mes);
    if(Class==VANILLAKEY) return(Code);
    if(Class==MOUSEBUTTONS && Code==SELECTDOWN) process_mouse(pla,(int) X/8,(int) Y/8);
    if(Class==MOUSEBUTTONS && Code==MENUDOWN) showstatus(pla);
    }
  }
coget()
  {
  long Code,Class;
  while(1)
    {
    WaitPort(ConWindow->UserPort);
    mes=(struct IntuiMessage *) GetMsg(ConWindow->UserPort);
    Class=mes->Class;
    Code=mes->Code;
    ReplyMsg(mes);
    if(Class==VANILLAKEY) return(Code);
    }
  }
wait_space(pla)
  {
  if(!display_check(pla)) return();
  DText.FrontPen=GREENPEN;
  DText.IText=(UBYTE *) "    PRESS SPACE TO CONTINUE   ";
  DText.DrawMode=JAM2 | INVERSVID;
  PrintIText(ConPort,&DText,400L,32L);
  while(coget()!=' ');
  DText.DrawMode=JAM2;
  DText.IText=(UBYTE *) "                              ";
  PrintIText(ConPort,&DText,400L,32L);
  }
cinput(str,line)
char *str;
int line;
  {
  int pos,c;
  DText.FrontPen=GREENPEN;
  DText.IText=(UBYTE *)">                             ";
  PrintIText(ConPort,&DText,400L,line*8L);
  pos=0;
  *str='\0';
  while(pos<STRLENGTH-1)
    {
    SetAPen(ConPort,GREENPEN);
    RectFill(ConPort,408L+pos*8,line*8L,415L+pos*8,line*8L+7);
    c=coget();
    SetAPen(ConPort,BLACKPEN);
    RectFill(ConPort,408L+pos*8,line*8L,415L+pos*8,line*8L+7);
    switch(c)
      {
      case '\b':if (pos) pos--;
                str[pos]='\0';
                DText.IText=(UBYTE *)" ";
                PrintIText(ConPort,&DText,408L+pos*8,line*8L);
                break;
      case 0x0d:pos=STRLENGTH-1;break;
      default  :str[pos]=c;
                pos++;
                str[pos]='\0';
                DText.IText=(UBYTE *)str;
                PrintIText(ConPort,&DText,408L,line*8L);
      }
    }
  }
mapdisplay(pla)
int pla;
  {
  int sta,tas,i,j,c;
  if(!display_check(pla)) return();
  for(sta=0;sta<STARNO;sta++)
    {
    DText.IText=(UBYTE *)" ";
    DText.IText[0]=(UBYTE) ('A'+sta);
    c=CommData[pla].star[sta].ruler;
    if(c<PLAYERNO) DText.FrontPen=PLAYERPEN(c);
    if(c==FIGHTING) DText.FrontPen=OTHERPEN;
    if(c==UNINHABITED || CommData[pla].star[sta].when<0) DText.FrontPen=WHITEPEN;
    if(!knowstar(pla,sta) && CommData[pla].star[sta].when>=0) DText.DrawMode |= INVERSVID;
                                                         else DText.DrawMode &=~INVERSVID;
    PrintIText(ConPort,&DText,16L*star[sta].x,8L*star[sta].y);
    DText.IText=(UBYTE *)" ";
    DText.DrawMode=JAM2;
    PrintIText(ConPort,&DText,16L*star[sta].x+8L,8L*star[sta].y);
    if(!knowstar(pla,sta)) continue;
    DText.IText=(UBYTE *)" ";
    DText.DrawMode=JAM1;
    for(i=0;i<PLAYERNO;i++)
      {
      DText.FrontPen=PLAYERPEN(i);
      for(j=0;j<TASKNO;j++)
        {
        DText.IText[0]=(UBYTE) TASKCHAR(j);
        if(task[i][j].headed==sta && task[i][j].arriving<=general.time)
          PrintIText(ConPort,&DText,16L*star[sta].x+8L,8L*star[sta].y);
        }
      }
    DText.DrawMode=JAM2;
    }
  DText.FrontPen=GREENPEN;
  }
starsummary(pla,sta)
int pla;
int sta;
  {
  int p,pla2,tas,sta2;
  if(!display_check(pla)) return;
  screen1=STARSUMMARY;show1=sta;
  SetAPen(ConPort,BLACKPEN);
  RectFill(ConPort,400L,40L,639L,119L);
  if(sta<0 || sta>=STARNO) return();
  if(sta!=sortsta) sortstars(sta);
  pla2=CommData[pla].star[sta].ruler;
  if(pla2<PLAYERNO)  DText.FrontPen=PLAYERPEN(pla2);
  if(pla2==FIGHTING) DText.FrontPen=OTHERPEN;
  if(CommData[pla].star[sta].when<0 ||pla2==UNINHABITED) DText.FrontPen=WHITEPEN;
  DText.IText=(UBYTE *)"Star %(%%,%%)                 ";
  *(DText.IText+5)=(UBYTE)'A'+sta;
  rstr(star[sta].x,DText.IText+7);
  rstr(star[sta].y,DText.IText+10);
  DText.DrawMode |= INVERSVID;
  PrintIText(ConPort,&DText,400L,40L);
  DText.DrawMode &=~INVERSVID;
  if(knowstar(pla,sta))
    for(p=0;p<PLANETNO;p++)
      {
      if(star[sta].planet[p].size==0) continue;
      if(star[sta].planet[p].mineable) DText.DrawMode=JAM2|INVERSVID;
      if(star[sta].planet[p].ruler!=PLAYERNO) DText.FrontPen=PLAYERPEN(star[sta].planet[p].ruler);
        else DText.FrontPen=WHITEPEN;
      DText.IText=(UBYTE *)"%:%%%/%%%/%%%  ";
      *(DText.IText+ 0)=(UBYTE)'0'+p;
      mstr(star[sta].planet[p].size,DText.IText+2);
      mstr(star[sta].planet[p].industry,DText.IText+10);
      *(DText.IText+13)=(UBYTE)' ';
      if(star[sta].planet[p].population*star[sta].planet[p].industry)
        {
        *(DText.IText+13)=(UBYTE) 'a'+star[sta].planet[p].producing;
        if(star[sta].planet[p].producing==RESEARCHING)
          *(DText.IText+13)=(UBYTE) 'R';
        if(star[sta].planet[p].producing==INVESTING)
          *(DText.IText+13)=(UBYTE) 'I';
        if(star[sta].planet[p].producing==MINING)
          *(DText.IText+13)=(UBYTE) 'M';
        if(star[sta].planet[p].producing==DO_NOTHING)
          *(DText.IText+13)=(UBYTE) ' ';
        }
      PrintIText(ConPort,&DText,400L+120*(p%2),48L+8*((int)p/2));
      if(star[sta].planet[p].poptype!=PLAYERNO) DText.FrontPen=PLAYERPEN(star[sta].planet[p].poptype);
        else DText.FrontPen=WHITEPEN;
      DText.IText=(UBYTE *)"   ";
      mstr(star[sta].planet[p].population,DText.IText);
      PrintIText(ConPort,&DText,448L+120*(p%2),48L+8*((int)p/2));
      DText.DrawMode=JAM2;
      }
  for(pla2=0;pla2<PLAYERNO;pla2 ++)
    {
    DText.DrawMode&=~INVERSVID;
    DText.IText=(UBYTE *)"Tf";
    DText.FrontPen=PLAYERPEN(pla2);
    PrintIText(ConPort,&DText,400L,80L+8*pla2);
    DText.IText=(UBYTE *) " ";
    for(tas=0;tas<TASKNO;tas++) if(task[pla2][tas].headed==sta)
      {
      *DText.IText=(UBYTE)TASKCHAR(tas);
      if(task[pla2][tas].arriving>general.time) DText.DrawMode|= INVERSVID;
                                          else DText.DrawMode&=~INVERSVID;
      if((task[pla2][tas].arriving>general.time || !knowstar(pla,sta)) && pla2!=pla && pla!=mode)
        continue;
      PrintIText(ConPort,&DText,424L+tas*8,80L+8*pla2);
      }
    }
  DText.IText=(UBYTE *) "%  ";
  if(pla!=PLAYERNO) for(sta2=1;sta2<8;sta2++)
    {
    DText.IText[0]=(UBYTE) 'A'+sort[sta2];
    tstr(dis[sta2],DText.IText+1);
    pla2=CommData[pla].star[sort[sta2]].ruler;
    if(pla2<PLAYERNO)  DText.FrontPen=PLAYERPEN(pla2);
    if(pla2==FIGHTING) DText.FrontPen=OTHERPEN;
    if(CommData[pla].star[sort[sta2]].when<0 ||pla2==UNINHABITED) DText.FrontPen=WHITEPEN;
    if(!knowstar(pla,sort[sta2]) && CommData[pla].star[sort[sta2]].when>=0) DText.DrawMode|=INVERSVID;
                      else DText.DrawMode&=~INVERSVID;
    PrintIText(ConPort,&DText,368L+sta2*32L,112L);
    }
  DText.FrontPen=GREENPEN;
  DText.DrawMode=JAM2;
  }
tasksummary(pla1,pla,tas)
int pla1;
int pla;
int tas;
  {
  int i,shi,line;
  if(!display_check(pla1)) return();
  SetAPen(ConPort,BLACKPEN);
  RectFill(ConPort,400L,120L,639L,199L);
  if(task[pla][tas].headed==-1) return();
  if(task[pla][tas].arriving>general.time||!knowstar(pla1,task[pla][tas].headed))
    if(pla1!=pla && pla1!=mode) return();
  screen2=TASKSUMMARY;showpla2=pla;show2=tas;
  DText.FrontPen=PLAYERPEN(pla);
  if(task[pla][tas].arriving<=general.time)
    {
    DText.IText=(UBYTE *)"Tf % at %    L:%%% S:%%% R:%%%";
    }
  else
    {
    DText.IText=(UBYTE *)"Tf % vs %/%% L:%%% S:%%% R:%%%";
    *(DText.IText+10)=(UBYTE)'0'+((task[pla][tas].arriving-general.time)/10);
    *(DText.IText+11)=(UBYTE)'0'+((task[pla][tas].arriving-general.time)%10);
    }
  if(tas<TASKNO) *(DText.IText+ 3)=(UBYTE) TASKCHAR(tas);
    else *(DText.IText+ 3)=(UBYTE) '*';
  *(DText.IText+ 8)=(UBYTE) 'A'+task[pla][tas].headed;
  if (pla1!=mode && pla1!=pla) mstr(task_free(pla,tas),DText.IText+15);
    else mstr(task_visi(pla,tas),DText.IText+15);
  mstr(task_speed(pla,tas),DText.IText+21);
  mstr(task_range(pla,tas),DText.IText+27);
  DText.DrawMode |= INVERSVID;
  PrintIText(ConPort,&DText,400L,120L);
  DText.DrawMode &=~INVERSVID;
  DText.FrontPen=WHITEPEN;
  line=1;
  for(shi=0;shi<SHIPNO;shi ++)
    {
    if(!task[pla][tas].ship_free[shi] && !task[pla][tas].ship_load[shi]) continue;
    if(pla1!=mode && pla1!=pla && !task[pla][tas].ship_free[shi]) continue;
    if(line==10)
      {
      wait_space(pla1);
      DText.FrontPen=WHITEPEN;
      SetAPen(ConPort,BLACKPEN);
      RectFill(ConPort,400L,128L,639L,199L);
      line=1;
      }
    DText.IText=(UBYTE *)ship[shi].name;
    PrintIText(ConPort,&DText,400L,120L+line*8);
    if(pla1==pla || pla1==mode)
      {
      DText.IText=(UBYTE *)"   /   ";
      mstr(task[pla][tas].ship_free[shi],DText.IText);
      mstr(task[pla][tas].ship_load[shi],DText.IText+4);
      PrintIText(ConPort,&DText,584L,120L+line*8);
      }
    else
      {
      DText.IText=(UBYTE *)"   ";
      mstr(task[pla][tas].ship_free[shi],DText.IText);
      PrintIText(ConPort,&DText,616L,120L+line*8);
      }
    line++;
    }
  DText.FrontPen=GREENPEN;
  }
shipsummary(pla,shi)
int pla;
int shi;
  {
  if(!display_check(pla)) return();
  screen2=SHIPSUMMARY;showpla2=pla;show2=shi;
  SetAPen(ConPort,BLACKPEN);
  RectFill(ConPort,400L,120L,639L,199L);
  DText.FrontPen=WHITEPEN;
  DText.DrawMode |= INVERSVID;
  DText.IText=(UBYTE *)"                              ";
  PrintIText(ConPort,&DText,400L,120L);
  DText.IText=(UBYTE *)ship[shi].name;
  PrintIText(ConPort,&DText,520L-IntuiTextLength(&DText)/2,120L);
  DText.DrawMode &=~INVERSVID;
  DText.IText=(UBYTE *)"Size      :000 Capacity  :000";
  mstr(ship[shi].size,DText.IText+11);
  mstr(ship[shi].capacity,DText.IText+26);
  PrintIText(ConPort,&DText,400L,128L);
  DText.IText=(UBYTE *)"HyperSpeed:000 HyperRange:000";
  mstr(ship[shi].hypspeed,DText.IText+11);
  mstr(ship[shi].hyprange,DText.IText+26);
  PrintIText(ConPort,&DText,400L,136L);
  DText.IText=(UBYTE *)"Attack    :000 No of At. :000";
  mstr(ship[shi].attack,DText.IText+11);
  mstr(ship[shi].attackno,DText.IText+26);
  PrintIText(ConPort,&DText,400L,144L);
  DText.IText=(UBYTE *)"Speed     :000 Shields   :000";
  mstr(ship[shi].speed,DText.IText+11);
  mstr(ship[shi].shields,DText.IText+26);
  PrintIText(ConPort,&DText,400L,152L);
  if(!(ship[shi].special&NOTPROD))
    {
    if(ship[shi].techlevel>general.techlevel[pla]) DText.DrawMode |= INVERSVID;
    DText.IText=(UBYTE *)"Techcost  :000 Techlevel :000";
    PrintIText(ConPort,&DText,400L,160L);
    DText.IText=(UBYTE *)"   ";
    mstr(ship[shi].techcost,DText.IText);
    PrintIText(ConPort,&DText,488L,160L);
    mstr(ship[shi].techlevel,DText.IText);
    PrintIText(ConPort,&DText,608L,160L);
    DText.DrawMode &=~INVERSVID;
    }
  DText.IText =(UBYTE *)"Starkill ";
  if(ship[shi].special&STARKILLER) PrintIText(ConPort,&DText,400L,168L);
  DText.IText =(UBYTE *)"One-Shot ";
  if(ship[shi].special&KAMIKAZE) PrintIText(ConPort,&DText,480L,168L);
  DText.IText =(UBYTE *)"Invisible";
  if(ship[shi].special&INVISIBLE) PrintIText(ConPort,&DText,560L,168L);
  DText.IText =(UBYTE *)"CataStart";
  if(ship[shi].special&FASTEJECTABLE) PrintIText(ConPort,&DText,400L,176L);
  DText.IText =(UBYTE *)"Not build";
  if(ship[shi].special&NOTPROD) PrintIText(ConPort,&DText,480L,176L);
  DText.IText =(UBYTE *)"Theft    ";
  if(ship[shi].special&STEAL) PrintIText(ConPort,&DText,560L,176L);
  DText.IText =(UBYTE *)"Catapult ";
  if(ship[shi].special&FASTEJECTER) PrintIText(ConPort,&DText,400L,184L);
  DText.IText =(UBYTE *)"         ";
  if(ship[shi].special&NULL) PrintIText(ConPort,&DText,480L,184L);
  DText.IText =(UBYTE *)"         ";
  if(ship[shi].special&NULL) PrintIText(ConPort,&DText,560L,184L);
  DText.IText =(UBYTE *)"         ";
  if(ship[shi].special&NULL) PrintIText(ConPort,&DText,400L,192L);
  DText.IText =(UBYTE *)"         ";
  if(ship[shi].special&NULL) PrintIText(ConPort,&DText,480L,192L);
  DText.IText =(UBYTE *)"         ";
  if(ship[shi].special&NULL) PrintIText(ConPort,&DText,560L,192L);
  DText.FrontPen=GREENPEN;
  }
showstatus(pla)
int pla;
  {
  int i,s,p;
  if(!display_check(pla)) return();
  screen1=PLAYERSUMMARY;showpla1=pla;
  SetAPen(ConPort,BLACKPEN);
  RectFill(ConPort,400L,40L,639L,119L);
  DText.FrontPen=PLAYERPEN(pla);
  DText.DrawMode |= INVERSVID;
  DText.IText=(UBYTE *)"                              ";
  PrintIText(ConPort,&DText,400L,40L);
  DText.IText=(UBYTE *)"Year %%%%";
  qstr(general.time,DText.IText+5);
  PrintIText(ConPort,&DText,520L-IntuiTextLength(&DText)/2,40L);
  DText.DrawMode &=~INVERSVID;
  DText.IText=(UBYTE *) general.pname[pla];
  PrintIText(ConPort,&DText,520L-IntuiTextLength(&DText)/2,48L);
  DText.IText=(UBYTE *)"Pop:0000 Ind:0000 Tec:0000";
  qstr(population_no(pla),DText.IText+4);
  qstr(industry_no(pla),DText.IText+13);
  qstr(general.techlevel[pla],DText.IText+22);
  PrintIText(ConPort,&DText,520L-IntuiTextLength(&DText)/2,56L);
  for(i=0;i<PLAYERNO;i++)
    {
    if (i==pla) continue;
    DText.FrontPen=PLAYERPEN(i);
    DText.IText=(UBYTE *) general.pname[i];
    PrintIText(ConPort,&DText,520L-IntuiTextLength(&DText)/2,48L+(i+(i<pla))*16);
    if (!exists(i))
      {
      DText.IText=(UBYTE *)"Killed";
      PrintIText(ConPort,&DText,520L-IntuiTextLength(&DText)/2,(i+(i<pla))*16+56L);
      continue;
      }
    DText.IText=(UBYTE *)"--------/--------";
    pstr(general.relation[pla][i],DText.IText);
    pstr(general.relation[i][pla],DText.IText+9);
    PrintIText(ConPort,&DText,452L,(i+(i<pla))*16+56L);
    DText.IText=(UBYTE *)"%:0000";
    if (pla==mode)
      {
      *DText.IText=(UBYTE)'T';
      qstr(general.techlevel[i],DText.IText+2);
      PrintIText(ConPort,&DText,592L,(i+(i<pla))*16+56L);
      *DText.IText=(UBYTE)'P';
      qstr(population_no(i),DText.IText+2);
      PrintIText(ConPort,&DText,400L,(i+(i<pla))*16+48L);
      *DText.IText=(UBYTE)'I';
      qstr(industry_no(i),DText.IText+2);
      PrintIText(ConPort,&DText,592L,(i+(i<pla))*16+48L);
      }
    }
  DText.FrontPen=GREENPEN;
  }
mstr(no,str)
int no;
char *str;
  {
  if(no<0) return();
  if(no<1000)
    {
    str[0]='0'+(no/100)%10;
    str[1]='0'+(no/10)%10;
    str[2]='0'+no%10;
    return();
    }
  str[0]=' ';
  str[1]='o';
  str[2]='o';
  return();
  }
rstr(no,str)
int no;
char *str;
  {
  if(no<0) return;
  if(no<100)
    {
    str[0]='0'+(no/10)%10;
    str[1]='0'+no%10;
    return();
    }
  str[0]='o';
  str[1]='o';
  return();
  }
tstr(no,str)
int no;
char *str;
  {
  if(no<0) return();
  if(no<10)
    {
    str[0]='0'+no;
    str[1]=' ';
    return();
    }
  if(no<100)
    {
    str[0]='0'+(no/10)%10;
    str[1]='0'+no%10;
    return();
    }
  str[0]='o';
  str[1]='o';
  }
qstr(no,str)
int no;
char *str;
  {
  if(no<0) return;
  if(no<10000)
    {
    str[0]='0'+(no/1000)%10;
    str[1]='0'+(no/100)%10;
    str[2]='0'+(no/10)%10;
    str[3]='0'+no%10;
    return();
    }
  str[0]=' ';
  str[1]=' ';
  str[2]='o';
  str[3]='o';
  return();
  }
pstr(rel,str)
int rel;
char *str;
  {
  if(rel==WAR)
    {
    str[0]='W';
    str[1]='a';
    str[2]='r';
    str[3]=' ';
    str[4]=' ';
    str[5]=' ';
    str[6]=' ';
    str[7]=' ';
    return();
    }
  if(rel==PEACE)
    {
    str[0]='P';
    str[1]='e';
    str[2]='a';
    str[3]='c';
    str[4]='e';
    str[5]=' ';
    str[6]=' ';
    str[7]=' ';
    return();
    }
  if(rel==ALLIANCE)
    {
    str[0]='A';
    str[1]='l';
    str[2]='l';
    str[3]='i';
    str[4]='a';
    str[5]='n';
    str[6]='c';
    str[7]='e';
    return();
    }
  }
clearall(pla)
int pla;
  {
  register long i,j;
  if(!display_check(pla)) return();
  fline=0;
  SetRast(ConPort,BLACKPEN);
  SetAPen(ConPort,WHITEPEN);
  for(i=4;i<XMAX*16+4;i += 16)
    for(j=4;j<YMAX*8+4;j += 8)
      WritePixel(ConPort,i,j);
  screen1=CLEAR;
  screen2=CLEAR;
  }
showtasks(pla1,pla)
int pla1;
int pla;
  {
  int i;
  if(!display_check(pla1)) return();
  screen2=TASKOVERVIEW;showpla2=pla;
  SetAPen(ConPort,BLACKPEN);
  RectFill(ConPort,400L,120L,639L,199L);
  SetAPen(ConPort,PLAYERPEN(pla));
  RectFill(ConPort,400L,120L,639L,127L);
  DText.FrontPen=PLAYERPEN(pla);
  DText.DrawMode |= INVERSVID;
  DText.IText=(UBYTE *)"Taskforceoverview";
  PrintIText(ConPort,&DText,520L-IntuiTextLength(&DText)/2,120L);
  DText.DrawMode &=~INVERSVID;
  DText.FrontPen=WHITEPEN;
  DText.IText=(UBYTE *)"Tf % %/%%";
  for(i=0;i<TASKNO;i++)
    {
    if(task[pla][i].headed==-1) continue;
    if(pla1!=pla && mode!=pla1 && (!knowstar(pla1,task[pla][i].headed) || task[pla][i].arriving>general.time) ) continue;
    DText.IText[3]=(UBYTE) TASKCHAR(i);
    DText.IText[5]=(UBYTE) 'A'+task[pla][i].headed;
    if(task[pla][i].arriving<=general.time)
      {
      DText.DrawMode&=~INVERSVID;
      DText.IText[6]=(UBYTE)' ';
      DText.IText[7]=(UBYTE)' ';
      DText.IText[8]=(UBYTE)' ';
      }
    else
      {
      DText.DrawMode|= INVERSVID;
      DText.IText[6]=(UBYTE)'/';
      rstr(task[pla][i].arriving-general.time,DText.IText+7);
      }
    PrintIText(ConPort,&DText,400L+80*(i/9),8*(i%9)+128L);
    }
  DText.DrawMode=JAM2;
  DText.FrontPen=GREENPEN;
  }
output_fight(pla,pla1,shi1,pla2,shi2,str)
int pla;
int pla1;
int shi1;
int pla2;
int shi2;
char *str;
  {
  long pos;
  if(!display_check(pla)) return();
  if(fline==25)
    {
    fline=0;
    wait_space(pla);
    }
  if (fline==0)
    {
    SetAPen(ConPort,BLACKPEN);
    RectFill(ConPort,0L,0L,399L,199L);
    }
  pos=0;
  DText.FrontPen=PLAYERPEN(pla1);
  DText.IText=(UBYTE *)ship[shi1].name;
  PrintIText(ConPort,&DText,pos,fline*8L);
  pos+=IntuiTextLength(&DText);
  DText.FrontPen=WHITEPEN;
  DText.IText=(UBYTE *)" fires on ";
  PrintIText(ConPort,&DText,pos,fline*8L);
  pos+=IntuiTextLength(&DText);
  DText.FrontPen=PLAYERPEN(pla2);
  DText.IText=(UBYTE *)ship[shi2].name;
  PrintIText(ConPort,&DText,pos,fline*8L);
  pos+=IntuiTextLength(&DText);
  DText.FrontPen=WHITEPEN;
  DText.IText=(UBYTE *)":";
  PrintIText(ConPort,&DText,pos,fline*8L);
  pos+=IntuiTextLength(&DText);
  DText.IText=(UBYTE *)str;
  PrintIText(ConPort,&DText,pos,fline*8L);
  pos+=IntuiTextLength(&DText);
  DText.FrontPen=GREENPEN;
  fline++;
  }
tshipsummary(pla1,pla,shi)
int pla1;
int pla;
int shi;
  {
  int i;
  char name[30];
  if(!display_check(pla1)) return();
  screen2=SHIPOVERVIEW;showpla2=pla;show2=shi;
  SetAPen(ConPort,BLACKPEN);
  RectFill(ConPort,400L,120L,639L,199L);
  DText.FrontPen=PLAYERPEN(pla);
  DText.DrawMode |= INVERSVID;
  SetAPen(ConPort,PLAYERPEN(pla));
  RectFill(ConPort,400L,120L,639L,127L);
  strcpy(name,"Shipoverview:");
  strcat(name,ship[shi].name);
  DText.IText=(UBYTE *)name;
  PrintIText(ConPort,&DText,520L-IntuiTextLength(&DText)/2,120L);
  DText.DrawMode &=~INVERSVID;
  DText.FrontPen=WHITEPEN;
  if(pla!=pla1 && pla1!=mode)
    {
    DText.IText=(UBYTE *)"Tf %  %%%";
    for(i=0;i<TASKNO;i++)
      if(task[pla][i].headed!=-1 && task[pla][i].arriving<=general.time
         && knowstar(pla1,task[pla][i].headed) && task[pla][i].ship_free[shi]>0)
      {
      if(task[pla][i].arriving>general.time) DText.DrawMode |= INVERSVID;
                                        else DText.DrawMode &=~INVERSVID;
      DText.IText[3]=(UBYTE)TASKCHAR(i);
      mstr(task[pla][i].ship_free[shi],DText.IText+6);
      PrintIText(ConPort,&DText,400L+80*(i/9),8*(i%9)+128L);
      }
    DText.FrontPen=GREENPEN;
    return();
    }
  DText.IText=(UBYTE *)"Tf% %%/%%";
  for(i=0;i<TASKNO;i++)
    if(task[pla][i].headed!=-1 && INTASK(pla,i,shi))
    {
    if(task[pla][i].arriving>general.time) DText.DrawMode |= INVERSVID;
                                      else DText.DrawMode &=~INVERSVID;
    DText.IText[2]=(UBYTE)TASKCHAR(i);
    rstr(task[pla][i].ship_free[shi],DText.IText+4);
    rstr(task[pla][i].ship_load[shi],DText.IText+7);
    PrintIText(ConPort,&DText,400L+80*(i/9),8*(i%9)+128L);
    }
  DText.DrawMode=JAM2;
  DText.FrontPen=GREENPEN;
  }
showhelp(pla)
int pla;
  {
  if(!display_check(pla)) return();
  SetAPen(ConPort,BLACKPEN);
  RectFill(ConPort,400L,0L,639L,39L);
  DText.FrontPen=GREENPEN;
  DText.IText=(UBYTE *)"G=Go on        c=Carry Ships  ";
  PrintIText(ConPort,&DText,400L,0L);
  DText.IText=(UBYTE *)"h=Hyperspace   b=Build Ship   ";
  PrintIText(ConPort,&DText,400L,8L);
  DText.IText=(UBYTE *)"e=Emigrate     p=Possess Plan.";
  PrintIText(ConPort,&DText,400L,16L);
  DText.IText=(UBYTE *)"k=Kill         d=Declaration  ";
  PrintIText(ConPort,&DText,400L,24L);
  DText.IText=(UBYTE *)"j=join Tf l=Land Tf n=Split Tf";
  PrintIText(ConPort,&DText,400L,32L);
  }
showstars(pla)
int pla;
  {
  int i;
  if(!display_check(pla)) return();
  screen1=STAROVERVIEW;
  SetAPen(ConPort,BLACKPEN);
  RectFill(ConPort,400L,40L,639L,119L);
  SetAPen(ConPort,PLAYERPEN(pla));
  RectFill(ConPort,400L,40L,639L,47L);
  DText.FrontPen=PLAYERPEN(pla);
  DText.DrawMode |= INVERSVID;
  DText.IText=(UBYTE *)"Staroverview";
  PrintIText(ConPort,&DText,520L-IntuiTextLength(&DText)/2,40L);
  DText.DrawMode &=~INVERSVID;
  DText.FrontPen=WHITEPEN;
  DText.IText=(UBYTE *)"% %%%/%%%/%%%";
  for(i=0;i<STARNO;i++)
    {
    if(CommData[pla].star[i].ruler==PLAYERNO) continue;
    if(CommData[pla].star[i].ruler<PLAYERNO) DText.FrontPen=PLAYERPEN(CommData[pla].star[i].ruler);
    if(CommData[pla].star[i].ruler==FIGHTING) DText.FrontPen=OTHERPEN;
    if(CommData[pla].star[i].ruler==UNINHABITED) DText.FrontPen=WHITEPEN;
    DText.DrawMode=JAM2;
    if(!knowstar(pla,i)) DText.DrawMode |= INVERSVID;
    DText.IText[0]= (UBYTE) ('A'+i);
    mstr(CommData[pla].star[i].totsize,DText.IText+2);
    mstr(CommData[pla].star[i].totpop,DText.IText+6);
    mstr(CommData[pla].star[i].totind,DText.IText+10);
    PrintIText(ConPort,&DText,400L+120*(i/(STARNO/2)),8*(i%(STARNO/2))+48L);
    }
  DText.DrawMode=JAM2;
  DText.FrontPen=GREENPEN;
  }
process_mouse(pla,x,y)
int pla;
int x;
int y;
  {
  int pla1,tas1,pla2,tas2,sta,shi1,shi2,shi3,p,i,j;
  /* Check star */
  for(sta=0;sta<STARNO;sta++) if(star[sta].y==y && star[sta].x*2==x)
    {
    starsummary(pla,sta);
    return();
    }
  /* Check for taskforce on main screen */
  for(sta=0;sta<STARNO;sta++) if(star[sta].y==y && star[sta].x*2+1==x) break;
  if(sta!=STARNO)
    {
    if(!knowstar(pla,sta))
      {
      starsummary(pla,sta);
      return();
      }
    pla2=-1;
    for(pla1=0;pla1<PLAYERNO;pla1++)
      for(tas1=0;tas1<TASKNO;tas1++)
        if(task[pla1][tas1].headed==sta && task[pla1][tas1].arriving<=general.time) pla2=pla1,tas2=tas1;
    if(pla2==-1)
      {
      starsummary(pla,sta);
      return();
      }
    for(pla1=0;pla1<PLAYERNO;pla1++)
      for(tas1=0;tas1<TASKNO;tas1++)
        if(task[pla1][tas1].headed==sta && task[pla1][tas1].arriving<=general.time && (pla1!=pla2 || tas1!=tas2))
          {
          starsummary(pla,sta);
          return();
          }
    tasksummary(pla,pla2,tas2);
    return();
    }
  /* Check for click on mapfield */
  if(x<50)
    {
    showstars(pla);
    return();
    }
  /* Check for planet in starsummary */
  if(screen1==STARSUMMARY && y>5 && y<10 && x>49)
    {
    p=(y-6)*2+(x>64);
    if(star[show1].planet[p].size>0)
      planetsummary(pla,show1,p);
    return();
    }
  /* Check for taskforce in starsummary */
  if(screen1==STARSUMMARY && x>52 && y>9 && y<14)
    {
    pla1=y-10;
    tas1=x-53;
    if(task[pla1][tas1].headed!=show1) return();
    tasksummary(pla,pla1,tas1);
    return();
    }
  /* Check for show all tasks in starsummary */
  if(screen1==STARSUMMARY && x<52 && y>9 && y<14)
    {
    pla1=y-10;
    showtasks(pla,pla1);
    return();
    }
  /* Check for star in starsummary */
  if(screen1==STARSUMMARY && x>49 && y==14)
    {
    sta=(x-50)/4+1;
    if (sta<8) starsummary(pla,sort[sta]);
    return();
    }
  /* Check for show shipsummary in tasksummary */
  if(screen2==TASKSUMMARY && x<70 && y>15)
    {
    shi2=0;
    for(shi1=0;shi1<SHIPNO;shi1++)
      if(task[showpla2][show2].ship_free[shi1]>0 ||
        (task[showpla2][show2].ship_load[shi1]>0 && (pla==showpla2||pla==mode))) shi2++;
    shi3=((shi2-1)/10)*10+y-15;
    shi2=0;
    for(shi1=0;shi1<SHIPNO;shi1++)
      {
      if(task[showpla2][show2].ship_free[shi1]>0 ||
        (task[showpla2][show2].ship_load[shi1]>0 && (pla==showpla2||pla==mode))) shi2++;
      if (shi3==shi2) break;
      }
    if(shi1==SHIPNO) return;
    shipsummary(pla,shi1);
    return();
    }
  /* Check for shipoverview */
  if(screen2==TASKSUMMARY && x>69 && y>15)
    {
    shi2=0;
    for(shi1=0;shi1<SHIPNO;shi1++)
      if(task[showpla2][show2].ship_free[shi1]>0 ||
        (task[showpla2][show2].ship_load[shi1]>0 && (pla==showpla2||pla==mode))) shi2++;
    shi3=((shi2-1)/10)*10+y-15;
    shi2=0;
    for(shi1=0;shi1<SHIPNO;shi1++)
      {
      if(task[showpla2][show2].ship_free[shi1]>0 ||
        (task[showpla2][show2].ship_load[shi1]>0 && (pla==showpla2||pla==mode))) shi2++;
      if (shi3==shi2) break;
      }
    if(shi1==SHIPNO) return;
    tshipsummary(pla,showpla2,shi1);
    return();
    }
  /* Check for click on shipname in planetsummary */
  if(screen2==PLANETSUMMARY && y==19)
    {
    if(star[show2].planet[showpla2].producing<SHIPNO)
      shipsummary(pla,star[show2].planet[showpla2].producing);
    return();
    }
  /* Check for click on Tf in Taskoverview */
  if(screen2==TASKOVERVIEW && y>15)
    {
    pla1=showpla2;
    tas1=((x-50)/10)*9+(y-16);
    tasksummary(pla,pla1,tas1);
    return();
    }
  /* Check for click on Tf in Shipoverview */
  if(screen2==SHIPOVERVIEW && y>15)
    {
    pla1=showpla2;
    tas1=((x-50)/10)*9+(y-16);
    if(task[pla1][tas1].ship_free[show2]==0 && pla1!=pla && pla!=mode) return();
    if(!INTASK(pla1,tas1,show2)) return();
    tasksummary(pla,pla1,tas1);
    return();
    }
  if(screen1==STAROVERVIEW && y>5 && y<14)
    {
    sta=((x-50)/15)*8+(y-6);
    starsummary(pla,sta);
    return();
    }
  if(y<4)
    {
    showhelp(pla);
    return();
    }
  }
planetsummary(pla1,sta,p)
int pla1;
int sta;
int p;
  {
  int pla2,cost;
  if(!display_check(pla1)) return();
  screen2=PLANETSUMMARY;
  show2=sta;
  showpla2=p;
  SetAPen(ConPort,BLACKPEN);
  RectFill(ConPort,400L,120L,639L,199L);
  if(sta<0 || sta>=STARNO) return();
  if(star[sta].planet[p].size<=0) return();
  if(!knowstar(pla1,sta)) return();
  pla2=star[sta].planet[p].ruler;
  if(pla2<PLAYERNO)  DText.FrontPen=PLAYERPEN(pla2);
  if(pla2==PLAYERNO) DText.FrontPen=WHITEPEN;
  SetAPen(ConPort,(long) DText.FrontPen);
  RectFill(ConPort,400L,120L,639L,127L);
  DText.DrawMode |= INVERSVID;
  DText.IText=(UBYTE *)"Star %(%%,%%)  Planet %       ";
  DText.IText[5]=(UBYTE) ('A'+sta);
  rstr(star[sta].x,DText.IText+7);
  rstr(star[sta].y,DText.IText+10);
  DText.IText[22]=(UBYTE) ('0'+p);
  PrintIText(ConPort,&DText,400L,120L);
  DText.DrawMode=JAM2;
  DText.FrontPen=WHITEPEN;
  DText.IText=(UBYTE *) "Size        :%%%";
  mstr(star[sta].planet[p].size,DText.IText+13);
  PrintIText(ConPort,&DText,400L,128L);
  DText.IText=(UBYTE *) "Population  :%%%";
  mstr(star[sta].planet[p].population,DText.IText+13);
  PrintIText(ConPort,&DText,400L,136L);
  DText.IText=(UBYTE *) "Industry    :%%%";
  mstr(star[sta].planet[p].industry,DText.IText+13);
  PrintIText(ConPort,&DText,400L,144L);
  DText.IText=(UBYTE *) "Building    :-";
  PrintIText(ConPort,&DText,400L,152L);
  if(star[sta].planet[p].producing<SHIPNO)
    {
    DText.IText=(UBYTE *) ship[star[sta].planet[p].producing].name;
    cost=ship[star[sta].planet[p].producing].techcost;
    }
  if(star[sta].planet[p].producing==MINING)
    {
    DText.IText=(UBYTE *) "MINING";
    cost=MCOST;
    }
  if(star[sta].planet[p].producing==INVESTING)
    {
    DText.IText=(UBYTE *) "INVESTING";
    cost=ICOST;
    }
  if(star[sta].planet[p].producing==RESEARCHING)
    {
    DText.IText=(UBYTE *) "RESEARCHING";
    cost=RCOST;
    }
  if(star[sta].planet[p].producing==DO_NOTHING)
    {
    DText.FrontPen=GREENPEN;
    return();
    }
  PrintIText(ConPort,&DText,504L,152L);
  DText.IText=(UBYTE *) "Cost        :%%% per Unit";
  mstr(cost,DText.IText+13);
  PrintIText(ConPort,&DText,400L,160L);
  DText.IText=(UBYTE *) "Average time:%%.%% Years";
  rstr(cost/star[sta].planet[p].industry,DText.IText+13);
  rstr(((cost*100)/star[sta].planet[p].industry)%100,DText.IText+16);
  PrintIText(ConPort,&DText,400L,168L);
  DText.IText=(UBYTE *) "Available   :Year %%%% (Est.)";
  qstr(general.time+(cost-star[sta].planet[p].ipoints+star[sta].planet[p].industry-1)/star[sta].planet[p].industry,DText.IText+18);
  PrintIText(ConPort,&DText,400L,176L);
  DText.FrontPen=GREENPEN;
  }
sortstars(sta)
int sta;
  {
  register i;
  int flag;
  sortsta=sta;
  for(i=0;i<STARNO;i++)
    {
    sort[i]=i;
    dis[i]=distance(sta,i);
    }
  flag=1;
  while(flag)
    {
    flag=0;
    for(i=0;i<STARNO-1;i++) if(dis[i]>dis[i+1])
      {
      flag=1;
      dis[i]   ^=dis[i+1];
      dis[i+1] ^=dis[i];
      dis[i]   ^=dis[i+1];
      sort[i]  ^=sort[i+1];
      sort[i+1]^=sort[i];
      sort[i]  ^=sort[i+1];
      /* Bet you didn't know that,did you ?                       */
      /* Exchanges sort[i] and sort[i+1], and dis[i] and dis[i+1] */
      /* Thanx for this algo,florian schwingenschloegel           */
      }
    }
  }
