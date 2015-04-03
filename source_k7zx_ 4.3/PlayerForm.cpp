//---------------------------------------------------------------------------

#include <vcl.h>
#include <mmsystem.h>
#include <inifiles.hpp>

#pragma hdrstop

#include "PlayerForm.h"
#include "SettingdForm.h"
#include "about.h"
#include "zxwav.h"
#include "zxcode.h"
#include "zxfiles.h"
#include "DirForm.h"
#include "ProressForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPlayerFrm *PlayerFrm;
//---------------------------------------------------------------------------
__fastcall TPlayerFrm::TPlayerFrm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TPlayerFrm::Settings1Click(TObject *Sender)
{
     SettingsFrm->ShowModal();
     FormShow(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TPlayerFrm::About1Click(TObject *Sender)
{
        AboutBox->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TPlayerFrm::FormCreate(TObject *Sender)
{                 
   TIniFile *ini;
   ini = new TIniFile( ChangeFileExt( Application->ExeName, ".INI" ) );
   Top     =  ini->ReadInteger( "Form", "Top", 100 );
   Left    =  ini->ReadInteger( "Form", "Left", 100 );
   ini->ReadBool( "Form", "InitMax", false ) ?
         WindowState = wsMaximized :
         WindowState = wsNormal;
   InfoChkBx->Checked  =ini->ReadBool ( "Ops", "InfoinFile", true);
   SelectedChkBx->Checked  =ini->ReadBool ( "Ops", "OnlySelected", true);

   try {
   DirectoryListBox1->Directory=
   ini->ReadString ( "Files", "Path", ExtractFilePath (Application->ExeName)+"samples\\screen_test\\" );
   }
   catch (...){}
   OutputEdt->Text=ini->ReadString ( "Files", "Output", ExtractFilePath (Application->ExeName)+"output" );
   LameEdt->Text  =ini->ReadString ( "Files", "Lame", ExtractFilePath (Application->ExeName)+"lame\\lame.exe" );
   delete ini;
}
//---------------------------------------------------------------------------

void __fastcall TPlayerFrm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
   readzxfile ("");
   TIniFile *ini;
   ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ) );


   ini->WriteInteger( "Form", "Top", Top );
   ini->WriteInteger( "Form", "Left", Left );
   ini->WriteBool   ( "Form", "InitMax", WindowState == wsMaximized );

   ini->WriteString ( "Files", "Path", DirectoryListBox1->Directory );
   ini->WriteString ( "Files", "OutPut", OutputEdt->Text);
   ini->WriteString ( "Files", "lame", LameEdt->Text );

   
   ini->WriteBool ( "Ops", "InfoinFile", InfoChkBx->Checked  );
   ini->WriteBool ( "Ops", "OnlySelected", SelectedChkBx->Checked);

   delete ini;

}

AnsiString flags()
{
 AnsiString res="Settings: ";
 res+=FqMuestreo;
 res+="Hz W";
 res+=(int)forma_onda;
 res+= " P";
 if (signo==1)
     res+="+ F";
 else
     res+="- F";
 if (tono_final==1)
     res+="1 S";
 else
     res+="0 S";
 if (CD==1)
     res+="1 A";
 else
     res+="0 A";
 if (acelerar_basic==1)
     res+="1 L";
 else
     res+="0 L";
 if (cargador==1)
     res+="1 C";
 else
     res+="0 C";
 if (control_chksum==1)
     res+="1 B";
 else
     res+="0 B";
 res+=  (int ) variosbloques;

 return res;

}
//---------------------------------------------------------------------------

