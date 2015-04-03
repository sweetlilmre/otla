//---------------------------------------------------------------------------

#ifndef zx81filesH
#define zx81filesH

#include "sbb.h"

#pragma pack(push, 1)
typedef struct
{

    unsigned short HLx;
    unsigned short DEx;
    unsigned short BCx;
    unsigned short AFx;
    unsigned short IY;
    unsigned short IX;
    unsigned short HL;
    unsigned short DE;
    unsigned short BC;
    unsigned short AF;

    unsigned short SP;
    unsigned short PC;

    unsigned char R;
    unsigned char I;
    unsigned char  IM;
    unsigned char  EI_DI;

    unsigned char  NMI;
    unsigned char  HSYNC;

    unsigned char  aux1;
    unsigned char  aux2;

}
st_snap_81;

#pragma pack(pop)

 
extern unsigned char prbuff_membot [0x40];

int lee_p   (const char *filename,ts_sbb_header *head,ts_sbb_block *blocks);

int lee_z81 (const char *filename,ts_sbb_header *head,ts_sbb_block *blocks);


//---------------------------------------------------------------------------
#endif
