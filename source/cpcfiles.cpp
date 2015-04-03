//---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h >
#include <string.h >


#include "sbb.h" 
#include "funciones.h"

#pragma hdrstop 

#include "cpcfiles.h"


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
    unsigned short IR;
    unsigned short BC;
    unsigned short AF;

    unsigned short SP;
    unsigned short PC;

    unsigned char  tipo_snap;
    unsigned char  im_di;

    char border;
    char pagina;

}
st_snap_cpc;

st_snap_cpc snap_cpc;




//////////////////////////////////////////////////////////////

int lee_cpc_bin (const char * filename, ts_sbb_header *head_sbb,ts_sbb_block *blocks_sbb)
{
    int x;
    st_header_amsdos * header_amsdos;

         memset ( head_sbb,0, sizeof (head_sbb));   //2.2
         strcpy(head_sbb->machine,"CPC"); 
         head_sbb->ei_di = 1;
         head_sbb->locate  = 0xff;
         head_sbb->n_blocks  = 1;
        head_sbb->origin=_binary_;

    x=load_file(filename);
    if (!x)
    {
        header_amsdos= (st_header_amsdos *)bufferfile;  
        strncpy(head_sbb->nombre,header_amsdos->filename,16);
        strcpy (blocks_sbb[0].blockname,header_amsdos->filename);
        blocks_sbb[0].ini=header_amsdos->ini;
        blocks_sbb[0].size=header_amsdos->real_length;
        blocks_sbb[0].exec=header_amsdos->exec;
        blocks_sbb[0].param3=0;
        switch (header_amsdos->type)
        {
        case 0x00:
            blocks_sbb[0].block_type=_cpc_basic_;
            break;
        case 0x01:
            blocks_sbb[0].block_type=_cpc_ascii_;
            break;
        case 0x02:
            blocks_sbb[0].block_type=_cpc_binary_;
            break;
        case 0x03:
            blocks_sbb[0].block_type=_cpc_protected_;
            break;
        }

        copy_data ( &blocks_sbb[0],bufferfile+128);
        if (blocks_sbb[0].exec)
               execjump  ( &blocks_sbb[0], 0xcd);
        else
               execjump  ( &blocks_sbb[0], 0x01);
    }
    blocks_sbb[1].block_type=0;
    return x;
}

char ga_crtc_regs_codec[110]  //={0xe1,0x31,0xfd,0xff,0xe9};
={
     0x21 , 0x31 , 0x80 , 0xAF , 0x06 , 0xBC , 0xED , 0x79 , 0x06 , 0xBD ,
     0x4E , 0x23 , 0xED , 0x49 , 0x3C , 0xFE , 0x12 , 0x20 , 0xF1 , 0x01 ,
     0xFF , 0x7F , 0x21 , 0x43 , 0x80 , 0xAF , 0xED , 0x79 , 0x56 , 0x23 ,
     0xED , 0x51 , 0x3C , 0xFE , 0x10 , 0x20 , 0xF5 , 0x56 , 0x23 , 0xED ,
     0x51 , 0x56 , 0xED , 0x51 , 0xE1 , 0x31 , 0xFf , 0xFF , 0xE9 , 0x00 ,

     0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
     0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
     0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
     0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
     0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,

     0x00
 };

unsigned char snap_cpc_decod [80] // =
={
      0x11 , 0x58 , 0xFF , 0x21 , 0x40 , 0x5B , 0x01 , 0xA8 , 0x00 , 0xED ,
      0xB0 , 0xF9 , 0xE1 , 0xD1 , 0xC1 , 0xF1 , 0xD9 , 0x08 , 0xFD , 0xE1 ,
      0xDD , 0xE1 , 0xE1 , 0xD1 , 0xF1 , 0xED , 0x47 , 0x1F , 0xED , 0x4F ,
      0x3E , 0x07 , 0xD3 , 0xFE , 0x01 , 0xFD , 0x1F , 0x3E , 0x00 , 0xED ,
      0x79 , 0x01 , 0xFD , 0x7F , 0x3E , 0x00 , 0xED , 0x79 , 0xC1 , 0xF1 ,

      0xED , 0x4E , 0x31 , 0x55 , 0x55 , 0xF3 , 0xC3 , 0x44 , 0x44
 };