void __fastcall TPlayerFrm::FormShow(TObject *Sender)
{

  BitRateLbl->Caption="bitrate " +AnsiString (bitrate)+" kbps";
  switch (conversion)
    {
    case 0 :
       FileListBox1->Mask= "*.tap;*.tzx";
       StatusBarra->Panels->Items[0]->Text="NORMAL";
       StatusBarra->Panels->Items[1]->Text="only .tap .tzx";
       StatusBarra->Panels->Items[2]->Text="";
       StatusBarra->Panels->Items[3]->Text=flags();//"W2 +T -CD -F";
       GroupBox1->Visible=false;
       PlayerFrm->Width=400;
       break;
    case 1 :         
       FileListBox1->Mask= "*.tap;*.tzx;*.sna;*.z80;*.hex;*.sbb";
       StatusBarra->Panels->Items[0]->Text="HiSpeed";
       StatusBarra->Panels->Items[1]->Text=tecnica[metodo];
       StatusBarra->Panels->Items[2]->Text=SettingsFrm->Speed(FqMuestreo,muestras_por_bit);
       StatusBarra->Panels->Items[3]->Text=flags();
       GroupBox1->Visible=true;
       PlayerFrm->Width=589;
       break;
    case 2 :
       FileListBox1->Mask= "*.tap;*.tzx;*.sna;*.z80;*.hex;*.sbb";
       StatusBarra->Panels->Items[0]->Text="Ma non troppo";
       StatusBarra->Panels->Items[1]->Text=tecnica[metodo];
       StatusBarra->Panels->Items[2]->Text="";
       StatusBarra->Panels->Items[3]->Text=flags();
       GroupBox1->Visible=true;
       PlayerFrm->Width=589;
       break;
    }

   switch (variosbloques)
         {
           case _CARGADOR_ORIGINAL_:
               if (metodo==_MILKS_)
                  {
                  esquema_bloques=_CARGADOR_ORIGINAL_;
                  break;
                  }
           case _VARIOSBLOQUES_:
               if ((metodo!=_SHAVINGS_RAUDO_))
                  {
                  esquema_bloques=_VARIOSBLOQUES_;
                  break;
                  }
           default:
               esquema_bloques=_UNBLOQUE_;
               break;
         }
   //FileListBox1Change(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TPlayerFrm::FileListBox1Change(TObject *Sender)
{
  int error;
  PokeadorChkBx->Checked=false;
  Label6->Caption="";
  StopBtn->Enabled=false;
  PlayBtn->Enabled=false;
  EmularBtn->Enabled=false;

  File2WAVBtn->Enabled=false;
  File2MP3Btn->Enabled=false;

  Memo1->Clear();
  TrozosCLB->Clear();
  NombreEdt->Text="";
  ClearEdt->Text="";//0;
  UsrEdt->Text="";//0;

 if (FileListBox1->FileName!=NULL)
   if (!(error=readzxfile (FileListBox1->FileName.c_str())))
     {
     if (m_errors[0])
        Memo1->Text= m_errors;
     else
        if (m_warnings[0])
           Memo1->Text= m_warnings;
        else
           Memo1->Text="0k";
           
     if (clearN)
         ClearEdt->Text=clearN;
     else
         ClearEdt->Text="";
         
     if (usrN)
         UsrEdt->Text=usrN;
     else
         UsrEdt->Text="";
     NombreEdt->Text=prg_name;
     //UnBloqueRB->Checked=true;
     SnapGB->Visible=snap.tipo_snap;
     if (snap.tipo_snap)
       {
       if (snap.tipo_snap==1)
          SnapGB->Caption="Snapshot 48k";
       else
          SnapGB->Caption="Snapshot 128k";
       PCLbl->Caption="PC: $"+IntToHex (snap.PC,4);
       SPLbl->Caption="SP: $"+IntToHex (snap.SP,4);
       CurrentLbl->Caption="out ($7FFD): $"+IntToHex (snap.pagina,2);
       }
     int i;
     for (i=0; i<7;i++)
      {
      AnsiString t;
      if (trozos[i].tipo<0)
        break;
      t=trozos[i].tipo;
      if (trozos[i].tipo=='0')
         {
         t+=" -Basic auto: ";
         t+=trozos[i].auto_run;
         t+=" long: ";
         t+=trozos[i].longitud-60;
         t+=" vars: ";
         t+=trozos[i].pam_2;
       //  verBasic (trozos[i].data, trozos[i].longitud-60);
         }
      else
         {
         t+=" -Bytes dir: ";
         t+=trozos[i].inicio;
         t+=" long: ";
         t+=trozos[i].longitud;
         }

      TrozosCLB->Items->Add(t);
      TrozosCLB->Checked[i]=trozos[i].procesar;
      }
     PlayBtn->Enabled=true;
    // StopBtn->Enabled=true;
     EmularBtn->Enabled=true;
     File2WAVBtn->Enabled=true;
     File2MP3Btn->Enabled=true;
     }
    else
      {
      Label6->Caption=error;
      }
     //PlayBtn->Enabled=true;
     //EmularBtn->Enabled=true;
     //}
  if (!SelectedChkBx->Checked)
    {
    File2WAVBtn->Enabled=true;
    File2MP3Btn->Enabled=true;
    }
}

//-------------------------------------------------------------------------
int  __fastcall TPlayerFrm::makewav()
{

   int basics=0;
   int n_t=0;
   int t;
   int error=0;

   strncpy (prg_name, NombreEdt->Text.c_str(),10);
   dir_clear= ClearEdt->Text.ToIntDef(0xfdbe);//(0xfdbe);
   dir_usr= UsrEdt->Text.ToIntDef(0);//(7997);

   for (t=0;t<TrozosCLB->Items->Count;t++)
        {
        trozos[t].procesar=0;
        if (TrozosCLB->Checked[t])
          {
          if (trozos[t].tipo == '3')
              {trozos[t].procesar=1;n_t++;}
          if (trozos[t].tipo == '0')
               //OJO if (esquema==_BASIC_Y_BLOQUES_)
                   {basics++;trozos[t].procesar=1;n_t++;}
          if (trozos[t].tipo == '5')
              trozos[t].procesar=0;//{error_men+="Bloque sin cabecera. ";error++;break;}
          }
        }

   trozos[t].tipo=-1;      
   if ((esquema_bloques==_CARGADOR_ORIGINAL_)&&(basics==0))
       esquema_bloques=_VARIOSBLOQUES_;

   int n,p=0;

   if (PokeadorChkBx->Checked)
     for (int i=1; i< PokesVLE->RowCount; i++)
      {
      n= PokesVLE->Cells[0][i].ToIntDef(-1);
      if (n>=0&&n<=0xffff)
        {
        pokes [p].dir=n;
        n= PokesVLE->Cells[1][i].ToIntDef(-1);
        if (n>=0&&n<=0xff)
          {
          pokes [p].value=n;
          p++;
          }
        }
      if (i>_MAX_POKES_)
         ShowMessage ("Too many pokes");
      }

   pokes [p].dir=0;
   
   AnsiString file_in =FileListBox1->FileName;
  file_out=OutputEdt->Text+"\\";
   if (emular)
      file_out+= "temp.tzx" ;
   else
      file_out+= "temp.wav" ;

   if (conversion==0)
      error=convierteNORMAL ( file_in.c_str() ,file_out.c_str());
   else
      error=convierteHI ( file_out.c_str());

   if (error )
       ShowMessage(m_errors);//(error);
   else
     if (emular)
      {
      ShellExecute(NULL,"open",file_out.c_str(),NULL,NULL,SW_SHOW );
      }
     else
      {
      if (play)
         { /*
         if (sndPlaySound (file_out.c_str(), SND_APPLICATION | SND_ASYNC))
                PlayBtn->Enabled=false;
         else
                ShellExecute(NULL,"open",file_out.c_str(),NULL,NULL,SW_SHOW );
       */  }
      else
         {
         file_final=OutputEdt->Text+"\\"+ExtractFileName(FileListBox1->FileName);//*info
         if (InfoChkBx->Checked)
             file_final+=info;
         file_final+=".wav";
         if (DeleteFile (file_final.c_str()))
             MoveFile (file_out.c_str(),file_final.c_str());
         else
             CopyFile (file_out.c_str(),file_final.c_str(),false);
         if (mp3)
            {
            //file_final.c_str()===>mp3
            /*
           AnsiString comando=LameEdt->Text;
           comando=comando+" \"";
           comando=comando+file_final;
           comando=comando+"\" -m m -f -b ";
           comando=comando+bitrate;
           if (bitrate==24)
             comando=comando+ " --resample 22.05" ;
           if (bitrate==32)
             comando=comando+ " --resample 44.1" ;
           if (bitrate==48)
             comando=comando+ " --resample 44.1" ;

           int error=system (comando.c_str());
           if (error)
              ShowMessage ("Error trying mp3 encoder");
              */

        //4.1
           AnsiString app_name=LameEdt->Text;
           AnsiString comand_line=" \"";
           comand_line=comand_line+file_final;
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
             ShowMessage("Error trying lame.exe\r\n"+e.Message);
             }
           }
         }
      }
   return error;

}
//---------------------------------------------------------------------------

