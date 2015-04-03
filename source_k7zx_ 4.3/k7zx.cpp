//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("about.cpp", AboutBox);
USEFORM("SettingdForm.cpp", SettingsFrm);
USEFORM("PlayerForm.cpp", PlayerFrm);
USEFORM("DirForm.cpp", DirFrm);
USEFORM("ProressForm.cpp", ProgresoFrm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
     try
     {
           Application->Initialize();
           Application->Title = "k7zx";
                 Application->CreateForm(__classid(TPlayerFrm), &PlayerFrm);
                 Application->CreateForm(__classid(TSettingsFrm), &SettingsFrm);
                 Application->CreateForm(__classid(TAboutBox), &AboutBox);
                 Application->CreateForm(__classid(TDirFrm), &DirFrm);
                 Application->CreateForm(__classid(TProgresoFrm), &ProgresoFrm);
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



