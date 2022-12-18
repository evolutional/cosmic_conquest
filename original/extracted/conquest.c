#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include "defs.h"
#include "ships.h"

extern long Revision;
extern struct NStar star[STARNO];
extern struct NTask task[PLAYERNO][TASKNO+1];
extern struct NCommData CommData[PLAYERNO];
extern struct NGeneral general;
extern int growth[PLANETNO];
extern struct NShip ship[SHIPNO];


extern int fline;
extern int mode,mode2,mode3;
extern int file;
extern int waittime;

conquest()
  {
  int pla,sta,x,y;
  char *year="Year 0000";
  init();
  do
    {
    general.time++;
    qstr(general.time,year+5);
    cprint(PLAYERNO,"COSMIC CONQUEST",year,"Please wait","while fighting","");
    for(sta=0;sta<STARNO;sta++)
      {
      update(sta);
      if(fight(sta))
        cprint(PLAYERNO,"COSMIC CONQUEST",year,"Please wait","while fighting","");
      }
    for(pla=0;pla<PLAYERNO;pla++)
      {
      cprint(PLAYERNO,"COSMIC CONQUEST",year,"It is the turn of",general.pname[pla],"");
      wait_space(pla);
      if(comp_cmp("klingon",general.pname[pla])) klingon(pla);
      else 
      if(comp_cmp("romulan",general.pname[pla])) romulan(pla);
      else 
      if(comp_cmp("alien",general.pname[pla])) alien(pla);
      else 
      if(comp_cmp("berserker",general.pname[pla])) berserker(pla);
      else if(!comp_cmp("dead",general.pname[pla])) human(pla);
      if (mode3)
        {
        x=mode2;mode2=0;y=mode3;mode3=0;
        if (!display_check(pla) && exists(pla))
          {
          mode2=x;mode3=y;
          human(pla);
          }
        else
          {
          mode2=x;
          mode3=y;
          }
        }
      clean_task(pla);
      clearall(pla);
      }
    cprint(PLAYERNO,"COSMIC CONQUEST",year,"Please wait","while processing turn","");
    process_turn();
    }
  while(playon());
  deinit();
  }
load(str)
char *str;
  {
  int pla,sta,tas;
  char fname[STRLENGTH+10];
  if(*str=='\0') cleanup("Bad filename");
  strcpy(fname,"games/");
  strcat(fname,str);
  file=open(fname,O_RDONLY);
  if(file==-1) return(0);
  read(file,&general,sizeof(struct NGeneral));
  for(sta=0;sta<STARNO;sta++) read(file,&star[sta],sizeof(struct NStar));
  for(pla=0;pla<PLAYERNO;pla++)
    for(tas=0;tas<TASKNO;tas++)
      read(file,&task[pla][tas],sizeof(struct NTask));
  for(pla=0;pla<PLAYERNO;pla++)
    read(file,&CommData[pla],sizeof(struct NCommData));
  close(file);file=-1;
  return(1);
  }
save(str)
char *str;
  {
  int pla,sta,tas;
  char fname[STRLENGTH+10];
  if(*str=='\0') cleanup("Bad filename");
  strcpy(fname,"games/");
  strcat(fname,str);
  file=open(fname,O_WRONLY|O_CREAT);
  if(file==-1) return(0);
  write(file,&general,sizeof(struct NGeneral));
  for(sta=0;sta<STARNO;sta++) write(file,&star[sta],sizeof(struct NStar));
  for(pla=0;pla<PLAYERNO;pla++)
    for(tas=0;tas<TASKNO;tas++)
      write(file,&task[pla][tas],sizeof(struct NTask));
  for(pla=0;pla<PLAYERNO;pla++)
    write(file,&CommData[pla],sizeof(struct NCommData));
  close(file);file=-1;
  return(1);
  }
init()
  {
  int i,j,k,c;
  char filename[STRLENGTH];
  char *version="V1.0000";
  version[6]='0'+Revision%10;
  version[5]='0'+(Revision/10)%10;
  version[4]='0'+(Revision/100)%10;
  version[3]='0'+(Revision/1000)%10;
  cprint(PLAYERNO,"COSMIC CONQUEST",version,"Placed in the PD","by Carl Edman","");
  wait_space(PLAYERNO);
  randomize();
  cprint(PLAYERNO,"COSMIC CONQUEST","","Do you wish to ...","1.Start a new game  ","2.Restore an old one");
  while((c=coget())<'1' || c>'2');
  if(c=='1')
    {
    cprint(PLAYERNO,"COSMIC CONQUEST","","Please enter name of players","","");
    for(i=0;i<PLAYERNO;i++)
      {
      cinput(general.pname[i],3);
      for(j=0;j<i;j++) if(!strcmp(general.pname[j],general.pname[i])) break;
      if (j<i || strlen(general.pname[i])==0) 
        {
        nprint(PLAYERNO,"Invalid name.");
        i--;
        }
      else nprint(PLAYERNO,"OK.");
      }
    cprint(PLAYERNO,"COSMIC CONQUEST","","Please wait while","creating the galaxy","");
    for(i=0;i<STARNO;i++) for(j=0;j<PLANETNO;j++)
      {
      star[i].planet[j].ruler=PLAYERNO;
      star[i].planet[j].size=0;
      star[i].planet[j].population=0;
      star[i].planet[j].poptype=PLAYERNO;
      star[i].planet[j].industry=0;
      star[i].planet[j].producing=DO_NOTHING;
      star[i].planet[j].ipoints=0;
      star[i].planet[j].mineable=0;
      }
    for(i=0;i<(11*PLANETNO*STARNO)/100;i++) star[RANDOM(STARNO)].planet[RANDOM(PLANETNO)].size=10;
    for(i=0;i<( 9*PLANETNO*STARNO)/100;i++) star[RANDOM(STARNO)].planet[RANDOM(PLANETNO)].size=20;
    for(i=0;i<( 7*PLANETNO*STARNO)/100;i++) star[RANDOM(STARNO)].planet[RANDOM(PLANETNO)].size=40;
    for(i=0;i<( 4*PLANETNO*STARNO)/100;i++) star[RANDOM(STARNO)].planet[RANDOM(PLANETNO)].size=60;
    for(i=0;i<( 3*PLANETNO*STARNO)/100;i++) star[RANDOM(STARNO)].planet[RANDOM(PLANETNO)].size=120;
    for(i=0;i<(10*PLANETNO*STARNO)/100;i++) star[RANDOM(STARNO)].planet[RANDOM(PLANETNO)].mineable=1;
    spread_stars();
    while(spread_players()) spread_stars();
    for(i=0;i<PLAYERNO;i++)
      {
      general.techlevel[i]=100;
      for(j=0;j<PLAYERNO;j++) general.relation[i][j]=PEACE;
      for(j=1;j<=TASKNO;j++)
        {
        task[i][j].headed=-1;
        task[i][j].arriving=0;
        for(k=0;k<SHIPNO;k++)
          {
          task[i][j].ship_free[k]=0;
          task[i][j].ship_load[k]=0;
          }
        }
      for(j=0;j<STARNO;j++)
        {
        CommData[i].star[j].ruler=PLAYERNO;
        CommData[i].star[j].totsize=0;
        CommData[i].star[j].totpop=0;
        CommData[i].star[j].totind=0;
        CommData[i].star[j].danger=100;
        CommData[i].star[j].when=-1000;
        }
      if(comp_cmp("dead",general.pname[i]))
        {
        task[i][0].headed=-1;
        continue;
        }
      star[task[i][0].headed].planet[3].size=60;
      star[task[i][0].headed].planet[3].mineable=1;
      star[task[i][0].headed].planet[3].population=60;
      star[task[i][0].headed].planet[3].poptype=i;
      star[task[i][0].headed].planet[3].industry=60;
      star[task[i][0].headed].planet[3].ruler=i;
      star[task[i][0].headed].planet[3].producing=MINING;
      star[task[i][0].headed].planet[4].size=40;
      star[task[i][0].headed].planet[4].mineable=0;
      task[i][0].headed=-1;
      }
    general.time=0;
    }
  else
    {
    cprint(PLAYERNO,"COSMIC CONQUEST","","Please enter name of game","","");
    cinput(filename,3);
    while(!load(filename))
      {
      nprint(PLAYERNO,"Error loading file.");
      cinput(filename,3);
      }
    }
  clearall(PLAYERNO);
  }
