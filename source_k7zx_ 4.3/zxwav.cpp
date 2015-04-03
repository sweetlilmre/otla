
//---------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <math.h>


#pragma hdrstop

#include "zxfiles.h"  // headtzx
#include "zxwav.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


char metodo;
char muestras_por_bit;
               
int  FqMuestreo;
char forma_onda;
int  signo ;
char tono_final;

char CD = 1;
int  inv_r;
char acelerar_basic;
int  divisor=1;

int  multipicador=1;

char max_vol;

//---------------------------------------------------------------------------

FILE *fwav;

WavHdr headerWav;

#pragma pack(push, 1)
//---------------------------------------------------------------------------

#define _largo_mensaje_ 50
struct  {
   st_head_tzx head_tzx;
   struct {
      char ID_30;
      unsigned char largo_men;
      char men[_largo_mensaje_];
      } mensaje;
   st_block_15 direct_record;
   } tzx_file;


#pragma pack(pop)





char name [2560];


char emular=0;
int  n_bit=1;
int  escritos_tzx;


double tempos[5]={54.8, 18.6, 21.6, 43.3 ,0 };

st_stereo ss[512];
unsigned  char  mono [512];
int f_d_x[512];

double db=0x75;
float  duracion;
float  duracion_loader;

double inv_raiz[]={1,
                     1     , 0.7071, 0.5774, 0.5   , 0.4472,
                     0.4472, 0.4472, 0.4472, 0.4472, 0.4472,
                     0.4472, 0.4472, 0.4472, 0.4472, 0.4472,
                     0.4472, 0.4472, 0.4472, 0.4472, 0.4472,
                     0.4472, 0.4472, 0.4472, 0.4472, 0.4472,

                     0.4082, 0.378 , 0.3536, 0.3333, 0.3162,

                     0.3162, 0.3162, 0.3162, 0.3162, 0.3162,

                     0.3162, 0.3162, 0.3162, 0.3162, 0.3162,
                     0.3162, 0.3162, 0.3162, 0.3162, 0.3162,
                     0.3162, 0.3162, 0.3162, 0.3162, 0.3162,
                     0.3162, 0.3162, 0.3162, 0.3162, 0.3162,
                     0.3162, 0.3162, 0.3162, 0.3162, 0.3162,
                     0.3162, 0.3162, 0.3162, 0.3162, 0.3162,
                     0.3162, 0.3162, 0.3162, 0.3162};



int escribeWavNEW ( int * y, int n)
{
     int res=0;
     unsigned char byte;
     int  m;

     if (emular)
        {
        for (int i=0; i <n ; i++)
          {
          n_bit<<=1;
          if ((signo*y[i])>0)
               n_bit++;
          if (n_bit>0x100)
            {
            byte=n_bit&0xff;
            res += fwrite (&byte,1,1,fwav);
            n_bit=1;
            }
          }
        escritos_tzx+=res;
        }
      else
        if (CD)
           {
           for (int i=0;i<n;i++)
              {
              ss[i].l= 0x100*y[i]*(-signo);
              ss[i].r =0x100*y[i]*(-signo)*inv_r;
              }
           res= fwrite (ss,1,n*4,fwav);
           headerWav.dataChunkSize+=n*4;//res;
           }
        else
           {
           for (int i=0; i <n ; i++)
             {
             mono[i]=y[i]*(signo)+0x80;
             }
           res= fwrite (mono,1,n,fwav);
           headerWav.dataChunkSize+=n;//res;
           }

  return  res;
}



int abreWav (const char * file_out)
{
    int res =0;
    duracion=-1;
    db=115;

   fwav = fopen (file_out,"wb");


    if (fwav!=NULL)
      {
      duracion_loader=0;
      if (emular)
        {
        strcpy(tzx_file.head_tzx.zx_tape,"ZXTape!");
        tzx_file.head_tzx._1a=0x1a;
        tzx_file.head_tzx.Va=1;
        tzx_file.head_tzx.Vb=19;
        tzx_file.mensaje.ID_30= 0x30;
        tzx_file.mensaje.largo_men=_largo_mensaje_;
        strcpy  (tzx_file.mensaje.men,"francisco systems");
        sprintf (tzx_file.mensaje.men,"Block from k7zx encoding:%2.2i s/b:%2.2i",(int)metodo,(int)muestras_por_bit);
        tzx_file.direct_record.ID=0x15;

        tzx_file.direct_record.tstates_sample=0.5+3500000.0/((multipicador*FqMuestreo)/divisor);
        tzx_file.direct_record.pause=100;
        tzx_file.direct_record.used_bits=0;
        tzx_file.direct_record.long_1=0;
        tzx_file.direct_record.long_2=0;

        escritos_tzx=sizeof(tzx_file)-1;
        escritos_tzx=fwrite(&tzx_file,1,escritos_tzx,fwav);
        if (escritos_tzx!=sizeof(tzx_file)-1)
          res=E_ESCRIBIR;
        escritos_tzx=0;
        }
      else
        {
        headerWav.ChunkID=0x46464952;
        headerWav.ChunkSize=40;
        headerWav.Format=0x45564157;
        headerWav.fmtChunkID=0x20746d66;
        headerWav.fmtChunkSize=0x10;
        headerWav.AudioFormat=1;

        headerWav.SampleRate=multipicador*FqMuestreo/divisor;
        if (CD)
           {
           headerWav.BitsPerSample=16;
           headerWav.NumChannels=2;
           }
        else
           {
           headerWav.BitsPerSample=8;
           headerWav.NumChannels=1;
           }

        headerWav.dataChunkID=0x61746164;
        headerWav.dataChunkSize=0;

        headerWav.BlockAlign=headerWav.NumChannels * (headerWav.BitsPerSample / 8);
        headerWav.ByteRate=headerWav.SampleRate * headerWav.BlockAlign;


        if (fwrite ((char*)&headerWav,1,sizeof (headerWav),fwav)!=sizeof (headerWav))
           res=E_ESCRIBIR;
        headerWav.dataChunkSize=0;
        }
      }
    else
        res=E_ABRIR;
    return res;
}
     
int ciclo_digital (int tt);
ciclo_analogico (double P);
int cierraWav ()
{
    int res=0;
    if (tono_final||CD||(metodo>=_ANDANTE_))
        {
        for (int t=0;t<1000;t++)
           {
           db=100000*db/(100000.0+t);
           ciclo_analogico (24);//ciclo_digital (24);
           }
        }

      if (emular)
        {
        fseek(fwav,0,SEEK_SET);
        tzx_file.direct_record.long_1=escritos_tzx&0xffff;
        tzx_file.direct_record.long_2=escritos_tzx>>16;
        duracion=(escritos_tzx*8.0)/((multipicador*FqMuestreo)/divisor);
        escritos_tzx= sizeof(tzx_file)-1;
        if (fwrite(&tzx_file,1,escritos_tzx,fwav)!=escritos_tzx)
             res=E_ESCRIBIR;
        }
      else
        {
        fseek(fwav,0,SEEK_SET);
        headerWav.ChunkSize = headerWav.dataChunkSize + 36;
        if (fwrite(&headerWav,1,44,fwav)!=44)
             res=E_ESCRIBIR;
        duracion=(headerWav.dataChunkSize/headerWav.BlockAlign)/((float)(multipicador* FqMuestreo)/divisor);
        }
        fclose (fwav);

     return res;
}




