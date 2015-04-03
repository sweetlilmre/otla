

#ifndef funcionesH
#define funcionesH


#define E_ABRIR    -1
#define E_ESCRIBIR -2
#define E_LEER     -3

#define E_VERSION  -4
#define E_FORMATO  -5


#define E_MACHINE  -6


    
                                                       
unsigned int CRCupdate(unsigned int CRC, unsigned char new_byte);
unsigned char checksum (const unsigned char * s, int length);



extern unsigned char * bufferfile;
extern unsigned tamanyo;

int load_file (const char *filename);

const char * error_file(int e);

int inputDecHex (const char * s);

const char * n2s(unsigned n , int h);

const char * modelo (char maquina, int model);



#endif
