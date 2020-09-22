#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <ncurses.h>
#include <pthread.h>

#include "../include/player.h"

#include <vlc/libvlc.h>
#include <vlc/libvlc_media.h>

libvlc_media_player_t* mp = NULL;
libvlc_instance_t* inst = NULL;
libvlc_media_t* m = NULL;

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
    mp = mmap(NULL, sizeof(mp), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    /*load the engine*/
    inst = libvlc_new(0, NULL);
    if(inst == NULL)
    {
        perror("libvlc_new() returns NULL:");
        return -1;
    }

    /*create the file to play*/
    m = libvlc_media_new_path(inst, filename);
    if(m == NULL)
    {
        perror("libvlc_media_new_path(): returns NULL:");
        return -1;
    }

    /*create a media play playing environment*/
    mp = libvlc_media_player_new_from_media(m);
    if(mp == NULL)
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
        perror("libvlc_media_player_player(mp): failed:");
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
    set_current_time(audio_position);

}

/*forward the audio by seconds (returns -1 on failure and 1 on success)
 * @seconds - the seconds to forward the audio by*/
void forward_audio(int64_t m_seconds)
{
    int64_t audio_position = get_current_time();
    audio_position += m_seconds;
    set_current_time(audio_position);
}

void set_bookmark(int64_t m_seconds,char *name)
{
    char buf[100]="\0";
//	sprintf(buf,"/home/sam/Programs/My_programs/vlc/bin/bookmark_management/book_management c %s %d",name,m_seconds);
//	system(buf);
//	system("/home/sam/Programs/My_programs/vlc/bin/bookmark_management/book_management c book4 100");
}

void play_until_then_bookmark(){}

int64_t get_bookmark(char *name)
{
    char buf[100]="\0";
//	sprintf(buf,"/home/sam/Programs/My_programs/vlc/bin/bookmark_management/book_management g %s",name);
//	system(buf);

//	FILE *fd = fopen("book","r");
//	fgets(buf,sizeof(buf),fd);
//	fclose(fd);

//	if(strcmp(buf,"null")==0)
//			return -1;

//	return atoi(buf);
}

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
    while (1)
    {
        usleep(5);
        //	sleep(1);
        if (time(&time_struct) >= time_limit)
        {
            int pause = 1;
            /*pause the audio so we can get an accurate time*/
            pause_resume(&pause);
            set_bookmark(get_current_time(), g_filename);
            system("/home/sam/Programs/My_programs/vlc/pkill.sh program1");
        }
    }
}

void *show_audio_status()
{
    /*Format the time*/
    int64_t miliseconds, seconds, minutes, rseconds, hours, rminutes;
    miliseconds = seconds = minutes = rseconds = hours = rminutes = 0;

    char buf1[100] = "\0";
    char buf2[100] = "\0";


    //set cancellation setting
//	sleep(1);
    while(1)
    {
        sleep(1);
        if(status_pause != 1)
        {
            //	sleep(2);
            int64_t total_length = get_total_audio_length();

            miliseconds = get_total_audio_length();
            seconds = miliseconds / 1000;
            minutes = seconds / 60;
            rseconds = seconds % 60;
            hours = minutes / 60;
            rminutes = minutes % 60;

            sprintf(buf2, "%d:%d:%d", hours, rminutes, rseconds);
            miliseconds = seconds = minutes = rseconds = hours = rminutes = 0;

            miliseconds = get_current_time();
            seconds = miliseconds / 1000;
            minutes = seconds / 60;
            rseconds = seconds % 60;
            hours = minutes / 60;
            rminutes = minutes % 60;

            sprintf(buf1, "%d:%d:%d", hours, rminutes, rseconds);

            mvprintw(LINES - 1, 40, "%s/%s", buf1, buf2);
            refresh();
        }
    }
}

