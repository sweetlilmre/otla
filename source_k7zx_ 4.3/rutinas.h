//---------------------------------------------------------------------------

#ifndef rutinasH
#define rutinasH



#define _MAX_ADDRESS_  0xff3c   //65340

/*
#define LD_B_B   0x40
#define LD_A_N   0x3e
#define LD_A_BC  0x0a
#define RET_Z    0xc8
#define INC_DE   0x13
#define DEC_DE   0x1b
#define CP_HL    0xBE
#define PUSH_HL  0xe5
#define POP_HL   0xe1
 */
#define INC_HL_  0x34
#define DEC_HL_  0x35
#define EX_SP_HL 0xe3


#define DIR_SISTEM_VARS    23552


#define NEWPPC    23618
#define NSPPC     23620
#define PPC       23621
#define SUBPPC    23623    
#define VARS      23627  
#define PROG      23635




// #define N_BASIC 22

#define  _POS_K0_ (rutina[1]+1)
#define  _POS_K1_ (rutina[2]+1)


#define _ROM_MIN_       0
#define _FSK_R_         1
#define _FI_MIN_        2
#define _FiQFAST_       3
#define _ESCSLOW_       4
#define _RAUDO48000_    5
#define _ULTRA_R_       6
#define _NPU_R_         7

extern unsigned char * codes_ [] ;
extern unsigned char * codes_extra[] ;


extern unsigned char lprint_code[];
extern unsigned char deco_snap [] ;



extern unsigned char multi_milks [] ;
extern unsigned char multi_slow []  ;
extern unsigned char multi_machester [];

extern unsigned char load_snap_blocks[];
extern unsigned char load_snap_blocks_milks[];
extern unsigned char load_snap_blocks_manchester[];

extern unsigned char SISTEM_VARS [] ;

extern unsigned char BAS [] ;

extern unsigned char multi_128 [];

//---------------------------------------------------------------------------
#endif
