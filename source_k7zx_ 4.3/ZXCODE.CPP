
//---------------------------------------------------------------------------
#include <stdio.h>  
#include <stdlib.h >
#include <string.h>
#include <math.h>


#pragma hdrstop

#include "rutinas.h"
#include "zxfiles.h"
#include "zxwav.h"

#include "zxcode.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


char antikolmogorov;

       
 char cargador;
 char control_chksum;

char esquema_bloques;



unsigned short dir_clear;
unsigned short dir_usr;



unsigned char memoria [64*1024];

st_poke pokes [_MAX_POKES_ +1 ];




#pragma pack(push, 1)
//---------------------------------------------------------------------------



typedef struct {

  unsigned short longitud_cabecera ; //19 fijo

  st_head_tap cabecera;

  unsigned short longitub_data  ; // longitud_datos+2 (2=flag +chksm)

  char flagff            ; //ff para data
  st_linea_basic linea;

  } st_cargador_basic ;

#pragma pack(pop)






unsigned   new_ini;
unsigned   new_fin;
unsigned  max;

unsigned  char multiload_code [70];
unsigned  total_trozos=0;

struct {
   double cuenta;
   char simbolo;
   }  histo [5];



int cambiaLOADCODE (unsigned char * bc, unsigned n_size)
{
   unsigned interpretados=0;
   st_linea_basic *linea;
   char num[7];
   short i_n;
   short posicion;
   bool comillas;
   short ini_LOAD;
   bool hayCODE;

   int c;

   while (interpretados<n_size)
    {
    linea=(st_linea_basic *)(bc+ interpretados);
    if (interpretados+linea->longitud>n_size)
        break;
    if (linea->data [linea->longitud-1]!='\r')
        {break;}
    else
        {
        posicion=1;
        i_n=0;
        comillas=false;
        ini_LOAD=0;
        hayCODE=false;
        for (c=0;c<linea->longitud;c++)
           {
           if (comillas)
              if (linea->data[c]=='"')
                {comillas=false; }
              else
                ;
           else
            if (linea->data[c]>127)
              switch (linea->data[c])
                {
                case 0xea: //REM
                    c=linea->longitud;
                    break;
                case 0xef: //load
                    if (posicion==1)
                        ini_LOAD=c+1;
                    break;
                case 0xaa: //screen
                case 0xaf: //code
                    if (posicion!=1)
                        hayCODE=true;
                    break;
                }
           else
              if (linea->data[c]=='"')
                {comillas=true; }
              else
                if (linea->data[c]==':'||linea->data[c]=='\r')
                  {          /*
                  if (hayCODE&&ini_LOAD)
                       {
                       linea->data[ini_LOAD-1]= 0xe0;
                       linea->data[ini_LOAD]=   0x27;
                       linea->data[ini_LOAD+1]=';';
                       linea->data[ini_LOAD+2]=':' ;':';
                       i_n=ini_LOAD+3;
                       while (i_n<c)
                          linea->data[i_n++]=':';
                       }   */
                  if (ini_LOAD)
                       {
                       linea->data[ini_LOAD-1]= 0xe0;
                       if (!hayCODE)
                           linea->data[ini_LOAD]=   0x27; // '
                       else
                           linea->data[ini_LOAD]=   0x2c; // ,
                       linea->data[ini_LOAD+1]=';';
                       i_n=ini_LOAD+2;
                       while (i_n<c)
                          linea->data[i_n++]=':';
                       }
                  posicion=0;
                  ini_LOAD=0;
                  hayCODE=false;
                  }

           posicion++;
           }
        }
    interpretados+=4+linea->longitud;
    }
  return 0;
}


void analisisestadistico()
{
   
   for (int t=0;t<4;t++)
      {
      histo[t].cuenta=0;
      histo[t].simbolo=t;
      }
   if (antikolmogorov)
     {
      for (unsigned  i=new_ini;i<new_fin;i++)
          {
          histo[memoria[i]>>6].cuenta++;
          histo[(memoria[i]>>4)&3].cuenta++;
          histo[(memoria[i]>>2)&3].cuenta++;
          histo[memoria[i]&3].cuenta++;
          }
     for (int t=0;t<3;t++)
       for (int  i=t+1;i<4;i++)
         if (histo[t].cuenta<histo[i].cuenta)
           {
           histo[4]=histo[t];
           histo[t]=histo[i];
           histo[i]=histo[4];
           }
      }

    histo[4]=histo[3];

    p [histo[0].simbolo]=1;
    p [histo[1].simbolo]=2;
    p [histo[2].simbolo]=3;
    p [histo[3].simbolo]=4;

}
void pokea (void)
{
  int i=0;
  while (pokes[i].dir!=0)
    {
    memoria[pokes[i].dir]=pokes[i].value;
    i++;
    }
}


unsigned char BasicTap[3000];

unsigned char rutina [513];

void make_tabla (int x,int d00,int d01,int d10,int d11)
{
   int i=1;
   while (i++)
     if (i<d00)
       rutina [x+(3*i)/2]= 0xfc|histo[0].simbolo;
     else
        if (i<d01)
           rutina [x+(3*i)/2]= 0xfc|histo[1].simbolo;
        else
            if (i<d10)
               rutina [x+(3*i)/2]= 0xfc|histo[2].simbolo;
            else
                if (i<d11)
                    rutina [x+(3*i)/2]= 0xfc|histo[3].simbolo;
                else
                    {
                    //rutina [x+(3*i)/2]=0;
                    return;
                    }

}



void make_tabla_raudo (int x,int d00,int d01,int d10,int d11)
{
   int i=2;

   do
     if (i<d00)
       rutina [x+i]= 0;
     else
        if (i<d01)
           rutina [x+i]= 1;
        else
            if (i<d10)
               rutina [x+i]= 2;
            else
                if (i<d11)
                    rutina [x+i]= 3;
                else
                      return;

     while (i+=3);
}


void make_tabla_slow (int x,int d00,int d01,int d10,int d11)
{
   
   int i=1;
   x=x-2;
   while (i++)
     if (i<d00)
       rutina [x+i]= 0xfc;
     else
        if (i<d01)
           rutina [x+i]= 0xfd;
        else
            if (i<d10)
               rutina [x+i]= 0xfe;
            else
                if (i<d11)
                    rutina [x+i]= 0xff;
                else
                    return;
}


void make_tabla_ESCU (int x,int d01,int d10,int d11,int d00)
{
   int i=0;
   while (++i)
     if (i<d01)
       rutina [x+(3*i)]= 0xfd;
     else
        if (i<d10)
           rutina [x+(3*i)]= 0xfe;
        else
            if (i<d11)
               rutina [x+(3*i)]= 0xff;
            else
                if (i<d00)
                    rutina [x+(3*i)]= 0xfc;
                else
                    return;
}

void make_tabla_ESCU2 (int x,int d01,int d10,int d11,int d00)
{
   int i=0;
   while (++i)
     if (i<d01)
       rutina [x+(3*i)/2]= 0xfd;
     else
        if (i<d10)
           rutina [x+(3*i)/2]= 0xfe;
        else
            if (i<d11)
               rutina [x+(3*i)/2]= 0xff;
            else
                if (i<d00)
                    rutina [x+(3*i)/2]= 0xfc;
                else
                    return;
}
void make_tabla_ESCU1 (int x,int d01,int d10,int d11,int d00)
{
   int i=0;
   while (++i)
     if (i<d01)
       rutina [x+i]= 0xfd;
     else
        if (i<d10)
           rutina [x+i]= 0xfe;
        else
            if (i<d11)
               rutina [x+i]= 0xff;
            else
                if (i<d00)
                    rutina [x+i]= 0xfc;
                else
                    return;
}

