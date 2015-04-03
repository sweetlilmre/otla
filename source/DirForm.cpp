//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DirForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDirFrm *DirFrm;
//---------------------------------------------------------------------------
__fastcall TDirFrm::TDirFrm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDirFrm::CancelBtnClick(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TDirFrm::AceptarBtnClick(TObject *Sender)
{
    ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TDirFrm::DirectorioInicial(AnsiString  Directorio)
{
     try{
     DirectoryListBox1->Directory=Directorio;
     }
     catch (...)
     {}
}
