
#include <stdio.h>
#include <string.h>
#include <mem.h>

#include "sbb.h"    

const char version_SBB[] = "2.2";

const unsigned char version_SBB_code[] ="SB\x22";

const char * NombreMachine (char machine)
{
    switch (machine)
    {
        case 'Z': return"ZX Spectrum";
        case 'C': return"CPC Amstrad";
        case 'M': return"MSX";
        case '8': return"ZX81";
        default : return"???";
    }
}

const char * TipoBloque(char tipo)
{
    switch (tipo)
    {
        case _test_: return "TestScreen";

        case _zx_program_   : return "Program";
        case _zx_char_array_: return "Char array";
        case _zx_num_array_ : return "Num array";
        case _zx_bytes_     : return "Bytes";

        case _zx_tap_hdlss_: return "headerless bytes";
        case _zx_tzx_hdlss_: return "headerless zx";
        case _zx_48k_snap_ : return "snap 48k";
        case _zx_128k_snap_: return "snap 128k";
        case _zx_16k_page_ : return "page 16k";
        case _zx_complete_ : return "$ff20-$ffff bytes";



        case _cpc_basic_    : return "Basic";
        case _cpc_ascii_    : return "Ascii";
        case _cpc_binary_   : return "Binary";
        case _cpc_protected_: return "Protected";
        case _cpc_64k_snap_ : return "Snap64k";

        case _msx_ascii_: return "ASCII";
        case _msx_bytes_: return "BIN";
        case _msx_token_: return "TOKEN";
        case _msx_bin_:   return "BIN";

        case _81_program_: return ".P FILE";
        case _81_snap_:    return ".Z81 FILE";

        
        default: return "UNKNOWN";
    }
}


void copy_data (ts_sbb_block * b, unsigned char *data)
{
    unsigned x;
    unsigned max=b->size;

    b->d_chksum=0;
    for (x=0;x<max;x++)
    {
        b->d_chksum^=data[x];
        b->data[x]=data[x];
    }
    b->d_chksum=0xff;
}

void execjump  (ts_sbb_block * b, unsigned char jump)
{
    unsigned temp=(b->ini-1)^ b->exec;
    b->jump = jump;
    b->h_chksum  = (temp>>8)^temp;
    b->h_chksum ^=  b->jump;
}

int save_sbb_file (const char *file_name,ts_sbb_header *head,ts_sbb_block *blocks)
{
    int res=0;
    FILE *f;
    int b=0;


    f=fopen (file_name, "wb");
    if (f!=NULL)
    {
        memcpy (head->sbb_version,version_SBB_code,3);
        if (fwrite ( head,1,sizeof (ts_sbb_header),f)!=sizeof (ts_sbb_header))
             res=-2;

        while (blocks[b].block_type!=0)
        {
            if (b>_MAX_SBB_BLOCKS_)
                break;

            fwrite ( &blocks[b],1,SIZE_BLOCK_HEADER+blocks[b].size,f);
            b++;
        }
        fclose (f);
    }
    else
        res=-1;
    return res;
}
//////////////////////////////////////////////////////////////

int ajusta_version (ts_sbb_header *head)
{
  int res=0;
  if  ( (head->sbb_version[0]=='S') && (head->sbb_version[1]=='B'))
  {
   if (head->sbb_version[2]<=0x20)
     switch (head->machine[0])
     {
                   case 'Z': head->locate=0xff;head->model=-1;
                   break;
                   case 'C': head->locate=0xff;head->model=-1;
                   break;
                   case 'M': head->locate=0xf4;head->model=-1;
                   break;
                   default:
                   res=-6;
      }
   if (head->sbb_version[2]<=0x21)
     if (
                (head->origin==_Snapshot4ZX48_) ||  //snap
                (head->origin==_Snapshot4ZX128_) ||  //snap
                (head->origin==_Snapshot4CPC_) )
                  head->ei_di&=0xfe ;
     else
                  head->ei_di|=0x01  ;
  }
  else
      res=-5;
  return res;
}


