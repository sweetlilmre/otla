//---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h >
#include <string.h >


#include "sbb.h"
#include "funciones.h"

#pragma hdrstop



#include "zxfiles.h"



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
    unsigned char I;
    unsigned char R;

    unsigned char  IM;
    unsigned char  EI_DI;

    unsigned char  BORDER;
    unsigned char  OUT_7FFD;

    unsigned char  tipo_snap;
}
st_snap_zx;

st_snap_zx snap_zx;


unsigned char memoria [0x10000];
unsigned char pages [8][0x4000];

unsigned char printer_buffer [0x100];

char m_warnings [256];
char m_errors [256];



unsigned clearN;
unsigned usrN;
char prg_name [11];


unsigned  char BAS []  ={
           0x80 ,
           0x0D ,
           0x80 ,
           0x21 , 0xCB , 0x5C ,
           0x22 , 0x4B , 0x5C ,
           0x21 , 0xCB , 0x5C ,
           0x22 , 0x59 , 0x5C ,
           0x22 , 0x5B , 0x5C ,
           0x23 ,
           0x23 ,
           0x22 , 0x61 , 0x5C ,
           0x22 , 0x63 , 0x5C ,
           0x22 , 0x65 , 0x5C ,
           0x23 ,
           0x22 , 0x5D , 0x5C ,
           0x23 ,
           0x22 , 0x55 , 0x5C ,
           0x21 , 0x0A , 0x00 ,
           0x22 , 0x42 , 0x5C ,
           0xAF ,
           0x32 , 0x44 , 0x5C ,
           0xc9 ,
           0xC3 , 0x3D , 0x1F ,
           0xC9
           };

void zxbasic2bytes(ts_sbb_block *block_sbb)
{
        if (block_sbb->block_type==_zx_program_)
        {
                *(unsigned short *)&BAS[4] = 23755 + block_sbb->param3;//Vars trozos[t].pam_2;
                *(unsigned short *)&BAS[10]= 23755 + block_sbb->size+1;// trozos[t].longitud+1;
                *(unsigned short *)&BAS[38]= block_sbb->ini;// trozos[t].auto_run;
                //memcpy (memoria + (23755+trozos[t].longitud), BAS, 60);
                memcpy (block_sbb->data+block_sbb->size, BAS, 60);  
                block_sbb->block_type=_zx_bytes_;
                block_sbb->ini=23755;
                block_sbb->exec=23755 +block_sbb->size+1;
                block_sbb->size+=60;
                copy_data(block_sbb,block_sbb->data);
        }

}


char complete_zx [256]={
                           0xf3,
                           0x21,   16 , 0x5b,
                           0x11, 0x20 , 0xff,
                           0x01, 0xe0 ,    0,
                           0xED, 0xB0 ,
                           0xfb,
                           0xC3 , 0x20 , 0xF4
                       };
void zxlastbytes(ts_sbb_header *head_sbb,ts_sbb_block *blocks_sbb)
{
    unsigned short dir_last= blocks_sbb[0].ini+blocks_sbb[0].size-1;

   complete_zx[8]=dir_last-MAX_DIR_ZX;
   complete_zx [14]=head_sbb->usr_pc&0xff;
   complete_zx [15]=head_sbb->usr_pc>>8;
   memcpy (complete_zx+16,blocks_sbb[0].data+(MAX_DIR_ZX-blocks_sbb[0].ini), 0xe0);

    blocks_sbb[0].size=MAX_DIR_ZX-blocks_sbb[0].ini;
   copy_data (&blocks_sbb[0],blocks_sbb[0].data);

   memcpy (blocks_sbb[1].blockname,blocks_sbb[0].blockname,10);
   memcpy (blocks_sbb[1].blockname+10," ii",3);
   blocks_sbb[1].block_type=_zx_complete_;
   blocks_sbb[1].ini =0x5b00;
   blocks_sbb[1].size=16+0xe0;
   blocks_sbb[1].exec=0x5b00;
   copy_data (&blocks_sbb[1],complete_zx);

   head_sbb->usr_pc= 0x5b00;
}