/*
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
    unsigned short IR;
    unsigned short BC;
    unsigned short AF;

    unsigned short SP;
    unsigned short PC;

    unsigned char  im_di;


}
st_snap_regs;
st_snap_regs regs;
*/
unsigned char restore_cpc_snap [32+22+10]  //=
={
        0x31 , 0x20 , 0xFe ,
        0xE1 , 0xD1 , 0xC1 , 0xF1 , 0xD9 , 0x08 ,
        0xFD , 0xE1 , 0xDD , 0xE1 , 0xE1 , 0xD1 ,
        0xF1 ,
        0xED , 0x47 ,
        0x1F ,
        0xED , 0x4F ,
        0xC1 , 0xF1 ,
        0xED , 0x4E ,
        0x31 , 0x55 , 0x55 ,
        0xF3 ,
        0xC3 , 0x44 , 0x44
};
//////////////////////////////////////////////////////////////
int lee_cpc_sna (const char * filename,ts_sbb_header *head_sbb,ts_sbb_block *blocks_sbb)
{
   int x;
    st_head_cpc_sna  *head_cpc_sna;
    
         memset ( head_sbb,0, sizeof (head_sbb));   //2.2
         strcpy(head_sbb->machine,"CPC"); 
         head_sbb->ei_di = 0;
        asigna_nombre (head_sbb , filename);
         head_sbb->locate  = 0xff;
         head_sbb->n_blocks  = 4; 
        head_sbb->origin=_Snapshot4CPC_;

    x=load_file(filename);
    if (!x)
    {

        head_cpc_sna= (st_head_cpc_sna *)bufferfile;
        if (memcmp(head_cpc_sna->description ,"MV - SNA",8))
            x=-5;
        if ((tamanyo!=0x10100)&&(tamanyo!=0x20100))
           x=-5;


        strcpy (blocks_sbb[0].blockname,"GA & CRTC regs");
        blocks_sbb[0].ini=0x8000;
        blocks_sbb[0].size=110;
        blocks_sbb[0].exec=0x8000;
        blocks_sbb[0].param3=0;
        blocks_sbb[0].block_type=_cpc_binary_;
        execjump  ( &blocks_sbb[0], 0xcd);

        memcpy (ga_crtc_regs_codec+0x31,&head_cpc_sna->CRCT_data,18);
        memcpy (ga_crtc_regs_codec+0x31+18,&head_cpc_sna->GA_inkval,16);
        ga_crtc_regs_codec[0x31+18+16]=head_cpc_sna->GA_mul_conf_reg;
        ga_crtc_regs_codec[0x31+18+17]=head_cpc_sna->GA_inkadd;
        copy_data ( &blocks_sbb[0],ga_crtc_regs_codec);

        strcpy (blocks_sbb[1].blockname,"0xc000-0xff00");
        blocks_sbb[1].ini=0xc000;
        blocks_sbb[1].size=0xff00-0xc000;
        blocks_sbb[1].exec=0;
        blocks_sbb[1].param3=0;
        blocks_sbb[1].block_type=_cpc_64k_snap_;
        copy_data ( &blocks_sbb[1],bufferfile+0x100+0xc000);
        execjump  ( &blocks_sbb[0], 0x01);

        strcpy (blocks_sbb[2].blockname,"0x0000-0xbfff");
        blocks_sbb[2].ini=0x0000;
        blocks_sbb[2].size=0xc000;
        blocks_sbb[2].exec=0;
        blocks_sbb[2].param3=0;
        blocks_sbb[2].block_type=_cpc_64k_snap_;
        copy_data ( &blocks_sbb[2],bufferfile+0x100); 
        execjump  ( &blocks_sbb[0], 0x01);


        strcpy (blocks_sbb[3].blockname,"SNAP restorer");

        snap_cpc.HLx=head_cpc_sna->HLx;
        snap_cpc.DEx=head_cpc_sna->DEx;
        snap_cpc.BCx=head_cpc_sna->BCx;
        snap_cpc.AFx=head_cpc_sna->AFx;

        snap_cpc.IY=head_cpc_sna->IY;
        snap_cpc.IX=head_cpc_sna->IX;
        snap_cpc.HL=head_cpc_sna->HL;
        snap_cpc.DE=head_cpc_sna->DE;

        snap_cpc.IR=head_cpc_sna->IR;
        snap_cpc.BC=head_cpc_sna->BC;
        snap_cpc.AF=head_cpc_sna->AF;

        memcpy (restore_cpc_snap+32,&snap_cpc,22);

        switch (head_cpc_sna->_Imode)
        {
        case 0:
            restore_cpc_snap[24]=0x46;
            break;
        case 1:
            restore_cpc_snap[24]=0x56;
            break;
        case 2:
            restore_cpc_snap[24]=0x5e;
            break;
        }
        restore_cpc_snap[26]=head_cpc_sna->SP&0xff;
        restore_cpc_snap[27]=head_cpc_sna->SP>>8;
        if (head_cpc_sna->_di_ei==1)
            restore_cpc_snap[28]= 0xfb;

        restore_cpc_snap[30]=head_cpc_sna->PC&0xff;
        restore_cpc_snap[31]=head_cpc_sna->PC>>8;

        blocks_sbb[3].ini=0xfe00;
        blocks_sbb[3].size=128;
        blocks_sbb[3].exec=0xfe00 ;
        head_cpc_sna->PC;
        blocks_sbb[3].param3=0;
        blocks_sbb[3].block_type=_cpc_binary_;
        copy_data ( &blocks_sbb[3],restore_cpc_snap);
        execjump  ( &blocks_sbb[0], 0xc3);

        blocks_sbb[4].block_type=0;

    }
    return x;
}

//////////////////////////////////////////////////////////////



//---------------------------------------------------------------------------

#pragma package(smart_init)