void make_tabla_npu_P (int x,int d00,int d01,int d10,int d11)
{
   int i=0;
   while (++i)
     if (i<d00)
       rutina [x+i]= 0;
     else
        if (i<d01)
           rutina [x+i]= 1;
        else
            if (i<d10)
               rutina [x+i]= 2;
            else
                if (i<d11)
                    rutina [x+i]= 3;
                else
                      return;
}

void make_tabla_npu_N (int x,int d00,int d01,int d10,int d11)
{
   int i=0;
   while (--i)
     if (i>-d00)
       rutina [x+i]= 0;
     else
        if (i>-d01)
           rutina [x+i]= 1;
        else
            if (i>-d10)
               rutina [x+i]= 2;
            else
                if (i>-d11)
                    rutina [x+i]= 3;
                else
                      return;
}

int pos_MAX;
int pos_JP;
int pos_EI;
int long_rutina;

//void copiarutina(int m)
void copiarutina(char * codigo)
{
 //strcpy (rutina,codigos [m]);
 memcpy (rutina,codigo,360);
 pos_MAX=rutina[0]+1 ;
 pos_EI= rutina[3];
 pos_JP=  pos_EI+1   ;
 long_rutina=strlen (rutina+5);
}

void rutinaCarga()
{
 float k1;
 char *tabla;


 copiarutina  (codes_ [metodo]);

 switch (metodo)
      {
      case _ROM_:
           if (muestras_por_bit==_2_50)
               {
               copiarutina  (codes_extra [_ROM_MIN_]);
               if (FqMuestreo==44100)
                  rutina[_POS_K0_]=0xf2+3;
               else
                  rutina[_POS_K0_]=0xf2+6;
               }
           else
               {
               if (FqMuestreo==44100)
                   rutina[_POS_K0_]=12;
               else
                   rutina[_POS_K0_]=9;
               rutina[_POS_K1_]=0x1f;
               }
            break;
      case _MILKS_:
            if (FqMuestreo==44100)
              switch (muestras_por_bit)
                {        // 3.1 mejorar 12kbs y 16kbps
                case _3_50:
                case _2_75:
                    rutina[26]= 0xfc  ;
                    break;
                default:
                    break;
                }
            else
              {
              rutina[26]= 0xfc  ;
              rutina[30]= 0xfd  ;
              rutina[36]= 0xfe  ;
              rutina[41]= 0xff  ;
              }
            break;
      case _FSK_:
         if (FqMuestreo==44100)
          switch (muestras_por_bit )
            {
            case _8_00:
                  rutina[rutina[2]+1]=0xda;//0xe6-0x10;
              break;
            case _7_00:
                  rutina[rutina[2]+1]=0xe9-0x10;
              break;
            case _6_00:
                  rutina[rutina[2]+1]=0xec-0x10;
              break;
            case _5_00:
                  rutina[rutina[2]+1]=0xf2-0x10;
              break;
            case _4_00:
                  //rutina[rutina[2]+1]=0xf4-0x10;
                  rutina[rutina[2]+1]=0xf5-0x10;
              break;
            case _3_00:
                 //+2ok rutina[rutina[2]+1]=0xf7-0x10;
                  rutina[rutina[2]+1]=0xf8-0x10;
              break;
            case _2_50:
                  //rutina[rutina[2]+1]=0xfa-0x10;
                  //copiarutina(_FSK_R_);
                  copiarutina  (codes_extra [_FSK_R_]);
                  rutina[rutina[2]+1]=109-3-3;
              break;
           }
         else
           switch (muestras_por_bit )
            {
            case _8_00:
                  rutina[rutina[2]+1]= 0xdb;//0xe8-0x10;
              break;
            case _7_00:
                  rutina[rutina[2]+1]=0xec-0x10;
              break;
            case _6_00:
                  rutina[rutina[2]+1]=0xf0-0x10;
              break;
            case _5_00:
                  rutina[rutina[2]+1]=0xf3-0x10;
              break;
            case _4_00:
                  //rutina[rutina[2]+1]=0xf5-0x10;
                  rutina[rutina[2]+1]=0xf6-0x10;
              break;
            case _3_00:
                  //+2casi okrutina[rutina[2]+1]=0xf8-0x10;
                  rutina[rutina[2]+1]=0xf9-0x10;
              break;
            case _2_50:
                 // rutina[rutina[2]+1]=0xea; //0xfb-0x10;
                  //copiarutina(_FSK_R_);
                  copiarutina  (codes_extra [_FSK_R_]);
                  rutina[rutina[2]+1]=107-3;
              break;
           }
           break;
           /*
      case _SHAVINGS_SLOW_:
             k1= (27.575*64*FqMuestreo)/3500000;
             rutina[rutina[2]+1]=k1;
             rutina[rutina[2]+1]=k1/2;
             if (FqMuestreo==44100)
               switch (muestras_por_bit)
                {
                case _2_50:
                   rutina[rutina[1]+1]= 7; 5; -2 ;- 2;
                   break;
                case _3_00:
                   rutina[rutina[1]+1]=-26; //26
                   break;
                case _3_50:
                   rutina[rutina[1]+1]=-53 ; //-49;//-53;
                   break;
                case _4_00:
                   rutina[rutina[1]+1]=-87; //-79;
                    break;
                }
             else
               switch (muestras_por_bit)
                {
                case _2_50:
                   rutina[rutina[1]+1]= 11; 12; 7; 8;
                   rutina[rutina[1]+1]=  12;
                   break;
                case _3_00:
                   rutina[rutina[1]+1]=-24; //-29; -25; -24;
                   break;
                case _3_50:
                   rutina[rutina[1]+1]=-57; -57 ;
                   break;
                case _4_00:
                   rutina[rutina[1]+1]=-89;-80;-71;//-69;
                   rutina[rutina[2]+1]=12;

                   rutina[rutina[1]+1]=-81;
                   rutina[rutina[1]-4]= 0x34;
                   rutina[rutina[1]-3]= 0xbb;

                   break;
                }
           break;      */
           
      case _SHAVINGS_SLOW_:
           if (FqMuestreo==44100)
              switch (muestras_por_bit)
                {
                case _1_75:
                    rutina[_POS_K0_]-=2;
                    make_tabla_slow(rutina[2],5,7,10,14);
                    break;
                case _2_50:
                    rutina[_POS_K0_]-=2;
            //        make_tabla_slow(rutina[2],7,12,17,24);
                    make_tabla_slow(rutina[2],7,11,17,24);
                    break;
                case _3_00:
                    rutina[_POS_K0_]-=5;
                    make_tabla_slow(rutina[2],7,12,17,23);
                    break;
                case _3_50:
                    rutina[_POS_K0_]-=7;
                 //   make_tabla_slow(rutina[2],6,12,17,23);
               make_tabla_slow(rutina[2],7,12,17,23); //for 48000

                    break;
                case _4_00:
                    rutina[_POS_K0_]-=10;
                    make_tabla_slow(rutina[2],7,12,17,23);
                    break;
                }
           else
              switch (muestras_por_bit)
                {
                case _1_75:
                    rutina[_POS_K0_]-=2;        
                    rutina[_POS_K0_+3]= rutina[_POS_K0_-10]= 0x2c;
                    make_tabla_slow(rutina[2],4,7,10,14);
                    break;
                case _2_50:
                    rutina[_POS_K0_+3]= rutina[_POS_K0_-10]= 0x2c;
                    rutina[_POS_K0_]-=2;
                    make_tabla_slow(rutina[2],6,11,16,22);
                    break;
                case _3_00:
                    rutina[_POS_K0_]-=4;
                    make_tabla_slow(rutina[2],6,11,16,22);
                    break;
                case _3_50:
                    rutina[_POS_K0_]-=7;
                    make_tabla_slow(rutina[2],6,11,15,21);
                    break;
                case _4_00:
                    rutina[_POS_K0_]-=9;
                    make_tabla_slow(rutina[2],6,11,16,22);
                    break;
                }
               
           break;

      case _SHAVINGS_DELTA_:
            analisisestadistico();
            if (FqMuestreo==44100)
                switch (muestras_por_bit)
                  {
                  case _1_75:
                     make_tabla (rutina[1]-3,6,10,13,26) ;
                     break;
                  case _2_25:
                     make_tabla (rutina[1]-3,9,13,16,26) ;
                     break;
                  case _2_75:
                     make_tabla (rutina[1]-3,13,16,20,26) ;
                     break;

                  case _3_00:
                     make_tabla (rutina[1]-3,10,17,24,36) ;
                     break;
                  case _3_50:
                     make_tabla (rutina[1]-3,14,21,28,37) ;
                     break;
                  }
             else
                switch (muestras_por_bit)
                  {
                  case _1_75:
                     //rutina[rutina[2]]=0x3e;
                     make_tabla (rutina[1]-3,5,8,12,25) ;
                     break;
                  case _2_25:
                     make_tabla (rutina[1]-3,8,12,15,25) ;
                     break;
                  case _2_75:
                     make_tabla (rutina[1]-3,12,15,18,25) ;
                     break;

                  case _3_00:
                     make_tabla (rutina[1]-3,10,16,23,35) ;
                     break;
                  case _3_50:
                     make_tabla (rutina[1]-3,13,19,25,34) ;
                     break;
                  }
             break;

      case _SHAVINGS_RAUDO_:
            long_rutina= 277;
            if (FqMuestreo==44100)
               switch (muestras_por_bit)
                 {
                 case _1_75:
                //    rutina[rutina[1]]=rutina[rutina[1]+0x40]=0x3e;
                     make_tabla_raudo (rutina[2],18,33,48-3,71) ;
                    make_tabla_raudo (rutina[2],18,33,48-3,71) ;
                     break;
                 case _2_25:
                     make_tabla_raudo (rutina[2],36-3,51-3,66-3,85) ;
                     break;
                 case _2_50:
                     make_tabla_raudo (rutina[2],24,54,84, 108);//110);
                     break;
                 case _2_75:
                //     make_tabla_raudo (rutina[2],51-3,66-3,81-3,99) ;
               //        make_tabla_raudo (rutina[2],51-3+3,66-3,81-3,99) ; //mejora 4410 desde 48000
                make_tabla_raudo (rutina[2],51-3,66-3,81-3,99) ;
                      break;
                 }
            else
               switch (muestras_por_bit)
                 {
                 case _1_75:
                     rutina[rutina[1]]=
                     rutina[rutina[1]+0x40]=0x3e;
                     make_tabla_raudo (rutina[2],15,27,39+3,84) ;

                     copiarutina  (codes_extra [_RAUDO48000_]);
                     long_rutina=262;
                     make_tabla_raudo (rutina[2]+1,20,30,42+3,68) ;
                     break;
                 case _2_25:
                     make_tabla_raudo (rutina[2],30,42,54,78) ;
                     break;
                 case _2_50:
                     make_tabla_raudo (rutina[2],21,45,72,99);
                     break;
                 case _2_75:
                     make_tabla_raudo (rutina[2],42+3,54+3,66+3,84) ;//+3 ok MP·!!!
                     break;
                 }
            break;

      case _ULTRA_:
           if  (FqMuestreo==44100)
              switch (muestras_por_bit)
                {
                case _1_50:
                    copiarutina  (codes_extra [_ULTRA_R_]);
                    rutina[_POS_K0_]=12;
                    rutina[_POS_K1_]=212;
                   break;
               case _2_00:
                    rutina[_POS_K0_]=(0x78+0x7e)/2;//err00x78; ////er10x7e;//0x7d; //0x7b;
                    break;
               case _2_50:
                    copiarutina  (codes_extra [_ULTRA_R_]);
                    rutina[_POS_K0_]=12-9;
                    rutina[_POS_K1_]=212-9;
                    break;
               case _3_00:
                    rutina[_POS_K0_]=0x78;
                    break;
                }
            else
              switch (muestras_por_bit)
                {
                case _1_50:
                    copiarutina  (codes_extra [_ULTRA_R_]);
                    rutina[_POS_K0_]=13;//5;
                    rutina[_POS_K1_]=213;//5;
                    break;
                case _2_00:
                    rutina[_POS_K0_]=0x7b;
                    break;
                case _2_50:
                    copiarutina  (codes_extra [_ULTRA_R_]);
                    rutina[_POS_K0_]=2+13-9;//5;
                    rutina[_POS_K1_]=2+213-12;//5;
                    break;
                case _3_00:
                     rutina[_POS_K0_]=0x79;
                     break;
                }
            break;

      case _NPU_:
          switch (muestras_por_bit)
             {
             case  _1_25:
                 copiarutina  (codes_extra [_NPU_R_]);
                 long_rutina=16+rutina[pos_JP+1]-rutina[19];
                 if (FqMuestreo==44100)
                    make_tabla_npu_P (rutina[2],11,23,32,55);
                 else
                    make_tabla_npu_P (rutina[2],11,21,31,55);
                 break;
             case  _1_75:
                 copiarutina  (codes_extra [_NPU_R_]);
                 long_rutina=16+rutina[pos_JP+1]-rutina[19];
                 if (FqMuestreo==44100)
                    make_tabla_npu_P (rutina[2],23,35,44,58);
                 else
                    make_tabla_npu_P (rutina[2],22,31,43,55);
                 break;
             case  _2_00:
                 long_rutina=16+rutina[pos_JP+1]-rutina[19];
                 if (FqMuestreo==44100)
                    make_tabla_npu_P (rutina[2]-1,5,10,15,23) ;
                 else
                    {
                    rutina[rutina[1]]   =rutina[rutina[1]+14]=
                    rutina[rutina[1]+28]=rutina[rutina[1]+42]= 0x2c;
                    make_tabla_npu_P (rutina[2]-1,5,10,15,23) ;
                    }
                 break;
             case  _2_50:
                 long_rutina=16+rutina[pos_JP+1]-rutina[19];
                 if (FqMuestreo==44100)
                    make_tabla_npu_P (rutina[2]-1,8,13,18,25) ;
                 else
                    {
                    rutina[rutina[1]]=   rutina[rutina[1]+14]=
                    rutina[rutina[1]+28]=rutina[rutina[1]+42]= 0x2c;
                    make_tabla_npu_P (rutina[2]-1,8,12,17,24) ;
                    }
                 break;
              }
             break;
                              
      case _FI_:
            if (muestras_por_bit==_3_00)
                {
                 copiarutina  (codes_extra [_FI_MIN_]);
                 rutina[_POS_K0_]=8-2;
                }
            else      /*
               if (FqMuestreo==44100)
                 rutina[_POS_K0_]=10 ;
               else
                 rutina[_POS_K0_]=10;   */
              // rutina[_POS_K0_]=0; //255;     
            break;
      case _FI_Q_:
          if (FqMuestreo==44100)
             switch (muestras_por_bit)
             {
             case  _1_75:
                 copiarutina  (codes_extra [_FiQFAST_]);
                 rutina[_POS_K1_]=0x2b;
                 rutina[_POS_K0_]=0x8;
                 break;
             case _2_00:
                rutina[_POS_K1_]=0x55+3;
                rutina[_POS_K0_]=rutina[_POS_K1_]+0x88+3;
                 break;
             case _2_25:
                rutina[_POS_K1_]=0x55;
                rutina[_POS_K0_]=rutina[_POS_K1_]+0x88+3;
                 break;
             case _2_50:
                rutina[_POS_K1_]=0x4f;
                rutina[_POS_K0_]=rutina[_POS_K1_]+0x88+3;
                 break;
             case  _2_75:
                rutina[_POS_K1_]=0x4a;
                rutina[_POS_K0_]=rutina[_POS_K1_]+0x88+3;
                 break;   
             case  _3_00:
                rutina[_POS_K1_]=0x41; //0xb7;
                rutina[_POS_K0_]=rutina[_POS_K1_]+0x88+3;
                 break;
             case  _3_50:
                rutina[_POS_K1_]=0x37;
                rutina[_POS_K0_]=rutina[_POS_K1_]+0x88+3;
                 break;
             }
          else
             switch (muestras_por_bit)
             {
             case  _1_75:
                 copiarutina  (codes_extra [_FiQFAST_]);
                 rutina[_POS_K1_]=0x28;
                 break;
             case _2_00:
                 rutina[_POS_K1_]=0x59;
                rutina[_POS_K0_]=rutina[_POS_K1_]+0x88+3;
                 break;
             case _2_25:
                 rutina[_POS_K1_]=0x59;
                rutina[_POS_K0_]=rutina[_POS_K1_]+0x88+3;
                 break;
             case  _2_50:
                rutina[_POS_K1_]=0x50;
                rutina[_POS_K0_]=rutina[_POS_K1_]+0x88;
                 break;
             case  _2_75:
                rutina[_POS_K1_]=0x50;
                rutina[_POS_K0_]=rutina[_POS_K1_]+0x88;
                 break;
             case  _3_00:
                 rutina[_POS_K1_]=0x47;
                rutina[_POS_K0_]=rutina[_POS_K1_]+0x88;
                 break;
             case  _3_50:
                 rutina[_POS_K1_]=0x3d;
                rutina[_POS_K0_]=rutina[_POS_K1_]+0x88+3;
                 break;
             }
            break;

      case _MANCHESTER_:
            rutina[rutina[2]+1]=rutina[rutina[1]+1];
            switch (muestras_por_bit)
               {
               case _2_00:
                  if (FqMuestreo==44100)
                     {
                     rutina[rutina[2]+1]-=4; //3 max a 48000
                     rutina[rutina[1]+1]-=4;  // 6 max a 44100
                     }
                  else
                     {
                     rutina[rutina[2]+1]-=2; //3 max a 48000
                     rutina[rutina[1]+1]-=2;  // 6 max a 44100
                     }
                  break;
               case _3_00:
                  if (FqMuestreo==44100)
                     {
                     rutina[rutina[2]+1]-=4; //3 max a 48000
                     rutina[rutina[1]+1]-=4;  // 6 max a 44100

                     rutina[rutina[2]+ 7]=INC_HL_;
                     rutina[rutina[2]+ 8]=DEC_HL_;
                     rutina[rutina[2]+ 9]=INC_HL_;
                     rutina[rutina[2]+10]=DEC_HL_;
                     }
                  else
                     {
                     rutina[rutina[2]+1]-=5; //3 max a 48000
                     rutina[rutina[1]+1]-=5;  // 6 max a 44100

                     rutina[rutina[2]+ 9]=DEC_HL_;
                     rutina[rutina[2]+10]=INC_HL_;
                     }

                  break;
               case _4_00:
                    if (FqMuestreo==44100)
                     {
                     rutina[rutina[2]+1]-=8; //3 max a 48000
                     rutina[rutina[1]+1]-=8;  // 6 max a 44100

                     rutina[rutina[2]+ 3]=EX_SP_HL;
                     rutina[rutina[2]+ 4]=EX_SP_HL;
                     rutina[rutina[2]+ 5]=INC_HL_;
                     rutina[rutina[2]+ 6]=DEC_HL_;
                     rutina[rutina[2]+ 7]=INC_HL_;
                     rutina[rutina[2]+ 8]=DEC_HL_;
                     rutina[rutina[2]+ 9]=INC_HL_;
                     rutina[rutina[2]+10]=DEC_HL_;
                     }
                   else
                    {
                     rutina[rutina[2]+1]-=8; //3 max a 48000
                     rutina[rutina[1]+1]-=8;  // 6 max a 44100

                     rutina[rutina[2]+ 3]=INC_HL_;
                     rutina[rutina[2]+ 4]=DEC_HL_;
                     rutina[rutina[2]+ 5]=INC_HL_;
                     rutina[rutina[2]+ 6]=DEC_HL_;
                     rutina[rutina[2]+ 7]=INC_HL_;
                     rutina[rutina[2]+ 8]=DEC_HL_;
                     rutina[rutina[2]+ 9]=INC_HL_;
                     rutina[rutina[2]+10]=DEC_HL_;
                     }
                  break;
               }
            break;

      case _MANCHESTER_DIF_:
            rutina[rutina[2]+1]=rutina[rutina[1]+1];
            switch (muestras_por_bit)
               {
               case _2_00:
                  if (FqMuestreo==44100)
                     {
                     rutina[rutina[1]+2]-=3;
                     rutina[rutina[2]+2]-=3;
                     }
                  else
                     {
                     rutina[rutina[1]+2]-=1;
                     rutina[rutina[2]+2]-=1;
                     }
                  break;
               case _3_00:
                  if (FqMuestreo==44100)
                     {
                     rutina[rutina[1]+2]-=4;
                     rutina[rutina[2]+2]-=4;

                     rutina[rutina[1]-12]=INC_HL_;
                     rutina[rutina[1]-13]=DEC_HL_;
                     rutina[rutina[1]-14]=INC_HL_;
                     rutina[rutina[1]-15]=DEC_HL_;

                     rutina[rutina[2]-12]=INC_HL_;
                     rutina[rutina[2]-13]=DEC_HL_;
                     rutina[rutina[2]-14]=INC_HL_;
                     rutina[rutina[2]-15]=DEC_HL_;
                     }
                  else
                     {
                     rutina[rutina[1]+2]-=4;
                     rutina[rutina[2]+2]-=4;

                     rutina[rutina[1]-12]=INC_HL_;
                     rutina[rutina[1]-13]=DEC_HL_;

                     rutina[rutina[2]-12]=INC_HL_;
                     rutina[rutina[2]-13]=DEC_HL_;
                     }
                  break;

               case _4_00:
                  if (FqMuestreo==44100)
                     {
                  rutina[rutina[1]+2]-=6;
                  rutina[rutina[1]-12]=EX_SP_HL;
                  rutina[rutina[1]-13]=EX_SP_HL;
                  rutina[rutina[1]-14]=EX_SP_HL;
                  rutina[rutina[1]-15]=EX_SP_HL;
                  rutina[rutina[1]-16]=INC_HL_;
                  rutina[rutina[1]-17]=DEC_HL_;
                  rutina[rutina[2]+2]-=6;
                  rutina[rutina[2]-12]=EX_SP_HL;
                  rutina[rutina[2]-13]=EX_SP_HL;
                  rutina[rutina[2]-14]=EX_SP_HL;
                  rutina[rutina[2]-15]=EX_SP_HL;
                  rutina[rutina[2]-16]=INC_HL_;
                  rutina[rutina[2]-17]=DEC_HL_;
                    }
                  else
                    {

                  rutina[rutina[1]+2]-=6;
                  rutina[rutina[1]-12]=EX_SP_HL;
                  rutina[rutina[1]-13]=EX_SP_HL;
                  rutina[rutina[1]-14]=INC_HL_;
                  rutina[rutina[1]-15]=DEC_HL_;
                  rutina[rutina[1]-16]=INC_HL_;
                  rutina[rutina[1]-17]=DEC_HL_;
                  rutina[rutina[2]+2]-=6;
                  rutina[rutina[2]-12]=EX_SP_HL;
                  rutina[rutina[2]-13]=EX_SP_HL;
                  rutina[rutina[2]-14]=INC_HL_;
                  rutina[rutina[2]-15]=DEC_HL_;
                  rutina[rutina[2]-16]=INC_HL_;
                  rutina[rutina[2]-17]=DEC_HL_;
                  }
                  break;
               }
            break;

      case _ESCURRIDO_:
           if (FqMuestreo==44100)
              switch (muestras_por_bit)
                 {
                 case _1_33:
                  make_tabla_ESCU2 (rutina[1]-1,3,7,10,14) ;
                  rutina[rutina[2]+16]=0xed;
                  rutina[rutina[2]+17]=0x41;
                     break;
                 case _1_50:
                    make_tabla_ESCU2 (rutina[1]-1,3,7,10,14) ;
                     break;
                 case _2_00:
               //   make_tabla_ESCU (rutina[1]-2+1,8,12,15,18) ;
               //   rutina[rutina[2]+14]-=1;
                /*  ITERESANTE   esc 5 o 6
                    make_tabla_ESCU (rutina[1]-2-1,7,10,13,18) ;
                    rutina[rutina[2]+14+2]-=1;     */
                                    /*MAs todvái
                    make_tabla_ESCU (rutina[1]-2-1,7,10,13,18) ;
                    rutina[rutina[2]+14+2];//-=1;*/
                /*
                    make_tabla_ESCU (rutina[1]-2+1,7,11,14,18) ;
                    rutina[rutina[2]+14+3]-=2;   */

                //   make_tabla_ESCU2 (rutina[1]-1,7,10,14,18) ;
                     make_tabla_ESCU2 (rutina[1]-1,7,10,14,18) ;
                     break;

                 case _2_25:
                     copiarutina  (codes_extra [_ESCSLOW_]);
                     rutina[rutina[2]+12]=8;  //pausa1
                     make_tabla_ESCU1 (rutina[1],8,13,19,25) ;
                     rutina[rutina[2]+1]+=7;  //pausaE=0
                     break;
                 case _2_50:
                     copiarutina  (codes_extra [_ESCSLOW_]);
                     rutina[rutina[2]+12]=10;  //pausa1
                     make_tabla_ESCU1 (rutina[1],8,13,18,25) ;
                     rutina[rutina[2]-18]=5;  //pausaE
                     break;
                }
           else
              switch (muestras_por_bit)
                 {
                 case _1_33:
                     make_tabla_ESCU2 (rutina[1]-1,3,7,10,14) ;
                     rutina[rutina[2]+1]+=2;
                     rutina[rutina[2]+16]=0x40;0xed;
                     rutina[rutina[2]+17]=0x40;0x41;
                     break;
                 case _1_50:
                     make_tabla_ESCU2 (rutina[1]-1,3,7,10,14) ;
                     rutina[rutina[2]+1]+=2;
                     rutina[rutina[2]+17]+=2;
                     break;
                 case _2_00:
                     make_tabla_ESCU2 (rutina[1]-1,7,10,14,18) ;
                     rutina[rutina[2]+17]+=2;
                     rutina[rutina[2]+1]+=2;
                     break;

                 case _2_25:
                     copiarutina  (codes_extra [_ESCSLOW_]);
                     make_tabla_ESCU1 (rutina[1],8,13,18,25) ;
                     rutina[rutina[2]+12]=6; //pausa1
                     rutina[rutina[2]+1]+=9;
                     break;
                 case _2_50:
                     copiarutina  (codes_extra [_ESCSLOW_]);
                     make_tabla_ESCU1 (rutina[1],7,11,17,26) ;
                     rutina[rutina[2]+12]=9; //pausa1
                     rutina[rutina[2]-18]=4; //pausaE
                     rutina[rutina[2]+1]+=0;
                     break;
                 }
            break;
      default:
            break;
      }
  
}