int buscaUSR (unsigned char * bc, unsigned tamanyo)
{
    unsigned interpretados=0;
    st_linea_basic *linea;
    char num[7];
    short i_n;
    short posicion;
    char comillas;
    char hayCLEAR;

    char hayUSR;
    int c;
    clearN=0;
    usrN=0;
    while (interpretados<tamanyo)
    {
        linea=(st_linea_basic *)(bc+ interpretados);
        if (interpretados+linea->longitud>tamanyo)
            break;
        if (linea->data [linea->longitud-1]!='\r')
        {
            break;
        }
        else
        {
            posicion=1;
            i_n=0;
            comillas=0;
            hayUSR=hayCLEAR=0;
            for (c=0;c<linea->longitud;c++)
            {
                if (comillas)
                    if (linea->data[c]=='"')
                    {
                        comillas=0;
                    }
                    else
                    {
                        if (linea->data[c]>='0'&&linea->data[c]<='9')
                            if (i_n<5)
                            {
                                num[i_n++]=linea->data[c];
                                num[i_n]=0;
                            }
                    }
                else
                    if (linea->data[c]>127)
                        switch (linea->data[c])
                        {
                        case 0xea: //REM
                            c=linea->longitud;
                            break;
                        case 0xfd: //clear
                            if (posicion==1)
                            {
                                hayCLEAR=1;
                                num[0]= i_n=0;
                            }
                            break;
                        case 0xc0://usr
                            if (posicion!=1)
                            {
                                hayUSR=1;
                                num[0]= i_n=0;
                            }
                            break;
                        }
                    else
                        if (linea->data[c]=='"')
                        {
                            comillas=1;
                        }
                        else
                            if (linea->data[c]==':'
                                    ||linea->data[c]==0x0e/*'\r'*/)
                            {

                                if (hayCLEAR)
                                    if (i_n==5)
                                        clearN=atoi(num);
                                if (hayUSR)
                                    if (i_n==5)
                                        usrN=atoi(num);
                                posicion=0;
                                hayUSR=hayCLEAR=0;
                            }
                            else
                                if (linea->data[c]>='0'&&linea->data[c]<='9')
                                    if (i_n<5)
                                    {
                                        num[i_n++]=linea->data[c];
                                        num[i_n]=0;
                                    }

                posicion++;
            }
        }
        interpretados+=4+linea->longitud;
    }
    return 0;
}




//////////////////////////////////////////////////////////////


int leescr (const char * filename, ts_sbb_header *head,ts_sbb_block *blocks_sbb)
{
    int x;
    memset (memoria,0,0x10000);
    memset (printer_buffer,0,0x100);

    memset ( head,0, sizeof (head));   //2.2
    strcpy(head->machine,"ZXS");
    head->ei_di = 1;
    head->origin=_screen_;
    asigna_nombre (head , filename);
    head->n_blocks=1;
    head->usr_pc=0x5b00;

    x=load_file(filename);
    if (!x)
    {
        strncpy(blocks_sbb[0].blockname,head->nombre,10);

        blocks_sbb[0].size=0x1b00+1;
        blocks_sbb[0].param3=0;
        blocks_sbb[0].block_type= _zx_bytes_ ;
        blocks_sbb[0].ini=0x4000;
        blocks_sbb[0].exec=0x5b00;
        memcpy (memoria+0x4000,bufferfile,0x1b00);
        memoria [0x5b00]=0xc9;
        copy_data ( &blocks_sbb[0],memoria+0x4000);
        execjump  ( &blocks_sbb[0], 0xcd);
    }
    blocks_sbb[1].block_type=0;
    return x;
}

//////////////////////////////////////////////////////////////


int leetap (const char *filename,ts_sbb_header *head_sbb,ts_sbb_block *blocks_sbb)
{
    st_bloq_tap *bloq_tap;
    st_head_tap *head_tap;
    unsigned pos=0,t=0;

    int error;

    memset (memoria,0,0x10000);
    memset (printer_buffer,0,0x100);

    blocks_sbb[t].blockname[0]=0;
    blocks_sbb[t].ini=0;
    blocks_sbb[t].size=0;
    blocks_sbb[t].exec=0;
    blocks_sbb[t].block_type=0;

    clearN=0;
    usrN=0;

    memset ( head_sbb,0, sizeof (head_sbb));   //2.2
    strcpy(head_sbb->machine,"ZXS");
    head_sbb->ei_di = 1;
    head_sbb->origin=_tape_;
    asigna_nombre (head_sbb , filename);

    error=load_file(filename);
    if (!error)
        while (pos<tamanyo)
        {
            if (t>_MAX_SBB_BLOCKS_)
                break;
            bloq_tap= (st_bloq_tap *)(bufferfile+pos);
            pos+=  2 + bloq_tap->logitud_bloq;
            if (bloq_tap->flag==0)
            {
                head_tap=(st_head_tap *)&bloq_tap->flag;
                if (head_sbb->nombre[0]==0)
                    strncpy (head_sbb->nombre,head_tap->nombre,10);
                strncpy (blocks_sbb[t].blockname,head_tap->nombre,10);
                blocks_sbb[t].blockname[10]=0;
                blocks_sbb[t].ini =head_tap->dir_ini;
                blocks_sbb[t].size=head_tap->longitud;
                blocks_sbb[t].exec=0;
                blocks_sbb[t].block_type=_zx_program_ +head_tap->tipo;

                if (blocks_sbb[t].block_type==_zx_program_ )
                {
                     blocks_sbb[t].param3= head_tap->pam2;
                }
            }
            else
                if (bloq_tap->flag==0xff)
                {
                    if (blocks_sbb[t].block_type==0)
                    {
                        strcpy (blocks_sbb[t].blockname,"headless block") ;
                        blocks_sbb[t].ini=0xffff;
                        blocks_sbb[t].size=bloq_tap->logitud_bloq-2;
                        blocks_sbb[t].param3=0;
                        blocks_sbb[t].exec=0;
                        blocks_sbb[t].block_type= _zx_tap_hdlss_ ;
                    }
                    else
                        if (blocks_sbb[t].size!=bloq_tap->logitud_bloq-2)
                            strcat (m_warnings,"wrong size?\r\n");


                    copy_data ( &blocks_sbb[t], bloq_tap->data);       
                    if (!usrN&&_zx_program_==blocks_sbb[t].block_type)
                        buscaUSR (blocks_sbb[t].data,blocks_sbb[t].size);

                    if (blocks_sbb[t].ini+blocks_sbb[t].size<=0x10000)
                        memcpy (memoria + blocks_sbb[t].ini ,
                                bloq_tap->data,
                                blocks_sbb[t].size);


                    t++;

                    blocks_sbb[t].block_type=0;
                }
                else
                {
                    error=-5;//error
                    strcat (m_errors,"format error in .tap file format\r\n");
                    break;
                }
        }

    memcpy (printer_buffer,memoria+0x5b00,0x100);
   /*
    if (blocks_sbb[0].block_type==_zx_program_)         // 2.2
      buscaUSR (blocks_sbb[0].data,blocks_sbb[0].size);          */

    if (blocks_sbb[t].blockname[0])
         strncpy (head_sbb->nombre, blocks_sbb[0].blockname , 10 ) ;
    head_sbb->n_blocks = t ;
    head_sbb->clear_sp = clearN;
    head_sbb->usr_pc  = usrN;

    return error;
}

