
/*
On the NES, you can't write to video RAM while the PPU
is active, so you have to do it during the vertical blank.
We have a module (vrambuf) which fills up a buffer with video
data, then NESLib writes it to VRAM during the NMI interrupt.
*/

#include "neslib.h"
#include <string.h>
#include <stdio.h>

// VRAM buffer module
#include "vrambuf.h"
//#link "vrambuf.c"

// link the pattern table into CHR ROM
//#link "chr_generic.s"

#define FPS 60

// function to scroll window up and down until end
void scroll_demo() {
  byte pos = 1;
  char str[8];
  byte pad;
  
  while(1) {
    memset(str, 0x00, 8);
    pad = pad_poll(0);
    
    if (pad & PAD_UP) str[0] = 0x1C;
    if (pad & PAD_DOWN) str[1] = 0x1D;
    if (pad & PAD_LEFT) str[2] = 0x1E;
    if (pad & PAD_RIGHT) str[3] = 0x1F;

    if (pad & PAD_START) str[4] = 0x1A;
    if (pad & PAD_SELECT) str[5] = 0x1B;
    if (pad & PAD_A) str[6] = 0x41;
    if (pad & PAD_B) str[7] = 0x42;
    
    if (pad > 0) ++pos;
    if (pos > 26) pos = 0;
    
    vrambuf_put(NTADR_A(4,pos), str, 8);
    vrambuf_flush();
  }
}

// main function, run after console reset
void main(void) {
  // set palette colors
  pal_col(0,0x0F);	// set screen to dark blue
  pal_col(1,0x15);	// pink
  pal_col(2,0x20);	// grey
  pal_col(3,0x30);	// white
  
  // clear vram buffer
  vrambuf_clear();
  
  // set NMI handler
  set_vram_update(updbuf);

  // enable PPU rendering (turn on screen)
  ppu_on_all();

  // scroll window back and forth
  scroll_demo();
}
