
FONT* game_font = "Asimov#32b";

STRING* level_str = "";

TEXT* time_txt =
{
    font = game_font;
    string("Time:");
    layer = 10;
    pos_x = 20;
    pos_y = 20;
    flags = OUTLINE;
}

TEXT* level_txt =
{
    font = game_font;
    string(level_str);
    layer = 10;
    pos_x = 0;
    pos_y = 20;
    flags = OUTLINE;
}

PANEL* time_display =
{
    digits(100, 20, "%.2f", game_font, 1, timer_seconds);
    layer = 10;
    flags = OUTLINE;
}

TEXT* coin_pan =
{
    font = game_font;
    string("Coins:");
    layer = 10;
    pos_x = 670;
    pos_y = 20;
    flags = OUTLINE;
}

PANEL* coin_digits_pan =
{
	digits(750, 20, 3, "Asimov#32b", 1, collected_coins);
	layer = 10;
	flags = OUTLINE;
}

PANEL* game_over_pan =
{
  bmap = "game_over.png";
  layer = 10;
  pos_x = 0;     
  pos_y = 0;    
  button (50, 570,"restart.png","restart.png","restart_over.png", restart_level, NULL, NULL);
  button (50, 640,"quit.png","quit.png","quit_over.png", back_to_menu, NULL, NULL);
  flags = OVERLAY | TRANSLUCENT;
}

PANEL* win_pan =
{
  bmap = "win.png";
  layer = 10;
  pos_x = 0;     
  pos_y = 0;    
  //button (50, 570,"restart.png","restart.png","restart_over.png", restart_level, NULL, NULL);
  button (50, 640,"quit.png","quit.png","quit_over.png", back_to_menu, NULL, NULL);
  flags = OVERLAY | TRANSLUCENT;
}

PANEL* finish_pan =
{
  bmap = "finished.png";
  layer = 10;
  pos_x = 0;     
  pos_y = 0;    
  button (50, 500,"next.png","next.png","next_over.png", next_level, NULL, NULL);
  button (50, 570,"restart.png","restart.png","restart_over.png", restart_level, NULL, NULL);
  button (50, 640,"quit.png","quit.png","quit_over.png", back_to_menu, NULL, NULL);
  flags = OVERLAY | TRANSLUCENT;
}

PANEL* ready_pan =
{
  bmap = "ready.png";
  layer = 10;
  pos_x = 0;     
  pos_y = 0;    
  flags = OVERLAY | TRANSLUCENT;
}

PANEL* pause_pan =
{
  bmap = "pause.png";
  layer = 10;
  pos_x = 0;     
  pos_y = 0;    
  button (50, 500,"resume.png","resume.png","resume_over.png", resume_level, NULL, NULL);
  button (50, 570,"restart.png","restart.png","restart_over.png", restart_level, NULL, NULL);
  button (50, 640,"quit.png","quit.png","quit_over.png", back_to_menu, NULL, NULL);
  flags = OVERLAY | TRANSLUCENT;
}

PANEL* menu_pan =
{
  bmap = "menu.png";
  layer = 10;
  pos_x = 0;     
  pos_y = 0;    
  button (50, 500,"new_game.png","new_game.png","new_game_over.png", new_game, NULL, NULL);
  //button (50, 570,"restart.png","restart.png","restart_over.png", restart_level, NULL, NULL); // here could be your options menu
  button (50, 640,"quit.png","quit.png","quit_over.png", quit_program, NULL, NULL);
  flags = OVERLAY | TRANSLUCENT;
}

function setup_ui() // sets up buttons and other ui elements
{
	BMAP* quit_bmp = "quit.png"; 
	pan_setpos(menu_pan,3,1,vector(50,(screen_size.y/2 + 100) + 70*0,0));
	pan_setpos(menu_pan,3,2,vector(50,(screen_size.y/2 + 100) + 70*1,0));
	
	pan_setpos(win_pan,3,1,vector(50,(screen_size.y/2 + 100) + 70*0,0));
	
	pan_setpos(finish_pan,3,1,vector(50,(screen_size.y/2 + 100) + 70*0,0));
	pan_setpos(finish_pan,3,2,vector(50,(screen_size.y/2 + 100) + 70*1,0));
	pan_setpos(finish_pan,3,3,vector(50,(screen_size.y/2 + 100) + 70*2,0));
	
	pan_setpos(pause_pan,3,1,vector(50,(screen_size.y/2 + 100) + 70*0,0));
	pan_setpos(pause_pan,3,2,vector(50,(screen_size.y/2 + 100) + 70*1,0));
	pan_setpos(pause_pan,3,3,vector(50,(screen_size.y/2 + 100) + 70*2,0));
	
	pan_setpos(game_over_pan,3,1,vector(50,(screen_size.y/2 + 100) + 70*0,0));
	pan_setpos(game_over_pan,3,2,vector(50,(screen_size.y/2 + 100) + 70*1,0));
	
	pan_setpos(win_pan,3,1,vector((screen_size.x  - bmap_width(quit_bmp))/2 + 75, screen_size.y - 100, 0));
}

