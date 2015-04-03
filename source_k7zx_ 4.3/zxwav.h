//---------------------------------------------------------------------------

#ifndef zxwavH
#define zxwavH  

#define  _NORMAL_      0
#define  _NONTROPO_    1
#define  _ESCALOFRIO_  2


#define _79_  79.36508

#define _ROM_              0
#define _MILKS_            1

#define _FSK_              2
#define _SHAVINGS_SLOW_    3
#define _SHAVINGS_DELTA_   4
#define _SHAVINGS_RAUDO_   5

#define _ULTRA_           6
#define _NPU_             7

#define _FI_              8
//#define _PWM_             9
#define _FI_Q_             9

#define _MANCHESTER_     10
#define _MANCHESTER_DIF_ 11

#define _ESCURRIDO_      12
  
#define  _ANDANTE_      13
#define  _AGITATTO_      14
#define  _VIVACE_       15
#define  _PRESTO_       16
#define  _MAESTOSO_     17
#define  _SCHERZANDO_     18



#define  _8_00     32
#define  _7_00     28
#define  _6_00     24
#define  _5_00     20
#define  _4_00     16
#define  _3_50     14
#define  _3_00     12
#define  _2_75     11
#define  _2_50     10
#define  _2_25      9
#define  _2_00      8
#define  _1_75      7
#define  _1_50      6
#define  _1_25      5

#define  _1_33      4


#define _CUADRADA_  0
#define _RAMPA_     1
#define _CUBICA_    2
#define _0_CONTINUA_  3
#define _IGUAL_E_   4
#define _PARABOLA_  5
#define _DELTA_     6



typedef struct {
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
} WavHdr;

#pragma pack(push, 1)

typedef struct {
   unsigned short l;
   unsigned short r;
   } st_stereo;

typedef struct {
  char zxt[6];//="ZXTape!"
  char eof;//=0x1a
  char M; //=1
  char n;//=5
  char id; //=15
  unsigned short _79;//=79
  unsigned short ms;  //=50
  char b;//=0
  unsigned short L1;
  unsigned char L2;
  char data[1];
  }st_Tzx15Hdr   ;



#pragma pack(pop)



extern char emular;

extern int FqMuestreo;

extern int signo;
extern char CD;
extern char forma_onda;
extern char tono_final;
extern char max_vol;
extern char acelerar_basic;

extern int inv_r;
extern int divisor;
extern int multipicador;

extern float duracion;
extern float duracion_loader;


extern char metodo;
extern char muestras_por_bit;




extern char p [5];

int preprocesa (const char * file );
int convierte ();


int abreWav (const char * file_out);
int cierraWav ();


void LoaderBlock (char *churro, unsigned longitud);


escalofridoWav (char * t , unsigned short inicio , unsigned short longitud );


#define _t_P1_   8
#define _t_P2_   8
#define _t_S1_   4
#define _t_S2_   2   

pilot_digital (int n);



puretone (double Ppilot, int n);
sync (double Psync1, double Psync2);  
secuence( unsigned short  *s ,int n);
//puredata(double Pcero, double Puno, char *d,int n);
puredata(double Pcero, double Puno, char *d,int n, int l_b);
pausa ( double P , int n );

directrecording (unsigned short,unsigned char *data, int n);

//---------------------------------------------------------------------------

#endif
