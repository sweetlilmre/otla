
#ifndef loadersH
#define loadersH


#include "sbb.h"



extern   double duracion_loader;
extern   double duracion_total;
extern   char nombre_head[17];

extern  int make_loader;
extern  int checkloaderror;
extern  int motor;




extern int pausa_ms;


int sbb2wav (const char * filen_in,const char * filen_out);

/*
int azimut2wav (const char * file_out);
int calibZ81_wav (const char * file_out);
*/

#endif