ciclo_digital (int t0, int t1)
{

   int i=0;
   int n_n=(multipicador*t0)/divisor ;
   int n_p=(multipicador*t1)/divisor;
   double amp,x,y;

   switch (forma_onda)
      {
      case  _CUADRADA_:
      default :
         for (;i<n_n;i++)
            f_d_x[i]=-113;
         for (;i<n_n+n_p;i++)
            f_d_x[i]= 113;
         break;
      case _RAMPA_:
         for (;i<n_n;i++)
            f_d_x[i]= -113+8*i;//*inv_raiz[n_n];
         for (;i<n_n+n_p;i++)
            f_d_x[i]=  113-8*(i-n_n);//*inv_raiz[n_p];
         break;
      case _CUBICA_:
          amp=480.0/(n_n*n_n);
         for ( i=-n_n;i<0; i++)
           {
           x=i+0.7;
           y=((amp)*(x)*(x+n_n));
           f_d_x[i+n_n]=y  ;
           }
         amp=(480.0)/(n_p*n_p);
         for ( ;i<n_p; i++)
           {
           x=i+0.3;
           y=((-amp)*(x)*(x-n_p));
           f_d_x[i+n_n]=y  ;
           }
          break;
      case _0_CONTINUA_:
         for (;i<n_n;i++)
            f_d_x[i]=-113/n_n;
         for (;i<n_n+n_p;i++)
            f_d_x[i]= 113/n_p;
         break;
      case _IGUAL_E_:
         for (;i<n_n;i++)
            f_d_x[i]=-113*inv_raiz[n_n];
         for (;i<n_n+n_p;i++)
            f_d_x[i]= 113*inv_raiz[n_p];
         break;
      case _PARABOLA_:
          amp=(490.0*inv_raiz[n_n])/(n_n*n_n);
         for ( i=-n_n;i<0; i++)
           {
           x=i+0.6;
           y=((amp)*(x)*(x+n_n));
           f_d_x[i+n_n]=y  ;
           }
         amp=(490.0*inv_raiz[n_p])/(n_p*n_p);
         for ( ;i<n_p; i++)
           {
           x=i+0.4;
           y=((-amp)*(x)*(x-n_p));
           f_d_x[i+n_n]=y  ;
           }
          break;
      }

   return  escribeWavNEW (f_d_x, n_n+n_p) ;
}


int ciclo_digital (int tt)
{
     int j=0;

      float y,x;
      float T=(multipicador*tt)/divisor;
      float amp;//= -20*(128 -min);

     int T0= ((multipicador*tt)/divisor)/2;
     int T1= ((multipicador*tt)/divisor)-T0;

     switch (forma_onda)
       {
       
      case _CUADRADA_:
      default:
        for (j=0;j<T0;j++)
              f_d_x[j]=-113;
        for (   ;j<T0+T1;j++)
              f_d_x[j]= 113;
          break;
       case _RAMPA_:

         amp=600*inv_raiz[T0+T1]/T;
         for ( j=0;j<T; j++)
           {
           x=j+0.5;
           y=amp*(x-T/2.0);
           f_d_x[j]=y  ;
           }
          break;
          
       case _CUBICA_:
         amp=-2260;
         for ( j=0;j<T; j++)
           {
           x=j+1;
            y=((amp)*(x-0.5)*(x-T-0.5)*(x-T/2.0-0.5))/(T*T*T);
           f_d_x[j]=y  ;
           }

          break;  
      case _0_CONTINUA_:
        for (j=0;j<T0;j++)
              f_d_x[j]=-113/T0;
        for (   ;j<T0+T1;j++)
              f_d_x[j]= 113/T1;
          break;

      case _IGUAL_E_:
        for (j=0;j<T0;j++)
              f_d_x[j]=-113*inv_raiz[T0];
        for (   ;j<T0+T1;j++)
               f_d_x[j]= 113*inv_raiz[T1];
          break;

      case _PARABOLA_:
          amp=(490.0*inv_raiz[T0])/(T0*T0);
         for ( j=-T0;j<0; j++)
           {
           x=j+0.6;
           y=((amp)*(x)*(x+T0));
           f_d_x[j+T0]=y  ;
           }
         amp=(490.0*inv_raiz[T1])/(T1*T1);
         for ( ;j<T1; j++)
           {
           x=j+0.4;
           y=((-amp)*(x)*(x-T1));
           f_d_x[j+T0]=y  ;
           }
          break;

      case _DELTA_:
        f_d_x[0]=-113;       /*
        for (j=1;j<(T0+T1-1);j++)
               f_d_x[j]=-113;  */
        for ( j=1; j<T0+T1; j++)
               f_d_x[j]= 113;
          break;

      }
  return  escribeWavNEW (f_d_x, (multipicador*tt)/divisor) ;
}

double desfase=0;

int  ciclo_analogico (double P1, double P2)
{
    int T=0;
    int T1;
    double amp,x,y;//,F;

    amp= (db*20)/(8*P1*P1*P1);


    for ( x=desfase ; x<P1; x+=1)
          f_d_x[T++]= floor (0.5-amp*(x*(x-P1)*(x-2*P1)));

    desfase=T+desfase-P1;
    T1=T;

    amp= (db*20)/(8*P2*P2*P2);

    for ( x=desfase ; x<P2; x+=1)    
          f_d_x[T++]= floor (0.5-amp*(x*(x+P2)*(x-P2)));

    desfase=T-T1+desfase-P2;

  return  escribeWavNEW (f_d_x,T);
}
ciclo_analogico (double P)
{

    return ciclo_analogico (P/2,P/2);

}

alonepulse_analogico(double P1)
{
    int T=0;
  //  int T1;
    double amp,x,y;//,F;

    amp= (db*20)/(8*P1*P1*P1);

    for ( x=desfase ; x<P1; x+=1)
          f_d_x[T++]= floor (0.5-amp*(x*(x-P1)*(x-2*P1)));

    desfase=T+desfase-P1;
    //T1=T;
    T=escribeWavNEW (f_d_x,T);

    db=-db;

    return  T;
}

           
int tone_cicle [120];
int sync_cicle [120];   
int one_cicle  [120];
int zero_cicle [120];

void wave_table ( int * f , int T )
{
    int j=0;

    float amp;
    float y,x;
    f[0]= T;

    //amp=-1*0x78;

    amp=(-db*0x7a)/115 ;

    for ( j=0;j<T; j++)
    {
        x=j+1;
        y=-sin((x-T/2.0-0.45)*2*M_PI/T);
        if (y<0)
            f[1+j]=-amp*(sqrt (-y));
        else
            f[1+j]= amp*(sqrt (y));
   }
}



LoaderNormal (char *churro, unsigned longitud)
{

   unsigned char bit;
   char x=forma_onda;
   int n_pilot;
   forma_onda=2;
   db=3;

   /*
   tempos[0]=2*2168*FqMuestreo/3500000.0;
   tempos[1]=(667+735)*FqMuestreo/3500000.0;
   tempos[2]=2*855*FqMuestreo/3500000.0;
   tempos[3]=2*1710*FqMuestreo/3500000.0;


   tempos[0]=multipicador*tempos[0]/divisor;
   tempos[1]=multipicador*tempos[1]/divisor;
   tempos[2]=multipicador*tempos[2]/divisor;
   tempos[3]=multipicador*tempos[3]/divisor;

    */


   db=115; 
   wave_table(tone_cicle,(0.5+multipicador*2*2168*FqMuestreo/3500000.0)/divisor);
   wave_table(sync_cicle,(0.5+multipicador*(667+735)*FqMuestreo/3500000.0)/divisor);
   wave_table(zero_cicle,(0.5+multipicador*2*855*FqMuestreo/3500000.0)/divisor);
   wave_table(one_cicle,(0.5+multipicador*2*1710*FqMuestreo/3500000.0)/divisor);

   churro[longitud-1]=0;
   for (unsigned i=0;i<longitud-1;i++)
      churro[longitud-1]^= churro[i];

   if (churro[0])
           n_pilot=  1500 ;
   else
           n_pilot=  2500 ;
   db=1;
   for (int i=1;i<n_pilot;i++)
      {
      if (db<115)
          {
          db=i*i/15;
          if (db>115) db=115;
          wave_table(tone_cicle,(0.5+multipicador*2*2168*FqMuestreo/3500000.0)/divisor);
          }


      //ciclo_analogico (tempos[0]) ;
      escribeWavNEW (tone_cicle+1,tone_cicle[0]);
      }

   db=115;
   //ciclo_analogico (tempos[1]) ;
   escribeWavNEW (sync_cicle+1,sync_cicle[0]);


   for (unsigned  i=0; i< longitud;i++)
       {
       bit=0x80;
       while (bit)
         {
         if (churro[i]&bit)
             //ciclo_analogico (tempos[3]) ;
             escribeWavNEW (one_cicle+1,one_cicle[0]);
         else
             //ciclo_analogico (tempos[2]) ;
             escribeWavNEW (zero_cicle+1,zero_cicle[0]);
         bit=bit>>1;
         }
       }

   for (int i=0;i<16;i++)
      {
       db=16*db/(16.0+i);
       //ciclo_analogico (tempos[2]) ;
       wave_table(zero_cicle,(0.5+multipicador*2*2168*FqMuestreo/3500000.0)/divisor);
       escribeWavNEW (zero_cicle+1,zero_cicle[0]);
      }

   forma_onda=x;
   return 0;
}


