#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include <unistd.h>

int main()
{
	if(init_vlc("test.opus") == -1)
	{
		perror("init_vlc() failed:");
		exit(EXIT_FAILURE);
	}

	if(play(00000) == -1)
	{
		perror("play(): failed:");
		exit(EXIT_FAILURE);
	}	

	/*play for 60seconds*/
	sleep(60);
	return 0;
}
