
#include <stdio.h>
#include <mem.h>
#include <math.h>
#include "wav2.h"
#include "funciones.h"
          
typedef struct
{
    unsigned int ChunkID;
    unsigned int ChunkSize;
    unsigned int Format;
    unsigned int fmtChunkID;
    unsigned int fmtChunkSize;
    unsigned short AudioFormat;
    unsigned short NumChannels;
    unsigned int SampleRate;
    unsigned int ByteRate;
    unsigned short BlockAlign;
    unsigned short BitsPerSample;
    unsigned int dataChunkID;
    unsigned int dataChunkSize;
}
WavHdr;

WavHdr headerWav;

FILE *fwav;

int acelerar=0;

int invert_pol=-1;

int forma_onda=_CUADRADA_;

int freq_muestreo=44100;

char s_p_b=_3_50;



double inv_raiz[]={  1,
                     1     , 0.7071, 0.5774, 0.5   , 0.4472,
                     0.4472, 0.4472, 0.4472, 0.4472, 0.4472,
                     0.4472, 0.4472, 0.4472, 0.4472, 0.4472,
                     0.4472, 0.4472, 0.4472, 0.4472, 0.4472,
                     0.4472, 0.4472, 0.4472, 0.4472, 0.4472,

                     0.4472, 0.4472, 0.4472, 0.4472, 0.4472,
                     0.4472, 0.4472, 0.4472, 0.4472, 0.4472,
                     0.4472, 0.4472, 0.4472, 0.4472, 0.4472,
                     0.4472, 0.4472, 0.4472, 0.4472, 0.4472,
                     0.4472, 0.4472, 0.4472, 0.4472, 0.4472,

                     0.4472, 0.4472, 0.4472, 0.4472, 0.4472,
                     0.4472, 0.4472, 0.4472, 0.4472, 0.4472,

                  };

//////////////////////////////////////////////////////
//
//              TABLAS de  ciclos completos
//              Primer byte = longitud en muestras del ciclo (milisegundos= (muestras/freq_muestreo))
//              resto bytes = forma de onda
//
//////////////////////////////////////////////////////

unsigned char c_pilot_zx[56]={54}; //54     15
unsigned char c_sync_zx [20]={18}; //18,    29
unsigned char c_zero_zx [22]={20}; //20
unsigned char c_one_zx  [44]={42}; //42

unsigned char c_zero_cpc [18]={16}; //   15
unsigned char c_one_cpc  [32]={30}; //   29

unsigned char c_zero_msx [22]={20}; //  20
unsigned char c_one_msx  [22]={20}; //  20



unsigned char c_zero_81 [190]={115};
unsigned char c_one_81  [190]={187}; 

unsigned char c_pseudopause  [19]={18};

unsigned char c_pilot_velozwav  [19]={18};
unsigned char c_sync_velozwav   [12]={8};
unsigned char c_sync2_velozwav  [12]={8}; //1.2

unsigned char c_00_velozwav [14];
unsigned char c_01_velozwav [14];
unsigned char c_10_velozwav [14];
unsigned char c_11_velozwav [14];

unsigned char c_t1_velozwav [25];
unsigned char c_t2_velozwav [25];
unsigned char c_t3_velozwav [25];
unsigned char c_t4_velozwav [25];


unsigned char * symbol_velozwav[4]
={
       c_00_velozwav,
       c_01_velozwav,
       c_10_velozwav,
       c_11_velozwav
};

#define _ACELERA_81_ (acelerar?0.85:1)