//////////////////////////////////////////////////////////////
int leetzx (const char *filename,ts_sbb_header *head_sbb,ts_sbb_block *blocks_sbb)
{

  //  st_head_tzx *tzx;
    st_block_10 *ID_10;
    st_block_11 *ID_11;
  //  st_block_12 *ID_12;
    st_block_13 *ID_13;
    st_block_14 *ID_14;
    st_block_15 *ID_15;
    unsigned pos,t;
   // int anterior_basic=0;
    int error;
    
    memset (memoria,0,0x10000);
    memset (printer_buffer,0,0x100);
    t=0;
    blocks_sbb[t].blockname[0]=0;
    blocks_sbb[t].ini=0;
    blocks_sbb[t].size=0;
    blocks_sbb[t].exec=0;
    blocks_sbb[t].block_type=0;

    clearN=0;
    usrN=0;
    
    memset ( head_sbb,0, sizeof (head_sbb));   //2.2
    strcpy(head_sbb->machine,"ZXS");
    head_sbb->ei_di = 1;
    head_sbb->origin=_tape_;
    asigna_nombre (head_sbb , filename);


    error=load_file(filename);
    if (!error)
    {
       // tzx= (st_head_tzx *)bufferfile;
        pos =sizeof (st_head_tzx);
        while (pos<tamanyo)
        {
            if (t>_MAX_SBB_BLOCKS_)
                break;
            switch (bufferfile[pos])
            {
            case 0x10:
                ID_10= (st_block_10 *) (bufferfile+pos) ;
                pos+= 5 + ID_10->long_block_tap;
                if (ID_10->flag==0)
                {
                    if (head_sbb->nombre[0]==0)
                        strncpy (head_sbb->nombre,ID_10->nombre,10);
                    strncpy (blocks_sbb[t].blockname,ID_10->nombre,10) ;
                    blocks_sbb[t].blockname[10]=0;
                    blocks_sbb[t].ini=ID_10->dir_ini; 
                    blocks_sbb[t].param3=ID_10->pam2;
                    blocks_sbb[t].exec=0;
                    if (ID_10->long_block_tap==19)
                    {
                        blocks_sbb[t].size=ID_10->longitud;  
                        blocks_sbb[t].block_type=_zx_program_ +ID_10->tipo;
                    }
                    else
                    {
                        blocks_sbb[t].block_type= _zx_tzx_hdlss_;
                        blocks_sbb[t].ini=0xffff;
                        blocks_sbb[t].exec=0;
                        blocks_sbb[t].size=ID_10->long_block_tap;
                        copy_data ( &blocks_sbb[t],&ID_10->tipo);
                        t++;
                        blocks_sbb[t].blockname[0]=0 ;
                        blocks_sbb[t].ini=0xffff;
                        blocks_sbb[t].size=0;
                        blocks_sbb[t].exec=0;
                        blocks_sbb[t].block_type=0;
                    }

                }
                else
                    if (ID_10->flag==0xff)
                    {
                        if (blocks_sbb[t].block_type==0)
                        {
                            strcpy (blocks_sbb[t].blockname,"headless block") ;
                            blocks_sbb[t].ini=0xffff;
                            blocks_sbb[t].size=ID_10->long_block_tap-2;
                            blocks_sbb[t].param3=0;
                            blocks_sbb[t].exec=0;
                            blocks_sbb[t].block_type=_zx_tap_hdlss_ ;
                        }
                        else
                            if (blocks_sbb[t].size!=ID_10->long_block_tap-2)
                                strcat (m_warnings,"wrong size?\r\n");

                        copy_data ( &blocks_sbb[t],&ID_10->tipo);
                        
                        if (!usrN&&_zx_program_==blocks_sbb[t].block_type)
                           buscaUSR (blocks_sbb[t].data,blocks_sbb[t].size);

                        if (blocks_sbb[t].ini+blocks_sbb[t].size<=0x10000)
                            memcpy (memoria + blocks_sbb[t].ini ,
                                    &ID_10->tipo,
                                    blocks_sbb[t].size);


                        t++;

                        blocks_sbb[t].blockname[0]=0 ;
                        blocks_sbb[t].ini=0xffff;
                        blocks_sbb[t].size=0;
                        blocks_sbb[t].exec=0;
                        blocks_sbb[t].block_type=0;
                    }
                break;
            case 0x11:
                ID_11= (st_block_11 *) (bufferfile+pos) ;
                pos+= 19+ ID_11->long_1+0x10000*ID_11->long_2;

                strcpy (blocks_sbb[t].blockname,"x11 tzx block") ;
                blocks_sbb[t].blockname[10]=0;
                blocks_sbb[t].ini=0xffff;
                blocks_sbb[t].size=ID_11->long_1;
                blocks_sbb[t].param3=0;
                blocks_sbb[t].exec=0;
                blocks_sbb[t].block_type=_zx_tzx_hdlss_;
                copy_data ( &blocks_sbb[t],&ID_11->data1);
                t++;
                break;
            case 0x12:
              //  ID_12= (st_block_12 *) (bufferfile+pos) ;
                pos+= 5;
                break;
            case 0x13:
                ID_13= (st_block_13 *) (bufferfile+pos) ;
                pos+= 2 + 2*ID_13->n_pulses;
                break;
            case 0x14:
                ID_14= (st_block_14 *) (bufferfile+pos) ;
                pos+= 11+ ID_14->long_1+0x10000*ID_14->long_2;
                strcpy (blocks_sbb[t].blockname,"x14 tzx block") ;
                blocks_sbb[t].blockname[10]=0;
                blocks_sbb[t].ini=0xffff;
                blocks_sbb[t].size=ID_14->long_1;
                blocks_sbb[t].param3=0;
                blocks_sbb[t].exec=0;
                blocks_sbb[t].block_type=_zx_tzx_hdlss_;
                copy_data ( &blocks_sbb[t],&ID_14->data1);
                t++;
                break;
            case 0x15:
                ID_15= (st_block_15 *) (bufferfile+pos) ;
                pos+= 9+ ID_15->long_1+0x10000*ID_15->long_2;
                break;
            case 0x18:
            case 0x19:
                pos+=5+ *(unsigned  *) (bufferfile+(pos+1));
                strcat (m_warnings,"non standard block in tzx\r\n");
                break;
            case 'Z':
                pos+= 10;
                break;
            case 0x20:
                pos+= 3;
                break;
            case 0x21:
                pos+= 1+bufferfile[pos+1]+1;
                break;
            case 0x22:
                pos+= 1;
                break;
            case 0x24: //loop
                pos+= 3;
                break;
            case 0x25: //loop
                pos+= 1;
                break;
            case 0x26:
                pos+= 3 +(*(unsigned short *) (bufferfile+(pos+1)))*2;
                break;
            case 0x27:
                pos+= 1;
                break;
            case 0x28:
                pos+= 3 +(*(unsigned short *) (bufferfile+(pos+1)));
                break;
            case 0x2a:
                pos+= 5;
                break;
            case 0x2b:
                pos+= 6;
                break;
            case 0x30:
                pos+= 1+bufferfile[pos+1]+1;
                break;
            case 0x31:
                pos+= 1+ bufferfile[pos+2]+2;
                break;
            case 0x32:
                pos+= 1+bufferfile[pos+1]+0x100*bufferfile[pos+2]+2;
                break;
            case 0x33:
                pos+= 1+bufferfile[pos+1]*3+1;
                break;
            case 0x34:
                pos+= 1+ 8;
                break;
            case 0x35:
                pos+= 1+ 14 + *((unsigned*)&(bufferfile[pos+11]));
                break;

            case 0x40:
                pos+= 1+ bufferfile[pos+2]+0x100*bufferfile[pos+3]
                      +0x10000*bufferfile[pos+4] + 4;
                break;
            default:
                error=-5;
                pos = tamanyo; //pos++;
                strcat (m_errors,"error in .tzx file format\r\n");
            }
        }
    }
    blocks_sbb[t].block_type=0;
    memcpy (printer_buffer,memoria+0x5b00,0x100);
                                           /*
    if (blocks_sbb[0].block_type=='0')                   //2.2
      buscaUSR (blocks_sbb[0].data,blocks_sbb[0].size);         */
    if (blocks_sbb[t].blockname[0])
         strncpy (head_sbb->nombre, blocks_sbb[0].blockname , 10 ) ;
    head_sbb->n_blocks = t ;
    head_sbb->clear_sp = clearN;
    head_sbb->usr_pc  = usrN;

    return error;
}