LoaderAcelerado (char *churro, unsigned longitud)
{

   unsigned char bit;
   char x=forma_onda;
   int n_pilot;
   forma_onda=0;

   db=115;

   if (FqMuestreo==44100)
      {
       tempos[0]=24;
       tempos[1]=10;
       tempos[2]=8;
       tempos[3]=18;
      }
   else
      {
       tempos[0]=26;
       tempos[1]=12;
       tempos[2]=8;
       tempos[3]=20;
      }

   tempos[0]=multipicador*tempos[0]/divisor;
   tempos[1]=multipicador*tempos[1]/divisor;
   tempos[2]=multipicador*tempos[2]/divisor;
   tempos[3]=multipicador*tempos[3]/divisor;

   churro[longitud-1]=0;
   for (unsigned i=0;i<longitud-1;i++)
      churro[longitud-1]^= churro[i];


   n_pilot=  1255;


   for (int i=0;i<n_pilot;i++)
      ciclo_digital (tempos[0],tempos[0]) ;

   ciclo_digital (tempos[1],tempos[1]) ;


   for (unsigned  i=0; i< longitud;i++)
       {
       bit=0x80;
       while (bit)
         {
         if (churro[i]&bit)
             ciclo_digital(tempos[3],tempos[3]);//cicloAcelerado (tempos[3]) ;
         else
             ciclo_digital(tempos[2],tempos[2]);//cicloAcelerado (tempos[2]) ;
         bit=bit>>1;
         }
       }

   for (int i=0;i<25;i++)
      ciclo_digital (tempos[2],tempos[2]) ;


   forma_onda=x;
   return 0;
}

void LoaderBlock (char *churro, unsigned longitud)
{
   if (acelerar_basic)
       LoaderAcelerado (churro,longitud);
   else
       LoaderNormal (churro,longitud);
   if (emular)
      duracion_loader=(escritos_tzx*8.0)/((multipicador*FqMuestreo)/divisor);
   else
      duracion_loader=(headerWav.dataChunkSize/headerWav.BlockAlign)/((float)(multipicador* FqMuestreo)/divisor);

}
/////////////////////////////////////////////////////+

unsigned char checksum (unsigned char * s, int longitud)
{
   unsigned char res=0;
   for (int ii=0;ii<longitud;ii++)
                     res^=s[ii];
   return res;
}
 ////////////////////////////////////////////////////////////////////////////



int Bloque2ROM (char *churro , unsigned  inicio, unsigned  longitud)
{
    unsigned  x;
    unsigned char mibyte;
    char bits_count;
    char bit;


    if (muestras_por_bit==_2_50)
       {
       ciclo_digital (2,2);
       ciclo_digital (2,2);
       ciclo_digital (1,2);
       for (x=0;x<longitud;x++)
          {
          mibyte= churro[x];
          bits_count=8;
          while (bits_count)
            {
            ciclo_digital (1,1+ (mibyte>127));
            mibyte<<=1;
            bits_count--;
            } // while byte
          } // for trama
       }
    else
       {
       ciclo_digital (4);
       ciclo_digital (4);
       ciclo_digital (4);
       for (x=0;x<longitud;x++)
          {
          mibyte= churro[x];
          bits_count=8;
          while (bits_count)
            {
            ciclo_digital (2*(1+ (mibyte>127)));
            mibyte<<=1;
            bits_count--;
            } // while byte
          } // for trama
       }

    ciclo_digital (8);
    ciclo_digital (8);
    ciclo_digital (16);

    return 0;//escritos;
}



    //////////////////////////////////////////////////////////////////////////

int bloque2ThrwMlks ( char * churro ,unsigned short inicio,unsigned short l )
{
     unsigned short x;
     unsigned short ii;
     int t_valle,t_cresta;
     char dos_bits_count;
     unsigned char chk_sum=0;
     unsigned char mibyte;
     unsigned short longitud;
//     unsigned pas= inicio+l;

     longitud=l;

     ciclo_digital (3,3);

     for (x=0;x<longitud+6;x++)
         {
         switch (x)
             {
             case 0:
                 mibyte=(inicio)>>8;
                 break;
             case 1:
                 mibyte=(inicio)&0xff;
                 break;
             case 2:
                 mibyte=(longitud+0xff)>>8;
                 break;
             case 3:
                 mibyte=(longitud+0xff)&0xff;
                 break;
             case 4:
                 mibyte=chk_sum;
                 break;
             case 5:
                 mibyte=checksum (churro,longitud);
                     break;
             default:
                 mibyte=churro[(x-6)];
             }

         chk_sum^=mibyte;
         dos_bits_count=4;
         while (dos_bits_count)
            {
            t_cresta= 2 + ((mibyte&0xc0)>>6);
            if (x<6)
               t_valle=t_cresta;
            else
               switch (muestras_por_bit)
                  {
                  case _2_75:
                      t_valle=2;
                      break;
                  case _2_50:
                      if (t_cresta == 5)
                         t_cresta=1;
                      t_valle=t_cresta;
                      break;
                  case _2_25:
                      if (t_cresta == 5)
                         t_cresta=1;
                      t_valle=2;
                      break;
                  case _1_75:
                      if (t_cresta == 5)
                         t_cresta=1;
                      t_valle=1;
                      break;
                  default :
                  case _3_50:
                       t_valle=t_cresta;
                       break;
                  }

            ciclo_digital ( t_valle, t_cresta);
            mibyte<<=2;
            dos_bits_count--;
            } // while byte
        } // for trama

     ciclo_digital ( t_valle, t_cresta);
     ciclo_digital ( 8);
     ciclo_digital ( 16);
     return 0;
}


//////////////////////////////////////////////////////////////////////////
Bloque2FSK  ( unsigned char*  churro  ,  unsigned inicio, unsigned longitud)
{
  unsigned x;
  unsigned char mask,mybyte;
  int cero=2;
  int uno=4;

  ciclo_digital (4,2);

  switch (muestras_por_bit )
     {
     case _8_00:       cero=5;uno=11;      break; //cero=4;uno=12;
     case _7_00:       cero=4;uno=10;      break;
     case _6_00:       cero=4;uno=8;       break;
     case _5_00:       cero=4;uno=6;       break;
     case _4_00:       cero=3;uno=5;       break;//cero=2;uno=6;
     case _3_00:       cero=2;uno=4;       break;
     case _2_50:       cero=2;uno=3;       break;
     }

  for (x=0;x<longitud+1;x++)
      {
      if (x==0)
           mybyte=checksum (churro,longitud);
      else
           mybyte= churro[x-1];
      mask=0x80;
      while (mask) //            for (int b=0;b<8;b++)
         {
         if (mybyte&mask)
             ciclo_digital (uno );
         else
             ciclo_digital (cero );
         mask=mask>>1;
         }
      } // for trama


  ciclo_digital (8);
  ciclo_digital (16);
  ciclo_digital (24);
  ciclo_digital (32);
  ciclo_digital (40);
  ciclo_digital (32);
  return 0;
}


  
//////////////////////////////////////////////////////////////////////////

