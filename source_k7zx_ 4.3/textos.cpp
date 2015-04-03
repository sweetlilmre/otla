//---------------------------------------------------------------------------


#pragma hdrstop

#include "textos.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

const char * t_rom="One bit is encoded by the length between a rising edge and the \
follwing falling edge (the length of a positive pulse). If length of negative \
pulse is equal to positive one, data is encoded in the reverse wav too. This is the \
technic used by the loading rutine in spectrum ROM \r\n\
If length of negative pulse is constant, the load will \
be faster. The code for the loading rutine is the simpliest.";

const char * t_milks="Same as ROM but a positive pulse encodes 2 bit at a time. Only\
3.5 sample/bit will work with both direct and reverse polarity.\r\n Milks loading routine \
is the only one in k7zx that includes a header with address and size of each block.\
So it can emulate LOAD \"\" CODE.";

const char * t_fsk="One bit is encoded by the length between two falling edges. It \
works reliablely in a wide range of speeds.";

const char * t_slow ="Same as FSK but a cicle encodes 2 bit  at a time. \
Cubic  or Saw waveform is prefered dependig on Spectrum model";

const char * t_delta ="Same as FSK but a cicle encodes 2 bit at a time. \
The length of positive pulse is constant (1 or 2 samples). Beacuase of that it \
will work with direct and reverse polarity and aditional code for detecting \
polarity is not needed.";

const char * t_raudo ="Same as FSK but a cicle encodes 2 bit  at a time. \
A more efficient code in the loading rutine allows 27,428 bps!! into a real\
Spectrum.\r\n No multi block , no control checksum,";

const char * t_ultra ="Negative pulse encodes one bit and positive pulse encodes \
following bit. This technic requieres a fine adjustement of volmen and select the\
 better wave form (0,3,4). But it's worth it : 22,050 bps can be achived";

const char * t_npu="Same as Ultra but encodes 2 bits for pulse.\r\n ";

const char * t_fi="One bit is encoded by the diference between the lengths of the positive\
 pulse ande negative pulse of a cicle. Cicle length is constant. The same routine in memory\
 can load data at diferent speeds. All others routines in k7zx have parameters to be adjusted\
  for each speed.";


const char * t_fiq="Same as Fi but encodes 2 bits at a time.";


const char * t_manchester="One bit is encoded by low and high levels syncronized with\
 a clock frequency. See details in wikipedia\r\n Better Wave form: 4 equal energy";

const char * t_man_dif="One bit is encoded by low and high levels syncronized with\
 a clock frequency. See details in wikipedia\r\n Better Wave form: 4 equal energy";


const char * t_escurrido="It works as Shavings Delta but more complicated to be faster ";

const char * t_none="???";


const char * explicacion []={

    t_rom,
    t_milks,
    t_fsk,
    t_slow,
    t_delta,
    t_raudo,
    t_ultra,
    t_npu,
    t_fi,
    t_fiq,
    t_manchester,
    t_man_dif,
    t_escurrido,
    t_none,
    t_none,
    t_none,
    t_none,
    t_none,
    t_none

};

const char * c_normal=" Tap and Tzx files (not snapshots) are converted to \
the original wav file just as many other wellknown utilities (tap2wav\
, tzx2wav.. ) do.\r\nBut the wave form in the .wav file is not a\
square one. It is  a polinomial (cubic) fuction ( x*(x-P1)*(x-2*P1)).\
With this wave form its posible to convert the wave file to a mp3 file with notable\
compression rates   For instance 32k  Manic Miner results a 8,200 KB wav (8 bits mono). \
And this wav can be compressed to a  559 KB mp3 file";

const char * c_hi=" Tap , tzx , sna and z80 files are converted to a wav \
file that it's loaded  into a real Spectrum in few seconds (for instance normal \
length 190 seconds of Manic Miner can be loaded in 9,5 s.). \r\n\
 Diferent encoding tecniques can be used. Fsk at 5 , S.Slow at 3.5 and S.Raudo at 2.75\
 samples/bit have the best speed/reliability ratio\r\n\
(Tap an Tzx files can be converted only if they contain blocks of data with their headers\
They should be also contain correct CLEAR and USR parameters. But if not, they can \
be fixed manually. Up to 7 blocks can be laoded with  \"many blocks\" option)";

const char * c_ma = " Tap , tzx , sna and z80 files are converted to a wav \
file that it's loaded  into a real Spectrum just as HiSpeed option but not too \
fast.\r\nThere are 6 diferent encoding tecnichs specifically designed to obtain a \
wav file that can be converted to an playable mp3 file or (why not?) be recorded to \
a real magnetic tape.";

const char * e_conversion []={
  c_normal,
  c_hi,
  c_ma

} ;

