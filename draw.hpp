#include <iostream>
#include <fstream>
#include <cstring>
#include <SDL.h>
#include <ctime>
#include "gbstruct.hpp"

typedef unsigned char byte;

using namespace std;

// Render the screen buffer to the screen.
void redraw(GB &gb, SDL_Renderer *renderer, SDL_Texture *texture, Uint32 *pixels)
{
    const unsigned int DARKEST_GREEN = 0x0F380F;

    const unsigned int DARK_GREEN = 0x306230;

    const unsigned int LIGHT_GREEN = 0x8BAC0F;

    const unsigned int LIGHTEST_GREEN = 0x9BBC0F;

    // Each screen is comprised of 32x32 tiles.
    // Each tile is comprised of 8x8 pixels.
    // The screen is comprised of the background (32 rows in VRAM, each 32 bytes long)
    // which says what background tiles to render.

    // There are two different Background Tile Maps. One is
    // located from $9800-9Bff. The other from $9C00-9FFF.

    // These two tile maps show where the tiles should be placed.
    byte * bgtm1 = &gb.mem[0x9800];
    byte * bgtm2 = &gb.mem[0x9C00];

    // These are the actual tiles, stored in the Tile pattern Tables
    // ($8000-8FFF) and ($8800-97FF)
    byte * tpt1 = &gb.mem[0x8000];
    byte * tpt2 = &gb.mem[0x9000];

    //clock_t begin = clock();

    // Render the background.  This is done in chunks of 8x8 tiles.
    for (int i=0; i<32; i++){
        for (int j=0; j<32; j++){
            char tilenum_pre = bgtm1[32*i + j];
            // The tilemap we will use is determined by the LCDC register (0xFF0F, bit 1)
            int tilenum;
            if (gb.mem[0xFF0F] & 0x40 == 0){
                tilenum = tilenum_pre + 127;
            } else {
                // tile numbers are signed
                tilenum = tilenum_pre;
            }

/*            if (tilenum < 0){ // Tile is stored in first tilemap.
                // Tile data is stored in 16 bytes, with each 2 bytes representing
                // an 8 pixel line.
                int linenum = 0;
                for (byte *tileline = tilenum*16 + tpt1; tileline < ((tilenum+1)*16 + tpt1); tileline+=2){
                    short line = *tileline; // 8 pixel lines are 2 bytes each
                    for (int pixel = 7; pixel >= 0; pixel--){
                        byte color = line & 0x3; // we just need the last two bits for color
                        switch (color){
                            case 0:
                                pixels[i*256*8 + linenum * 256 + j*8 + pixel] = DARKEST_GREEN; //tilex
                                break;
                            case 1:
                                pixels[i*256*8 + linenum * 256 + j*8 + pixel] = DARK_GREEN;
                                break;
                            case 2:
                                pixels[i*256*8 + linenum * 256 + j*8 + pixel] = LIGHT_GREEN;
                                break;
                            case 3:
                                pixels[i*256*8 + linenum * 256 + j*8 + pixel] = LIGHTEST_GREEN;
                                break;
                            default:
                                break;
                        }
                        line = line >> 2; // get the next pixel
                    }
                    cout << linenum;
                    linenum++;
                }
            } else { // Tile is stored in second tilemap.
*/                tilenum = tilenum;
                //cout << std::hex << (int)tilenum*16 + 0x9000 << " ";
                for (int linenum = 0; linenum < 8; linenum++){
                    byte *tileline = tilenum*16 + tpt2 + linenum*2;
                    byte line = tileline[0];
                    byte line2 = tileline[1];
                    for (int pixel = 7; pixel >= 0; pixel--){
                        byte color = ((line & 1) << 1 ) | (line2 & 1); // we just need the last two bits for color
                        //cout << (int)color << " ";
                        switch (color){
                            case 0:
                                pixels[i*256*8 + linenum * 256 + j*8 + pixel] = DARKEST_GREEN;
                                break;
                            case 1:
                                pixels[i*256*8 + linenum * 256 + j*8 + pixel] = DARK_GREEN;
                                break;
                            case 2:
                                pixels[i*256*8 + linenum * 256 + j*8 + pixel] = LIGHT_GREEN;
                                break;
                            case 3:
                                pixels[i*256*8 + linenum * 256 + j*8 + pixel] = LIGHTEST_GREEN;
                                break;
                            default:
                                break;
                        }
                        line = line >>  1;
                        line2 = line >> 1;
                    }
                }
            }
            //cout << std::hex << (int)tilenum << " ";
//        }
        //cout << endl;
    }
    //foo

        SDL_UpdateTexture(texture, NULL, pixels, 256 * sizeof(Uint32));

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture, NULL, NULL);

        SDL_RenderPresent(renderer);
}

void setGpuState(GB &gb, SDL_Renderer *renderer, SDL_Texture *texture, Uint32 *pixels){
    // Cycle timings are as follows:
    // H-Blank: 228 cycles (0x80)
    // H-Draw: 1004 cycles (0x83)
    // V-Blank: After 160 lines, for 68 lines (0x81).
    if (gb.gpucount <= 0){
        byte mode = gb.mem[0xFF41];
        byte ly = gb.mem[0xFF44];
        if (ly >= 228){ // Exit V-Blank.  Enter H-Draw.  Reset Wnd-Y.  Redraw screen.  Set timer to 1004 cycles.
            gb.mem[0xFF44] = 0;
            gb.mem[0xFF41] = 0x83;
            gb.gpucount = 1004;
            redraw(gb, renderer, texture, pixels);
        }
        else if (ly >= 160){ // Enter V-Blank.  Increment Wnd-Y.  Set timer to 1232 cycles.
            // If we just hit line 160, fire V-Blank interrupt (CALL 0x40)
            if (ly == 160){
                gb.reg.sp--;
                gb.mem[gb.reg.sp] = ((gb.reg.pc) & 0xFF00) >> 8;
                gb.reg.sp--;
                gb.mem[gb.reg.sp] = (gb.reg.pc) & 0xFF;

                gb.reg.pc = 0x40;
            }
            gb.mem[0xFF41] = 0x81;
            gb.mem[0xFF44] += 1;
            gb.gpucount = 1232;
        }
        else if (mode == 0x83){ // Enter H-Blank.  Increment Wnd-Y.  Set timer to 228 cycles.
            gb.mem[0xFF41] = 0x81;
            gb.mem[0xFF44] += 1;
            gb.gpucount = 228;
        }
        else if (mode == 0x80){ // Enter H-Draw.  Set timer to 1004 cycles.
            gb.mem[0xFF41] = 0x83;
            gb.gpucount = 1004;
        } else { // Not sure where we are at.  Start V-Blank.
            gb.mem[0xFF41] = 0x81;
            gb.mem[0xFF44] = 160;
            gb.gpucount = 1232;
        }
    }
}