Bloque2ShavingsSlow ( char *churro , unsigned  inicio, unsigned l )
 {
  unsigned  short x;
  //int t_;
  unsigned char mibyte=0;
  unsigned short longitud;
  //unsigned tope= inicio+l;

  longitud=l;

  int pulsos[4];
  switch (muestras_por_bit )
    {
    case _4_00:
       pulsos[0]=5;pulsos[1]=7;pulsos[2]=9;pulsos[3]=11; break;
    case _3_50:
       pulsos[0]=4;pulsos[1]=6;pulsos[2]=8;pulsos[3]=10; break;
    case _3_00:
       pulsos[0]=3;pulsos[1]=5;pulsos[2]=7;pulsos[3]=9;  break;
    case _2_50:
       pulsos[0]=2;pulsos[1]=4;pulsos[2]=6;pulsos[3]=8;  break;/*
    case _1_75:
       pulsos[0]=2;pulsos[1]=3;pulsos[2]=4;pulsos[3]=5;  break; */
    }


  ciclo_digital (6);

  for (x=0;x<longitud+1;x++)
      {
      if (x==0)
           mibyte=checksum (churro,longitud);
      else
           mibyte= churro[x-1];
      ciclo_digital (pulsos[(mibyte&0xc0)>>6]);  // 5+6=11  => 00
      ciclo_digital (pulsos[(mibyte&0x30)>>4]);  // 5+6=11  => 00
      ciclo_digital (pulsos[(mibyte&0x0c)>>2]);  // 5+6=11  => 00
      ciclo_digital (pulsos[(mibyte&0x03)   ]);  // 5+6=11  => 00
      } // for trama

        /*
     ciclo_digital (16);
     ciclo_digital (16);
     ciclo_digital (18);
     ciclo_digital (18);
                 */

  ciclo_digital (pulsos[3]+1);
  ciclo_digital (pulsos[3]+2);
  ciclo_digital (pulsos[3]+3);
  ciclo_digital (16);

  return 0;
 }

//////////////////////////////////////////////////////////////////////////

char p [5];


Bloque2ShavingsDelta ( char *churro , unsigned  inicio, unsigned l )
{
    unsigned  short x;
    unsigned char mibyte;
    char dos_bits_count;
    unsigned short longitud;

    longitud=l;

    ciclo_digital (6,2);

    ciclo_digital (2,2);
    if (muestras_por_bit==_1_75||muestras_por_bit==_2_50)
       ciclo_digital (2,1);
    else
       ciclo_digital (2,2);

    for (x=0;x<longitud;x++)
        {
        mibyte= churro[x];
        dos_bits_count=4;
        while (dos_bits_count)
           {
           switch (muestras_por_bit)
                {
                case _1_75:
                    ciclo_digital (p[(mibyte&0xc0)>>6]   , 1);   break;
                case _2_25:
                    ciclo_digital (p[(mibyte&0xc0)>>6]   , 2);   break;
                case _2_75:
                    ciclo_digital (p[(mibyte&0xc0)>>6] +1, 2);   break;
                case _3_00:
                    ciclo_digital (p[(mibyte&0xc0)>>6] *2-1, 2); break;
                case _3_50:
                    ciclo_digital (p[(mibyte&0xc0)>>6] *2  , 2); break;
                }
           mibyte<<=2;
           dos_bits_count--;
           } // while byte
        } // for trama

    mibyte=forma_onda;

    if (forma_onda==1)
        forma_onda=0;

    ciclo_digital (4,8);
    ciclo_digital (12);
    ciclo_digital (14);
    ciclo_digital (14);

    forma_onda=mibyte;

    return 0;
 }


//////////////////////////////////////////////////////////////////////////
Bloque2ShavingsRaudoOld ( unsigned char*  churro  ,  unsigned inicio, unsigned longitud)
{
  unsigned x;
  int ancho;
  int m;
  unsigned char mibyte;

  bool snp= (inicio==0xc000)&&(longitud==0x4000);

  //x=forma_onda;
 // forma_onda=0;
  ciclo_digital (3,2);//  ciclo_digital (5);
//  forma_onda=x;

  ciclo_digital (4);
                     /*
  if ( longitud > 0xfeb8-inicio)
       longitud = 0xfeb8-inicio;
                       */

  if (!snp)
     {
     if ( longitud > 0xfeff-inicio)
         longitud = 0xfeff-inicio;

     if (muestras_por_bit==_1_75)
       if (longitud > 0xfdbf-inicio)
           longitud = 0xfdbf-inicio;
     if (muestras_por_bit!=_1_75)
       if (longitud > 0xfeff-inicio)
           longitud = 0xfeff-inicio;
     }

  switch (muestras_por_bit)
    {
      case  _1_75:
    for (x=0;x<longitud;x++)
      {
      if (!snp)
        if (inicio+x==0xfebf)
            mibyte=0xdd;
        else
            if (inicio+x==0xfec0)
              mibyte=0xe9;
            else
              mibyte=churro[x] ;
      else
         mibyte=churro[x] ;

      ciclo_digital (((mibyte&0xc0)>>6)+2);
      ciclo_digital (((mibyte&0x30)>>4)+2);
      ciclo_digital (((mibyte&0x0c)>>2)+2);
      ciclo_digital (((mibyte&0x03))   +2);
      }

        ciclo_digital ( 7);
        ciclo_digital ( 12);
        ciclo_digital ( 12);

        break;

      case  _2_25:
    for (x=0;x<longitud;x++)
      {
      if (!snp)
        if (inicio+x==0xfebf)
          mibyte=0xdd;
        else
         if (inicio+x==0xfec0)
             mibyte=0xe9;
          else
             mibyte=churro[x] ;
      else
        mibyte=churro[x] ;

      ciclo_digital (((mibyte&0xc0)>>6)+3);
      ciclo_digital (((mibyte&0x30)>>4)+3);
      ciclo_digital (((mibyte&0x0c)>>2)+3);
      ciclo_digital (((mibyte&0x03))   +3);
      }
        ciclo_digital ( 8);
        ciclo_digital ( 12);
        ciclo_digital ( 12);
        break;

      case  _2_50:
    for (x=0;x<longitud;x++)
      {
    if (!snp)
      if (inicio+x==0xfebf)
          mibyte=0xdd;
      else
         if (inicio+x==0xfec0)
             mibyte=0xe9;
          else
             mibyte=churro[x] ;
     else
      mibyte=churro[x] ;

      ciclo_digital (((mibyte&0xc0)>>5)+2);
      ciclo_digital (((mibyte&0x30)>>3)+2);
      ciclo_digital (((mibyte&0x0c)>>1)+2);
      ciclo_digital (((mibyte&0x03) *2) +2);
      }

        ciclo_digital ( 12);
        ciclo_digital ( 12);
        ciclo_digital ( 12);
        break;

      case  _2_75:
    for (x=0;x<longitud;x++)
      {
    if (!snp)
      if (inicio+x==0xfebf)
          mibyte=0xdd;
      else
         if (inicio+x==0xfec0)
             mibyte=0xe9;
          else
             mibyte=churro[x] ;
    else
      mibyte=churro[x] ;
      ciclo_digital (((mibyte&0xc0)>>6)+4);
      ciclo_digital (((mibyte&0x30)>>4)+4);
      ciclo_digital (((mibyte&0x0c)>>2)+4);
      ciclo_digital (((mibyte&0x03))   +4);
      }

        ciclo_digital ( 12);
        ciclo_digital ( 12);
        ciclo_digital ( 12);
        break;
    }

    ciclo_digital ( 16);
    return 0;
}

