//---------------------------------------------------------------------------

#include <vcl.h>    
#include <inifiles.hpp>


#include "funciones.h"
#include "sbb.h"
#include "loaders.h"
#include "wav2.h"

#pragma hdrstop

#include "mainForm.h"
#include "InputForm.h"
#include "SettingsForm.h"
#include "wavmonitorForm.h"
#include "BatchForm.h"
#include "AdjustForm.h"
#include "AboutForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TotlaFrm *otlaFrm;


AnsiString ToolRom2Bin;
AnsiString ToolDiskMgr;
AnsiString ToolCPC;


ts_sbb_header e_head_sbb;
ts_sbb_block  e_blocks_sbb[_MAX_SBB_BLOCKS_+1];

//---------------------------------------------------------------------------
__fastcall TotlaFrm::TotlaFrm(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TotlaFrm::FormCreate(TObject *Sender)
{
    TIniFile *ini;
    AnsiString nombre="O.T.L.A. ";
    Caption = nombre+ version_SBB; 
    
    show_info=true;

    IniFileName=ExtractFilePath(Application->ExeName)+"otla.ini";
    ini = new TIniFile( IniFileName );

    try
    {
        Top =ini->ReadInteger ( "otla", "Top", Top);
        Left=ini->ReadInteger ( "otla", "Left", Left);
        associate_sbb=  ini->ReadBool( "otla", "associate_sbb",  false);
        InPutDir = ini->ReadString ( "otla", "OpenPath", ExtractFilePath(Application->ExeName)+ "sbb\\");

        OpenDialog1->InitialDir=ini->ReadString ( "otlaBuilder", "AddPath", ExtractFilePath(Application->ExeName)+"samples\\");

        OutPutZX =ini->ReadString ( "otlaBuilder", "OutPutZX", ExtractFilePath(Application->ExeName)+"sbb\\zx\\");
        OutPutCPC=ini->ReadString ( "otlaBuilder", "OutPutCPC",ExtractFilePath(Application->ExeName)+"sbb\\cpc\\");
        OutPutMSX=ini->ReadString ( "otlaBuilder", "OutPutMSX",ExtractFilePath(Application->ExeName)+"sbb\\msx\\");
        OutPut81 =ini->ReadString ( "otlaBuilder", "OutPut81", ExtractFilePath(Application->ExeName)+"sbb\\81\\");

        ToolRom2Bin= ini->ReadString ( "otlaBuilder", "msxr2b", ExtractFilePath(Application->ExeName)+"tools\\msx_r2b\\msxr2b.exe");
        ToolDiskMgr= ini->ReadString ( "otlaBuilder", "msxdskmgr", ExtractFilePath(Application->ExeName)+"tools\\msx_diskmgr\\DISKMGR.EXE");
        ToolCPC    = ini->ReadString ( "otlaBuilder", "cpctool", "WinApe.exe");

        MaquinaCmbBx->ItemIndex= ini->ReadInteger( "otlaBuilder", "machine", 0);

        

         OutPutDir=ini->ReadString ( "otlaPlayer", "OutPutDir", ExtractFilePath(Application->ExeName)+"output\\");
        output_info= ini->ReadBool( "otlaPlayer", "output_info",  false);

        view_hex=  ini->ReadBool( "otlaPlayer", "view_hex",  false);

        LameExe= ini->ReadString ( "otlaPlayer", "lame", ExtractFilePath(Application->ExeName)+"tools\\lame\\lame.exe");
        bit_rate_mp3= ini->ReadInteger ( "otlaPlayer", "bit_rate", 256);
        force_cubic= ini->ReadBool ( "otlaPlayer", "force_cubic", true);


        MakeLoaderChkBx->Checked=ini->ReadInteger ( "otlaPlayer", "make_loader", 1);
        AccelerateCHkBx->Checked=ini->ReadInteger ( "otlaPlayer", "acelerate", 0);
        CheckLoadErrorChkBx->Checked=ini->ReadInteger ( "otlaPlayer", "checkloaderror", 1);
        ReleMotorChkBx->Checked=ini->ReadInteger ( "otlaPlayer", "motor", 1);


        FrecuenciaCmbBx->ItemIndex=ini->ReadInteger ( "otlaPlayer", "freq", 44100 )==48000;
        MuestrasCmbBx->ItemIndex=ini->ReadInteger ( "otlaPlayer", "s_p_b", 1 );    
        FormaCmbBx->ItemIndex = ini->ReadInteger ( "otlaPlayer", "wav_form", 1);  
        InvertChkBx->Checked= (ini->ReadInteger ( "otlaPlayer", "invert_pol", 1)==-1)?1:0; 
        pausaEdt->Text  =    ini->ReadInteger ( "otlaPlayer", "pause_blocks", 500);

        if (MuestrasCmbBx->ItemIndex<0)
            MuestrasCmbBx->ItemIndex=1;

        //MaquinaCmbBxChange(NULL);
    }
    catch (...)
    {}
    delete ini;

    /*ClearData();
    CambiaMaquina(e_head_sbb.machine[0]); */
    
     New1Click(NULL);

     if (ParamCount()>=1)
         OpenFileSBB (ParamStr(1));

}
//---------------------------------------------------------------------------

void __fastcall TotlaFrm::FormClose(TObject *Sender, TCloseAction &Action)
{
    TIniFile *ini;
    ini = new TIniFile(IniFileName );
    ini->WriteInteger ( "otla", "Top" , Top );
    ini->WriteInteger ( "otla", "Left", Left );
    ini->WriteString  ( "otla", "OpenPath",  InPutDir);
    
    ini->WriteString ( "otlaBuilder", "AddPath", OpenDialog1->InitialDir );
    ini->WriteInteger( "otlaBuilder", "machine", MaquinaCmbBx->ItemIndex );



        ini->WriteInteger ( "otlaPlayer", "make_loader", MakeLoaderChkBx->Checked);
        ini->WriteInteger ( "otlaPlayer", "acelerate", AccelerateCHkBx->Checked);
        ini->WriteInteger ( "otlaPlayer", "checkloaderror", CheckLoadErrorChkBx->Checked);
        ini->WriteInteger ( "otlaPlayer", "motor", ReleMotorChkBx->Checked);



        ini->WriteInteger ( "otlaPlayer", "freq", FrecuenciaCmbBx->ItemIndex?48000:44100);
        ini->WriteInteger ( "otlaPlayer", "s_p_b", MuestrasCmbBx->ItemIndex);    
        ini->WriteInteger ( "otlaPlayer", "wav_form", FormaCmbBx->ItemIndex);   
        ini->WriteInteger ( "otlaPlayer", "invert_pol", InvertChkBx->Checked?-1:1);  
        ini->WriteInteger ( "otlaPlayer", "pause_blocks", pausaEdt->Text.ToIntDef(1000));

    delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TotlaFrm::ClearData()
{

    AddBtn->Enabled=true;
    elFichero="";
    memset ( e_blocks_sbb , 0, sizeof (e_blocks_sbb));
    memset (&e_head_sbb , 0, sizeof (e_head_sbb)); 
    MaquinaCmbBxChange(NULL);  
    MaquinaCmbBx->Enabled=true;
    Modificado=0;
    
    PlayBtn ->Enabled=false;
    soundPlay1->Enabled=false;
    WavBtn  ->Enabled=false;
    wav1  ->Enabled=false;
    Mp3Btn  ->Enabled=false;
    mp31  ->Enabled=false;
    TzxBtn  ->Enabled=false;
    tzx1  ->Enabled=false;

    
    StatusBar1->Panels->Items[0]->Text = "";

}
//---------------------------------------------------------------------------
void __fastcall TotlaFrm::FillData()
{
    TListItem *Item;  
    
    FileEdt->Text=elFichero;

    BlocksLV->Items->Clear();
    int b=0;
    while (e_blocks_sbb[b].block_type!=0)
    {
        Item=BlocksLV->Items->Add();
        Item->Caption=e_blocks_sbb[b].blockname;
        Item->SubItems->Add(n2s(e_blocks_sbb[b].size,view_hex));
        Item->SubItems->Add(TipoBloque(e_blocks_sbb[b].block_type));
        Item->SubItems->Add(n2s(e_blocks_sbb[b].ini,view_hex));
        Item->SubItems->Add((char)e_blocks_sbb[b].jump);
        Item->SubItems->Add(n2s(e_blocks_sbb[b].exec,view_hex));
        b++;
    }

        e_head_sbb.n_blocks=b;

        CambiaMaquina(e_head_sbb.machine[0]);

        ModelCmbBx->ItemIndex= (char)e_head_sbb.model-1;
        NameEdt->Text=e_head_sbb.nombre;
        AditionalInfoEdt->Text=e_head_sbb.extra_info;
        BloksEdt->Text=(int)e_head_sbb.n_blocks;   
        OriginEdt->Text=e_head_sbb.origin;

        EnableIntChkBx->Checked= e_head_sbb.ei_di&1;
        ReubicaEdt->Text=n2s(e_head_sbb.locate,view_hex);
        PokeEdt->Text=n2s((int)e_head_sbb.poke_ffff,view_hex);
        ClearEdt->Text=n2s((int)e_head_sbb.clear_sp,view_hex);
        UsrEdt->Text =n2s((int)e_head_sbb.usr_pc,view_hex);


}
//---------------------------------------------------------------------------
void __fastcall TotlaFrm::CambiaMaquina(char maquina)
{                          
        LocateLbl->Enabled=false;
        ReubicaEdt->Enabled=false;
        PokeLbl->Enabled=false ;
        PokeEdt->Enabled=false ;

        ClearLbl->Enabled=true;
        ClearEdt->Enabled=true;
        UsrLbl->Enabled=true ;
        UsrEdt->Enabled=true ;
        switch (maquina)
        {
            case 'Z':
                MaquinaCmbBx->ItemIndex=0;
                OpenDialog1->Filter="ZX files (*.tap;*.tzx;*.z80;*.sna;*.scr;*.dsk;*.sbb)|*.tap;*.tzx;*.z80;*.sna;*.scr;*.dsk;*.sbb;*.hst";
                OutPutSBB=OutPutZX;
                NameEdt->MaxLength=10;
                ClearLbl->Caption="CLEAR";
                UsrLbl->Caption="USR";
                                              
                CheckLoadErrorChkBx->Enabled=true;
                EnableIntChkBx->Enabled=true;
                ReleMotorChkBx->Enabled=false;
                TzxBtn->Caption="SBB => TZX";
                break;
            case 'C':              
                MaquinaCmbBx->ItemIndex=1;
                OpenDialog1->Filter="CPC files (*.sna;*.bin;*.cdt;*.dsk;*.sbb)|*.sna;*.bin;*.dsk;*.cdt;*.sbb;*.hst";
                OutPutSBB=OutPutCPC;
                NameEdt->MaxLength=16;
                LocateLbl->Enabled=true;
                ReubicaEdt->Enabled=true;
                ClearLbl->Caption="SP";
                UsrLbl->Caption="JP";
                                            
                CheckLoadErrorChkBx->Enabled=true;
                EnableIntChkBx->Enabled=true;
                ReleMotorChkBx->Enabled=true;
                TzxBtn->Caption="SBB => CDT";
                break;  
            case 'M':
                MaquinaCmbBx->ItemIndex=2; 
                OpenDialog1->Filter="MSX files (*.bin;*.cas;*.dsk;*.rom;*.sbb)|*.bin;*.rom;*.cas;*.dsk;*.sbb;*.hst";
                OutPutSBB=OutPutMSX;
                NameEdt->MaxLength=6;
                LocateLbl->Enabled=true;
                ReubicaEdt->Enabled=true;
                PokeLbl->Enabled=true ;
                PokeEdt->Enabled=true ;
                ClearLbl->Caption="CLEAR";
                UsrLbl->Caption="JP";

                CheckLoadErrorChkBx->Enabled=true;
                EnableIntChkBx->Enabled=true;
                ReleMotorChkBx->Enabled=true;
                TzxBtn->Caption="SBB => ¿TZX?";
                break;
            case '8':
                MaquinaCmbBx->ItemIndex=3;
                OpenDialog1->Filter="81 files (*.p;*.z81;*.sbb)|*.p;*.z81;*.sbb;*.hst";
                OutPutSBB=OutPut81;
                NameEdt->MaxLength=2;
                ClearLbl->Caption="SP";
                UsrLbl->Caption="USR";
                                   
               ClearLbl->Enabled=false;
               ClearEdt->Enabled=false;
               UsrLbl->Enabled=false ;
               UsrEdt->Enabled=false ;

                CheckLoadErrorChkBx->Enabled=false;
                EnableIntChkBx->Enabled=false;
                ReleMotorChkBx->Enabled=false;
                TzxBtn->Caption ="SBB => TZX";
                break;
            default:
                MaquinaCmbBx->ItemIndex=-1;
                //Warning="Error: Unknowmachine";

        MessageDlg("Error: Unknowmachine",  mtError	, TMsgDlgButtons() << mbOK, 0);
                break;
        }

      FrecuenciaCmbBxChange (NULL);
}
//---------------------------------------------------------------------------
void __fastcall TotlaFrm::MaquinaCmbBxChange(TObject *Sender)
{
   ModelCmbBx->Items->Clear();
   
   switch (MaquinaCmbBx->ItemIndex)
    {
    case 0:
        strcpy (e_head_sbb.machine,"ZXS");
        e_head_sbb.locate=0xff;
        break;
    case 1:
        strcpy (e_head_sbb.machine,"CPC");  
        e_head_sbb.locate=0xff;
        break;
    case 2:
        strcpy (e_head_sbb.machine,"MSX");
        e_head_sbb.locate=0xf4;
        break;
    case 3:
        strcpy (e_head_sbb.machine,"81-");
        e_head_sbb.locate=0x0;
        break;
    default:
        e_head_sbb.locate=0x0;
        break;
    }         
        ModelCmbBx->Items->Add(modelo (e_head_sbb.machine[0],1));
        ModelCmbBx->Items->Add(modelo (e_head_sbb.machine[0],2));
        ModelCmbBx->Items->Add(modelo (e_head_sbb.machine[0],3));

    CambiaMaquina(e_head_sbb.machine[0]);
    ReubicaEdt->Text=n2s(e_head_sbb.locate,view_hex);
}
//---------------------------------------------------------------------------

void __fastcall TotlaFrm::MaquinaCmbBxDrawItem(TWinControl *Control,
      int Index, TRect &Rect, TOwnerDrawState State)
{
    MachineImgLst->Draw( MaquinaCmbBx->Canvas,Rect.Left,Rect.Top,Index,true);
}
//---------------------------------------------------------------------------

void __fastcall TotlaFrm::New1Click(TObject *Sender)
{
   ClearData();
   elFichero="new.SBB";
   FillData();
   Modificado=0;   
   Nuevo=1;
}
//---------------------------------------------------------------------------

void __fastcall TotlaFrm::NewSave1Click(TObject *Sender)
{
   New1Click(NULL);
   AddBtnClick(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TotlaFrm::AddBtnClick(TObject *Sender)
{
    ts_sbb_header cabecera_aux=e_head_sbb;

    if (OpenDialog1->Execute())
    {
        OpenDialog1->InitialDir=ExtractFilePath(OpenDialog1->FileName);

        InputBlocksDlg->Caption="Adding blocks";
        InputBlocksDlg->enheaxadecimal =  view_hex;
        InputBlocksDlg->head_sbb=cabecera_aux;
        InputBlocksDlg->res_blocks_sbb= &e_blocks_sbb[e_head_sbb.n_blocks];
        if (InputBlocksDlg->Abre(OpenDialog1->FileName)==mrOk)
        {
            
            Modificado=1;
            if (e_head_sbb.origin==0)
            {
                elFichero= OutPutSBB+"\\";
                elFichero += ExtractFileName(ChangeFileExt( OpenDialog1->FileName,".sbb"));
            }

            cabecera_aux=InputBlocksDlg->head_sbb;

            if (
                (cabecera_aux.origin==_Snapshot4ZX48_) ||  //snap
                (cabecera_aux.origin==_Snapshot4ZX128_) ||  //snap
                (cabecera_aux.origin==_Snapshot4CPC_) )
                {
                e_head_sbb.origin=cabecera_aux.origin;
                e_head_sbb.ei_di&=0xfe ;
                }
            else
               if (e_head_sbb.origin==0)
               {
                e_head_sbb.origin=cabecera_aux.origin;
                e_head_sbb.ei_di|=0x01  ;
               }

            if (e_head_sbb.model==0)
               e_head_sbb.model=cabecera_aux.model;
            if (e_head_sbb.nombre[0]==0)
                strncpy (e_head_sbb.nombre,cabecera_aux.nombre,16);
                                                           
            if (e_head_sbb.usr_pc==0)
                e_head_sbb.usr_pc=cabecera_aux.usr_pc;
            if (e_head_sbb.clear_sp==0)
                e_head_sbb.clear_sp=cabecera_aux.clear_sp;
            if (e_head_sbb.poke_ffff==0)
                e_head_sbb.poke_ffff=cabecera_aux.poke_ffff;

            FillData();
            MaquinaCmbBx->Enabled=false;
    PlayBtn ->Enabled=true;
    soundPlay1->Enabled=true;
    WavBtn  ->Enabled=true;
    wav1  ->Enabled=true;
    Mp3Btn  ->Enabled=true;
    mp31  ->Enabled=true;
    TzxBtn  ->Enabled=true;
    tzx1  ->Enabled=true;
            if (e_head_sbb.machine[0]=='8')
               AddBtn->Enabled=false;

        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TotlaFrm::Editblocks1Click(TObject *Sender)
{
     
     InputBlocksDlg->Caption="Editing blocks";
     InputBlocksDlg->enheaxadecimal =  view_hex;
     InputBlocksDlg->res_blocks_sbb= &e_blocks_sbb[ 0];
     if (InputBlocksDlg->Edit (&e_head_sbb,e_blocks_sbb)==mrOk)
     {
         FillData();
     }

}
//---------------------------------------------------------------------------

int __fastcall TotlaFrm::OpenFileSBB(AnsiString FileToOpen)
{
   int res;
   ts_sbb_header  head;

 //  Warning="";
   
   InPutDir=ExtractFilePath(FileToOpen);
   ClearData();


   if ((res=load_sbb_file (FileToOpen.c_str(),&head, e_blocks_sbb))==0)
   {

      elFichero= FileToOpen;
      e_head_sbb=head;
      FillData();
      MaquinaCmbBx->Enabled=false;
      if (e_head_sbb.machine[0]=='8')
               AddBtn->Enabled=false;

      if (e_head_sbb.sbb_version[2]<version_SBB_code[2])
      {
      //     ShowMessage("actualiza version");
           e_head_sbb.sbb_version[2]=version_SBB_code[2];
           Modificado = 1;
      }


      FrecuenciaCmbBxChange (NULL);

      PlayBtn ->Enabled=true;
      soundPlay1->Enabled=true;
      WavBtn  ->Enabled=true;
      wav1  ->Enabled=true;
      Mp3Btn  ->Enabled=true;
      mp31  ->Enabled=true;
      TzxBtn  ->Enabled=true;
      tzx1  ->Enabled=true;
    }
    else
    {
       // Warning="Error: couldn not open "+FileToOpen;
        AnsiString men=FileToOpen+"\r\n";
        men+=error_file(res);
        MessageDlg(men,  mtError	, TMsgDlgButtons() << mbOK, 0);
    }
    
   // StatusBar1->Panels->Items[0]->Text =Warning;
    return res;

}

void __fastcall TotlaFrm::Open1Click(TObject *Sender)
{
    AnsiString back_dir=OpenDialog1->InitialDir;
    AnsiString back_filter=OpenDialog1->Filter;
    OpenDialog1->InitialDir=InPutDir ;
    OpenDialog1->Filter="SBB files (*.sbb)|*.sbb";

    if (OpenDialog1->Execute())
    {
    OpenFileSBB (OpenDialog1->FileName);
    Modificado=0;
    }
    OpenDialog1->InitialDir=back_dir;
    OpenDialog1->Filter= back_filter;
}
//---------------------------------------------------------------------------
void __fastcall TotlaFrm::UpdateHeader()
{

    strncpy (e_head_sbb.machine,MaquinaCmbBx->Text.c_str(),sizeof (e_head_sbb.machine));
    e_head_sbb.model=ModelCmbBx->ItemIndex + 1;

    strncpy (e_head_sbb.extra_info,AditionalInfoEdt->Text.c_str(),sizeof (e_head_sbb.extra_info));

    e_head_sbb.ei_di   =EnableIntChkBx->Checked;

    strncpy (e_head_sbb.nombre,NameEdt->Text.c_str(), sizeof (e_head_sbb.nombre));


    e_head_sbb.locate  =inputDecHex(ReubicaEdt->Text.c_str());
    e_head_sbb.origin ;
    e_head_sbb.clear_sp =inputDecHex(ClearEdt->Text.c_str());
    e_head_sbb.usr_pc   =inputDecHex(UsrEdt->Text.c_str());
    e_head_sbb.poke_ffff=inputDecHex(PokeEdt->Text.c_str());

}
//---------------------------------------------------------------------------

void __fastcall TotlaFrm::Save1Click(TObject *Sender)
{
    UpdateHeader();

    e_head_sbb.n_blocks=0;

    while (e_blocks_sbb[e_head_sbb.n_blocks].block_type)
    {
        if (e_blocks_sbb[e_head_sbb.n_blocks+1].block_type==0 )
            if (e_blocks_sbb[e_head_sbb.n_blocks].exec)
                execjump  (&e_blocks_sbb[e_head_sbb.n_blocks], 0xc3);
           else
               execjump  (&e_blocks_sbb[e_head_sbb.n_blocks], 0xc9);
        else
           if (e_blocks_sbb[e_head_sbb.n_blocks].exec)
              execjump  (&e_blocks_sbb[e_head_sbb.n_blocks], 0xcd);
            else
              execjump  (&e_blocks_sbb[e_head_sbb.n_blocks], 0x01);


        if (e_head_sbb.machine[0]=='C'||e_head_sbb.machine[0]=='M')
        {                                                                
            int ini= e_blocks_sbb[e_head_sbb.n_blocks].ini;
            int last = e_blocks_sbb[e_head_sbb.n_blocks].ini+ e_blocks_sbb[e_head_sbb.n_blocks].size;
            if  (ini < e_head_sbb.locate*0x100 && last > e_head_sbb.locate*0x100 )
            {
             AnsiString men = "Last bytes of block " +AnsiString( e_head_sbb.n_blocks+1);
             men+=" override address of loading routine located at ";
             men+=AnsiString (e_head_sbb.locate*0x100) ;
             men+="\r\n This block will be truncate when converting to wav";
             MessageDlg(men,  mtWarning	, TMsgDlgButtons() << mbOK, 0);
            }
            if (ini > e_head_sbb.locate*0x100 && ini  < e_head_sbb.locate*0x100+0x100)
            {
             AnsiString men = "Start address of block " +AnsiString( e_head_sbb.n_blocks+1);
             men+=" overrides address of loading routine located at ";
             men+=AnsiString (e_head_sbb.locate*0x100) ;
             MessageDlg(men,  mtError	, TMsgDlgButtons() << mbOK, 0);
             return;
            }
        }  
        e_head_sbb.n_blocks++;
    }

    int res=save_sbb_file (elFichero.c_str(),&e_head_sbb,e_blocks_sbb);
    if (res)
    {
        AnsiString  mens=elFichero  +"\r\n";
        mens += error_file(res);
        MessageDlg(mens,  mtError		, TMsgDlgButtons() << mbOK, 0);

    }
    else
         Modificado=0;
    
   InPutDir=OutPutSBB;
}
//---------------------------------------------------------------------------

void __fastcall TotlaFrm::Saveas1Click(TObject *Sender)
{
      SaveDialog1->FileName = elFichero ;
      if  (SaveDialog1->Execute())
      {
      elFichero = SaveDialog1->FileName ;
      Save1Click(NULL);
      InPutDir=ExtractFilePath(SaveDialog1->FileName);
      }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TotlaFrm::FrecuenciaCmbBxChange(TObject *Sender)
{
    int s=MuestrasCmbBx->ItemIndex;

    MuestrasCmbBx->Items->Clear();
    if (FrecuenciaCmbBx->ItemIndex)
    {
        MuestrasCmbBx->Items->Add("4   (12000 bps)");
        MuestrasCmbBx->Items->Add("3.5 (13600 bps)");
        MuestrasCmbBx->Items->Add("3   (16200 bps)");
        MuestrasCmbBx->Items->Add("2.5 (19200 bps)");
        if (e_head_sbb.machine[0]=='Z')
        {               
            if ((s==5)||(s==6))
                s=3;
            if (s==4)
                s=2;
        }
        else
        {
            MuestrasCmbBx->Items->Add("2.75 (17454 bps)");
            MuestrasCmbBx->Items->Add("2.25 (21333 bps)");
            MuestrasCmbBx->Items->Add("1.75 (27428 bps)");
        }
        if (e_head_sbb.machine[0]=='8')
        {
            MuestrasCmbBx->Items->Clear();
            MuestrasCmbBx->Items->Add("6 ( 8000 bps)");
            MuestrasCmbBx->Items->Add("5 ( 9600 bps)");
            MuestrasCmbBx->Items->Add("4 (12000 bps)");
            MuestrasCmbBx->Items->Add("3 (16200 bps)");
        }
    }
    else
    {
        MuestrasCmbBx->Items->Add("4   (11025 bps)");
        MuestrasCmbBx->Items->Add("3.5 (12600 bps)");
        MuestrasCmbBx->Items->Add("3   (14700 bps)");
        MuestrasCmbBx->Items->Add("2.5 (17640 bps)");
        if (e_head_sbb.machine[0]=='Z')
        {    
            if ((s==5)||(s==6))
                s=3;
            if (s==4)
                s=2;
        }
        else
        {
            MuestrasCmbBx->Items->Add("2.75 (16036 bps)");
            MuestrasCmbBx->Items->Add("2.25 (19600 bps)");
            MuestrasCmbBx->Items->Add("1.75 (25200 bps)");
        } 
        if (e_head_sbb.machine[0]=='8')
        {
            MuestrasCmbBx->Items->Clear();
            MuestrasCmbBx->Items->Add("6 ( 7350 bps)");
            MuestrasCmbBx->Items->Add("5 ( 8820 bps)");
            MuestrasCmbBx->Items->Add("4 (11025 bps)");
            MuestrasCmbBx->Items->Add("3 (14700 bps)");
        }
    }
    MuestrasCmbBx->ItemIndex=s;
}
//---------------------------------------------------------------------------

void __fastcall TotlaFrm::FormaCmbBxDrawItem(TWinControl *Control,
      int Index, TRect &Rect, TOwnerDrawState State)
{
    wavImgLst->Draw( FormaCmbBx->Canvas,Rect.Left,Rect.Top,Index,true);
    if (Index>=0)
        FormaCmbBx->Canvas->TextOutA(Rect.Left+30,Rect.Top,
                                     FormaCmbBx->Items->Strings[Index]);
}
//---------------------------------------------------------------------------

void __fastcall TotlaFrm::Settings1Click(TObject *Sender)
{

   
    SettingsFrm->ExtensionChkBx->Checked= associate_sbb;

    SettingsFrm->DirZXEdt->Text=OutPutZX;
    SettingsFrm->DirCPCEdt->Text=OutPutCPC;
    SettingsFrm->DirMSXEdt->Text=OutPutMSX;
    SettingsFrm->Dir81Edt->Text=OutPut81;
    SettingsFrm->Rom2BinEdt->Text=ToolRom2Bin;
    SettingsFrm->DiskMgrEdt->Text=ToolDiskMgr;
    SettingsFrm->CPCToolEdt->Text=ToolCPC;

    SettingsFrm->HexadecimalChkBx->Checked= view_hex;
    SettingsFrm->WavDirEdt->Text=OutPutDir;
    SettingsFrm->InfoChkBx->Checked= output_info;

    SettingsFrm->LameEdt->Text=LameExe;
    SettingsFrm->ForceCubicChkBx->Checked=force_cubic;

    SettingsFrm->BitRateCmbBx->ItemIndex=3;
    for (int i=0;i<SettingsFrm->BitRateCmbBx->Items->Count;i++)
        if (bit_rate_mp3==SettingsFrm->BitRateCmbBx->Items->Strings[i].ToIntDef(0))
        {
            SettingsFrm->BitRateCmbBx->ItemIndex=i;
            break;
        }

    SettingsFrm->ShowModal();

    if (SettingsFrm->ModalResult==mrOk)
    {
        TIniFile *ini;
        ini = new TIniFile( IniFileName );
                                                    
        associate_sbb= SettingsFrm->ExtensionChkBx->Checked;
        ini->WriteBool( "otla", "associate_sbb",  associate_sbb);

        OutPutZX=SettingsFrm->DirZXEdt->Text;
        ini->WriteString ( "otlaBuilder", "OutPutZX", OutPutZX);
        OutPutCPC=SettingsFrm->DirCPCEdt->Text;
        ini->WriteString ( "otlaBuilder", "OutPutCPC",OutPutCPC);
        OutPutMSX=SettingsFrm->DirMSXEdt->Text;
        ini->WriteString ( "otlaBuilder", "OutPutMSX",OutPutMSX );
        OutPut81=SettingsFrm->Dir81Edt->Text;
        ini->WriteString ( "otlaBuilder", "OutPut81",OutPut81 );
        ToolRom2Bin=SettingsFrm->Rom2BinEdt->Text;
        ini->WriteString ( "otlaBuilder", "msxr2b",  ToolRom2Bin);
        ToolDiskMgr=SettingsFrm->DiskMgrEdt->Text;
        ini->WriteString ( "otlaBuilder", "dskmgr",  ToolDiskMgr);
        ToolCPC=SettingsFrm->CPCToolEdt->Text;
        ini->WriteString ( "otlaBuilder", "cpctool",  ToolCPC);

        view_hex= SettingsFrm->HexadecimalChkBx->Checked;
        ini->WriteBool( "otlaPlayer", "view_hex",  view_hex);

        OutPutDir=SettingsFrm->WavDirEdt->Text;
        ini->WriteString ( "otlaPlayer", "OutPutDir", OutPutDir);  
        output_info= SettingsFrm->InfoChkBx->Checked;
        ini->WriteBool( "otlaPlayer", "output_info",  output_info);

        LameExe=SettingsFrm->LameEdt->Text;
        ini->WriteString ( "otlaPlayer", "lame",  LameExe);
        force_cubic=SettingsFrm->ForceCubicChkBx->Checked;
        ini->WriteBool( "otlaPlayer", "force_cubic",  force_cubic);
        bit_rate_mp3=SettingsFrm->BitRateCmbBx->Text.ToIntDef(256);
        ini->WriteInteger( "otlaPlayer", "bit_rate",  bit_rate_mp3);

        delete ini;
        //FileCmbBxChange(NULL);
         UpdateHeader();   
         FillData();
        
    }
}
//---------------------------------------------------------------------------
void TotlaFrm::set_wav_vars ()
{
    
    make_loader=MakeLoaderChkBx->Checked;
    acelerar=AccelerateCHkBx->Checked;
    checkloaderror=CheckLoadErrorChkBx->Checked;
    motor=ReleMotorChkBx->Checked;

    pausa_ms= pausaEdt->Text.ToIntDef(500);

    if (InvertChkBx->Checked)
        invert_pol=-1;
    else
        invert_pol=1;

    forma_onda= FormaCmbBx->ItemIndex;

    if (FrecuenciaCmbBx->ItemIndex)
        freq_muestreo=48000;
    else
        freq_muestreo=44100;

    if (e_head_sbb.machine[0]=='8')
    switch (MuestrasCmbBx->ItemIndex)
    {
    case 0:    s_p_b=_3_50; break;
    case 1:    s_p_b=_2_75; break;
    case 2:    s_p_b=_2_50; break;
    case 3:    s_p_b=_1_75; break;
    default:   s_p_b=_3_50; break;
    }
    else
    switch (MuestrasCmbBx->ItemIndex)
    {
    case 0:    s_p_b=_4_00;  break;
    case 1:    s_p_b=_3_50;  break;
    case 2:    s_p_b=_3_00;  break;
    case 3:    s_p_b=_2_50;  break;
    case 4:    s_p_b=_2_75;  break;
    case 5:    s_p_b=_2_25;  break;
    case 6:    s_p_b=_1_75;  break;
    case 7:    s_p_b=_4_25;  break;
    case 8:    s_p_b=_3_75;  break;
    default:         break;
    }
}
//---------------------------------------------------------------------------
int  __fastcall TotlaFrm::AutoSaveSBB ()
{
   int res=0;
   if (Modificado)
   {
      if (Nuevo)
      {
         if (FileExists(elFichero))
         {
          res=Application->MessageBoxA(
               AnsiString (elFichero+" already exists.\r\nDo you want to overwrite it?").c_str(),
               "Warning",MB_YESNO);

         if (res==IDNO)
            return -1;
         res=0;
         }
      }
      Save1Click(NULL);
      Nuevo=0;
   }
   return res;
}
//---------------------------------------------------------------------------
void __fastcall TotlaFrm::PlayBtnClick(TObject *Sender)
{
    int res;
    PlayBtn->Enabled=false;
    soundPlay1->Enabled=false;

    StatusBar1->Panels->Items[0]->Text="Creating wav..." ;
    Application->ProcessMessages();
    
    AnsiString final ;

    final = OutPutDir+"\\";
    final+= "play.wav";

    set_wav_vars ();

    if (0==AutoSaveSBB ())
    {
      res=sbb2wav (elFichero.c_str(), final.c_str());
      if (res)
      {
         //ShowMessage (error_file(res));  
        AnsiString  mens=elFichero  +"\r\n";
        mens += error_file(res);
         
         MessageDlg( mens,  mtError		, TMsgDlgButtons() << mbOK, 0);
      }
      else
      {
         StatusBar1->Panels->Items[0]->Text="Playing..." ;

         Application->ProcessMessages();

         MonitorFrm->OpenWaveFile(final);
         MonitorFrm->PlayBtnClick(NULL);
         MonitorFrm->ShowModal();
      }
    }

    PlayBtn->Enabled=true;
    soundPlay1->Enabled=true;
    StatusBar1->Panels->Items[0]->Text="" ;
    
    return;

}
//---------------------------------------------------------------------------
AnsiString __fastcall TotlaFrm::MakeNameWav (AnsiString elFichero)
{
    AnsiString s ;
    AnsiString extrainfo;
    float samples;
    if (e_head_sbb.machine[0]=='8')
        samples=MuestrasCmbBx->Text.SubString(1,1).ToIntDef(6);
    else
        samples=s_p_b/4.0;

    if (output_info)
       extrainfo.sprintf("_%i_%.2f_w%i",freq_muestreo/1000,samples,forma_onda);
    else
       extrainfo="";

    if (output_info)
      if (invert_pol==-1)
        extrainfo+="n";
      else
        extrainfo+="p";
                             /*
    s.printf ("%s\\%s%s.wav",
              OutPutDir.c_str(),
              ChangeFileExt (ExtractFileName (elFichero),"").c_str(),
              extrainfo);   */
    s= OutPutDir+"\\";
    s+=ChangeFileExt (ExtractFileName (elFichero),"");
    s+=extrainfo;
    s+=".wav";
    return s;
}
//---------------------------------------------------------------------------
void __fastcall TotlaFrm::WavBtnClick(TObject *Sender)
{
    AnsiString  mens;
    set_wav_vars ();
    wavFile=NULL;
    if (AutoSaveSBB ())
        return;
    wavFile= MakeNameWav  (elFichero);
    int res=sbb2wav (elFichero.c_str(), wavFile.c_str());
    if (res)
    {
        mens= elFichero+" =>\r\n";
        mens += wavFile+"\r\n";
        mens += error_file(res);
        //ShowMessage (mens);
        MessageDlg(mens,  mtError	, TMsgDlgButtons() << mbOK, 0);

        wavFile=NULL;
    }
    else
    {
        mens=mens.sprintf("%s \r\n Total time: %.1f",wavFile.c_str(),dura()+0.05);
        if (show_info)
            MessageDlg(mens,  mtInformation		, TMsgDlgButtons() << mbOK, 0);
    }

}
//---------------------------------------------------------------------------

void __fastcall TotlaFrm:: LameProcess(AnsiString WavIn, char maq , char *title )
{
   AnsiString comand_line= LameExe;
    AnsiString params ;
    AnsiString Title;
    AnsiString Comments;

    Title.sprintf("%s, %s",NombreMachine(maq),title);
    Comments.sprintf("s/b: %s waveform:%s",MuestrasCmbBx->Text,FormaCmbBx->Text);


    params.printf (" \"%s\"  -m m -f -b %i --tt \"%s\" --ta \"O.T.L.A.\" --tc \"%s\"",
                   WavIn.c_str(),
                   bit_rate_mp3,
                   Title.c_str(),
                   Comments.c_str()
                  );

    comand_line=LameExe+params;

    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;
    try
    {
        Win32Check( CreateProcess( LameExe.c_str(),
                                   comand_line.c_str(),
                                   NULL, NULL,
                                   false,
                                   0,
                                   NULL, NULL,
                                   &si, &pi) );
        Win32Check( CloseHandle(pi.hThread) );
        Win32Check( CloseHandle(pi.hProcess) );
    }
    catch (const Exception& e)
    {
        //ShowMessage("Error trying lame.exe\r\n"+e.Message);
        MessageDlg("Error trying lame.exe\r\n"+e.Message,  mtError	, TMsgDlgButtons() << mbOK, 0);

    }
}

//---------------------------------------------------------------------------
void __fastcall TotlaFrm::Mp3BtnClick(TObject *Sender)
{
    //AnsiString   file_wav;
    int tem_wf= FormaCmbBx->ItemIndex;
    if (force_cubic)
        FormaCmbBx->ItemIndex=1;

    show_info=false;
    WavBtnClick(NULL);
    show_info=true;
    
    FormaCmbBx->ItemIndex=tem_wf;

    if (wavFile!=NULL)
        LameProcess (wavFile,e_head_sbb.machine[0], NameEdt->Text.c_str());



}
//---------------------------------------------------------------------------
void __fastcall TotlaFrm::TzxBtnClick(TObject *Sender)
{
    AnsiString final ;
    AnsiString params ;
    float samples;

    set_wav_vars (); 
    if (AutoSaveSBB ())
        return;

    AnsiString auxFichero = OutPutDir+"\\";
    auxFichero += "temp.wav";

    int res=  sbb2wav (elFichero.c_str(), auxFichero.c_str());
    if (res)
    {
        AnsiString  mens= elFichero +"=>\n";
        mens+ auxFichero +"\r\n";
        mens+=error_file(res);
        MessageDlg(mens,  mtError, TMsgDlgButtons() << mbOK, 0);

        return;
    }      

    final= OutPutDir.c_str();
    final+=ChangeFileExt (ExtractFileName (elFichero),"");
             
    if  (e_head_sbb.machine[0]=='C')
        final+=".cdt";
    else
        final+=".tzx" ;

    if (e_head_sbb.machine[0]=='8')
       samples=MuestrasCmbBx->Text.SubString(1,1).ToIntDef(6);
    else
       samples=s_p_b/4.0;
       
    params.sprintf("O.T.L.A. Samples/bit:%.2f, %iHz " ,
                   samples , freq_muestreo);

    res=wav2tzx ( auxFichero.c_str(),final.c_str(), params.c_str());
    if (res)
    {
        AnsiString  mens= final +"\r\n";
        mens+=error_file(res);
        MessageDlg(mens,  mtError, TMsgDlgButtons() << mbOK, 0);
    }
    DeleteFile (auxFichero);
}
//---------------------------------------------------------------------------

void __fastcall TotlaFrm::Wavplayer1Click(TObject *Sender)
{
    MonitorFrm->ShowModal();
}

void __fastcall TotlaFrm::Batchsbb1Click(TObject *Sender)
{                                     
    BatchFrm->InputDirEdt->Text=OpenDialog1->InitialDir;
    BatchFrm->OutputDirEdt->Text=OutPutSBB;
    BatchFrm->Batch ( e_head_sbb.machine[0] , 1 ) ;
}
//---------------------------------------------------------------------------

void __fastcall TotlaFrm::Batchwav1Click(TObject *Sender)
{
    set_wav_vars();
    BatchFrm->InputDirEdt->Text=OutPutSBB;
    BatchFrm->OutputDirEdt->Text=OutPutDir;
    BatchFrm->Batch ( e_head_sbb.machine[0] , 2) ;
}
//---------------------------------------------------------------------------

void __fastcall TotlaFrm::Adjustaudio1Click(TObject *Sender)
{     
   motor   =ReleMotorChkBx->Checked;
   acelerar=AccelerateCHkBx->Checked;
   AdjustFrm->Maquina =e_head_sbb.machine[0];
   AdjustFrm->FrecuenciaCmbBx->ItemIndex= FrecuenciaCmbBx->ItemIndex;
   AdjustFrm->FormaCmbBx->ItemIndex= 0;//FormaCmbBx->ItemIndex;
   AdjustFrm->Caption="Adjust test for "+AnsiString (NombreMachine(e_head_sbb.machine[0]));
   AdjustFrm->OutputDir=OutPutDir;
   AdjustFrm->ShowModal();
}                                                            
//---------------------------------------------------------------------------
void __fastcall TotlaFrm::Wiki1Click(TObject *Sender)
{
    ShellExecute(Handle, NULL,"http://code.google.com/p/otla/w/list", NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------
void __fastcall TotlaFrm::About1Click(TObject *Sender)
{
   AboutBox->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TotlaFrm::NameEdtChange(TObject *Sender)
{
   Modificado=1;
}
//---------------------------------------------------------------------------

