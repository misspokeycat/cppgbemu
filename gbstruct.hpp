#ifndef GBSTRUCT
#define GBSTRUCT

#include <cstdlib>

typedef unsigned char byte;
    struct registers
    {
        byte a, b, c, d, e, h, l = 0;
        short pc, sp = 0;
        short bc (){return (((short)b) << 8) | c;}
        short de (){return (((short)d) << 8) | e;}
        short hl (){return (((short)h) << 8) | l;}
        void bcstore(short bc_){b = bc_ >> 8; c = bc_ & 0xFF;}
        void destore(short de_){d = de_ >> 8; e = de_ & 0xFF;}
        void hlstore(short hl_){h = hl_ >> 8; l = hl_ & 0xFF;}
    };

    struct flags
    {
        bool z, n, h, c = 0;

        // Occasionally flags need to be treated as a register (normally for CALLS and pushing flags onto the stack).
        // This deals with those ops.
        byte getF(){
            return (byte)z << 3 | (byte)n << 2 | (byte)h << 1 | (byte)c;
        }
        void setF(byte f){
            z = f >> 3 & 1;
            n = f >> 2 & 1;
            h = f >> 1 & 1;
            c = f & 1;
        }
    };

    struct GB
    {
        registers reg;
        flags flag;

        byte *mem = (byte *)std::malloc(65535);
        unsigned int cycles = 0;
        int gpucount = 0;
    };

#endif
