//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "OptionsForm.h"
#include "DirForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOptionsFrm *OptionsFrm;
//---------------------------------------------------------------------------
__fastcall TOptionsFrm::TOptionsFrm(TComponent* Owner)
        : TForm(Owner)
{}
//---------------------------------------------------------------------------

void __fastcall TOptionsFrm::DirBtnClick(TObject *Sender)
{
    DirFrm->ShowModal();
    if (DirFrm->ModalResult==mrOk)
        WavDirEdt->Text=DirFrm->DirectoryListBox1->Directory+"\\";
}
//---------------------------------------------------------------------------

void __fastcall TOptionsFrm::LameBtnClick(TObject *Sender)
{
    if (OpenDialog1->Execute())
        LameEdt->Text=OpenDialog1->FileName;
}
//---------------------------------------------------------------------------

void __fastcall TOptionsFrm::CancelBtnClick(TObject *Sender)
{
    Close();   
}
//---------------------------------------------------------------------------

void __fastcall TOptionsFrm::AceptarBtnClick(TObject *Sender)
{
    ModalResult=mrOk;
}
//---------------------------------------------------------------------------

