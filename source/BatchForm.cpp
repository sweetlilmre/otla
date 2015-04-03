//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BatchForm.h"  
#include "DirForm.h"
#include "loaders.h"       

#include "zxfiles.h"
#include "cpcfiles.h"
#include "msxfiles.h"  
#include "81files.h"
#include "mainForm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBatchFrm *BatchFrm;


ts_sbb_header  batch_head;
ts_sbb_block   batch_blocks[_MAX_SBB_BLOCKS_+1] ;
//---------------------------------------------------------------------------
__fastcall TBatchFrm::TBatchFrm(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TBatchFrm::InputDirBtnClick(TObject *Sender)
{
     DirFrm->DirectorioInicial(InputDirEdt->Text);
     DirFrm->ShowModal();
     if (DirFrm->ModalResult==mrOk)
         InputDirEdt->Text=DirFrm->DirectoryListBox1->Directory;

}
//---------------------------------------------------------------------------
void __fastcall TBatchFrm::OutputDirBtnClick(TObject *Sender)
{
     DirFrm->DirectorioInicial(OutputDirEdt->Text);
     DirFrm->ShowModal();
     if (DirFrm->ModalResult==mrOk)
         OutputDirEdt->Text=DirFrm->DirectoryListBox1->Directory;

}
//---------------------------------------------------------------------------
void __fastcall TBatchFrm::ProccedBtnClick(TObject *Sender)
{
   if (Tipo==1)
      M_y_E=(Maquina<<8) + ExtensionCmbBx->Text[2];
   ProccedBtn->Enabled=false;
    if (Tipo==1)
         ConverToSbb( );
    if (Tipo==2)
         ConverToWav( );
   ProccedBtn->Enabled=true;
}
int analiza_tap()
{
   int res=0;
   int b;
    for (b=0;b<_MAX_SBB_BLOCKS_;b++)
    {
    if (b>batch_head.n_blocks||0==batch_blocks[0].block_type)
       break;

    if (_zx_program_==batch_blocks[0].block_type)
      if (1==batch_head.n_blocks)
      {
         zxbasic2bytes(batch_blocks);
         batch_head.usr_pc= batch_blocks->exec;
         break;
      }
      else
      {
          for (int i=b ; i<_MAX_SBB_BLOCKS_ ; i++)
         {
           batch_blocks[i]=batch_blocks[i+1];
         }
         b--;       
         batch_head.n_blocks--;
         continue;
      }
    
    if (_zx_tap_hdlss_==batch_blocks[b].block_type)
    {
      res=-11;
      break;
    }
    if ( batch_blocks[b].ini+batch_blocks[b].size -1 >= MAX_DIR_ZX )
    {
        for (int i=_MAX_SBB_BLOCKS_ ; i>b ; i--)
        {
           batch_blocks[i]=batch_blocks[i-1];
        }
        zxlastbytes(&batch_head,&batch_blocks[b]);
        batch_head.n_blocks++;
        b++;
    }
    }
   return res;
}
//---------------------------------------------------------------------------
int __fastcall TBatchFrm::BuildSbb(AnsiString File )
{
   int res;
     switch (M_y_E)
     {            
          case 0x5a54: res=leetap (File.c_str(),&batch_head,batch_blocks);
                       res=analiza_tap();
            break;
          case 0x5a53: res=lee_zx_sna (File.c_str(),&batch_head,batch_blocks);
            break;
          case 0x5a5a: res=leez80 (File.c_str(),&batch_head,batch_blocks);
            break;
          case 0x4342: res=lee_cpc_bin (File.c_str(),&batch_head,batch_blocks);
            break;
          case 0x4353: res=lee_cpc_sna (File.c_str(),&batch_head,batch_blocks);
            break;
          case 0x4d42: res=lee_msx_bin (File.c_str(),&batch_head,batch_blocks);
            break;
          case 0x4d43: res=leecas (File.c_str(),&batch_head,batch_blocks);
            break;
          case 0x4d52: res=lee_msx_rom (File.c_str(),&batch_head,batch_blocks ,NULL);
            break;
          case 0x3850: res=lee_p (File.c_str(),&batch_head,batch_blocks);
            break;
          case 0x385a: res=lee_z81 (File.c_str(),&batch_head,batch_blocks);
            break;
          default : res=-1;
     }
     return res;
}
//---------------------------------------------------------------------------
void __fastcall TBatchFrm::ConverToSbb( )
{
  int res;
  TSearchRec sr;
  AnsiString linea;
  AnsiString FileIn;
  AnsiString FileOut;
  AnsiString mask= InputDirEdt->Text; //ExtractFilePath(ToolRom2Bin) +"*.bin";
  mask+="\\*";
  mask+=ExtensionCmbBx->Text;

  ProgressBar1->Position=0;
  ProgressBar1->Max=N_Files(mask);


  if (FindFirst( mask ,  faArchive  ,sr) == 0)
  {
    do
    {
       ProgressBar1->Position++;
         linea="Converting";
         linea+=sr.Name;
        FileIn=InputDirEdt->Text+"\\";
        FileIn+=sr.Name;
        FileOut=OutputDirEdt->Text+"\\";
        FileOut+=ChangeFileExt (sr.Name,"")+".sbb";

         res= BuildSbb(FileIn);
         if (!res)
            res=save_sbb_file (FileOut.c_str(),&batch_head,batch_blocks);

        if (res)
        {
         linea+="  error";
        }
        else
         linea+="  OK";
        Memo1->Lines->Add(linea);

    } while (FindNext(sr) == 0);
    FindClose(sr);
  }

}
//---------------------------------------------------------------------------
void __fastcall TBatchFrm::ConverToWav( )
{
  int res;
  TSearchRec sr;
  AnsiString linea;
  AnsiString FileIn;
  AnsiString FileOut;
  AnsiString mask= InputDirEdt->Text+"\\*.sbb";
  ProgressBar1->Position=0;
  ProgressBar1->Max=N_Files(mask);

  if (FindFirst( mask ,  faArchive  ,sr) == 0)
  {
    do
    {
        ProgressBar1->Position++;
        FileIn=InputDirEdt->Text+"\\";
        FileIn+=sr.Name;
        FileOut=OutputDirEdt->Text+"\\";
        FileOut+=ChangeFileExt (sr.Name,"")+".wav";
         linea="converting";
         linea+=sr.Name;
        res=sbb2wav (FileIn.c_str() ,FileOut.c_str() );
        if (res)
        {
         linea+="  error";
        }
        else
        {
         linea+="  OK";
         if (MakeMp3ChkBx->Checked)
             otlaFrm->LameProcess (FileOut, Maquina , nombre_head);
        }
        Memo1->Lines->Add(linea);

    } while (FindNext(sr) == 0);
    FindClose(sr);
  }
}
//---------------------------------------------------------------------------
int  __fastcall TBatchFrm::N_Files(AnsiString mask )
{   
   TSearchRec sr;
   int n=0;
   if (FindFirst( mask ,  faArchive  ,sr) == 0)
   {
      do
         n++;
      while (FindNext(sr) == 0);
      FindClose(sr);
   }
  return n;
}
//---------------------------------------------------------------------------
void __fastcall TBatchFrm::Batch ( char m , int t )
{
    Maquina=m;
    Tipo=t;
    Memo1->Clear();
    ExtensionCmbBx->Items->Clear();
    switch (m)
    {
       case 'Z':
         ExtensionCmbBx->Items->Add(".Z80");
         ExtensionCmbBx->Items->Add(".SNA");
         ExtensionCmbBx->Items->Add(".TAP");
         break;
       case 'C':
         ExtensionCmbBx->Items->Add(".SNA");
         ExtensionCmbBx->Items->Add(".BIN");
         break;
       case 'M':
         ExtensionCmbBx->Items->Add(".ROM");
         ExtensionCmbBx->Items->Add(".CAS");
         ExtensionCmbBx->Items->Add(".BIN");
         break;
       case '8':
         ExtensionCmbBx->Items->Add(".P");
         ExtensionCmbBx->Items->Add(".Z81");
         break;
    }
    Label2->Caption=AnsiString ("Select file type for ")+ NombreMachine(m);;
    ExtensionCmbBx->ItemIndex=0;
    if (t==1)
    {
      Caption=" Converting to .SBB ...";
      InputLbl->Caption="Input dir";
      OutputLbl->Caption="Output dir for .sbb files";
      Label2->Visible=1;
      ExtensionCmbBx->Visible=1;
      MakeMp3ChkBx->Visible=0;
    }
    else
    {                                 
      Caption=" Converting to .Wav ...";
      InputLbl->Caption="Input dir of .sbb files";
      OutputLbl->Caption="Output dir for .wav files";
      Label2->Visible=0;
      ExtensionCmbBx->Visible=0;  
      MakeMp3ChkBx->Visible=1;
    }
    ShowModal();
}