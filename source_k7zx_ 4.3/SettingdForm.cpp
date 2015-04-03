//---------------------------------------------------------------------------

#include <vcl.h>   
#include <inifiles.hpp>
#include "zxwav.h"
#include "zxcode.h"
#pragma hdrstop

#include "SettingdForm.h"
#include "textos.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSettingsFrm *SettingsFrm;

char conversion;
int metodo_HI;
int metodo_MA;

int fq_HI;
int forma_HI;

int CD_HI;
int acelerar_basic_HI;
int chk_op;


char variosbloques;

int bitrate;
int bitrate_NOR;
int bitrate_MA;



char *tecnica[]={"ROM","Milks","FSK","S. Slow","S. Delta","S. Raudo",
                     "Ultra","NPU","Fi","Fi Quadruple","Manchester","Man. diferencial",
                     "Escurrido","Andante","Allegro Agitato","Vivace","Presto",
                     "Allegro Maestoso","Allegro Scherzando","?"};

char *info_tecnica []={"_ROM_","_MLK_","_FSK_","_SSL_","_SDE_","_SRA_",
                       "_ULT_","_NPU_","_FI__" ,"_FIQ_","_MAN_","_MDF_",
                       "_ESC_",
                       "_AND_","_AGI_","_VIV_","_PRE_","_MAE_","_SCH_","_???_"};

char *info_kbps []={"?","?","?","?",
                   "1.33","1.25","1.50","1.75",
                   "2.00","2.25","2.50","2.75",
                   "3.00","?"   ,"3.50","?",
                   "4.00","?","?","?",
                   "5.00","?","?","?",
                   "6.00","?","?","?",
                   "7.00","?","?","?",
                   "8.00","?","?","?"
                   };

AnsiString info ;
int m_p_b []={_8_00,
              _7_00,_6_00,_5_00,_4_00,
              _3_50,_3_00,_2_75,_2_50,
              _2_25,_2_00,_1_75,_1_50,
              _1_33,_1_25 , 0 
              };

