#ifndef IOINTERRUPTS_HPP_INCLUDED
#define IOINTERRUPTS_HPP_INCLUDED
include "gbstruct.hpp";

void fireInterrups(GB gb, bool &interrupted){
    // We need to fire the interrupts here based on the elapsed cycle counts.
    // The GB runs at 4194304 Hz.

    // VBLANK (Every 1/59.7 seconds/70,256 cycles)
    if (gb.cycles % 70256 < 30){

    }

    // VBLANK lasts about 1.1ms
    if (gb.cycles % 70256 < 30 && gb.cycles % 70256 < 79)
}


#endif // IOINTERRUPTS_HPP_INCLUDED
