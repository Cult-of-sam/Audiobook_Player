#include<stdio.h>
#include<stdlib.h>
#include "player.h"
#include<ncurses.h>
#include "ncursesw/ncurses.h"
#include<locale.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

//WINDOW *menu1;


WINDOW *create_window(int height,int width,int starty,int startx)
{
	WINDOW *local_win;
	local_win = newwin(height,width,starty,startx);
	box(local_win,0,0);
	wrefresh(local_win);

	return local_win;
}

void *audio_status(void *args)
{
	int status_pause = 0;
	WINDOW *win = (WINDOW *)args;

	int64_t total_audio,t_total;
	int64_t f_hour,f_minute,f_second,whole;
	double remainder,number,total_seconds,total_minutes,total_hours;


	char buf1[100]="\0";
	char buf2[100]="\0";
	int64_t c,t; c=t=0;

	sleep(1);
	/*workout total audio*/
	t_total = total_audio = get_total_audio_length();

	//convert to seconds
	total_seconds = total_audio / 1000;

	total_minutes = total_seconds / 60; //minutes
	whole = (int) total_minutes;
	remainder = total_minutes - whole;
	number = remainder * 60;

	f_second = (int) number;

	//convert hour and minutes
	total_hours = total_minutes / 60; //hours & minutes
	whole = (int) total_hours;
	remainder = total_hours - whole;
	number = remainder * 60;
	
	f_minute = (int)number;
	f_hour = whole;

	sprintf(buf2,"%d:%d:%d",f_hour,f_minute,f_second);

	int percentage;
	t_total = total_seconds;

	//fill progress bar map
/*	fill_map();
	char progress_bar[200]="\0"; */

	while(1)
	{
		usleep(1500000);
		
			if(status_pause!=1)
			{
				//get current timestamp
				total_audio = get_current_time();
			
				//convert to seconds
				total_seconds = total_audio / 1000;

				percentage = total_seconds / t_total * 100;

				total_minutes = total_seconds / 60;
				whole = (int) total_minutes;
				remainder = total_minutes - whole;
				number = remainder * 60;

				f_second = (int) number;

				//convert hours and minutes
				total_hours = total_minutes / 60;
				whole = (int) total_hours;
				remainder = total_hours - whole;
				number = remainder * 60;

				f_minute = (int) number;
				f_hour = whole;
	
				sprintf(buf1,"%d:%d:%d",f_hour,f_minute,f_second);


				/*progress bar*/
			/*	for(int i=0;i<=200;i++)
					progress_bar[i]='\0';
				get_image(&percentage,progress_bar,sizeof(progress_bar));		*/ 

				mvwprintw(win,13,30,"%s/%s (%d%)",buf1,buf2,percentage);
			//	mvwprintw(win,15,30,"%s",progress_bar);
				wrefresh(win);

			
		}
	}
}



void Draw_Menu1(WINDOW **win)
{
	int height = 18; int width = 70;
	int starty = (LINES - height) / 12;
	int startx = (COLS - width) / 2;

	*win = create_window(height,width,starty,startx);

	/*map ascii art*/
	FILE *fd = NULL;
	char buf[120]="\0"; //120 bytes
	int i=1;

	if((fd=fopen("title","r"))==NULL)
	{
		fprintf(stderr,"error opening image.txt\n");
		/*print all error messages to menu 2*/
	}
	while(fgets(buf,sizeof(buf),fd)!=NULL)
	{
		mvwprintw(*win,i,10,buf);
		mvwprintw(*win,i,69,"|");
		i++;
	}
	fclose(fd);

	if((fd=fopen("book_image","r"))==NULL)
	{
		fprintf(stderr,"error opening book\n");
		
	} i=9;
	while(fgets(buf,sizeof(buf),fd)!=NULL) 
	{
		mvwprintw(*win,i,8,buf);
		mvwprintw(*win,i,69,"|");i++;
		wrefresh(*win);
	}

	fclose(fd);

	/*these will be used in the show timer function and print
	 * within this window from the thread, as well as the progress bar*/
//	mvprintw(*win,10,30,"playing X book");
//	mvprintw(*win,13,30,"2:00/4.00(50%");
}

void Draw_Menu2(WINDOW **win)
{
	int height = 3; int width = 70;
	int starty = (LINES-25)/1;
	int startx = (COLS - width)/2;
	*win = create_window(height,width,starty,startx);

//	mvwprintw(*win,1,6,"Input/output information prompt didsplay");
	wrefresh(*win);
}

void Draw_Menu3(WINDOW **win)
{
	int height = 10; int width = 70;
	int starty = (LINES-22)/1;
	int startx = (COLS-width)/2;

	*win = create_window(height,width,starty,startx);

	mvwprintw(*win,1,6,"(1) - Start Book");
	mvwprintw(*win,3,6,"(2) - Continue");
	mvwprintw(*win,5,6,"(3) - Bookmark Now");
	mvwprintw(*win,7,6,"(4) - Bookmark Later");

	mvwprintw(*win,1,40,"(Left Key) - Rewind");
	mvwprintw(*win,3,40,"(Right Key) - Forward");
	mvwprintw(*win,5,40,"(SPACEBAR) - Pause/Resume");
	mvwprintw(*win,7,40,"(F1) - Exit");

	wrefresh(*win);
}



