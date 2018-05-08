#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <iterator>
#include <algorithm>
#include "include/SDL.h"
#include "opinfo.hpp"
#include "cbops.hpp"
#include "gbstruct.hpp"
#include "draw.hpp"
#include "gbinit.hpp"

using namespace std;
typedef unsigned char byte;

//Key press surfaces constants
enum KeyPressSurfaces{
  KEY_PRESS_SURFACE_DEFAULT,
  KEY_PRESS_SURFACE_UP,
  KEY_PRESS_SURFACE_DOWN,
  KEY_PRESS_SURFACE_LEFT,
  KEY_PRESS_SURFACE_RIGHT,
  KEY_PRESS_SURFACE_TOTAL
};

SDL_Event event;



SDL_Window * window;
SDL_Renderer *renderer;

SDL_Texture *texture;

Uint32 *pixels;

unsigned short get16BArg(GB gb){
((short)gb.mem[gb.reg.pc + 2]) | (((short)gb.mem[gb.reg.pc + 1]) << 8);
}

void dumpRegisters(GB gb){
    cout << "#########################" << endl;
    cout << hex  << "a:" << setfill('0') << setw(2) << +gb.reg.a
                << " bc:" << setfill('0') << setw(2) << +gb.reg.b << setfill('0') << setw(2) << +gb.reg.c
                << " de:" <<  setfill('0') << setw(2) << +gb.reg.d << setfill('0') << setw(2) << +gb.reg.e
                << " hl:" <<  setfill('0') << setw(2) << +gb.reg.h << setfill('0') << setw(2) << +gb.reg.l
                << " sp:" << +gb.reg.sp
                << " pc:" <<  +gb.reg.pc
                << " counter " << dec << gb.gpucount << endl;
    cout << "#########################" << endl;

}