void __fastcall TPlayerFrm::VerBarra(bool ver )
{
    if (ver)
     {
     ProgressBar1->Visible=true;
     LoaderLbl->Visible=true;
     DuraLbl->Visible=true;
     LoaderLbl->Width=(ProgressBar1->Width*duracion_loader)/duracion;
     LoaderLbl->Caption= ((int)(duracion_loader*10))/10.0;;
     DuraLbl->Caption=((int)(duracion*10))/10.0; ;

     ProgressBar1->Max=MediaPlayer1->Length;
     }
    else
     {
     ProgressBar1->Visible=false;
     LoaderLbl->Visible=false;
     DuraLbl->Visible=false;
     }
}

//---------------------------------------------------------------------------

void __fastcall TPlayerFrm::PlayBtnClick(TObject *Sender)
{

    DriveComboBox1->Enabled=false;
    DirectoryListBox1->Enabled=false;
    FileListBox1 ->Enabled=false;
     PlayBtn->Enabled=false;
     EmularBtn->Enabled=false;
     File2WAVBtn->Enabled=false;
     File2MP3Btn->Enabled=false;

     emular=false;
     play=true;
     Label6->Caption="Creating wav..." ;
     Application->ProcessMessages();
     if (makewav())
        {
        Label6->Caption="error creating wav" ;
        DriveComboBox1->Enabled=true;
        DirectoryListBox1->Enabled=true;
        FileListBox1 ->Enabled=true;
        PlayBtn->Enabled=true;
        EmularBtn->Enabled=true;
        File2WAVBtn->Enabled=true;
        File2MP3Btn->Enabled=true;
        }
     else
     {
     Label6->Caption="Playing..." ;
     Application->ProcessMessages();
     
     StopBtn->Enabled=true;
     MediaPlayer1->FileName=file_out;
     MediaPlayer1->Notify=true;
     MediaPlayer1->Open();      
       MediaPlayer1->Notify=true;
     if (!MediaPlayer1->Error)
       {
       MediaPlayer1->Play();      
       MediaPlayer1->Notify=true;
       if (!MediaPlayer1->Error)
         {
         Timer1->Enabled=true;
         VerBarra(true);
         Timer1Timer(NULL);
         }
       else   
         Label6->Caption="Error Play mediaPlayer..." ;
       }
     else
        Label6->Caption="Error Open mediaPlayer..." ;
     }
}
//---------------------------------------------------------------------------
void __fastcall TPlayerFrm::StopBtnClick(TObject *Sender)
{
    // PlaySound (NULL,NULL,0);
     MediaPlayer1->Stop();
     MediaPlayer1->Notify=true;

}
//---------------------------------------------------------------------------

