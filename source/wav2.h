

#ifndef wav2H
#define wav2H


#define _CUADRADA_  0
#define _CUBICA_    1
#define _HYP_       2
#define _RAMPA_     3
#define _IGUAL_E_   4
#define _DELTA_     5
#define _DELTA_2    6

#define  _4_25   17
#define  _4_00   16
#define  _3_75   15

#define  _3_50   14
#define  _3_00   12
#define  _2_50   10
#define  _2_75   11
#define  _2_25   9
#define  _1_75   7



extern  int acelerar;


extern int invert_pol;
extern int forma_onda;
extern int freq_muestreo;
extern char s_p_b;

double dura ();



int open_wav  (const char * file_out);
int close_wav ( void );


int block2zx_wav   ( unsigned char * data , int length);
int block2cpc_wav  ( const unsigned char * data );
int block2msx_bin_wav (const char * name ,const unsigned char * data , int length , unsigned char filetype);
int block281_wav  ( const unsigned char * data , int length);


pseudopause (int ms);

pilot_veloz (int n);
int block2veloz_wav ( const unsigned char * data , int length);
int block2veloz81_wav ( const unsigned char * data , int length);


int wav2tzx (const char * file_in, const char * file_out, const char * mens);

#endif