//////////////////////////////////////////////////////////////
//    SNAP SHOTS
///////////////////////////////////////////////////////


const unsigned char  IM_CODES [4]={0x46,0x56,0x5e,0x46};

unsigned char restore_snap48k []=
    {
        0x3e , 0  ,
        0xd3 , 0xfe ,       // out (fe) . a
        0x3e , 0 ,
        0xED , 0x47 ,       // ld i,a
        0x3e , 0 ,
        0xED , 0x4F ,       // ld r,a
        0xED , 0x4E ,       // im012

        0x01 , 0xc0 , 0x00 ,       //  ld bc, $c0
        0x21 , 0x40-20 , 0x5B ,    //  ld hl, $$b40-20
        0xED , 0xB0 ,              //  ldir
        0xF9 ,                     //  ld sp,hl

        0xE1 ,                     //  pop ...
        0xD1 ,
        0xC1 ,
        0xF1 ,

        0xD9 ,
        0x08 ,
        0xFD , 0xE1 ,
        0xDD , 0xE1 ,

        0xE1 ,
        0xD1 ,
        0xC1 ,
        0xF1 ,                     //...pop

        0x31 , 0x0 , 0x0 ,         // ld sp,nnnn
        0xF3 ,                     // ei/di
        0xC3 , 0x0 , 0x0           // jp nnnn
    };


