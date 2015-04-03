//---------------------------------------------------------------------------

#ifndef msxfilesH
#define msxfilesH

#include "sbb.h"

#pragma pack(push, 1)


typedef struct
{        
    unsigned char  type;     //1 byte   File type     (FEh=binary)
    unsigned short ini ;     //2 bytes  Load Address  (Destination Address for Data in RAM)
    unsigned short last;     //2 bytes  Last Address  (Load Address + Data Length - 1)
    unsigned short exec;     //2 bytes  Start Address (Used if started as BLOAD"FILE",R)

}
st_header_binmsx;



#pragma pack(pop)



int lee_msx_bin (const char *filename,ts_sbb_header *head,ts_sbb_block *blocks);

int leecas (const char *filename,ts_sbb_header *head,ts_sbb_block *blocks);


int lee_msx_rom (const char *filename,ts_sbb_header *head,ts_sbb_block *blocks,const char * rom2bin );


//---------------------------------------------------------------------------
#endif
