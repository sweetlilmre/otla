
#include <stdio.h>
#include <stdlib.h >
#include "funciones.h"

/////////////////////////////////////////////////////

unsigned char checksum (const unsigned char * s, int length)
{
   int i;
   unsigned char res=0;
   for (i=0;i<length;i++)
         res^=s[i];
   return res;
}

//------------------------------------------------
//------------------------------------------------
// from 2CDT Copyright (c) Kevin Thacker   :
/* CRC code shamelessly taken from Pierre Guerrier's AIFF decoder! */
#define kCRCpoly  4129  /* used for binary long division in CRC */

/* CRC polynomial: X^16+X^12+X^5+1 */
unsigned int CRCupdate(unsigned int CRC, unsigned char new_byte)
{
 unsigned int aux = CRC ^ (new_byte << 8);
 int i;

 for(i=0; i<8; i++)
   if (aux & 0x8000)
       aux = (aux <<= 1) ^ kCRCpoly;
     else
       aux <<= 1;

 return (aux);
}
//------------------------------------------------


unsigned char * bufferfile=NULL;
unsigned tamanyo;
int load_file (const char *filename)
{
    FILE *f;
    int res=-1;
    

    if (bufferfile!=NULL)
    {
        free ( bufferfile );
        bufferfile=NULL;
    }
    f=fopen (filename,"rb");
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
                res=0;
            }
        }
        fclose (f);
    }
    return res;
}
//------------------------------------------------
const char * mens []=
{
   "no error",
   "could not open file",
   "error writing file",
   "error reading file",
   "error version file",
   "error format file",
   "file for wrong machine",
   "not supported file",
   "?????"
};

const char * error_file(int e)
{
     if (e<0 && e>-8)
     return mens [-e];
     else
     return "unknown error";
}

int inputDecHex (const char * s)
{
   int input=0;

   if (s[0]=='$')
        sscanf (s+1,"%x",&input);
   else
        input=atoi(s);
   return input;
}

char temp[20] ;
const char * n2s(unsigned n , int h)
{
    if (h)
        if (n<256)            
            sprintf (temp,"$%2.2x" ,n);
        else
            sprintf (temp,"$%4.4x" ,n);
    else
        sprintf (temp,"%u",n);
    return temp ;
}


const char * modelo (char  maquina, int model)
{
    switch (maquina)
        {
            case 'Z':
                switch (model)
                    {
                    case 1: return "48k";
                    case 2: return "128k";
                    case 3: return "+2a";
                    }
                break;
            case 'C':
                switch (model)
                    {
                    case 1: return "464";
                    case 2: return "6128";
                    case 3: return "664";
                    }
                break;
            case 'M':
                switch (model)
                    {
                    case 1: return "16k";
                    case 2: return "32k";
                    case 3: return "64k";
                    }
                break;
            case '8':
                switch (model)
                    {
                    case 1: return "1k";
                    case 2: return "2k";
                    case 3: return "16k";
                    case 4: return "48k";
                    case 5: return "64k";
                    }
                break;
        }

    return "";
}



