//---------------------------------------------------------------------------

#ifndef zxfilesH
#define zxfilesH

#include "sbb.h"

#define E_ABRIR    -1
#define E_ESCRIBIR -2
#define E_LEER     -3

#define E_VERSION  -4
#define E_FORMATO  -5

#define E_CLEAR    -6
#define E_USR      -7

#define E_NONSENSE -8
#define E_MEMORIA  -9


#pragma pack(push, 1)



#define MAX_DIR_ZX 0xff20


typedef struct
{
    unsigned char lineaA;
    unsigned char lineaB;
    unsigned short longitud;
    unsigned char data[256];
}
st_linea_basic;


typedef struct
{
    unsigned short logitud_bloq;
    unsigned char flag;
    unsigned char data[2];
}
st_bloq_tap;



typedef struct
{
    unsigned char I;
    unsigned short HLx;
    unsigned short DEx;
    unsigned short BCx;
    unsigned short AFx;
    unsigned short HL;
    unsigned short DE;
    unsigned short BC;
    unsigned short IY;
    unsigned short IX;
    unsigned char _1ff2_2;
    unsigned char R;
    unsigned short AF;
    unsigned short SP;
    unsigned char imode;
    unsigned char border;
}
st_head_sna;


typedef struct
{
    unsigned char A;
    unsigned char F;
    unsigned short BC;
    unsigned short HL;
    unsigned short PC;
    unsigned short SP;
    unsigned char I;
    unsigned char R;
    unsigned char borderX;
    unsigned short DE;
    unsigned short BCx;
    unsigned short DEx;
    unsigned short HLx;
    unsigned char Ax;
    unsigned char Fx;
    unsigned short IY;
    unsigned short IX;
    unsigned char _di_ei;
    unsigned char _1ff2;
    unsigned char _Imode;


    unsigned short long_head2;
    unsigned short PC2;
    unsigned char hardware_mode;
    unsigned char page_ram;
    unsigned char interface_1;
    unsigned char emulation_flags;
    unsigned char last_out;
    unsigned char sound_chip[16];

    unsigned char t_states[3];
    unsigned char ql;
    unsigned char rom_ram[4];
    unsigned char kb_joys [20];
    unsigned char disciple [3];


    ///////////////////////////
}
st_head_z80;

typedef struct
{
    unsigned short longitud;
    unsigned char pagina;
    unsigned char data[16*1024];
}
st_page ;


typedef struct
{
    //unsigned short long_block_tap;
    unsigned char flag;             //ff para data
    char tipo        ; //Type (0,1,2 or 3) 3 para code
    char nombre [10 ]  ;
    unsigned short longitud ; //  (lsb fisrt)
    unsigned short dir_ini        ; //  (lsb fisrt)
    unsigned short pam2        ;
    unsigned char chksm_hd  ;
}
st_head_tap;


/*
        0       "ZXTape!"
        7       0x1A
        8       Major version number
        9       Minor version number
        A       ID of first block
        B       Body of first block

 */
typedef struct
{
    char zx_tape[7];
    char _1a;
    char Va;
    char Vb;
}
st_head_tzx;



/*
ID : 10 -

00 2  Pause After this block in milliseconds (ms)                      [1000]
02 2  Length of following data
04 x  Data, as in .TAP File

- Length: [02,03]+04
*/

typedef struct
{
    char ID;
    unsigned short pause;
    unsigned short long_block_tap;
    unsigned char flag;             //ff para data
    char tipo        ; //Type (0,1,2 or 3) 3 para code
    char nombre [10 ]  ;
    unsigned short longitud ; //  (lsb fisrt)
    unsigned short dir_ini        ; //  (lsb fisrt)
    unsigned short pam2        ;
    char chksm_hd  ;
}
st_block_10;


/*
ID : 11 - Turbo Loading Data Block
This block is very similar to the normal TAP block but with some additional info on the timings and other important differences. The same tape encoding is used as for the standard speed data block. If a block should use some non-standard sync or pilot tones (i.e. all sorts of protection schemes) then use the next three blocks to describe it.
00 2  Length of PILOT pulse                                            [2168]
02 2  Length of SYNC First pulse                                        [667]
04 2  Length of SYNC Second pulse                                       [735]
06 2  Length of ZERO bit pulse                                          [855]
08 2  Length of ONE bit pulse                                          [1710]
0A 2  Length of PILOT tone (in PILOT pulses)         [8064 Header, 3220 Data]
0C 1  Used bits in last byte (other bits should be 0)                     [8]
      i.e. if this is 6 then the bits (x) used in last byte are: xxxxxx00
0D 2  Pause After this block in milliseconds (ms)                      [1000]
0F 3  Length of following data
12 x  Data; format is as for TAP (MSb first)

- Length: [0F,10,11]+18
*/



