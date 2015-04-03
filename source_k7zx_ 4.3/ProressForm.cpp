//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ProressForm.h"
#include "zxcode.h"
#include "zxwav.h"
#include "zxfiles.h"
#include "SettingdForm.h"




//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TProgresoFrm *ProgresoFrm;



//---------------------------------------------------------------------------
__fastcall TProgresoFrm::TProgresoFrm(TComponent* Owner)
        : TForm(Owner)
{
}
int fin;
//---------------------------------------------------------------------------
void __fastcall __fastcall TProgresoFrm::Proces(TStrings *files,AnsiString out)
{

}

//---------------------------------------------------------------------------

void __fastcall __fastcall TProgresoFrm::ProcesHI()
{
   Show();                   
   Caption="Converting files...";
   Button1->Caption="Cancel";
   
   Label1->Caption="Files converted";
   ProgressBar1->Visible=true;

   fin=0;
   AnsiString line;

   AnsiString temp;
   AnsiString final;
   AnsiString destino=OutputDir;//"d:\\out";
  // MkDir (  destino   );
   destino+="\\";
   temp=destino+"temp.wzx";
   ProgressBar1->Max=FilesLV->Items->Count;
   ProgressBar1->Position=0;

   for (int f=0;(!fin)&&(f<FilesLV->Items->Count);f++)
     {
     ProgressBar1->Position++;
     final=ExtractFileName (FilesLV->Items->Item[f]->Caption);

     if (!readzxfile (FilesLV->Items->Item[f]->Caption.c_str()))
     {
     clearN;
     usrN;
     prg_name;
     if (clearN)
         dir_clear= clearN;
     else
         dir_clear=0xfdbe;
         /*
     if (!usrN)
         dir_usr=7997;
     else
              */
         dir_usr= usrN;

     FilesLV->Items->Item[f]->SubItems->Strings[0]=prg_name;
     FilesLV->Items->Item[f]->SubItems->Strings[1]=dir_usr;
     FilesLV->Items->Item[f]->SubItems->Strings [2]=dir_clear;
     FilesLV->Items->Item[f]->SubItems->Strings [3]=N_trozos;


        if (m_warnings[0])
            FilesLV->Items->Item[f]->SubItems->Strings [5]= m_warnings;
        else
            FilesLV->Items->Item[f]->SubItems->Strings [5]= "OK";

     Application->ProcessMessages();

     snap.tipo_snap;
     pokes [0].dir=0;

     emular=false;

     convierteHI (temp.c_str());

     FilesLV->Items->Item[f]->SubItems->Strings [4]=((int)(duracion*10))/10.0;;

           final+=extra_info;

     final+=".wav";
     FilesLV->Items->Item[f]->Caption=final;
     final=destino+final;



     Show();
     Application->ProcessMessages();

       if (DeleteFile (final.c_str()))
          MoveFile (temp.c_str(),final.c_str());
       else
          CopyFile (temp.c_str(),final.c_str(),false);
       if (mp3)
         {       /*
           AnsiString comando=Lame;
           comando=comando+" ";
           comando=comando+final;
           comando=comando+" -m m -f -b ";
           comando=comando+bitrate;
           if (bitrate==24)
              comando=comando+ " --resample 22.05" ;

           int error=system (comando.c_str());
                   */
     //4.1
           AnsiString app_name=Lame;
           AnsiString comand_line=" \"";
           comand_line=comand_line+final;
           comand_line=comand_line+"\" -m m -f -b ";
           comand_line=comand_line+bitrate;
           if (bitrate==24)
             comand_line=comand_line+ " --resample 22.05" ;
           if (bitrate==32)
             comand_line=comand_line+ " --resample 44.1" ;
           if (bitrate==48)
             comand_line=comand_line+ " --resample 44.1" ;


           STARTUPINFO si = { sizeof(STARTUPINFO) };
           PROCESS_INFORMATION pi;
           try
             {
             Win32Check( CreateProcess( app_name.c_str(),
                             comand_line.c_str(),
                             NULL, NULL,
                             false,
                             0,
                             NULL, NULL,
                             &si, &pi) );

             Win32Check( CloseHandle(pi.hThread) );
             Win32Check( CloseHandle(pi.hProcess) );
             }
             catch(const Exception& e)
             {
             FilesLV->Items->Item[f]->SubItems->Strings [5]= "Error trying lame.exe. "+e.Message;
             }

         }
        }
      else
        {
        FilesLV->Items->Item[f]->SubItems->Strings [5]= m_errors;
        }

     }

    Invalidate();
     Application->ProcessMessages();
     Show();

    ProgressBar1->Visible=false;
    Caption="End of conversion";
    Button1->Caption="OK";
    fin=1;
}//---------------------------------------------------------------------------