void __fastcall TPlayerFrm::EmularBtnClick(TObject *Sender)
{
     PlayBtn->Enabled=false;
     EmularBtn->Enabled=false;
    File2WAVBtn->Enabled=false;
    File2MP3Btn->Enabled=false;

     emular=true;
     
     Label6->Caption="Creating tzx..." ;
     Application->ProcessMessages();
    if ( makewav())
       Label6->Caption="error creating tzx" ;
    else
      Label6->Caption="" ;

     EmularBtn->Enabled=true;
     PlayBtn->Enabled=true;
    File2WAVBtn->Enabled=true;
    File2MP3Btn->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TPlayerFrm::File2WAVBtnClick(TObject *Sender)
{
     PlayBtn->Enabled=false;
     EmularBtn->Enabled=false;
    File2WAVBtn->Enabled=false;
    File2MP3Btn->Enabled=false;
     emular=false;
     play=false;
     mp3=false;
   if (SelectedChkBx->Checked)
     {
      makewav();
       AnsiString s=ExtractFileName(file_final);
       s+="\r\n Duración: ";
       s+=((int)(duracion*10))/10.0;
       s+="segs. \r\n";//+error_men;
       ShowMessage(s.c_str());

     }
   else
     {
     ProgresoFrm->mp3=false;
     ProgresoFrm->OutputDir=OutputEdt->Text;
     if (InfoChkBx->Checked)
        ProgresoFrm->extra_info= info;
     else
       ProgresoFrm->extra_info= "";
     FillProgres( );
     if (conversion)
       ProgresoFrm->ProcesHI();
     else
       ProgresoFrm->ProcesNORMAL(); 
     }

     PlayBtn->Enabled=true;
     EmularBtn->Enabled=true;
    File2WAVBtn->Enabled=true;
    File2MP3Btn->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TPlayerFrm::File2MP3BtnClick(TObject *Sender)
{

     emular=false;
     play=false;
     mp3=true;
     PlayBtn->Enabled=false;
     EmularBtn->Enabled=false;
     File2WAVBtn->Enabled=false;
     File2MP3Btn->Enabled=false;

  if (SelectedChkBx->Checked)
     {  
     makewav();
     }
  else
     {
     ProgresoFrm->mp3=true;
     ProgresoFrm->Lame= LameEdt->Text;
     ProgresoFrm->OutputDir=OutputEdt->Text;
     
     if (InfoChkBx->Checked)
        ProgresoFrm->extra_info= info;
     else
       ProgresoFrm->extra_info= "";

   FillProgres( );
   if (conversion)
       ProgresoFrm->ProcesHI();
   else
       ProgresoFrm->ProcesNORMAL();
     }

    PlayBtn->Enabled=true;
    EmularBtn->Enabled=true;
    File2WAVBtn->Enabled=true;
    File2MP3Btn->Enabled=true;
}


//---------------------------------------------------------------------------

void __fastcall TPlayerFrm::DirBtnClick(TObject *Sender)
{
       DirFrm->ShowModal();
       if (DirFrm->ModalResult==2)
           OutputEdt->Text=DirFrm->DirectoryListBox1->Directory;
}
//---------------------------------------------------------------------------

void __fastcall TPlayerFrm::LameBtnClick(TObject *Sender)
{
    if (OpenDialog1->Execute())
         LameEdt->Text=OpenDialog1->FileName ;

}

//---------------------------------------------------------------------------
void __fastcall TPlayerFrm::FillProgres( )
{
   TListItem *li;
   AnsiString sets="Conversion: ";
   sets+=StatusBarra->Panels->Items[0]->Text;
   if (conversion)
     {
     sets+="\r\n tecniq ";
     sets+=StatusBarra->Panels->Items[1]->Text;
     sets+=" ";
     if (conversion==1)
        sets+=StatusBarra->Panels->Items[2]->Text;
     }
   else
     sets+="\r\n ";

   sets+="\r\n ";
   sets+=StatusBarra->Panels->Items[3]->Text;
   sets+="\r\n " ;
   sets+=BitRateLbl->Caption;

   if (ProgresoFrm!=NULL)
   {
   ProgresoFrm->InfoLbl->Caption=sets;
   
   ProgresoFrm->FilesLV->Clear();
   for (int i=0;i<FileListBox1->Items->Count;i++)
        {
        li=ProgresoFrm->FilesLV->Items->Add();
        sets =  DirectoryListBox1->Directory;
        sets+="\\";
        sets+=FileListBox1->Items->Strings[i];
        li-> Caption=sets;
        li->SubItems->Add("");
        li->SubItems->Add("");
        li->SubItems->Add("");
        li->SubItems->Add("");
        li->SubItems->Add("");
        li->SubItems->Add("");
        li->SubItems->Add("");
        li->SubItems->Add("");
        }
   }
}



void __fastcall TPlayerFrm::SelectedChkBxClick(TObject *Sender)
{
    if (!SelectedChkBx->Checked)
        {
    File2WAVBtn->Enabled=true;
    File2MP3Btn->Enabled=true;
        }
    else
       {
    File2WAVBtn->Enabled=EmularBtn->Enabled;
    File2MP3Btn->Enabled=EmularBtn->Enabled;
        }
}
//---------------------------------------------------------------------------

void __fastcall TPlayerFrm::MediaPlayer1Notify(TObject *Sender)
{
#ifdef _DEBUG_MEDIA_
     Memo1->Lines->Add(MediaPlayer1->Mode);
     Memo1->Lines->Add(MediaPlayer1->ErrorMessage);
#endif
     MediaPlayer1->Notify=true;

     if (MediaPlayer1->Mode==2)
       {
       if ( MediaPlayer1->Position==MediaPlayer1->Length)
         {
         MediaPlayer1->Stop();
         MediaPlayer1->Notify=true;
         MediaPlayer1->Close();
         MediaPlayer1->Notify=true;
         MediaPlayer1->FileName="";

         Timer1->Enabled=false;
         VerBarra(false);
         Label6->Caption="Stopped";
         DriveComboBox1->Enabled=true;
         DirectoryListBox1->Enabled=true;
         FileListBox1 ->Enabled=true;
         PlayBtn->Enabled=true;
         EmularBtn->Enabled=true;
         File2WAVBtn->Enabled=true;
         File2MP3Btn->Enabled=true;
         StopBtn->Enabled=false;
         }       
       }
     if (MediaPlayer1->Mode==1)
       {

       MediaPlayer1->Close();
       MediaPlayer1->Notify=true;
       MediaPlayer1->FileName="";

       Timer1->Enabled=false;
       VerBarra(false )  ;
       Label6->Caption="Stopped";
       DriveComboBox1->Enabled=true;
       DirectoryListBox1->Enabled=true;
       FileListBox1 ->Enabled=true;
       PlayBtn->Enabled=true;
       EmularBtn->Enabled=true;
       File2WAVBtn->Enabled=true;
       File2MP3Btn->Enabled=true;
       StopBtn->Enabled=false;
       }
      
}
//---------------------------------------------------------------------------

void __fastcall TPlayerFrm::Timer1Timer(TObject *Sender)
{
     Label6->Caption=MediaPlayer1->Position;
     ProgressBar1->Position=500+MediaPlayer1->Position;
}
//---------------------------------------------------------------------------

