//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
                           
#include "wav2.h"      
#include "signaltest.h"
#include "wavmonitorForm.h"

#include "AdjustForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAdjustFrm *AdjustFrm;
//---------------------------------------------------------------------------
__fastcall TAdjustFrm::TAdjustFrm(TComponent* Owner)
   : TForm(Owner)
{
   FrecuenciaCmbBx->ItemIndex=0;
   TestSignalCmbBx->ItemIndex=1;
   FrecuenciaCmbBxChange(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TAdjustFrm::FormaCmbBxDrawItem(TWinControl *Control,
      int Index, TRect &Rect, TOwnerDrawState State)
{
    wavImgLst->Draw( FormaCmbBx->Canvas,Rect.Left,Rect.Top,Index,true);
    if (Index>=0)
        FormaCmbBx->Canvas->TextOutA(Rect.Left+30,Rect.Top,
                                     FormaCmbBx->Items->Strings[Index]);
}
//---------------------------------------------------------------------------
void __fastcall TAdjustFrm::MakeProgramBtnClick(TObject *Sender)
{
   int res=-1;
     switch (Maquina)
     {
      case 'Z':
            ProgramWav= OutputDir+"\\adjust_zx.wav";
            res=adjust_zx(ProgramWav.c_str());
            break;
      case 'C':
            ProgramWav= OutputDir+"\\adjust_cpc.wav";
            res=adjust_cpc(ProgramWav.c_str());
            break;
      case 'M':
            ProgramWav= OutputDir+"\\adjust_msx.wav";
            res=adjust_msx(ProgramWav.c_str());
            break;
      case '8':
            ProgramWav= OutputDir+"\\adjust_81.wav";
            res=adjust_81(ProgramWav.c_str());
            break;
      }

   if (!res)
      ShowMessage(ProgramWav+"\r\nhas been maden");
   else
      ShowMessage("ERROR!!! with\r\n"+ProgramWav);
}
//---------------------------------------------------------------------------
void __fastcall TAdjustFrm::MakeSignalBtnClick(TObject *Sender)
{
    int r;
    char tone [6*0x400];
    char f_c;
    int div=2;


    switch (TestSignalCmbBx->ItemIndex)
    {
     case 1:  f_c=0x55;break;
     case 2:  f_c=0xaa;break;
     case 3:  f_c=0xff;break;
     default: f_c=0x00;break;
    }
    memset (tone,f_c,sizeof (tone));

    s_p_b=_2_50;

    if (InvertChkBx->Checked)
        invert_pol=-1;
    else
        invert_pol=1;

    forma_onda= FormaCmbBx->ItemIndex;
                                   
    SignalName= OutputDir+"\\signal_";//.wav"
    if (FrecuenciaCmbBx->ItemIndex)
    {
        freq_muestreo=48000;
        switch (TestSignalCmbBx->ItemIndex)
        {
         case 1:  SignalName+="12"  ;break;
         case 0 : SignalName+="24"   ; div=1;break;
         default:    ;break;
        }
    }
    else
    {
        freq_muestreo=44100;  
        switch (TestSignalCmbBx->ItemIndex)
        {                              
         case 1:  SignalName+="11"  ;break;
         case 0:  SignalName+="22"  ; div=1 ;break;
         default:    ;break;

        }
    }
    SignalName+="kHz.wav";
    r=open_wav  (SignalName.c_str());

    for (int i=0;i<30;i++)
    {
    pseudopause (900-i);
    pilot_veloz (25);
    block2veloz_wav (tone,sizeof (tone)/div-1);
    }
    close_wav ( );
}
//---------------------------------------------------------------------------
void __fastcall TAdjustFrm::CloseBtnClick(TObject *Sender)
{
     Close();
}
//---------------------------------------------------------------------------

void __fastcall TAdjustFrm::FrecuenciaCmbBxChange(TObject *Sender)
{
    int s=TestSignalCmbBx->ItemIndex;

    TestSignalCmbBx->Items->Clear();
    if (FrecuenciaCmbBx->ItemIndex)
    {
        TestSignalCmbBx->Items->Add("24 kHz");
        TestSignalCmbBx->Items->Add("12 kHz");    /*
        TestSignalCmbBx->Items->Add(" 8 kHz");
        TestSignalCmbBx->Items->Add(" 6 kHz");      */
    }
    else
    {
        TestSignalCmbBx->Items->Add("22 kHz");
        TestSignalCmbBx->Items->Add("11 kHz");/*
        TestSignalCmbBx->Items->Add(" 7 kHz");
        TestSignalCmbBx->Items->Add(" 5 kHz");  */
    }
    TestSignalCmbBx->ItemIndex=s;
}
//---------------------------------------------------------------------------

void __fastcall TAdjustFrm::PlayProgramBtnClick(TObject *Sender)
{
    MakeProgramBtnClick(NULL);
    MonitorFrm->OpenWaveFile(ProgramWav);
    MonitorFrm->PlayBtnClick(NULL);
    MonitorFrm->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TAdjustFrm::PlaySignalBtnClick(TObject *Sender)
{
    MakeSignalBtnClick(NULL);
    MonitorFrm->OpenWaveFile(SignalName);
    MonitorFrm->PlayBtnClick(NULL);
    MonitorFrm->ShowModal();
}
//---------------------------------------------------------------------------

