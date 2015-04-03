//---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h >
#include <string.h >
#include <ctype.h >

#include "sbb.h"   
#include "funciones.h"

#pragma hdrstop



#include "81files.h"



unsigned char prbuff_membot [0x40];

//////////////////////////////////////////////////////////////
int lee_p (const char * filename,ts_sbb_header *head_sbb,ts_sbb_block *blocks_sbb)
{
    int x;               
        memset ( head_sbb,0, sizeof (head_sbb));
        strcpy(head_sbb->machine,"81-");
        head_sbb->origin= _81_program_;
        head_sbb->n_blocks= 1;
        asigna_nombre (head_sbb , filename);
    x=load_file(filename);
    if (!x)
    {

        strncpy(blocks_sbb[0].blockname,head_sbb->nombre,16);
        blocks_sbb[0].size=tamanyo;
        blocks_sbb[0].param3=0;
        blocks_sbb[0].block_type=_81_program_;
        blocks_sbb[0].ini=0x4009;
        blocks_sbb[0].jump=0x1;
        blocks_sbb[0].exec=0x0;
        
        copy_data ( &blocks_sbb[0],bufferfile );

        execjump  ( &blocks_sbb[0], 0x01);
        memcpy (prbuff_membot,bufferfile+51 ,sizeof(prbuff_membot));

        tamanyo+=24;

        if (tamanyo<=0x400)
            head_sbb->model = 1;
        else
        if (tamanyo<=0x800)
            head_sbb->model = 2;
        else
        if (tamanyo<=0x4000)
            head_sbb->model = 3;
        else
        if (tamanyo<=0xc000)
            head_sbb->model = 4;
        else
        if (tamanyo<=0x10000)
            head_sbb->model = 5;
    }

    blocks_sbb[1].block_type=0;
    return x;
}




int leeregistro (char * p_in , char * nombre)
{
    int input;
    char *p=  strstr (p_in , nombre );
    if (p!=NULL)
       sscanf (p+3,"%x",&input);
    return input;
}

unsigned char memoria81 [0x10000];
int lee_z81 (const char *filename,ts_sbb_header *head_sbb,ts_sbb_block *blocks_sbb)
{
   st_snap_81 snap;
   int x;
                
   memset ( head_sbb,0, sizeof (head_sbb));
   strcpy(head_sbb->machine,"81-");
   head_sbb->origin=_81_snap_;
   head_sbb->n_blocks= 2;
   asigna_nombre (head_sbb , filename);

   x=load_file(filename);
   if (!x)
   {

    unsigned input,input2;
    char *p;
    char *p_cpu;
    char *p_zx81;
    char *p_memory;

    
    p_cpu=  strstr ((char*)bufferfile ,"[CPU]" );
    p_zx81=  strstr ((char*)bufferfile ,"[ZX81]" );
    p_memory=  strstr ((char*)bufferfile ,"[MEMORY]" );
    *p_zx81++ =0;
    *p_memory++ =0;

    snap.SP=leeregistro (p_cpu ,"SP" );
    snap.PC=leeregistro (p_cpu ,"PC" );

    snap.HL=leeregistro (p_cpu ,"HL " );
    snap.DE=leeregistro (p_cpu ,"DE " );
    snap.BC=leeregistro (p_cpu ,"BC " );
    snap.AF=leeregistro (p_cpu ,"AF " );
    snap.HLx=leeregistro(p_cpu ,"HL_" );
    snap.DEx=leeregistro(p_cpu ,"DE_" );
    snap.BCx=leeregistro(p_cpu ,"BC_" );
    snap.AFx=leeregistro(p_cpu ,"AF_" );
    snap.IX=leeregistro (p_cpu ,"IX" );
    snap.IY=leeregistro (p_cpu ,"IY" );
    snap.R =leeregistro (p_cpu ,"IR" )&0xff;
    snap.I =leeregistro (p_cpu ,"IR" )>>8;
    snap.IM=leeregistro (p_cpu ,"IM" );

    snap.EI_DI=  leeregistro (p_cpu ,"IF2" )&4;

    snap.NMI=leeregistro (p_zx81 ,"NMI" );
    snap.HSYNC=leeregistro (p_zx81 ,"YNC" );
            
    unsigned index;
    unsigned n_repeat;
    unsigned data;

    p=  strstr (p_memory ,"MEMRANGE" );
    if (p!=NULL)
       sscanf (p+9,"%x %x",&input, &input2);
       
    index=input;
    p=strstr(p,"\r\n");
    p+=2;
    do
    {
        if (*p=='*')
        {
            sscanf (p+1,"%x",&n_repeat);
            p=strstr(p+1," ");
            if (p==NULL)
                break;
            p++;
        }
        else
            n_repeat=1;

        sscanf (p,"%x",&data);
        while (n_repeat--)
                memoria81[index++]=data;
        p=strstr(p," ");
        if (p==NULL)
            break;
        p++;
    }
    while (index<input2+1);

    strncpy(blocks_sbb[0].blockname,"z80 regs",10);
    blocks_sbb[0].block_type =_81_snap_;
    blocks_sbb[0].size=32;
    blocks_sbb[0].ini =0;
    blocks_sbb[0].exec=0;
    blocks_sbb[0].param3=0;
    blocks_sbb[0].jump=0x01;
    memcpy(blocks_sbb[0].data,(char*)&snap,blocks_sbb[0].size);
    copy_data ( &blocks_sbb[0],blocks_sbb[0].data);


    strncpy(blocks_sbb[1].blockname,"memory",10);
    blocks_sbb[1].block_type =_81_snap_;
    blocks_sbb[1].size=1+input2-0x4000;
    blocks_sbb[1].ini =0x4000;
    blocks_sbb[1].exec=0;
    blocks_sbb[1].param3=0;
    blocks_sbb[1].jump=0x01;
    memcpy(blocks_sbb[1].data,memoria81+0x4000,1+input2-0x4000);
    copy_data ( &blocks_sbb[1],blocks_sbb[1].data);

    memcpy (prbuff_membot,memoria81+16444,sizeof(prbuff_membot));

   tamanyo= blocks_sbb[1].size;
     if (tamanyo<=0x400)
            head_sbb->model = 1;
        else
        if (tamanyo<=0x400)
            head_sbb->model = 2;
        else
        if (tamanyo<=0x4000)
            head_sbb->model = 3;
        else
        if (tamanyo<=0xc000)
            head_sbb->model = 4;
        else
        if (tamanyo<=0x10000)
            head_sbb->model = 5;
   }       

   blocks_sbb[2].block_type=0;
   return x;
}


//////////////////////////////////////////////////////////////



//---------------------------------------------------------------------------

#pragma package(smart_init)
