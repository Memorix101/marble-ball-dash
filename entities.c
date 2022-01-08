
// sprite-emulated particle function
function p_fountain_sprite(ENTITY* p)
{
   VECTOR vTemp;
   vec_randomize(vTemp, 25);
   vec_add(p._VEL_X, vTemp);
   vec_set(p.blue, vector(random(255),random(255),255));
   set(p, _MOVE | BRIGHT | TRANSLUCENT);
   p.alpha = 100;
   p._SIZE = 5;
   p._GRAVITY = 1;
   p._FADE = 5;  // fade factor
   p.event = p_fade_sprite;
}

action coin()
{  
	set(me, CAST);
	set(me, SHADOW);

  	/*pXent_settype(my, PH_STATIC, PH_BOX );
	pXent_settriggerflag(my, NX_TRIGGER_ON_ENTER, 1);
	my.event = coin_trigger;*/ 
	set(my,PASSABLE);
	var trigger_entered = false;
	var fade = 100;
	while(my)
	{
		/*my.pan += 2*time_step;
		pXent_rotate (my, my.pan, nullvector);*/
		c_scan(my.x, my.pan, vector(360, 360, 100), IGNORE_ME);
		my.pan += 2*time_step; //my.pan += 1*time_step;
		if(you == player && trigger_entered == false)
    	{
			trigger_entered = true;
    		collected_coins += 1;
    		snd_play(coin_wav,100,0);
			effect_sprite("star.bmp",p_fountain_sprite,maxv(1,20*time_step),vector(my.x, my.y, my.z),vector(0,0,5));
			my.alpha = 0; // hide it until it gets removed from level
			reset(me, CAST);
			reset(me, SHADOW);
			set(my, TRANSLUCENT);
			while (fade> 0)
       		{
        		fade -= 5 * time_step;
           		wait (1);
			}
			wait(1);		
			ent_remove(me);
    	}
		wait(1);
 	}
}

action rotate() // tilt
{
	set(me, CAST);
	set(me, SHADOW);
	
	pXent_settype(my, PH_STATIC , PH_POLY);
	while(my)
	{
		pXent_enable(my, 0 ); // disable physics entity so we can manually alter its position.
		ang_rotate(my.pan,vector(0, 2*time_step, 0)); // workaround to fix weird my.tilt glitch
		pXent_enable(my, 1 ); // re-enable entity, this will inform the physics system that position/orientation has changed.
		wait(1);
  	}
}

action rotate2() // pan
{
	set(me, CAST);
	set(me, SHADOW);
	
	pXent_settype(my, PH_STATIC , PH_POLY);
	while(my)
	{
		pXent_enable(my, 0 ); // disable physics entity so we can manually alter its position.
		my.pan += 2 * time_step;
		pXent_enable(my, 1 ); // re-enable entity, this will inform the physics system that position/orientation has changed.
		wait(1);
  	}
}

action crate()
{
	set(me, CAST);
	set(me, SHADOW);
	
	pXent_settype (my, PH_RIGID, PH_BOX); // set the physics entity type
	pXent_setmass (my, 1); // and its mass
	pXent_setfriction (my, 50); // set the friction
	pXent_setdamping (my, 10, 10); // set the damping
	pXent_setelasticity (my, 50); // set the elasticity
}

action finish()
{		
	pXent_settype(my, PH_STATIC, PH_BOX );
	pXent_settriggerflag(my, NX_TRIGGER_ON_ENTER, 1);
	my.event = finish_trigger;
	my.flags |= OVERLAY;
}

action dummy_ball()
{
	set(me, CAST);
	set(me, SHADOW);
	pXent_settype (my, PH_RIGID, PH_SPHERE); // set the physics entity type
	pXent_setmass (my, 15); // and its mass
	pXent_setfriction (my, 100); // set the friction
	pXent_setdamping (my, 40, 40); // set the damping
	pXent_setelasticity (my, 45); // set the elasticity
}

action ball()
{
	set(me, CAST);
	set(me, SHADOW);
	
	VECTOR ball_dir;
	var ball_speed = 100;
	var jumptimer = -1.5;
	
	var rot_angle = 0;
	var rot_height = 0;
	
	VECTOR spawn_pos;
	vec_set(spawn_pos, vector(my.x, my.y, my.z));
	
	player = my;
	
	//setup player ui
	set(time_txt, SHOW);
	set(level_txt, SHOW);
	set(time_display, SHOW);
	set(coin_pan, SHOW);
	set(coin_digits_pan, SHOW);
	
	timer_seconds = my.time_limit; // get time in seconds for the level to finish
	number_coins = my.coin_limit; //set required coin value to pass level
	set_level_text();
	
	pX_setgravity (vector(0, 0, -15)); // set the gravity - default: -9.81
	pXent_settype (my, PH_RIGID, PH_SPHERE); // set the physics entity type
	pXent_setmass (my, 15); // and its mass
	pXent_setfriction (my, 100); // set the friction
	pXent_setdamping (my, 10, 10); // set the damping
	pXent_setelasticity (my, 60); // set the elasticity
	
   while (my != NULL)
	{		
		ball_dir.x = ball_speed * (key_d - key_a); // move the ball using the cursor keys
		ball_dir.y = ball_speed * (key_w - key_s); // sets the x / y movement speeds
		ball_dir.z = 0; // no need to move on the vertical axis

		if (key_esc)
		{
			set(pause_pan, SHOW);
			freeze_cam = true;
			freeze_mode = 1;
		}

		if(key_space)
		{
			pXent_addvelcentral(my,vector(0,0,1000*time_step));
			snd_play(jump_wav,100,0);
			wait(jumptimer);
		}
		
		vec_rotate (ball_dir, camera.pan); // move player in camera direction
		pXent_addtorqueglobal (my, ball_dir); // add a torque (an angular force) to the ball

		if(my.z<-850)
		{
			//pXent_setvelocity(my, nullvector);
			//pXent_setposition(my, spawn_pos);
			gameover_fn();
		}
		
		wait (1);
    }
}
