#include <am.h>
#include <nemu.h>
#include <stdio.h>

#define AUDIO_FREQ_ADDR      (AUDIO_ADDR + 0x00)
#define AUDIO_CHANNELS_ADDR  (AUDIO_ADDR + 0x04)
#define AUDIO_SAMPLES_ADDR   (AUDIO_ADDR + 0x08)
#define AUDIO_SBUF_SIZE_ADDR (AUDIO_ADDR + 0x0c)
#define AUDIO_INIT_ADDR      (AUDIO_ADDR + 0x10)
#define AUDIO_COUNT_ADDR     (AUDIO_ADDR + 0x14)

typedef struct _AudioData {
  int freq, channels, samples, sbuf_size;
}_AudioData;

static _AudioData audio; 

void __am_audio_init() {
  audio.freq = inl(AUDIO_FREQ_ADDR);
  audio.channels = inl(AUDIO_CHANNELS_ADDR);
  audio.samples = inl(AUDIO_SAMPLES_ADDR);
  audio.sbuf_size = inl(AUDIO_SBUF_SIZE_ADDR);
}

void __am_audio_config(AM_AUDIO_CONFIG_T *cfg) {
  cfg->present = true;
  /* Buf_size will not change. */
  cfg->bufsize = audio.sbuf_size;
}

void __am_audio_ctrl(AM_AUDIO_CTRL_T *ctrl) {
  ctrl->freq = audio.freq;
  ctrl->channels = audio.channels;
  ctrl->samples = audio.samples;
}

void __am_audio_status(AM_AUDIO_STATUS_T *stat) {
  stat->count = inl(AUDIO_COUNT_ADDR);
}

void __am_audio_play(AM_AUDIO_PLAY_T *ctl) {
 //add_mmio_map("audio-sbuf", CONFIG_SB_ADDR, sbuf, CONFIG_SB_SIZE, NULL);
  printf("Well, trying to init\n");
  uint32_t *start = ctl->buf.start;
  uint32_t *end = ctl->buf.end;
  uint32_t size = end - start;
  int count = inl(AUDIO_COUNT_ADDR);
  while(size > count) ;
  uint32_t offset = 0;
  printf("Well, trying to copy\n");
  while(size != 0) {
    if(size / sizeof(uint32_t)) {
      uint32_t data = *(start + offset);
      outl(AUDIO_SBUF_ADDR + offset, data);
      offset += 4;
      size -= 4;
    }
    else {
      uint8_t data = (uint8_t)*(start + offset);
      outb(AUDIO_SBUF_ADDR + offset, data);
      offset += 1;
      size -= 1;
    }
  }
  outl(AUDIO_COUNT_ADDR, count - size);
}