void wave_81table (unsigned char * c)
{
    int j;
    int delta;

    float y,x;
                                          
    float amp=-invert_pol*0x78;
    float T= _ACELERA_81_*1050*freq_muestreo/3500000.0;


    int T1=( unsigned char)c[0];
    //float T0= 0.5 + T1 - (4689-525)*freq_muestreo/3500000.0;

    float T0=  T1 - _ACELERA_81_*(4689-525)*freq_muestreo/3500000.0;


#define _COS81_

#ifdef _COS81_
    for ( j=0;j<T0; j++)
        {
            x=j+1;
            y=-cos((x-T/2.0-0.5)*2*M_PI/T);
            if (y<0)
                c[1+j]=0x80- amp*(sqrt (-y));
            else
                c[1+j]=0x80+ amp*(sqrt (y));
        }
    for ( ;j<T1; j++)
        c[1+j]=c[j];
#else
    for ( j=0;j<T0; j++)
        {
            x=j+1;
            y=-sin((x-T/2.0-0.5)*2*M_PI/T);
            if (y<0)
                c[1+j]=0x80- amp*(sqrt (-y));
            else
                c[1+j]=0x80+ amp*(sqrt (y));
        }
    for ( ;j<T1; j++)
        c[1+j]=0x80;
#endif

}
#define  _MAX_VOL_    125 //121
void wave_table ( char * c)
{

    int j;
    int delta;

    float y,x;
    float T=c[0];
    float amp;

    int T0= c[0]/2;
    int T1= c[0]-T0;

    switch (forma_onda)
    {
    case _CUADRADA_:
    default:
        amp=_MAX_VOL_*invert_pol;
        for (j=0;j<T0;j++)
            c[1+j]=-amp;
        for (   ;j<T0+T1;j++)
            c[1+j]= amp;
        break;

    case _DELTA_ :
        amp=_MAX_VOL_*invert_pol;
        if (T==2)
            delta=1;
        else
            delta=2;
        for (j=0;j<T-delta;j++)
            c[1+j]=-amp;
        for (   ;j<T;j++)
            c[1+j]= amp;
        break;

    case _DELTA_2 :
        amp=_MAX_VOL_*invert_pol;
        if (T==2)
            delta=1;
        else
            delta=2;
        for (j=0;j<delta;j++)
            c[1+j]=-amp;
        for (   ;j<T;j++)
            c[1+j]= amp;
        break;

    case _IGUAL_E_:
        amp=_MAX_VOL_*invert_pol;
        for (j=0;j<T0;j++)
            c[1+j]=-amp*inv_raiz[T0];
        for (   ;j<T0+T1;j++)
            c[1+j]= amp*inv_raiz[T1];
        break;

    case _RAMPA_:
        //amp=invert_pol*240/(T-1);
        amp=invert_pol*_MAX_VOL_*2/(T-1);
        for ( j=0;j<T; j++)
        {
            x=j+0.5;
            y=amp*(x-T/2.0);
            c[1+j]=y  ;
        }
        break;

    case _CUBICA_:
        //amp=-invert_pol*2500 ;//2260;
        amp=-invert_pol*20.78*_MAX_VOL_ ;//2260;
        for ( j=0;j<T; j++)
        {
            x=j+1;
            y=((amp)*(x-0.5)*(x-T-0.5)*(x-T/2.0-0.5))/(T*T*T);
            c[1+j]=y  ;
        }
        break;

    case _HYP_:
        //amp=-invert_pol*0x78;
        amp=-invert_pol*_MAX_VOL_;
        for ( j=0;j<T; j++)
        {
            x=j+1;
            y=-sin((x-T/2.0-0.5)*2*M_PI/T);
            if (y<0)
                c[1+j]=-amp*(sqrt (-y));
            else
                c[1+j]= amp*(sqrt (y));
        }
        break;
    }
    for (j=1;j<=c[0];j++)
        c[j]=0x80+c[j];
}
//////////////////////////////////////////////////////
void make_wave_tables ()
{
    int fa;
    int inc ;

    if (s_p_b&1)
        inc =1;
    else
        inc =2;
    c_00_velozwav[0]=(s_p_b+1)/2-1-inc;

    if (s_p_b==_3_75||s_p_b==_4_25)
    {
        c_00_velozwav[0]-=3;
        inc =3;
    }

    c_01_velozwav[0]=c_00_velozwav[0]+inc;
    c_10_velozwav[0]=c_01_velozwav[0]+inc;
    c_11_velozwav[0]=c_10_velozwav[0]+inc;

    c_t1_velozwav[0]=c_11_velozwav[0]+2*(inc==3);
    c_t2_velozwav[0]=c_t1_velozwav[0]+2;
    c_t3_velozwav[0]=c_t2_velozwav[0]+1;
    c_t4_velozwav[0]=c_t3_velozwav[0]+1;


    fa= forma_onda;

    forma_onda= _HYP_;

    wave_table (c_pilot_zx);
    wave_table (c_sync_zx);
    wave_table (c_zero_zx);
    wave_table (c_one_zx);

    wave_table (c_zero_cpc);
    wave_table (c_one_cpc);

    wave_table (c_zero_msx);
    c_one_msx[0]=10;
    wave_table (c_one_msx);
    c_one_msx[0]=20;
    memcpy (c_one_msx+11,c_one_msx+1,10);

    
    c_zero_81[0]= 0.5+_ACELERA_81_*(8369)*freq_muestreo/3500000.0;
    c_one_81[0]=  0.5+_ACELERA_81_*(5250+8369)*freq_muestreo/3500000.0;
    wave_81table(c_zero_81);
    wave_81table(c_one_81);

    forma_onda=fa;

    if (forma_onda>=_DELTA_)
        forma_onda= _CUADRADA_;

    wave_table (c_pilot_velozwav);

    memcpy (c_pseudopause,c_pilot_velozwav,19) ;

    for (int k=1;k<=c_pseudopause[0];k++)
        c_pseudopause[k]=128.5+(c_pseudopause[k]-0x80)*0.80;

    wave_table (c_sync_velozwav);
    /*
    c_sync_velozwav [4]=c_sync_velozwav [3];
    c_sync_velozwav [3]=c_sync_velozwav [2];  //simetrico 1.2
     */
    c_sync2_velozwav[0]=c_10_velozwav[0];  //1.2
    wave_table (c_sync2_velozwav);

    forma_onda=fa;

    wave_table (c_00_velozwav);
    wave_table (c_01_velozwav);
    wave_table (c_10_velozwav);
    wave_table (c_11_velozwav);

    wave_table (c_t1_velozwav);
    wave_table (c_t2_velozwav);
    wave_table (c_t3_velozwav);
    wave_table (c_t4_velozwav);
}