void Main_Loop(char *filename,WINDOW **win1,WINDOW **win2, WINDOW **win3)
{



	int ch; int pause = 1; int64_t value; int quit_loop = 0;
//	nodelay(stdscr,FALSE);
	char message[100]="\0"; //100 bytes
	int play_sig = 0; int pause_sig = 0; int continue_sig = 0;
	int forward_sig=0; int rewind_sig = 0;
	int quit_sig = 0; int bookmark_sig=0;
	char title[100]="\0";
	strcpy(title,filename);

	int status_pause=0;

	/*create thread for showing audiostatus*/
	pthread_t thread_id2;
//	pthread_create(&thread_id2,NULL,audio_status,*win1);
//	pthread_detach(thread_id2);

	//blocking input on to save on cpu usage *****
	do
	{
			erase();
			refresh();
			Draw_Menu1(&*win1);
			Draw_Menu2(&*win2);
			Draw_Menu3(&*win3);
			/*printing what is being played*/
			mvwprintw(*win1,10,30,title);
			wrefresh(*win1);
			wrefresh(*win2);
			wrefresh(*win3);
			if(play_sig == 1)
			{
				mvwprintw(*win2,1,30,"Playing!");
				wrefresh(*win2);
				play_sig=0;

				pthread_create(&thread_id2,NULL,audio_status,*win1);
				pthread_detach(thread_id2);

			}
			if(pause_sig==1)
			{
				mvwprintw(*win2,1,30,"Paused");
				wrefresh(*win2);
				pause_sig=0;
			}
			if(continue_sig==1)
			{
				mvwprintw(*win2,1,20,"continuing from last bookmark");
				wrefresh(*win2);
				sleep(2);
				werase(*win2); Draw_Menu2(&*win2);
				mvwprintw(*win2,1,30,"Playing!");
				wrefresh(*win2);
				continue_sig=0;
			}
			if(quit_sig==1)
			{
				mvwprintw(*win2,1,30,"Exiting");
				wrefresh(*win2);
				sleep(1);
				quit_loop = 1;
			}
			if(bookmark_sig==1)
			{
				mvwprintw(*win2,1,30,"Bookmarked!");
				wrefresh(*win2);
				sleep(1);
				werase(*win2); Draw_Menu2(&*win2);
				mvwprintw(*win2,1,30,"Playing");
				wrefresh(*win2);
				bookmark_sig=0;
			}


			if(getch()==' ')
			{
				pause_resume(&pause);
				werase(*win2);
				if(pause==0)
					pause_sig = 1;
				else if(pause==1)
					play_sig=1;
	
			}
			else if(getch()==KEY_F(1))
			{
				quit_sig = 1;
			}
			else if(getch()==KEY_RIGHT)
			{

				forward_audio(convert_to_ms(5));
				play_sig = 1;
			}
			else if(getch()==KEY_LEFT)
			{
				rewind_audio(convert_to_ms(5));
				play_sig=1;
			}
			else if(getch()=='1')
			{
				play(convert_to_ms(0));
				werase(*win2);
			//	mvwprintw(*win2,1,6,"Playing!");
				play_sig=1;
			}
			else if(getch()=='2')
			{
				int64_t value;
				value = get_bookmark(filename);
				if(value!=-1)
				{
					set_current_time(value);
					continue_sig = 1;
				}
			}
			else if(getch()=='3')
			{
				set_bookmark(get_current_time(),filename);
				bookmark_sig = 1;

			}
			else if(getch()=='4')
			{
				status_pause =0;
				pthread_join(thread_id2,NULL);
				pthread_cancel(thread_id2);
			}


	
	/*	erase();
		refresh();
		Draw_Menu1(&*win1);
		Draw_Menu2(&*win2);
		Draw_Menu3(&*win3);
		
		if(display_sig==1)
		{
			mvwprintw(*win2,1,6,message);
			display_sig = 0;
		} */


	}while(quit_loop!=1);
}


int main(int argc,char **argv)
{
	if(argc!=2)
	{
		exit(EXIT_FAILURE);
	}

	setlocale(LC_ALL,"");
	WINDOW *menu1; WINDOW *menu2; WINDOW *menu3;
	int ch;

	initscr();
	cbreak;
	noecho();
	keypad(stdscr,TRUE);

	refresh();
	Draw_Menu1(&menu1);
	Draw_Menu2(&menu2);
	Draw_Menu3(&menu3);

	/*init vlc*/
	if(init_vlc(argv[1])==-1)
	{
		exit(EXIT_FAILURE);
	}

	Main_Loop(argv[1],&menu1,&menu2,&menu3);

	quit();
	endwin();
}