void restore48k (unsigned char * r)
{
    memcpy (r, restore_snap48k,44);

    r[1] = snap_zx.BORDER;
    r[5] = snap_zx.I;
    r[9] = snap_zx.R;
    r[13]= IM_CODES [3&snap_zx.IM];

    *(short *)(r+38) = snap_zx.SP;
    
    r[40]= snap_zx.EI_DI? 0xfb : 0xf3 ;

    if (!snap_zx.PC)
        r[41]=0xc9;  //0x4ded;
    else
        r[41]=0xc3;

    *(short *)(r+42) = snap_zx.PC;

    return    ;
}

int memory2sbb (ts_sbb_block *blocks_sbb)
{
    memcpy (printer_buffer,memoria+0x5b00,0x100);

    restore48k (memoria+0x5b00);

    memcpy (memoria+0x5b40-20,memoria+0xff40, 0xc0);
    memcpy (memoria+0x5c00-20,&snap_zx,20);
    
    strcpy (blocks_sbb[0].blockname,"$4000-$FF3f");
    blocks_sbb[0].ini=0x4000;
    blocks_sbb[0].size=0xff40-0x4000;
    blocks_sbb[0].exec=0x5b00;
    blocks_sbb[0].param3=0;
    blocks_sbb[0].block_type=_zx_48k_snap_;

    copy_data ( &blocks_sbb[0],memoria+0x4000);

    blocks_sbb[1].block_type=0;
    
    usrN=0x5b00;

    return 0;
}

unsigned char auto_page [0x20+0x4000+1]=
    {
        0x3E , 0x00 ,           //LD A, $00
        0x01 , 0xFD , 0x7F ,    //LD BC, $7FFD
        0xED , 0x79 ,           //OUT (C), A
        0x21 , 0x20 , 0x5b ,    //LD HL, $5b20
        0x11 , 0x00 , 0xc0 ,    //LD DE, $C000
        0x01 , 0x00 , 0x40 ,    //LD BC, $4000
        0xED , 0xB0 ,           //LDIR
        0x3E , 0x00 ,           //LD A, $00
        0x01 , 0xFD , 0x7F ,    //LD BC, $7FFD
        0xED , 0x79 ,           //OUT (C), A
        0xC9                    //RET
    };


unsigned char restore_snap128k_I [0x20+0x4000]=
    {
        0x3E , 0x00 ,           //LD A, $00
        0xED , 0x79 ,           //OUT (C), A
        0xC3 ,  0   , 0x48      //jp pc
    }   ;

