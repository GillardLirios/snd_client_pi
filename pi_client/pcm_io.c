#include "pcm_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <tinyalsa/pcm.h>
#include "log4z.h"
static struct pcm *pcm_cap;
static struct pcm *pcm_out;
static unsigned int cap_frames_per_interval; 
static unsigned int cap_interval = 20;	//ms
static unsigned int cap_sampes = 320;	//ms
int pcm_cap_init(void)
{
   unsigned int card = 1;
    unsigned int device = 0;
    int flags = PCM_IN;

    const struct pcm_config config = {
        .channels = 1,
        .rate = 48000,
        .period_size = 1024,
        .period_count = 2,
        .format = PCM_FORMAT_S16_LE,
        .start_threshold = 1024,
        .stop_threshold = 1024 * 2,
        .silence_threshold = 1024 * 2,
    };

    pcm_cap = pcm_open(card, device, flags, &config);
    if (pcm_cap == NULL) {
        LOGFMTE( "failed to allocate memory for PCM\n");
        return EXIT_FAILURE;
    } else if (!pcm_is_ready(pcm_cap)){
        pcm_close(pcm_cap);
        LOGFMTE( "failed to open PCM\n");
        return EXIT_FAILURE;
    }

    unsigned int frame_size = pcm_frames_to_bytes(pcm_cap, 1);
	cap_frames_per_interval  = pcm_get_rate(pcm_cap)/(1000/cap_interval) ;
	return frame_size * cap_sampes;

}

int pcm_out_init(void)
{
	LOGFMTT("%s\n", __FUNCTION__);

    unsigned int card = 1;
    unsigned int device = 0;
    int flags = PCM_OUT;

    const struct pcm_config config = {
        .channels = 2,
        .rate = 48000,
        .period_size = 1024,
        .period_count = 2,
        .format = PCM_FORMAT_S16_LE,
        .start_threshold = 1024,
        .stop_threshold = 1024 * 2,
        .silence_threshold = 1024 * 2,
    };

	pcm_out = pcm_open(card, device, flags, &config);
    if (pcm_out == NULL) {
        LOGFMTE( "failed to allocate memory for PCM\n");
        return -1;
    } else if (!pcm_is_ready(pcm_out)){
        pcm_close(pcm_out);
        LOGFMTE( "failed to open PCM\n");
        return -1;
    }


	return 0;
}

size_t read_frames(void** frames)
{
   int read_count = pcm_readi(pcm_cap, *frames, cap_sampes );
   size_t byte_count = pcm_frames_to_bytes(pcm_cap, read_count);
    return byte_count;
}

int write_frames(const void * frames, size_t byte_count)
{
	//LOGFMTT("%s\n %x, %d", __FUNCTION__,(unsigned int)frames,byte_count);
    unsigned int frame_count = pcm_bytes_to_frames(pcm_out, byte_count);
    pcm_writei(pcm_out, frames, frame_count);
	return 0;
}

int pcm_cap_close(void)
{
	LOGFMTT("%s", __FUNCTION__);
	 if (pcm_cap) {
		pcm_close(pcm_cap);
	}
	return 0;
}

int pcm_out_close(void)
{
	LOGFMTT("%s\n", __FUNCTION__);
	 if (pcm_out) {
		pcm_close(pcm_out);
	}
	return 0;
}