deinit()
  {
  int c;
  char *version="V1.0000";
  version[6]='0'+Revision%10;
  version[5]='0'+(Revision/10)%10;
  version[4]='0'+(Revision/100)%10;
  version[3]='0'+(Revision/1000)%10;
  cprint(PLAYERNO,"COSMIC CONQUEST",version,"by Carl Edman","(C) 1988","GOODBYE !");
  wait_space(PLAYERNO);
  }
task_speed(pla,tas)
int pla;
int tas;
  {
  int shi,k=10000;
  for(shi=0;shi<SHIPNO;shi++) if(task[pla][tas].ship_free[shi]) k=MIN(k,ship[shi].hypspeed);
  return(k);
  }
task_range(pla,tas)
int pla;
int tas;
  {
  int shi,k=10000;
  for(shi=0;shi<SHIPNO;shi++) if(task[pla][tas].ship_free[shi]) k=MIN(k,ship[shi].hyprange);
  return(k);
  }
task_capa(pla,tas)
int pla;
int tas;
  {
  int shi,k=0;
  for(shi=0;shi<SHIPNO;shi++)
    k+=INTASK(pla,tas,shi)*ship[shi].capacity;
  return(k);
  }
task_visi(pla,tas)
int pla;
int tas;
  {
  int shi,k=0;
  for(shi=0;shi<SHIPNO;shi++)
    k+=task[pla][tas].ship_free[shi]*ship[shi].capacity;
  return(k);
  }
task_free(pla,tas)
int pla;
int tas;
  {
  int shi,k=0;
  for(shi=0;shi<SHIPNO;shi++)
    k+=task[pla][tas].ship_load[shi]*ship[shi].size;
  return(task_capa(pla,tas)-k);
  }
task_size(pla,tas)
int pla;
int tas;
  {
  int shi,k=0;
  for(shi=0;shi<SHIPNO;shi++)
    k+=task[pla][tas].ship_free[shi]*ship[shi].size;
  return(k);
  }
task_size_po(pla,tas)
int pla;
int tas;
  {
  int shi,k=0;
  for(shi=0;shi<SHIPNO-2;shi++)
    k+=task[pla][tas].ship_free[shi]*ship[shi].size;
  return(k);
  }
knowstar(pla,sta)
int pla;
int sta;
  {
  int p;
  if (mode==pla) return(1);
  for(p=0;p<PLANETNO;p++)
    if(star[sta].planet[p].ruler==pla || star[sta].planet[p].poptype==pla) return(1);
  if(search_task(pla,sta)!=TASKNO) return(1);else return(0);
  }
headedstar(pla,sta)
int pla;
int sta;
  {
  int tas;
  for(tas=0;tas<TASKNO;tas++) if(task[pla][tas].headed==sta) return(1);
  return(0);
  }
controlstar(sta)
int sta;
  {
  int p,pla;
  for(p=0;p<PLANETNO;p++) if(star[sta].planet[p].ruler!=PLAYERNO) break;
  if (p==PLANETNO) return(UNINHABITED);
  pla=star[sta].planet[p].ruler;
  for(p++;p<PLANETNO;p++) if(star[sta].planet[p].ruler!=PLAYERNO && star[sta].planet[p].ruler!=pla) break;
  if(p!=PLANETNO) return(FIGHTING);
  return(pla);
  }
rule_star(pl1,pl2)
int pl1;
int pl2;
  {
  int sta,p;
  for(sta=0;sta<STARNO;sta++)
    for(p=0;p<PLANETNO;p++) if(star[sta].planet[p].ruler==pl1 && star[sta].planet[p].poptype==pl2) return(1);
  return(0);
  }
search_task(pla,sta)
int pla;
int sta;
  {
  int tas;
  for(tas=0;tas<TASKNO;tas++) if(ATSTAR(pla,tas,sta)) return(tas);
  return(TASKNO);
  }