unsigned char restore_in_screen[0x20+0x100+10]=
{
        0x21 , 0x20 , 0x48 ,    //  ld hl, $4800+40
        0x11 , 0x00 , 0x5B ,    //  ld de, $5b00
        0x01 , 0x00 , 0x01 ,    //  ld bc, $100
        0xED , 0xB0 ,              //  ldir    
        0xF9 ,                     //  ld sp,hl

        0x01 , 0xFD , 0x1F ,    //LD BC, $1FFD
        0x3E , 0x00 ,           //LD A, $00     //rom
        0xed , 0x79 ,           //OUT (C), A

        0xe1 ,
        0xd1 ,
        0xc1 ,
        0xf1 ,

        0x31 , 0x00 , 0x40 ,    //LD SP, $4000
        0xF3 ,                  //di
        0xC3 , 0    ,0  ,       //jp pc
};

unsigned char restore_in_stack_I[11]=
{
        0x01 , 0xfd , 0x7f ,    //LD BC, $7FFD
        0x3E , 0x00 ,           //LD A, $00     //rom
        
        0x31 , 0 -4 , 0 ,    //LD SP, $4000
        0xC3 , 0 -12 ,0  ,                  //jp pc

};
unsigned char restore_in_stack_II[12]=
{
        0xED , 0x79 ,           //OUT (C), A 
        0xc1 ,                   //pop bc
        0xf1 ,                   //pop af
        0xF3 ,                        //di
        0xC3 , 0xff ,0xff  ,          //jp pc
        0xff,0xff,
        0xff,0xff
};
void pages2sbb (ts_sbb_block *blocks_sbb)
{
     unsigned char  pages_order []={5,7,6,4,3,1,2,0};
     unsigned char  *restore128k;
     unsigned short real_BC=  snap_zx.BC;
     unsigned char  altertative_screen = 0x08& snap_zx.OUT_7FFD ;

     memcpy (printer_buffer,memoria+0x5b00,0x100);

     restore_in_screen[2] =0x48;
     restore_in_screen[17]=0x3e;//not 1ffd

     *(short *)(restore_in_screen+24) = snap_zx.SP;
     restore_in_screen[26] = snap_zx.EI_DI? 0xfb : 0xf3 ;
     restore_in_screen[27] = 0xc3 ;
     *(short *)(restore_in_screen+28) = snap_zx.PC;
     memcpy (restore_in_screen+0x20 ,printer_buffer,0x100);
     memcpy (restore_in_screen+0x120,&snap_zx.HL,8);



    snap_zx.BC=0x7ffd;

    restore48k (memoria+0x5b00);

    memcpy (memoria+0x5b40-20,memoria+0xff40, 0xc0);
    memcpy (memoria+0x5c00-20,&snap_zx,20);

     if  (altertative_screen)
         {
         restore_in_screen[2] =0xc8;

         restore_snap128k_I[1]=0x0f;
         restore_snap128k_I[6]=0xc8;
         restore128k=pages[7]+0x800;
         }
     else
         {
         restore_snap128k_I[1]=snap_zx.OUT_7FFD;
         restore_snap128k_I[6]=0x48;
         restore128k=memoria+0x4800;
         }
     
     memcpy (memoria+0x5b00+36, restore_snap128k_I , 7 );
     
     memcpy (restore128k,restore_in_screen,0x120+8);

     if  (altertative_screen&&(7!=(snap_zx.OUT_7FFD&7)))
     {
         restore_in_stack_I [4]=snap_zx.OUT_7FFD;
        *(short *)(restore_in_stack_I + 6) = snap_zx.SP-4;
        *(short *)(restore_in_stack_I + 9) = snap_zx.SP-12;
        memcpy (restore128k+21,restore_in_stack_I,11);

        restore_in_stack_II[4]= snap_zx.EI_DI? 0xfb : 0xf3 ;  
        *(short *)(restore_in_stack_II + 6) = snap_zx.PC;
        *(short *)(restore_in_stack_II + 8) = real_BC;
        *(short *)(restore_in_stack_II + 10) = snap_zx.AF;
        memcpy (memoria+(snap_zx.SP-12),restore_in_stack_II,12);
     }

     strcpy (blocks_sbb[0].blockname,"[5...");
     blocks_sbb[0].size= 0x1b00+1;     //+1
     blocks_sbb[0].block_type= _zx_128k_snap_;
     blocks_sbb[0].ini = 0x4000;
     blocks_sbb[0].exec= 0x5b00;
     unsigned char temp= memoria[0x5b00];
     memoria[0x5b00]=0xc9; // RET for k7zx compatible
     copy_data (&blocks_sbb[0],memoria+0x4000);
     memoria[0x5b00]=temp;

     strcpy (blocks_sbb[6].blockname,"...5+2+0]");
     blocks_sbb[6].size= 0xff40-0x5b00;
     blocks_sbb[6].block_type= _zx_128k_snap_;
     blocks_sbb[6].ini = 0x5b00;
     blocks_sbb[6].exec= 0x5b00;
     copy_data (&blocks_sbb[6],memoria+0x5b00);


     for (int i=1;i<=5;i++)
     {
        auto_page [1] = altertative_screen |pages_order[i];
        auto_page [19]= altertative_screen ;

        memcpy (auto_page+0x20,pages[pages_order[i]],0x4000);

        sprintf (blocks_sbb[i].blockname,"Page[%i]" , (int)pages_order[i]);
        blocks_sbb[i].size= 0x20+0x4000;
        blocks_sbb[i].block_type= _zx_16k_page_;
        blocks_sbb[i].ini = 0x5b00;
        blocks_sbb[i].exec= 0x5b00;
        copy_data (&blocks_sbb[i],auto_page);
     }
     blocks_sbb[7].block_type=0;
}