//---------------------------------------------------------------------------
__fastcall TSettingsFrm::TSettingsFrm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSettingsFrm::AceptarBtnClick(TObject *Sender)
{

   conversion =  ConversionCmbBx->ItemIndex;
   
   if (FrecuenciaCmbBx->ItemIndex)
      fq_HI=48000;
   else
      fq_HI=44100;

   if (FrecuenciaCmbBx->ItemIndex==2)
      multipicador=2;
   else
      multipicador=1;

   forma_HI=FormaCmbBx->ItemIndex;;

   if (InvertidaChkBx->Checked)
       signo=-1;
   else
       signo=1;

   tono_final=FinalChkBx->Checked;
                      
   CD_HI=CDChk->Checked;

   if (InvRChkBx->Checked)
       inv_r=-1;
   else
       inv_r=1;

   acelerar_basic_HI=AcelerarChk->Checked;

   if ((DivisorFChkBx->Enabled)&&(DivisorFChkBx->Checked))
        divisor=2;
   else
        divisor=1;

   antikolmogorov=0;//KolmogorovChk->Checked;        
   cargador=CargadorChk->Checked;
   chk_op =TapeErrorChkBx->Checked;
   control_chksum=TapeErrorChkBx->Enabled&&TapeErrorChkBx->Checked;

   if (OriginalRB->Checked)
       variosbloques=_CARGADOR_ORIGINAL_;
   else
        if (BloquesRB->Checked)
            variosbloques=_VARIOSBLOQUES_;
        else
             variosbloques=_UNBLOQUE_;


         metodo_HI= MetodoCmbBx->ItemIndex;
         muestras_por_bit=m_p_b[MPBCmbBx->ItemIndex];
         
         if (AndanteRB->Checked)
            metodo_MA=_ANDANTE_;
         else
            if (AllegroRB->Checked)
                metodo_MA=_AGITATTO_;
             else
                if (VivaceRB->Checked)
                    metodo_MA=_VIVACE_;
                 else
                    if (PrestoRB->Checked)
                        metodo_MA=_PRESTO_;
                     else  
                        if (ScherzandoRB->Checked)
                           metodo_MA=_SCHERZANDO_;
                        else
                            metodo_MA=_MAESTOSO_;

    
    bitrate_NOR=RatioNORCmbBx->Text.ToIntDef(32);
    bitrate_MA=RatioMACmbBx->Text.ToIntDef(128);

    switch (conversion)
       {
       case 1:
          FqMuestreo= fq_HI;
          metodo=metodo_HI;
          forma_onda=forma_HI;
          acelerar_basic=acelerar_basic_HI;
          CD=CD_HI;
          bitrate = 256 ; //320;    
          info=info_tecnica[metodo];
          info+=info_kbps[muestras_por_bit];
          break;
       case 2:
          FqMuestreo= 48000;
          metodo=metodo_MA;
          forma_onda=2;
          bitrate = bitrate_MA;
          acelerar_basic=0;
          CD=0;
          info=info_tecnica[metodo];
          break;
       default:
          FqMuestreo= 44100;
          forma_onda=2;
          bitrate = bitrate_NOR;  
          acelerar_basic=0;
          CD=0;          
          info="";
       break;

       }


    metodo_MA-=_ANDANTE_;

    TIniFile *ini;
   ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ) );


   ini->WriteInteger( "Ops", "Conversor", conversion );

   ini->WriteInteger( "Ops", "Frecuencia", fq_HI );
   ini->WriteInteger( "Ops", "Forma",  forma_HI);

   ini->WriteBool   ( "Ops", "Invertida",  (signo==-1));
   ini->WriteBool   ( "Ops", "Final",  tono_final);
   ini->WriteBool   ( "Ops", "CD",  CD_HI);
   ini->WriteBool   ( "Ops", "Inv_derecho", ( inv_r==-1));
   ini->WriteInteger( "Ops", "Acelerar",  acelerar_basic_HI );
   ini->WriteBool   ( "Ops", "Divisor",  (divisor==2));

   ini->WriteInteger( "Ops", "MetodoHI", metodo_HI);
   ini->WriteInteger( "Ops", "MetodoMA", metodo_MA);
   ini->WriteInteger( "Ops", "Muestras", muestras_por_bit );
   ini->WriteBool   ( "Ops", "Kolmogorov",  antikolmogorov );

   ini->WriteBool   ( "Ops", "control_chksum",  chk_op );
   ini->WriteBool   ( "Ops", "Cargador",  cargador);
   ini->WriteInteger( "Ops", "Bloques",  variosbloques);


   ini->WriteInteger( "Ops", "RatioNOR",  bitrate_NOR);
   ini->WriteInteger( "Ops", "RatioMA",   bitrate_MA);

   delete ini;

   Close ();
}         //---------------------------------------------------------------------------
void __fastcall TSettingsFrm::FormCreate(TObject *Sender)
{
   TIniFile *ini;
   ini = new TIniFile( ChangeFileExt( Application->ExeName, ".INI" ) );

   conversion=ini->ReadInteger( "Ops", "Conversor",1);

   fq_HI=     ini->ReadInteger( "Ops", "Frecuencia",44100);

   if ( ini->ReadBool   ( "Ops", "Invertida",  false))
      signo=-1;
   else
      signo=1;
   tono_final= ini->ReadInteger  ( "Ops", "Final",  1);
   CD_HI=  ini->ReadBool  ( "Ops", "CD",  false);
   if (ini->ReadBool ( "Ops", "Inv_derecho",  false))
     inv_r=-1;
   else
      inv_r=1;
   acelerar_basic_HI=ini->ReadInteger ( "Ops", "Acelerar",  0);

   if (ini->ReadBool ( "Ops", "Divisor",  false))
      divisor=2;
   else
      divisor=1;


   metodo_HI=  ini->ReadInteger( "Ops", "MetodoHI",     _SHAVINGS_RAUDO_);
   forma_HI =  ini->ReadInteger( "Ops", "Forma",  2);
   muestras_por_bit = ini->ReadInteger( "Ops", "Muestras",  _2_75 );

   antikolmogorov=0;  //ini->ReadBool    ( "Ops", "Kolmogorov",  false );

   metodo_MA=        ini->ReadInteger( "Ops", "MetodoMA", _PRESTO_-_ANDANTE_);

   cargador=         ini->ReadBool ( "Ops", "Cargador", true);
   control_chksum=
   chk_op=  ini->ReadBool ( "Ops", "control_chksum",  true );
   variosbloques=   ini->ReadInteger( "Ops", "Bloques", _VARIOSBLOQUES_ );


   bitrate_NOR =   ini->ReadInteger ( "Ops", "RatioNOR",  64);
   bitrate_MA =    ini->ReadInteger ( "Ops", "RatioMA",   128);

   delete ini;

    switch (conversion)
       {
       case 1:
          FqMuestreo= fq_HI;
          metodo=metodo_HI;
          forma_onda=forma_HI; 
          bitrate = 256;  
          acelerar_basic=acelerar_basic_HI;
          CD=CD_HI;      
          info=info_tecnica[metodo];
          info+=info_kbps[muestras_por_bit];
          if ((metodo_HI==_ROM_)||
              (metodo_HI==_SHAVINGS_DELTA_)||
              (metodo_HI==_SHAVINGS_RAUDO_)||
              (metodo_HI==_ULTRA_)||
              (metodo_HI==_FI_Q_)||
              (metodo_HI==_ESCURRIDO_))
                 control_chksum= 0;
          break;
       case 2:
          FqMuestreo= 48000;
          metodo=metodo_MA+_ANDANTE_;
          forma_onda=2;   
          bitrate = bitrate_MA;
          acelerar_basic=0;
          CD=0;
          info=info_tecnica[metodo];
          break;
       default:
          FqMuestreo= 44100;
          forma_onda=2;
          bitrate = bitrate_NOR;  
          acelerar_basic=0;
          CD=0;
       break;
       }


} //---------------------------------------------------------------------------
void __fastcall TSettingsFrm::FormShow(TObject *Sender)
{
   int i;
    CargadorChk->Checked=cargador;
    TapeErrorChkBx->Checked=chk_op;
    switch (variosbloques)
           {
            case _UNBLOQUE_:
               UnBloqueRB->Checked=true;
               break;
            case _VARIOSBLOQUES_:
               BloquesRB->Checked=true;
               break;
            case _CARGADOR_ORIGINAL_:
               OriginalRB->Checked=true;
               break;
            }
            
      ConversionCmbBx->ItemIndex=conversion;

      MetodoCmbBx->ItemIndex=metodo_HI;
      
         switch (metodo_MA+_ANDANTE_)
            {
            case _ANDANTE_:
               AndanteRB->Checked=true;
               break;
            case _AGITATTO_:
               AllegroRB->Checked=true;
               break;
            case _VIVACE_:
               VivaceRB->Checked=true;
               break;
            case _MAESTOSO_:
               MaestosoRB->Checked=true;
               break;
            case _SCHERZANDO_:
               ScherzandoRB->Checked=true;
               break;
            default:     
            case _PRESTO_:
               PrestoRB->Checked=true;
               break;
            }
    if  (fq_HI==44100)
      {  FrecuenciaCmbBx->ItemIndex=0; }
    else
      {
      FrecuenciaCmbBx->ItemIndex=1;
      }

    InvertidaChkBx->Checked=(signo==-1);
    FinalChkBx->Checked=tono_final;
    CDChk->Checked=CD_HI;
    InvRChkBx->Checked=(inv_r==-1);
    InvRChkBx->Enabled= CDChk->Checked;
    AcelerarChk->Checked=acelerar_basic_HI;
    DivisorFChkBx->Checked=(divisor==2);

    RatioNORCmbBx->ItemIndex=1;
    for (int i=0;i<RatioNORCmbBx->Items->Count;i++)
        {
        if (RatioNORCmbBx->Items->Strings[i].ToIntDef(0)==bitrate_NOR)
           RatioNORCmbBx->ItemIndex=i;
        }

    RatioMACmbBx->ItemIndex=1;
    for (int i=0;i<RatioMACmbBx->Items->Count;i++)
        {
        if (RatioMACmbBx->Items->Strings[i].ToIntDef(0)==bitrate_MA)
            RatioMACmbBx->ItemIndex=i;
        }

    MetodoCmbBxChange(NULL);

    ConversionCmbBxChange(NULL);
    

    FormaCmbBx->ItemIndex=forma_HI;

           for (i= 14;i>=0;i--)
              if (m_p_b[i]==muestras_por_bit)
                 break;
          MPBCmbBx->ItemIndex=i;
          MPBCmbBx->Invalidate();

}
//---------------------------------------------------------------------------

