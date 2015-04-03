//---------------------------------------------------------------------------

#include <vcl.h>
#include <registry.hpp>
#pragma hdrstop

#include "SettingsForm.h"
#include "DirForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSettingsFrm *SettingsFrm;
//---------------------------------------------------------------------------
__fastcall TSettingsFrm::TSettingsFrm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSettingsFrm::DirZXBtnClick(TObject *Sender)
{
     DirFrm->DirectorioInicial(DirZXEdt->Text);
     DirFrm->ShowModal();
     if (DirFrm->ModalResult==mrOk)
         DirZXEdt->Text=DirFrm->DirectoryListBox1->Directory;
}
//---------------------------------------------------------------------------
void __fastcall TSettingsFrm::DirCPCBtnClick(TObject *Sender)
{
     DirFrm->DirectorioInicial(DirCPCEdt->Text);
     DirFrm->ShowModal();
     if (DirFrm->ModalResult==mrOk)
         DirCPCEdt->Text=DirFrm->DirectoryListBox1->Directory;

}
//---------------------------------------------------------------------------

void __fastcall TSettingsFrm::DirMSXBtnClick(TObject *Sender)
{
     DirFrm->DirectorioInicial(DirMSXEdt->Text);
     DirFrm->ShowModal();
     if (DirFrm->ModalResult==mrOk)
         DirMSXEdt->Text=DirFrm->DirectoryListBox1->Directory;
}
//---------------------------------------------------------------------------
void __fastcall TSettingsFrm::Dir81BtnClick(TObject *Sender)
{
     DirFrm->DirectorioInicial(Dir81Edt->Text);
     DirFrm->ShowModal();
     if (DirFrm->ModalResult==mrOk)
         Dir81Edt->Text=DirFrm->DirectoryListBox1->Directory;

}
//---------------------------------------------------------------------------

