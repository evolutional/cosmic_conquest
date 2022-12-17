berserker(pla)
int pla;
  {
  struct NCommand c;
  int i;
  c.com=DECLARE;
  c.par[1]=WAR;
  for(c.par[0]=0;c.par[0]<PLAYERNO;c.par[0]++) if(general.relation[pla][c.par[0]]!=WAR && pla!=c.par[0]) do_command(pla,&c);
  /* Only mini routine programmed */
  /* Fully implemented in the rest of the code and only waiting to be programmed */
  /* with any interesting strategical idea */
  /* (This core routine and reading Fred Saberhagens Berserker books should give */
  /* you an idea of their 'strategy'(yet to be programmed) )*/
  }
