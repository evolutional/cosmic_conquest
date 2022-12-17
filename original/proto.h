
/* alien.c */
int alien ( int pla );

/* berserker.c */
int berserker ( int pla );

/* conquest.c */
int conquest ( void );
int load ( char *str );
int save ( char *str );
int init ( void );
int deinit ( void );
int task_speed ( int pla , int tas );
int task_range ( int pla , int tas );
int task_capa ( int pla , int tas );
int task_visi ( int pla , int tas );
int task_free ( int pla , int tas );
int task_size ( int pla , int tas );
int task_size_po ( int pla , int tas );
int knowstar ( int pla , int sta );
int headedstar ( int pla , int sta );
int controlstar ( int sta );
int rule_star ( int pl1 , int pl2 );
int search_task ( int pla , int sta );
int clean_task ( int pla );
int process_turn ( void );
int get_task ( int pla , int sta );
int make_task ( int pla , int sta );
int playon ( void );
int distance ( int sta1 , int sta2 );
int do_command ( int pla , struct NCommand *co );
int display_check ( int pla );
int fight ( int sta );
int fight_now ( int disp );
int search_att_task ( int pla , int sta );
int att_task ( int pla , int tas );
int popore_task ( int pla , int tas );
int exists ( int pla );
int population_no ( int pla );
int industry_no ( int pla );
int nearstar ( int sta );
int set_ships_free ( int pla , int tas , int shi , int no );
int set_ships_load ( int pla , int tas , int shi , int no );
int comp_cmp ( char *name , char *str );
int spread_stars ( void );
int spread_players ( void );
int update ( int sta );
int danger ( int pla , int sta );
int fi_pres ( int pla , int sta );
int fi_strength ( int pla , int tas );
int fi_value ( int shi );

/* human.c */
int human ( int pla );
int make_displ ( int pla );
int input_action ( int line , int pla );
int input_task ( int line , int pla );
int input_task_or_ret ( int line , int pla );
int input_ships_task ( int line , int pla , struct NCommand *co );
int input_many_tasks ( int line , int pla , struct NCommand *co );
int input_star ( int line , int pla );
int input_star_or_ret ( int line , int pla );
int input_player_or_ret ( int line , int pla );
int input_planet ( int line , int pla );
int input_planet_or_ret ( int line , int pla );
int input_ship_in_task ( int line , int pla , int tas );
int input_product ( int line , int pla , int tech );
int input_ship ( int line , int pla );
int input_status ( int line , int pla , int status );
int input_number ( int line , int pla , int max );
int input_player ( int line , int pla );
int input_player_pref ( int line , int pla );
int cprint ( int pla , char *str1 , char *str2 , char *str3 , char *str4 , char *str5 );
int nprint ( int pla , char *str );
int randomize ( void );
int cget ( int pla );
int coget ( void );
int wait_space ( int pla );
int cinput ( char *str , int line );
int mapdisplay ( int pla );
int starsummary ( int pla , int sta );
int tasksummary ( int pla1 , int pla , int tas );
int shipsummary ( int pla , int shi );
int showstatus ( int pla );
int mstr ( int no , char *str );
int rstr ( int no , char *str );
int tstr ( int no , char *str );
int qstr ( int no , char *str );
int pstr ( int rel , char *str );
int clearall ( int pla );
int showtasks ( int pla1 , int pla );
int output_fight ( int pla , int pla1 , int shi1 , int pla2 , int shi2 , char *str );
int tshipsummary ( int pla1 , int pla , int shi );
int showhelp ( int pla );
int showstars ( int pla );
int process_mouse ( int pla , int x , int y );
int planetsummary ( int pla1 , int sta , int p );
int sortstars ( int sta );

/* klingon.c */
int klingon ( int pla );
int fi_star ( int pla , int sta );
int tr_star ( int pla , int sta );
int fi_need ( int pla , int sta );
int fi_send ( int pla , int sta );
int best_fi ( int pla , int sta );
int tr_need ( int pla , int sta );
int tr_pres ( int pla , int sta );
int tr_send ( int pla , int sta );
int tr_value ( int shi );
int best_tr ( int pla , int sta );
int pop_pres ( int pla , int sta );
int pop_need ( int pla , int sta );
int pop_send ( int pla , int sta );
int pop_star ( int pla , int sta );
int ore_pres ( int pla , int sta );
int ore_need ( int pla , int sta );
int ore_send ( int pla , int sta );
int ore_star ( int pla , int sta );

/* main.c */
int main ( void );
int cleanup ( char *str );
int title ( void );

/* romulan.c */
int romulan ( int pla );
