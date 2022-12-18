klingon(pla)
int pla;
  {
  struct NCommand c;
  int i,j,k,shi,shival,sta,sta2,tas,p,dis;
  /*Join all tasks at all stars*/
  c.com=JOIN;
  for(sta=0;sta<STARNO;sta++)
    if(knowstar(pla,sta))
    {
    j=0;
    for(i=0;i<TASKNO;i++) if (ATSTAR(pla,i,sta)) c.par[j++]=i;
    c.par[j]=TASKNO;
    if (j>1) do_command(pla,&c);
    }
  /* Uncarry all ships*/
  c.com=CARRY;
  for(shi=0;shi<SHIPNO;shi++) c.par[shi+1]=0;
  for(tas=0;tas<TASKNO;tas++)
    if(task[pla][tas].headed!=-1 && task[pla][tas].arriving<=general.time)
      {
      c.par[0]=tas;
      do_command(pla,&c);
      }
  /* Declare war at all who have declared war*/
  c.com=DECLARE;
  c.par[1]=WAR;
  for(i=0;i<PLAYERNO;i++)
    if (i!=pla && general.relation[i][pla]==WAR && general.relation[pla][i]!=WAR)
      {
      c.par[0]=i;
      do_command(pla,&c);
      }
  /* Declare war on concurrents and possess all planets*/
  for(tas=0;tas<TASKNO;tas++)
    {
     if(task[pla][tas].headed==-1 || task[pla][tas].arriving>general.time) continue;
    sta=task[pla][tas].headed;
    for(i=0;i<PLAYERNO;i++)
      if(i!=pla && search_task(i,sta)!=TASKNO && general.relation[i][pla]==PEACE)
        {
        c.com=DECLARE;
        c.par[0]=i;
        c.par[1]=WAR;
        do_command(pla,&c);
        }
    for(p=0;p<PLANETNO;p++)
      if(star[sta].planet[p].population>0 && star[sta].planet[p].ruler!=pla)
        {
        c.com=POSSESS;
        c.par[0]=sta;
        c.par[1]=p;
        do_command(pla,&c);
        }
    }
  /* Declare War on invaders */
  c.com=DECLARE;
  c.par[1]=WAR;
  for(sta=0;sta<STARNO;sta++) if(knowstar(pla,sta)) for(p=0;p<PLANETNO;p++)
    if(star[sta].planet[p].poptype==pla && star[sta].planet[p].ruler!=pla)
      {
      c.par[0]=star[sta].planet[p].ruler;
      do_command(pla,&c);
      }
  /* Land Tasks */
  c.com=LAND;
  for(tas=0;tas<TASKNO;tas++)
    {
    if(task[pla][tas].headed==-1 || task[pla][tas].arriving>general.time) continue;
    sta=task[pla][tas].headed;
    c.par[0]=tas;
    for(p=0;p<PLANETNO;p++)
      {
      if(task[pla][tas].ship_free[POPUNIT]+task[pla][tas].ship_free[OREUNIT]==0) continue;
      if(star[sta].planet[p].ruler!=pla && star[sta].planet[p].population>0) continue;
      if(star[sta].planet[p].size==0) continue;
      if(star[sta].planet[p].size<=star[sta].planet[p].industry) continue;
      c.par[1]=p;
      do_command(pla,&c);
      }
    }
  /* Set production on all planets*/
  c.com=PRODUCE;
  for(sta=0;sta<STARNO;sta++) for(p=0;p<PLANETNO;p++)
    if(star[sta].planet[p].ruler==pla && star[sta].planet[p].industry!=0 && star[sta].planet[p].ipoints<star[sta].planet[p].industry)
    {
    c.par[0]=sta;
    c.par[1]=p;
    tas=search_task(pla,sta);
    if(star[sta].planet[p].mineable)
      {
      c.par[2]=MINING;
      if(star[sta].planet[p].producing!=c.par[2]) do_command(pla,&c);
      continue;
      }
    if(5*fi_need(pla,sta)/4>fi_pres(pla,sta))
      {
      c.par[2]=best_fi(pla,sta);
      if(star[sta].planet[p].producing!=c.par[2]) do_command(pla,&c);
      continue;
      }
    if(tr_need(pla,sta)>tr_pres(pla,sta))
      {
      c.par[2]=best_tr(pla,sta);
      if(star[sta].planet[p].producing!=c.par[2]) do_command(pla,&c);
      continue;
      }
    if(star[sta].planet[p].industry+star[sta].planet[p].industry/ICOST<star[sta].planet[p].population)
      {
      c.par[2]=INVESTING;
      if(star[sta].planet[p].producing!=c.par[2]) do_command(pla,&c);
      continue;
      }
    c.par[2]=RESEARCHING;
    if(star[sta].planet[p].producing!=c.par[2]) do_command(pla,&c);
    }
  /* Send all fighting taskforces */
  for(tas=0;tas<TASKNO;tas++) if((sta=task[pla][tas].headed)!=-1 && task[pla][tas].arriving<=general.time)
    while(fi_pres(pla,sta)>fi_need(pla,sta))
    {
    sta2=fi_star(pla,sta);
    if(sta2==-1) break;
    dis=distance(sta,sta2);
    i=fi_pres(pla,sta )-fi_need(pla,sta );
    j=fi_need(pla,sta2)-fi_send(pla,sta2);
    k=fi_pres(pla,sta )-fi_need(pla,sta );
    i=MIN(i,j);
    j=0;
    c.com=SPLIT;
    c.par[0]=tas;
    for(shi=0;shi<SHIPNO;shi++) c.par[shi+1]=0;
    shi=0;
    while(j<i && shi!=SHIPNO)
      for(shi=0;shi<SHIPNO;shi++)
        if(task[pla][tas].ship_free[shi]>c.par[shi+1] && ship[shi].hyprange>=dis && fi_value(shi)<=k && fi_value(shi)!=0)
          {
          c.par[shi+1]++;
          j+= fi_value(shi);
          k-= fi_value(shi);
          break;
          }
    for(shi=0;shi<SHIPNO;shi++) if(c.par[shi+1]!=0) break;
    if(shi==SHIPNO) break;
    c.par[0]=do_command(pla,&c);
    if(c.par[0]==-1) break;
    c.com=DESTINATION;
    c.par[1]=sta2;
    if(do_command(pla,&c)==-1) break;
    }
  /* Send POPUNITS */
  for(tas=0;tas<TASKNO;tas++) if((sta=task[pla][tas].headed)!=-1 && task[pla][tas].arriving<=general.time)
    while(pop_pres(pla,sta)>pop_need(pla,sta))
    {
    sta2=pop_star(pla,sta);
    if(sta2==-1) break;
    dis=distance(sta,sta2);
    i=pop_pres(pla,sta )-pop_need(pla,sta );
    j=pop_need(pla,sta2)-pop_send(pla,sta2);
    i=MIN(i,j);
    j=0;
    k=fi_pres(pla,sta)-fi_need(pla,sta);
    c.com=SPLIT;
    c.par[0]=tas;
    for(shi=0;shi<SHIPNO;shi++) c.par[shi+1]=0;
    shi=0;
    while(i>j && shi!=SHIPNO)
      for(shi=0;shi<SHIPNO;shi++)
        if(task[pla][tas].ship_free[shi]>c.par[shi+1] && ship[shi].hyprange>=dis && (k>=fi_value(shi)||fi_value(shi)<=0) && ship[shi].capacity>0)
          {
          c.par[shi+1]++;
          j+= ship[shi].capacity;
          k-= fi_value(shi);
          break;
          }
    for(shi=0;shi<SHIPNO;shi++) if(c.par[shi+1]!=0) break;
    if(shi==SHIPNO) break;
    i=MIN(i,j);
    c.par[POPUNIT+1]=MIN(i,task[pla][tas].ship_free[POPUNIT]);
    c.par[0]=do_command(pla,&c);
    if(c.par[0]==-1) break;
    c.com=DESTINATION;
    c.par[1]=sta2;
    if(do_command(pla,&c)==-1) break;
    }
  /* Send OREUNITS */
  for(tas=0;tas<TASKNO;tas++) if((sta=task[pla][tas].headed)!=-1 && task[pla][tas].arriving<=general.time)
    while(ore_pres(pla,sta)>ore_need(pla,sta))
    {
    sta2=ore_star(pla,sta);
    if(sta2==-1) break;
    dis=distance(sta,sta2);
    i=ore_pres(pla,sta )-ore_need(pla,sta );
    j=ore_need(pla,sta2)-ore_send(pla,sta2);
    i=MIN(i,j);
    j=0;
    k=fi_pres(pla,sta)-fi_need(pla,sta);
    c.com=SPLIT;
    c.par[0]=tas;
    for(shi=0;shi<SHIPNO;shi++) c.par[shi+1]=0;
    shi=0;
    while(i>j && shi!=SHIPNO)
      for(shi=0;shi<SHIPNO;shi++)
        if(task[pla][tas].ship_free[shi]>c.par[shi+1] && ship[shi].hyprange>=dis && (k>=fi_value(shi)||fi_value(shi)<=0) && ship[shi].capacity>0)
          {
          c.par[shi+1]++;
          j+= ship[shi].capacity;
          k-= fi_value(shi);
          break;
          }
    for(shi=0;shi<SHIPNO;shi++) if(c.par[shi+1]!=0) break;
    if(shi==SHIPNO) break;
    i=MIN(i,j);
    c.par[OREUNIT+1]=MIN(i,task[pla][tas].ship_free[OREUNIT]);
    c.par[0]=do_command(pla,&c);
    if(c.par[0]==-1) break;
    c.com=DESTINATION;
    c.par[1]=sta2;
    if(do_command(pla,&c)==-1) break;
    }
  /* Send all empty transporters */
  for(tas=0;tas<TASKNO;tas++) if((sta=task[pla][tas].headed)!=-1 && task[pla][tas].arriving<=general.time)
    while(tr_pres(pla,sta)>tr_need(pla,sta))
    {
    sta2=tr_star(pla,sta);
    if(sta2==-1) break;
    dis=distance(sta,sta2);
    i=tr_pres(pla,sta )-tr_need(pla,sta );
    j=tr_need(pla,sta2)-tr_send(pla,sta2);
    i=MIN(i,j);
    j=0;
    k=fi_pres(pla,sta)-fi_need(pla,sta);
    c.com=SPLIT;
    c.par[0]=tas;
    for(shi=0;shi<SHIPNO;shi++) c.par[shi+1]=0;
    shi=0;
    while(i>j && shi!=SHIPNO)
      for(shi=0;shi<SHIPNO;shi++)
        if(task[pla][tas].ship_free[shi]>c.par[shi+1] && ship[shi].hyprange>=dis && (k>=fi_value(shi)||fi_value(shi)<=0))
          {
          c.par[shi+1]++;
          j+= ship[shi].capacity;
          k-= fi_value(shi);
          break;
          }
    for(shi=0;shi<SHIPNO;shi++) if(c.par[shi+1]!=0) break;
    if(shi==SHIPNO) break;
    c.par[0]=do_command(pla,&c);
    if(c.par[0]==-1) break;
    c.com=DESTINATION;
    c.par[1]=sta2;
    if(do_command(pla,&c)==-1) break;
    }
  }