void __fastcall TSettingsFrm::Rom2BinBtnClick(TObject *Sender)
{
      if(OpenDialog1->Execute())
         Rom2BinEdt->Text=OpenDialog1->FileName;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsFrm::CancelBtnClick(TObject *Sender)
{
      Close();  
}
//---------------------------------------------------------------------------

void __fastcall TSettingsFrm::AceptarBtnClick(TObject *Sender)
{
    TRegistry *reg;
    reg= new TRegistry();
    try
    {     /*
      reg->RootKey = HKEY_CLASSES_ROOT;
      if (ExtensionChkBx->Checked)
      {
      if ( reg->OpenKey( ".SBB", True ))
      {
        reg->WriteString("", "SBB.Files" );  // goes into default value
        reg->CloseKey();
        if ( reg->OpenKey( "SBB.Files", True ))
        {
          reg->WriteString("", "SBB Data file" ); // type shown inexplorer
          reg->WriteString("DefaultIcon",
                          Application->ExeName);//"C:\programs\XYZ Application\xyz.exe,5");
          reg->CloseKey();
        }
      }
      }
      else
      {
         reg->DeleteKey( ".SBB" ) ;
         reg->DeleteKey( "SBB.Files" ) ;
      }       */
   if (ExtensionChkBx->Checked)
   {
   reg->RootKey=HKEY_CLASSES_ROOT;

   reg->OpenKey("OTLA.SBB",True);
   reg->WriteString("","OTLA Stream of Binary Blocks");
   reg->CloseKey();

   reg->OpenKey("OTLA.SBB\\DefaultIcon",True);
   reg->WriteString("",ParamStr(0)+",0");
   reg->CloseKey();

   reg->OpenKey("OTLA.SBB\\Shell",True);
   reg->WriteString("","Open");
   reg->CloseKey();

   reg->OpenKey("OTLA.SBB\\Shell\\Open",True);
   reg->WriteString("","");
   reg->CloseKey();

   reg->OpenKey("OTLA.SBB\\Shell\\Open\\command",True);
   reg->WriteString("","\""+ParamStr(0)+"\" \"%1\"");
   reg->CloseKey();

   reg->OpenKey(".sbb",True);
   reg->WriteString("","OTLA.SBB");

   reg->CloseKey();

   reg->RootKey=HKEY_CURRENT_USER;

   reg->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.sbb",True);
   reg->WriteString("Progid","OTLA.SBB");
   reg->CloseKey();
   }
   else
   {
   reg->RootKey=HKEY_CLASSES_ROOT;
   reg->DeleteKey("OTLA.SBB");   
   reg->RootKey=HKEY_CURRENT_USER;
   reg->DeleteKey("Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.sbb");
   }

 //  SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL) ;
   }

    __finally
    {
      delete reg;
      ModalResult=mrOk;
    }   
}
/*
procedure RegisterFileType(EN:String);
var
   reg:TRegistry;

begin

  reg := TRegistry.Create;
  try
   reg->RootKey:=HKEY_CLASSES_ROOT;

   reg->OpenKey("OTLA.SBB",True);
   reg->WriteString("","OTLA Stream of Binary Blocks');
   reg->CloseKey;

   reg->OpenKey("OTLA.SBB\DefaultIcon",True);
   reg->WriteString("",ParamStr(0)+",0");
   reg->CloseKey;

   reg->OpenKey("OTLA.SBB\Shell",True);
   reg->WriteString("","Open");
   reg->CloseKey;

   reg->OpenKey("OTLA.SBB\Shell\Open",True);
   reg->WriteString("","");
   reg->CloseKey;

   reg->OpenKey("OTLA.SBB\Shell\Open\command",True);
   reg->WriteString("","\""+ParamStr(0)+"\" \"%1\"");
   reg->CloseKey;

   reg->OpenKey(".sbb",True);
   reg->WriteString("","OTLA.SBB");

   reg->CloseKey;

   reg->RootKey:=HKEY_CURRENT_USER;

   reg->OpenKey("Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.sbb",True);
   reg->WriteString("Progid","OTLA.SBB");
   reg->CloseKey;

   SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL) ;
  finally
   reg->Free;
  end;
*/

//---------------------------------------------------------------------------


void __fastcall TSettingsFrm::DiskMngrBtnClick(TObject *Sender)
{
      if(OpenDialog1->Execute())
         DiskMgrEdt->Text=OpenDialog1->FileName;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsFrm::CPCToolBtnClick(TObject *Sender)
{
      if(OpenDialog1->Execute())
         CPCToolEdt->Text=OpenDialog1->FileName;
}

//---------------------------------------------------------------------------

void __fastcall TSettingsFrm::DirBtnClick(TObject *Sender)
{   
     DirFrm->DirectorioInicial(WavDirEdt->Text);
     DirFrm->ShowModal();
     if (DirFrm->ModalResult==mrOk)
         WavDirEdt->Text=DirFrm->DirectoryListBox1->Directory;
   
}
//---------------------------------------------------------------------------

void __fastcall TSettingsFrm::LameBtnClick(TObject *Sender)
{   
      if(OpenDialog1->Execute())
         LameEdt->Text=OpenDialog1->FileName;
   
}
//---------------------------------------------------------------------------



/*
RegisterFileType('avi');

procedure RegisterFileType(EN:String);
var
   reg:TRegistry;

begin
  EN:=UpperCase(EN);

  reg := TRegistry.Create;
  try
   reg.RootKey:=HKEY_CLASSES_ROOT;

   reg.OpenKey('HyperCube.'+EN,True);
   reg.WriteString('','HyperCube Media File');
   reg.CloseKey;

   reg.OpenKey('HyperCube.'+EN+'\DefaultIcon',True);
   reg.WriteString('',ParamStr(0)+',0');
   reg.CloseKey;

   reg.OpenKey('HyperCube.'+EN+'\Shell',True);
   reg.WriteString('','Open');
   reg.CloseKey;

   reg.OpenKey('HyperCube.'+EN+'\Shell\Open',True);
   reg.WriteString('','');
   reg.CloseKey;

   reg.OpenKey('HyperCube.'+EN+'\Shell\Open\command',True);
   reg.WriteString('','"'+ParamStr(0)+'" "%1"');
   reg.CloseKey;

   reg.OpenKey('.'+LowerCase(EN),True);
   reg.WriteString('','HyperCube.'+EN);

   reg.CloseKey;

   reg.RootKey:=HKEY_CURRENT_USER;

   reg.OpenKey('Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.'+LowerCase(EN),True);
   reg.WriteString('Progid','HyperCube.'+EN);
   reg.CloseKey;

   SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, nil, nil) ;
  finally
   reg.Free;
  end;
end;
*/