clean_task(pla)
int pla;
  {
  int tas,shi;
  for(tas=0;tas<TASKNO;tas++)
    {
    if (task[pla][tas].headed==-1) continue;
    for(shi=0;shi<SHIPNO;shi++)
      if(task[pla][tas].ship_free[shi] || task[pla][tas].ship_load[shi]) break;
    if(shi==SHIPNO)
      {
      task[pla][tas].headed=-1;
      continue;
      }
    while(task_free(pla,tas)<0)
      for(shi=0;shi<SHIPNO;shi++)
        if(task[pla][tas].ship_load[shi]>0)
          {
          task[pla][tas].ship_load[shi]--;
          task[pla][tas].ship_free[shi]++;
          break;
          }
    }
  }
process_turn()
  {
  int sta,tas,p,gr,pla,pro;
  for(sta=0;sta<STARNO;sta++) for(p=0;p<PLANETNO;p++) if(star[sta].planet[p].size)
    {
    if(star[sta].planet[p].population==0 || star[sta].planet[p].ruler==PLAYERNO)
      {
      star[sta].planet[p].poptype=PLAYERNO;
      star[sta].planet[p].population=0;
      star[sta].planet[p].industry=0;
      star[sta].planet[p].ruler=PLAYERNO;
      star[sta].planet[p].producing=DO_NOTHING;
      continue;
      }
    pla=star[sta].planet[p].ruler;
    pro=star[sta].planet[p].producing;
    gr=growth[p]-(4*star[sta].planet[p].population/star[sta].planet[p].size);
    if(gr<0) gr=0;
    gr *= (int)star[sta].planet[p].population;
    if(RANDOM(100)<gr%100)
      star[sta].planet[p].population+= gr/100+1;
    else
      star[sta].planet[p].population+= gr/100;
    if(star[sta].planet[p].population>star[sta].planet[p].size)
      {
      if(star[sta].planet[p].poptype==pla)
        {
        tas=get_task(pla,sta);
        if(tas!=-1)
          set_ships_free(pla,tas,POPUNIT,
                       (((int) star[sta].planet[p].population)-star[sta].planet[p].size)
                       +task[pla][tas].ship_free[POPUNIT]);
        }
      star[sta].planet[p].population=star[sta].planet[p].size;
      }
    if(star[sta].planet[p].industry>star[sta].planet[p].population) star[sta].planet[p].industry=star[sta].planet[p].population;
    star[sta].planet[p].ipoints += star[sta].planet[p].industry;
    switch(pro)
      {
      case INVESTING:
          star[sta].planet[p].industry+=star[sta].planet[p].ipoints/ICOST;
          star[sta].planet[p].ipoints %= ICOST;
          break;
      case RESEARCHING:
          general.techlevel[pla]+=(star[sta].planet[p].ipoints/RCOST)/(general.techlevel[pla]/100);
          star[sta].planet[p].ipoints %= RCOST*(general.techlevel[pla]/100);
          break;
      case MINING:
          tas=get_task(pla,sta);
          if(tas!=-1)
          set_ships_free(pla,tas,OREUNIT,
                         ((int) task[pla][tas].ship_free[OREUNIT])+star[sta].planet[p].ipoints/MCOST);
          star[sta].planet[p].ipoints %= MCOST;
          break;
      case DO_NOTHING:
          star[sta].planet[p].ipoints=0;
          break;
      default:
          tas=get_task(pla,sta);
          if(tas!=-1)
          set_ships_free(pla,tas,pro,
                         ((int) task[pla][tas].ship_free[pro]) 
                         +star[sta].planet[p].ipoints/ship[pro].techcost);
          star[sta].planet[p].ipoints %= ship[pro].techcost;
          break;
      }
    if(star[sta].planet[p].industry<5) star[sta].planet[p].industry++;
    if(star[sta].planet[p].industry>star[sta].planet[p].population) star[sta].planet[p].industry=star[sta].planet[p].population;
    }
  }
get_task(pla,sta)
int pla;
int sta;
  {
  int tas;
  tas=search_task(pla,sta);
  if (tas<TASKNO) return(tas);
  return(make_task(pla,sta));
  }
make_task(pla,sta)
int pla;
int sta;
  {
  int tas,shi;
  for(tas=0;tas<TASKNO;tas++) if(task[pla][tas].headed==-1) break;
  if (tas==TASKNO) return(-1);
  task[pla][tas].headed=sta;
  task[pla][tas].arriving=general.time;
  for(shi=0;shi<SHIPNO;shi++)
    task[pla][tas].ship_free[shi]=task[pla][tas].ship_load[shi]=0;
  return(tas);
  }
playon()
  {
  int c;
  int p,pla,pla2;
  char filename[STRLENGTH];
  if (waittime>general.time) return(1);
  for(pla=0;pla<PLAYERNO;pla++) if(exists(pla)) break;
  if (pla==PLAYERNO)
    {
    cprint(PLAYERNO,"COSMIC CONQUEST","GAME OVER","","No survivors","");
    wait_space(PLAYERNO);
    return(0);
    }
  for(pla2=pla+1;pla2<PLAYERNO;pla2++) if(exists(pla2)) break;
  if(pla2==PLAYERNO)
    {
    cprint(PLAYERNO,"COSMIC CONQUEST","GAME OVER",general.pname[pla],"has won the galaxy","");
    wait_space(PLAYERNO);
    return(0);
    }
  cprint(PLAYERNO,"COSMIC CONQUEST","","Do you wish to ...","1.Save Game","2.Don't save");
  while((c=coget())<'1' || c>'2') ;
  if(c=='1')
    {
    cprint(PLAYERNO,"COSMIC CONQUEST","","Please enter name of game","","");
    cinput(filename,3);
    while(!save(filename))
      {
      cinput(filename,3);
      nprint(PLAYERNO,"Error saving file.");
      }
    }
  cprint(PLAYERNO,"COSMIC CONQUEST","","Do you wish to ...","1.Quit","2.Play on");
  while((c=coget())<'1' || c>'2') ;
  return(c=='2');
  }
distance(sta1,sta2)
int sta1;
int sta2;
  {
  int r,q;
  if(sta1<0 || sta1>=STARNO || sta2<0 || sta2>=STARNO) return(1000);
  q =(star[sta1].x-star[sta2].x)*(star[sta1].x-star[sta2].x);
  q+=(star[sta1].y-star[sta2].y)*(star[sta1].y-star[sta2].y);
  for(r=0;r*r<q;r++);
  return(r);
  }