function set_level_text()
{	
	str_cpy(level_str, "You need ");
	str_cat_num(level_str,"%.0f", number_coins);
	str_cat(level_str, " coins to win!");	
	level_txt.pos_x = screen_size.x/2 - str_width(level_str ,game_font)/2;
	coin_pan.pos_x = screen_size.x - str_width("Coins:", game_font) - 60;
	pan_setdigits(coin_digits_pan, 1, screen_size.x - 50, 20,"%2.f",game_font,1,collected_coins);
}

function new_game()
{
	reset(menu_pan, SHOW);	
	level_load("level1.wmb");
}

function quit_program()
{
  sys_exit(NULL);
}

function resume_level()
{
	freeze_mode = 0;
	freeze_cam = false;
	reset(pause_pan, SHOW);
}

function back_to_menu()
{
	reset(game_over_pan, SHOW);
	reset(finish_pan, SHOW);
	reset(win_pan, SHOW);
	reset(pause_pan, SHOW);	
	
	reset(time_txt, SHOW);
	reset(level_txt, SHOW);
	reset(time_display, SHOW);
	reset(coin_pan, SHOW);
	reset(coin_digits_pan, SHOW);
	
	player = NULL;
	
	player_is_ready = false;
	level_load("menu.wmb");
	wait(1);
	timer_seconds = 0;
	number_coins = 0;
	collected_coins = 0;
	level_passed = false;
	freeze_cam = false;
	
	freeze_mode = 0;
	
	set(menu_pan, SHOW);
}

function next_level()
{
	STRING* lvl_name = level_name;
	str_clip(lvl_name, 5);
	str_trunc(lvl_name, 4);

	var level_no = str_to_num(lvl_name)+1;
	str_cpy(lvl_name,"");
	
	STRING* tmp;	
	tmp = str_create("level");
	str_cat_num(tmp,"%.0f", level_no);
	str_cat(tmp, ".wmb");
	
	if(level_no > 10)
	{
		// you have won
		reset(game_over_pan, SHOW);
		reset(finish_pan, SHOW);
		set(win_pan, SHOW);
	}
	else if(level_no <= 10)
	{	
		reset(game_over_pan, SHOW);
		reset(finish_pan, SHOW);
		reset(win_pan, SHOW);
		reset(pause_pan, SHOW);	
	
		reset(time_txt, SHOW);
		reset(level_txt, SHOW);
		reset(time_display, SHOW);
		reset(coin_pan, SHOW);
		reset(coin_digits_pan, SHOW);
	
		player = NULL;
	
		player_is_ready = false;
		level_load(tmp);
		wait(1);
		timer_seconds = 0;
		number_coins = 0;
		collected_coins = 0;
		level_passed = false;
		freeze_cam = false;
	
		freeze_mode = 0;
		wait(1);
	}	
}

function ready_up()
{	
	if(player_is_ready == false)
	{
		freeze_mode = 1;
		set(ready_pan, SHOW); // show
		freeze_cam = true;
		cam_rot_angle = 0;
		cam_rot_height = 0;
		
		if(key_w)
		{
			player_is_ready = true;
			freeze_mode = 0;
			reset(ready_pan, SHOW); // hide
			freeze_cam = false;
		}
	}
}

function gameover_fn()
{
	set(game_over_pan, SHOW);
	freeze_mode = 1;
	freeze_cam = true;
}

function level_timer()
{
	if(timer_seconds > 0 && !level_passed && !freeze_cam)
	{
		timer_seconds -= time_step / 16; // decrement per second;
	}
    
	if(timer_seconds < 0)
	{
		gameover_fn();
	}
}

function restart_level()
{   
	reset(game_over_pan, SHOW);
	reset(finish_pan, SHOW);
	reset(pause_pan, SHOW);
	player_is_ready = false;
	
	level_load(level_name);
	
	timer_seconds = player.time_limit;
	collected_coins = 0;
	level_passed = false;
	player_is_ready = false;
	freeze_cam = false;
}