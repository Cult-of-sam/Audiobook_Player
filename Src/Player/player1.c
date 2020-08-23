#include<stdio.h>
#include<stdlib.h>
#include "player.h"
#include<unistd.h>
#include<ncurses.h>
#include<string.h>
#include<pthread.h>

/*very rough ncurses interface, will be improved upon and changed in later versions, however the 
 * 1.continue 2.bookmark 3.set_play_time rely upon ./manage_books to set and obtain bookmarks for a book
 *
 *Also not the final layout in terms of code logic positioning etc
 *
 * */

char *g_filename;
int64_t g_minutes;
int64_t g_current_time;

int status_pause = 0;

void *check_duration()
{
	time_t time_struct;
	int64_t seconds = g_minutes * 60;
	int64_t time_limit = time(&time_struct) + seconds;


	/*loop and print the status to help show if it's paused or not, or being forwarded etc.*/
	while(1)
	{
		usleep(5);
	//	sleep(1);
		if(time(&time_struct) >= time_limit)
		{
			int pause=1;
			/*pause the audio so we can get an accurate time*/
			pause_resume(&pause);
			set_bookmark(get_current_time(),g_filename);
			system("/home/sam/Programs/My_programs/vlc/pkill.sh program1");

		}
	
	}	
}
void *show_audio_status()
{
	/*Format the time*/
	int64_t miliseconds , seconds , minutes , rseconds , hours , rminutes;
	miliseconds = seconds = minutes = rseconds = hours = rminutes = 0;
	
	char buf1[100]="\0";
	char buf2[100]="\0";

	
	//set cancellation setting
//	sleep(1);
	while(1){
		sleep(1);
		if(status_pause!=1){
	//	sleep(2);
		int64_t total_length = get_total_audio_length();

		miliseconds = get_total_audio_length();
		seconds = miliseconds / 1000;
		minutes = seconds / 60;
		rseconds = seconds % 60;
		hours = minutes / 60;
		rminutes = minutes % 60;


		sprintf(buf2,"%d:%d:%d",hours,rminutes,rseconds);
		miliseconds = seconds = minutes = rseconds = hours = rminutes = 0;
	
		
		miliseconds = get_current_time();
		seconds = miliseconds / 1000;
		minutes = seconds / 60;
		rseconds = seconds % 60;
		hours = minutes / 60;
		rminutes = minutes % 60;
		
		sprintf(buf1,"%d:%d:%d",hours,rminutes,rseconds);

		mvprintw(LINES-1,40,"%s/%s",buf1,buf2);
		refresh();
		}
	}  
}

void print_options()
{
	char msg[]="(1) - Start book \t(2) - continue book\t(3) - bookmark\t(4) - set_bookmark_time";
	char msg1[]="<spacebar> - pause \t<left> - rewind \t<right> - forward";
	int row,col;
	
	/*obtain screen dimensions*/
	getmaxyx(stdscr,row,col);

	/*print to the screen*/
	mvprintw(row/15,(col-strlen(msg))/6,"%s",msg);
	mvprintw(row/10,(col-strlen(msg1))/6,"%s",msg1);

}

void get_user_string(char *mesg_prompt,char *input)
{
	/*turn echo on so user can see what they're typing*/
	echo();

	/*get screen dimensions*/
	int row,col;
	getmaxyx(stdscr,row,col);
	
	mvprintw(row/2,(col-strlen(mesg_prompt))/2,"%s",mesg_prompt);
	getstr(input);
	mvprintw(LINES-2,0,"You entered:%s",input);

	/*turn echo off*/
	noecho();

}

void process_user_input(char *filename)
{
	int quit_loop = 0; int pause = 1;
	char buf[100] = "\0";
	int64_t miliseconds;
	pthread_t thread_id2;
	pthread_create(&thread_id2,NULL,show_audio_status,NULL);
	pthread_detach(thread_id2);
	nodelay(stdscr,FALSE);

	do
	{
	//	pthread_create(&thread_id,NULL,show_audio_status,NULL);
	//	pthread_join(thread_id,NULL);
	//	pthread_detach(thread_id);

		/*spacebar*/
		if(getch()==' ')
		{
			/*pause or resume audio*/
			pause_resume(&pause);
			
		}/*right key*/
		else if(getch()==KEY_RIGHT)
		{
			/*foward the audio*/
			forward_audio(convert_to_ms(10));
			usleep(2);
		}/*left key*/
		else if(getch()==KEY_LEFT)
		{
			/*rewind the audio*/
			rewind_audio(convert_to_ms(10));
			usleep(2);
		}
		else if(getch()=='1')
		{
			/*start the audiobook from the beginning*/
			if(play(convert_to_ms(0))==-1)
			{
				perror("play(): failed:");
				exit(EXIT_FAILURE);
			}
			mvprintw(LINES-2,0,"Playing!");
		}
		else if(getch()=='2')
		{
			int64_t value;
			value = get_bookmark(filename);
			if(value!=-1)
			{
				set_current_time(value);
				mvprintw(LINES-2,0,"Resuming");
			}
		}
		else if(getch()=='3')
		{
			set_bookmark(get_current_time(),filename);
			mvprintw(LINES-2,0,"bookmarked!");
		}
		else if(getch()=='4')
		{	status_pause=0;	
			//kill the time status thread as that specific thread is causing seg fault when we try  to set bookmark
			pthread_join(thread_id2,NULL);
			pthread_cancel(thread_id2);
		//	pthread_create(&thread_id2,NULL,show_audio_status,NULL);
		//	pthread_detach(thread_id2);
			nodelay(stdscr,FALSE);
			//get_user_string("Enter time to bookmark:",buf);
			echo();
			char mesg[]="ENter play duration(minutes):\0";
			char str[80];
			int row,col;
			getmaxyx(stdscr,row,col);
			mvprintw(row/2,(col-strlen(mesg))/2,"%s",mesg);

			getstr(str);
			mvprintw(LINES-2,0,"You entered:%s",str);
			sleep(2);
			erase();
			print_options();	
	//		nodelay(stdscr,TRUE);
			status_pause = 0;

			int minutes = atoi(str);
			int seconds = minutes * 60;
	
			g_filename = filename;
			g_minutes = minutes;

			/*thread*/
			pthread_t thread_id;
		//	pthread_cancel(thread_id);
			pthread_create(&thread_id,NULL,check_duration,NULL);
//			pthread_join(thread_id,NULL);
			pthread_detach(thread_id);
//			
			g_current_time = get_current_time();	
			pthread_create(&thread_id2,NULL,show_audio_status,NULL);
			pthread_detach(thread_id2);
				
		}
		else if(getch()=='5')
		{
			quit_loop=1;
		}


		refresh();
		

	}while(quit_loop!=1);

	
	
}


int main(int argc, char **argv)
{



	/*get the soundfile to play from a command line argument*/
	if(argc!=2)
	{
		exit(EXIT_FAILURE);
	}

	//init curses screen
	initscr();
	cbreak();
	noecho();
	keypad(stdscr,TRUE);

	print_options();

	if(init_vlc(argv[1])==-1)
	{
		perror("init_vlc() failed:");
		exit(EXIT_FAILURE);
	}
	
	process_user_input(argv[1]);


	//quit curses
	endwin();

	/*stop vlc*/
	quit();


	return 0;
}
