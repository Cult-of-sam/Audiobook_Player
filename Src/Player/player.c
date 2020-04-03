
#include"player.h"


int64_t convert_to_ms(int64_t seconds)
{
	int64_t result = seconds * 10000;
	return result;
}

int init_vlc(char *filename)
{
	/*load the engine*/
	inst = libvlc_new(0,NULL);
	if(inst==NULL)
	{
		perror("libvlc_new() returns NULL:");
		return -1;
	}
	/*create the file to play*/
	m = libvlc_media_new_path(inst,filename);
	if(m==NULL)
	{
		perror("libvlc_media_new_path(): returns NULL:");
		return -1;
	}

	/*create a media play playing environment*/
	mp = libvlc_media_player_new_from_media(m);
	if(mp==NULL)
	{
		perror("libvlc_media_player_new_from_media() returns NULL:");
		return -1;
	}

	/*release_media*/
	libvlc_media_release(m);

	return 1;
}

int play(int64_t position_in_seconds)
{
	int error;
	/*run the media player*/
	if(libvlc_media_player_play(mp)==-1)
	{
		perror("libvlc_media_player_player(mp): failed:");
		return -1;
	}
	/*set the current time or it will be taken from get_bookmark()*/
	libvlc_media_player_set_time(mp,convert_to_ms(position_in_seconds));

	return 1;
}
void pause_resume(){}
int quit()
{
	/*stop playing*/
	if(libvlc_media_player_stop(mp)==-1)
	{
		perror("libvlc_media_player_stop():failed:");
		return -1;
	}

	/*free memory*/
	libvlc_media_player_release(mp);
	libvlc_release(inst);

	return 1;
}

int64_t get_total_audio_length()
{
	int64_t total_length = libvlc_media_player_get_length(mp);
	return total_length;
}

int64_t get_current_time()
{
	int64_t current_time = libvlc_media_player_get_time(mp);
	return current_time;
}
void rewind_audio(){}
void forward_audio(){}
void set_bookmark(){}
void play_until_then_bookmark(){}
void get_bookmark(){}
