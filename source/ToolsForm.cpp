//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ToolsForm.h"
#include "DirForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TToolsFrm *ToolsFrm;
//---------------------------------------------------------------------------
__fastcall TToolsFrm::TToolsFrm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TToolsFrm::DirZXBtnClick(TObject *Sender)
{
     DirFrm->ShowModal();
     if (DirFrm->ModalResult==mrOk)
         DirZXEdt->Text=DirFrm->DirectoryListBox1->Directory;
}
//---------------------------------------------------------------------------
void __fastcall TToolsFrm::DirCPCBtnClick(TObject *Sender)
{  
     DirFrm->ShowModal();
     if (DirFrm->ModalResult==mrOk)
         DirCPCEdt->Text=DirFrm->DirectoryListBox1->Directory;

}
//---------------------------------------------------------------------------

void __fastcall TToolsFrm::DirMSXBtnClick(TObject *Sender)
{   
     DirFrm->ShowModal();
     if (DirFrm->ModalResult==mrOk)
         DirMSXEdt->Text=DirFrm->DirectoryListBox1->Directory;
}
//---------------------------------------------------------------------------
void __fastcall TToolsFrm::Dir81BtnClick(TObject *Sender)
{  
     DirFrm->ShowModal();
     if (DirFrm->ModalResult==mrOk)
         Dir81Edt->Text=DirFrm->DirectoryListBox1->Directory;

}
//---------------------------------------------------------------------------

void __fastcall TToolsFrm::Rom2BinBtnClick(TObject *Sender)
{
      if(OpenDialog1->Execute())
         Rom2BinEdt->Text=OpenDialog1->FileName;
}
//---------------------------------------------------------------------------

void __fastcall TToolsFrm::CancelBtnClick(TObject *Sender)
{
      Close();  
}
//---------------------------------------------------------------------------

void __fastcall TToolsFrm::AceptarBtnClick(TObject *Sender)
{
     ModalResult=mrOk;   
}
//---------------------------------------------------------------------------


void __fastcall TToolsFrm::DiskMngrBtnClick(TObject *Sender)
{
      if(OpenDialog1->Execute())
         DiskMgrEdt->Text=OpenDialog1->FileName;
}
//---------------------------------------------------------------------------

void __fastcall TToolsFrm::CPCToolBtnClick(TObject *Sender)
{
      if(OpenDialog1->Execute())
         CPCToolEdt->Text=OpenDialog1->FileName;
}

//---------------------------------------------------------------------------

