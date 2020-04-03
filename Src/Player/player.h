#include<vlc/vlc.h>

/*data structures for the following functions*/
libvlc_instance_t *inst;
libvlc_media_player_t *mp;
libvlc_media_t *m;


/*Functions*/
int init_vlc(char *filename);
int play(int64_t position);
void pause_resume();
void quit();
void rewind_audio();
void forward_audio();
void set_bookmark();
void play_until_then_bookmark();