do_command(pla,co)
int pla;
struct NCommand *co;
  {
  int i,j,k,l,sta,tas;
  int c0,c1,c2,c3;
  c0=co->par[0];
  c1=co->par[1];
  c2=co->par[2];
  c3=co->par[3];
  switch(co->com)
    {
    case DUMMY: /* DUMMY   */
      if (mode2) printf("Pla %d:DUMMY\n",pla);
      return(0);
    case LAND:/* [0]=task [1]=planet */
      if (mode2) printf("Pla %d:LAND Task %d Planet %d\n",pla,c0,c1);
      if(task[pla][c0].headed==-1)
        {
        nprint(pla,"No such taskforce.");
        return(-1);
        }
      if(task[pla][c0].arriving>general.time)
        {
        nprint(pla,"Taskforce in hyperspace.");
        return(-1);
        }
      i=task[pla][c0].headed;
      c2=MIN(INTASK(pla,c0,POPUNIT),star[i].planet[c1].size-star[i].planet[c1].population);
      c3=MIN(INTASK(pla,c0,OREUNIT),star[i].planet[c1].population+c2-star[i].planet[c1].industry);
      if(star[i].planet[c1].size==0)
        {
        nprint(pla,"No such planet.");
        return(-1);
        }
      if(star[i].planet[c1].ruler!=pla && star[i].planet[c1].ruler!=PLAYERNO && c2>0)
        {
        nprint(pla,"Planet under alien control.");
        return(-1);
        }
      if(star[i].planet[c1].poptype!=pla && star[i].planet[c1].population>0 && star[i].planet[c1].poptype!=PLAYERNO && c2>0)
        {
        nprint(pla,"Alien population on planet.");
        return(-1);
        }
      if(star[i].planet[c1].ruler!=pla && c2==0 && c3>0)
        {
        nprint(pla,"Planet not under control.");
        return(-1);
        }
      if(star[i].planet[c1].population+c2==0 && c3>0)
        {
        nprint(pla,"No colony on planet.");
        return(-1);
        }
      if(c2>0)
        {
        star[i].planet[c1].ruler=pla;
        star[i].planet[c1].poptype=pla;
        }
      star[i].planet[c1].population += c2;
      c2 = task[pla][c0].ship_free[POPUNIT]-c2;
      set_ships_free(pla,c0,POPUNIT,c2);
      if(c2<0)
        set_ships_load(pla,c0,POPUNIT,((int) task[pla][c0].ship_load[POPUNIT])+c2);
      star[i].planet[c1].industry += c3;
      c3 = task[pla][c0].ship_free[OREUNIT]-c3;
      set_ships_free(pla,c0,OREUNIT,c3);
      if(c3<0)
        set_ships_load(pla,c0,OREUNIT,((int) task[pla][c0].ship_load[OREUNIT])+c3);
      clean_task(pla);
      update(i);
      nprint(pla,"OK.");
      return(1);
    case CARRY:/* [0]=task [1]=ship of type 0........*/
      if (mode2) printf("Pla %d:CARRY Task %d POPUNIT:%d OREUNIT:%d\n",pla,c0,co->par[POPUNIT+1],co->par[OREUNIT+1]);
      if(task[pla][c0].headed==-1)
        {
        nprint(pla,"No such taskforce.");
        return(-1);
        }
      if(task[pla][c0].arriving>general.time)
        {
        nprint(pla,"Taskforce in hyperspace.");
        return(-1);
        }
      for(i=0;i<SHIPNO;i++) 
        if (co->par[i+1]>INTASK(pla,c0,i))
          break;
      if(i!=SHIPNO)
        {
        nprint(pla,"Insufficient number of ships.");
        return(-1);
        }
      for(i=0;i<SHIPNO;i++) if(co->par[i+1]!=task[pla][c0].ship_load[i]) break;
      if(i==SHIPNO)
        {
        nprint(pla,"No change.");
        return(1);
        }
      k=0;
      for(i=0;i<SHIPNO;i++)
        k+=INTASK(pla,c0,i)*ship[i].capacity-co->par[i+1]*ship[i].size;
      if(k<0)
        {
        nprint(pla,"Insufficient capacity.");
        return(-1);
        }
      for(i=0;i<SHIPNO;i++)
        {
        k=INTASK(pla,c0,i);
        set_ships_load(pla,c0,i,co->par[i+1]);
        set_ships_free(pla,c0,i,k-co->par[i+1]);
        }
      clean_task(pla);
      nprint(pla,"OK.");
      return(1);
    case DESTINATION:/* [0]=task [1]=star */
      if (mode2) printf("Pla %d:DESTINATION Task %d Star %d\n",pla,c0,c1);
      if(task[pla][c0].headed==-1)
        {
        nprint(pla,"No such taskforce.");
        return(-1);
        }
      if(task[pla][c0].arriving>general.time)
        {
        nprint(pla,"Taskforce in hyperspace.");
        return(-1);
        }
      if(task[pla][c0].headed==c1)
        {
        nprint(pla,"Taskforce already at star.");
        return(-1);
        }
      i=distance(task[pla][c0].headed,c1);
      k=task_free(pla,c0);
      for(j=0;j<SHIPNO;j++)
        if(task[pla][c0].ship_free[j]>0 && ship[j].hyprange<i)
          k-=task[pla][c0].ship_free[j]*ship[j].size;
      if(k<0)
        {
        nprint(pla,"Distance to long.");
        return(-1);
        }
      for(j=0;j<SHIPNO;j++) if(ship[j].hyprange<i)
        {
        set_ships_load(pla,c0,j,INTASK(pla,c0,j));
        task[pla][c0].ship_free[j]=0;
        }
      k=task_free(pla,c0);
      for(l=0;l<50;l++)
        {
        for(j=0;j<SHIPNO;j++) if(ship[j].hypspeed==l)
          k -= task[pla][c0].ship_free[j]*ship[j].size;
        if(k<0) break;
        }
      for(j=0;j<SHIPNO;j++) if(ship[j].hypspeed<l)
        {
        set_ships_load(pla,c0,j,INTASK(pla,c0,j));
        task[pla][c0].ship_free[j]=0;
        }
      j=task_speed(pla,c0);
      task[pla][c0].headed=c1;
      task[pla][c0].arriving=general.time+ (i+j-1)/j;
      nprint(pla,"OK.");
      return(1);
    case EMIGRATION:/* [0]=star [1]=planet [2]=amount */
      if (mode2) printf("Pla %d:EMIGRATION Star %d Planet %d Amount %d\n",pla,c0,c1,c2);
      if(star[c0].planet[c1].size==0)
        {
        nprint(pla,"No such planet.");
        return(-1);
        }
      if(star[c0].planet[c1].ruler!=pla)
        {
        nprint(pla,"Planet not under control.");
        return(-1);
        }
      if(star[c0].planet[c1].population==0)
        {
        nprint(pla,"Not inhabited.");
        return(-1);
        }
      if(star[c0].planet[c1].poptype!=pla)
        {
        nprint(pla,"Alien population.");
        return(-1);
        }
      if(star[c0].planet[c1].population<c2)
        {
        nprint(pla,"Population to small.");
        return(-1);
        }
      i=get_task(pla,c0);
      if(i==-1)
        {
        nprint(pla,"Too few Taskforces.");
        return(-1);
        }
      set_ships_free(pla,i,POPUNIT,((int) task[pla][i].ship_free[POPUNIT])+c2);
      star[c0].planet[c1].population -= c2;
      if(star[c0].planet[c1].population==0)
        {
        star[c0].planet[c1].poptype=PLAYERNO;
        star[c0].planet[c1].ruler=PLAYERNO;
        }
      if(star[c0].planet[c1].industry>star[c0].planet[c1].population)
        star[c0].planet[c1].industry=star[c0].planet[c1].population;
      update(c0);
      nprint(pla,"OK.");
      return(i);
    case JOIN:/* [0]=task1 [1]=task2 ...*/
      if (mode2) printf("Pla %d:JOIN Tasks %d %d %d %d\n",pla,c0,c1,c2,c3);
      if(c0==TASKNO)
        {
        nprint(pla,"At least 1 taskforce to join.");
        return(-1);
        }
      for(i=0;co->par[i]!=TASKNO;i++) if(task[pla][co->par[i]].headed==-1) break;
      if(co->par[i]!=TASKNO)
        {
        nprint(pla,"No such taskforce.");
        return(-1);
        }
      for(i=0;co->par[i]!=TASKNO;i++) if(task[pla][co->par[i]].arriving>general.time) break;
      if(co->par[i]!=TASKNO)
        {
        nprint(pla,"Taskforce in hyperspace.");
        return(-1);
        }
      j=task[pla][c0].headed;
      for(i=0;co->par[i]!=TASKNO;i++) if(task[pla][co->par[i]].headed!=j) break;
      if(co->par[i]!=TASKNO)
        {
        nprint(pla,"Taskforces not at same star.");
        return(-1);
        }
      for(i=0;co->par[i]!=TASKNO;i++)
        {
        for(j=0;;j++) if(co->par[j]==TASKNO || (j!=i && co->par[i]==co->par[j])) break;
        if(co->par[j]!=TASKNO) break;
        }
      if(co->par[i]!=TASKNO)
        {
        nprint(pla,"Can't selfjoin taskforce.");
        return(-1);
        }
      for(i=1;co->par[i]!=TASKNO;i++)
        {
        for(j=0;j<SHIPNO;j++)
          {
          set_ships_free(pla,c0,j,((int) task[pla][c0].ship_free[j])+task[pla][co->par[i]].ship_free[j]);
          set_ships_load(pla,c0,j,((int) task[pla][c0].ship_load[j])+task[pla][co->par[i]].ship_load[j]);
          }
        task[pla][co->par[i]].headed=-1;
        }
      nprint(pla,"OK.");
      return(1);
    case KILL:/* [0]=star [1]=planet*/
      if (mode2) printf("Pla %d:KILL Star %d Planet %d\n",pla,c0,c1);
      if(c1==PLANETNO)
        {
        j=SHIPNO;
        for(i=0;i<TASKNO && j==SHIPNO;i++)
          if(ATSTAR(pla,i,c0))
            for(j=0;j<SHIPNO;j++)
              if((ship[j].special&STARKILLER)&&task[pla][i].ship_free[j]) break;
        if(i==TASKNO && j==SHIPNO)
          {
          nprint(pla,"No starkiller unloaded.");
          return(-1);
          }
        for(i=0;i<PLAYERNO;i++)
          for(j=0;j<TASKNO;j++) if(ATSTAR(i,j,c0)) task[i][j].headed=-1;
        for(i=0;i<PLANETNO;i++)
          {
          star[c0].planet[i].size=0;
          star[c0].planet[i].poptype=PLAYERNO;
          star[c0].planet[i].population=0;
          star[c0].planet[i].industry=0;
          star[c0].planet[i].ruler=PLAYERNO;
          star[c0].planet[i].producing=DO_NOTHING;
          }
        update(c0);
        return(1);
        }
      if(star[c0].planet[c1].size==0)
        {
        nprint(pla,"No such planet.");
        return(-1);
        }
      if(star[c0].planet[c1].ruler!=pla)
        {
        nprint(pla,"Planet not under control.");
        return(-1);
        }
      if(star[c0].planet[c1].population==0)
        {
        nprint(pla,"Not inhabited.");
        return(-1);
        }
      if(star[c0].planet[c1].poptype==pla)
        {
        nprint(pla,"Own population.");
        return(-1);
        }
      star[c0].planet[c1].poptype=PLAYERNO;
      star[c0].planet[c1].population=0;
      star[c0].planet[c1].industry=0;
      star[c0].planet[c1].ruler=PLAYERNO;
      star[c0].planet[c1].producing=DO_NOTHING;
      update(c0);
      nprint(pla,"OK.");
      return(1);
    case SPLIT:/* [0]=task [1]=ship of type 0........*/
      if (mode2) printf("Pla %d:SPLIT Task %d POPUNITS %d OREUNITS %d\n",pla,c0,co->par[POPUNIT+1],co->par[OREUNIT+1]);
      if(task[pla][c0].headed==-1)
        {
        nprint(pla,"No such taskforce.");
        return(-1);
        }
      if(task[pla][c0].arriving>general.time)
        {
        nprint(pla,"Taskforce in hyperspace.");
        return(-1);
        }
      for(i=0;i<SHIPNO;i++) 
        if (co->par[i+1]>INTASK(pla,c0,i))
          break;
      if(i!=SHIPNO)
        {
        nprint(pla,"Insufficient number of ships.");
        return(-1);
        }
      for(i=0;i<SHIPNO;i++) if(co->par[i+1]!=0) break;
      if(i==SHIPNO)
        {
        nprint(pla,"New taskforce without ships.");
        return(-1);
        }
      for(i=0;i<SHIPNO;i++) if(co->par[i+1]!=INTASK(pla,c0,i)) break;
      if(i==SHIPNO)
        {
        nprint(pla,"Old taskforce without ships.");
        return(c0);
        }
      j=make_task(pla,task[pla][c0].headed);
      if(j==-1)
        {
        nprint(pla,"To few taskforces.");
        return(-1);
        }
      for(i=0;i<SHIPNO;i++)
        {
        k = task[pla][c0].ship_free[i] + task[pla][c0].ship_load[i];
        task[pla][j].ship_free[i]=co->par[i+1];
        k -= task[pla][j].ship_free[i];
        task[pla][c0].ship_free[i] = MIN(k,250);
        k -= task[pla][c0].ship_free[i];
        task[pla][c0].ship_load[i] = k;
        }
      tas=task[pla][c0].headed;
      clean_task(pla);
      nprint(pla,"OK.");
      return(j);
    case PRODUCE:/* [0]=star [1]=planet [2]=what */
      if (mode2) printf("Pla %d:PRODUCE Star %d Planet %d Product %d\n",pla,c0,c1,c2);
      if(star[c0].planet[c1].size==0)
        {
        nprint(pla,"No such planet.");
        return(-1);
        }
      if(star[c0].planet[c1].ruler!=pla)
        {
        nprint(pla,"Planet not under control.");
        return(-1);
        }
      if(star[c0].planet[c1].industry==0)
        {
        nprint(pla,"No production facilities.");
        return(-1);
        }
      if(c2<SHIPNO && (ship[c2].special&NOTPROD))
        {
        nprint(pla,"Not producable.");
        return(-1);
        }
      if(c2<SHIPNO && ship[c2].techlevel>general.techlevel[pla])
        {
        nprint(pla,"Techlevel too low.");
        return(-1);
        }
      if(c2==MINING && !star[c0].planet[c1].mineable)
        {
        nprint(pla,"Planet not mineable.");
        return(-1);
        }
      if(star[c0].planet[c1].producing!=c2)
        star[sta].planet[c1].ipoints=MIN(star[sta].planet[c1].ipoints,star[sta].planet[c1].industry-1);
      star[c0].planet[c1].producing=c2;
      nprint(pla,"OK.");
      return(1);
    case DECLARE:/* [0]=on whom [1]=what */
      if (mode2) printf("Pla %d:DECLARE DecPla %d Status %d\n",pla,c0,c1);
      if(c0==pla)
        {
        nprint(pla,"Impossible.");
        return(-1);
        }
      general.relation[pla][c0]=c1;
      nprint(pla,"OK.");
      return(1);
    case POSSESS:/* [0]=star [1]=planet */
      if (mode2) printf("Pla %d:POSSESS Star %d Planet %d\n",pla,c0,c1);
      if(search_att_task(pla,c0)==TASKNO)
        {
        nprint(pla,"No fighting taskforce at star.");
        return(-1);
        }
      if(star[c0].planet[c1].size==0)
        {
        nprint(pla,"No such planet.");
        return(-1);
        }
      if(star[c0].planet[c1].ruler==pla)
        {
        nprint(pla,"Planet already controlled.");
        return(-1);
        }
      if(star[c0].planet[c1].population==0)
        {
        nprint(pla,"Planet uninhabited.");
        return(-1);
        }
      for(i=0;i<PLAYERNO;i++) if (i!=pla && general.relation[i][pla]!=ALLIANCE && search_att_task(i,c0)!=TASKNO) break;
      if(i!=PLAYERNO)
        {
        nprint(pla,"Other powers present.");
        return(-1);
        }
      if(general.techlevel[star[c0].planet[c1].ruler]>general.techlevel[pla])
        general.techlevel[pla]+=MIN(general.techlevel[star[c0].planet[c1].ruler]-general.techlevel[pla],star[c0].planet[c1].industry/2);
      star[c0].planet[c1].ruler=pla;
      update(c0);
      nprint(pla,"OK.");
      return(1);
    default   :;/*ERROR*/
      if (mode2) printf("Pla %d:ERROR %d\n",pla,co->com);
      return(-1);
    }
  }