/////////////////////////////////////////////////

void preparaBasic (int t)
{

  *(unsigned short *)&BAS[4] = 23755 +trozos[t].pam_2;
  *(unsigned short *)&BAS[10]= 23755 + trozos[t].longitud+1;
  *(unsigned short *)&BAS[38]=  trozos[t].auto_run;
  memcpy (memoria + 23755 , trozos[t].data, trozos[t].longitud);
  memcpy (memoria + (23755+trozos[t].longitud), BAS, 60);
}

int juntabloques()
{
    memset (memoria , 0 , 64*1024 ) ;

    memcpy (memoria + DIR_SISTEM_VARS, SISTEM_VARS , 203 ) ;

    total_trozos=0;
    new_ini=0xffff;
    new_fin=0x0000;

     for (int t=0;t<10;t++)
       {
       if (trozos[t].tipo==-1)
          break;
       if (trozos[t].procesar)
          {
          if ((unsigned)trozos[t].inicio<new_ini)
                 new_ini=trozos[t].inicio;
          if ((unsigned)(trozos[t].inicio+trozos[t].longitud)>new_fin)
                 new_fin= trozos[t].inicio+trozos[t].longitud;

          if (dir_usr != 7997)
            if (trozos[t].dir_exe)
              dir_usr = trozos[t].dir_exe ;

          if (trozos[t].tipo=='0')
             {
             preparaBasic (t);
             new_fin+=60;
             }
          else
             memcpy (memoria + trozos[t].inicio, trozos[t].data, trozos[t].longitud);


          total_trozos++;
          }
       }
                    /*
    if (new_fin>_MAX_ADDRESS_)
         new_fin= _MAX_ADDRESS_;
                       */
    if (new_fin>0xff40)
         new_fin= 0xff40;    //4.4

    max=1+(new_fin-1>>8);

    if (max>0xff)
       max=0xff;



    return total_trozos;
}