void __fastcall TSettingsFrm::CancelarBtnClick(TObject *Sender)
{
      Close();
}
//---------------------------------------------------------------------------
AnsiString  __fastcall TSettingsFrm::Speed (int f,int mpb)
{
     if (mpb==4)
       {
       return AnsiString ((f*3)/4)+" bps";
       }
     else
       {
       return AnsiString ((f*4)/mpb)+" bps" ;
       }

}

//---------------------------------------------------------------------------

void __fastcall TSettingsFrm::FrecuenciaCmbBxChange(TObject *Sender)
{
      MPBCmbBx->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TSettingsFrm::CDChkClick(TObject *Sender)
{
    InvRChkBx->Enabled= CDChk->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsFrm::ConversionCmbBxChange(TObject *Sender)
{
      Memo1->Clear();
      switch (ConversionCmbBx->ItemIndex)
        {
        case 0:
           Memo1->Text = e_conversion [0];  
           MaNonTrppoGB->Visible=true;
           NormalGB->Visible=true;
           break;
        case 1:
           Memo1->Text = e_conversion [1];
           MaNonTrppoGB->Visible=false;
           NormalGB->Visible=false;
           
          TapeErrorChkBx->Enabled=! ((metodo_HI==_ROM_)||
              (metodo_HI==_SHAVINGS_DELTA_)||
              (metodo_HI==_SHAVINGS_RAUDO_)||
              (metodo_HI==_ULTRA_)||
              (metodo_HI==_FI_Q_)||
              (metodo_HI==_ESCURRIDO_));
           break;
        case 2: 
           Memo1->Text = e_conversion [2];
           MaNonTrppoGB->Visible=true;
           NormalGB->Visible=false;
           
           TapeErrorChkBx->Enabled=true;
           BloquesRB->Enabled=true;
             OriginalRB->Enabled=false;
           break;
        };
}
 
//---------------------------------------------------------------------------

void __fastcall TSettingsFrm::MetodoCmbBxChange(TObject *Sender)
{

      TapeErrorChkBx->Enabled=true;
      BloquesRB->Enabled=true;
      OriginalRB->Enabled=false;

    for (int i=0;i<MPBCmbBx->Items->Count ;i++)
      MPBCmbBx->Items->Strings[i] ="x";

      switch (MetodoCmbBx->ItemIndex)
           {
       case _ROM_ :              
             TapeErrorChkBx->Enabled=false;
             MPBCmbBx->Items->Strings[6] ="o";
             MPBCmbBx->Items->Strings[8] ="r";
             MPBCmbBx->ItemIndex=6;
             FormaCmbBx->ItemIndex=0;
             break;
       case _MILKS_ :
             OriginalRB->Enabled=true;
             MPBCmbBx->Items->Strings[5] ="g";
             MPBCmbBx->Items->Strings[7] ="o"; 
             MPBCmbBx->Items->Strings[8] ="r";
             MPBCmbBx->Items->Strings[9] ="o";
             MPBCmbBx->Items->Strings[11]="r";
             MPBCmbBx->ItemIndex=5;
             FormaCmbBx->ItemIndex=0;
             break;
       case _FSK_:
             MPBCmbBx->Items->Strings[0] ="g";
             MPBCmbBx->Items->Strings[1] ="g";
             MPBCmbBx->Items->Strings[2] ="g";
             MPBCmbBx->Items->Strings[3] ="g";
             MPBCmbBx->Items->Strings[4] ="g";
             MPBCmbBx->Items->Strings[6] ="g";
             MPBCmbBx->Items->Strings[8] ="r";
             MPBCmbBx->ItemIndex=6;
             FormaCmbBx->ItemIndex=2;
             break;
       case _SHAVINGS_SLOW_ :
             MPBCmbBx->Items->Strings[4] ="g";
             MPBCmbBx->Items->Strings[5] ="g";
             MPBCmbBx->Items->Strings[6] ="g";
             MPBCmbBx->Items->Strings[8] ="g";
             MPBCmbBx->ItemIndex=6;
             FormaCmbBx->ItemIndex=2;
             break;
       case _SHAVINGS_DELTA_ :
             TapeErrorChkBx->Enabled=false;
             MPBCmbBx->Items->Strings[5] ="g";
             MPBCmbBx->Items->Strings[6] ="g";
             MPBCmbBx->Items->Strings[7] ="g";
             MPBCmbBx->Items->Strings[9] ="o";
             MPBCmbBx->Items->Strings[11]="r";
             MPBCmbBx->ItemIndex=7;
             FormaCmbBx->ItemIndex=0;
             break;
       case _SHAVINGS_RAUDO_ :
             TapeErrorChkBx->Enabled=false;
             BloquesRB->Enabled=false;
             MPBCmbBx->Items->Strings[7] ="g";
             MPBCmbBx->Items->Strings[8] ="g";
             MPBCmbBx->Items->Strings[9] ="g";
             MPBCmbBx->Items->Strings[11]="o";
             MPBCmbBx->ItemIndex=9;
             FormaCmbBx->ItemIndex=2;
             break;
       case _ULTRA_:
             TapeErrorChkBx->Enabled=false;
             MPBCmbBx->Items->Strings[6] ="g";
             MPBCmbBx->Items->Strings[8] ="r";
             MPBCmbBx->Items->Strings[10]="o";
             MPBCmbBx->Items->Strings[12]="r";
             MPBCmbBx->ItemIndex=6;
             FormaCmbBx->ItemIndex=0;
             break;
       case _NPU_ :
             MPBCmbBx->Items->Strings[8] ="o";
             MPBCmbBx->Items->Strings[10]="o";
             MPBCmbBx->Items->Strings[11]="r";
             MPBCmbBx->Items->Strings[14]="r";
             MPBCmbBx->ItemIndex=8;
             FormaCmbBx->ItemIndex=0;
             break;
       case _FI_:
             MPBCmbBx->Items->Strings[0] ="g";
             MPBCmbBx->Items->Strings[1] ="g";
             MPBCmbBx->Items->Strings[2] ="g";
             MPBCmbBx->Items->Strings[3] ="o";
             MPBCmbBx->Items->Strings[4] ="o";
             MPBCmbBx->Items->Strings[6] ="r";
             MPBCmbBx->ItemIndex=4;
             FormaCmbBx->ItemIndex=0;
             break;
       case _FI_Q_:
             TapeErrorChkBx->Enabled=false;
             MPBCmbBx->Items->Strings[5] ="g";
             MPBCmbBx->Items->Strings[6] ="o";
             MPBCmbBx->Items->Strings[7] ="o";
             MPBCmbBx->Items->Strings[8] ="o";
             MPBCmbBx->Items->Strings[9] ="r";
             MPBCmbBx->Items->Strings[10]="r";
             MPBCmbBx->Items->Strings[11]="r";
             MPBCmbBx->ItemIndex=5;
             FormaCmbBx->ItemIndex=0;
             break;
       case _MANCHESTER_:
             MPBCmbBx->Items->Strings[4] ="g";
             MPBCmbBx->Items->Strings[6] ="g";
             MPBCmbBx->Items->Strings[10]="r";
             MPBCmbBx->ItemIndex=4;
             FormaCmbBx->ItemIndex=4;
             break;
       case _MANCHESTER_DIF_:
             MPBCmbBx->Items->Strings[4] ="g";
             MPBCmbBx->Items->Strings[6] ="g";
             MPBCmbBx->Items->Strings[10]="r";
             MPBCmbBx->ItemIndex=4;
             FormaCmbBx->ItemIndex=4;
             break;
       case _ESCURRIDO_:
             TapeErrorChkBx->Enabled=false;
             MPBCmbBx->Items->Strings[8] ="g";
             MPBCmbBx->Items->Strings[9] ="r";
             MPBCmbBx->Items->Strings[10]="r";
             MPBCmbBx->Items->Strings[12]="r";
             MPBCmbBx->Items->Strings[13]="r";
             MPBCmbBx->ItemIndex=8;
             FormaCmbBx->ItemIndex=4;
             break;
      }

     Memo2->Text=explicacion[MetodoCmbBx->ItemIndex];
     Image1->Canvas->FillRect(Rect(0,0,50,190));
     BorderIL->Draw(Image1->Canvas, 0, 0, MetodoCmbBx->ItemIndex);
     Image1->Invalidate();
}

//---------------------------------------------------------------------------


void __fastcall TSettingsFrm::MPBCmbBxChange(TObject *Sender)
{
     int i= MPBCmbBx->ItemIndex ;
     if (MPBCmbBx->Items->Strings[i]=="x")
       for (;i>=0;i--)
         if (MPBCmbBx->Items->Strings[i]!="x")
            break;
       if (i<0)
        for (i= 0 ;i<14;i++)
         if (MPBCmbBx->Items->Strings[i]!="x")
            break;
    MPBCmbBx->ItemIndex=i;
}

//---------------------------------------------------------------------------
void __fastcall TSettingsFrm::MPBCmbBxDrawItem(TWinControl *Control,
      int Index, TRect &Rect, TOwnerDrawState State)
{

    AnsiString s;

     if (Index==13)
         {
         s= "1.3  (" ;
          s+=Speed(FrecuenciaCmbBx->ItemIndex==0?44100:48000,4); 
         s+=")" ;
         }
    else
        {
         s=m_p_b[Index]/4.0 ;
         s+="    (" ;
         s+=Speed(FrecuenciaCmbBx->ItemIndex==0?44100:48000,m_p_b[Index]);
         s+=")" ;
        }
     int c= MPBCmbBx->Canvas->Brush->Color;

     if (!State.Contains(odSelected))
          {
     if (MPBCmbBx->Items->Strings[Index]=="r")
         MPBCmbBx->Canvas->Brush->Color=clRed;
     if (MPBCmbBx->Items->Strings[Index]=="g")
         MPBCmbBx->Canvas->Brush->Color=clLime;
     if (MPBCmbBx->Items->Strings[Index]=="o")
         MPBCmbBx->Canvas->Brush->Color=clYellow;
          }
     MPBCmbBx->Canvas->FillRect(Rect) ;
     if (MPBCmbBx->Items->Strings[Index]!="x")
        MPBCmbBx->Canvas->TextOutA(Rect.Left+3,Rect.Top,s);
     
     MPBCmbBx->Canvas->Brush->Color=(TColor)c;
}//---------------------------------------------------------------------------

void __fastcall TSettingsFrm::FormaCmbBxDrawItem(TWinControl *Control,
      int Index, TRect &Rect, TOwnerDrawState State)
{
    ImageList1->Draw( FormaCmbBx->Canvas,Rect.Left,Rect.Top,Index,true);
    if (Index>=0)
       FormaCmbBx->Canvas->TextOutA(Rect.Left+30,Rect.Top,
                                    FormaCmbBx->Items->Strings[Index]);
}