//////////////////////////////////////////////////////////////////////////
Bloque2ShavingsRaudo ( unsigned char*  churro  ,  unsigned inicio, unsigned longitud)
{
  unsigned x;
  int ancho;
  int m;
  unsigned char mibyte;

  bool snp= (inicio==0xc000)&&(longitud==0x4000);

  ciclo_digital (3,2);//  ciclo_digital (5);

  ciclo_digital (4);

  if (!snp)
       if (longitud > 0xfdbf-inicio)
           longitud = 0xfdbf-inicio;

  for (x=0;x<longitud;x++)
     {
     if (!snp)
        if (inicio+x==0xfebf)
           mibyte=0xdd;
        else
           if (inicio+x==0xfec0)
              mibyte=0xe9;
           else
              mibyte=churro[x] ;
     else
        mibyte=churro[x] ;

     switch (muestras_por_bit)
       {
       case  _1_75:
           ciclo_digital (((mibyte&0xc0)>>6)+2);
           ciclo_digital (((mibyte&0x30)>>4)+2);
           ciclo_digital (((mibyte&0x0c)>>2)+2);
           ciclo_digital (((mibyte&0x03))   +2);
           break;
       case  _2_25:
           ciclo_digital (((mibyte&0xc0)>>6)+3);
           ciclo_digital (((mibyte&0x30)>>4)+3);
           ciclo_digital (((mibyte&0x0c)>>2)+3);
           ciclo_digital (((mibyte&0x03))   +3);
           break;
       case  _2_50:
           ciclo_digital (((mibyte&0xc0)>>5)+2);
           ciclo_digital (((mibyte&0x30)>>3)+2);
           ciclo_digital (((mibyte&0x0c)>>1)+2);
           ciclo_digital (((mibyte&0x03) *2) +2);
           break;
       case  _2_75:
           ciclo_digital (((mibyte&0xc0)>>6)+4);
           ciclo_digital (((mibyte&0x30)>>4)+4);
           ciclo_digital (((mibyte&0x0c)>>2)+4);
           ciclo_digital (((mibyte&0x03))   +4);
           break;
       }
     }

  ciclo_digital ( 3,4);
  ciclo_digital ( 12);
  ciclo_digital ( 12);
  ciclo_digital ( 16);

  return 0;
}

 //////////////////////////////////////////////////////////////////////////

Bloque2Ultra (  char *churro , unsigned  inicio, unsigned  longitud)
 {
  unsigned x;
  char cero=2;
  char uno=4;


  
  if ((muestras_por_bit==_1_50)||(muestras_por_bit==_2_50))  // 4.3 no hace falta para ultra lento
    if (!((inicio==0xc000)&&(longitud==0x4000)))
     if ( longitud > 0xfeff-inicio)
          longitud = 0xfeff-inicio;

  ciclo_digital (4,2);

  switch (muestras_por_bit)
     {
     case _1_50:      cero=1;      uno=2;      break;
     case _2_00:      cero=1;      uno=3;      break;
     case _2_50:      cero=2;      uno=3;      break;
     case _3_00:      cero=2;      uno=4;      break;
     }

  for (x=0;x<longitud;x++)
      {
      ciclo_digital (churro[x]&0x80?uno:cero, churro[x]&0x40?uno:cero);
      ciclo_digital (churro[x]&0x20?uno:cero, churro[x]&0x10?uno:cero);
      ciclo_digital (churro[x]&0x08?uno:cero, churro[x]&0x04?uno:cero);
      ciclo_digital (churro[x]&0x02?uno:cero, churro[x]&0x01?uno:cero);
      } // for trama

  if ((muestras_por_bit==_1_50)||(muestras_por_bit==_2_50))
    for (x=inicio+longitud;x<(0xff|(inicio+longitud-1));x++)
       for (int b=0;b<4;b++)
            ciclo_digital (2, 2);
  else
    {
    ciclo_digital (2,4);
    ciclo_digital (16);
    }

  ciclo_digital (16);
  ciclo_digital (16);

  return 0;
}
 //////////////////////////////////////////////////////////////////////////

int Bloque2NonPlusUltra ( char *churro , unsigned  inicio, unsigned  longitud)

{
    unsigned  x;
    unsigned char mibyte;


    if (muestras_por_bit==_2_50)
        ciclo_digital (4,2);
    else
        if (muestras_por_bit==_1_75)
            ciclo_digital (2,1);
        else
            ciclo_digital (2,1);

    for (x=0;x<longitud+1;x++)
       {
       if (x==0)
           mibyte=checksum (churro,longitud);
       else
           mibyte= churro[x-1];

       switch (muestras_por_bit)
         {
         case  _1_25:
             ciclo_digital ((1+((mibyte&0xc0)>>6)), (1+((mibyte&0x30)>>4)));
             ciclo_digital ((1+((mibyte&0x0c)>>2)), (1+( mibyte&0x03   )));
             break;
         case  _1_75:
             ciclo_digital ((2+((mibyte&0xc0)>>6)), (2+((mibyte&0x30)>>4)));
             ciclo_digital ((2+((mibyte&0x0c)>>2)), (2+( mibyte&0x03   )));
             break;
         case  _2_00:
             ciclo_digital (2*(1+((mibyte&0xc0)>>6))-1, 2*(1+((mibyte&0x30)>>4))-1);
             ciclo_digital (2*(1+((mibyte&0x0c)>>2))-1, 2*(1+( mibyte&0x03   ))-1);
             break;
         case  _2_50:
             ciclo_digital (2*(1+((mibyte&0xc0)>>6)), 2*(1+((mibyte&0x30)>>4)));
             ciclo_digital (2*(1+((mibyte&0x0c)>>2)), 2*(1+( mibyte&0x03   )));
             break;
         }
       } // for trama


    if (muestras_por_bit >=_2_00)
        {
        ciclo_digital (10,10);
        ciclo_digital (10,10);
        ciclo_digital (10,10);
        ciclo_digital (10,10);
        }
    else
        {
        ciclo_digital (7,7);
        ciclo_digital (7,7);
        ciclo_digital (16);
        ciclo_digital (16);
        }

  return 0;
}

//////////////////////////////////////////////////////////////////////////

Bloque2Fi (  char *churro , unsigned  inicio, unsigned  l)
 {
  unsigned x,longitud;
  char p0=4;
  char p1=2;
  char mybyte=0;

  longitud =l;

  switch ( muestras_por_bit)
     {
     case _8_00:           p0=5;p1=3;break;
     case _7_00:           p0=5;p1=2;break;
     case _6_00:           p0=4;p1=2;break;
     case _5_00:           p0=3;p1=2;break;
     case _4_00:           p0=3;p1=1;break;
     case _3_00:           p0=2;p1=1;break;
     }


  ciclo_digital (2,3);
 
  for (x=0;x<longitud+1;x++)
     {
     if (x==0)
        mybyte=checksum (churro,longitud);
     else
        mybyte= churro[x-1];

     ciclo_digital (mybyte&0x80?p0:p1,mybyte&0x80?p1:p0);
     ciclo_digital (mybyte&0x40?p0:p1,mybyte&0x40?p1:p0);
     ciclo_digital (mybyte&0x20?p0:p1,mybyte&0x20?p1:p0);
     ciclo_digital (mybyte&0x10?p0:p1,mybyte&0x10?p1:p0);
     ciclo_digital (mybyte&0x08?p0:p1,mybyte&0x08?p1:p0);
     ciclo_digital (mybyte&0x04?p0:p1,mybyte&0x04?p1:p0);
     ciclo_digital (mybyte&0x02?p0:p1,mybyte&0x02?p1:p0);
     ciclo_digital (mybyte&0x01?p0:p1,mybyte&0x01?p1:p0);
     } // for trama

  ciclo_digital (12);
  ciclo_digital (14);
  ciclo_digital (16);
  ciclo_digital (20);
  ciclo_digital (24);
  ciclo_digital (32);
  ciclo_digital (40);

  return 0;
}
 //////////////////////////////////////////////////////////////////////////
   /*
Bloque2PWM (  char *churro , unsigned  inicio, unsigned  longitud)
{
  unsigned x;
  int ancho;
  int m;

  ciclo_digital (4,2);

  if (muestras_por_bit==_2_50)
      ancho=4 ;
  else
      ancho=5;

  if (muestras_por_bit==_3_50)
      m=2 ;
  else
      m=1;

     for (x=0;x<longitud;x++)
      {
      ciclo_digital (ancho-((churro[x]&0xc0)>>6),m+((churro[x]&0xc0)>>6));
      ciclo_digital (ancho-((churro[x]&0x30)>>4),m+((churro[x]&0x30)>>4));
      ciclo_digital (ancho-((churro[x]&0x0c)>>2),m+((churro[x]&0x0c)>>2));
      ciclo_digital (ancho-( churro[x]&0x03   ), m+((churro[x]&0x03))    );
      } // for trama


   ciclo_digital (8);
   ciclo_digital (16);
   ciclo_digital (24);

  return 0;
}        */

 //////////////////////////////////////////////////////////////////////////