void restorefromscreen (ts_sbb_block *blocks_sbb)
{
     restore_in_screen[2] =0x48;
     restore_in_screen[17]=0x3e;//not 1ffd
     
    *(short *)(restore_in_screen+24) = *(short *)(blocks_sbb->data+0x1b00+38);
     memcpy (restore_in_screen+26,blocks_sbb->data+0x1b00+40,4);

     memcpy (restore_in_screen+0x20 ,printer_buffer,0x100);
     memcpy (restore_in_screen+0x120,blocks_sbb->data+0x1c00-8,8);

     blocks_sbb->data[0x1b00+40]=0xc3;
     *(short *)(blocks_sbb->data+0x1b00+41) = 0x4800;

     memcpy(blocks_sbb->data+0x0800,restore_in_screen,0x128);
}


int lee_zx_sna (const char * filename,ts_sbb_header *head,ts_sbb_block *blocks_sbb)
{
    st_head_sna *snap_sna;
    unsigned short *stack ;
    unsigned pos;


    int error;
    
    memset (memoria,0,0x10000);
    memset (printer_buffer,0,0x100);

    clearN=0;
    usrN=0x5b00;

    memset ( head,0, sizeof (head));   //2.2
    strcpy(head->machine,"ZXS");
    head->ei_di = 0;
    asigna_nombre (head , filename);
    head->clear_sp = clearN;
    head->usr_pc  =  usrN;


   error=load_file(filename);

  if (!error)
  {

    snap_sna=(st_head_sna *) bufferfile;

    snap_zx.HLx=snap_sna->HLx;
    snap_zx.DEx=snap_sna->DEx;
    snap_zx.BCx=snap_sna->BCx;
    snap_zx.AFx=snap_sna->AFx;
    snap_zx.IY=snap_sna->IY;
    snap_zx.IX=snap_sna->IX;
    snap_zx.HL=snap_sna->HL;
    snap_zx.DE=snap_sna->DE;
    snap_zx.BC=snap_sna->BC;
    snap_zx.AF=snap_sna->AF;
                           
    snap_zx.I=snap_sna->I;
    snap_zx.R=snap_sna->R;
    snap_zx.SP=snap_sna->SP;


    snap_zx.EI_DI= ((4&snap_sna->_1ff2_2)<<5);
    snap_zx.IM= snap_sna-> imode&3;
    snap_zx.BORDER=snap_sna->border&0x7;


    if (tamanyo==49179)
    {
        snap_zx.PC= 0;
        0x4545;
        snap_zx.OUT_7FFD=0;
        snap_zx.tipo_snap=1;//sna48k=true;

        memcpy (memoria+0x4000,bufferfile+sizeof (st_head_sna),0xc000);

        //prepara_memoria48k ( blocks_sbb);
        memory2sbb (blocks_sbb);
        head->origin=_Snapshot4ZX48_;  
        head->n_blocks = 1; //2.2
    }
    else
    {
        if (tamanyo != 131103 && tamanyo != 147487)
        {
            strcat (m_errors,"error in .sna file format\r\n");
            return -5;
        }

        snap_zx.tipo_snap=2;
        snap_zx.PC=*(unsigned short *)&bufferfile[27+49152];

        snap_zx.OUT_7FFD=bufferfile[27+49152+2];

        memcpy (memoria+0x4000,bufferfile+sizeof (st_head_sna),0xc000);

        memcpy (pages[2],memoria+0x4000,0x4000);
        memcpy (pages[5],memoria+0x8000,0x4000);
        memcpy (pages[(snap_zx.OUT_7FFD&0x7)],memoria+0xc000,0x4000);


        char *dir_pages= bufferfile+ 49183 ;//27+49152+2;

        for (int p=0;p<8;p++)
        {
            if (p==2 || p== 5||p==(snap_zx.OUT_7FFD&0x7))
                continue;

            memcpy (pages[p], dir_pages , 0x4000);

            dir_pages+=0x4000;
        }

        pages2sbb (blocks_sbb);

        head->origin=_Snapshot4ZX128_;
        head->n_blocks = 7 ; //2.2
    }
  }

  return error;
}

//////////////////////////////////////////////////////////////

