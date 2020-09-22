#include<vlc/vlc.h>
#include<stdlib.h>
#include<string.h>
#include<sys/mman.h>
#include<unistd.h>
/*data structures for the following functions*/
libvlc_instance_t *inst;
libvlc_media_player_t *mp;
libvlc_media_t *m;

//mp = mmap(NULL,sizeof mp,PROT_READ | PROT_WRITE , MAP_SHARED | MAP_ANONYMOUS,-1,0);

/*Functions*/
int64_t convert_to_ms(int64_t seconds);
int init_vlc(char *filename);
int64_t get_total_audio_length();
int64_t get_current_time();
void set_current_time(int64_t m_seconds);
int play(int64_t position_in_m_seconds);
void pause_resume(int *pause_status);
void quit();
void rewind_audio(int64_t m_seconds);
void forward_audio(int64_t m_seconds);

void set_bookmark(int64_t m_seconds,char *name);
void play_until_then_bookmark();
int64_t get_bookmark(char *name);



