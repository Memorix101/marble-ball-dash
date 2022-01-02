
/*function coin_trigger()
{
	pXent_settype(my, NULL, NULL);
	wait(1);
	collected_coins += 1;
	snd_play(coin_wav,100,0);
	ent_remove(my);
}*/

function finish_trigger()
{
	if(collected_coins >= number_coins && you == player){
		level_passed = true;
		freeze_cam = true;
		set(finish_pan, SHOW);
  		freeze_mode=1;
  		snd_play(clapping_wav,100,0);
	}
}