void  multibloque()
{

   total_trozos=0;


    switch (metodo)
       {
       case  _MILKS_:
           strcpy(multiload_code,multi_milks);
           break;

       case  _MANCHESTER_:
       case  _MANCHESTER_DIF_:
       case  _MAESTOSO_:
           strcpy(multiload_code,multi_machester);
           break;

       default:
           strcpy(multiload_code,multi_slow);
           break;
       }

   for (int  t=0;t<10;t++)
       {
       if (trozos[t].tipo==-1)
          break;
       if (trozos[t].procesar)
          {
          if (dir_usr != 7997)
            if (trozos[t].dir_exe)
              dir_usr = trozos[t].dir_exe ;

          if (trozos[t].tipo=='0')
             preparaBasic (t);


          switch (metodo)
             {
             case  _MILKS_:
                if (trozos[t].inicio+trozos[t].longitud>_MAX_ADDRESS_)
                      trozos[t].longitud= _MAX_ADDRESS_-trozos[t].inicio;
                break;

             case  _MANCHESTER_:
             case  _MANCHESTER_DIF_:  
             case  _MAESTOSO_:
               multiload_code[2+8*total_trozos]=(trozos[t].inicio-1)&0xff;
               multiload_code[3+8*total_trozos]=(trozos[t].inicio-1)>>8;

               if (trozos[t].tipo=='0')
                  multiload_code[5+8*total_trozos]=
                     (1+(0xff|(trozos[t].inicio+trozos[t].longitud+60-1)))>>8;
               else
                 multiload_code[5+8*total_trozos]=
                   (1+(0xff|(trozos[t].inicio+trozos[t].longitud-1)))>>8;
               if (multiload_code[5+8*total_trozos]==0)
                   multiload_code[5+8*total_trozos]=0xff;
               break;

             default:
                if (trozos[t].inicio+trozos[t].longitud>_MAX_ADDRESS_)
                      trozos[t].longitud= _MAX_ADDRESS_-trozos[t].inicio;

               multiload_code[2+6*total_trozos]=(trozos[t].inicio-1)&0xff;
               multiload_code[3+6*total_trozos]=(trozos[t].inicio-1)>>8;
               break;
             }
          total_trozos++;
          }
       }

     switch (metodo)
             {
             case  _MILKS_:
                multiload_code[4+total_trozos]=0xc9;
                break;
             case  _MANCHESTER_:
             case  _MANCHESTER_DIF_: 
             case  _MAESTOSO_:
                multiload_code[1+8*total_trozos]=0xc9;
                break;
             default:
               multiload_code[1+6*total_trozos]=0xc9;
               break;
             }
}