fi_star(pla,sta)
int pla;
int sta;
  {
  int st,b_sta,b_dist;
  b_sta=-1;
  b_dist=1000;
  for(st=0;st<STARNO;st++)
    if(distance(st,sta)<=b_dist && fi_need(pla,st)>fi_send(pla,st))
    {
    b_sta=st;
    b_dist=distance(st,sta);
    }
  return(b_sta);
  }
tr_star(pla,sta)
int pla;
int sta;
  {
  int st,b_sta,b_dist;
  b_sta=-1;
  b_dist=1000;
  for(st=0;st<STARNO;st++)
    if(distance(st,sta)<=b_dist && tr_need(pla,st)>tr_send(pla,st))
    {
    b_sta=st;
    b_dist=distance(st,sta);
    }
  return(b_sta);
  }
fi_need(pla,sta)
int pla;
int sta;
  {
  int p,pl,need;
  need=1;
  if(!knowstar(pla,sta)) return(need);
  for(p=0;p<PLANETNO;p++)
    {
    need+=(star[sta].planet[p].mineable+1)*star[sta].planet[p].size/10;
    need+=star[sta].planet[p].population/10;
    need+=star[sta].planet[p].industry/10;
    }
  need *= general.techlevel[pla]/100;
  need += CommData[pla].star[sta].danger;
  return(need);
  }