void print_options()
{
    char msg[] = "(1) - Start book \t(2) - continue book\t(3) - bookmark\t(4) - set_bookmark_time";
    char msg1[] = "<spacebar> - pause \t<left> - rewind \t<right> - forward \t q - exit";
    int row, col;

    /*obtain screen dimensions*/
    getmaxyx(stdscr, row, col);

    /*print to the screen*/
    mvprintw(row / 15, (col - strlen(msg)) / 6, "%s", msg);
    mvprintw(row / 10, (col - strlen(msg1)) / 6, "%s", msg1);
}

void get_user_string(char *mesg_prompt, char *input)
{
    /*turn echo on so user can see what they're typing*/
    echo();

    /*get screen dimensions*/
    int row, col;
    getmaxyx(stdscr, row, col);

    mvprintw(row / 2, (col - strlen(mesg_prompt)) / 2, "%s", mesg_prompt);
    getstr(input);
    mvprintw(LINES - 2, 0, "You entered:%s", input);

    /*turn echo off*/
    noecho();

}

void process_user_input(char *filename)
{
    int quit_loop = 0; int pause = 1;
    char buf[100] = "\0";
    int64_t miliseconds;
    pthread_t thread_id2;
    pthread_create(&thread_id2, NULL, show_audio_status, NULL);
    pthread_detach(thread_id2);
    nodelay(stdscr, FALSE);

    int key;

    do
    {
        //	pthread_create(&thread_id,NULL,show_audio_status,NULL);
        //	pthread_join(thread_id,NULL);
        //	pthread_detach(thread_id);
        key = getch();

        /*spacebar*/
        if (key == ' ')
        {
            /*pause or resume audio*/
            pause_resume(&pause);
        }/*right key*/
        else if (key == KEY_RIGHT)
        {
            /*foward the audio*/
            forward_audio(convert_to_ms(10));
            usleep(2);
        }/*left key*/
        else if (key == KEY_LEFT)
        {
            /*rewind the audio*/
            rewind_audio(convert_to_ms(10));
            usleep(2);
        }
        else if (key == '1')
        {
            /*start the audiobook from the beginning*/
            if (play(convert_to_ms(0)) == -1)
            {
                perror("play(): failed:");
                exit(EXIT_FAILURE);
            }
            mvprintw(LINES - 2, 0, "Playing!");
        }
        else if (key == '2')
        {
            int64_t value;
            value = get_bookmark(filename);
            if(value != -1)
            {
                set_current_time(value);
                mvprintw(LINES-2,0,"Resuming");
            }
        }
        else if (key == '3')
        {
            set_bookmark(get_current_time(), filename);
            mvprintw(LINES - 2, 0, "bookmarked!");
        }
        else if (key == '4')
        {
            status_pause = 0;
            //kill the time status thread as that specific thread is causing seg fault when we try  to set bookmark
            pthread_join(thread_id2, NULL);
            pthread_cancel(thread_id2);
            //pthread_create(&thread_id2,NULL,show_audio_status,NULL);
            //pthread_detach(thread_id2);
            nodelay(stdscr, FALSE);
            //get_user_string("Enter time to bookmark:",buf);
            echo();
            char mesg[] = "Enter play duration(minutes):\0";
            char str[80];
            int row, col;
            getmaxyx(stdscr, row, col);
            mvprintw(row / 2, (col - strlen(mesg)) / 2, "%s", mesg);

            getstr(str);
            mvprintw(LINES - 2, 0, "You entered:%s", str);
            sleep(2);
            erase();
            print_options();
            //nodelay(stdscr,TRUE);
            status_pause = 0;

            int minutes = atoi(str);
            int seconds = minutes * 60;

            g_filename = filename;
            g_minutes = minutes;

            //SM - I understand the need to put preliminary comments, but 'thread' isn't a useful comment
            /*thread*/
            pthread_t thread_id;
            //pthread_cancel(thread_id);
            pthread_create(&thread_id, NULL, check_duration, NULL);
            //pthread_join(thread_id,NULL);
            pthread_detach(thread_id);

            g_current_time = get_current_time();
            pthread_create(&thread_id2, NULL, show_audio_status, NULL);
            pthread_detach(thread_id2);
        }
        else if (key == 'q')
        {
            quit_loop = 1;
        }

        refresh();

    }
    while (quit_loop != 1);
}