///////////////////////////////////

   st_cargador_basic * cargador_tap;
   st_linea_basic * linea_basic;

void empiezaBasic ()
{
 //  unsigned n_size=0;

   cargador_tap= (st_cargador_basic *) BasicTap;


   cargador_tap->longitud_cabecera=19;
   cargador_tap->cabecera.flag=0;
   cargador_tap->cabecera.tipo=0;


   strncpy (cargador_tap->cabecera.nombre,prg_name,10);

   cargador_tap->flagff=0xff;
   linea_basic=&cargador_tap->linea;
   linea_basic->lineaA=0;
   linea_basic->lineaB=0;

   ;
   rutinaCarga();


   memset (linea_basic->data,0,300);

   sprintf (linea_basic->data,
         "\xfd\xB0\"%i\":\xf9\xc0\xB0\"23781\":\xea" ,dir_clear);

   memcpy (linea_basic->data+22,rutina+5,long_rutina);
   linea_basic->longitud=22+long_rutina ;

   pos_JP +=17 ; //19;
   pos_MAX+=17 ;
 }

 ///////

acabaBasic()
{
   linea_basic->data[linea_basic->longitud]=13;
   linea_basic->longitud++;

   cargador_tap->cabecera.longitud=4+linea_basic->longitud ;
   cargador_tap->cabecera.dir_ini=0;
   cargador_tap->cabecera.pam2=cargador_tap->cabecera.longitud;

   cargador_tap->longitub_data=2+cargador_tap->cabecera.longitud;// 1+n_size+1;

   if (cargador)
     {
     LoaderBlock (&(cargador_tap->cabecera.flag),cargador_tap->longitud_cabecera);
     LoaderBlock (&(cargador_tap->flagff),cargador_tap->longitub_data);
     }
   return 0;
}

