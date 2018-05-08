#include <iostream>
#include "gbstruct.hpp"

using namespace std;
typedef unsigned char byte;

// Helper class for "CB" opcodes (the second "instruction set")
void cbop(GB &gb)
{
    short unsigned op = gb.mem[gb.reg.pc + 1];
    switch (op)
    {
    case 0x0: //	RLC B
            gb.flag.c = gb.reg.b & 8;
            gb.reg.b = gb.reg.b << 1;
            gb.flag.z = gb.reg.b == 0;
            gb.flag.n = 0;
            gb.flag.h = 0;
        cout<< "RLC B" << endl;
        break;
    case 0x1: //	RLC C
            gb.flag.c = gb.reg.c & 8;
            gb.reg.c = gb.reg.c << 1;
            gb.flag.z = gb.reg.c == 0;
            gb.flag.n = 0;
            gb.flag.h = 0;
        cout<< "RLC C" << endl;
        break;
    case 0x2: //	RLC D
            gb.flag.c = gb.reg.d & 8;
            gb.reg.d = gb.reg.d << 1;
            gb.flag.z = gb.reg.d == 0;
            gb.flag.n = 0;
            gb.flag.h = 0;
        cout<< "RLC D" << endl;
        break;
    case 0x3: //	RLC E
        {
            gb.flag.c = gb.reg.e & 8;
            gb.reg.e = gb.reg.e << 1;
            gb.flag.z = gb.reg.e == 0;
            gb.flag.n = 0;
            gb.flag.h = 0;
            cout<< "RLC E" << endl;
            break;
        }
        break;
    case 0x4: //	RLC H
            gb.flag.c = gb.reg.h & 8;
            gb.reg.h = gb.reg.h << 1;
            gb.flag.z = gb.reg.h == 0;
            gb.flag.n = 0;
            gb.flag.h = 0;
        //cout<< "RLC H" << endl;
        break;
    case 0x5: //	RLC L
            gb.flag.c = gb.reg.l & 8;
            gb.reg.l = gb.reg.l << 1;
            gb.flag.z = gb.reg.l == 0;
            gb.flag.n = 0;
            gb.flag.h = 0;
        cout<< "RLC L" << endl;
        break;
    case 0x6: //	RLC (HL)
        {
            gb.flag.c = gb.reg.h & 8;
            short hl = ((short)((short)gb.reg.l) << 8) + (short)gb.reg.h;
            hl = hl << 1;
            gb.reg.l = (byte)(hl >> 8);
            gb.reg.h = (byte)(hl & 0xF);
            gb.flag.z = hl == 0;
            gb.flag.n = 0;
            gb.flag.h = 0;
        }
        cout<< "RLC (HL)" << endl;
        break;
    case 0x7: //	RLC A
        cout<< "UNIMP RLC A" << endl;
        break;
    case 0x8: //	RRC B
        cout<< "UNIMP RRC B" << endl;
        break;
    case 0x9: //	RRC C
        cout<< "UNIMP RRC C" << endl;
        break;
    case 0xA: //	RRC D
        cout<< "UNIMP RRC D" << endl;
        break;
    case 0xB: //	RRC E
        cout<< "UNIMP RRC E" << endl;
        break;
    case 0xC: //	RRC H
        cout<< "UNIMP RRC H" << endl;
        break;
    case 0xD: //	RRC L
        cout<< "UNIMP RRC L" << endl;
        break;
    case 0xE: //	RRC (HL)
        cout<< "UNIMP RRC (HL)" << endl;
        break;
    case 0xF: //	RRC A
        cout<< "UNIMP RRC A" << endl;
        break;
    case 0x10: //	RL B
        {
            byte flag = gb.flag.c;
            gb.flag.c = gb.reg.b >> 7;
            gb.reg.b = (gb.reg.b << 1) + flag;
            gb.flag.z = gb.reg.b == 0;
            gb.flag.n = 0;
            gb.flag.h = 0;
            cout<< "RL B" << endl;
        }
        break;
    case 0x11: //	RL C
        {
            byte flag = gb.flag.c;
            gb.flag.c = gb.reg.c >> 7;
            gb.reg.c = (gb.reg.c << 1) + flag;
            gb.flag.z = gb.reg.c == 0;
            gb.flag.n = 0;
            gb.flag.h = 0;
            cout<< "RL C" << endl;
        }
        break;
    case 0x12: //	RL D
        {
            byte flag = gb.flag.c;
            gb.flag.c = gb.reg.d >> 7;
            gb.reg.d = (gb.reg.d << 1) + flag;
            gb.flag.z = gb.reg.d == 0;
            gb.flag.n = 0;
            gb.flag.h = 0;
        cout<< "RL D" << endl;
        }
        break;
    case 0x13: //	RL E
        {
            byte flag = gb.flag.c;
            gb.flag.c = gb.reg.e >> 7;
            gb.reg.e = (gb.reg.e << 1) + flag;
            gb.flag.z = gb.reg.e == 0;
            gb.flag.n = 0;
            gb.flag.h = 0;
        }
        cout<< "RL E" << endl;
        break;
    case 0x14: //	RL H
        {
            byte flag = gb.flag.c;
            gb.flag.c = gb.reg.h >> 7;
            gb.reg.h = (gb.reg.h << 1) + flag;
            gb.flag.z = gb.reg.h == 0;
            gb.flag.n = 0;
            gb.flag.h = 0;
        }
        cout<< "RL H" << endl;
        break;
    case 0x15: //	RL L
    {
            byte flag = gb.flag.c;
            gb.flag.c = gb.reg.l >> 7;
            gb.reg.l = (gb.reg.l << 1) + flag;
            gb.flag.z = gb.reg.l == 0;
            gb.flag.n = 0;
            gb.flag.h = 0;
    }
        cout<< "RL L" << endl;
        break;
    case 0x16: //	RL (HL)
        cout<< "UNIMP RL (HL)" << endl;
        break;
    case 0x17: //	RL A
        {
            byte flag = gb.flag.c;
            gb.flag.c = gb.reg.a >> 7;
            gb.reg.a = (gb.reg.a << 1) + flag;
            gb.flag.z = gb.reg.a == 0;
            gb.flag.n = 0;
            gb.flag.h = 0;
        }
        cout<< "UNIMP RL A" << endl;
        break;
    case 0x18: //	RR B
        {
            byte flag = gb.flag.c;
            gb.flag.c = gb.reg.b & 1;
            gb.reg.b = (gb.reg.b >> 1) + (flag << 7);
            gb.flag.z = gb.reg.b == 0;
            gb.flag.n = 0;
            gb.flag.h = 0;
        }
        cout<< "RR B" << endl;
        break;
    case 0x19: //	RR C
        {
            byte flag = gb.flag.c;
            gb.flag.c = gb.reg.c & 1;
            gb.reg.c = (gb.reg.c >> 1) + (flag << 7);
            gb.flag.z = gb.reg.c == 0;
            gb.flag.n = 0;
            gb.flag.h = 0;
        }
        cout<< "RR C" << endl;
        break;
    case 0x1A: //	RR D
        {
            byte flag = gb.flag.c;
            gb.flag.c = gb.reg.d & 1;
            gb.reg.d = (gb.reg.d >> 1) + (flag << 7);
            gb.flag.z = gb.reg.d == 0;
            gb.flag.n = 0;
            gb.flag.h = 0;
        }
        cout<< "RR D" << endl;
        break;
    case 0x1B: //	RR E
        {
            byte flag = gb.flag.c;
            gb.flag.c = gb.reg.e & 1;
            gb.reg.e = (gb.reg.e >> 1) + (flag << 7);
            gb.flag.z = gb.reg.e == 0;
            gb.flag.n = 0;
            gb.flag.h = 0;
        }
        cout<< "RR E" << endl;
        break;
    case 0x1C: //	RR H
        {
            byte flag = gb.flag.c;
            gb.flag.c = gb.reg.h & 1;
            gb.reg.h = (gb.reg.h >> 1) + (flag << 7);
            gb.flag.z = gb.reg.h == 0;
            gb.flag.n = 0;
            gb.flag.h = 0;
        }
        cout<< "RR H" << endl;
        break;
    case 0x1D: //	RR L
        {
            byte flag = gb.flag.c;
            gb.flag.c = gb.reg.l & 1;
            gb.reg.l = (gb.reg.l >> 1) + (flag << 7);
            gb.flag.z = gb.reg.l == 0;
            gb.flag.n = 0;
            gb.flag.h = 0;
        }
        cout<< "RR L" << endl;
        break;
    case 0x1E: //	RR (HL)
        cout<< "UNIMP RR (HL)" << endl;
        break;
    case 0x1F: //	RR A
        {
            byte flag = gb.flag.c;
            gb.flag.c = gb.reg.a & 1;
            gb.reg.a = (gb.reg.a >> 1) + (flag << 7);
            gb.flag.z = gb.reg.a == 0;
            gb.flag.n = 0;
            gb.flag.h = 0;
        }
        cout<< "RR A" << endl;
        break;
    case 0x20: //	SLA B
        gb.flag.c = gb.reg.b >> 7;
        gb.reg.b = gb.reg.b << 1;
        gb.flag.z = gb.reg.b;
        gb.flag.n = 0;
        gb.flag.h = 0;
        cout<< "SLA B" << endl;
        break;
    case 0x21: //	SLA C
        gb.flag.c = gb.reg.c >> 7;
        gb.reg.c = gb.reg.c << 1;
        gb.flag.z = gb.reg.c;
        gb.flag.n = 0;
        gb.flag.h = 0;
        cout<< "SLA C" << endl;
        break;
    case 0x22: //	SLA D
        gb.flag.c = gb.reg.d >> 7;
        gb.reg.d = gb.reg.d << 1;
        gb.flag.z = gb.reg.d;
        gb.flag.n = 0;
        gb.flag.h = 0;
        cout<< "SLA D" << endl;
        break;
    case 0x23: //	SLA E
        gb.flag.c = gb.reg.e >> 7;
        gb.reg.e = gb.reg.e << 1;
        gb.flag.z = gb.reg.e;
        gb.flag.n = 0;
        gb.flag.h = 0;
        cout<< "SLA e" << endl;
        break;
    case 0x24: //	SLA H
        gb.flag.c = gb.reg.h >> 7;
        gb.reg.h = gb.reg.h << 1;
        gb.flag.z = gb.reg.h;
        gb.flag.n = 0;
        gb.flag.h = 0;
        cout<< "SLA H" << endl;
        break;
    case 0x25: //	SLA L
        gb.flag.c = gb.reg.l >> 7;
        gb.reg.l = gb.reg.l << 1;
        gb.flag.z = gb.reg.l;
        gb.flag.n = 0;
        gb.flag.h = 0;
        cout<< "SLA L" << endl;
        break;
    case 0x26: //	SLA (HL)
        cout<< "UNIMP SLA (HL)" << endl;
        break;
    case 0x27: //	SLA A
        gb.flag.c = gb.reg.a >> 7;
        gb.reg.a = gb.reg.a << 1;
        gb.flag.z = gb.reg.a;
        gb.flag.n = 0;
        gb.flag.h = 0;
        cout<< "SLA A" << endl;
        break;
    case 0x28: //	SRA B
        cout<< "UNIMP SRA B" << endl;
        break;
    case 0x29: //	SRA C
        cout<< "UNIMP SRA C" << endl;
        break;
    case 0x2A: //	SRA D
        cout<< "UNIMP SRA D" << endl;
        break;
    case 0x2B: //	SRA E
        cout<< "UNIMP SRA E" << endl;
        break;
    case 0x2C: //	SRA H
        cout<< "UNIMP SRA H" << endl;
        break;
    case 0x2D: //	SRA L
        cout<< "UNIMP SRA L" << endl;
        break;
    case 0x2E: //	SRA (HL)
        cout<< "UNIMP SRA (HL)" << endl;
        break;
    case 0x2F: //	SRA A
        cout<< "UNIMP SRA A" << endl;
        break;
    case 0x30: //	SWAP B
        {
        bool f = gb.reg.b & 1;
        bool l = gb.reg.b >> 7;
        gb.reg.b &= f*0x8;
        gb.reg.b &= l;
        gb.flag.z = gb.reg.a == 0;
        gb.flag.n = 0;
        gb.flag.h = 0;
        gb.flag.c = 0;
        //cout<< "SWAP B" << endl;
        }
        break;
    case 0x31: //	SWAP C
                {
        bool f = gb.reg.c & 1;
        bool l = gb.reg.c >> 7;
        gb.reg.c &= f*0x8;
        gb.reg.c &= l;
        gb.flag.z = gb.reg.a == 0;
        gb.flag.n = 0;
        gb.flag.h = 0;
        gb.flag.c = 0;
        //cout<< "SWAP C" << endl;
                }
        break;
    case 0x32: //	SWAP D
                {
        bool f = gb.reg.d & 1;
        bool l = gb.reg.d >> 7;
        gb.reg.d &= f*0x8;
        gb.reg.d &= l;
        gb.flag.z = gb.reg.a == 0;
        gb.flag.n = 0;
        gb.flag.h = 0;
        gb.flag.c = 0;
        //cout<< "SWAP D" << endl;
                }
        break;
    case 0x33: //	SWAP E
                {
        bool f = gb.reg.e & 1;
        bool l = gb.reg.e >> 7;
        gb.reg.e &= f*0x8;
        gb.reg.e &= l;
        gb.flag.z = gb.reg.a == 0;
        gb.flag.n = 0;
        gb.flag.h = 0;
        gb.flag.c = 0;
        //cout<< "SWAP E" << endl;
                }
        break;
    case 0x34: //	SWAP H
                {
        bool f = gb.reg.h & 1;
        bool l = gb.reg.h >> 7;
        gb.reg.h &= f*0x8;
        gb.reg.h &= l;
        gb.flag.z = gb.reg.a == 0;
        gb.flag.n = 0;
        gb.flag.h = 0;
        gb.flag.c = 0;
        //cout<< "SWAP H" << endl;
                }
        break;
    case 0x35: //	SWAP L
                {
        bool f = gb.reg.l & 1;
        bool l = gb.reg.l >> 7;
        gb.reg.l &= f*0x8;
        gb.reg.l &= l;
        gb.flag.z = gb.reg.a == 0;
        gb.flag.n = 0;
        gb.flag.h = 0;
        gb.flag.c = 0;
        //cout<< "SWAP L" << endl;
                }
        break;
    case 0x36: //	SWAP (HL)
        cout<< "UNIMP SWAP (HL)" << endl;
        break;
    case 0x37: //	SWAP A
                {
        bool f = gb.reg.a & 1;
        bool l = gb.reg.a >> 7;
        gb.reg.a &= f*0x8;
        gb.reg.a &= l;
        gb.flag.z = gb.reg.a == 0;
        gb.flag.n = 0;
        gb.flag.h = 0;
        gb.flag.c = 0;
        //cout<< "SWAP A" << endl;
                }
        break;
    case 0x38: //	SRL B
        cout<< "UNIMP SRL B" << endl;
        break;
    case 0x39: //	SRL C
        cout<< "UNIMP SRL C" << endl;
        break;
    case 0x3A: //	SRL D
        cout<< "UNIMP SRL D" << endl;
        break;
    case 0x3B: //	SRL E
        cout<< "UNIMP SRL E" << endl;
        break;
    case 0x3C: //	SRL H
        cout<< "UNIMP SRL H" << endl;
        break;
    case 0x3D: //	SRL L
        cout<< "UNIMP SRL L" << endl;
        break;
    case 0x3E: //	SRL (HL)
        cout<< "UNIMP SRL (HL)" << endl;
        break;
    case 0x3F: //	SRL A
        cout<< "UNIMP SRL A" << endl;
        break;
    case 0x40: //	BIT 0,B
        gb.flag.z = (gb.reg.b & 0x1);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "BIT 0,B" << endl;
        break;
    case 0x41: //	BIT 0,C
        gb.flag.z = (gb.reg.c & 0x1);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "BIT 0,C" << endl;
        break;
    case 0x42: //	BIT 0,D
        gb.flag.z = (gb.reg.d & 0x1);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "BIT 0,D" << endl;
        break;
    case 0x43: //	BIT 0,E
        gb.flag.z = (gb.reg.e & 0x1);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "BIT 0,E" << endl;
        break;
    case 0x44: //	BIT 0,H
        gb.flag.z = (gb.reg.h & 0x1);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "BIT 0,H" << endl;
        break;
    case 0x45: //	BIT 0,L
        gb.flag.z = (gb.reg.l & 0x1);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "BIT 0,L" << endl;
        break;
    case 0x46: //	BIT 0,(HL)
        cout<< "UNIMP BIT 0,(HL)" << endl;
        break;
    case 0x47: //	BIT 0,A
        gb.flag.z = (gb.reg.a & 0x1);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "BIT 0,A" << endl;
        break;
    case 0x48: //	BIT 1,B
        gb.flag.z = (gb.reg.b & 0x2);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "BIT 1,B" << endl;
        break;
    case 0x49: //	BIT 1,C
        gb.flag.z = (gb.reg.c & 0x2);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 1,C" << endl;
        break;
    case 0x4A: //	BIT 1,D
        gb.flag.z = (gb.reg.d & 0x2);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 1,D" << endl;
        break;
    case 0x4B: //	BIT 1,E
        gb.flag.z = (gb.reg.e & 0x2);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 1,E" << endl;
        break;
    case 0x4C: //	BIT 1,H
        gb.flag.z = (gb.reg.h & 0x2);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 1,H" << endl;
        break;
    case 0x4D: //	BIT 1,L
        gb.flag.z = (gb.reg.l & 0x2);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 1,L" << endl;
        break;
    case 0x4E: //	BIT 1,(HL)
        cout<< "UNIMP BIT 1,(HL)" << endl;
        break;
    case 0x4F: //	BIT 1,A
        gb.flag.z = (gb.reg.a & 0x2);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 1,A" << endl;
        break;
    case 0x50: //	BIT 2,B
        gb.flag.z = (gb.reg.b & 0x4);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 2,B" << endl;
        break;
    case 0x51: //	BIT 2,C
        gb.flag.z = (gb.reg.c & 0x4);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 2,C" << endl;
        break;
    case 0x52: //	BIT 2,D
        gb.flag.z = (gb.reg.d & 0x4);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 2,D" << endl;
        break;
    case 0x53: //	BIT 2,E
        gb.flag.z = (gb.reg.e & 0x4);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 2,E" << endl;
        break;
    case 0x54: //	BIT 2,H
        gb.flag.z = (gb.reg.h & 0x4);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 2,H" << endl;
        break;
    case 0x55: //	BIT 2,L
        gb.flag.z = (gb.reg.l & 0x4);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 2,L" << endl;
        break;
    case 0x56: //	BIT 2,(HL)
        cout<< "UNIMP BIT 2,(HL)" << endl;
        break;
    case 0x57: //	BIT 2,A
        gb.flag.z = (gb.reg.a & 0x4);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 2,A" << endl;
        break;
    case 0x58: //	BIT 3,B
        gb.flag.z = (gb.reg.b & 0x8);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 3,B" << endl;
        break;
    case 0x59: //	BIT 3,C
        gb.flag.z = (gb.reg.c & 0x8);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 3,C" << endl;
        break;
    case 0x5A: //	BIT 3,D
        gb.flag.z = (gb.reg.d & 0x8);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 3,D" << endl;
        break;
    case 0x5B: //	BIT 3,E
        gb.flag.z = (gb.reg.e & 0x8);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 3,E" << endl;
        break;
    case 0x5C: //	BIT 3,H
        gb.flag.z = (gb.reg.h & 0x8);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 3,H" << endl;
        break;
    case 0x5D: //	BIT 3,L
        gb.flag.z = (gb.reg.l & 0x8);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 3,L" << endl;
        break;
    case 0x5E: //	BIT 3,(HL)
        cout<< "UNIMP BIT 3,(HL)" << endl;
        break;
    case 0x5F: //	BIT 3,A
        gb.flag.z = (gb.reg.a & 0x8);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 3,A" << endl;
        break;
    case 0x60: //	BIT 4,B
        gb.flag.z = (gb.reg.b & 0x10);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 4,B" << endl;
        break;
    case 0x61: //	BIT 4,C
        gb.flag.z = (gb.reg.c & 0x10);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 4,C" << endl;
        break;
    case 0x62: //	BIT 4,D
        gb.flag.z = (gb.reg.d & 0x10);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 4,D" << endl;
        break;
    case 0x63: //	BIT 4,E
        gb.flag.z = (gb.reg.e & 0x10);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 4,E" << endl;
        break;
    case 0x64: //	BIT 4,H
        gb.flag.z = (gb.reg.h & 0x10);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 4,H" << endl;
        break;
    case 0x65: //	BIT 4,L
        gb.flag.z = (gb.reg.l & 0x10);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 4,L" << endl;
        break;
    case 0x66: //	BIT 4,(HL)
        cout<< "UNIMP BIT 4,(HL)" << endl;
        break;
    case 0x67: //	BIT 4,A
        gb.flag.z = (gb.reg.a & 0x10);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 4,A" << endl;
        break;
    case 0x68: //	BIT 5,B
        gb.flag.z = (gb.reg.b & 0x20);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 5,B" << endl;
        break;
    case 0x69: //	BIT 5,C
        gb.flag.z = (gb.reg.c & 0x20);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 5,C" << endl;
        break;
    case 0x6A: //	BIT 5,D
        gb.flag.z = (gb.reg.d & 0x20);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 5,D" << endl;
        break;
    case 0x6B: //	BIT 5,E
        gb.flag.z = (gb.reg.e & 0x20);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 5,E" << endl;
        break;
    case 0x6C: //	BIT 5,H
        gb.flag.z = (gb.reg.h & 0x20);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 5,H" << endl;
        break;
    case 0x6D: //	BIT 5,L
        gb.flag.z = (gb.reg.l & 0x20);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 5,L" << endl;
        break;
    case 0x6E: //	BIT 5,(HL)
        cout<< "UNIMP BIT 5,(HL)" << endl;
        break;
    case 0x6F: //	BIT 5,A
        gb.flag.z = (gb.reg.a & 0x20);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 5,A" << endl;
        break;
    case 0x70: //	BIT 6,B
        gb.flag.z = (gb.reg.b & 0x40);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 6,B" << endl;
        break;
    case 0x71: //	BIT 6,C
        gb.flag.z = (gb.reg.c & 0x40);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 6,C" << endl;
        break;
    case 0x72: //	BIT 6,D
        gb.flag.z = (gb.reg.d & 0x40);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 6,D" << endl;
        break;
    case 0x73: //	BIT 6,E
        gb.flag.z = (gb.reg.e & 0x40);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 6,E" << endl;
        break;
    case 0x74: //	BIT 6,H
        gb.flag.z = (gb.reg.h & 0x40);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 6,H" << endl;
        break;
    case 0x75: //	BIT 6,L
        gb.flag.z = (gb.reg.l & 0x40);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 6,L" << endl;
        break;
    case 0x76: //	BIT 6,(HL)
        cout<< "UNIMP BIT 6,(HL)" << endl;
        break;
    case 0x77: //	BIT 6,A
        gb.flag.z = (gb.reg.a & 0x40);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 6,A" << endl;
        break;
    case 0x78: //	BIT 7,B
        gb.flag.z = (gb.reg.b & 0x80);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 7,B" << endl;
        break;
    case 0x79: //	BIT 7,C
        gb.flag.z = (gb.reg.c & 0x80);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 7,C" << endl;
        break;
    case 0x7A: //	BIT 7,D
        gb.flag.z = (gb.reg.d & 0x80);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 7,D" << endl;
        break;
    case 0x7B: //	BIT 7,E
        gb.flag.z = (gb.reg.e & 0x80);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 7,E" << endl;
        break;
    case 0x7C: //	BIT 7,H
        gb.flag.z = (gb.reg.h & 0x80);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "BIT 7,H" << endl;
        break;
    case 0x7D: //	BIT 7,L
        gb.flag.z = (gb.reg.l & 0x80);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 7,L" << endl;
        break;
    case 0x7E: //	BIT 7,(HL)
        cout<< "UNIMP BIT 7,(HL)" << endl;
        break;
    case 0x7F: //	BIT 7,A
        gb.flag.z = (gb.reg.a & 0x80);
        gb.flag.n = 0;
        gb.flag.h = 1;
        cout<< "UNIMP BIT 7,A" << endl;
        break;
    case 0x80: //	RES 0,B
        gb.reg.b &= 0xFE;
        cout<< "UNIMP RES 0,B" << endl;
        break;
    case 0x81: //	RES 0,C
        gb.reg.c &= 0xFE;
        cout<< "UNIMP RES 0,C" << endl;
        break;
    case 0x82: //	RES 0,D
        gb.reg.d &= 0xFE;
        cout<< "UNIMP RES 0,D" << endl;
        break;
    case 0x83: //	RES 0,E
        gb.reg.e &= 0xFE;
        cout<< "UNIMP RES 0,E" << endl;
        break;
    case 0x84: //	RES 0,H
        gb.reg.h &= 0xFE;
        cout<< "UNIMP RES 0,H" << endl;
        break;
    case 0x85: //	RES 0,L
        gb.reg.l &= 0xFE;
        cout<< "UNIMP RES 0,L" << endl;
        break;
    case 0x86: //	RES 0,(HL)
        cout<< "UNIMP RES 0,(HL)" << endl;
        break;
    case 0x87: //	RES 0,A
        gb.reg.a &= 0xFE;
        cout<< "UNIMP RES 0,A" << endl;
        break;
    case 0x88: //	RES 1,B
        gb.reg.b &= 0xFD;
        cout<< "UNIMP RES 1,B" << endl;
        break;
    case 0x89: //	RES 1,C
        gb.reg.c &= 0xFD;
        cout<< "UNIMP RES 1,C" << endl;
        break;
    case 0x8A: //	RES 1,D
        gb.reg.d &= 0xFD;
        cout<< "UNIMP RES 1,D" << endl;
        break;
    case 0x8B: //	RES 1,E
        gb.reg.e &= 0xFD;
        cout<< "UNIMP RES 1,E" << endl;
        break;
    case 0x8C: //	RES 1,H
        gb.reg.h &= 0xFD;
        cout<< "UNIMP RES 1,H" << endl;
        break;
    case 0x8D: //	RES 1,L
        gb.reg.l &= 0xFD;
        cout<< "UNIMP RES 1,L" << endl;
        break;
    case 0x8E: //	RES 1,(HL)
        cout<< "UNIMP RES 1,(HL)" << endl;
        break;
    case 0x8F: //	RES 1,A
        gb.reg.a &= 0xFD;
        cout<< "UNIMP RES 1,A" << endl;
        break;
    case 0x90: //	RES 2,B
        gb.reg.b &= 0xFB;
        cout<< "UNIMP RES 2,B" << endl;
        break;
    case 0x91: //	RES 2,C
        gb.reg.c &= 0xFB;
        cout<< "UNIMP RES 2,C" << endl;
        break;
    case 0x92: //	RES 2,D
        gb.reg.d &= 0xFB;
        cout<< "UNIMP RES 2,D" << endl;
        break;
    case 0x93: //	RES 2,E
        gb.reg.e &= 0xFB;
        cout<< "UNIMP RES 2,E" << endl;
        break;
    case 0x94: //	RES 2,H
        gb.reg.h &= 0xFB;
        cout<< "UNIMP RES 2,H" << endl;
        break;
    case 0x95: //	RES 2,L
        gb.reg.l &= 0xFB;
        cout<< "UNIMP RES 2,L" << endl;
        break;
    case 0x96: //	RES 2,(HL)
        cout<< "UNIMP RES 2,(HL)" << endl;
        break;
    case 0x97: //	RES 2,A
        gb.reg.a &= 0xFB;
        cout<< "UNIMP RES 2,A" << endl;
        break;
    case 0x98: //	RES 3,B
        gb.reg.b &= 0xF7;
        cout<< "UNIMP RES 3,B" << endl;
        break;
    case 0x99: //	RES 3,C
        gb.reg.c &= 0xF7;
        cout<< "UNIMP RES 3,C" << endl;
        break;
    case 0x9A: //	RES 3,D
        gb.reg.d &= 0xF7;
        cout<< "UNIMP RES 3,D" << endl;
        break;
    case 0x9B: //	RES 3,E
        gb.reg.e &= 0xF7;
        cout<< "UNIMP RES 3,E" << endl;
        break;
    case 0x9C: //	RES 3,H
        gb.reg.h &= 0xF7;
        cout<< "UNIMP RES 3,H" << endl;
        break;
    case 0x9D: //	RES 3,L
        gb.reg.l &= 0xF7;
        cout<< "UNIMP RES 3,L" << endl;
        break;
    case 0x9E: //	RES 3,(HL)
        cout<< "UNIMP RES 3,(HL)" << endl;
        break;
    case 0x9F: //	RES 3,A
        gb.reg.a &= 0xF7;
        cout<< "UNIMP RES 3,A" << endl;
        break;
    case 0xA0: //	RES 4,B
        gb.reg.b &= 0xEF;
        cout<< "UNIMP RES 4,B" << endl;
        break;
    case 0xA1: //	RES 4,C
        gb.reg.c &= 0xEF;
        cout<< "UNIMP RES 4,C" << endl;
        break;
    case 0xA2: //	RES 4,D
        gb.reg.d &= 0xEF;
        cout<< "UNIMP RES 4,D" << endl;
        break;
    case 0xA3: //	RES 4,E
        gb.reg.e &= 0xEF;
        cout<< "UNIMP RES 4,E" << endl;
        break;
    case 0xA4: //	RES 4,H
        gb.reg.h &= 0xEF;
        cout<< "UNIMP RES 4,H" << endl;
        break;
    case 0xA5: //	RES 4,L
        gb.reg.l &= 0xEF;
        cout<< "UNIMP RES 4,L" << endl;
        break;
    case 0xA6: //	RES 4,(HL)
        cout<< "UNIMP RES 4,(HL)" << endl;
        break;
    case 0xA7: //	RES 4,A
        gb.reg.a &= 0xEF;
        cout<< "UNIMP RES 4,A" << endl;
        break;
    case 0xA8: //	RES 5,B
        gb.reg.b &= 0xDF;
        cout<< "UNIMP RES 5,B" << endl;
        break;
    case 0xA9: //	RES 5,C
        gb.reg.c &= 0xDF;
        cout<< "UNIMP RES 5,C" << endl;
        break;
    case 0xAA: //	RES 5,D
        gb.reg.d &= 0xDF;
        cout<< "UNIMP RES 5,D" << endl;
        break;
    case 0xAB: //	RES 5,E
        gb.reg.e &= 0xDF;
        cout<< "UNIMP RES 5,E" << endl;
        break;
    case 0xAC: //	RES 5,H
        gb.reg.h &= 0xDF;
        cout<< "UNIMP RES 5,H" << endl;
        break;
    case 0xAD: //	RES 5,L
        gb.reg.l &= 0xDF;
        cout<< "UNIMP RES 5,L" << endl;
        break;
    case 0xAE: //	RES 5,(HL)
        cout<< "UNIMP RES 5,(HL)" << endl;
        break;
    case 0xAF: //	RES 5,A
        gb.reg.a &= 0xDF;
        cout<< "UNIMP RES 5,A" << endl;
        break;
    case 0xB0: //	RES 6,B
        gb.reg.b &= 0xBF;
        cout<< "UNIMP RES 6,B" << endl;
        break;
    case 0xB1: //	RES 6,C
        gb.reg.c &= 0xBF;
        cout<< "UNIMP RES 6,C" << endl;
        break;
    case 0xB2: //	RES 6,D
        gb.reg.d &= 0xBF;
        cout<< "UNIMP RES 6,D" << endl;
        break;
    case 0xB3: //	RES 6,E
        gb.reg.e &= 0xBF;
        cout<< "UNIMP RES 6,E" << endl;
        break;
    case 0xB4: //	RES 6,H
        gb.reg.h &= 0xBF;
        cout<< "UNIMP RES 6,H" << endl;
        break;
    case 0xB5: //	RES 6,L
        gb.reg.l &= 0xBF;
        cout<< "UNIMP RES 6,L" << endl;
        break;
    case 0xB6: //	RES 6,(HL)
        cout<< "UNIMP RES 6,(HL)" << endl;
        break;
    case 0xB7: //	RES 6,A
        gb.reg.a &= 0xBF;
        cout<< "UNIMP RES 6,A" << endl;
        break;
    case 0xB8: //	RES 7,B
        gb.reg.b &= 0x7F;
        cout<< "UNIMP RES 7,B" << endl;
        break;
    case 0xB9: //	RES 7,C
        gb.reg.c &= 0x7F;
        cout<< "UNIMP RES 7,C" << endl;
        break;
    case 0xBA: //	RES 7,D
        gb.reg.d &= 0x7F;
        cout<< "UNIMP RES 7,D" << endl;
        break;
    case 0xBB: //	RES 7,E
        gb.reg.e &= 0x7F;
        cout<< "UNIMP RES 7,E" << endl;
        break;
    case 0xBC: //	RES 7,H
        gb.reg.h &= 0x7F;
        cout<< "UNIMP RES 7,H" << endl;
        break;
    case 0xBD: //	RES 7,L
        gb.reg.l &= 0x7F;
        cout<< "UNIMP RES 7,L" << endl;
        break;
    case 0xBE: //	RES 7,(HL)
        cout<< "UNIMP RES 7,(HL)" << endl;
        break;
    case 0xBF: //	RES 7,A
        gb.reg.a &= 0x7F;
        cout<< "UNIMP RES 7,A" << endl;
        break;
    case 0xC0: //	SET 0,B
        gb.reg.b |= 0x01;
        cout<< "UNIMP SET 0,B" << endl;
        break;
    case 0xC1: //	SET 0,C
        gb.reg.c |= 0x01;
        cout<< "UNIMP SET 0,C" << endl;
        break;
    case 0xC2: //	SET 0,D
        gb.reg.d |= 0x01;
        cout<< "UNIMP SET 0,D" << endl;
        break;
    case 0xC3: //	SET 0,E
        gb.reg.e |= 0x01;
        cout<< "UNIMP SET 0,E" << endl;
        break;
    case 0xC4: //	SET 0,H
        gb.reg.h |= 0x01;
        cout<< "UNIMP SET 0,H" << endl;
        break;
    case 0xC5: //	SET 0,L
        gb.reg.l |= 0x01;
        cout<< "UNIMP SET 0,L" << endl;
        break;
    case 0xC6: //	SET 0,(HL)
        cout<< "UNIMP SET 0,(HL)" << endl;
        break;
    case 0xC7: //	SET 0,A
        gb.reg.a |= 0x01;
        cout<< "UNIMP SET 0,A" << endl;
        break;
    case 0xC8: //	SET 1,B
        gb.reg.b |= 0x02;
        cout<< "UNIMP SET 1,B" << endl;
        break;
    case 0xC9: //	SET 1,C
        gb.reg.c |= 0x02;
        cout<< "UNIMP SET 1,C" << endl;
        break;
    case 0xCA: //	SET 1,D
        gb.reg.d |= 0x02;
        cout<< "UNIMP SET 1,D" << endl;
        break;
    case 0xCB: //	SET 1,E
        gb.reg.e |= 0x02;
        cout<< "UNIMP SET 1,E" << endl;
        break;
    case 0xCC: //	SET 1,H
        gb.reg.h |= 0x02;
        cout<< "UNIMP SET 1,H" << endl;
        break;
    case 0xCD: //	SET 1,L
        gb.reg.l |= 0x02;
        cout<< "UNIMP SET 1,L" << endl;
        break;
    case 0xCE: //	SET 1,(HL)
        cout<< "UNIMP SET 1,(HL)" << endl;
        break;
    case 0xCF: //	SET 1,A
        gb.reg.a |= 0x02;
        cout<< "UNIMP SET 1,A" << endl;
        break;
    case 0xD0: //	SET 2,B
        gb.reg.b |= 0x04;
        cout<< "UNIMP SET 2,B" << endl;
        break;
    case 0xD1: //	SET 2,C
        gb.reg.c |= 0x04;
        cout<< "UNIMP SET 2,C" << endl;
        break;
    case 0xD2: //	SET 2,D
        gb.reg.d |= 0x04;
        cout<< "UNIMP SET 2,D" << endl;
        break;
    case 0xD3: //	SET 2,E
        gb.reg.e |= 0x04;
        cout<< "UNIMP SET 2,E" << endl;
        break;
    case 0xD4: //	SET 2,H
        gb.reg.h |= 0x04;
        cout<< "UNIMP SET 2,H" << endl;
        break;
    case 0xD5: //	SET 2,L
        gb.reg.l |= 0x04;
        cout<< "UNIMP SET 2,L" << endl;
        break;
    case 0xD6: //	SET 2,(HL)
        cout<< "UNIMP SET 2,(HL)" << endl;
        break;
    case 0xD7: //	SET 2,A
        gb.reg.a |= 0x04;
        cout<< "UNIMP SET 2,A" << endl;
        break;
    case 0xD8: //	SET 3,B
        gb.reg.b |= 0x08;
        cout<< "UNIMP SET 3,B" << endl;
        break;
    case 0xD9: //	SET 3,C
        gb.reg.c |= 0x08;
        cout<< "UNIMP SET 3,C" << endl;
        break;
    case 0xDA: //	SET 3,D
        gb.reg.d |= 0x08;
        cout<< "UNIMP SET 3,D" << endl;
        break;
    case 0xDB: //	SET 3,E
        gb.reg.e |= 0x08;
        cout<< "UNIMP SET 3,E" << endl;
        break;
    case 0xDC: //	SET 3,H
        gb.reg.h |= 0x08;
        cout<< "UNIMP SET 3,H" << endl;
        break;
    case 0xDD: //	SET 3,L
        gb.reg.l |= 0x08;
        cout<< "UNIMP SET 3,L" << endl;
        break;
    case 0xDE: //	SET 3,(HL)
        cout<< "UNIMP SET 3,(HL)" << endl;
        break;
    case 0xDF: //	SET 3,A
        gb.reg.a |= 0x08;
        cout<< "UNIMP SET 3,A" << endl;
        break;
    case 0xE0: //	SET 4,B
        gb.reg.b |= 0x10;
        cout<< "UNIMP SET 4,B" << endl;
        break;
    case 0xE1: //	SET 4,C
        gb.reg.c |= 0x10;
        cout<< "UNIMP SET 4,C" << endl;
        break;
    case 0xE2: //	SET 4,D
        gb.reg.d |= 0x10;
        cout<< "UNIMP SET 4,D" << endl;
        break;
    case 0xE3: //	SET 4,E
        gb.reg.e |= 0x10;
        cout<< "UNIMP SET 4,E" << endl;
        break;
    case 0xE4: //	SET 4,H
        gb.reg.h |= 0x10;
        cout<< "UNIMP SET 4,H" << endl;
        break;
    case 0xE5: //	SET 4,L
        gb.reg.l |= 0x10;
        cout<< "UNIMP SET 4,L" << endl;
        break;
    case 0xE6: //	SET 4,(HL)
        cout<< "UNIMP SET 4,(HL)" << endl;
        break;
    case 0xE7: //	SET 4,A
        gb.reg.a |= 0x10;
        cout<< "UNIMP SET 4,A" << endl;
        break;
    case 0xE8: //	SET 5,B
        gb.reg.b |= 0x20;
        cout<< "UNIMP SET 5,B" << endl;
        break;
    case 0xE9: //	SET 5,C
        gb.reg.c |= 0x20;
        cout<< "UNIMP SET 5,C" << endl;
        break;
    case 0xEA: //	SET 5,D
        gb.reg.d |= 0x20;
        cout<< "UNIMP SET 5,D" << endl;
        break;
    case 0xEB: //	SET 5,E
        gb.reg.e |= 0x20;
        cout<< "UNIMP SET 5,E" << endl;
        break;
    case 0xEC: //	SET 5,H
        gb.reg.h |= 0x20;
        cout<< "UNIMP SET 5,H" << endl;
        break;
    case 0xED: //	SET 5,L
        gb.reg.l |= 0x20;
        cout<< "UNIMP SET 5,L" << endl;
        break;
    case 0xEE: //	SET 5,(HL)
        cout<< "UNIMP SET 5,(HL)" << endl;
        break;
    case 0xEF: //	SET 5,A
        gb.reg.a |= 0x20;
        cout<< "UNIMP SET 5,A" << endl;
        break;
    case 0xF0: //	SET 6,B
        gb.reg.b |= 0x40;
        cout<< "UNIMP SET 6,B" << endl;
        break;
    case 0xF1: //	SET 6,C
        gb.reg.c |= 0x40;
        cout<< "UNIMP SET 6,C" << endl;
        break;
    case 0xF2: //	SET 6,D
        gb.reg.d |= 0x40;
        cout<< "UNIMP SET 6,D" << endl;
        break;
    case 0xF3: //	SET 6,E
        gb.reg.e |= 0x40;
        cout<< "UNIMP SET 6,E" << endl;
        break;
    case 0xF4: //	SET 6,H
        gb.reg.h |= 0x40;
        cout<< "UNIMP SET 6,H" << endl;
        break;
    case 0xF5: //	SET 6,L
        gb.reg.l |= 0x40;
        cout<< "UNIMP SET 6,L" << endl;
        break;
    case 0xF6: //	SET 6,(HL)
        cout<< "UNIMP SET 6,(HL)" << endl;
        break;
    case 0xF7: //	SET 6,A
        gb.reg.a |= 0x40;
        cout<< "UNIMP SET 6,A" << endl;
        break;
    case 0xF8: //	SET 7,B
        gb.reg.b |= 0x80;
        cout<< "UNIMP SET 7,B" << endl;
        break;
    case 0xF9: //	SET 7,C
        gb.reg.c |= 0x80;
        cout<< "UNIMP SET 7,C" << endl;
        break;
    case 0xFA: //	SET 7,D
        gb.reg.d |= 0x80;
        cout<< "UNIMP SET 7,D" << endl;
        break;
    case 0xFB: //	SET 7,E
        gb.reg.e |= 0x80;
        cout<< "UNIMP SET 7,E" << endl;
        break;
    case 0xFC: //	SET 7,H
        gb.reg.h |= 0x80;
        cout<< "UNIMP SET 7,H" << endl;
        break;
    case 0xFD: //	SET 7,L
        gb.reg.l |= 0x80;
        cout<< "UNIMP SET 7,L" << endl;
        break;
    case 0xFE: //	SET 7,(HL)
        cout<< "UNIMP SET 7,(HL)" << endl;
        break;
    case 0xFF: //	SET 7,A
        gb.reg.a |= 0x80;
        cout<< "UNIMP SET 7,A" << endl;
        break;
    }
    return;
}
