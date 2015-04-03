//---------------------------------------------------------------------------

#include <vcl.h>
#include <inifiles.hpp>

#include <mmsystem.h>
#pragma hdrstop

#include "MainForm.h"
#include "zxwav.h"
#include "zxfiles.h"
#include "rutinas.h"
#include "zxcode.h"
#include "ProressForm.h"
#include "about.h"
#include "SettingdForm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
     : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{




#ifndef _ENGLISH_

#else

   OndaGB->Caption="Wave properties";
   Label1->Caption="Sampling frequency";
   InvertidaChkBx->Caption="Invert wave";

   FinalChkBx->Caption="Final tone";
   MaxVolChkBx->Caption="Max volumen";

   AcelerarChk->Caption="Basic acelerator";
   InvRChkBx->Caption="Invert right channel";
   Label2->Caption="Waveform";

   InfoChkBx->Caption="Info in file's name";
   ConvertirBtn        ->Caption="Convert";
   TodosBtn      ->Caption="Convert all";
   EmularBtn      ->Caption="Emulator";
   MethodGB      ->Caption="Loading method";
   KolmogorovChk      ->Caption="Statistic optimitation";

   FormaCmbBx->Items->Strings[_CUADRADA_]="Square";
   FormaCmbBx->Items->Strings[_RAMPA_]="rampa";
   FormaCmbBx->Items->Strings[_CUBICA_]="Cubic";
   FormaCmbBx->Items->Strings[_0_CONTINUA_]="Continuous Compensate";
   FormaCmbBx->Items->Strings[_IGUAL_E_]="E Compensate";
   FormaCmbBx->Items->Strings[_PARABOLA_]="parabola";
   FormaCmbBx->Items->Strings[_DELTA_]="delta";

   SpeedGB      ->Caption="Speed";
   CargadorChk      ->Caption="Generate loader";
   EsquemaGB      ->Caption="Loadig scheme";
   TapeErrorChkBx      ->Caption="Verify tape loading error";
   UnBloqueRB   ->Caption="All in one block";
   BloquesRB ->Caption="Many blocks";
   OriginalRB ->Caption="As likely as original loader";
   PokeadorChkBx   ->Caption="Do pokes";
   PokesVLE->   TitleCaptions->Strings[0]="Address";
   PokesVLE->   TitleCaptions->Strings[1]="Value";

   TabControl1->Tabs->Strings[1]="samples/bit";
   Label7->Caption="b.p.s=Frequency/(s/b)";
#endif

   TIniFile *ini;
   ini = new TIniFile( ChangeFileExt( Application->ExeName, ".INI" ) );
   Top     =  ini->ReadInteger( "Form", "Top", 100 );
   Left    =  ini->ReadInteger( "Form", "Left", 100 );
   ini->ReadBool( "Form", "InitMax", false ) ?
         WindowState = wsMaximized : 
         WindowState = wsNormal;

   FqMuestreo=ini->ReadInteger( "Ops", "Frecuencia",48000);

   /*
   if (FqMuestreo==44100)
        FrecuenciaCmbBx->ItemIndex=0;
   else
        FrecuenciaCmbBx->ItemIndex=1;
    */
   switch (FqMuestreo)
      {
      case 44100:   FrecuenciaCmbBx->ItemIndex=0;
         break;
      case 96000:   FrecuenciaCmbBx->ItemIndex=2;
         break;
      default:  FrecuenciaCmbBx->ItemIndex=1;
         break;

      }
   FrecuenciaCmbBxChange(Sender);

   metodo=ini->ReadInteger( "Ops", "Metodo", _MILKS_);
   switch (metodo)
      {
       case _FI_:
             FiRB->Checked=true;break;
       case _FSK_:
             FSKRB->Checked=true;break;
       case _PWM_:
             PWMRB->Checked=true;break;
       case _SHAVINGS_SLOW_ :
             SlowRB->Checked=true;break;
       case _MANCHESTER_DIF_:
             Manch_difRB->Checked=true;break;
       case _MANCHESTER_:
             Manch_ieeeRB->Checked=true; break;
       case _SHAVINGS_DELTA_ :
             DeltaRB->Checked=true;break;
       case _ULTRA_:
             UltraRB->Checked=true;break;
       case _ESCURRIDO_:
             EscurridoRB->Checked=true;break;
       case _NPU_ :
             NonPlusRB->Checked=true;break;
       case _SHAVINGS_TURBO_ :
             TurboRB->Checked=true;break;
       case _ROM_ :
             ROMRB->Checked=true;break;
             /*
       case _ANDANTE_ :
             Lento4000RB->Checked=true;break;
       case _ALEGRO_ :
             Lento6000RB->Checked=true;break;
       case _VIVACE_ :
             Lento8000RB->Checked=true;break;
       case _PRESTO_ :
             Lento11RB->Checked=true;break;    */
       default :
             MilksRB->Checked=true;break;
      }


  
   UltraRBClick(NULL) ;

   muestras_por_bit=ini->ReadInteger( "Ops", "Muestras" , _3_50);
   switch (muestras_por_bit)
       {
       case   _8_00 :
           r8_00RB->Checked=true;break;
       case   _7_00 :
           r7_00RB->Checked=true;break;
       case   _6_00 :
           r6_00RB->Checked=true;break;
       case   _5_00 :
           r5_00RB->Checked=true;break;
       case   _4_00 :
           r4_00RB->Checked=true;break;
       case   _3_50 :
           r3_50RB->Checked=true;break;
       case   _3_00  :
           r3_00RB->Checked=true;break;
       case   _2_75  :
           r2_75RB->Checked=true;break;
       case   _2_50  :
           r2_50RB->Checked=true;break;
       case   _2_25   :
           r2_25RB->Checked=true;break;
       case   _2_00  :
           r2_00RB->Checked=true;break;
       case   _1_75  :
           r1_75RB->Checked=true;break;
       case   _1_50  :
           r1_50RB->Checked=true;break;
       case   _1_25  :
           r1_25RB->Checked=true;break;
       case   _1_33  :
           r1_33RB->Checked=true;break;
         }

   InvertidaChkBx->Checked=ini->ReadBool   ( "Ops", "Invertida",  false);
   FormaCmbBx->ItemIndex=ini->ReadInteger  ( "Ops", "Forma",  0);
   FinalChkBx->Checked=ini->ReadInteger  ( "Ops", "Final",  0);
   MaxVolChkBx->Checked=ini->ReadInteger ( "Ops", "Volumen",  0);
   AcelerarChk->Checked=ini->ReadInteger ( "Ops", "Acelerar",  0);

   CDChk->Checked=ini->ReadBool            ( "Ops", "CD",  false);

   DivisorFChkBx->Checked=ini->ReadBool ( "Ops", "Divisor",  false);
   InvRChkBx->Checked=ini->ReadBool ( "Ops", "Inv_derecho",  false);


   KolmogorovChk->Checked=ini->ReadBool    ( "Ops", "Kolmogorov",  false );

   CargadorChk->Checked=ini->ReadBool      ( "Ops", "Cargador", true);
   TapeErrorChkBx->Checked=ini->ReadBool   ( "Ops", "control_chksum",  true );
   CargadorChk->Checked=ini->ReadBool      ( "Ops", "Cargador",  true );
   BloquesRB->Checked= ini->ReadBool       ( "Ops", "Bloques", true );
   UnBloqueRB->Checked=!BloquesRB->Checked;
   OriginalRB->Checked=ini->ReadBool       ( "Ops", "Basic", false );
   InfoChkBx->Checked=ini->ReadBool        ( "Ops", "Info", false );
   PokeadorChkBx->Checked=ini->ReadBool    ( "Ops", "Pokeador", false );


   try {
   DirectoryListBox1->Directory=ini->ReadString ( "Files", "Path", "" );
   }
   catch (...){}


   delete ini;

}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
   readzxfile ("");
   TIniFile *ini;
   ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ) );


   ini->WriteInteger( "Form", "Top", Top );
   ini->WriteInteger( "Form", "Left", Left );
   ini->WriteString ( "Form", "Caption", Caption );
   ini->WriteBool   ( "Form", "InitMax",
                       WindowState == wsMaximized );

   ini->WriteString ( "Files", "Path", DirectoryListBox1->Directory );

   interfaz();

   ini->WriteInteger( "Ops", "Frecuencia", FqMuestreo );

   ini->WriteBool   ( "Ops", "Invertida",  (signo==-1));
   ini->WriteBool   ( "Ops", "Final",  tono_final);
   ini->WriteInteger( "Ops", "Volumen",  max_vol );
   ini->WriteInteger( "Ops", "Acelerar",  acelerar_basic );


   ini->WriteBool   ( "Ops", "CD",  CD);
   ini->WriteBool   ( "Ops", "Divisor",  (divisor==2));
   ini->WriteBool   ( "Ops", "Inv_derecho", ( inv_r==-1));

   ini->WriteInteger( "Ops", "Forma",  FormaCmbBx->ItemIndex);


   ini->WriteInteger( "Ops", "Metodo", metodo);
    ini->WriteInteger( "Ops", "Muestras", muestras_por_bit );
   ini->WriteBool   ( "Ops", "Kolmogorov",  antikolmogorov );
   ini->WriteBool   ( "Ops", "control_chksum",  control_chksum );
   ini->WriteBool   ( "Ops", "Cargador",  CargadorChk->Checked);
   ini->WriteBool   ( "Ops", "Bloques",  BloquesRB->Checked);
   ini->WriteBool   ( "Ops", "Basic",  OriginalRB->Checked);
   ini->WriteBool   ( "Ops", "Info",  InfoChkBx->Checked);
   ini->WriteBool   ( "Ops", "Pokeador",  PokeadorChkBx->Checked);

   delete ini;

}