bool init() { //Initialization flag
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("GBEmu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 512, 512, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STATIC, 256, 256);
    pixels = new Uint32[256 * 256];
    memset(pixels, 255, 256 * 256 * sizeof(Uint32));
    SDL_RenderSetScale(renderer, 2, 2);
    return true;
}

void close() {
    //Deallocate surface
    //SDL_FreeSurface( gHelloWorld );
    //gHelloWorld = NULL;
    //Destroy window
    SDL_DestroyWindow( window );
    window = NULL;
    //Quit SDL subsystems
    SDL_Quit();
}

int startemu()
{

    GB gb;
    // clear memory
    for (int i = 0; i < 65536; i++)
    {
        gb.mem[i] = 0;
    }
    initRam(gb);
    dumpRegisters(gb);

    FILE * file = fopen("TESTGAME.GB", "rb+");
    int furthestint = 0x100;
    if (file == NULL) return 1;
    fseek(file, 0, SEEK_END);
    long int size = ftell(file);
    fclose(file);
    // Reading data to array of unsigned chars
    file = fopen("TESTGAME.GB", "rb+");
    byte * cart = (byte *) malloc(size);
    int bytes_read = fread(cart, sizeof(byte), size, file);
    fclose(file);

    memcpy(gb.mem, cart, 0x7FFF*sizeof(byte));

    cout << std::hex << (int)gb.mem[0] << endl;

    bool stopped = false;
    bool interruptEnabled = true;

    //Event handler
    SDL_Event e;

    gb.reg.pc = 0x100;

    unsigned short furthestinst = 0;
    cout << "start execution" << endl;
    while (gb.reg.pc <= 0xFFFF)
    {
        // event handling code
        if (interruptEnabled){
            while( SDL_PollEvent( &e ) != 0 )
            {
                if (e.type == SDL_KEYDOWN){
                    dumpRegisters(gb);
                    // continue execution
                    stopped = false;
                    //Pass event
                    switch( e.key.keysym.sym ) {
                        default:

                            break;
                    }
                }
            }
        }
        if (!stopped)
        { // Eventualy need this to wait for interrupt
            // Fetch
            byte inst = gb.mem[gb.reg.pc];
            cout<< "0x" << hex << +gb.reg.pc << ":\t";
            // Execute.  Switch statements are by far the fastest in terms of exectution time, as they get
            // translated into jump tables.  Code gets a bit messy.
            switch (inst)
            {
            case 0x0:
            { //    NOP
                cout<< "NOP" << endl;
                break;
            }
            case 0x1:
            { //    LD BC,d16
                short unsigned addr = get16BArg(gb);
                gb.reg.b = gb.mem[gb.reg.pc + 2];
                gb.reg.c = gb.mem[gb.reg.pc + 1];
                cout<< "LD BC," << hex << "0x" << addr << endl;
            }
            case 0x2:
            { //    LD (BC),A
              // Write A into address BC
                gb.mem[gb.reg.bc()] = gb.reg.a;
                cout<< "LD (BC),A" << endl;
                break;
            }
            case 0x3:
            { //    INC BC
                gb.reg.bcstore(gb.reg.bc() + 1);
                cout<< "INC BC" << endl;
                break;
            }
            case 0x4:
            { //    INC B
                gb.reg.b++;
                cout<< "INC B" << endl;
                break;
            }
            case 0x5:
            { //    DEC B
                gb.reg.b--;
                gb.flag.h = gb.reg.b & 0xF != 0xF;
                gb.flag.z = gb.reg.b == 0;
                gb.flag.n = 1;
                cout<< "DEC B" << endl;
                break;
            }
            case 0x6:
            { //    LD B,d8
                byte imm = gb.mem[gb.reg.pc + 1];
                gb.reg.b = imm;
                cout<< "LD B," << hex << +imm << endl;
                break;
            }
            case 0x7:
            { //    RLCA
                cout<< "UNIMPLEMENTED 0x7" << endl;
                break;
            }
            case 0x8:
            { //    LD (a16),SP
                cout<< "UNIMPLEMENTED 0x8" << endl;
                break;
            }
            case 0x9:
            { //    ADD HL,BC
                cout<< "UNIMPLEMENTED 0x9" << endl;
                break;
            }
            case 0xA:
            { //    LD A,(BC)
                gb.reg.a = gb.mem[gb.reg.bc()];
                cout<< "LD A,(BC)" << endl;
                break;
            }
            case 0xB:
            { //    DEC BC
                gb.reg.bcstore(gb.reg.bc() - 1);
                cout<< "DEC BC" << endl;
                break;
            }
            case 0xC:
            { //    INC C
                gb.reg.c++;
                gb.flag.z = gb.reg.c == 0;
                gb.flag.n = 0;
                gb.flag.h = gb.reg.c << 4;
                cout<< "INC C" << endl;
                break;
            }
            case 0xD:
            { //    DEC C
                gb.reg.c--;
                gb.flag.h = gb.reg.c & 0xF != 0xF;
                gb.flag.z = gb.reg.c == 0;
                gb.flag.n = 1;
                cout<< "DEC C" << endl;
                break;
            }
            case 0xE:
            { //    LD C,d8
                byte imm = gb.mem[gb.reg.pc + 1];
                gb.reg.c = imm;
                cout<< "LD C, " << dec << +imm << endl;
                break;
            }
            case 0xF:
            { //    RRCA
                dumpRegisters(gb);
                break;
                gb.flag.c = gb.reg.a & 1;
                gb.reg.a = gb.reg.a >> 1 | ((byte)gb.flag.c << 7);
                gb.flag.n = 0;
                gb.flag.h = 0;
                gb.flag.z = gb.reg.a;
                cout<< "RRCA" << endl;
                break;
            }
            case 0x10:
            { //   STOP 0
                // Halt execution until interrupt.
                stopped = true;
                cout<< "STOP" << endl;
                break;
            }
            case 0x11:
            { //   LD DE,d16
                short unsigned addr = get16BArg(gb);
                gb.reg.d = gb.mem[gb.reg.pc + 2];
                gb.reg.e = gb.mem[gb.reg.pc + 1];
                cout<< "LD DE," << hex << "0x" << addr << endl;
                break;
            }
            case 0x12:
            { //   LD (DE),A
                gb.mem[gb.reg.de()] = gb.reg.a;
                cout<< "LD (DE),A" << endl;
                break;
            }
            case 0x13:
            { //   INC DE
                gb.reg.destore(gb.reg.de() + 1);
                cout<< "INC DE" << endl;
                break;
            }
            case 0x14:
            { //   INC D
                gb.reg.d++;
                gb.flag.z = gb.reg.d == 0;
                gb.flag.n = 0;
                gb.flag.h = gb.reg.d << 4;
                cout<< "INC D" << endl;
                break;
            }
            case 0x15:
            { //   DEC D
                gb.reg.d--;
                gb.flag.h = gb.reg.d & 0xF != 0xF;
                gb.flag.z = gb.reg.d == 0;
                gb.flag.n = 1;
                cout<< "DEC D" << endl;
                break;
            }
            case 0x16:
            { //   LD D,d8
                gb.reg.d = gb.mem[gb.reg.pc + 1];
                cout<< "LD D," << hex << gb.reg.d << endl;
                break;
            }
            case 0x17:
            { //   RLA
                gb.flag.c = gb.reg.a >> 7;
                gb.reg.a = gb.reg.a << 1 | gb.reg.a >> 7;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                gb.flag.h = 0;

                cout<< "RLA" << endl;
                break;
            }
            case 0x18:
            { //   JR r8
                cout<< "UNIMPLEMENTED 0x18" << endl;
                break;
            }
            case 0x19:
            { //   ADD HL,DE
                unsigned short result = gb.reg.hl() + gb.reg.de();
                gb.flag.n = 0;
                gb.reg.hlstore(result);
                //fixme: add flags h and c
                cout<< "ADD HL,DE" << endl;
                break;
            }
            case 0x1A:
            { //   LD A,(DE)
                gb.reg.a = gb.mem[gb.reg.de()];
                cout<< "LD A,(DE)" << endl;
                break;
            }
            case 0x1B:
            { //   DEC DE
                gb.reg.destore(gb.reg.de() - 1);
                cout<< "DEC DE" << endl;
                break;
            }
            case 0x1C:
            { //   INC E
                gb.reg.e++;
                gb.flag.z = gb.reg.e == 0;
                gb.flag.n = 0;
                gb.flag.h = gb.reg.e << 4;
                cout<< "INC E" << endl;
                break;
            }
            case 0x1D:
            { //   DEC E
                cout << hex << gb.reg.e << endl;
                gb.reg.e--;
                gb.flag.h = gb.reg.e & 0xF != 0xF;
                gb.flag.z = gb.reg.e == 0;
                gb.flag.n = 1;
                cout<< "DEC E" << endl;
                break;
            }
            case 0x1E:
            { //   LD E,d8
                gb.reg.e = gb.mem[gb.reg.pc + 1];
                cout<< "LD E," << hex << gb.reg.e << endl;
                break;
            }
            case 0x1F:
            { //   RRA
                cout<< "UNIMPLEMENTED 0x1F" << endl;
                break;
            }
            case 0x20:
            { //   JR NZ,r8
                char addr = gb.mem[gb.reg.pc + 1];
                if (!gb.flag.z)
                {
                    gb.reg.pc += addr;
                }
                cout<< "JR NZ, " << dec << +addr << endl;
                break;
            }
            case 0x21:
            { //   LD HL,d16
                short unsigned addr = get16BArg(gb);
                gb.reg.h = gb.mem[gb.reg.pc + 2];
                gb.reg.l = gb.mem[gb.reg.pc + 1];
                cout<< "LD HL," << hex << "0x" << +addr << endl;
                break;
            }
            case 0x22:
            { //   LD (HL+),A
                gb.mem[gb.reg.hl()] = gb.reg.a;
                gb.reg.hlstore(gb.reg.hl() + 1);
                cout<< "LD (HL+),A" << endl;
                break;
            }
            case 0x23:
            { //   INC HL
                gb.reg.hlstore(gb.reg.hl() + 1);
                cout<< "INC HL" << endl;
                break;
            }
            case 0x24:
            { //   INC H
                cout<< "UNIMPLEMENTED 0x24" << endl;
                break;
            }
            case 0x25:
            { //   DEC H
                cout<< "UNIMPLEMENTED 0x25" << endl;
                break;
            }
            case 0x26:
            { //   LD H,d8
                cout<< "UNIMPLEMENTED 0x26" << endl;
                break;
            }
            case 0x27:
            { //   DAA
                cout<< "UNIMPLEMENTED 0x27" << endl;
                break;
            }
            case 0x28:
            { //   JR Z,r8/8
                cout<< "UNIMPLEMENTED 0x28" << endl;
                break;
            }
            case 0x29:
            { //   ADD HL,HL
                gb.reg.hlstore(gb.reg.hl() << 1);
                gb.flag.n = 0;
                //FIXME: Add H and C flags
                cout<< "ADD HL,HL" << endl;
                break;
            }
            case 0x2A:
            { //   LD A,(HL+)
                gb.reg.a = gb.mem[gb.reg.hl()];
                gb.reg.hlstore(gb.reg.hl() + 1);
                cout<< "LD A,(HL+)" << endl;
                break;
            }
            case 0x2B:
            { //   DEC HL
                cout<< "UNIMPLEMENTED 0x2B" << endl;
                break;
            }
            case 0x2C:
            { //   INC L
                cout<< "UNIMPLEMENTED 0x2C" << endl;
                break;
            }
            case 0x2D:
            { //   DEC L
                cout<< "UNIMPLEMENTED 0x2D" << endl;
                break;
            }
            case 0x2E:
            { //   LD L,d8
                cout<< "UNIMPLEMENTED 0x2E" << endl;
                break;
            }
            case 0x2F:
            { //   CPL
                gb.reg.a = ~gb.reg.a;
                gb.flag.n = 1;
                gb.flag.h = 1;
                cout<< "CPL" << endl;
                break;
            }
            case 0x30:
            { //   JR NC,r8
                cout<< "UNIMPLEMENTED 0x30" << endl;
                break;
            }
            case 0x31:
            { //   LD SP,d16
                short unsigned addr = get16BArg(gb);
                gb.reg.sp = addr;
                cout<< "LD SP," << hex << "0x" << addr << endl;
                break;
            }
            case 0x32:
            { //   LD (HL-),A
                gb.mem[gb.reg.hl()] = gb.reg.a;
                gb.reg.hlstore(gb.reg.hl() - 1);
                cout<< "LD (HL-),A" << endl;
                break;
            }
            case 0x33:
            { //   INC SP
                gb.reg.sp++;
                cout<< "INC SP" << endl;
                break;
            }
            case 0x34:
            { //   INC (HL)
                cout<< "UNIMPLEMENTED 0x34" << endl;
                break;
            }
            case 0x35:
            { //   DEC (HL)
                cout<< "UNIMPLEMENTED 0x35" << endl;
                break;
            }
            case 0x36:
            { //   LD (HL),d8
                gb.mem[gb.reg.hl()] = gb.mem[gb.reg.pc+1];
                cout<< "LD (HL),d8" << endl;
                break;
            }
            case 0x37:
            { //   SCF
                cout<< "UNIMPLEMENTED 0x37" << endl;
                break;
            }
            case 0x38:
            { //   JR C,r8/8
                char addr = gb.mem[gb.reg.pc + 1];
                if (gb.flag.c){
                    gb.reg.pc += addr - 2;
                }
                cout<< "JR C, " << hex << +addr << endl;
                break;
            }
            case 0x39:
            { //   ADD HL,SP
                cout<< "UNIMPLEMENTED 0x39" << endl;
                break;
            }
            case 0x3A:
            { //   LD A,(HL-)
                cout<< "UNIMPLEMENTED 0x3A" << endl;
                break;
            }
            case 0x3B:
            { //   DEC SP
                cout<< "UNIMPLEMENTED 0x3B" << endl;
                break;
            }
            case 0x3C:
            { //   INC A
                cout<< "UNIMPLEMENTED 0x3C" << endl;
                break;
            }
            case 0x3D:
            { //   DEC A
                cout<< "UNIMPLEMENTED 0x3D" << endl;
                break;
            }
            case 0x3E:
            { //   LD A,d8
                byte imm = gb.mem[gb.reg.pc + 1];
                gb.reg.a = imm;
                cout<< "LD A, " << dec << +imm << endl;
                break;
            }
            case 0x3F:
            { //   CCF
                cout<< "UNIMPLEMENTED 0x3F" << endl;
                break;
            }
            case 0x40:
            { //   LD B,B
                cout<< "LD B,B" << endl;
                break;
            }
            case 0x41:
            { //   LD B,C
                gb.reg.b = gb.reg.c;
                cout<< "LD B,C" << endl;
                break;
            }
            case 0x42:
            { //   LD B,D
                gb.reg.b = gb.reg.d;
                cout<< "LD B,D" << endl;
                break;
            }
            case 0x43:
            { //   LD B,E
                gb.reg.b = gb.reg.e;
                cout<< "LD B,E" << endl;
                break;
            }
            case 0x44:
            { //   LD B,H
                gb.reg.b = gb.reg.h;
                cout<< "LD B,H" << endl;
                break;
            }
            case 0x45:
            { //   LD B,L
                gb.reg.b = gb.reg.l;
                cout<< "LD B,L" << endl;
                break;
            }
            case 0x46:
            { //   LD B,(HL)
                gb.reg.b = gb.mem[gb.reg.hl()];
                cout<< "LD B,(HL)" << endl;
                break;
            }
            case 0x47:
            { //   LD B,A
                gb.reg.b = gb.reg.a;
                cout<< "LD B,A" << endl;
                break;
            }
            case 0x48:
            { //   LD C,B
                gb.reg.c = gb.reg.b;
                cout<< "LD C,B" << endl;
                break;
            }
            case 0x49:
            { //   LD C,C
                cout<< "LD C,C" << endl;
                break;
            }
            case 0x4A:
            { //   LD C,D
                gb.reg.c = gb.reg.d;
                cout<< "LD C,D" << endl;
                break;
            }
            case 0x4B:
            { //   LD C,E
                gb.reg.c = gb.reg.e;
                cout<< "LD C,E" << endl;
                break;
            }
            case 0x4C:
            { //   LD C,H
                gb.reg.c = gb.reg.h;
                cout<< "LD C,H" << endl;
                break;
            }
            case 0x4D:
            { //   LD C,L
                gb.reg.c = gb.reg.l;
                cout<< "LD C,L" << endl;
                break;
            }
            case 0x4E:
            { //   LD C,(HL)
                gb.reg.c = gb.mem[gb.reg.hl()];
                cout<< "LD C,(HL)" << endl;
                break;
            }
            case 0x4F:
            { //   LD C,A
                gb.reg.c = gb.reg.a;
                cout<< "LD C,A" << endl;
                break;
            }
            case 0x50:
            { //   LD D,B
                gb.reg.d = gb.reg.b;
                cout<< "LD D,B" << endl;
                break;
            }
            case 0x51:
            { //   LD D,C
                gb.reg.d = gb.reg.c;
                cout<< "LD D,C" << endl;
                break;
            }
            case 0x52:
            { //   LD D,D
                cout<< "LD D,D" << endl;
                break;
            }
            case 0x53:
            { //   LD D,E
                gb.reg.d = gb.reg.e;
                cout<< "LD D,E" << endl;
                break;
            }
            case 0x54:
            { //   LD D,H
                gb.reg.d = gb.reg.h;
                cout<< "LD D,H" << endl;
                break;
            }
            case 0x55:
            { //   LD D,L
                gb.reg.d = gb.reg.l;
                cout<< "LD D,L" << endl;
                break;
            }
            case 0x56:
            { //   LD D,(HL)
                gb.reg.d = gb.mem[gb.reg.hl()];
                cout<< "LD D,(HL)" << endl;
                break;
            }
            case 0x57:
            { //   LD D,A
                gb.reg.d = gb.reg.a;
                cout<< "LD D,A" << endl;
                break;
            }
            case 0x58:
            { //   LD E,B
                gb.reg.e = gb.reg.b;
                cout<< "LD E,B" << endl;
                break;
            }
            case 0x59:
            { //   LD E,C
                gb.reg.e = gb.reg.c;
                cout<< "LD E,C" << endl;
                break;
            }
            case 0x5A:
            { //   LD E,D
                gb.reg.e = gb.reg.d;
                cout<< "LD E,D" << endl;
                break;
            }
            case 0x5B:
            { //   LD E,E
                cout<< "LD E,E" << endl;
                break;
            }
            case 0x5C:
            { //   LD E,H
                gb.reg.e = gb.reg.h;
                cout<< "LD E,H" << endl;
                break;
            }
            case 0x5D:
            { //   LD E,L
                gb.reg.e = gb.reg.l;
                cout<< "LD E,L" << endl;
                break;
            }
            case 0x5E:
            { //   LD E,(HL)
                gb.reg.e = gb.mem[gb.reg.hl()];
                cout<< "LD E,(HL)" << endl;
                break;
            }
            case 0x5F:
            { //   LD E,A
                gb.reg.e = gb.reg.a;
                cout<< "LD E,A" << endl;
                break;
            }
            case 0x60:
            { //   LD H,B
                gb.reg.h = gb.reg.b;
                cout<< "LD H,B" << endl;
                break;
            }
            case 0x61:
            { //   LD H,C
                gb.reg.h = gb.reg.c;
                cout<< "LD H,C" << endl;
                break;
            }
            case 0x62:
            { //   LD H,D
                gb.reg.h = gb.reg.d;
                cout<< "LD H,D" << endl;
                break;
            }
            case 0x63:
            { //   LD H,E
                gb.reg.h = gb.reg.e;
                cout<< "LD H,E" << endl;
                break;
            }
            case 0x64:
            { //   LD H,H
                cout<< "LD H,H" << endl;
                break;
            }
            case 0x65:
            { //   LD H,L
                gb.reg.h = gb.reg.l;
                cout<< "LD H,L" << endl;
                break;
            }
            case 0x66:
            { //   LD H,(HL)
                gb.reg.h = gb.mem[gb.reg.hl()];
                cout<< "LD H,(HL)" << endl;
                break;
            }
            case 0x67:
            { //   LD H,A
                gb.reg.h = gb.reg.a;
                cout<< "LD H,A" << endl;
                break;
            }
            case 0x68:
            { //   LD L,B
                gb.reg.l = gb.reg.b;
                cout<< "LD L,B" << endl;
                break;
            }
            case 0x69:
            { //   LD L,C
                gb.reg.l = gb.reg.c;
                cout<< "LD L,C" << endl;
                break;
            }
            case 0x6A:
            { //   LD L,D
                gb.reg.l = gb.reg.d;
                cout<< "LD L,D" << endl;
                break;
            }
            case 0x6B:
            { //   LD L,E
                gb.reg.l = gb.reg.e;
                cout<< "LD L,E" << endl;
                break;
            }
            case 0x6C:
            { //   LD L,H
                gb.reg.l = gb.reg.h;
                cout<< "LD L,H" << endl;
                break;
            }
            case 0x6D:
            { //   LD L,L
                cout<< "LD L,L" << endl;
                break;
            }
            case 0x6E:
            { //   LD L,(HL)
                gb.reg.l = gb.mem[gb.reg.hl()];
                cout<< "LD L,(HL)" << endl;
                break;
            }
            case 0x6F:
            { //   LD L,A
                gb.reg.l = gb.reg.a;
                cout<< "LD L,A" << endl;
                break;
            }
            case 0x70:
            { //   LD (HL),B
                gb.mem[gb.reg.hl()] = gb.reg.b;
                cout<< "LD (HL),B" << endl;
                break;
            }
            case 0x71:
            { //   LD (HL),C
                gb.mem[gb.reg.hl()] = gb.reg.c;
                cout<< "LD (HL),C" << endl;
                break;
            }
            case 0x72:
            { //   LD (HL),D
                gb.mem[gb.reg.hl()] = gb.reg.d;
                cout<< "LD (HL),D" << endl;
                break;
            }
            case 0x73:
            { //   LD (HL),E
                gb.mem[gb.reg.hl()] = gb.reg.e;
                cout<< "LD (HL),E" << endl;
                break;
            }
            case 0x74:
            { //   LD (HL),H
                gb.mem[gb.reg.hl()] = gb.reg.h;
                cout<< "LD (HL),H" << endl;
                break;
            }
            case 0x75:
            { //   LD (HL),L
                gb.mem[gb.reg.hl()] = gb.reg.l;
                cout<< "LD (HL),L" << endl;
                break;
            }
            case 0x76:
            { //   HALT
                stopped = true;
                cout<< "HALT" << endl;
                break;
            }
            case 0x77:
            { //   LD (HL),A
                gb.mem[gb.reg.hl()] = gb.reg.a;
                cout<< "LD (HL),A" << endl;
                break;
            }
            case 0x78:
            { //   LD A,B
                gb.reg.a = gb.reg.b;
                cout<< "LD A,B" << endl;
                break;
            }
            case 0x79:
            { //   LD A,C
                gb.reg.a = gb.reg.c;
                cout<< "LD A,C" << endl;
                break;
            }
            case 0x7A:
            { //   LD A,D
                gb.reg.a = gb.reg.d;
                cout<< "LD A,D" << endl;
                break;
            }
            case 0x7B:
            { //   LD A,E
                gb.reg.a = gb.reg.e;
                cout<< "LD A,E" << endl;
                break;
            }
            case 0x7C:
            { //   LD A,H
                gb.reg.a = gb.reg.h;
                cout<< "LD A,H" << endl;
                break;
            }
            case 0x7D:
            { //   LD A,L
                gb.reg.a = gb.reg.l;
                cout<< "LD A,L" << endl;
                break;
            }
            case 0x7E:
            { //   LD A,(HL)
                gb.reg.a = gb.mem[gb.reg.hl()];
                cout<< "LD A,(HL)" << endl;
                break;
            }
            case 0x7F:
            { //   LD A,A
                cout<< "LD A,A" << endl;
                break;
            }
            case 0x80:
            { //   ADD A,B
                short carrycheck = (short)gb.reg.a + (short)gb.reg.b;
                byte hc = gb.reg.c & 0xF + gb.reg.b & 0xF;
                gb.reg.a = gb.reg.a + gb.reg.b;
                gb.flag.c = carrycheck & 0x100;
                gb.flag.h = hc & 0x10;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                cout<< "ADD A,B" << endl;
                break;
            }
            case 0x81:
            { //   ADD A,C
                short carrycheck = (short)gb.reg.a + (short)gb.reg.c;
                byte hc = gb.reg.c & 0xF + gb.reg.c & 0xF;
                gb.reg.a = gb.reg.a + gb.reg.c;
                gb.flag.c = carrycheck & 0x100;
                gb.flag.h = hc & 0x10;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                cout<< "ADD A,C" << endl;
                break;
            }
            case 0x82:
            { //   ADD A,D
                short carrycheck = (short)gb.reg.a + (short)gb.reg.d;
                byte hc = gb.reg.c & 0xF + gb.reg.d & 0xF;
                gb.reg.a = gb.reg.a + gb.reg.d;
                gb.flag.c = carrycheck & 0x100;
                gb.flag.h = hc & 0x10;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                cout<< "ADD A,D" << endl;
                break;
            }
            case 0x83:
            { //   ADD A,E
                short carrycheck = (short)gb.reg.a + (short)gb.reg.e;
                byte hc = gb.reg.c & 0xF + gb.reg.e & 0xF;
                gb.reg.a = gb.reg.a + gb.reg.e;
                gb.flag.c = carrycheck & 0x100;
                gb.flag.h = hc & 0x10;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                cout<< "ADD A,E" << endl;
                break;
            }
            case 0x84:
            { //   ADD A,H
                short carrycheck = (short)gb.reg.a + (short)gb.reg.h;
                byte hc = gb.reg.c & 0xF + gb.reg.h & 0xF;
                gb.reg.a = gb.reg.a + gb.reg.h;
                gb.flag.c = carrycheck & 0x100;
                gb.flag.h = hc & 0x10;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                cout<< "ADD A,H" << endl;
                break;
            }
            case 0x85:
            { //   ADD A,L
                short carrycheck = (short)gb.reg.a + (short)gb.reg.l;
                byte hc = gb.reg.c & 0xF + gb.reg.l & 0xF;
                gb.reg.a = gb.reg.a + gb.reg.l;
                gb.flag.c = carrycheck & 0x100;
                gb.flag.h = hc & 0x10;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                cout<< "ADD A,L" << endl;
                break;
            }
            case 0x86:
            { //   ADD A,(HL)
                cout<< "UNIMPLEMENTED 0x86" << endl;
                break;
            }
            case 0x87:
            { //   ADD A,A
                short carrycheck = (short)gb.reg.a + (short)gb.reg.a;
                byte hc = gb.reg.c & 0xF + gb.reg.a & 0xF;
                gb.reg.a = gb.reg.a + gb.reg.a;
                gb.flag.c = carrycheck & 0x100;
                gb.flag.h = hc & 0x10;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                cout<< "ADD A,A" << endl;
                break;
            }
            case 0x88:
            { //   ADC A,B
                short carrycheck = (short)gb.reg.a + (short)gb.reg.b + (short)gb.flag.c;
                byte hc = gb.reg.a & 0xF + gb.reg.b & 0xF + gb.flag.c;
                gb.reg.a = gb.reg.a + gb.reg.b + gb.flag.c;
                gb.flag.c = carrycheck & 0x100;
                gb.flag.h = hc & 0x10;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;

                cout<< "ADC A,B" << endl;
                break;
            }
            case 0x89:
            { //   ADC A,C
                short carrycheck = (short)gb.reg.a + (short)gb.reg.c + (short)gb.flag.c;
                byte hc = gb.reg.a & 0xF + gb.reg.c & 0xF + gb.flag.c;
                gb.reg.a = gb.reg.a + gb.reg.c + gb.flag.c;
                gb.flag.c = carrycheck & 0x100;
                gb.flag.h = hc & 0x10;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                cout<< "ADC A,C" << endl;
                break;
            }
            case 0x8A:
            { //   ADC A,D
                short carrycheck = (short)gb.reg.a + (short)gb.reg.d + (short)gb.flag.c;
                byte hc = gb.reg.a & 0xF + gb.reg.d & 0xF + gb.flag.c;
                gb.reg.a = gb.reg.a + gb.reg.d + gb.flag.c;
                gb.flag.c = carrycheck & 0x100;
                gb.flag.h = hc & 0x10;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                cout<< "ADC A,D" << endl;
                break;
            }
            case 0x8B:
            { //   ADC A,E
                short carrycheck = (short)gb.reg.a + (short)gb.reg.e + (short)gb.flag.c;
                byte hc = gb.reg.a & 0xF + gb.reg.e & 0xF + gb.flag.c;
                gb.reg.a = gb.reg.a + gb.reg.e + gb.flag.c;
                gb.flag.c = carrycheck & 0x100;
                gb.flag.h = hc & 0x10;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                cout<< "ADC A,E" << endl;
                break;
            }
            case 0x8C:
            { //   ADC A,H
                short carrycheck = (short)gb.reg.a + (short)gb.reg.h + (short)gb.flag.c;
                byte hc = gb.reg.a & 0xF + gb.reg.h & 0xF + gb.flag.c;
                gb.reg.a = gb.reg.a + gb.reg.h + gb.flag.c;
                gb.flag.c = carrycheck & 0x100;
                gb.flag.h = hc & 0x10;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                cout<< "ADC A,H" << endl;
                break;
            }
            case 0x8D:
            { //   ADC A,L
                short carrycheck = (short)gb.reg.a + (short)gb.reg.l + (short)gb.flag.c;
                byte hc = gb.reg.a & 0xF + gb.reg.l & 0xF + gb.flag.c;
                gb.reg.a = gb.reg.a + gb.reg.l + gb.flag.c;
                gb.flag.c = carrycheck & 0x100;
                gb.flag.h = hc & 0x10;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                cout<< "ADC A,L" << endl;
                break;
            }
            case 0x8E:
            { //   ADC A,(HL)
                cout<< "UNIMPLEMENTED 0x8E" << endl;
                break;
            }
            case 0x8F:
            { //   ADC A,A
                short carrycheck = (short)gb.reg.a + (short)gb.reg.a + (short)gb.flag.c;
                byte hc = gb.reg.a & 0xF + gb.reg.a & 0xF + gb.flag.c;
                gb.reg.a = gb.reg.a + gb.reg.a + gb.flag.c;
                gb.flag.c = carrycheck & 0x100;
                gb.flag.h = hc & 0x10;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                cout<< "ADC A, A" << endl;
                break;
            }
            case 0x90:
            { //   SUB B
                gb.reg.a -= gb.reg.b;
                gb.flag.n = 1;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.h = gb.reg.a & 0xFF == 0xFF;
                cout<< "SUB B" << endl;
                break;
            }
            case 0x91:
            { //   SUB C
                gb.reg.a -= gb.reg.c;
                gb.flag.n = 1;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.h = gb.reg.a & 0xFF == 0xFF;
                cout<< "SUB C" << endl;
                break;
            }
            case 0x92:
            { //   SUB D
                gb.reg.a -= gb.reg.d;
                gb.flag.n = 1;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.h = gb.reg.a & 0xFF == 0xFF;
                cout<< "SUB D" << endl;
                break;
            }
            case 0x93:
            { //   SUB E
                gb.reg.a -= gb.reg.e;
                gb.flag.n = 1;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.h = gb.reg.a & 0xFF == 0xFF;
                cout<< "SUB E" << endl;
                break;
            }
            case 0x94:
            { //   SUB H
                gb.reg.a -= gb.reg.h;
                gb.flag.n = 1;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.h = gb.reg.a & 0xFF == 0xFF;
                cout<< "SUB H" << endl;
                break;
            }
            case 0x95:
            { //   SUB L
                gb.reg.a -= gb.reg.l;
                gb.flag.n = 1;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.h = gb.reg.a & 0xFF == 0xFF;
                cout<< "SUB L" << endl;
                break;
            }
            case 0x96:
            { //   SUB (HL)
                gb.reg.a -= gb.mem[gb.reg.hl()];
                gb.flag.n = 1;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.h = gb.reg.a & 0xFF == 0xFF;
                cout<< "SUB (HL)" << endl;
                break;
            }
            case 0x97:
            { //   SUB A
                gb.reg.a = 0;
                gb.flag.z = 1;
                gb.flag.n = 1;
                gb.flag.c = 0;
                cout<< "SUB A" << endl;
                break;
            }
            case 0x98:
            { //   SBC A,B
                cout<< "UNIMPLEMENTED 0x98" << endl;
                break;
            }
            case 0x99:
            { //   SBC A,C
                cout<< "UNIMPLEMENTED 0x99" << endl;
                break;
            }
            case 0x9A:
            { //   SBC A,D
                cout<< "UNIMPLEMENTED 0x9A" << endl;
                break;
            }
            case 0x9B:
            { //   SBC A,E
                cout<< "UNIMPLEMENTED 0x9B" << endl;
                break;
            }
            case 0x9C:
            { //   SBC A,H
                cout<< "UNIMPLEMENTED 0x9C" << endl;
                break;
            }
            case 0x9D:
            { //   SBC A,L
                cout<< "UNIMPLEMENTED 0x9D" << endl;
                break;
            }
            case 0x9E:
            { //   SBC A,(HL)
                cout<< "UNIMPLEMENTED 0x9E" << endl;
                break;
            }
            case 0x9F:
            { //   SBC A,A
                cout<< "UNIMPLEMENTED 0x9F" << endl;
                break;
            }
            case 0xA0:
            { //   AND B
                gb.reg.a &= gb.reg.b;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                gb.flag.h = 1;
                gb.flag.c = 0;
                cout<< "AND B" << endl;
                break;
            }
            case 0xA1:
            { //   AND C
                gb.reg.a &= gb.reg.c;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                gb.flag.h = 1;
                gb.flag.c = 0;
                cout<< "AND C" << endl;
                break;
            }
            case 0xA2:
            { //   AND D
                gb.reg.a &= gb.reg.d;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                gb.flag.h = 1;
                gb.flag.c = 0;
                cout<< "AND D" << endl;
                break;
            }
            case 0xA3:
            { //   AND E
                gb.reg.a &= gb.reg.e;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                gb.flag.h = 1;
                gb.flag.c = 0;
                cout<< "AND E" << endl;
                break;
            }
            case 0xA4:
            { //   AND H
                gb.reg.a &= gb.reg.h;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                gb.flag.h = 1;
                gb.flag.c = 0;
                cout<< "AND H" << endl;
                break;
            }
            case 0xA5:
            { //   AND L
                gb.reg.a &= gb.reg.l;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                gb.flag.h = 1;
                gb.flag.c = 0;
                cout<< "AND L" << endl;
                break;
            }
            case 0xA6:
            { //   AND (HL)
                gb.reg.a &= gb.mem[gb.reg.hl()];
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                gb.flag.h = 1;
                gb.flag.c = 0;
                cout<< "AND (HL)" << endl;
                break;
            }
            case 0xA7:
            { //   AND A
                gb.reg.a &= gb.reg.a;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                gb.flag.h = 1;
                gb.flag.c = 0;
                cout<< "AND A" << endl;
                break;
            }
            case 0xA8:
            { //   XOR B
                gb.reg.a ^= gb.reg.b;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                gb.flag.h = 0;
                gb.flag.c = 0;
                cout<< "XOR B" << endl;
                break;
            }
            case 0xA9:
            { //   XOR C
                gb.reg.a ^= gb.reg.c;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                gb.flag.h = 0;
                gb.flag.c = 0;
                cout<< "XOR C" << endl;
                break;
            }
            case 0xAA:
            { //   XOR D
                gb.reg.a ^= gb.reg.d;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                gb.flag.h = 0;
                gb.flag.c = 0;
                cout<< "XOR D" << endl;
                break;
            }
            case 0xAB:
            { //   XOR E
                gb.reg.a ^= gb.reg.e;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                gb.flag.h = 0;
                gb.flag.c = 0;
                cout<< "XOR E" << endl;
                break;
            }
            case 0xAC:
            { //   XOR H
                gb.reg.a ^= gb.reg.h;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                gb.flag.h = 0;
                gb.flag.c = 0;
                cout<< "XOR H" << endl;
                break;
            }
            case 0xAD:
            { //   XOR L
                gb.reg.a ^= gb.reg.l;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                gb.flag.h = 0;
                gb.flag.c = 0;
                cout<< "XOR L" << endl;
                break;
            }
            case 0xAE:
            { //   XOR (HL)
                gb.reg.a ^= gb.mem[gb.reg.hl()];
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                gb.flag.h = 0;
                gb.flag.c = 0;
                cout<< "XOR (HL)" << endl;
                break;
            }
            case 0xAF:
            { //   XOR A
                gb.reg.a = 0;
                gb.flag.z = true;
                gb.flag.c = false;
                gb.flag.h = false;
                gb.flag.n = false;
                cout<< "XOR A" << endl;
                break;
            }
            case 0xB0:
            { //   OR B
                gb.reg.a = gb.reg.b | gb.reg.a;
                gb.flag.z = gb.reg.a;
                gb.flag.h = 0;
                gb.flag.c = 0;
                gb.flag.n = 0;
                cout<< "OR B" << endl;
                break;
            }
            case 0xB1:
            { //   OR C
                gb.reg.a = gb.reg.c | gb.reg.a;
                gb.flag.z = gb.reg.a;
                gb.flag.h = 0;
                gb.flag.c = 0;
                gb.flag.n = 0;
                cout<< "OR C" << endl;
                break;
            }
            case 0xB2:
            { //   OR D
                gb.reg.a = gb.reg.d | gb.reg.a;
                gb.flag.z = gb.reg.a;
                gb.flag.h = 0;
                gb.flag.c = 0;
                gb.flag.n = 0;
                cout<< "OR D" << endl;
                break;
            }
            case 0xB3:
            { //   OR E
                gb.reg.a = gb.reg.e | gb.reg.a;
                gb.flag.z = gb.reg.a;
                gb.flag.h = 0;
                gb.flag.c = 0;
                gb.flag.n = 0;
                cout<< "OR E" << endl;
                break;
            }
            case 0xB4:
            { //   OR H
                gb.reg.a = gb.reg.h | gb.reg.a;
                gb.flag.z = gb.reg.a;
                gb.flag.h = 0;
                gb.flag.c = 0;
                gb.flag.n = 0;
                cout<< "OR H" << endl;
                break;
            }
            case 0xB5:
            { //   OR L
                gb.reg.a = gb.reg.h | gb.reg.a;
                gb.flag.z = gb.reg.a;
                gb.flag.h = 0;
                gb.flag.c = 0;
                gb.flag.n = 0;
                cout<< "OR L" << endl;
                break;
            }
            case 0xB6:
            { //   OR (HL)
                gb.reg.a = gb.mem[gb.reg.hl()] | gb.reg.a;
                gb.flag.z = gb.reg.a;
                gb.flag.h = 0;
                gb.flag.c = 0;
                gb.flag.n = 0;
                cout<< "OR (HL)" << endl;
                break;
            }
            case 0xB7:
            { //   OR A
                gb.flag.z = gb.reg.a;
                gb.flag.h = 0;
                gb.flag.c = 0;
                gb.flag.n = 0;
                cout<< "OR A" << endl;
                break;
            }
            case 0xB8:
            { //   CP B
                cout<< "UNIMPLEMENTED 0xB8" << endl;
                break;
            }
            case 0xB9:
            { //   CP C
                cout<< "UNIMPLEMENTED 0xB9" << endl;
                break;
            }
            case 0xBA:
            { //   CP D
                cout<< "UNIMPLEMENTED 0xBA" << endl;
                break;
            }
            case 0xBB:
            { //   CP E
                cout<< "UNIMPLEMENTED 0xBB" << endl;
                break;
            }
            case 0xBC:
            { //   CP H
                cout<< "UNIMPLEMENTED 0xBC" << endl;
                break;
            }
            case 0xBD:
            { //   CP L
                cout<< "UNIMPLEMENTED 0xBD" << endl;
                break;
            }
            case 0xBE:
            { //   CP (HL)
                cout<< "UNIMPLEMENTED 0xBE" << endl;
                break;
            }
            case 0xBF:
            { //   CP A
                cout<< "UNIMPLEMENTED 0xBF" << endl;
                break;
            }
            case 0xC0:
            { //   RET NZ
                cout<< "UNIMPLEMENTED 0xC0" << endl;
                break;
            }
            case 0xC1:
            { //   POP BC
                gb.reg.b = gb.mem[gb.reg.sp];
                gb.reg.sp++;
                gb.reg.c = gb.mem[gb.reg.sp];
                gb.reg.sp++;
                cout<< "POP BC" << endl;
                break;
            }
            case 0xC2:
            { //   JP NZ,a16/12
                unsigned short addr = (unsigned short)(((unsigned short)gb.mem[gb.reg.pc + 2]) << 8) | gb.mem[gb.reg.pc + 1];
                if (!gb.flag.z){
                    gb.reg.pc = addr;
                    gb.reg.pc -= 3;
                }
                cout<< "JP NZ," << hex << +addr << endl;
                break;
            }
            case 0xC3:
            { //   JP a16
                short address = (short)(((short)gb.mem[gb.reg.pc+2]) << 8) | gb.mem[gb.reg.pc+1];
                gb.reg.pc = address - 3;
                cout<< "JP " << std::hex << address << endl;
                break;
            }
            case 0xC4:
            { //   CALL NZ,a16/12
                cout<< "UNIMPLEMENTED 0xC4" << endl;
                break;
            }
            case 0xC5:
            { //   PUSH BC
                gb.reg.sp--;
                gb.mem[gb.reg.sp] = gb.reg.b;
                gb.reg.sp--;
                gb.mem[gb.reg.sp] = gb.reg.c;
                cout<< "PUSH BC" << endl;
                break;
            }
            case 0xC6:
            { //   ADD A,d8
                byte imm = gb.mem[gb.reg.pc + 1];
                short carrycheck = (short)gb.reg.a + (short)gb.mem[gb.reg.pc + 1];
                byte hc = gb.reg.c & 0xF + gb.reg.a & 0xF;
                gb.reg.a = gb.reg.a + gb.mem[gb.reg.pc + 1];
                gb.flag.c = carrycheck & 0x100;
                gb.flag.h = hc & 0x10;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                cout<< "ADD A," << +imm << endl;
                break;
            }
            case 0xC7:
            { //   RST 00H
                gb.reg.sp--;
                gb.mem[gb.reg.sp] = gb.reg.pc;
                gb.reg.pc = 0xFFFF;
                cout<< "UNIMPLEMENTED 0xC7" << endl;
                break;
            }
            case 0xC8:
            { //   RET Z/8
                cout<< "UNIMPLEMENTED 0xC8" << endl;
                break;
            }
            case 0xC9:
            { //   RET
                gb.reg.pc = ((short)gb.mem[gb.reg.sp + 1]) << 8 | (short)gb.mem[gb.reg.sp];
                gb.reg.pc--;
                gb.reg.sp += 2;
                cout << "returning to " << gb.reg.pc + 1;
                cout<< "RET" << endl;
                break;
            }
            case 0xCA:
            { //   JP Z,a16
                cout<< "UNIMPLEMENTED 0xCA" << endl;
                break;
            }
            case 0xCB:
            { //   PREFIX CB (There are 256 more "cb" ops.  Most are just bit operations.)
                cbop(gb);
                gb.reg.pc++;
                break;
            }
            case 0xCC:
            { //   CALL Z,a16
                cout<< "UNIMPLEMENTED 0xCC" << endl;
                break;
            }
            case 0xCD:
            { //   CALL a16
                //  Push address of next instruction onto stack and then
                //  jump to address nn.
                byte first = ((gb.reg.pc+3) & 0xFF00) >> 8;
                byte second = (gb.reg.pc+3) & 0xFF;
                gb.reg.sp--;
                gb.mem[gb.reg.sp] = ((gb.reg.pc+3) & 0xFF00) >> 8;
                gb.reg.sp--;
                gb.mem[gb.reg.sp] = (gb.reg.pc+3) & 0xFF;

                cout << "pushing" << hex << +first << +second << endl;

                short unsigned addr = gb.mem[gb.reg.pc + 1] | gb.mem[gb.reg.pc + 2] << 8;
                gb.reg.pc = addr - 3;
                cout<< "CALL 0x" << hex << addr << endl;
                break;
            }
            case 0xCE:
            { //   ADC A,d8
                cout<< "UNIMPLEMENTED 0xCE" << endl;
                break;
            }
            case 0xCF:
            { //   RST 08H
                gb.reg.sp--;
                gb.mem[gb.reg.sp] = gb.reg.pc;
                gb.reg.pc = 0x0007;
                cout<< "RST 08H" << endl;
                break;
            }
            case 0xD0:
            { //   RET NC
                cout<< "UNIMPLEMENTED 0xD0" << endl;
                break;
            }
            case 0xD1:
            { //   POP DE
                gb.reg.destore(((short)gb.mem[gb.reg.sp + 1]) << 8 | (short)gb.mem[gb.reg.sp]);
                gb.reg.sp += 2;
                cout<< "POP DE" << endl;
                break;
            }
            case 0xD2:
            { //   JP NC,a16
                cout<< "UNIMPLEMENTED 0xD2" << endl;
                break;
            }
            case 0xD3:
            { //
                cout<< "UNIMPLEMENTED 0xD3" << endl;
                break;
            }
            case 0xD4:
            { //   CALL NC,a16
                cout<< "UNIMPLEMENTED 0xD4" << endl;
                break;
            }
            case 0xD5:
            { //   PUSH DE
                gb.reg.sp--;
                gb.mem[gb.reg.sp] = gb.reg.d;
                gb.reg.sp--;
                gb.mem[gb.reg.sp] = gb.reg.e;
                cout<< "PUSH DE" << endl;
                break;
            }
            case 0xD6:
            { //   SUB d8
                cout<< "UNIMPLEMENTED 0xD6" << endl;
                break;
            }
            case 0xD7:
            { //   RST 10H
                gb.reg.sp--;
                gb.mem[gb.reg.sp] = gb.reg.pc;
                gb.reg.pc = 0x000F;
                cout<< "RST 10H" << endl;
                break;
            }
            case 0xD8:
            { //   RET C/8
                cout<< "UNIMPLEMENTED 0xD8" << endl;
                break;
            }
            case 0xD9:
            { //   RETI
                cout<< "UNIMPLEMENTED 0xD9" << endl;
                break;
            }
            case 0xDA:
            { //   JP C,a16
                cout<< "UNIMPLEMENTED 0xDA" << endl;
                break;
            }
            case 0xDB:
            { //
                cout<< "UNIMPLEMENTED 0xDB" << endl;
                break;
            }
            case 0xDC:
            { //   CALL C,a16
                cout<< "UNIMPLEMENTED 0xDC" << endl;
                break;
            }
            case 0xDD:
            { //
                cout<< "UNIMPLEMENTED 0xDD" << endl;
                break;
            }
            case 0xDE:
            { //   SBC A,d8
                cout<< "UNIMPLEMENTED 0xDE" << endl;
                break;
            }
            case 0xDF:
            { //   RST 18H
                gb.reg.sp--;
                gb.mem[gb.reg.sp] = gb.reg.pc;
                gb.reg.pc = 0x0017;
                cout<< "RST 18H" << endl;
                break;
            }
            case 0xE0:
            { //   LDH (a8),A
                unsigned short imm = gb.mem[gb.reg.pc + 1] + 0xFF00;
                gb.mem[imm] = gb.reg.a;
                cout<< "LD " << "0x" << hex << imm << ",A" << endl;
                break;
            }
            case 0xE1:
            { //   POP HL
                gb.reg.hlstore(((short)gb.mem[gb.reg.sp + 1]) << 8 | (short)gb.mem[gb.reg.sp]);
                gb.reg.sp += 2;
                cout<< "POP HL" << endl;
                break;
            }
            case 0xE2:
            { //   LD (C),A
                // Put A into address $FF00 + register C.
                unsigned short addr = 0xFF00 + (unsigned short)gb.reg.c;
                gb.mem[addr] = gb.reg.a;
                cout<< "LD (C),A" << endl;
                break;
            }
            case 0xE3:
            { //
                cout<< "NON-EXISTANT OPCODE" << endl;
                break;
            }
            case 0xE4:
            { //
                cout<< "NON-EXISTANT OPCODE" << endl;
                break;
            }
            case 0xE5:
            { //   PUSH HL
                gb.reg.sp--;
                gb.mem[gb.reg.sp] = gb.reg.h;
                gb.reg.sp--;
                gb.mem[gb.reg.sp] = gb.reg.l;
                cout<< "PUSH HL" << endl;
                break;
            }
            case 0xE6:
            { //   AND d8
                byte val = gb.mem[gb.reg.pc + 1];
                gb.reg.a = gb.reg.a & val;
                gb.flag.z = gb.reg.a == 0;
                gb.flag.n = 0;
                gb.flag.h = 1;
                gb.flag.c = 0;
                cout<< "AND " << hex << +val << endl;
                break;
            }
            case 0xE7:
            { //   RST 20H
                gb.reg.sp--;
                gb.mem[gb.reg.sp] = gb.reg.pc;
                gb.reg.pc = 0x001F;
                cout<< "RST 20H" << endl;
                break;
            }
            case 0xE8:
            { //   ADD SP,r8
                cout<< "UNIMPLEMENTED 0xE8" << endl;
                break;
            }
            case 0xE9:
            { //   JP (HL)
                gb.reg.sp = gb.reg.hl();
                gb.reg.sp--;
                cout<< "JP (HL)" << endl;
                break;
            }
            case 0xEA:
            { //   LD (a16),A
                unsigned short addr = (unsigned short)(((unsigned short)gb.mem[gb.reg.pc + 2]) << 8) | gb.mem[gb.reg.pc + 1];
                gb.mem[addr] = gb.reg.a;
                cout<< "LD (" << hex << addr << "),A" << endl;
                break;
            }
            case 0xEB:
            { //
                cout<< "UNIMPLEMENTED 0xEB" << endl;
                break;
            }
            case 0xEC:
            { //
                cout<< "UNIMPLEMENTED 0xEC" << endl;
                break;
            }
            case 0xED:
            { //
                cout<< "UNIMPLEMENTED 0xED" << endl;
                break;
            }
            case 0xEE:
            { //   XOR d8
                cout<< "UNIMPLEMENTED 0xEE" << endl;
                break;
            }
            case 0xEF:
            { //   RST 28H
                gb.reg.sp--;
                gb.mem[gb.reg.sp] = gb.reg.pc;
                gb.reg.pc = 0x0027;
                cout<< "RST 28h" << endl;
                break;
            }
            case 0xF0:
            { //   LDH A,(a8)
                short address = 0xFF00 + gb.mem[gb.reg.pc + 1];
                gb.reg.a = gb.mem[address];
                cout<< "LDH A, " << hex << address << endl;
                break;
            }
            case 0xF1:
            { //   POP AF
                cout<< "UNIMPLEMENTED 0xF1" << endl;
                break;
            }
            case 0xF2:
            { //   LD A,(C)
                cout<< "UNIMPLEMENTED 0xF2" << endl;
                break;
            }
            case 0xF3:
            { //   DI
                interruptEnabled = false;
                cout<< "DI" << endl;
                break;
            }
            case 0xF4:
            { //
                cout<< "UNIMPLEMENTED 0xF4" << endl;
                break;
            }
            case 0xF5:
            { //   PUSH AF
                gb.reg.sp--;
                gb.mem[gb.reg.sp] = gb.reg.a;
                gb.reg.sp--;
                gb.mem[gb.reg.sp] = gb.flag.getF();
                cout<< "PUSH AF" << endl;
                break;
            }
            case 0xF6:
            { //   OR d8
                cout<< "UNIMPLEMENTED 0xF6" << endl;
                break;
            }
            case 0xF7:
            { //   RST 30H
                gb.reg.sp--;
                gb.mem[gb.reg.sp] = gb.reg.pc;
                gb.reg.pc = 0x002F;
                cout<< "RST 30H" << endl;
                break;
            }
            case 0xF8:
            { //   LD HL,SP+r8
                cout<< "UNIMPLEMENTED 0xF8" << endl;
                break;
            }
            case 0xF9:
            { //   LD SP,HL
                cout<< "UNIMPLEMENTED 0xF9" << endl;
                break;
            }
            case 0xFA:
            { //   LD A,(a16)
                cout<< "UNIMPLEMENTED 0xFA" << endl;
                break;
            }
            case 0xFB:
            { //   EI
                interruptEnabled = true;
                cout<< "EI" << endl;
                break;
            }
            case 0xFC:
            { //
                cout<< "UNIMPLEMENTED 0xFC" << endl;
                break;
            }
            case 0xFD:
            { //
                cout<< "UNIMPLEMENTED 0xFD" << endl;
                break;
            }
            case 0xFE:
            { //   CP d8
                byte imm = gb.mem[gb.reg.pc + 1];
                byte res = gb.reg.a - imm;
                gb.flag.z = (gb.reg.a < imm);
                //cout << gb.flag.z << endl;
                gb.flag.n = true;
                gb.flag.c = (gb.reg.a < imm);
                cout<< "CP a," << hex << (int)imm << endl;
                break;
            }
            case 0xFF:
            { //   RST 38H
                gb.reg.sp--;
                gb.mem[gb.reg.sp] = gb.reg.pc;
                gb.reg.pc = 0x0037;
                cout<<  "RST 38H" << endl;
                break;
            }
            default:
                break;
            }

            //if (gb.reg.pc == 0x207){
            //   cout << "done" << endl;
            //    redraw(gb, renderer, texture, pixels);
            //}
            //redraw(gb, renderer, texture, pixels);
            // cout << gb.reg.pc << endl;
            // Increment PC by oplen
            gb.reg.pc += oplen[inst];
            if (gb.reg.pc > furthestint){
                furthestint = gb.reg.pc;
               //cout << furthestint << endl;
            }
            // Add cycle count to timer
            gb.cycles += cycles[inst];
            gb.gpucount -= cycles[inst];
            // update GPU interrupts
            setGpuState(gb, renderer, texture, pixels);
            // Fire interrupts based on cycles
            //fireInterrupts(gb);
            if (oplen[inst] == 0)
            {
                gb.reg.pc++;
            }
            //if ()
            //dumpRegisters(gb);
            //stopped = true;
        }
    }

    delete gb.mem;
    return 0;

}

int main(int argc, char **argv){
    init();
    startemu();
    close();
    return 0;
}