void recolocacodigo128 ()
{
  if (metodo==_SHAVINGS_RAUDO_)
    {
    // acabada la cargade pagina seleccinamos puerto 7FFD y retornamos

    linea_basic->data[pos_JP-1]=0x01; //ld bc,n
    *(short *)(linea_basic->data+(pos_JP))=0x7ffd;
    linea_basic->data[pos_JP+2]=0xc9;

    // se reubica codigo
    linea_basic->data[27]=0xbd;
  /////////////////////////////////////
  //  if (muestras_por_bit==_1_75)
  /////////////////////////////////////

      for (int i=22;i<linea_basic->longitud;i++)
       switch (linea_basic->data[i])
         {
         case 0xff:
           if (linea_basic->data[i-1]==0x26)
             linea_basic->data[i]=0xbf;
           else
            switch (linea_basic->data[i-2])
               {
                     case 0x22: case 0x32:
                     case 0x21:case 0x11:case 0xc2:case 0xc3:
                     case 0xea:case 0xe2:case 0xd2:case 0xfa:
                     case 0xda:
                        linea_basic->data[i]=0xbf;break;
                     default:break;
               }
         break;

         case 0xfe:
           if (linea_basic->data[i-1]==0x26)
             linea_basic->data[i]=0xbe;
           else
             switch (linea_basic->data[i-2])
               {
                     
                     case 0x22: case 0x32:
                     case 0x21:case 0x11:case 0xc2:case 0xc3:
                     case 0xea:case 0xe2:case 0xd2:case 0xfa:
                     case 0xda:
                        linea_basic->data[i]=0xbe;break;
                     default:break;
               }
            break;
         case 0xfd:
            if (linea_basic->data[i-1]==0x26)
               linea_basic->data[i]=0xbd;
            else
               switch (linea_basic->data[i-2])
                 {
                     case 0x22: case 0x32:
                     case 0x21:case 0x11:case 0xc2:case 0xc3:
                     case 0xea:case 0xe2:case 0xd2:case 0xfa:
                     case 0xda:
                        linea_basic->data[i]=0xbd;break;
                     default:break;
                }
            break;  
         }
  /////////////////////////////////////
  /*  else
      for (int i=22;i<linea_basic->longitud;i++)
       switch (linea_basic->data[i])
         {
         case 0xff:
           if (linea_basic->data[i-1]==0x26)
             linea_basic->data[i]=0xbe;
           else
            switch (linea_basic->data[i-2])
               {
                     case 0x32:
                     case 0x21:case 0x11:case 0xc2:case 0xc3:
                     case 0xea:case 0xe2:case 0xd2:case 0xfa:
                     case 0xda:
                        linea_basic->data[i]=0xbe;break;
                     default:break;
               }
         break;

         case 0xfe:
            if (linea_basic->data[i-1]==0x26)
                linea_basic->data[i]=0xbd;
            else
               switch (linea_basic->data[i-2])
                 {
                     case 0x53:
                     case 0x22:
                     case 0x32:
                     case 0x21:case 0x11:case 0xc2:case 0xc3:
                     case 0xea:case 0xe2:case 0xd2:case 0xfa:
                     case 0xda:
                        linea_basic->data[i]=0xbd;break;
                     default:break;
                 }
            break;
         }   */
    /////////////////////////////////////
  

        // dir entrada pilot
     *(short *)&multi_128[74]=*(short *)&linea_basic->data[22+14];

     // dir entrada cargador 129
     *(short *)&linea_basic->data[22+14]=
                               23797 -38 +linea_basic->longitud;

     //dir origen para ldir
     *(short *)&multi_128[1]= 23797-38+22+ linea_basic->longitud;


      memcpy (linea_basic->data + linea_basic->longitud,
                     multi_128 ,76+4);

     linea_basic->data[linea_basic->longitud+4] += 20;
     linea_basic->data[linea_basic->longitud+20] += 20;
     linea_basic->data[linea_basic->longitud+25] += 20;
     linea_basic->data[linea_basic->longitud+30] += 20;
     linea_basic->data[linea_basic->longitud+35] += 20;
     linea_basic->data[linea_basic->longitud+40] += 20;
     linea_basic->data[linea_basic->longitud+45] += 20;
     linea_basic->data[linea_basic->longitud+50] += 20;
     linea_basic->data[linea_basic->longitud+55] += 20;

      linea_basic->longitud+= 4+76 ;

       }
    else
      {
      //tope primer bloque 0x4000 0xbf00
      linea_basic->data[pos_MAX]=0xbf;

      // acabada la cargade pagina seleccinamos puerto 7FFD y retornamos

      linea_basic->data[pos_JP-1]=0x01; //ld bc,n
      *(short *)(linea_basic->data+(pos_JP))=0x7ffd;
      linea_basic->data[pos_JP+2]=0xc9;

      // se reubica codigo

      for (int i=22;i<linea_basic->longitud;i++)
        if (linea_basic->data[i]==0xff)
          if (linea_basic->data[i-1]==0x26)
             linea_basic->data[i]=0xbf;
          else
             switch (linea_basic->data[i-2])
               {
                     case 0x32:
                     case 0x21:case 0x11:case 0xc2:case 0xc3:
                     case 0xea:case 0xe2:case 0xd2:case 0xfa:
                     case 0xda:
                     case 0xcd: // 4.0 for ultra_r
                        linea_basic->data[i]=0xbf;break;
                     default:break;
               }

        // dir entrada pilot
      *(short *)&multi_128[74]=*(short *)&linea_basic->data[22+14];

      // dir entrada cargador 129
      *(short *)&linea_basic->data[22+14]=
                               23797 -38 +linea_basic->longitud;

      //dir origen para ldir
      *(short *)&multi_128[1]= 23797-38+22+ linea_basic->longitud;

      // control de tope en siguentes paginas
      if (//(metodo==_ULTRA_)||
          (metodo==_MANCHESTER_)||(metodo==_MANCHESTER_DIF_)||
          (metodo==_MAESTOSO_))
            multi_128[65]=linea_basic->data[22+4]+pos_MAX-20-18;
      else
            multi_128[65]= 6;

       memcpy (linea_basic->data + linea_basic->longitud,
                     multi_128 ,76+4);

     if ((metodo==_NPU_) && (muestras_por_bit<=_1_75))
     {
     linea_basic->data[linea_basic->longitud+4] += 194;
     linea_basic->data[linea_basic->longitud+20] += 194;
     linea_basic->data[linea_basic->longitud+25] += 194;
     linea_basic->data[linea_basic->longitud+30] += 194;
     linea_basic->data[linea_basic->longitud+35] += 194;
     linea_basic->data[linea_basic->longitud+40] += 194;
     linea_basic->data[linea_basic->longitud+45] += 194;
     linea_basic->data[linea_basic->longitud+50] += 194;
     linea_basic->data[linea_basic->longitud+55] += 194;
     }
      linea_basic->longitud+= 4+76 ;
      }
}