Bloque2FiQ (  char *churro , unsigned  inicio, unsigned  longitud)
 {
  unsigned x,bits_count;
  char p0a=2;
  char p0b=2;
  char p1a=3;
  char p1b=3;
  char mybyte=0;


  switch ( muestras_por_bit)
     {
     case _1_75:  p0a=1;p0b=2;p1a=1;p1b=3;break;
     case _2_00:  p0a=1;p0b=2;p1a=2;p1b=3;break;
     case _2_25:  p0a=1;p0b=3;p1a=2;p1b=3;break;
     case _2_50:  p0a=1;p0b=3;p1a=2;p1b=4;break;
     case _2_75:  p0a=2;p0b=3;p1a=2;p1b=4;break;
     case _3_00:  p0a=2;p0b=3;p1a=3;p1b=4;break;
     case _3_50:  p0a=2;p0b=4;p1a=3;p1b=5;break;
     }

  ciclo_digital (3,2);

  for (x=0;x<longitud;x++)
      {
      mybyte= churro[x];
      bits_count=4;
      while (bits_count--)
         {
         switch (mybyte&0xc0)
           {
           case 0x00:
              ciclo_digital (p0a,p0b);    break ;
           case 0x40 :
              ciclo_digital (p0b,p0a);    break  ;
           case 0x80  :
              ciclo_digital (p1a,p1b);    break   ;
           case 0xc0   :
              ciclo_digital (p1b,p1a);     break    ;

           }
         mybyte<<=2;
         }
      } // for trama

  ciclo_digital (8);
  ciclo_digital (16);
  ciclo_digital (24);

  return 0;
}

//////////////////////////////////////////////////////////////////////////

Bloque2Manchester (  char *churro , unsigned  inicio, unsigned  longitud)
 {
  unsigned x;
  char last_bit=0;
  char bit;
  unsigned char mibyte;
  unsigned char mask;
  int v=0;
  int c=0;
  int d=1;

  if (muestras_por_bit==_3_00)
     c=1;
  else
     if (muestras_por_bit==_4_00)
        d=2;

  ciclo_digital (4,2*d + c*2);

  for (x=inicio-1;x<(0xff|(inicio+longitud-1))+1+1;x++)
      {
      if (x==(inicio-1))
         mibyte=checksum (churro,(inicio+longitud)<0xff00?longitud:0xff00-inicio);
      else
         if (x<(inicio+longitud) )
              mibyte=churro[x-inicio];
         else
             mibyte=0;

      mask=0x80;
      while (mask)
        {
        if (mibyte&mask?1:0)
           if (last_bit)
               {ciclo_digital (d*(1+v+c) , d*1 ),v=0;}//ciclo_digital (1, 1);
           else
               v=1;
        else
           if (last_bit)
               {ciclo_digital (d*(1+v+c), d*(2+c));v=0;}
           else
               ciclo_digital (d*1, d*(1+c) );

        last_bit=mibyte&mask?1:0;
        mask=mask>>1;
        }
      } // for trama

   ciclo_digital (d*(2+c), d*(2+c)) ;
   ciclo_digital (8);
   ciclo_digital (16);
   return 0;
}

//////////////////////////////////////////////////////////////////////////

Bloque2ManchesterDIf (  char *churro , unsigned  inicio, unsigned  longitud)
 {
  unsigned x;
  char last_bit=0;
  char bit;
  unsigned char mibyte;
  unsigned char mask;
  int v=0;
  int c=0;
  int d=1;

  int cresat=1;

  if (muestras_por_bit==_3_00)
     c=1;
  else
    if (muestras_por_bit==_4_00)
      d=2;

  ciclo_digital (4,2*d+ c*2);

  last_bit=  0;

  for (x=inicio-1;x<(0xff|(inicio+longitud-1))+1+1;x++)
      {
      if (x==(inicio-1))
         mibyte=checksum (churro,(inicio+longitud)<0xff00?
                                   longitud:0xff00-inicio);
      else
         if (x<(inicio+longitud) )
             mibyte=churro[x-inicio];
         else
             mibyte=0;

      mask=0x80;
      while (mask)
        {
        if ((mibyte&mask?1:0)!=last_bit)
           {
           if  (cresat)
              ciclo_digital (d*(1+v) , d*(1+c) );
           else
              ciclo_digital (d*(1+v+c) , d*1 );
           v=0;
           }
        else
           {
           if (cresat)
               v=1;
           else
               {
               ciclo_digital (d*(1+v+c), d*(2+c) );
               v=0;
               }
           cresat=!cresat;
           }
        last_bit=mibyte&mask?1:0;
        mask=mask>>1;
        }
      } // for trama

   ciclo_digital (d*(2+c), d*(2+c)) ;
   ciclo_digital (8);
   ciclo_digital (16);

   return 0;
}
////////////////////////////////////////////////////////////////////////////