display_check(pla)
int pla;
  {
  if(waittime>general.time) return(0);
  if(pla==PLAYERNO || mode2 || mode3) return(1);
  if(pla==-1) return(0);
  if(comp_cmp("klingon",general.pname[pla])) return(0);
  if(comp_cmp("romulan",general.pname[pla])) return(0);
  if(comp_cmp("alien",general.pname[pla])) return(0);
  if(comp_cmp("berserker",general.pname[pla])) return(0);
  if(comp_cmp("dead",general.pname[pla])) return(0);
  return(1);
  }
fight(sta)
int sta;
  {
  int speed,tas,shi,pla1,pla2,i,j,disp;
  char *c=" ";
  for(pla1=0;pla1<PLAYERNO;pla1++)
    {
    if(search_att_task(pla1,sta)==TASKNO) continue;
    for(pla2=0;pla2<PLAYERNO;pla2++)
      if(search_task(pla2,sta)!=TASKNO && general.relation[pla1][pla2]==WAR) break;
    if(pla2<PLAYERNO) break;
    }
  if(pla1==PLAYERNO) return(0);
  for(pla2=0;pla2<PLAYERNO;pla2++)
    {
    if(!knowstar(pla2,sta)) continue;
    if(display_check(pla2)) break;
    }
  if(pla2<PLAYERNO) disp=PLAYERNO;else disp=-1;
  c[0]='A'+sta;
  cprint(disp,"COSMIC CONQUEST","","Battle at Star",c,"");
  starsummary(disp,sta);
  for(pla1=0;pla1<PLAYERNO;pla1++)
    {
    task[pla1][TASKNO].headed=-1;
    for(shi=0;shi<SHIPNO;shi++)
      {
      task[pla1][TASKNO].ship_free[shi]=0;
      task[pla1][TASKNO].ship_load[shi]=0;
      }
    for(tas=0;tas<TASKNO;tas++)
      if(ATSTAR(pla1,tas,sta))
        {
        for(shi=0;shi<SHIPNO;shi++)
          {
          set_ships_free(pla1,TASKNO,shi,((int) task[pla1][TASKNO].ship_free[shi])+task[pla1][tas].ship_free[shi]);
          set_ships_load(pla1,TASKNO,shi,((int) task[pla1][TASKNO].ship_load[shi])+task[pla1][tas].ship_load[shi]);
          } 
        task[pla1][tas].headed=-1;
        task[pla1][TASKNO].headed=sta;
        task[pla1][TASKNO].arriving=0;
        }
    i=0;
    for(shi=0;shi<SHIPNO;shi++) if(ship[shi].special&FASTEJECTER) i+=task[pla1][TASKNO].ship_free[shi]*ship[shi].capacity;
    for(shi=0;shi<SHIPNO && i>0;shi++)
      {
      j=0;
      if(ship[shi].special&FASTEJECTABLE) j=MIN(i/ship[shi].size,task[pla1][TASKNO].ship_load[shi]);
      set_ships_free(pla1,TASKNO,shi,task[pla1][TASKNO].ship_free[shi]+j);
      task[pla1][TASKNO].ship_load[shi] -= j;
      i -= j*ship[shi].size;
      }
    if (task[pla1][TASKNO].headed!=-1)
      {
      tasksummary(disp,pla1,TASKNO);
      wait_space(disp);
      }
    }
  fight_now(disp);
  for(pla1=0;pla1<PLAYERNO;pla1++)
    if(popore_task(pla1,TASKNO))
      {
      i=0;
      for(pla2=0;pla2<PLAYERNO;pla2++)
        if(att_task(pla2,TASKNO)&&general.relation[pla2][pla1]==WAR) i++;
      if(i==0) continue;
      i=INTASK(pla1,TASKNO,OREUNIT)/i;
      task[pla1][TASKNO].ship_free[POPUNIT]=task[pla1][TASKNO].ship_load[POPUNIT]=0;
      task[pla1][TASKNO].ship_free[OREUNIT]=task[pla1][TASKNO].ship_load[OREUNIT]=0;
      for(pla2=0;pla2<PLAYERNO;pla2++)
        if(att_task(pla2,TASKNO)&&general.relation[pla2][pla1]==WAR)
          set_ships_free(pla2,TASKNO,OREUNIT,task[pla2][TASKNO].ship_free[OREUNIT]+i);
      }
  for(pla1=0;pla1<PLAYERNO;pla1++)
    {
    tas=get_task(pla1,sta);
    if (tas==-1) continue;
    for(shi=0;shi<SHIPNO;shi++)
      {
      task[pla1][tas].ship_free[shi]=task[pla1][TASKNO].ship_free[shi];
      task[pla1][tas].ship_load[shi]=task[pla1][TASKNO].ship_load[shi];
      }
    clean_task(pla1);
    }
  wait_space(disp);
  clearall(disp);
  return(1);
  }