#pragma pack(push, 1)
typedef struct
{
    char zx_tape[7];
    char _1a;
    char Va;
    char Vb;
}
st_head_tzx;


typedef struct
{
    char ID;
    unsigned short  tstates_sample;
    unsigned short  pause;
    unsigned char   used_bits;
    unsigned short  long_1;
    unsigned char   long_2;
    unsigned char   data [1];
}
st_block_15 ;



struct
{
    st_head_tzx head_tzx;
    struct
    {
        char ID_30;
        unsigned char largo_men;
        char men[50];
    }
    mensaje;
    st_block_15 direct_record;
}
tzx_file;

#pragma pack(pop)

int wav2tzx (const char * file_in, const char * file_out, const char * mens)
{
    int res=0;
    FILE * fwav = fopen (file_in,"rb");
    FILE * ftzx;
    if (fwav!=NULL)
    {
        ftzx = fopen (file_out,"wb");
        if (ftzx!=NULL)
        {
            fread(&headerWav,1,44,fwav) ;

            memcpy(tzx_file.head_tzx.zx_tape,"ZXTape!",7);
            tzx_file.head_tzx._1a=0x1a;
            tzx_file.head_tzx.Va=1;
            tzx_file.head_tzx.Vb=19;

            tzx_file.mensaje.ID_30= 0x30;
            tzx_file.mensaje.largo_men=50;
            memcpy(tzx_file.mensaje.men,mens,tzx_file.mensaje.largo_men);

            tzx_file.direct_record.ID=0x15;
            tzx_file.direct_record.tstates_sample=0.5+3500000.0/(freq_muestreo);
            tzx_file.direct_record.pause=100;
            tzx_file.direct_record.used_bits=0;
            tzx_file.direct_record.long_1=0;
            tzx_file.direct_record.long_2=0;

            int escritos_tzx=sizeof(tzx_file)-1;
            escritos_tzx=fwrite(&tzx_file,1,escritos_tzx,ftzx);
            if (escritos_tzx!=sizeof(tzx_file)-1)
                return -2;//res=E_ESCRIBIR;

            escritos_tzx=0;

            char sample;
            unsigned char data;
            int  n_bit=1;

            while (fread (&sample,1,1,fwav)==1)
            {
                n_bit<<=1;
                if (sample>0)
                    n_bit++;
                if (n_bit>0x100)
                {
                    data=n_bit&0xff;
                    fwrite (&data,1,1,ftzx);
                    n_bit=1;
                    escritos_tzx ++;
                }
            }

            fseek(ftzx,0,SEEK_SET);
            tzx_file.direct_record.long_1=escritos_tzx&0xffff;
            tzx_file.direct_record.long_2=escritos_tzx>>16;
            escritos_tzx= sizeof(tzx_file)-1;
            if (fwrite(&tzx_file,1,escritos_tzx,ftzx)!=escritos_tzx)
                return -2;//res=E_ESCRIBIR;
            fclose (ftzx) ;
        }
        else
            res=-1;
        fclose (fwav) ;
    } 
    else
        res=-1;
    return res;
}