//////////////////////////////////////////////////////////////
int load_sbb_file (const char *file_name,ts_sbb_header *head,ts_sbb_block *blocks)
{
    FILE *f;
    ts_old_header oldheader;
    ts_old_block  oldblock;

    int b=0;  
    int res=0;
    f=fopen (file_name, "rb");
    if (f!=NULL)
    {
        if (head!=NULL)
        {
            if (fread ( head->sbb_version,1,3,f)!=3)
               res=-3;
            if ((head->sbb_version[0]=='S') && (head->sbb_version[1]=='B') )
            {
                fread ( head->machine,1,sizeof (ts_sbb_header)-3,f);
                if (blocks!=NULL)
                    while (fread ( &blocks[b],1,SIZE_BLOCK_HEADER,f)==SIZE_BLOCK_HEADER)
                    {
                        fread ( blocks[b].data,1,blocks[b].size,f);
                        b++;
                    }
            }
            else
                if ((head->sbb_version[0]==0) && (head->sbb_version[1]==0))
                { //old format                
                    head->sbb_version[0]='S';
                    head->sbb_version[1]='B';
                    head->sbb_version[2]=0x10;
                    fread ( oldheader.maquina,1,sizeof (ts_old_header)-3,f);
                    memcpy (head->machine, oldheader.maquina,5);
                    memset (head->extra_info, ' ',6);
                    memcpy (head->nombre, oldheader.nombre,16);
                    //head->caux=oldheader.caux;
                    head->origin=oldheader.tipo;
                    head->n_blocks=oldheader.n_blocks;
                    head->poke_ffff=oldheader.poke_ffff;
                    head->clear_sp=oldheader.clear;
                    head->usr_pc=oldheader.usr;
                    if (blocks!=NULL)
                        while (fread ( &oldblock,1,26,f)==26)
                        {
                        fread ( oldblock.data,1,oldblock.size,f);
                        memcpy (blocks[b].blockname,oldblock.blockname,16);
                        blocks[b].size=oldblock.size;
                        blocks[b].param3=oldblock.param3;
                        blocks[b].block_type=oldblock.type;
                        blocks[b].h_chksum=0xff;
                        blocks[b].ini=oldblock.ini;
                        blocks[b].jump=oldblock.exec;
                        blocks[b].exec=oldblock.exec;
                        if (blocks[b].exec)
                            blocks[b].jump=0xcd;
                        else
                            blocks[b].jump=0x01;
                        blocks[b].d_chksum=oldblock.chksum;
                        memcpy (blocks[b].data,oldblock.data,blocks[b].size=oldblock.size);
                        b++;
                        }
                    head->n_blocks=b;
                }
                else
                    res=-5;
             if (!res)
               res=ajusta_version (head);
        }
        fclose (f);
    }        
    else
       res=-1;

    if (blocks!=NULL)
        blocks[b].block_type=0;
   return res;
}
//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
unsigned  hex2int (char hex)
{
    if (hex>='0'&&hex<='9')
        return hex-'0';
    if (hex>='A'&&hex<='F')
        return 10+hex-'A';
    if (hex>='a'&&hex<='f')
        return 10+hex-'a';
    return 0;
}


#define PATH_SEPARATOR  '\\'
void asigna_nombre ( ts_sbb_header *head , const char *name)
{
   if (strrchr (name,PATH_SEPARATOR)!=NULL)
      strncpy(head->nombre,strrchr (name,PATH_SEPARATOR)+1,sizeof (head->nombre));
   else
      strncpy(head->nombre, name,sizeof (head->nombre));
}
int load_hst_file (const char *file_name,ts_sbb_header *head,ts_sbb_block *blocks)
{
    FILE *f;
    int x;
    f=fopen (file_name, "rb");
    if (f!=NULL)
    {
        asigna_nombre (head , file_name); 
        head->model = -1;
        head->origin=_test_;
        head->ei_di=1;
        head->n_blocks=1;
        head->poke_ffff=head->clear_sp=head->usr_pc=0;

        fread ( blocks[0].data+0x4000,1,0x1800*2,f);

        blocks[0].param3=0;
        blocks[0].block_type=_test_;
        blocks[0].size=0x1800;
        blocks[0].jump=0xc9;
        blocks[0].exec=0x0000;

        switch (head->machine[0])
        {
        case 'C':
            blocks[0].ini=0xc000;
            break ;
        case 'M':
            blocks[0].ini=0xc200;
            break ; 
        case '8':
            blocks[0].ini=16514;    
            blocks[0].size=64;
            break ;
        default:
        case 'Z':
            blocks[0].ini=0x4000;
            break ;
        }
       

        for (x=0;x<blocks[0].size;x++)
            blocks[0].data[x]=0x10*hex2int (blocks[0].data[0x4000+x*2])
                              +hex2int (blocks[0].data[0x4000+1+x*2]);
        copy_data (&blocks[0],blocks[0].data);
        execjump  (&blocks[0], 0xc9);
        blocks[1].block_type=0;
        return 0;
    }
    else
        return -1;
}