fight_now(disp)
int disp;
  {
  int cspeed,shi1,shi2,pla1,pla2,pla3,pl,x,ano,target,last;
  register int attack;
  int plashi[PLAYERNO];
  int platar[PLAYERNO];
  for(pla1=0;pla1<PLAYERNO;pla1++)
    {
    platar[pla1]=0;
    for(pla2=0;pla2<PLAYERNO;pla2++)
      if(general.relation[pla1][pla2]==WAR || general.relation[pla2][pla1]==WAR) platar[pla1]+=task_size_po(pla2,TASKNO);
    }
  for(cspeed=0;cspeed<MAXSPEED;cspeed++)
  for(shi1=0;shi1<SHIPNO;shi1++)
    {
    if(ship[shi1].attackno==0 || ship[shi1].speed!=cspeed) continue;
    pl=RANDOM(PLAYERNO);
    last=x;
    for(pla1=0;pla1<PLAYERNO;pla1++) plashi[pla1]=task[pla1][TASKNO].ship_free[shi1];
    for(x=pl;x<=last+PLAYERNO;x++)
      {
      pla1=x%PLAYERNO;
      if(plashi[pla1]!=0)
        {
        last=x;
        plashi[pla1]--;
        for(ano=0;ano<ship[shi1].attackno;ano++)
          {
          if(platar[pla1]<=0) continue;
          target=RANDOM(platar[pla1]);
          for(pla2=0;pla2<PLAYERNO;pla2++)
            {
            if(general.relation[pla1][pla2]!=WAR && general.relation[pla2][pla1]!=WAR) continue;
            for(shi2=0;shi2<SHIPNO-2;shi2++)
              {
              target -= task[pla2][TASKNO].ship_free[shi2]*ship[shi2].size;
              if(target<0) break;
              }
            if(shi2!=SHIPNO-2) break;
            }
          attack  = RSGN() * ship[shi1].attack;
          attack += RSGN() * ship[shi1].attack;
          attack += RSGN() * ship[shi1].attack;
          attack += RSGN() * ship[shi1].attack;
          attack = ship[shi1].attack+attack/4;
          if(attack<1)
            {
            output_fight(disp,pla1,shi1,pla2,shi2,"Miss");
            continue;
            }
          if(ship[shi2].shields>attack)
            {
            output_fight(disp,pla1,shi1,pla2,shi2,"Shields");
            continue;
            }
          output_fight(disp,pla1,shi1,pla2,shi2,"Destroyed");
          task[pla2][TASKNO].ship_free[shi2]--;
          if(ship[shi1].special&KAMIKAZE)
            {
            task[pla1][TASKNO].ship_free[shi1]--;
            for(pla3=0;pla3<PLAYERNO;pla3++)
              if(general.relation[pla3][pla1]==WAR || general.relation[pla1][pla3]==WAR) platar[pla3]-= ship[shi1].size;
            }
          for(pla3=0;pla3<PLAYERNO;pla3++)
            if(general.relation[pla3][pla2]==WAR || general.relation[pla2][pla3]==WAR) platar[pla3]-= ship[shi2].size;
          }
        }
      }
    }
  }