fi_send(pla,sta)
int pla;
int sta;
  {
  int send;
  int tas,shi;
  send=0;
  for(tas=0;tas<TASKNO;tas++) if(task[pla][tas].headed==sta)
    for(shi=0;shi<SHIPNO;shi++) send+=task[pla][tas].ship_free[shi]*fi_value(shi);
  return(send);
  }
best_fi(pla,sta)
int pla;
int sta;
  {
  int b_shi,b_val,shi,dis;
  dis=distance(sta,nearstar(sta));
  b_shi=-1;b_val=0;
  for(shi=0;shi<SHIPNO;shi++)
    if(ship[shi].hyprange>=dis && ship[shi].techlevel<=general.techlevel[pla]
       && (20*fi_value(shi))/ship[shi].techcost>b_val)
      {
      b_shi=shi;
      b_val=20*fi_value(shi)/ship[shi].techcost;
      }
  return(b_shi);
  }
tr_need(pla,sta)
int pla;
int sta;
  {
  int shi,tas,p,pl,need,dis;
  need=0;
  dis=distance(sta,nearstar(sta));
  if(!knowstar(pla,sta)) return(0);
  for(tas=0;tas<TASKNO;tas++) if(ATSTAR(pla,tas,sta))
    for(shi=0;shi<SHIPNO;shi++) if(ship[shi].hyprange<dis)
       need += ship[shi].size*task[pla][tas].ship_free[shi];
  return(need);
  }
