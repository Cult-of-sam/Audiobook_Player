
#include"player.h"

/*Under the assumption that the user will be entering time in seconds, so convert to miliseconds before
 * using any of the functions which work with miliseconds (returns the miliseconds)
 * @seconds - the seconds to convert to miliseconds*/
int64_t convert_to_ms(int64_t seconds)
{
	int64_t result = seconds * 1000;
	return result;
}
/*setup the vlc backend (returns -1 on failure and 1 on success) 
 * @filename - the audiobook to open*/
int init_vlc(char *filename)
{
//	m = mmap(NULL,sizeof m,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1,0);
	mp = mmap(NULL,sizeof mp,PROT_READ | PROT_WRITE , MAP_SHARED | MAP_ANONYMOUS,-1,0);

	/*load the engine*/
	inst = libvlc_new(0,NULL);
	if(inst==NULL)
	{
		perror("libvlc_new() returns NULL:");
		return -1;
	}

	/*set error messages to log file*/
	FILE *fd = fopen("LOG","a+");
	libvlc_log_set_file(inst,fd);	


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
/*get the length of audiobook to determine how long the program needs to run for (returns the total_length)
 * */
int64_t get_total_audio_length()
{
	int64_t total_length = libvlc_media_player_get_length(mp);
	return total_length;
}

/*get the current time position of the playing audiobook (returns the current_time)*/
int64_t get_current_time()
{
	int64_t current_time = libvlc_media_player_get_time(mp);
	return current_time;
}

/*set the current time of audiobook (returns -1 on failure and 1 on success)
 * @m_seconds - the miliseconds to set the audio to*/
void set_current_time(int64_t m_seconds)
{
	libvlc_media_player_set_time(mp,m_seconds);	
}
/*play the audiobook (returns -1 on failure and 1 on success)
 * @position_in_seconds - either 0 or the bookmark if an record exists , thus meaning a bookmark already exists*/
int play(int64_t position_in_m_seconds)
{
	int error;
	/*run the media player*/
	if(libvlc_media_player_play(mp)==-1)
	{
	//	perror("libvlc_media_player_player(mp): failed:");
		return -1;
	}
	/*set the current time or it will be taken from get_bookmark()*/
//	libvlc_media_player_set_time(mp,position_in_m_seconds);
	set_current_time(position_in_m_seconds);

	return 1;

}

/*pause_resume the audiobook
 * @pause_status - used to determine whether to pause or resume the audio*/
void pause_resume(int *pause_status)
{ //under the assumption by default pause_status = 1
	if(*pause_status==1) //pause
	{
		libvlc_media_player_set_pause(mp,*pause_status);
		*pause_status = 0;
	}
	else if(*pause_status==0) //resume
	{
		libvlc_media_player_set_pause(mp,*pause_status);
		*pause_status = 1;
	}

}
/*safely shutdown the vlc backend and eject the media (returns -1 on failure and 1 on success)*/
void quit()
{
	/*stop playing*/
	libvlc_media_player_stop(mp);
	

	/*free memory*/
	libvlc_media_player_release(mp);
	libvlc_release(inst);

}

/*rewind the audio by seconds (returns -1 on failure and 1 on success)
 * @seconds - the seconds to rewind the audio by*/
void rewind_audio(int64_t m_seconds)
{
	int64_t audio_position = get_current_time();
	audio_position -= m_seconds;
	usleep(10);
	set_current_time(audio_position);

}
/*forward the audio by seconds (returns -1 on failure and 1 on success)
 * @seconds - the seconds to forward the audio by*/
void forward_audio(int64_t m_seconds)
{
	int64_t audio_position = get_current_time();
	audio_position += m_seconds;
	usleep(10);
	set_current_time(audio_position);
}


/*Functions for bookmarking...*/
void set_bookmark(int64_t m_seconds,char *name)
{

}

int64_t get_bookmark(char *name)
{

}