int Bloque2Escurrido (char *churro , unsigned  inicio, unsigned  longitud)
{
   unsigned  x;
   int t_,t_anterior;
   int seguidos;
   unsigned char mibyte;
   char dos_bits_count;

   int inc_cero=0;
   int inc_data=0;
   int escala_data=1;
   int escala_cero=1;

   if (muestras_por_bit==_2_50)
       escala_data=escala_cero=2;
   if (muestras_por_bit==_2_25)
       escala_data=2;
   if ((muestras_por_bit==_1_50)||(muestras_por_bit==_2_00)
       || (muestras_por_bit==_2_25))
       inc_cero=1;
   if ((muestras_por_bit==_2_00))
       inc_data=1;

   t_anterior=2 ;

   ciclo_digital (3*escala_data,(1+inc_cero)*escala_cero);
   ciclo_digital (2*escala_data,(1+inc_cero)*escala_cero);
   ciclo_digital (2*escala_data,(1+inc_cero)*escala_cero);

   seguidos=0;
   for (x=0;x<longitud;x++)
       {
       mibyte= churro[x];
       dos_bits_count=4;
       while (dos_bits_count)
         {
         t_=  (mibyte>>6);
         if (t_==0)
            {
            seguidos++;
            if (seguidos>2)         //if (seguidos>3)
               {
               //  ciclo_digital (t_anterior+(muestras_por_bit==_2_00),seguidos+(muestras_por_bit!=_1_33));
               ciclo_digital (escala_data*(t_anterior+inc_data),escala_cero*(seguidos+inc_cero));
               t_anterior=4 ;
               seguidos =0;
               }
            }
         else
            {
             //       ciclo_digital (t_anterior+(muestras_por_bit==_2_00),1+seguidos+(muestras_por_bit!=_1_33));
             ciclo_digital (escala_data*(t_anterior+inc_data),escala_cero*(1+seguidos+inc_cero));
             t_anterior=t_;
             seguidos=0;
             }
             mibyte<<=2;
             dos_bits_count--;
         } // while byte
       } // for trama

    //  ciclo_digital (t_anterior+(muestras_por_bit==_2_00),1+seguidos+(muestras_por_bit!=_1_33));
    ciclo_digital (escala_data*(t_anterior+inc_data),escala_cero*(1+seguidos+inc_cero));

    ciclo_digital (escala_data*(3+inc_data),escala_cero*(1+inc_cero));
     //ciclo_digital (escala_data*(6+inc_data),escala_cero*(1+inc_cero));
     //ciclo_digital (escala_data*(8+inc_data),escala_cero*(1+inc_cero));

     ciclo_digital (escala_data*(6+inc_data),escala_cero*(1+inc_cero));
     ciclo_digital (escala_data*(8+inc_data),escala_cero*(1+inc_cero));

     return 0;//escritos;
}


 ////////////////////////////////////////////////////////////////////////////

 int Bloque2Andante (char *churro , unsigned  inicio, unsigned  longitud)
{
   unsigned  x;
   unsigned char mibyte;
   char bits_count;
   char bit;
   double p0=(multipicador*333*FqMuestreo)/(3500000.0*divisor);
   double p1=(multipicador*542*FqMuestreo)/(3500000.0*divisor);

   p0=4;
   p1=8;

   p0=5;
   p1=7;

   ciclo_analogico (p1);
   ciclo_analogico (p1);
   ciclo_analogico (p0);


   for (x=0;x<longitud+1;x++)
      {
      if (x==0)
         mibyte=checksum (churro,longitud);
      else
         mibyte= churro[x-1];
      bits_count=8;
      while (bits_count)
        {
        if ((mibyte>127))
           {
           ciclo_analogico (p0);
           ciclo_analogico (p1);
           }
        else
           {
           ciclo_analogico (p1);
           ciclo_analogico (p0);
           }
        mibyte<<=1;
        bits_count--;
        } // while byte
      } // for trama

   ciclo_analogico (p1*2);
   ciclo_analogico (p1*3);
   ciclo_analogico (p1*4);
   ciclo_analogico (24);

   ciclo_analogico (32);
   ciclo_analogico (32);

   ciclo_analogico (40);
   ciclo_analogico (40);
   
   ciclo_analogico (32);
   ciclo_analogico (32);

   return 0;//escritos;
}
 
////////////////////////////////////////////////////////////////////////////

int Bloque2Agitatto (char *churro , unsigned  inicio, unsigned  longitud)
{
   unsigned  x;
   unsigned char mibyte;
   char bits_count;
   char bit;


   double p0=(multipicador*333*FqMuestreo)/(3500000.0*divisor);
   double p1=(multipicador*581*FqMuestreo)/(3500000.0*divisor);
   double p2=(multipicador*1077*FqMuestreo)/(3500000.0*divisor);

   p0=5;
   p1=9;
   p2=13;

   ciclo_analogico (p0);
   ciclo_analogico (p1);
   ciclo_analogico (p0);
   ciclo_analogico (p1);

   for (x=0;x<longitud+1;x++)
      {
      if (x==0)
         mibyte=checksum (churro,longitud);
      else
         mibyte= churro[x-1];
      bits_count=4;
      while (bits_count)
        {
        switch (mibyte&0xc0)
           {
           case 0x0:
               ciclo_analogico (p0);ciclo_analogico (p1);break;
           case 0x40:
               ciclo_analogico (p0);ciclo_analogico (p2);break;
           case 0x80:
               ciclo_analogico (p1);ciclo_analogico (p0);break;
           case 0xc0:
               ciclo_analogico (p2);ciclo_analogico (p0);break;
           }
        mibyte<<=2;
        bits_count--;
        } // while byte
      } // for trama

  ciclo_analogico (p1);
  ciclo_analogico (p2);
  ciclo_analogico (p2*2);
  ciclo_analogico (p2*2);
  ciclo_analogico (p2*2);
  ciclo_analogico (p2*2);

  return 0;//escritos;
}////////////////////////////////////////////////////////////////////////////

int Bloque2Scherzando (char *churro , unsigned  inicio, unsigned  longitud)
{
   unsigned  x;
   unsigned char mibyte;
   char bits_count;
   char bit;
   double p0;
   double p1;


   p0=2;
   p1=6;

   ciclo_analogico (p0,p1);
   ciclo_analogico (p1,p0);
   ciclo_analogico (p0,p1);
   ciclo_analogico (p1,p0);

   for (x=0;x<longitud+1;x++)
      {
      if (x==0)
         mibyte=checksum (churro,longitud);
      else
         mibyte= churro[x-1];
      bits_count=4;
      while (bits_count)
        {
        switch (mibyte&0xc0)
          {
          case 0x0:
               ciclo_analogico (p0,p1);ciclo_analogico (p1,p0);break;
          case 0x40:
               ciclo_analogico (p1,p1);ciclo_analogico (p0,p0);break;
          case 0x80:
               ciclo_analogico (p0,p0);ciclo_analogico (p1,p1);break;
          case 0xc0:
               ciclo_analogico (p1,p0);ciclo_analogico (p0,p1);break;
          }
        mibyte<<=2;
        bits_count--;
        } // while byte
      } // for trama

   ciclo_analogico (p1*2);
   ciclo_analogico (p1*3);
   ciclo_analogico (p1*4);

   return 0;//escritos;
}
 //////////////////////////////////////////////////////////////////////////

Bloque2Maestoso(  char *churro , unsigned  inicio, unsigned  longitud)
 {
  unsigned x;
  char last_bit=0;
  char bit;
  unsigned char mibyte;
  unsigned char mask;
  int v=0;

  int cresat=1;

  ciclo_digital (8,8);

  last_bit=  0;

  for (x=inicio-1;x<(0xff|(inicio+longitud-1))+1+1;x++)
      {
      if (x==(inicio-1))
           mibyte=checksum (churro,(inicio+longitud)<0xff00?
                                   longitud:0xff00-inicio);
      else
         if (x<(inicio+longitud) )
              mibyte=churro[x-inicio];
         else
             mibyte=0;

      mask=0x80;
      while (mask)
        {
        if ((mibyte&mask?1:0)!=last_bit)
           {
           if  (cresat)
              ciclo_digital (4*(1+v) , 4 );
           else
              ciclo_digital (4*(1+v) , 4);
           v=0;
           }
        else
           {
           if (cresat)
               v=1;
           else
               {
               ciclo_digital (4*(1+v), 4*2 );
               v=0;
               }
           cresat=!cresat;
           }
        last_bit=mibyte&mask?1:0;
        mask=mask>>1;
        }
     } // for trama

  ciclo_digital ( 16 );
  return 0;
}
//////////////////////////////////////////////////////////////////////////
Bloque2Vivace  ( unsigned char*  churro  ,  unsigned inicio, unsigned longitud)
{
  unsigned x;
  unsigned char mask,mybyte;


  ciclo_digital (4,2);

  for (x=0;x<longitud+1;x++)
     {
     if (x==0)
        mybyte=checksum (churro,longitud);
     else
        mybyte= churro[x-1];
     mask=0x80;
     while (mask)
         {
         if (mybyte&mask)
            ciclo_digital (8 );
         else
            ciclo_digital (4 );
         mask=mask>>1;
         }
     } // for trama

   ciclo_digital (8);
   ciclo_digital (16);
   ciclo_digital (24);
   ciclo_digital (32);
   ciclo_digital (36);
   ciclo_digital (40);
   ciclo_digital (36);
   return 0;
}

 ////////////////////////////////////////////////////////////////////////////

