//---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h >
#include <string.h >
#include <SysUtils.hpp>

#pragma hdrstop

#include "zxfiles.h"
#include "rutinas.h"



unsigned char * bufferfile=NULL;


char m_warnings [256];
char m_errors [256];

extern  unsigned char memoria [64*1024];

unsigned char pages [8][0x4000];

unsigned clearN;
unsigned usrN;
st_trozo  trozos[32];
int N_trozos;

st_bloque bloques[10];

char prg_name [11];

char bloques_de_snap;//4.3

unsigned HexToInt (char Hex)
{
  if (Hex>='0'&&Hex<='9')
     return Hex-'0';
  if (Hex>='A'&&Hex<='F')
     return 10+Hex-'A';
  if (Hex>='a'&&Hex<='f')
     return 10+Hex-'a';
  return 0;
}



int buscaUSR (unsigned char * bc, unsigned tamanyo)
{
   unsigned interpretados=0;
   st_linea_basic *linea;
   char num[7];
   short i_n;
   short posicion;
   bool comillas;
   bool hayCLEAR;
   //short ini_LOAD;
   //bool hayCODE;
   bool hayUSR;
   int c;
   clearN=0;
   usrN=0;
   while (interpretados<tamanyo)
    {
    linea=(st_linea_basic *)(bc+ interpretados);
    if (interpretados+linea->longitud>tamanyo)
        break;
    if (linea->data [linea->longitud-1]!='\r')
        {break;}
    else
        {
        posicion=1;
        i_n=0;
        comillas=false;
        hayUSR=hayCLEAR=false;
        for (c=0;c<linea->longitud;c++)
           {
           if (comillas)
              if (linea->data[c]=='"')
                {comillas=false; }
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
                        hayCLEAR=true;
                        num[0]= i_n=0;
                        }
                    break;
                case 0xc0://usr
                    if (posicion!=1)
                        {
                        hayUSR=true;
                        num[0]= i_n=0;
                        }
                    break;
                }
           else
              if (linea->data[c]=='"')
                {comillas=true; }
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
                  hayUSR=hayCLEAR=false;
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



unsigned tamanyo;



int leesbb ()
{
    ts_sbb_header * cabecera_sbb;
    ts_sbb_block  * bloque_sbb;


  unsigned pos=0,t=0;
  int error=0;

  
  cabecera_sbb= (ts_sbb_header *)(bufferfile+pos);
  if ((cabecera_sbb->sbb_version[0]!='S')||(cabecera_sbb->sbb_version[1]!='B'))
  {
     strcat (m_warnings,"not sb file\r\n");
     return -1;
  }
  if (cabecera_sbb->machine[0]!='Z')
  {
     strcat (m_warnings,"non zx data\r\n");
     return -1;
  }
  snap.tipo_snap=0;
  clearN=  cabecera_sbb-> clear_sp;
  usrN=    cabecera_sbb->usr_pc;
  pos = sizeof (ts_sbb_header);
  while (pos<tamanyo)
          {
          bloque_sbb= (ts_sbb_block *)(bufferfile+pos);

          trozos[t].procesar=1; 
          trozos[t].tipo='3';
          trozos[t].longitud=bloque_sbb->size;
          trozos[t].inicio=bloque_sbb->ini;
          trozos[t].dir_exe = 0;

          trozos[t].data=bloque_sbb->data;            

          if (!usrN)
              usrN=  bloque_sbb ->exec;
          
          if (bloque_sbb ->block_type== '6')
              bloques_de_snap='X';
          else
              if (bloque_sbb ->block_type== '7')
                  bloques_de_snap='Y';
              else
                if (trozos[t].inicio+trozos[t].longitud>0xff3c)
                     strcat (m_warnings,"block overrides address $ff3c\r\n");
          t++;
          pos+=  28 + bloque_sbb->size;
          }
    trozos[t].tipo=-1;
    N_trozos=t;
    if (N_trozos>7)
       strcat (m_warnings,"more than 7 seven blocks\r\n");
    return error;

}


//////////////////////////////////////////////////////////////
int leehex  ()
{
    int x;
    for (x=0;x<0x1800;x++)
       //paginas[x+0x4000]=0x10*HexToInt (bufferfile[x*2])+HexToInt (bufferfile[1+x*2]);
       pages[5][x]=0x10*HexToInt (bufferfile[x*2])
                           +HexToInt (bufferfile[1+x*2]);

          //clearN=0xff0f;
          clearN=60000;
          usrN= 7997 ;
          trozos[0].procesar=1;
          trozos[0].tipo='3';
          trozos[0].inicio=0x4000;
          trozos[0].longitud=0x1800  ;// - 243;
          trozos[0].dir_exe= 7997 ;
          trozos[0].data=pages[5];//paginas+0x4000;
          trozos[1].tipo=-1;
          N_trozos=1;
     return 0;
}

//////////////////////////////////////////////////////////////


int leetap ()
{
  st_bloq_tap *bloq_tap;
  st_head_tap *head;
  unsigned pos=0,t=0;
  int error=0;
  while (pos<tamanyo)
          {
          bloq_tap= (st_bloq_tap *)(bufferfile+pos);
          pos+=  2 + bloq_tap->logitud_bloq;
          if (bloq_tap->flag==0)
             {
             head=(st_head_tap *)&bloq_tap->flag;
             trozos[t].procesar=1;
             trozos[t].tipo='0'+head->tipo;
             trozos[t].longitud=head->longitud;
             trozos[t].inicio=head->dir_ini;//22755;//tap_bloq->n_bytes;
             trozos[t].dir_exe = 0;

             if (trozos[t].tipo=='0')
                {
                trozos[t].inicio = 23755;
                if (head->dir_ini)
                    trozos[t].auto_run =head->dir_ini;
                else
                    trozos[t].auto_run=0xfffe;
                trozos[t].pam_2  =  head->pam2;
                }
             else
                trozos[t].dir_exe=0;

             if (prg_name[0]==0)
                   strncpy (prg_name,head->nombre,10);
             if (trozos[t].inicio+trozos[t].longitud>_MAX_ADDRESS_)
                 strcat (m_warnings,"block overrides address $ff3c\r\n");
             }
          else
             if (bloq_tap->flag==0xff)
               {
               if (trozos[t].tipo==-1)
                  {
                  trozos[t].tipo='5';
                  trozos[t].inicio=0;
                  trozos[t].longitud=bloq_tap->logitud_bloq-2;
                  strcat (m_warnings,"block without header\r\n");
                  }
               /*
               memcpy (memoria + trozos[t].inicio , bloq_tap->data,
                       trozos[t].longitud);
               trozos[t].data=memoria + trozos[t].inicio;
                */
               trozos[t].data=bloq_tap->data;

               if  (trozos[t].tipo=='0')
                  {
                  buscaUSR (trozos[t].data,trozos[t].longitud);
                /*
                  *(unsigned short *)&BAS[4] = 23755 +trozos[t].pam_2;
                  *(unsigned short *)&BAS[10]= 23755 + trozos[t].longitud+1;
                  *(unsigned short *)&BAS[38]=  trozos[t].auto_run;
                 // memcpy (memoria + (23755+trozos[t].longitud), BAS, 60);
                     */
                  trozos[t].dir_exe=23755+trozos[t].longitud+1;
                //  trozos[t].longitud+=60;
                  }
                else
                  if ((unsigned)trozos[t].inicio<=usrN)
                    if (usrN < (unsigned)trozos[t].inicio +trozos[t].longitud)
                       trozos[t].dir_exe = usrN;
               t++;
               trozos[t].tipo=-1;
               }
             else
               {
               error=-5;//error
               strcat (m_errors,"format error in .tap file format\r\n");
               break;
               }
           }
    N_trozos=t;
    if (N_trozos>7)
       strcat (m_warnings,"more than 7 seven blocks\r\n");
    return error;
}

//////////////////////////////////////////////////////////////
int leetzx ()
{

  int error=0;
  unsigned pos,t=0;

 // st_head_tzx *tzx;
  st_block_10 *ID_10;
  st_block_11 *ID_11;
 // st_block_12 *ID_12;
  st_block_13 *ID_13;
  st_block_14 *ID_14;
  st_block_15 *ID_15;

  t=0;
  trozos[t].tipo=-1;
  trozos[t].inicio=0;
  trozos[t].longitud=0;
  trozos[t].dir_exe = 0;
  trozos[t].data=NULL;
 // int anterior_basic=0;

   //     tzx= (st_head_tzx *)bufferfile;
        pos =sizeof (st_head_tzx);
        while (pos<tamanyo)
          {
          switch (bufferfile[pos])
                {
                case 0x10:
                   ID_10= (st_block_10 *) (bufferfile+pos) ;
                   pos+= 5 + ID_10->long_block_tap;
                   if (ID_10->flag==0)
                      {
                      trozos[t].procesar=1;
                      trozos[t].tipo='0'+ID_10->tipo;
                      trozos[t].inicio=ID_10->dir_ini;
                      trozos[t].longitud=ID_10->longitud;

                      if (trozos[t].tipo=='0')
                         {
                         trozos[t].inicio=23755;//ID_10->dir_ini;
                         if (ID_10->dir_ini)
                            trozos[t].auto_run=ID_10->dir_ini;
                         else
                            trozos[t].auto_run=0xfffe;
                         trozos[t].longitud=ID_10->longitud;
                         trozos[t].pam_2=ID_10->pam2;
                         trozos[t].dir_exe=0;
                         }
                      if (prg_name[0]==0)
                         strncpy (prg_name,ID_10->nombre,10);
                      if (trozos[t].inicio+trozos[t].longitud>_MAX_ADDRESS_)
                         strcat (m_warnings,"block overrides address $ff3c\r\n");
                      }
                   else
                     if (ID_10->flag==0xff)
                       {
                       if (trozos[t].tipo==-1)
                        {
                        trozos[t].tipo='5';
                        trozos[t].inicio=0;
                        trozos[t].longitud=ID_10->long_block_tap-2;
                        strcat (m_warnings,"block without header\r\n");
                        }

                       /*
                       memcpy (memoria + trozos[t].inicio , &ID_10->tipo,
                                          trozos[t].longitud);
                      trozos[t].data=memoria + trozos[t].inicio;
                         */
                      trozos[t].data=&ID_10->tipo;
                      if (trozos[t].tipo=='0')
                          {
                          buscaUSR (trozos[t].data,trozos[t].longitud);
                          
                          *(unsigned short *)&BAS[4] = 23755 +trozos[t].pam_2;
                          *(unsigned short *)&BAS[10]= 23755 + trozos[t].longitud+1;
                          *(unsigned short *)&BAS[38]=  trozos[t].auto_run;
                            /*
                          //memcpy (memoria + (23755+trozos[t].longitud), BAS, 60);
                          memcpy (memBasic , &ID_10->tipo,
                                          trozos[t].longitud);
                          memcpy (memBasic + (0+trozos[t].longitud), BAS, 60);
                          trozos[t].data=memBasic;
                            */
                          trozos[t].dir_exe=23755+trozos[t].longitud;
                      //    trozos[t].longitud+=60;
                          }
                       else
                         if ((unsigned)trozos[t].inicio<=usrN)
                            if (usrN < (unsigned)trozos[t].inicio +trozos[t].longitud)
                               trozos[t].dir_exe = usrN;

                      t++;
                      trozos[t].tipo=-1;
                      trozos[t].inicio=0;
                      trozos[t].longitud=0;
                      trozos[t].data=NULL;
                      }
                   break;
                case 0x11:
                   ID_11= (st_block_11 *) (bufferfile+pos) ;
                   pos+= 19+ ID_11->long_1+0x10000*ID_11->long_2;
                   strcat (m_warnings,"non standard block in tzx\r\n");
                   break;
                case 0x12:
                //   ID_12= (st_block_12 *) (bufferfile+pos) ;
                   pos+= 5;
                   break;
                case 0x13:
                   ID_13= (st_block_13 *) (bufferfile+pos) ;
                   pos+= 2 + 2*ID_13->n_pulses;
                   break;
                case 0x14:
                   ID_14= (st_block_14 *) (bufferfile+pos) ;
                   pos+= 11+ ID_14->long_1+0x10000*ID_14->long_2;
                   strcat (m_warnings,"non standard block in tzx\r\n");
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
    N_trozos=t;    
    if (N_trozos>7)
       strcat (m_warnings,"more than 7 seven blocks\r\n");
    return error;
}

//////////////////////////////////////////////////////////////
st_snap_data snap;

void preparamemoriasnap (char m_141)
{
 // char *dir_snap;
  unsigned short stack128 [7];

  
  memcpy (memoria + 0x4000, pages [5] ,0x4000);
  memcpy (memoria + 0x8000, pages [2] ,0x4000);
  memcpy (memoria + 0xc000, pages [snap.pagina&7] ,0x4000);

  if ( snap.tipo_snap == 1)
      {
      if (m_141)
         {
         trozos[0].longitud= 0xfdbf - 0x4000 ;
         *(short *)(deco_snap+1)=0xfdbf;
         *(short *)(deco_snap+4)=0x4800-0x141;
         *(short *)(deco_snap+7)=0x241;
         memcpy (memoria + 0x4800-0x141, memoria + 0xfdbf, 0x241);
         }
      else
         {
         trozos[0].longitud= 0xff00 - 0x4000 ;
         *(short *)(deco_snap+1)=0xff00;
         *(short *)(deco_snap+4)=0x4800;
         *(short *)(deco_snap+7)=0x100;
         memcpy (memoria + 0x4800, memoria + 0xff00, 0x100);
         }
      }
  else
     {
    // memcpy (memoria + 0x4000, pages [5] ,0x4000);
    // memcpy (memoria + 0x8000, pages [2] ,0x4000);
     memcpy (memoria + 0xc000, pages [7] ,0x4000);

     trozos[0].longitud= 0xbf00 - 0x4000 ; //0x8000 - 0x100;

     if (snap.pagina&8)
       {

       stack128 [0]=0x79ed;
       stack128 [1]=0xf1c1;
       if (snap.im_di_brdr&0x80)
           stack128 [2]=0x4ded;
       else
           stack128 [2]=0xc9f3;
       stack128 [3]=snap.BC;
       stack128 [4]=snap.AF;
       stack128 [5]=snap.PC;
       
       memcpy(memoria +snap.SP -12, stack128, 12);

       if (m_141)
         {
         trozos[0].longitud= 0xbdbf - 0x4000 ;
         *(short *)(deco_snap+1)=0xbdbe;
         *(short *)(deco_snap+4)=0xc800-0x142;
         *(short *)(deco_snap+7)=0x242;
         memcpy (memoria + 0xc800-0x142 , memoria+ 0xbdbe, 0x242);
         }
       else
         {
         trozos[0].longitud= 0xbf00 - 0x4000 ;
         *(short *)(deco_snap+1)=0xbf00;
         *(short *)(deco_snap+4)=0xc800;
         *(short *)(deco_snap+7)=0x100;
         memcpy (memoria + 0xc800    , memoria+0xbf00, 0x100);
         }
       }
     else
       {
       memcpy (memoria + 0x4800    , pages [2] + 0x3f00, 0x100);

       if (m_141)
         {
         trozos[0].longitud= 0xbdbf - 0x4000 ;
         *(short *)(deco_snap+1)=0xbdbe;
         *(short *)(deco_snap+4)=0x4800-0x142;
         *(short *)(deco_snap+7)=0x242;
         memcpy (memoria + 0x4800-0x142 , memoria+ 0xbdbe, 0x242);
         }
       else
         {
         trozos[0].longitud= 0xbf00 - 0x4000 ;
         *(short *)(deco_snap+1)=0xbf00;
         *(short *)(deco_snap+4)=0x4800;
         *(short *)(deco_snap+7)=0x100;
         memcpy (memoria + 0x4800    , memoria+0xbf00, 0x100);
         }
       }
    }


    if (( snap.tipo_snap !=1 )&&(snap.pagina&8))
       {          
       memcpy (memoria + 0xc900    , &snap.HLx , 29);
       memcpy (memoria + 0xc900+29 , deco_snap , 63);
       int n_i=45;

       memoria [0xc900+29+n_i++]=0x3e;
       memoria [0xc900+29+n_i++]=snap.pagina;
       memoria [0xc900+29+n_i++]=0x01;
       memoria [0xc900+29+n_i++]=0xfd;
       memoria [0xc900+29+n_i++]=0x7f;
       memoria [0xc900+29+n_i++]=0x31;
       memoria [0xc900+29+n_i++]=(snap.SP-6)&0xff;
       memoria [0xc900+29+n_i++]=(snap.SP-6)>>8;
       memoria [0xc900+29+n_i++]=0xc3;
       memoria [0xc900+29+n_i++]=(snap.SP-12)&0xff;
       memoria [0xc900+29+n_i++]=(snap.SP-12)>>8;
       }
    else
      {      
      memcpy (memoria + 0x4900    , &snap.HLx , 29);
      memcpy (memoria + 0x4900+29 , deco_snap , 63);
      }


}

int leesna ()
{
  st_head_sna *snap_sna;
  unsigned short *stack ;
  unsigned pos;
  int error=0;

  clearN=0xbdb8;   //0xff0f;

  usrN=0x4900+29;  //18400+26+3;//DIR_CODE_SNAP;

  
      trozos[0].procesar=1;
      trozos[0].tipo='3';
      trozos[0].inicio=0x4000;
      trozos[0].longitud=0xc000;
      trozos[0].data=memoria+0x4000;
      trozos[0].dir_exe= 0x4900 +29 ;18400 +256+26+3 ;
      trozos[1].tipo=-1;
      
      N_trozos=1;

  snap_sna=(st_head_sna *) bufferfile;

  snap.HLx=snap_sna->HLx;
  snap.DEx=snap_sna->DEx;
  snap.BCx=snap_sna->BCx;
  snap.AFx=snap_sna->AFx;
  snap.IY=snap_sna->IY;
  snap.IX=snap_sna->IX;
  snap.HL=snap_sna->HL;
  snap.DE=snap_sna->DE;
  snap.im_di_brdr=(( 0x80*((4&snap_sna->_1ff2_2)==4)+(snap_sna->border&0x7))<<8) +
                   ((0x40*(snap_sna-> imode==0)+(snap_sna-> imode==2)))
                    ;
  snap.IR=(snap_sna->I<<8)+snap_sna->R;
  snap.BC=snap_sna->BC;
  snap.AF=snap_sna->AF;
  snap.SP=snap_sna->SP;


 //memcpy (memoria + 0x4000 ,bufferfile +sizeof (st_head_sna) ,48*1024);

  if (tamanyo==49179)
     {
     memcpy (pages[5] ,bufferfile +sizeof (st_head_sna) ,0x4000);
     memcpy (pages[2] ,bufferfile +sizeof (st_head_sna)+0x4000 ,0x4000);
     memcpy (pages[0] ,bufferfile +sizeof (st_head_sna)+0x8000 ,0x4000);

     snap.jp_reti = 0xed;
     snap.PC= 0x4545;
     snap.pagina=0;
     snap.tipo_snap=1;//sna48k=true;
     }
  else
     {
     if (tamanyo != 131103 && tamanyo != 147487)
	    {
            strcat (m_errors,"error in .sna file format\r\n");
            return -5;
            }

     memcpy (pages[5] ,bufferfile +sizeof (st_head_sna) ,0x4000);
     memcpy (pages[2] ,bufferfile +sizeof (st_head_sna)+0x4000 ,0x4000);

     snap.tipo_snap=2;
     snap.jp_reti = 0xc3;
     snap.PC=*(unsigned short *)&bufferfile[27+49152];

     snap.pagina=bufferfile[27+49152+2];

     memcpy (pages[7&snap.pagina] ,bufferfile +sizeof (st_head_sna)+0x8000 ,0x4000);

     multi_128 [3+20]=snap.pagina;

     multi_128 [3+26]=multi_128 [3+31]=multi_128 [3+36]=
     multi_128 [3+41]=multi_128 [3+46]=multi_128 [3+51]=0x11;

     if (snap.pagina&8)
             {
             multi_128 [58]=(snap.pagina&0xf8)|7;
             multi_128 [63]=0xc9;
             }
         else
             {
             multi_128 [58]=snap.pagina;
             multi_128 [63]=0x49;
             }

     char *dir_pages= bufferfile+ 49183 ;//27+49152+2;

     int t;
     int bloque=2;
     for (int p=0;p<8;p++)
        {
        if (p==2 || p== 5)
           continue;
        else
           if  (p==7)
               t=1;
           else
              if (p==(snap.pagina&0x7))
                 t=6;
              else
                 t=bloque++;

        
         trozos[t].procesar=1;
         trozos[t].tipo='3';
         trozos[t].inicio=0xc000;
         trozos[t].longitud=0x4000 ;

         if (p==(snap.pagina&0x07))
           {
            trozos[t].data=pages[p];
           }
         else
            {
            memcpy (pages[p] , dir_pages , 0x4000);
            trozos[t].data=pages[p]; //dir_pages;
            dir_pages+=0x4000;
            }

         if (p==7)
             trozos[t].data=memoria+0xc000;

         multi_128 [3+20+t*5]=(snap.pagina&0xf8)|p;
         multi_128 [3+21+t*5]=0xcd;
        }
     trozos[7].tipo=-1;
     
     N_trozos=7;
     }

   return error;
}

//////////////////////////////////////////////////////////////

void descomp(unsigned char* klar, unsigned char * comp, unsigned short longitud )
{
 int in=0;
 int out=0;
 if (longitud==0xffff)
     memcpy (klar,comp,0x4000) ;
 else
 while (in<longitud && out<0x10000)
  if (comp[in]==0xed)
     if (comp[in+1]==0xed)
        {
        for (unsigned char i=0;i<comp[in+2];i++)
           klar[out++]=comp[in+3];
         in+=4;
         }
     else
       klar[out++]=comp[in++];
  else
     klar[out++]=comp[in++];
}

int leez80 ()
{
  int error=0;
  unsigned procesados=0;

  st_head_z80 *snap_z80;
  st_page *page;
  unsigned short *stack ;

  clearN=0xff0f;
  usrN=0x4900+29;
  
      trozos[0].procesar=1;
      trozos[0].tipo='3';
      trozos[0].inicio=0x4000; 
      trozos[0].longitud=0xc000;
      trozos[0].data=memoria+0x4000;
      trozos[0].dir_exe= 0x4900 +29 ;18400 +256+26+3 ;
      trozos[1].tipo=-1;
      
      N_trozos=1;

  snap_z80=(st_head_z80 *)bufferfile;//(mem+0x4000-sizeof (st_head_sna));


  snap.HLx=snap_z80->HLx;
  snap.DEx=snap_z80->DEx;
  snap.BCx=snap_z80->BCx;
  snap.AFx=0x100*snap_z80->Ax+snap_z80->Fx;;
  snap.IY=snap_z80->IY;
  snap.IX=snap_z80->IX;
  snap.HL=snap_z80->HL;
  snap.DE=snap_z80->DE;
  snap.im_di_brdr=0x100 * ( 0x80*(snap_z80->_di_ei!=0)+(snap_z80->borderX>>1)) +
                   (0x40*((snap_z80-> _Imode&3)==0)+((snap_z80-> _Imode&3)==2));
   ;
  snap.IR=(snap_z80->I<<8)+snap_z80->R;
  snap.BC=snap_z80->BC;
  snap.AF=0x100*snap_z80->A+snap_z80->F;
  snap.SP=snap_z80->SP;
  snap.jp_reti=0xc3;


  if (snap_z80->PC !=0)
     {
     //sna48k=true;
     snap.tipo_snap=1;
     snap.PC=snap_z80->PC;
     snap.pagina=0;
     if (snap_z80->borderX&32)
         descomp (memoria+0x4000,bufferfile+30,tamanyo-30);
     else
          memcpy (memoria+0x4000, bufferfile+30, 48*1024);

     memcpy (pages[5] ,memoria+0x4000 ,0x4000);
     memcpy (pages[2] ,memoria+0x8000 ,0x4000);
     memcpy (pages[0] ,memoria+0xc000,0x4000);
     }
   else
      {
      snap.PC=snap_z80->PC2;
      //snap_z80->PC=snap_z80->PC2;
      snap.pagina = snap_z80->page_ram  ;
      procesados=30+2+snap_z80->long_head2;

      if (snap_z80->hardware_mode==0)
        {//48k
        snap.tipo_snap=1;

        while (procesados < tamanyo)
           {
           page=(st_page*)(bufferfile+procesados);
           switch (page->pagina)
              {
              case 8:
                 //descomp (memoria+0x4000,page->data,page->longitud);
                 descomp (pages[5],page->data,page->longitud);
                 break;
              case 4:
                 //descomp (memoria+0x8000,page->data,page->longitud);
                 descomp (pages[2],page->data,page->longitud);
                 break;
              case 5:
                 //descomp (memoria+0xc000,page->data,page->longitud);
                 descomp (pages[7&snap.pagina],page->data,page->longitud);
                 break;
              }
            procesados+=3+page->longitud;
            }
         }
      else
         {//128k
         snap.tipo_snap=2;
         trozos[7].tipo=-1;
         N_trozos=7;

         multi_128 [3+20]=snap_z80->page_ram;

         multi_128 [3+26]=multi_128 [3+31]=multi_128 [3+36]=
         multi_128 [3+41]=multi_128 [3+46]=multi_128 [3+51]=0x11;

        if (snap.pagina&8)
             {
             multi_128 [58]=(snap.pagina&0xf8)|7;
             multi_128 [63]=0xc9;
             }
         else
             {
             multi_128 [58]=snap.pagina;
             multi_128 [63]=0x49;
             }


         int t;
         int bloque=2;
         while (procesados < tamanyo)
            {
            page=(st_page*)(bufferfile+procesados);
            switch (page->pagina)
              {
              case 8:
                 descomp (pages[5],page->data,page->longitud);
                 break;
              case 5:
                 descomp (pages[2],page->data,page->longitud);
                 break;

              default:

                if ((page->pagina-3)==7)
                     t=1 ;
                else
                     if ((page->pagina-3)==(snap_z80->page_ram & 7))
                        t=6;
                     else
                        {
                        t=bloque ;
                        bloque++;
                        }

                trozos[t].tipo='3';
                trozos[t].inicio=0xc000;
                trozos[t].longitud=0x4000;
                if (page->longitud!=260)
                  {

                  descomp (pages[page->pagina-3],page->data,page->longitud);
                  
                  if (t==1)
                      trozos[t].data=memoria+0xc000;
                  else
                      trozos[t].data=pages[page->pagina-3];

                  multi_128 [3+20+5*t]=(snap_z80->page_ram&0xf8)|(page->pagina-3);
                  multi_128 [3+21+5*t]=0xcd;
                  trozos[t].procesar=1;
                  }
                else
                  {
                  trozos[t].procesar=0;
                  trozos[t].data =pages[page->pagina-3];
                  }
                break;
              }
            procesados+=3+page->longitud;
            }
         }
      }


  return error;

}

int extension (const char *filename)
{
   int res=-1;
   AnsiString ext=ExtractFileExt(filename).LowerCase();
   if (ext==".hex")
        res=0;
   if (ext==".tap")
        res=1;
   if (ext==".tzx")
        res=2;
   if (ext==".sna")
        res=3;
   if (ext==".z80")
        res=4;
   if (ext==".sbb")
        res=5;
   return res;
}

int readzxfile (const char *filename)
{
  FILE *f;
  int e;

  int res;

  
  m_warnings [0]=0;
  m_errors [0]=0;

  trozos[0].tipo=-1;
  clearN=0;
  usrN=0;
  prg_name[0]=0;

  snap.tipo_snap=0;
  bloques_de_snap=0;

  if (bufferfile!=NULL)
      {
      free ( bufferfile );
      bufferfile=NULL;
      }
  f=fopen (filename,"rb");
//  int anterior_basic=0;
  if (f!=NULL)
     {
     fseek(f, 0L, SEEK_END);
     tamanyo = ftell(f);
     fseek(f, 0, SEEK_SET );
     bufferfile = (unsigned char *) malloc(tamanyo +1 );
     if (bufferfile!=NULL)
        {
        if (fread (bufferfile,1,tamanyo,f)==tamanyo)
           {
           fclose (f);
           e=extension (filename);
           switch (e)
             {
             case 0:
                res=leehex ();
                break;
             case 1:
                res=leetap ();
                break;
             case 2:
                res=leetzx ();
                break;
             case 3:
                res=leesna ();
                break;
             case 4:
                res=leez80 ();
                break;
             case 5:
                res=leesbb ();
                break;
             default:
                res = E_FORMATO;
                break;
            }
         if (prg_name[0]==0)
            strncpy (prg_name,ExtractFileName (filename).c_str(),10);

         if (clearN>_MAX_ADDRESS_)
            strcat (m_warnings,"CLEAR overirdes $ff3c\r\n");

            /*
        if (clearN==0)
            ;//clearN=0xfdbf;
        if (usrN==0)
            ;    */
         }
      else
         {
         res= E_LEER;
         strcat (m_errors,"error reading file\r\n");
         }
      }
     else
       {
       res = E_MEMORIA;
       strcat (m_errors,"not enough memory\r\n");
       }
     }
   else
     {
     res= E_ABRIR;
     strcat (m_errors,"format opening file\r\n");
     }

    return res;
}

//---------------------------------------------------------------------------

#pragma package(smart_init)
