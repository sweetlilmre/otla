//---------------------------------------------------------------------------

#ifndef zxcodeH
#define zxcodeH



#define _SNAPSHOT_ 0
#define _UNBLOQUE_ 1
#define _VARIOSBLOQUES_ 2
#define _CARGADOR_ORIGINAL_ 3





typedef struct {
   unsigned short dir;
   unsigned char value;
   } st_poke;

int preprocesa (const char * file );

extern char *nom_metodo [];
extern char *nom_kbps [];
extern char *nom_esquema [];



extern char antikolmogorov;

extern char cargador;
extern char control_chksum;
extern char variosbloques;
//extern char esquema;

extern char esquema_bloques ;


extern unsigned short dir_clear;
extern unsigned short dir_usr;





#define _MAX_POKES_ 20
extern st_poke pokes [];


//int convierte ();

int convierteHI (const char * file_out);
int convierteNORMAL (const char * file_in,const char * file_out);

//---------------------------------------------------------------------------

#endif