typedef struct
{
    char ID;
    unsigned short l_pilot;
    unsigned short l_sync1;
    unsigned short l_sync2;
    unsigned short l_zero;
    unsigned short l_one;
    unsigned short n_pilot;
    char  u_bits;
    unsigned short pause;

    unsigned short  long_1;
    unsigned char   long_2;
    unsigned char   data1; // [1];

}
st_block_11 ;

/*
ID : 12  -  Pure tone
-------
        This will produce a tone which is  basically the same as the pilot tone
        in the first two data blocks. You can define how long the pulse is  and
        how many pulses are in the tone.

00 2  Length of pulse in T-States
02 2  Number of pulses

- Length: 04
*/

typedef struct
{
    char ID;
    unsigned short  ancho_pulso;
    unsigned short  n_pulsos;
}
st_block_12 ;

/*
ID : 13  -  Sequence of pulses of different lengths
-------
        This  will  produce  n  pulses,  each having its own timing.  Up to 255
        pulses  can be  stored in  this  block; this is useful for non-standard
        sync tones used by some protection schemes.

00 1  Number of pulses
01 2  Length of first pulse in T-States
03 2  Length of second pulse...
.. .  etc.

- Length: [00]*02+01
*/


typedef struct
{
    char ID;
    unsigned char n_pulses;
    unsigned short ancho_pulso [1];

}
st_block_13 ;

/*
ID : 14  -  Pure data block
-------
        This is the same as in the turbo loading data block, except that it has
        no pilot or sync pulses.

00 2  Length of ZERO bit pulse
02 2  Length of ONE bit pulse
04 1  Used bits in LAST Byte
05 2  Pause after this block in milliseconds (ms)
07 3  Length of following data
0A x  Data (MSb fitst)

- Length: [07,08,09]+0A
*/



typedef struct
{
    char ID;
    unsigned short  zero_length;
    unsigned short  one_length;
    unsigned char used_bits;
    unsigned short pause ;

    unsigned short  long_1;
    unsigned char   long_2;
    unsigned char   data1;  // [1];

}
st_block_14 ;

/*

ID : 15  -  Direct recording
-------
        This block is used for tapes  which have some  parts in  a format  such
        that the  turbo loader  block cannot  be used. This is *not* like a VOC
        file, since the information is much more compact.  Each sample value is
        represented  by one  bit only  (0 for low, 1 for high) which means that
        the block will be at most 1/8 the size of the equivalent VOC.

        *Please* use this  block  *only*  if  you cannot  use the turbo loading
        block.  The preffered sampling frequencies are  22050 (158 T states) or
        44100  Hz  (79 T states/sample).  Please, if  you can, don't  use other
        sampling frequencies.

00 2  Number of T states per sample (bit of data)
02 2  Pause after this block in milliseconds (ms)
04 1  Used bits (samples) in last byte of data (1-8)
      i.e. If this is 2 only first two samples of the last byte will be played
05 3  Data length
08 x  Data. Each bit represents a state on the EAR port (i.e. one sample);
      MSb is played first.

- Length: [05,06,07]+08
*/


typedef struct
{
    char ID;
    unsigned short  tstates_sample;
    unsigned short  pause;
    unsigned char used_bits;
    unsigned short  long_1;
    unsigned char   long_2;
    unsigned char data [1];

}
st_block_15 ;





#pragma pack(pop)

typedef struct
{
    unsigned char procesar;
    char tipo ;
    unsigned short inicio;
    unsigned short longitud;
    unsigned short auto_run;
    unsigned short pam_2;
    unsigned short dir_exe;
    unsigned char * data;

}
st_trozo;



typedef struct
{
    unsigned char procesar;
    char tipo ;
    unsigned short inicio;
    unsigned short longitud;
    unsigned short auto_run;
    unsigned short pam_2;
    unsigned short dir_exe;
    unsigned char pagina;
    int file_pos; //unsigned char * data;
}
st_bloque;




extern char prg_name [11];

//extern unsigned clearN;
//extern unsigned usrN;



extern char m_warnings [256];
extern char m_errors [256];


extern unsigned char printer_buffer [0x100];



int leescr (const char *filename,ts_sbb_header *head,ts_sbb_block *blocks);
int leetap (const char *filename,ts_sbb_header *head,ts_sbb_block *blocks);
int leetzx (const char *filename,ts_sbb_header *head,ts_sbb_block *blocks);
int lee_zx_sna (const char *filename,ts_sbb_header *head,ts_sbb_block *blocks);
int leez80 (const char *filename,ts_sbb_header *head,ts_sbb_block *blocks);

int buscaUSR (unsigned char * bc, unsigned tamanyo);
void zxbasic2bytes(ts_sbb_block *block_sbb);
void zxlastbytes(ts_sbb_header *head,ts_sbb_block *block_sbb);
void restorefromscreen (ts_sbb_block *blocks_sbb);



//---------------------------------------------------------------------------
#endif