double dura ()
{
    return (headerWav.dataChunkSize/headerWav.BlockAlign)/((double)( freq_muestreo));
}

int write_cicle (const unsigned char * ciclye)
{
    int res;

    res = fwrite (ciclye+1,1,ciclye[0],fwav);
    headerWav.dataChunkSize+=ciclye[0];

    return res;
}


int open_wav (const char * file_out)
{
    int res =0;

    make_wave_tables ();

    fwav = fopen (file_out,"wb");

    if (fwav!=NULL)
    {
        headerWav.ChunkID=0x46464952;
        headerWav.ChunkSize=40;
        headerWav.Format=0x45564157;
        headerWav.fmtChunkID=0x20746d66;
        headerWav.fmtChunkSize=0x10;
        headerWav.AudioFormat=1;

        headerWav.SampleRate=freq_muestreo; //multipicador*FqMuestreo/divisor;

        headerWav.BitsPerSample=8;
        headerWav.NumChannels=1;

        headerWav.dataChunkID=0x61746164;
        headerWav.dataChunkSize=0;

        headerWav.BlockAlign=headerWav.NumChannels * (headerWav.BitsPerSample / 8);
        headerWav.ByteRate=headerWav.SampleRate * headerWav.BlockAlign;


        if (fwrite ((char*)&headerWav,1,sizeof (headerWav),fwav)!=sizeof (headerWav))
            res=E_ESCRIBIR;
        headerWav.dataChunkSize=0;

    }
    else
        res=E_ABRIR;
    return res;
}


int close_wav ()
{
    int t,k;
    int res=0;
    unsigned char finale [32];
    double db=1;
    memcpy (finale,c_t3_velozwav,32);
    for ( t=0;t<1000;t++)
    {
        for (k=1;k<=finale[0];k++)
            finale[k]=128.5+(finale[k]-0x80)*db;
        db=100000*db/(100000.0+t);
        res = fwrite (finale+1,1,finale[0],fwav);
        headerWav.dataChunkSize+=finale[0];
    }

    fseek(fwav,0,SEEK_SET);
    headerWav.ChunkSize = headerWav.dataChunkSize + 36;
    if (fwrite(&headerWav,1,44,fwav)!=44)
        res=E_ESCRIBIR;

    fclose (fwav);

    fwav=NULL;

    return res;
}

//////////////////////////////////////////////////////
//   Bloque de cabecera/ datos a Wav según la norma ZX
//////////////////////////////////////////////////////

int byte2_zx_wav (unsigned char byte)
{
    int res;
    unsigned char mask=0x80;       //MSB first !!

    do
        if (mask&byte)
            res=write_cicle (c_one_zx);
        else
            res=write_cicle (c_zero_zx);
    while (mask>>=1);

    return res;
}

//////////////////////////////////////////////////////

int block2zx_wav  ( unsigned char * data , int length)
{
    int i,pulsos;
    unsigned char b;
    int res=0;

    if (acelerar)
        if (data[0]==0x00)
            pulsos=1200;
        else
            pulsos=1200;
    else
        if (data[0]==0x00)
            pulsos=3000;
        else
            pulsos=1600;

    for ( i=0;i<pulsos;i++)
        write_cicle (c_pilot_zx);       //tono guia

    write_cicle (c_sync_zx);               //syncronismo

    data [length-1]=0;
    for (i=0;i<length;i++)                  //data
    {
        byte2_zx_wav (data[i]);
        data [length-1]^= data[i];          //chksum
    }

    for (i=0;i<20;i++)                       //fin no standar
        write_cicle (c_zero_zx);

    return res;
}


