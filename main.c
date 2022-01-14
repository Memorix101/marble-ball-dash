#include <acknex.h>
#include <ackphysx.h>
//#include <default.c>
#include <mtlView.c>
#include <mtlFX.c>
#include <shadows.c>
#include <particles.c>
#include <windows.h>

#define PRAGMA_PATH "fonts";
#define PRAGMA_PATH "levels";
#define PRAGMA_PATH "models";
#define PRAGMA_PATH "music";
#define PRAGMA_PATH "sounds";
#define PRAGMA_PATH "sprites";
#define PRAGMA_PATH "textures";

// player skills
#define time_limit skill1
#define coin_limit skill2

var timer_seconds = 60;
var collected_coins = 0;
var number_coins = 0;
var level_passed = false;
var player_is_ready = false;
var freeze_cam = false;

VECTOR cam_offset;
var cam_rot_angle = 0;
var cam_rot_height = 0;

SOUND* coin_wav = "coin.wav";
SOUND* clapping_wav = "clapping.wav";
SOUND* jump_wav = "ball.wav";

#include "ui.c"
#include "triggers.c"
#include "entities.c"

function orbit_cam()
{	
   if (player != NULL)
	{
		camera.x = player.x;
		camera.y = player.y;
		camera.z = player.z;
		ready_up(); // call it when player is found
		if(freeze_cam == false){
			cam_rot_angle += mickey.x * 4;
			cam_rot_height = clamp(cam_rot_height + mickey.y * 2,0,45);
		}
		vec_set(cam_offset,vector(-350,0,100));
		vec_rotate(cam_offset,vector(cam_rot_angle,-cam_rot_height,0));
		vec_set(camera.x,player.x);
		vec_add(camera.x,cam_offset);
		camera.pan = cam_rot_angle;
		camera.tilt = -cam_rot_height;
	}
}

function song_startup()
{
	var song_id = integer(random(5))+1;
	
	if(media_playing(media_handle) == 0)
	{
		STRING* tmp;
		tmp = str_create("music/track");		
		str_cat_num(tmp,"%.0f", song_id);
		str_cat(tmp, ".ogg");
		media_play(tmp, NULL, 100);
   }
}

function exit_event() 
{ 
	RemoveFontResource("Fonts/Asimov.ttf");
	quit_program();
}

function level_load_complete()
{
	pXent_settype (level_ent, PH_STATIC, PH_POLY); // set the physics entity type
	pXent_setfriction (level_ent, 100); // set the friction
}

function main()
{	
	fps_max = 60;	// limit the frame rate to 60 fps 		
	video_set(1024, 768, 32, 2);
	
	mouse_mode = 4;
	
	on_level_load = level_load_complete;
	on_exit = exit_event; // called when the engine is shutting down
	on_close = exit_event; // The event is executed when the application window close icon is clicked at.
	
	AddFontResource("Fonts/Asimov.ttf"); // neat trick from AUM 114 how to use TTF without installing it
	wait(1);
	
	setup_ui();
	
	physX_open();
	
	ent_createlayer("skycube+6.bmp", SKY | CUBE | SHOW, 0); //Layer and sky entities created before the first level_load call are not removed on level change
	level_load("menu.wmb");
	
	if (d3d_shaderversion >= 3) // does only run if shadermodel v3 is found AND A8 Commercial or Pro is used
	{
  		shadow_stencil = 8; // activate external shadows
	
  		camera.clip_far = 15000;
  		camera.clip_near = 0.1;
  		pssm_fbias = 0.0005;
		pssm_run(3);		
		stencil_blur(3);
	}
	else
	{
		shadow_stencil = 0;
	}	

	set(menu_pan, SHOW);
	
	while(1)
	{
		// debug stuff
		//draw_text("Beta Version", 10, screen_size.y - 30, vector(255, 255, 255));
		//draw_text(level_name, 10, screen_size.y - 50, vector(255, 255, 255));
		//DEBUG_VAR(media_playing(media_handle), 10);
		
		song_startup();
		if(player)
		{
			orbit_cam();
			level_timer();
		}
		
		wait(1);
	}
}