tr_pres(pla,sta)
int pla;
int sta;
  {
  int pres,tas;
  pres=0;
  for(tas=0;tas<TASKNO;tas++) if(ATSTAR(pla,tas,sta)) pres += task_capa(pla,tas);
  return(pres);
  }
tr_send(pla,sta)
int pla;
int sta;
  {
  int send,tas;
  send=0;
  for(tas=0;tas<TASKNO;tas++) if(task[pla][tas].headed==sta) send += task_capa(pla,tas);
  return(send);
  }
tr_value(shi)
int shi;
  {
  return(ship[shi].hypspeed*ship[shi].capacity);
  }
best_tr(pla,sta)
int pla;
int sta;
  {
  int b_shi,b_val,shi,dis;
  dis=distance(sta,tr_star(pla,sta));
  b_shi=RESEARCHING;b_val=0;
  for(shi=0;shi<SHIPNO;shi++)
    if(ship[shi].hyprange>=dis && ship[shi].techlevel<=general.techlevel[pla]
       && (20*tr_value(shi))/ship[shi].techcost>b_val)
      {
      b_shi=shi;
      b_val=20*tr_value(shi)/ship[shi].techcost;
      }
  return(b_shi);
  }
pop_pres(pla,sta)
int pla;
int sta;
  {
  int pres,tas;
  pres=0;
  for(tas=0;tas<TASKNO;tas++) if(ATSTAR(pla,tas,sta))
    pres += task[pla][tas].ship_free[POPUNIT]+task[pla][tas].ship_load[POPUNIT];
  return(pres);
  }
pop_need(pla,sta)
int pla;
int sta;
  {
  int need,p;
  need=0;
  for(p=0;p<PLANETNO;p++) if((star[sta].planet[p].ruler==pla && star[sta].planet[p].poptype==pla)
                             || star[sta].planet[p].ruler==PLAYERNO)
    need += star[sta].planet[p].size-star[sta].planet[p].population;
  return(need);
  }
pop_send(pla,sta)
int pla;
int sta;
  {
  int send,tas;
  send=0;
  for(tas=0;tas<TASKNO;tas++) if(task[pla][tas].headed==sta)
    send += task[pla][tas].ship_load[POPUNIT]+task[pla][tas].ship_free[POPUNIT];
  return(send);
  }
pop_star(pla,sta)
int pla;
int sta;
  {
  int st,b_sta,b_dist;
  b_sta=-1;
  b_dist=1000;
  for(st=0;st<STARNO;st++)
    if(distance(st,sta)<=b_dist && pop_need(pla,st)>pop_send(pla,st))
    {
    b_sta=st;
    b_dist=distance(st,sta);
    }
  return(b_sta);
  }
ore_pres(pla,sta)
int pla;
int sta;
  {
  int pres,tas;
  pres=0;
  for(tas=0;tas<TASKNO;tas++) if(ATSTAR(pla,tas,sta))
    pres += task[pla][tas].ship_free[OREUNIT]+task[pla][tas].ship_load[OREUNIT];
  return(pres);
  }
ore_need(pla,sta)
int pla;
int sta;
  {
  int need,p;
  need=0;
  for(p=0;p<PLANETNO;p++) if(star[sta].planet[p].ruler==pla)
    need += star[sta].planet[p].population-star[sta].planet[p].industry;
  return(need);
  }
ore_send(pla,sta)
int pla;
int sta;
  {
  int send,tas;
  send=0;
  for(tas=0;tas<TASKNO;tas++) if(task[pla][tas].headed==sta)
    send += task[pla][tas].ship_free[OREUNIT]+task[pla][tas].ship_load[OREUNIT];
  return(send);
  }
ore_star(pla,sta)
int pla;
int sta;
  {
  int st,b_sta,b_dist;
  b_sta=-1;
  b_dist=1000;
  for(st=0;st<STARNO;st++)
    if(distance(st,sta)<=b_dist && ore_need(pla,st)>ore_send(pla,st))
    {
    b_sta=st;
    b_dist=distance(st,sta);
    }
  return(b_sta);
  }
