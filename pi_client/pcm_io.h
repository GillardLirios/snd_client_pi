#ifndef __PCM_IO_H__
#define __PCM_IO_H__

#include <stdio.h>
#include <stdlib.h>

#include <tinyalsa/pcm.h>
int pcm_cap_init(void);
int pcm_out_init(void);
int pcm_out_init2(void);
size_t read_frames(void **frames);
int write_frames(const void * frames, size_t byte_count);
int write_frames2(const void * frames, size_t byte_count);
int pcm_cap_close(void);
int pcm_out_close(void);
int pcm_out_close2(void);
#endif