int convierteHI (const char * out_file)
{
    int t,res;

    unsigned ini, largo;


    res=abreWav (out_file);

    if (res)
      {
      strcat (m_errors,"error opening output file\r\n");
      return res;
      }

    if (snap.tipo_snap)
      {
          if (snap.tipo_snap==1)
          {

             empiezaBasic ();

             linea_basic->data[pos_MAX]=0xff;

             linea_basic->data[pos_JP-1]=0x040; //no EI
             *(short *)(linea_basic->data+(pos_JP+1))=dir_usr;

             acabaBasic();

             preparamemoriasnap (metodo==_SHAVINGS_RAUDO_);

             pokea ();
             escalofridoWav (trozos[0].data,0x4000,trozos[0].longitud);
          }
          else
          {
             empiezaBasic ();

             recolocacodigo128 ();

             acabaBasic();

             preparamemoriasnap (metodo==_SHAVINGS_RAUDO_);

             pokea ();

             t= 0; 0x141;

             escalofridoWav (trozos[0].data, 0x4000 , trozos[0].longitud-t);

             for (t=1;t<7;t++)
                if (trozos[t].procesar)
                  escalofridoWav (trozos[t].data, 0xc000 ,trozos[t].longitud);

          }
      }
   else
    switch (esquema_bloques)
      {
      case _UNBLOQUE_ :

            juntabloques();

            empiezaBasic ();

            linea_basic->data[pos_MAX]=max;

            *(short *)(linea_basic->data+(22+11))=new_ini-1;

            if (dir_usr==0)
                linea_basic->data[pos_JP]=0xc9;
            else
                linea_basic->data[pos_JP]=0xc3;

            *(short *)(linea_basic->data+(pos_JP+1))=dir_usr;

            acabaBasic();

            pokea ();

            escalofridoWav (memoria + new_ini,new_ini,new_fin-new_ini);

            break;


      case _VARIOSBLOQUES_ :

            if (!bloques_de_snap) //4.4
                 multibloque();

            empiezaBasic ();

            if (dir_usr==0)
               *(short *)(linea_basic->data+(22+11))=8019;
            else
               *(short *)(linea_basic->data+(22+11))=dir_usr;

            linea_basic->data[22+14]=linea_basic->data[pos_JP+1];//(DIR_PILOT+N_PILOT+N_MILKS)&0xff;
            linea_basic->data[22+15]=0xff;//(DIR_PILOT+34+113)>>8;

            if (control_chksum)
                {
                linea_basic->data[pos_JP]=0xC8;
                linea_basic->data[pos_JP+1]=0xCF;
                linea_basic->data[pos_JP+2]=0x1A;
                }
            else
                linea_basic->data[pos_JP]=0xc9;

          if (bloques_de_snap)
          {
          linea_basic->data[pos_JP-1]=0x040; //no EI
          linea_basic->data[pos_JP]=0xc9;
          switch (metodo)
            {
             case  _MILKS_:  
                memcpy (linea_basic->data + linea_basic->longitud,
                        load_snap_blocks_milks,14);
                linea_basic->longitud+= 14;
                   break;
             case  _MANCHESTER_:
             case  _MANCHESTER_DIF_:
             case  _MAESTOSO_:   
                memcpy (linea_basic->data + linea_basic->longitud,
                        load_snap_blocks_manchester,24);
                linea_basic->longitud+= 24;
                   break;  
             default:
                memcpy (linea_basic->data + linea_basic->longitud,
                        load_snap_blocks,20);
                linea_basic->longitud+= 20;
                   break;
            }
          }
          else
            switch (metodo)
             {
             case  _MILKS_:
                 memcpy (linea_basic->data + linea_basic->longitud,
                        multiload_code ,4+total_trozos+1 );
                 linea_basic->longitud+= 4+total_trozos+1;
                 break;
             case  _MANCHESTER_:
             case  _MANCHESTER_DIF_:
             case  _MAESTOSO_:
                 memcpy (linea_basic->data + linea_basic->longitud,
                        multiload_code ,1+8*total_trozos+1);
                 linea_basic->longitud+= 1+8*total_trozos+1;
                 break;
             default:
                 memcpy (linea_basic->data + linea_basic->longitud,
                        multiload_code ,1+6*total_trozos+1);
                 linea_basic->longitud+= 1+6*total_trozos+1;
                 break;
             }

           acabaBasic();

           for (t=0;t<7;t++)
              {
              if (trozos[t].tipo==-1)
                 break;
              if (trozos[t].procesar)
                {
                if (trozos[t].tipo=='0')
                  {
                  ini=23755;
                  largo=60+trozos[t].longitud;
                  }
                else
                  {
                  ini=trozos[t].inicio  ;
                  largo=trozos[t].longitud ;
                  memcpy (memoria+ini,trozos[t].data,largo);
                  }
                pokea ();
                escalofridoWav (memoria+ini, ini , largo);
                };
              }
           break;

     case _CARGADOR_ORIGINAL_ :

            multibloque();

            empiezaBasic ();

            *(short *)(linea_basic->data+(22+11))= 23618;
            linea_basic->data[22+14]=linea_basic->data[pos_JP+1];//(DIR_PILOT+N_PILOT+N_MILKS)&0xff;
            linea_basic->data[22+15]=0xff;//(DIR_PILOT+34+113)>>8;

            if (control_chksum)
                {
                linea_basic->data[pos_JP]=0xC8;
                linea_basic->data[pos_JP+1]=0xCF;
                linea_basic->data[pos_JP+2]=0x1A;
                }
            else
                linea_basic->data[pos_JP]=0xc9;


            strcat (linea_basic->data,lprint_code);

            linea_basic->longitud=strlen (linea_basic->data);

            acabaBasic();

            for (t=0;t<10;t++)
              {
              if (trozos[t].tipo==-1)
                 break;

              pilot_digital (2500);

              if (trozos[t].procesar)
                   if (trozos[t].tipo=='0')
                      {
                      cambiaLOADCODE (memoria+23755 , trozos[t].longitud ) ;
                      escalofridoWav (memoria+23755 , trozos[t].inicio ,
                                      60+trozos[t].longitud);
                      }
                   else
                      escalofridoWav (trozos[t].data, trozos[t].inicio ,trozos[t].longitud);

              }

          break;
      }      

    return cierraWav ();

}


