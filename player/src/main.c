#include <stdio.h>
#include <stdlib.h>

#include <ncurses.h>

#include <logging.h>
#include "../include/player.h"

void shutdown()
{
    log_info("Starting cleanup");

    // Quit curses
    endwin();

    // Stop vlc
    quit();

    log_info("All cleanup finished, exiting");
}


int main(int argc, char **argv)
{
    log_info("Started");

    // Get the soundfile to play from a command line argument
    if (argc != 2)
    {
        fprintf(stderr, "No sound file provided!\n");
        exit(EXIT_FAILURE);
    }

    log_info("Initialising cleanup and curses");
    atexit(shutdown);

    //init curses screen
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    print_options();

    if (init_vlc(argv[1]) == -1)
    {
        perror("init_vlc() failed:");
        exit(EXIT_FAILURE);
    }

    process_user_input(argv[1]);


    return 0;
}