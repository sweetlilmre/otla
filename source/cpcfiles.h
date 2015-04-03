//---------------------------------------------------------------------------

#ifndef cpcfilesH
#define cpcfilesH

#include "sbb.h"

#pragma pack(push, 1)


typedef struct
{

    unsigned char  user;                //00 user number (0 , possible values 0-15)
    unsigned char  filename [15];       //01 - 0F filename+extension (possibly filled with 0)
    unsigned char  block_nuber;         //10 block number (0)
    unsigned char  last_block;          //11 last block flag (0)
    unsigned char  type;                //12 file type (0=basic, 1=protected basic, 2=binary,...)
    unsigned short length_block ;       //13 - 14 length of block (0)
    unsigned short ini  ;               //15 - 16 load address (0-FFFF)
    unsigned char  first_block;         //17 first block flag (0)
    unsigned short logical_length;      //18 - 19 logical length (0-FFFF)
    unsigned short exec;                //1A - 1B entry address (0-FFFF)
    unsigned char  free [0x3f-0x1c+1];  //1C - 3F free for the user (0)
    unsigned short real_length;
    unsigned char  real_length_MSB;     //40 - 42 real length of file (1-FFFFFF)
    unsigned short chksm  ;             //43 - 44 checksum of bytes 00-42
    unsigned char  unused [0x7f-0x45+1];//45 - 7F unused (random values from sector buffer)

}
st_header_amsdos;


//formato del fichero .sna que guarda snapshots del cpc

#define SIZE_OF_CPCSNA_HEADERFILE  256
/*
Snapshot files consist of a 0x100-byte header and a memory dump.

The Snapshot header:
Byte (hex):  Meaning:
00 - 07      'MV - SNA' (as characteristic)
08 - 0F      unused (0)
10           snapshot version (2, former 1)
11 - 1A      Z80 register  AF, BC, DE, HL, R, I
1B - 1C      Z80 flags  IFF0, IFF1 (0=disabled, 1=enabled)
1D - 24      Z80 register IX, IY, SP, PC
25           Z80 interrupt mode IMD (0 - 2)
26 - 2D      Z80 register  AF', BC', DE', HL'
2E           Gate Array: ink number register
2F - 3F      Gate Array: ink value register (0, 1, ..., 15, 16)
40           Gate Array: multi configuration register
41           Gate Array: RAM configuration register
42           CRTC: address register
43 - 54      CRTC: data register (0, 1, ..., 17)
55           Upper ROM number
56 - 59      PIO: port A, port B, port C, control port
5A           PSG: address register
5B - 6A      PSG: data register (0, 1, ..., 15)
6B - 6C      memory dump size (64K or 128K)
(the following parameter exist only from snapshot version 2.0 onwards:)
6D           CPC type where the snapshot was generated
(0=CPC 464, 1=CPC 664, 2=CPC 6128, 3 = unknown)
6E           interrupt number (0..5), the latest interrupt
during a screen update cycle.
6F - 74      6 multimode bytes, thus screen modes (0..2)
for the interrupts 0..5.
75 - FF      unused (0), maybe later for emulator configuration

*/

typedef struct
{
    unsigned char description [8]; //MV - SNA
    unsigned char not_used [8];
    unsigned char snapversion;
    //unsigned char F;
    //unsigned char A;
    unsigned short AF;
    unsigned short BC;
    unsigned short DE;
    unsigned short HL;

    unsigned short IR;
    unsigned char _1ff2;
    unsigned char _di_ei;
    unsigned short IX;
    unsigned short IY;
    unsigned short SP;
    unsigned short PC;
    unsigned char _Imode;

    unsigned short AFx;
    unsigned short BCx;
    unsigned short DEx;
    unsigned short HLx;


    unsigned char GA_inkadd;
    unsigned char GA_inkval[17];
    unsigned char GA_mul_conf_reg;
    unsigned char GA_RAM_conf;

    unsigned char CRCT_address;
    unsigned char CRCT_data[18];
    ///////////////////////////
    //  to do ...

}
st_head_cpc_sna ;

// formato de una pagina dentro de un fichero .sna
typedef struct
{
    unsigned short longitud;
    unsigned char pagina;
    unsigned char data[16*1024];
}
st_page_sna ;



#pragma pack(pop)




int lee_cpc_bin (const char *filename,ts_sbb_header *head,ts_sbb_block *blocks_sbb);
int lee_cpc_sna (const char *filename,ts_sbb_header *head,ts_sbb_block *blocks);



//---------------------------------------------------------------------------
#endif