//---------------------------------------------------------------

unsigned char * filebuf=NULL;
unsigned n_size;

int TAP2WAV ()
{
  st_bloq_tap *bloq_tap;
  unsigned pos=0;//,t=0;
  int  error=0;

  while (pos<n_size)
      {
      bloq_tap= (st_bloq_tap *)(filebuf+pos);

      if (bloq_tap->flag==0)
          puretone (2168/_79_,8064);//Npilot=8064;
      else
          if (bloq_tap->flag==0xff)
             puretone (2168/_79_,3220);//Npilot=3220;
          else
             {
             error=-5;//error
             strcat (m_errors,"error .tap file format\r\n");
             break;
             }
      sync (667/_79_, 735/_79_);
//      puredata(855/_79_, 1710/_79_, &bloq_tap->flag ,bloq_tap->logitud_bloq);
      puredata(855/_79_, 1710/_79_, &bloq_tap->flag ,bloq_tap->logitud_bloq,8);
      pausa (1710/_79_,1000);
      pos+=  2 + bloq_tap->logitud_bloq;
      }
  return error;
}

//////////////////////////////////////////////////////////////
int TZX2WAV ()
{
  int error=0;
  unsigned pos;//,t=0;

  //st_head_tzx *tzx;
  st_block_10 *ID_10;
  st_block_11 *ID_11;
  st_block_12 *ID_12;
  st_block_13 *ID_13;
  st_block_14 *ID_14;
  st_block_15 *ID_15;

  //tzx= (st_head_tzx *)filebuf;
  pos =sizeof (st_head_tzx);
  while (pos<n_size)
     {
     switch (filebuf[pos])
         {
         case 0x10:
             ID_10= (st_block_10 *) (filebuf+pos) ;
             pos+= 5 + ID_10->long_block_tap;
             if (ID_10->flag==0)
                puretone (2168/_79_,8064);//Npilot=8064;
             else
                if (ID_10->flag==0xff)
                   puretone (2168/_79_,3220);// Npilot=3220;
                else
                   error=-5;
             sync (667/_79_, 735/_79_);
            // puredata(855/_79_, 1710/_79_, &ID_10->flag  ,ID_10->long_block_tap);
             puredata(855/_79_, 1710/_79_, &ID_10->flag  ,ID_10->long_block_tap,8);
             pausa (1710/_79_,ID_10->pause);
             break;

         case 0x11:
             ID_11= (st_block_11 *) (filebuf+pos) ;
             pos+= 19+ ID_11->long_1+0x10000*ID_11->long_2;
             puretone (ID_11->l_pilot/_79_,ID_11->n_pilot);// Npilot=3220;
             sync (ID_11->l_sync1/_79_, ID_11->l_sync2/_79_);
            // puredata(ID_11->l_zero/_79_, ID_11->l_one/_79_, ID_11->data ,ID_11->long_1+0x10000*ID_11->long_2);
             puredata(ID_11->l_zero/_79_, ID_11->l_one/_79_,
                      ID_11->data ,ID_11->long_1+0x10000*ID_11->long_2,ID_11->u_bits);

             pausa (ID_11->l_one/_79_,ID_11->pause);
             break;

         case 0x12:
             ID_12= (st_block_12 *) (filebuf+pos) ;
             puretone (ID_12-> ancho_pulso/_79_,ID_12->n_pulsos);
             pos+= 5;
             break;

         case 0x13:
             ID_13= (st_block_13 *) (filebuf+pos) ;
             secuence (ID_13->ancho_pulso ,ID_13->n_pulses);
             pos+= 2 + 2*ID_13->n_pulses;
             break;

         case 0x14:
             ID_14= (st_block_14 *) (filebuf+pos) ;
             //puredata(ID_14->zero_length/_79_,ID_14->one_length/_79_,
               //       ID_14->data, ID_14->long_1+0x10000*ID_14->long_2);
             puredata(ID_14->zero_length/_79_,ID_14->one_length/_79_,
                      ID_14->data, ID_14->long_1+0x10000*ID_14->long_2,ID_14->used_bits);
             pausa (  ID_14->zero_length/_79_ ,ID_14->pause);
             pos+= 11+ ID_14->long_1+0x10000*ID_14->long_2;
             break;

         case 0x15:
             ID_15= (st_block_15 *) (filebuf+pos) ;
             directrecording (ID_15->tstates_sample,ID_15->data,
                              ID_15->long_1+0x10000*ID_15->long_2);
             pausa ( 1500/_79_ ,ID_15->pause);
             pos+= 9+ ID_15->long_1+0x10000*ID_15->long_2;
             break;

         case 0x18:
         case 0x19:
             pos+=5+ *(unsigned *) (filebuf+(pos+1));
             strcat (m_warnings,"k7zx can't read 0x18 0x19 tzx blocks\r\n");
             break;
         case 'Z':
             pos+= 10;
             break;
         case 0x20:
             pos+= 3;
             break;
         case 0x21:
             pos+= 1+filebuf[pos+1]+1;
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
             pos+= 3 +(*(unsigned short *) (filebuf+(pos+1)))*2;
             break;
         case 0x27:
             pos+= 1;
             break;
         case 0x28:
             pos+= 3 +(*(unsigned short *) (filebuf+(pos+1)));
             break;
         case 0x2a:
             pos+= 5;
             break;
         case 0x2b:
             pos+= 6;
             break;
         case 0x30:
             pos+= 1+filebuf[pos+1]+1;
             break;
         case 0x31:
             pos+= 1+ filebuf[pos+2]+2;
             break;
         case 0x32:
             pos+= 1+filebuf[pos+1]+0x100*filebuf[pos+2]+2;
             break;
         case 0x33:
             pos+= 1+filebuf[pos+1]*3+1;
             break;
         case 0x34:
             pos+= 1+ 8;
             break;
         case 0x35:
             pos+= 1+ 14 + *((unsigned*)&(filebuf[pos+11]));
             break;
         case 0x40:
             pos+= 1+ filebuf[pos+2]+0x100*filebuf[pos+3]
                    + 0x10000*filebuf[pos+4] + 4;
             break;
         default:
              error=-5;
              strcat (m_errors,"error .tzx file format\r\n");
              pos=n_size;//pos++;
         }
    }

  pausa ( 1710/_79_ , 1000);

  return error;
}

int convierteNORMAL (const char * file_in,const char * file_out)
{
  FILE *f;
  int e;

  int res=0;

  m_errors [0]=0;
  m_warnings [0]=0;

  if (filebuf!=NULL)
      {
      free ( filebuf );
      filebuf=NULL;
      }
  f=fopen (file_in,"rb");
 // int anterior_basic=0;
  if (f!=NULL)
     {
     fseek(f, 0L, SEEK_END);
     n_size = ftell(f);
     fseek(f, 0, SEEK_SET	);
     filebuf = (unsigned char *) malloc(n_size +5 );
     if (filebuf!=NULL)
        {
        if (fread (filebuf,1,n_size,f)==n_size)
           {
           fclose (f);
           e=FqMuestreo;
           FqMuestreo=44100;
           res= abreWav(file_out);
           if (res==0)
            {
            if ((file_in[strlen (file_in)-1]|0x20)=='p')
               res=TAP2WAV ();
            else
               res=TZX2WAV ();
            cierraWav();
            }
           else
            {
            res=-4;
            strcat (m_errors,"error opening output file\r\n");
            }
          FqMuestreo=e;
          }
        else
          {
          res= -3;
          strcat (m_errors,"error reading input file\r\n");
          }
        }
     else
        {
        res = -2;
        strcat (m_errors,"not enough memory\r\n");
        }
     }
   else
     {
     res= -1;
     strcat (m_errors,"error opening input file\r\n");
     }
   return res;
}