//---------------------------------------------------------------------------

void __fastcall TForm1::DivisorFChkBxClick(TObject *Sender)
{
    UltraRBClick (NULL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::UltraRBClick(TObject *Sender)
{

      r1_25RB->Enabled=false;
      r1_33RB->Enabled=false;
      r1_50RB->Enabled=false;
      r1_75RB->Enabled=false;
      r2_00RB->Enabled=false;
      r2_25RB->Enabled=false;
      r2_50RB->Enabled=false;
      r2_75RB->Enabled=false;
      r3_00RB->Enabled=false;
      r3_50RB->Enabled=false;
      r4_00RB->Enabled=false;
      r5_00RB->Enabled=false;
      r6_00RB->Enabled=false;
      r7_00RB->Enabled=false;
      r8_00RB->Enabled=false;

      
        r1_25RB->Color=clBtnFace;
        r1_33RB->Color=clBtnFace;
        r1_50RB->Color=clBtnFace;
        r1_75RB->Color=clBtnFace;
        r2_00RB->Color=clBtnFace;
        r2_25RB->Color=clBtnFace;
        r2_50RB->Color=clBtnFace;
        r2_75RB->Color=clBtnFace;
        r3_00RB->Color=clBtnFace;
        r3_50RB->Color=clBtnFace;
        r4_00RB->Color=clBtnFace;
        r5_00RB->Color=clBtnFace;
        r6_00RB->Color=clBtnFace;
        r7_00RB->Color=clBtnFace;
        r8_00RB->Color=clBtnFace;

      if (DivisorFChkBx->Checked)
        {
        r1_25RB->Visible=false;
        r1_33RB->Visible=false;
        r1_50RB->Visible=false;
        r1_75RB->Visible=false;
        r2_00RB->Visible=false;
        r2_25RB->Visible=false;
        r2_50RB->Visible=false;
        r2_75RB->Visible=false;
        r3_00RB->Visible=false;
        r3_50RB->Visible=false;
        r4_00RB->Visible=false;
        r5_00RB->Visible=false;
        r6_00RB->Visible=false;
        r7_00RB->Visible=false;
        r8_00RB->Visible=false;
        }
      else
        { 
        r1_25RB->Visible=true;
        r1_33RB->Visible=true;
        r1_50RB->Visible=true;
        r1_75RB->Visible=true;
        r2_00RB->Visible=true;
        r2_25RB->Visible=true;
        r2_50RB->Visible=true;
        r2_75RB->Visible=true;
        r3_00RB->Visible=true;
        r3_50RB->Visible=true;
        r4_00RB->Visible=true;
        r5_00RB->Visible=true;
        r6_00RB->Visible=true;
        r7_00RB->Visible=true;
        r8_00RB->Visible=true;
        }

      TapeErrorChkBx->Enabled=false;

      BloquesRB->Enabled=false;
      OriginalRB->Enabled=false;

      Sync1ChkBx->Visible=false;

      KolmogorovChk->Enabled=false;

      EscurridoCmbBx->Enabled=false;

      if (FormaCmbBx->ItemIndex<0)
          FormaCmbBx->ItemIndex=0;

      EscurridoCmbBx->Enabled=false;
      
      if (EscurridoCmbBx->ItemIndex<0)
          EscurridoCmbBx->ItemIndex=0;

    //  FormaCmbBx-> ItemIndex=0;
      if (Lento4000RB->Checked||Lento6000RB->Checked)
         {
         SpeedGB->Visible=false;
          TapeErrorChkBx->Enabled=true;
          BloquesRB->Enabled=true;
         }
      else
         SpeedGB->Visible=true;

      if (ROMRB->Checked)
          {
          r2_50RB->Enabled=true;
          r2_50RB->Color=clRed;

          r3_00RB->Enabled=true;
          r3_00RB->Checked=true;
          TapeErrorChkBx->Enabled=true;
          BloquesRB->Enabled=true;
          }

      if (FiRB->Checked)
          {
          r3_00RB->Enabled=true;
          r3_00RB->Color=clRed;
          r4_00RB->Enabled=true;
          r5_00RB->Enabled=true;
          r6_00RB->Enabled=true;
          r6_00RB->Visible=true;
          r7_00RB->Enabled=true;
          r8_00RB->Enabled=true;
          r8_00RB->Visible=true;

          r6_00RB->Checked=true;
          TapeErrorChkBx->Enabled=true;
          BloquesRB->Enabled=true;
          }

      if (FSKRB->Checked)
          {
          r2_50RB->Enabled=true;
          r3_00RB->Enabled=true;
          r4_00RB->Enabled=true;
          r5_00RB->Enabled=true; 
          r5_00RB->Visible=true;
          r6_00RB->Enabled=true;
          r6_00RB->Visible=true;
          r7_00RB->Enabled=true;
          r7_00RB->Visible=true;
          r8_00RB->Enabled=true;
         // r8_00RB->Visible=true;

          r5_00RB->Checked=true;

          TapeErrorChkBx->Enabled=true;
          BloquesRB->Enabled=true;

          }

      if (PWMRB->Checked)
          {
          r2_50RB->Enabled=true;
          r3_00RB->Enabled=true;
          r3_50RB->Enabled=true;

          r3_50RB->Checked=true;
          }

      if (SlowRB->Checked)
          {
          r2_50RB->Enabled=true;
          r3_00RB->Enabled=true;
          r3_50RB->Enabled=true;
          r3_50RB->Visible=true;
          r4_00RB->Enabled=true;

          r3_50RB->Checked=true;

          TapeErrorChkBx->Enabled=true;
          BloquesRB->Enabled=true;

          }
      
      if (Manch_ieeeRB->Checked)
          {
          r2_00RB->Enabled=true;
          r3_00RB->Enabled=true;
          r4_00RB->Enabled=true;
          r4_00RB->Visible=true;

          r4_00RB->Checked=true;

          TapeErrorChkBx->Enabled=true;
          BloquesRB->Enabled=true;
          }
      if (Manch_difRB->Checked)
          {
          r2_00RB->Enabled=true;
          r3_00RB->Enabled=true;
          r4_00RB->Enabled=true;
          r4_00RB->Visible=true;

          r4_00RB->Checked=true;

          TapeErrorChkBx->Enabled=true;
          BloquesRB->Enabled=true;
          }

      if (MilksRB->Checked)
          {
          r1_75RB->Enabled=true;
          r2_25RB->Enabled=true;
          r2_50RB->Enabled=true;
          r2_75RB->Enabled=true;
          r3_50RB->Enabled=true;
          
          r3_50RB->Checked=true;

          TapeErrorChkBx->Enabled=true;
          BloquesRB->Enabled=true;
          OriginalRB->Enabled=true;
          }

      if (DeltaRB->Checked)
          {
          r1_75RB->Enabled=true;
          r2_25RB->Enabled=true;
          r2_75RB->Enabled=true;
          r3_00RB->Enabled=true;
          r3_50RB->Enabled=true;
          r3_50RB->Visible=true;

          r3_50RB->Checked=true;
          KolmogorovChk->Enabled=true;
          
          BloquesRB->Enabled=true;
          }

      if (TurboRB->Checked)
         {
          r1_75RB->Enabled=true;
          r2_25RB->Enabled=true;
          r2_50RB->Enabled=true;
          r2_75RB->Enabled=true;
          r2_25RB->Checked=true;
         }

      if (UltraRB->Checked)
          {
          r1_50RB->Enabled=true;
          r2_00RB->Enabled=true;
          r2_50RB->Enabled=true;
          r3_00RB->Enabled=true;
          r3_00RB->Visible=true;

          r3_00RB->Checked=true;
         }

      if (EscurridoRB->Checked)
          {
          r1_33RB->Enabled=true;
          r1_75RB->Enabled=true;
          r2_00RB->Enabled=true;
        //  r2_50RB->Enabled=true;
          r1_75RB->Checked=true;

          EscurridoCmbBx->Enabled=true;
          }

      if (NonPlusRB->Checked)
          {
          r1_25RB->Enabled=true;
          r1_75RB->Enabled=true;
          r2_00RB->Enabled=true;
          r2_50RB->Enabled=true;
          r2_50RB->Visible=true;

          r2_50RB->Checked=true;

          
          TapeErrorChkBx->Enabled=true;
          BloquesRB->Enabled=true;
          }

      /*
      if (MilksTurboRB->Checked)
         {
          r1_75RB->Enabled=true;
          r1_75RB->Checked=true;
         }

      if (npu_luxRB->Checked)
          {
          r1_25RB->Enabled=true;
          r1_75RB->Enabled=true;
          r2_00RB->Enabled=true;
          r2_50RB->Enabled=true;
          r2_50RB->Visible=true;
          r2_50RB->Checked=true;

          TapeErrorChkBx->Enabled=true;
          BloquesRB->Enabled=true;
          }
          */
}
 //---------------------------------------------------------------------------


void __fastcall TForm1::FrecuenciaCmbBxChange(TObject *Sender)
{
     //
     AnsiString s;
     int f;
   if (FrecuenciaCmbBx->ItemIndex==0)
      {
      f=4*44100;
      }
   else
      {   
      f=4*48000;
      }



   if (TabControl1->TabIndex !=1)
     {
     s.printf ("%i.%3.3i bps", (int)(3*f/16)/1000,((int)(3*f/16))%1000);
     r1_33RB->Caption=s;
     s.printf ("%i.%3.3i bps", (f/_1_25)/1000,(f/_1_25)%1000);
     r1_25RB->Caption=s;
     s.printf ("%i.%3.3i bps", (f/_1_50)/1000,(f/_1_50)%1000);
     r1_50RB->Caption=s;
     s.printf ("%i.%3.3i bps", (f/_1_75)/1000,(f/_1_75)%1000);
     r1_75RB->Caption=s;
     s.printf ("%i.%3.3i bps", (f/_2_00)/1000,(f/_2_00)%1000);
     r2_00RB->Caption=s;
     s.printf ("%i.%3.3i bps", (f/_2_25)/1000,(f/_2_25)%1000);
     r2_25RB->Caption=s;
     s.printf ("%i.%3.3i bps", (f/_2_50)/1000,(f/_2_50)%1000);
     r2_50RB->Caption=s;
     s.printf ("%i.%3.3i bps", (f/_2_75)/1000,(f/_2_75)%1000);
     r2_75RB->Caption=s;
     s.printf ("%i.%3.3i bps", (f/_3_00)/1000,(f/_3_00)%1000);
     r3_00RB->Caption=s;
     s.printf ("%i.%3.3i bps", (f/_3_50)/1000,(f/_3_50)%1000);
     r3_50RB->Caption=s;
     s.printf ("%i.%3.3i bps", (f/_4_00)/1000,(f/_4_00)%1000);
     r4_00RB->Caption=s;
     s.printf ("%i.%3.3i bps", (f/_5_00)/1000,(f/_5_00)%1000);
     r5_00RB->Caption=s;
     s.printf ("%i.%3.3i bps", (f/_6_00)/1000,(f/_6_00)%1000);
     r6_00RB->Caption=s;
     s.printf ("%i.%3.3i bps", (f/_7_00)/1000,(f/_7_00)%1000);
     r7_00RB->Caption=s;
     s.printf ("%i.%3.3i bps", (f/_8_00)/1000,(f/_8_00)%1000);
     r8_00RB->Caption=s;

     }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::CargadorChkClick(TObject *Sender)
{
        NombreEdt->Enabled= CargadorChk->Checked;
}
//---------------------------------------------------------------------------
        /*
void verBasic (unsigned char *mbasic, unsigned tbasic)
{
       st_linea_basic * linea;
       int n_l;
       AnsiString l;
       unsigned i=0;
       while (i<tbasic)
        {

        linea= (st_linea_basic *)  (mbasic +i);
        n_l=linea->lineaA*256+linea->lineaB;

        if (i+linea->longitud>tbasic)
           break;

        if (linea->data[linea->longitud-1]!=0x0d)
            break;//error
        l=n_l;
        l+=" ";

       for (int d=0;d<linea->longitud;d++)
          if  (linea->data[d]>127)
              switch (linea->data[d])
                {
                case 0xe0:
                    l+= "LLOAD ";
                    break;
                case 0xfd:
                    l+= "CLEAR ";
                    break;
                case 0xef:
                    l+= "LOAD ";
                    break;
                case 0xaf:
                    l+= "CODE ";
                    break;
                case 0xaa:
                    l+= "SCREEN$ ";
                    break;
                case 0xf9:
                    l+= "RANDOMIZE ";
                    break;
                case 0xc0:
                    l+= "USR ";
                    break;
                case 0xea:
                    l+= "REM \r";
                    d=linea->longitud;
                    break;
                default:
                    l+= " <";
                    l+= IntToHex ((linea->data[d]&0xf0)>>4,1);
                    l+= IntToHex (linea->data[d]&0x0f,1);
                    l+= ">";
                    break;
                }
            else
                if (linea->data [d]>=32)
                   {
                   l+= (char)linea->data [d];
                   if (linea->data [d]==':')
                        l+= " ";
                   }
                else
                   {
                   l+= " <";
                   l+= IntToHex ((linea->data[d]&0xf0)>>4,1);
                   l+= IntToHex (linea->data[d]&0x0f,1);
                   l+= ">";
                   }

        i+=linea->longitud+4;
        Form1->Memo1->Lines->Add(l);
        Form1->Memo1->Lines->Add("");

        }
     Form1->Memo1->Lines->Add("----------------------------\r");
 }
      */
void __fastcall TForm1::FileListBox1Change(TObject *Sender)
{
  PlayBtn->Enabled=false;
  ConvertirBtn->Enabled=false;
  EmularBtn->Enabled=false;

  TrozosCLB->Clear();
  Memo1->Clear();
  ClearEdt->Text=0;
  UsrEdt->Text=0;

  if (!readzxfile (FileListBox1->FileName.c_str()))
     {
     ClearEdt->Text=clearN;
     UsrEdt->Text=usrN;
     NombreEdt->Text=prg_name;
     //UnBloqueRB->Checked=true;
     SnapGB->Visible=snap.tipo_snap;
     if (snap.tipo_snap)
       {
       if (snap.tipo_snap==1)
          SnapGB->Caption="Snapshot 48k";
       else
          SnapGB->Caption="Snapshot 128k";
       PCLbl->Caption="PC: $"+IntToHex (snap.PC,4);
       SPLbl->Caption="SP: $"+IntToHex (snap.SP,4);
       CurrentLbl->Caption="out: $"+IntToHex (snap.pagina,2);

       }
     int i;
     for (i=0; i<7;i++)
      {
      AnsiString t;
      if (trozos[i].tipo<0)
        break;
      t=trozos[i].tipo;
      if (trozos[i].tipo=='0')
         {
         t+=" -Basic auto: ";
         t+=trozos[i].auto_run;
         t+=" long: ";
         t+=trozos[i].longitud-60;
         t+=" vars: ";
         t+=trozos[i].pam_2;

       //  verBasic (trozos[i].data, trozos[i].longitud-60);
         }
      else
         {
         t+=" -Bytes dir: ";
         t+=trozos[i].inicio;
         t+=" long: ";
         t+=trozos[i].longitud;
         }

      TrozosCLB->Items->Add(t);
      TrozosCLB->Checked[i]=trozos[i].procesar;



      }
     PlayBtn->Enabled=true;
     ConvertirBtn->Enabled=true;
     EmularBtn->Enabled=true;
   }
}

//---------------------------------------------------------------------------


AnsiString error_men;
int __fastcall  TForm1::interfaz()
{

   int error=0;

   error_men="";

   if (FrecuenciaCmbBx->ItemIndex)
      FqMuestreo=48000;
   else
      FqMuestreo=44100;

   if (FrecuenciaCmbBx->ItemIndex==2)
      multipicador=2;
   else
      multipicador=1;

   if (InvertidaChkBx->Checked)
       signo=-1;
   else
       signo=1;

   tono_final=FinalChkBx->Checked;

   acelerar_basic=AcelerarChk->Checked;

   if ((DivisorFChkBx->Enabled)&&(DivisorFChkBx->Checked))
        divisor=2;
   else
        divisor=1;

   CD=CDChk->Checked;
   
   if (InvRChkBx->Checked)
       inv_r=-1;
   else
       inv_r=1;
   
   forma_onda=FormaCmbBx->ItemIndex;;

   max_vol=MaxVolChkBx->Checked;;


   antikolmogorov=KolmogorovChk->Checked;
   control_chksum=TapeErrorChkBx->Enabled&&TapeErrorChkBx->Checked;


   if (MilksRB->Checked)
       metodo=_MILKS_;
   else
      if (SlowRB->Checked)
          metodo=_SHAVINGS_SLOW_;
      else
         if (DeltaRB->Checked)
            metodo=_SHAVINGS_DELTA_;
         else
            if (UltraRB->Checked)
                metodo=_ULTRA_;
            else
               if (NonPlusRB->Checked)
                   metodo=_NPU_;
               else
                  if (EscurridoRB->Checked)
                     {
                     metodo=_ESCURRIDO_;    /*
                     //escurrido_type=EscurridoCmbBx->ItemIndex;
                     if (r1_33RB->Checked)
                        escurrido_type=0;

                     else
                         if (r1_75RB->Checked)
                            escurrido_type=2;
                         else
                            escurrido_type=4;
                            */
                     }
                   else
                      if (FiRB->Checked)
                          metodo=_FI_;
                      else
                         if (PWMRB->Checked)
                             metodo=_PWM_;
                        else
                          if (Manch_ieeeRB->Checked)
                             metodo=_MANCHESTER_;
                          else
                            if (Manch_difRB->Checked)
                             metodo=_MANCHESTER_DIF_;
                            else
                              if (FSKRB->Checked)
                                 metodo=_FSK_;
                              else
                                 if (TurboRB->Checked)
                                   metodo=_SHAVINGS_TURBO_;
                                 else
                                 if (ROMRB->Checked)
                                   metodo=_ROM_;
                                 else  
                                 if (Lento4000RB->Checked)
                                   metodo=_ANDANTE_;
                                 else
                                   if (Lento6000RB->Checked)
                                     metodo=_ALLEGRO_;
                                   else   
                                     if (Lento11RB->Checked)
                                        metodo=_PRESTO_;
                                     else
                                     if (Lento8000RB->Checked)
                                        metodo=_VIVACE_;
                                     else
                                     /*
                                   if (MilksTurboRB->Checked)
                                     metodo=_RECORD_MILKS_;
                                   else
                                     if (npu_luxRB->Checked)
                                       metodo=_NPU_LUX_;
                                     else           */
                                       metodo=_SHAVINGS_TURBO_+1;



   if (r1_25RB->Checked)
        muestras_por_bit=_1_25;
   if (r1_33RB->Checked)
        muestras_por_bit=_1_33;
   if (r1_50RB->Checked)
        muestras_por_bit=_1_50;
   if (r1_75RB->Checked)
        muestras_por_bit=_1_75;
   if (r2_00RB->Checked)
        muestras_por_bit=_2_00;
   if (r2_25RB->Checked)
           muestras_por_bit=_2_25;
   if (r2_50RB->Checked)
        muestras_por_bit=_2_50;
   if (r2_75RB->Checked)
        muestras_por_bit=_2_75;
   if (r3_00RB->Checked)
        muestras_por_bit=_3_00;
   if (r3_50RB->Checked)
        muestras_por_bit=_3_50;
   if (r4_00RB->Checked)
        muestras_por_bit=_4_00;
   if (r5_00RB->Checked)
        muestras_por_bit=_5_00;
   if (r6_00RB->Checked)
        muestras_por_bit=_6_00;
   if (r7_00RB->Checked)
        muestras_por_bit=_7_00;
   if (r8_00RB->Checked)
        muestras_por_bit=_8_00;


   strncpy (prg_name, NombreEdt->Text.c_str(),10);

   dir_clear= ClearEdt->Text.ToIntDef(0);
   dir_usr= UsrEdt->Text.ToIntDef(0);


   if (snap.tipo_snap)
      esquema=_SNAPSHOT_;
   else
      if (UnBloqueRB->Checked||!BloquesRB->Enabled)
         esquema=_UNBLOQUE_;
      else
         if (OriginalRB->Checked&&OriginalRB->Enabled)
            esquema=_CARGADOR_ORIGINAL_;
         else
            esquema=_VARIOSBLOQUES_;

   int basics=0;
   int n_t=0;
   int t;
   for (t=0;t<TrozosCLB->Items->Count;t++)
        {
        trozos[t].procesar=0;
        if (TrozosCLB->Checked[t])
          {
          if (trozos[t].tipo == '3')
              {trozos[t].procesar=1;n_t++;}
          if (trozos[t].tipo == '0')
               //OJO if (esquema==_BASIC_Y_BLOQUES_)
                   {basics++;trozos[t].procesar=1;n_t++;}
          if (trozos[t].tipo == '5')
              {error_men+="Bloque sin cabecera. ";error++;break;}

          }
        }

   trozos[t].tipo=-1;

   if ((esquema==_CARGADOR_ORIGINAL_)&&(basics==0))
       esquema=_VARIOSBLOQUES_;
   if (CargadorChk->Checked)
      esquema=esquema|0x80;


   int n,p=0;

   if (PokeadorChkBx->Checked)
     for (int i=1; i< PokesVLE->RowCount; i++)
      {
      n= PokesVLE->Cells[0][i].ToIntDef(-1);
      if (n>=0&&n<=0xffff)
        {
        pokes [p].dir=n;
        n= PokesVLE->Cells[1][i].ToIntDef(-1);
        if (n>=0&&n<=0xff)
          {
          pokes [p].value=n;
          p++;
          }
        }
      if (i>_MAX_POKES_)
         ShowMessage ("Demasiados pokes");
      }

   pokes [p].dir=0;

   if (n_t==0)
     {error_men+="No se encontraron bloques. ";error++;}
   if (dir_clear==0)
     dir_clear=65344;//error++;
   if (dir_usr==0)
     {error_men+="No se encuentra dirección de arranque. ";}


   return  error;
}


//---------------------------------------------------------------------------


void __fastcall TForm1::ConvertirBtnClick(TObject *Sender)
{
  ConvertirBtn->Enabled=false;
  try {
   if (interfaz()==0)
     {
     emular=false;
     convierteNEW ( );

     AnsiString p= FileListBox1->FileName;
     if (InfoChkBx->Checked)
        {
        p=p+nom_metodo[metodo];
        p=p+nom_esquema[0x7f&esquema];
        p=p+nom_kbps[muestras_por_bit];
        if (signo==-1)
          p=p+"i";
        }
     p=p+".wav";

       if (DeleteFile (p.c_str()))
          MoveFile ("temp.wav",p.c_str());
       else
          CopyFile ("temp.wav",p.c_str(),false);

      AnsiString s=ExtractFileName(p);
      s+="\r\n Duración: ";
      s+=((int)(duracion*10))/10.0;
      s+="segs. \r\n"+error_men;
      
      ShowMessage(s.c_str());
     }
   else
       Application-> MessageBox(error_men.c_str(),"ERROR",MB_OK);
   }
   catch (...)
   {}
   ConvertirBtn->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::EmularBtnClick(TObject *Sender)
{

   EmularBtn->Enabled=false;
   try
   {
   if (interfaz()==0)
     {
     emular=true;

     convierteNEW();
     emular=false;

     AnsiString p= FileListBox1->FileName;
     if (InfoChkBx->Checked)
        {
        p=p+nom_metodo[metodo];
        p=p+nom_esquema[0x7f&esquema];
        p=p+nom_kbps[muestras_por_bit];
        if (signo==-1)
          p=p+"i";
        }

     p=p+".tzx";


     p="temp.tzx";
     ShellExecute(NULL,"open",p.c_str(),NULL,NULL,SW_SHOW );
     }
   else
       Application-> MessageBox(error_men.c_str(),"ERROR",MB_OK);
  }
   catch (...)
   {}
   EmularBtn->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PlayBtnClick(TObject *Sender)
{

  try {
   if (interfaz()==0)
     {
     emular=false;
     convierteNEW ();

     AnsiString p= DirectoryListBox1->Directory;
     p=p+"\\temp.wav";
     if (sndPlaySound (p.c_str(), SND_APPLICATION | SND_ASYNC))
        {
        PlayBtn->Enabled=false;
        StopBtn->Enabled=true;
        }
     else
        {
        ShellExecute(NULL,"open","temp.wav",NULL,NULL,SW_SHOW	);
        StopBtn->Enabled=true;
        }
     }
   else
       Application-> MessageBox(error_men.c_str(),"ERROR",MB_OK);
  }
  catch (...)
  {}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StopBtnClick(TObject *Sender)
{
     PlayBtn->Enabled=true;
     PlaySound (NULL,NULL,0);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TodosBtnClick(TObject *Sender)
{

    ProgresoFrm->ShowModal ();


}
//---------------------------------------------------------------------------



void __fastcall TForm1::CDChkClick(TObject *Sender)
{
     InvRChkBx->Enabled= CDChk ->Checked;
}

//---------------------------------------------------------------------------

void __fastcall TForm1::TabControl1Change(TObject *Sender)
{

  AnsiString s;
  
  int f=1;

     if (TabControl1->TabIndex ==1)
        {
     s.printf ("%.2f", 4/3.0);
     r1_33RB->Caption=s;

     s.printf ("%.2f", _1_25/4.0);
     r1_25RB->Caption=s;
     s.printf ("%.2f", _1_50/4.0);
     r1_50RB->Caption=s;
     s.printf ("%.2f", _1_75/4.0);
     r1_75RB->Caption=s;
     s.printf ("%.2f", _2_00/4.0);
     r2_00RB->Caption=s;
     s.printf ("%.2f", _2_25/4.0);
     r2_25RB->Caption=s;
     s.printf ("%.2f", _2_50/4.0);
     r2_50RB->Caption=s;
     s.printf ("%.2f", _2_75/4.0);
     r2_75RB->Caption=s;
     s.printf ("%.2f", _3_00/4.0);
     r3_00RB->Caption=s;
     s.printf ("%.2f", _3_50/4.0);
     r3_50RB->Caption=s;
     s.printf ("%.2f", _4_00/4.0);
     r4_00RB->Caption=s;
     s.printf ("%.2f", _5_00/4.0);
     r5_00RB->Caption=s;
     s.printf ("%.2f", _6_00/4.0);
     r6_00RB->Caption=s;
     s.printf ("%.2f", _7_00/4.0);
     r7_00RB->Caption=s;
     s.printf ("%.2f", _8_00/4.0);
     r8_00RB->Caption=s;
        }
     else
        {
        FrecuenciaCmbBxChange (NULL);
        }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::About1Click(TObject *Sender)
{
    AboutBox->Show();    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormaCmbBxDrawItem(TWinControl *Control, int Index,
      TRect &Rect, TOwnerDrawState State)
{
     ImageList1->Draw( FormaCmbBx->Canvas,Rect.Left,Rect.Top,Index,true);
     FormaCmbBx->Canvas->TextOutA(Rect.Left+30,Rect.Top,Index);

}
//---------------------------------------------------------------------------
 



void __fastcall TForm1::FormDblClick(TObject *Sender)
{
        SettingsFrm->Show();
}
//---------------------------------------------------------------------------