//////////////////////////////////////////////////////
//   Bloque de cabecera/ datos ?? a Wav según la norma CPC
//////////////////////////////////////////////////////

int byte2_cpc_wav (unsigned char byte)
{
    int res;
    unsigned char mask=0x80;       //MSB first !!

    do
        if (mask&byte)
            res=write_cicle (c_one_cpc);
        else
            res=write_cicle (c_zero_cpc);
    while (mask>>=1);

    return res;
}

//////////////////////////////////////////////////////

int block2cpc_wav  (const unsigned char * data )
{
    int i,pulsos;
    unsigned char b;
    unsigned short CRC;
    int res=0;

    if (acelerar)
        pulsos= 333 ;//500 ;//999;
    else
        pulsos=2050;
        
    for (i=0;i<pulsos;i++)               //tono guia
        write_cicle (c_one_cpc);
    write_cicle (c_zero_cpc);           //syncronismo

    byte2_cpc_wav (data[0]);   //0x16 , 0x2c  //id bloque

    CRC = 0x0ffff;
    for (i=0;i<256;i++)              //data
    {
        byte2_cpc_wav (data[i+1]);
        CRC = CRCupdate(CRC, data[i+1]);
    }
    CRC = CRC^0x0ffff;

    byte2_cpc_wav (CRC>>8);          //crc
    byte2_cpc_wav (CRC&0xff);        //crc

    /*
    for (i=0;i<22;i++)
        write_cicle (c_one_cpc);       //trail */
    for (i=0;i<20;i++)
        write_cicle (c_zero_cpc);       //trail

    return res;
}


//////////////////////////////////////////////////////
//   Bloque de datos a Wav según la norma MSX

/*
memory dump (BSAVE/BLOAD)
  filedescriptor: long header
                  10x #D0
                  6 bytes filename

  file data:      short header
                  2 bytes start address
                  2 bytes end address
                  2 bytes exec addr
                  memory dump
//////////////////////////////////////////////////////*/

int byte2msx_wav (unsigned char byte)
{
    unsigned char mask=1;        //LSB first !!

    write_cicle (c_zero_msx);    //1 startbit

    do
        if (mask&byte)
            write_cicle (c_one_msx);
        else
            write_cicle (c_zero_msx);
    while ( mask<<=1 );

    write_cicle (c_one_msx);
    write_cicle (c_one_msx);    //2 stopbits

    return 0;
}

int block2msx_bin_wav  (const char * name ,const unsigned char * data , int length , unsigned char filetype=0xd0)
{
    int i,pulsos;
    unsigned char b;
    int res=0;

    if (acelerar)
        pulsos=1222;
    else
        pulsos=8000;

    for (i=0;i<pulsos;i++)
        write_cicle (c_one_msx);                   //Long pilot 2400bps

    for (i=0;i<10;i++)
        byte2msx_wav (filetype);//(0xd0);  //(binario msx);    // filetipe

    for (i=0;i<6;i++)
        byte2msx_wav (name[i]);                    //name

    write_cicle (c_zero_msx);
    for (i=0;i<500;i++)
        write_cicle (c_one_msx);   //silencio (falso no standard) 1 segundo ????

    if (acelerar)
        pulsos=1100;
    else
        pulsos=2000;
    for (i=0;i<pulsos;i++)
        write_cicle (c_one_msx);                //short pilot 2400bps

    for (i=0;i<length;i++)
        byte2msx_wav (data [i]);               // data [0]...[1] =  start address
    // data [2]...[3] =  end address
    // data [4]...[5] =  exec address
    // data [6]...[length-1] memori_DUMP


    for (i=0;i<10;i++)
        write_cicle (c_one_msx);               //final no standard

    return res;
}


//////////////////////////////////////////////////////
//   Bloque de cabecera/ datos a Wav según la norma ZX
//////////////////////////////////////////////////////

int byte2_81_wav (unsigned char byte)
{
    int res;
    unsigned char mask=0x80;       //MSB first !!

    do
        if (mask&byte)
            res=write_cicle (c_one_81);
        else
            res=write_cicle (c_zero_81);
    while (mask>>=1);

    return res;
}

//////////////////////////////////////////////////////

