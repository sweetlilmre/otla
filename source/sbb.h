


#ifndef sbbH
#define sbbH

#define  _MAX_SBB_BLOCKS_  32

                    

//sbb origin

#define _test_   'H'
#define _screen_ 'S'
#define _tape_   'T'
#define _cas_    'C'
#define _rom_    'R'
#define _binary_ 'B'

#define _Snapshot4CPC_    'W'

#define _Snapshot4ZX48_   'X'
#define _Snapshot4ZX128_  'Y'


//block type zx

#define _zx_program_     '0'   // - program (basic)
#define _zx_char_array_  '1'   // - character array
#define _zx_num_array_   '2'   // - numeric array
#define _zx_bytes_       '3'   // - code (bytes)

#define _zx_tap_hdlss_   '4'   //  - headerless block
#define _zx_tzx_hdlss_   '5'   //  - headerless block tzx

#define _zx_48k_snap_   '6'   //  6 snapshot 48k
#define _zx_128k_snap_  '7'   //  7 snapshot 128k
#define _zx_16k_page_   '8'   //  8 pagina 16k


#define _zx_complete_   '9'   //  8 pagina 16k


//block type  cpc
#define _cpc_basic_      '-'    //basic
#define _cpc_ascii_      'A'    //ascci
#define _cpc_binary_     'B'    //binario
#define _cpc_protected_  'P'    //protegido
#define _cpc_64k_snap_   'S'    //snapshot

//block type  msx

#define _msx_token_  (char) 0xD3  //basic
#define _msx_bytes_  (char) 0xD0  //memory dump  cas
#define _msx_ascii_  (char) 0xEA  //datafile

#define _msx_bin_    (char) 0xFE  //memory dump  dsk


//block type  zx81

#define _81_program_     'p'    //programm
#define _81_snap_        's'    //snapshot

#pragma pack(push, 1)


typedef struct
{
    char sbb_version [3];
    char maquina [5];
    char nombre [16];

    unsigned char  caux;
    unsigned char  tipo;
    unsigned char  n_blocks;
    unsigned char  poke_ffff;
    unsigned short clear;
    unsigned short usr;
}
ts_old_header;

typedef struct
{
    char  blockname[16];
    unsigned short ini;
    unsigned short size;
    unsigned short param3;
    unsigned short exec;
    char  type;
    unsigned char  chksum;
    unsigned char  data [0x10000];
}
ts_old_block;

///////////////////////////////////////

typedef struct
{
    char sbb_version [3];
    char machine [4];
    char model;
    //char extra_info [8];
    char extra_info [7];
    unsigned char  ei_di;
    char nombre [16];

    unsigned char  locate;//caux;
    char  origin;
    unsigned char  n_blocks;
    unsigned char  poke_ffff;
    unsigned short clear_sp;
    unsigned short usr_pc;
}
ts_sbb_header;

typedef struct
{
    char           blockname[16];
    unsigned short size;
    unsigned short param3;
    char           block_type;
    unsigned char  h_chksum;
    unsigned short ini;
    unsigned char  jump;
    unsigned short exec;
    unsigned char  d_chksum;
    unsigned char  data [0x10000];
}
ts_sbb_block;


#pragma pack(pop)


#define SIZE_BLOCK_HEADER 28

extern const char version_SBB[];
extern const unsigned char version_SBB_code[] ;

void copy_data (ts_sbb_block * b, unsigned char *data);
void execjump  (ts_sbb_block * b, unsigned char jump);

int save_sbb_file (const char *file_name,ts_sbb_header *head,ts_sbb_block *blocks);
int load_sbb_file (const char *file_name,ts_sbb_header *head,ts_sbb_block *blocks);


int ajusta_version (ts_sbb_header *head);

int load_hst_file (const char *file_name,ts_sbb_header *head,ts_sbb_block *blocks);
                 
const char * TipoBloque(char tipo);

const char * NombreMachine (char machine);

void asigna_nombre ( ts_sbb_header *head , const char *name);

#endif