search_att_task(pla,sta)
int pla;
int sta;
  {
  int tas;
  for(tas=0;tas<TASKNO;tas++)
    if(ATSTAR(pla,tas,sta) && att_task(pla,tas)) return(tas);
  return(TASKNO);
  }
att_task(pla,tas)
int pla;
int tas;
  {
  int shi;
  if(task[pla][tas].headed==-1) return(0);
  for(shi=0;shi<SHIPNO-2;shi++) if(INTASK(pla,tas,shi) && ship[shi].attackno) return(1);
  return(0);
  }
popore_task(pla,tas)
int pla;
int tas;
  {
  int shi;
  for(shi=0;shi<SHIPNO-2;shi++) if(INTASK(pla,tas,shi)) return(0);
  return(1);
  }
exists(pla)
int pla;
  {
  int t,s,p;
  for(s=0;s<STARNO;s++) for(p=0;p<PLANETNO;p++)
    if(star[s].planet[p].poptype==pla && star[s].planet[p].ruler==pla && star[s].planet[p].population>0) return(1);
  for(t=0;t<TASKNO;t++) if(task[pla][t].headed!=-1 && INTASK(pla,t,POPUNIT)) return(1);
  return(0);
  }
population_no(pla)
int pla;
  {
  int i=0,s,p;
  for(s=0;s<STARNO;s++) for(p=0;p<PLANETNO;p++)
        if(star[s].planet[p].poptype==pla) i += star[s].planet[p].population;
  return(i);
  }
