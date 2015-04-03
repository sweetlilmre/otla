//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("mainForm.cpp", otlaFrm);
USEFORM("InputForm.cpp", InputBlocksDlg);
USEFORM("ModificaForm.cpp", ModificaDlg);
USEFORM("SettingsForm.cpp", SettingsFrm);
USEFORM("DirForm.cpp", DirFrm);
USEFORM("BatchForm.cpp", BatchFrm);
USEFORM("AdjustForm.cpp", AdjustFrm);
USEFORM("wavmonitorForm.cpp", MonitorFrm);
USEFORM("AboutForm.cpp", AboutBox);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
   try
   {
       Application->Initialize();
       Application->CreateForm(__classid(TotlaFrm), &otlaFrm);
       Application->CreateForm(__classid(TInputBlocksDlg), &InputBlocksDlg);
       Application->CreateForm(__classid(TModificaDlg), &ModificaDlg);
       Application->CreateForm(__classid(TSettingsFrm), &SettingsFrm);
       Application->CreateForm(__classid(TDirFrm), &DirFrm);
       Application->CreateForm(__classid(TMonitorFrm), &MonitorFrm);
       Application->CreateForm(__classid(TBatchFrm), &BatchFrm);
       Application->CreateForm(__classid(TAdjustFrm), &AdjustFrm);
       Application->CreateForm(__classid(TAboutBox), &AboutBox);
       Application->Run();
   }
   catch (Exception &exception)
   {
       Application->ShowException(&exception);
   }
   catch (...)
   {
       try
       {
          throw Exception("");
       }
       catch (Exception &exception)
       {
          Application->ShowException(&exception);
       }
   }
   return 0;
}
//---------------------------------------------------------------------------
