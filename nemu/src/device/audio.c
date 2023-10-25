/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <common.h>
#include <device/map.h>
#include <SDL2/SDL.h>

enum {
  reg_freq,
  reg_channels,
  reg_samples,
  reg_sbuf_size,
  reg_init,
  reg_count,
  nr_reg
};

typedef struct AudioData {
  uint8_t *idx;
  uint32_t len;
}AudioData;

static AudioData audio;
static uint8_t *sbuf = NULL;
static uint32_t *audio_base = NULL;

/*data could be an application-specific parameter saved in the SDL_AudioSpec structure's userdata field
* buffer could be a pointer to the audio data buffer filled in by SDL_AudioCallback()
*/
void callback_func(void *data, Uint8 *buffer, int buffer_len)
{
    /* Clear buffer */
    SDL_memset(buffer, 0, buffer_len);
    AudioData *audio = (AudioData*)data;
    if(audio->len <= 0)  return;
   
    uint32_t length = (uint32_t)buffer_len;
    length = (length > audio->len ? audio->len : length);

    SDL_memcpy(buffer, audio->idx, length);

    audio->idx += length;
    audio->len -= length;

}

static void audio_io_handler(uint32_t offset, int len, bool is_write) {

}

static void audio_sbuf_handler(uint32_t offset, int len, bool is_write) {

}


void init_audio() {
  uint32_t space_size = sizeof(uint32_t) * nr_reg;
  audio_base = (uint32_t *)new_space(space_size);
#ifdef CONFIG_HAS_PORT_IO
  add_pio_map ("audio", CONFIG_AUDIO_CTL_PORT, audio_base, space_size, audio_io_handler);
#else
  add_mmio_map("audio", CONFIG_AUDIO_CTL_MMIO, audio_base, space_size, audio_io_handler);
#endif

  sbuf = (uint8_t *)new_space(CONFIG_SB_SIZE);
  add_mmio_map("audio-sbuf", CONFIG_SB_ADDR, sbuf, CONFIG_SB_SIZE, audio_sbuf_handler);



  audio.idx = (uint8_t *)new_space(CONFIG_SB_SIZE);
  audio.len = CONFIG_SB_SIZE;
  SDL_AudioSpec s = {};
  s.format = AUDIO_S16SYS;  
  s.userdata = &audio;        
  s.freq = 44100;
  s.channels = 2;
  s.samples = 1024;
  s.callback = (SDL_AudioCallback)callback_func;
  SDL_InitSubSystem(SDL_INIT_AUDIO);
  printf("Trying to open audio\n");
  SDL_OpenAudio(&s, NULL);
  printf("Trying to play audio\n");
  SDL_PauseAudio(0);
}
