//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "about.h"
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TAboutBox *AboutBox;
//--------------------------------------------------------------------- 
__fastcall TAboutBox::TAboutBox(TComponent* AOwner)
	: TForm(AOwner)
{

#ifndef _ENGLISH_
    Comments->Caption="   Este programa sirve para cargar datos al zx Spectrum\
    \r Es freeware.";
#else
    Memo1->Text="   This program explores diferents encoding technics for \
loading data to zx Spectrum.\r\n\
   It can be used directly conecting PC sound card to EAR socket of Spectrum \
or used to convert zx files (tap, tzx, sna, z80) to audio files that can be \
played by audio devices (cd, mp3, tape).\r\n\
   It is freeware. If more than free you think it is great, you can express \
yourself by sending a post card to the author:\r\n\r\n\
 Francisco Villa\r\n\ c/ Sofora 19, 2º A\r\n\ 28020 MADRID\r\n Spain\r\n\r\n\
 Gratefulness to Antonio Villena, Black Hole and good people of es.comp.sistemas.sinclair for their\
 inspiration and enthusiasm";
#endif
}
//---------------------------------------------------------------------
void __fastcall TAboutBox::OKButtonClick(TObject *Sender)
{
     Close();
}
//---------------------------------------------------------------------------

