#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

# define W    400
# define H    300

void __am_gpu_init() {
  /*
  //vgactl_port_base[0] = (screen_width() << 16) | screen_height();
  
  int i;
  uint32_t vga_info = inl(VGACTL_ADDR);
  int w = (vga_info >> 16) & 0xffff;
  int h = vga_info & 0xffff; 
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 1);
  */
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  uint32_t vga_info = inl(VGACTL_ADDR);
  int w = (vga_info >> 16) & 0xffff;
  int h = vga_info & 0xffff; 
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = w, .height = h,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
  if (w == 0 || h == 0) return;
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  uint32_t *pi = ctl->pixels;
  int i, j;              
  for (i = 0; i < h; i ++) {
    for (j = 0; j < w; j ++) {
      fb[(y + i) * W + x + j] = pi[i * w + j];
    }
  }
  outl(SYNC_ADDR, 1);
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