Bloque2Presto ( char *churro , unsigned  inicio, unsigned l )
{
   unsigned  short x;
   unsigned char mibyte;
   char dos_bits_count;
   unsigned short longitud;
  // unsigned tope= inicio+l-1;



   longitud=l;
  // tope= inicio+longitud;

   ciclo_digital (6);
   ciclo_digital (4);
   ciclo_digital (4);

   for (x=0;x<longitud+1;x++)
      {
      if (x==0)
         mibyte=checksum (churro,longitud);
      else
         mibyte= churro[x-1];
      ciclo_digital (4+3*((mibyte&0xc0)>>6) );
      ciclo_digital (4+3*((mibyte&0x30)>>4) );
      ciclo_digital (4+3*((mibyte&0x0c)>>2) );
      ciclo_digital (4+3*((mibyte&0x03)   ) );
      } // for trama


   ciclo_digital (12);
   ciclo_digital (24);
   ciclo_digital (28);

   return 0;
 }

#define _TPILOT_  1750
#define _TSYNC1_  365  //438
#define _TSYNC2_  292

pilotMaNonTroppo (int n)
{
    double ps1,ps2;//,F;
    double pi;

    desfase=0.5;
    pi= _TPILOT_*FqMuestreo/3500000.0;//1264*FqMuestreo/3500000;
    pi=multipicador*pi/divisor;  
    ps1= _TSYNC1_*FqMuestreo/3500000.0;//1264*FqMuestreo/3500000;
    ps1=multipicador*ps1/divisor;
    ps2= _TSYNC2_*FqMuestreo/3500000.0;//1264*FqMuestreo/3500000;
    ps2=multipicador*ps2/divisor;


    db=115;
    desfase=0.5;
    pi=24;
    ps1=5;
    ps2=4;
    for ( int i=0; i<  n  ;i++)
         ciclo_analogico (pi);
    ciclo_analogico (ps1,ps2);
    return 0;
}
//////////////////////////////////////////////////////////////////////////

pilot_digital (int n)
{
    db=115;
    for ( int i=0; i<  n  ;i++)
      if (_t_P1_==_t_P2_)
            ciclo_digital (_t_P1_+_t_P2_);
      else
            ciclo_digital (_t_P1_,_t_P2_);
    return 0;
}

//////////////////////////////////////////////////////////////////////////

escalofridoWav (char * t , unsigned short inicio , unsigned short longitud )
{
   int i;
   char x=forma_onda;

   if ((forma_onda==_RAMPA_) ||(forma_onda==_0_CONTINUA_)||(forma_onda==_DELTA_))
       forma_onda=_CUADRADA_;

   if (metodo>=_ANDANTE_)
      {
      pilotMaNonTroppo (900);
      }
   else
      {
      if (bloques_de_snap)
          pilot_digital (1305);
      else
          pilot_digital (305);
      ciclo_digital(_t_P1_,_t_P2_);
      ciclo_digital(_t_S1_,_t_S2_);
      }

   forma_onda=x;

   switch (metodo)
      {
      case _ROM_:
           Bloque2ROM ( t , inicio ,longitud) ;            break;
      case _MILKS_:
           bloque2ThrwMlks ( t , inicio , longitud) ;      break;
      case _FSK_:
           Bloque2FSK (  t ,   inicio,  longitud);         break;
      case _SHAVINGS_SLOW_:
           Bloque2ShavingsSlow ( t , inicio ,longitud) ;   break;
      case _SHAVINGS_DELTA_:
           Bloque2ShavingsDelta ( t , inicio ,longitud) ;  break;
      case _SHAVINGS_RAUDO_:
           Bloque2ShavingsRaudo (  t , inicio,  longitud); break;
      case _ULTRA_ :
           Bloque2Ultra ( t , inicio ,longitud) ;          break;
      case _NPU_:
           Bloque2NonPlusUltra ( t , inicio ,longitud) ;   break;
      case _FI_:
           Bloque2Fi (  t ,   inicio,  longitud);          break;
      case _FI_Q_:
           Bloque2FiQ(  t ,   inicio,  longitud);          break;
      case _MANCHESTER_:
           Bloque2Manchester (  t ,   inicio,  longitud);  break;
      case _MANCHESTER_DIF_:
           Bloque2ManchesterDIf (  t , inicio,  longitud); break;
      case _ESCURRIDO_:
           Bloque2Escurrido ( t , inicio ,longitud) ;      break;

      case _ANDANTE_:
           Bloque2Andante( t , inicio ,longitud) ;         break;
      case _AGITATTO_:
           Bloque2Agitatto( t , inicio ,longitud) ;        break;
      case _MAESTOSO_:
           Bloque2Maestoso( t , inicio ,longitud) ;        break;
      case _VIVACE_:
           Bloque2Vivace( t , inicio ,longitud) ;          break;
      case _SCHERZANDO_:
           Bloque2Scherzando( t , inicio ,longitud) ;      break;
      default:
      case _PRESTO_:
           Bloque2Presto( t , inicio ,longitud) ;          break;
      }

                      /*
      for ( i=4; i< 30  ;i+=2)
        {
           ciclo_digital(i);//(4);
           ciclo_digital(i);//(4);
           ciclo_digital(i);//(4);
           ciclo_digital(i);//(4);
         }          */
  return 0;
}


/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////


#define _79_  79.36508


puretone (double Ppilot , int Npilot)
{
  int i=0;
  if (Npilot&1)
      {
      alonepulse_analogico(Ppilot);
      i=1;
      }
  for (;i<Npilot;i+=2)
      ciclo_analogico(Ppilot,Ppilot);
  return 0;
}
sync (double Psync1, double Psync2)
{
   ciclo_analogico(Psync1,Psync2);
   return 0;
}
secuence( unsigned short  *s ,int n)
{
   int i=0;
   if (n&1)
      {
      alonepulse_analogico(s[0]/_79_);
      i=1;
      }
   for (; i<n;i+=2)
      ciclo_analogico(s[i]/_79_ ,s[i+1]/_79_);
   return 0;

}
/*
puredata(double Pcero, double Puno, char *d,int n)
{
 unsigned char b;

 for (int i=0; i<n;i++)
     {
     b=0x80;
     while (b)
         {
         if (b&d[i])
              ciclo_analogico(Puno,Puno);
         else
              ciclo_analogico(Pcero,Pcero);
         b=b>>1;
         }
     }
} */

puredata(double Pcero, double Puno, char *d,int n, int l_b)
{
 unsigned char b;
 int i;
 for ( i=0; i<n-1;i++)
     {
     b=0x80;
     while (b)
         {
         if (b&d[i])
              ciclo_analogico(Puno,Puno);
         else
              ciclo_analogico(Pcero,Pcero);
         b=b>>1;
         }
     }

  b=0x80;
  for (i=0;i<l_b;i++)
     {
     if (b&d[n-1])
           ciclo_analogico(Puno,Puno);
     else
           ciclo_analogico(Pcero,Pcero);
     b=b>>1;
     }
  return 0;
}

pausa ( double Pp , int pausa )
{
   int i;
   double m;
   if (pausa!=0)
     {  
     if (db<0)
         alonepulse_analogico (Pp);
     ciclo_analogico(Pp,Pp);
     m=db ;
     Pp=250;
     for (i=1;i<44100*pausa/(Pp*2000.0);i++)
      {
   //   db=300*db/(300.0+i);
        db=200*db/(200.0+i);
      if (db<1)
          db=1;
      ciclo_analogico(Pp,Pp);
      }
     db=m;
     }
   return 0;
}

directrecording (unsigned short tstates,unsigned char *data, int n)
{
   int i;
   bool firs_bit=(data[0]&0x80);
   int p1=0,p2=0;
   unsigned char mask;
   for (i=0;i<n;i++)
     {
     mask  = 0x80;
     while (mask)
       {
       if (firs_bit==!(data[i]&mask))
          p2++;
       else
         if (p2)
            {
            ciclo_analogico ((p1*tstates)/_79_,(p2*tstates)/_79_);
            p1=p2=0;
            }
         else
            p1++;
       mask=mask>>1;
       }
     }       
   if (p2)
       ciclo_analogico ((p1*tstates)/_79_,(p2*tstates)/_79_);
   return 0;
}