int block281_wav  ( const unsigned char * data , int length)
{
    int i,pulsos;
    unsigned char b;
    int res=0; 
    unsigned char silencio [22];
    unsigned char trans [3];

    silencio[0]=20 ;
/*
    for (i=0;i<20;i++)
        silencio[i+1]=0x80;
    for (i=0;i<100;i++)
        write_cicle (silencio);

    trans[0]=3 ;
    trans[1]=(0x100+  c_zero_81[c_zero_81[0]-1])/3;
    trans[2]=(0x80 +2*c_zero_81[c_zero_81[0]-1])/3;
    trans[3]=         c_zero_81[c_zero_81[0]-1];
    write_cicle (trans);
*/

    for (i=0;i<20;i++)
        silencio[i+1]=c_zero_81[c_zero_81[0]-1];
    for (i=0;i<100;i++)
        write_cicle (silencio);

    for (i=0;i<length;i++)                  //data
        byte2_81_wav (data[i]);


    for (i=0;i<20;i++)
        silencio[i+1]=c_zero_81[c_zero_81[0]-1];
    for (i=0;i<50;i++)
        write_cicle (silencio);
/*
    trans[0]=3 ;
    trans[1]=         c_zero_81[c_zero_81[0]-1];
    trans[2]=(0x80 +2*c_zero_81[c_zero_81[0]-1])/3;
    trans[3]=(0x100+  c_zero_81[c_zero_81[0]-1])/3;
    write_cicle (trans);

    for (i=0;i<20;i++)
        silencio[i+1]=0x80;
    for (i=0;i<50;i++)
        write_cicle (silencio);
*/
    return res;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
///     datos a velozloader
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

int pseudopause (int ms)
{
    int res;
    int i,n=(freq_muestreo*ms)/(18000) ;
    for (i=0;i<n;i++)
        res=write_cicle (c_pseudopause);
    return res;
}

pilot_veloz (int n)
{
    int res;
    int i;
    for (i=0;i<n;i++)
        res=write_cicle (c_pilot_velozwav);
    return res;
}

int block2veloz_wav (const unsigned char * data , int length)
{
    int i;
    int res=0;
    unsigned char mibyte;

    pilot_veloz (25);
    write_cicle (c_sync_velozwav);
    write_cicle (c_sync2_velozwav);

    for (i=-1;i<length;i++)
    {
        if (i==-1)
            mibyte=checksum (data,length);
        else
            mibyte= data[i];
        write_cicle (symbol_velozwav[(mibyte&0xc0)>>6]);  // 5+6=11  => 00
        write_cicle (symbol_velozwav[(mibyte&0x30)>>4]);  // 5+6=11  => 00
        write_cicle (symbol_velozwav[(mibyte&0x0c)>>2]);  // 5+6=11  => 00
        write_cicle (symbol_velozwav[(mibyte&0x03)   ]);  // 5+6=11  => 00
    }

    write_cicle (c_t1_velozwav);
    write_cicle (c_t2_velozwav);
    write_cicle (c_t3_velozwav);
    write_cicle (c_t3_velozwav);

    return res;
}

//////////////////////////////////////////////////////
//   velozloader para zx81
//////////////////////////////////////////////////////

int byte2_zx81fast (unsigned char byte)
{
    int res;
    unsigned char mask=0x80;       //MSB first !!

    do
        if (mask&byte)
            res=write_cicle (symbol_velozwav[2]);
        else
            res=write_cicle (symbol_velozwav[0]);
    while (mask>>=1);

    return res;
}

int block2veloz81_wav (const unsigned char * data , int length)
{
    int i;
    int res=0;

    for (i=0;i<4;i++)
        byte2_zx81fast (0xff);//(0xcc);

    byte2_zx81fast (0xf0);
    /*
    byte2_zx81fast (0x00);//
    write_cicle (c_t1_velozwav); */
    byte2_zx81fast (0x01);

    for (i=0;i<length;i++)
        byte2_zx81fast (data[i]);


    write_cicle (c_t1_velozwav);
    write_cicle (c_t2_velozwav);
    write_cicle (c_t3_velozwav);
    write_cicle (c_t4_velozwav);
    write_cicle (c_t4_velozwav);

    return res;
}

