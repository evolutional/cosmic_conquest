/*
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
  };*/
struct NShip ship[SHIPNO]={
{"Clearer"       ,   3,  0, 0, 0, 1, 1,20, 4,100,620,NOSPEC},
{"UltraStinger"  ,   1,  0,50,50, 3, 8, 1, 4, 60,800,NOSPEC},
{"Stinger"       ,   1,  0, 0, 0, 3, 7, 1, 2, 25,150,FASTEJECTABLE},
{"Light Cruiser" ,   5,  0, 6, 1, 5,10, 1,10,100,100,NOSPEC},
{"Space Cannon"  ,  10,  0, 0, 0, 3,25, 1, 2,120,550,NOSPEC},
{"Destroyer"     ,  12,  0, 9, 2, 6,18, 1,10,110,310,NOSPEC},
{"Battleship"    ,  25,  3, 8, 2, 6,18, 3,18,150,450,FASTEJECTER},
{"Battlestar"    , 950,100, 9, 1, 8,15,15,40,900,950,FASTEJECTER},
{"Heavy Cruiser" ,   8,  0, 8, 1, 7,13, 1,14,120,220,NOSPEC},
{"Mothership"    , 100, 20,11, 1, 6,12, 2,24,230,620,NOSPEC},
{"Satellite"     ,2000,  0, 0, 0, 9,12, 1, 6, 20,660,NOSPEC},
{"Trans III"     ,  20,  8, 9, 2,10, 0, 0, 6,100,360,NOSPEC},
{"UltraTrans"    ,  40, 30,50, 4,10, 0, 0, 8,160,900,NOSPEC},
{"Glitter"       ,  20, 19, 0, 0,10, 0, 0, 0, 10,420,NOSPEC},
{"Scout"         ,   2,  0,11, 3,10, 0, 0, 6, 50,180,NOSPEC},
{"Transporter"   ,  10,  4, 4, 1,10, 0, 0, 6, 40, 50,NOSPEC},
{"Spy Sat"       ,   1,  0, 9, 1,10, 0, 0,70,280,750,INVISIBLE},
{"Torpedo"       ,   1,  0, 0, 0, 1,20, 1, 0, 45,240,KAMIKAZE|FASTEJECTABLE},
{"Pirate"        ,  10,  3, 9, 2, 5, 6, 3,14,180,370,NOSPEC},
{"Carrier"       ,  30,  5, 7, 1,10, 0, 0,28,100,430,FASTEJECTER},
{"Q-Bomb"        ,  30,  0, 0, 0, 0, 0, 0, 0,350,950,STARKILLER},
{"Trans II"      ,  10,  4, 9, 1, 0, 0, 0, 6, 70,210,NOSPEC},
{"Sidewinder"    ,   1,  0, 0, 0, 1, 4, 1, 0,  5,240,FASTEJECTABLE|KAMIKAZE},
{"Space Fighter" ,   2,  0, 0, 0, 3, 6, 2, 6, 60,470,FASTEJECTABLE},
{"Light Carrier" ,  20,  5, 9, 2, 8, 2, 2,20,130,430,FASTEJECTER},
{"Fleet Carrier" ,  30, 10, 8, 1, 8, 2, 3,24,160,520,FASTEJECTER},
{"Defence Sat"   , 200, 20, 0, 0, 9,12,10,30,300,680,FASTEJECTER},
{"Space Station" ,1000,120, 0, 0, 9,12,30,50,800,900,FASTEJECTER},
{"Base Ship"     ,  80, 25,12, 1, 7,12, 2,30,230,620,FASTEJECTER},
{"Ore Unit"      ,   1,  0, 0, 0,10, 0, 0, 0,MCOST,0,NOTPROD},
{"Pop Unit"      ,   1,  0, 0, 0,10, 0, 0, 0, 20,  0,NOTPROD}
};