industry_no(pla)
int pla;
  {
  int i=0,s,p;
  for(s=0;s<STARNO;s++) for(p=0;p<PLANETNO;p++) 
    if(star[s].planet[p].ruler==pla) 
      i += star[s].planet[p].industry;
  return(i);
  }
nearstar(sta)
int sta;
  {
  int sta1=-1,dis,j;
  dis=1000;
  for(j=0;j<STARNO;j++) if(j!=sta && distance(sta,j)<dis) {sta1=j;dis=distance(sta,sta1);}
  return(sta1);
  }
set_ships_free(pla,tas,shi,no)
int pla;
int tas;
int shi;
int no;
  {
  if(no<0) no=0;
  if(no>250) no=250;
  task[pla][tas].ship_free[shi]=no;
  }
set_ships_load(pla,tas,shi,no)
int pla;
int tas;
int shi;
int no;
  {
  if(no<0) no=0;
  if(no>250) no=250;
  task[pla][tas].ship_load[shi]=(unsigned char) no;
  }
comp_cmp(name,str)
char *name;
char *str;
  {
  while(_tolower(*name)==_tolower(*str) && *name!='\0') {name++;str++;}
  if(*name=='\0') return(1);
  return(0);
  }
spread_stars()
  {
  int sta,sta2;
  for(sta=0;sta<STARNO;sta++)
    {
    star[sta].x=RANDOM(XMAX);
    star[sta].y=RANDOM(YMAX);
    for(sta2=0;sta2<sta;sta2++)
      if(star[sta2].x==star[sta].x && star[sta2].y==star[sta].y)
        {
        sta--;
        break;
        }
    }
  }
spread_players()
  {
  int i,j,k,l;
  for(i=0;i<PLAYERNO;i++)
    {
    task[i][0].headed=-1;
    for(l=0;l<100 && task[i][0].headed==-1;l++)
      {
      j=RANDOM(STARNO);
      if(distance(j,nearstar(j))>4) continue;
      for(k=0;k<i;k++) if(task[k][0].headed==j || distance(task[k][0].headed,j)<8) break;
      if(k==i) task[i][0].headed=j;
      }
    if(l==100) return(1);
    }
  return(0);
  }
update(sta)
int sta;
  {
  int p,pla,pla2,tas,ruler,totsize,totpop,totind;
  ruler=controlstar(sta);
  totsize=0;
  totpop=0;
  totind=0;
  for(p=0;p<PLANETNO;p++)
    {
    totsize+=star[sta].planet[p].size;
    totpop+=star[sta].planet[p].population;
    totind+=star[sta].planet[p].industry;
    }
  for(pla=0;pla<PLAYERNO;pla++)
    {
    if(!knowstar(pla,sta)) continue;
    CommData[pla].star[sta].ruler=ruler;
    CommData[pla].star[sta].totsize=totsize;
    CommData[pla].star[sta].totpop=totpop;
    CommData[pla].star[sta].totind=totind;
    CommData[pla].star[sta].danger=danger(pla,sta);
    CommData[pla].star[sta].when=general.time;
    }
  }
danger(pla,sta)
int pla;
int sta;
  {
  int pl,danger;
  for(pl=0;pl<PLAYERNO;pl++)
    if(pl!=pla)
      switch(general.relation[pl][pla])
        {
        case PEACE:danger += fi_pres(pl,sta);
                   break;
        case WAR  :danger += fi_pres(pl,sta)*2;
                   break;
        case ALLIANCE:danger += fi_pres(pl,sta)/2;
                   break;
        }
  return(danger);
  }
fi_pres(pla,sta)
int pla;
int sta;
  {
  int pres,tas;
  pres=0;
  for(tas=0;tas<TASKNO;tas++) if(ATSTAR(pla,tas,sta)) pres+=fi_strength(pla,tas);
  return(pres);
  }
fi_strength(pla,tas)
int pla;
int tas;
  {
  int shi,str;
  str=0;
  for(shi=0;shi<SHIPNO;shi++) str+=task[pla][tas].ship_free[shi]*fi_value(shi);
  return(str);
  }
fi_value(shi)
int shi;
  {
  if(ship[shi].attack*ship[shi].attackno==0) return(0);
  return(ship[shi].attack*ship[shi].attackno+ship[shi].shields*ship[shi].shields/5);
  }
#include "klingon.c"
#include "berserker.c"
#include "alien.c"
#include "romulan.c"