void descomp(unsigned char* klar, unsigned char * comp, unsigned short longitud )
{
    int in=0;
    int out=0;
    unsigned char i;
    if (longitud==0xffff)
        memcpy (klar,comp,0x4000) ;
    else
        while (in<longitud && out<0x10000)
            if (comp[in]==0xed)
                if (comp[in+1]==0xed)
                {
                    for (i=0;i<comp[in+2];i++)
                        klar[out++]=comp[in+3];
                    in+=4;
                }
                else
                    klar[out++]=comp[in++];
            else
                klar[out++]=comp[in++];
}



int leez80 (const char *filename,ts_sbb_header *head,ts_sbb_block *blocks_sbb)
{
    int error;

    unsigned procesados;

    st_head_z80 *snap_z80;
    st_page *page;
    unsigned short *stack ;


    memset (memoria,0,0x10000);
    memset (printer_buffer,0,0x100);

    clearN=0;
    usrN=0x5b00;

    memset ( head,0, sizeof (head));   //2.2
    strcpy(head->machine,"ZXS");
    head->ei_di = 0;
    asigna_nombre (head , filename);
    head->clear_sp = clearN;
    head->usr_pc  =  usrN;


   error=load_file(filename);

  if (!error)
    {

    clearN=0x00;
    usrN=0x5b00;

    snap_z80=(st_head_z80 *)bufferfile;//(mem+0x4000-sizeof (st_head_sna));


    snap_zx.EI_DI = snap_z80->_di_ei?0x80:0;
    snap_zx.IM= snap_z80-> _Imode&3;

    snap_zx.I = snap_z80->I;
    snap_zx.R = snap_z80->R;
    snap_zx.HLx=snap_z80->HLx;
    snap_zx.DEx=snap_z80->DEx;
    snap_zx.BCx=snap_z80->BCx;
    snap_zx.AFx=0x100*snap_z80->Ax+snap_z80->Fx;
    ;
    snap_zx.IY=snap_z80->IY;
    snap_zx.IX=snap_z80->IX;
    snap_zx.HL=snap_z80->HL;
    snap_zx.DE=snap_z80->DE;
    snap_zx.BC=snap_z80->BC;
    snap_zx.AF=0x100*snap_z80->A+snap_z80->F;
    snap_zx.SP=snap_z80->SP;
    //snap_zx.jp_reti=0xc3;

    snap_zx.BORDER=snap_z80->borderX;

    if (snap_z80->PC !=0)
    {
        //sna48k=true;
        snap_zx.tipo_snap=1;
        snap_zx.PC=snap_z80->PC;
        snap_zx.OUT_7FFD=0;
        if (snap_z80->borderX&32)
            descomp (memoria+0x4000,bufferfile+30,tamanyo-30);
        else
            memcpy (memoria+0x4000, bufferfile+30, 48*1024);

        //prepara_memoria48k ( blocks_sbb);
        memory2sbb (blocks_sbb);  
        head->origin=_Snapshot4ZX48_;
        head->n_blocks = 1 ; //2.2
    }
    else
    {
        snap_zx.PC=snap_z80->PC2;

        snap_zx.OUT_7FFD = snap_z80->page_ram  ;
        procesados=30+2+snap_z80->long_head2;

        if (snap_z80->hardware_mode==0)
        {//48k
            snap_zx.tipo_snap=1;

            while (procesados < tamanyo)
            {
                page=(st_page*)(bufferfile+procesados);
                switch (page->pagina)
                {
                case 8:
                    descomp (memoria+0x4000,page->data,page->longitud);
                    //    descomp (pages[5],page->data,page->longitud);
                    break;
                case 4:
                    descomp (memoria+0x8000,page->data,page->longitud);
                    //    descomp (pages[2],page->data,page->longitud);
                    break;
                case 5:
                    descomp (memoria+0xc000,page->data,page->longitud);
                    //     descomp (pages[7&snap_zx.pagina],page->data,page->longitud);
                    break;
                }
                procesados+=3+page->longitud;
            }
            //prepara_memoria48k ( blocks_sbb);
            memory2sbb (blocks_sbb);  
            head->origin=_Snapshot4ZX48_;
            head->n_blocks = 1 ; //2.2
        }
        else
        {//128k
            snap_zx.tipo_snap=2;

            blocks_sbb[8].block_type=0;
            while (procesados < tamanyo)
            {
                page=(st_page*)(bufferfile+procesados);
                if (page->pagina>=3&&page->pagina<=10)
                {
                    descomp (pages[page->pagina-3],page->data,page->longitud);
                    procesados+=3+page->longitud;
                }
                else
                    ;
            }
            memcpy (memoria+0x4000,pages[5],0x4000);
            memcpy (memoria+0x8000,pages[2],0x4000);
            memcpy (memoria+0xc000,pages[0],0x4000);
            pages2sbb (blocks_sbb); 
            head->origin=_Snapshot4ZX128_;
            head->n_blocks = 1 ; //2.2
            
        }
    }
  }
  return error;

}



//---------------------------------------------------------------------------

#pragma package(smart_init)