void __fastcall __fastcall TProgresoFrm::ProcesNORMAL()
{
   Show();                
   Caption="Converting files...";
   Button1->Caption="Cancel";

   Label1->Caption="Files converted";
   ProgressBar1->Visible=true;
   fin=0;
   AnsiString final;
   AnsiString destino=OutputDir;
//   MkDir (  destino   );
   destino+="\\";


   AnsiString temp=destino+"temp.wzx";
   ProgressBar1->Max=FilesLV->Items->Count;
   ProgressBar1->Position=0;

   for (int f=0;(!fin)&&(f<FilesLV->Items->Count);f++)
     {
     ProgressBar1->Position++;
     
     final=ExtractFileName (FilesLV->Items->Item[f]->Caption);
     Application->ProcessMessages();

     convierteNORMAL (FilesLV->Items->Item[f]->Caption.c_str(),
                      temp.c_str());

     if (m_errors[0])
        FilesLV->Items->Item[f]->SubItems->Strings [5]= m_errors;
     else
        if (m_warnings[0])
            FilesLV->Items->Item[f]->SubItems->Strings [5]= m_warnings;
        else
            FilesLV->Items->Item[f]->SubItems->Strings [5]= "OK";

     FilesLV->Items->Item[f]->SubItems->Strings [4]=((int)(duracion*10))/10.0;;

           final+=extra_info;
     final+=".wav";
     FilesLV->Items->Item[f]->Caption=final;
     final=destino+final; 
          
       if (DeleteFile (final.c_str()))
          MoveFile (temp.c_str(),final.c_str());
       else
          CopyFile (temp.c_str(),final.c_str(),false);

       if (mp3)
         {
           /*     bug in 4.0
           AnsiString comando=Lame;
           comando=comando+" \"";
           comando=comando+final;
           comando=comando+"\" -m m -f -b ";
           comando=comando+bitrate;

           int error=system (comando.c_str());
           if (error)
              FilesLV->Items->Item[f]->SubItems->Strings [5]= error;
              */

     //4.1
           AnsiString app_name=Lame;
           AnsiString comand_line=" \"";
           comand_line=comand_line+final;
           comand_line=comand_line+"\" -m m -f -b ";
           comand_line=comand_line+bitrate;
           if (bitrate==24)
             comand_line=comand_line+ " --resample 22.05" ;
           if (bitrate==32)
             comand_line=comand_line+ " --resample 44.1" ;
           if (bitrate==48)
             comand_line=comand_line+ " --resample 44.1" ;


           STARTUPINFO si = { sizeof(STARTUPINFO) };
           PROCESS_INFORMATION pi;
           try
             {
             Win32Check( CreateProcess( app_name.c_str(),
                             comand_line.c_str(),
                             NULL, NULL,
                             false,
                             0,
                             NULL, NULL,
                             &si, &pi) );

             Win32Check( CloseHandle(pi.hThread) );
             Win32Check( CloseHandle(pi.hProcess) );
             }
             catch(const Exception& e)
             {
             FilesLV->Items->Item[f]->SubItems->Strings [5]= "Error trying lame.exe. "+e.Message;
             }
         }
     }

    Button1->Caption="OK";

    ProgressBar1->Visible=false;

    Caption="End of conversion";
    fin=1;
}

void __fastcall TProgresoFrm::Button1Click(TObject *Sender)
{
    if (fin)

      Close();
    fin=1;
    Button1->Caption="OK";     
    Label1->Caption="Conversion canceled";
}
//---------------------------------------------------------------------------
