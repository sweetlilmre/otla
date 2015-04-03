//---------------------------------------------------------------------
#include <vcl.h>


#pragma hdrstop

#include "InputForm.h"


#include "funciones.h"

#include "zxfiles.h"
#include "cpcfiles.h"
#include "msxfiles.h"  
#include "81files.h"

#include "ModificaForm.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TInputBlocksDlg *InputBlocksDlg;

//---------------------------------------------------------------------
__fastcall TInputBlocksDlg::TInputBlocksDlg(TComponent* AOwner)
        : TForm(AOwner)
{}
//---------------------------------------------------------------------
extern AnsiString ToolRom2Bin;
extern AnsiString ToolDiskMgr;
extern AnsiString ToolCPC;

int lee_msx_rom (const char *filename,ts_sbb_header *head,ts_sbb_block *blocks_rom,const char * rom2bin )
{
   int res=0;
   AnsiString comand_line;

   AnsiString SaveDir=GetCurrentDir();
   if (!SetCurrentDir (ExtractFilePath(ToolRom2Bin)))
   {
       // ShowMessage ("Coud not set directory for msx rom2bin");
        MessageDlg("Coud not set directory for msx rom2bin",  mtError, TMsgDlgButtons() << mbOK, 0);

        return -1;
   }

   AnsiString mask= ExtractFilePath(ToolRom2Bin) +"temp*.bin";
   //todo
   TSearchRec sr;

   if (FindFirst( mask ,  faArchive  ,sr) == 0)
   {
      do
      {
        DeleteFile (sr.Name) ; 
      } while (FindNext(sr) == 0);
   FindClose(sr);
   }

   DeleteFile("temp.rom");
   DeleteFile("out.txt");
   CopyFile(filename,"temp.rom",false);
   comand_line= ExtractFileName(ToolRom2Bin)+" temp.rom >> out.txt";

   system (comand_line.c_str());



   int b=0;
   if (FindFirst( mask ,  faArchive  ,sr) == 0)
   {
   do
   {
        lee_msx_bin (sr.Name.c_str(),head,&blocks_rom[b]);
        blocks_rom[b].blockname[0]=b+'1';
        memcpy (blocks_rom[b].blockname+1,ExtractFileName (filename ).c_str(),5);
        b++;
   } while (FindNext(sr) == 0);
   FindClose(sr);
   }
   if (b==0)
      res=-4;

   memset ( head,0, sizeof (head));   //2.2
   strcpy(head->machine,"MSX");
   head->model = -1;
   head->ei_di = 1;
   asigna_nombre (head, filename);
   head->locate  = 0xf4;
   head->n_blocks  = b;
   head->origin= _rom_;
   head->poke_ffff=170;


   InputBlocksDlg->Memo1->Lines->LoadFromFile ("out.txt");
   SetCurrentDir(SaveDir);

   return res;
}

int __fastcall TInputBlocksDlg::ROMMSX (const char * file)
{

    int res;
  

    res= lee_msx_rom ( file , &head_sbb,&blocks_sbb[0],NULL);

    return res;
/*
    AnsiString comand_line;

    AnsiString SaveDir=GetCurrentDir();
    if (!SetCurrentDir (ExtractFilePath(ToolRom2Bin)))
    {
        ShowMessage ("Coud not set directory for msx rom2bin");
        return -1;
    }

    AnsiString mask= ExtractFilePath(ToolRom2Bin) +"temp*.bin";
    TSearchRec sr;


  if (FindFirst( mask ,  faArchive  ,sr) == 0)
  {
    do
    {
        DeleteFile (sr.Name)   ;

    } while (FindNext(sr) == 0);
    FindClose(sr);
  }

    DeleteFile("temp.rom");



    DeleteFile("out.txt");

    CopyFile(file,"temp.rom",false);

    comand_line= ExtractFileName(ToolRom2Bin)+" temp.rom >> out.txt";

    system (comand_line.c_str());
                     
    Memo1->Lines->LoadFromFile ("out.txt");



  int b=0;
  if (FindFirst( mask ,  faArchive  ,sr) == 0)
  {
    do
    {
      {
        lee_msx_bin (sr.Name.c_str(),&head_sbb,&blocks_sbb[b]);
        blocks_sbb[b].blockname[0]=b+'1';
        memcpy (blocks_sbb[b].blockname+1,ExtractFileName (file ).c_str(),5);
        b++;
      }
    } while (FindNext(sr) == 0);
    FindClose(sr);
  }
  head_sbb.poke_ffff=170;
    SetCurrentDir(SaveDir);
    return res;
    */
}

int __fastcall TInputBlocksDlg::Abre(AnsiString Fichero)
{
    int res=-4;
    AnsiString  extension=ExtractFileExt(Fichero).UpperCase();

    strncpy (blocks_sbb[0].blockname,ExtractFileName (Fichero).c_str(),16);
    blocks_sbb[0].block_type=0;

   // usrN=clearN=0;

    Memo1->Visible=true;
    Memo1->Clear();

    if  (extension==".SBB")
    {
        Memo1->Visible=false;
        char mq=head_sbb.machine[0];
        res=load_sbb_file (Fichero.c_str(),&head_sbb,blocks_sbb);
        if (!res)
            if (mq!=head_sbb.machine[0])
              res=-6;
    }
    if  (extension==".HST")
    {
        Memo1->Visible=false;
        res=load_hst_file (Fichero.c_str(),&head_sbb,blocks_sbb);
    }
    if  (extension==".SCR")
    {
        Memo1->Visible=false;
        res=leescr (Fichero.c_str(),&head_sbb,blocks_sbb);
    }

    if  (extension==".CAS")
        res=leecas (Fichero.c_str(),&head_sbb,blocks_sbb);
    if  (extension==".CDT")
        res=leetzx (Fichero.c_str(),&head_sbb,blocks_sbb);
    if  (extension==".TZX")
        res=leetzx (Fichero.c_str(),&head_sbb,blocks_sbb);
    if  (extension==".TAP")
        res=leetap (Fichero.c_str(),&head_sbb,blocks_sbb);

    if  (extension==".Z80")
        res=leez80 (Fichero.c_str(),&head_sbb,blocks_sbb);

    if  (extension==".SNA")
        if (head_sbb.machine[0]=='C')
            res=lee_cpc_sna (Fichero.c_str(),&head_sbb,blocks_sbb);
        else
            res=lee_zx_sna (Fichero.c_str(),&head_sbb,blocks_sbb);

    if  (extension==".BIN")
        if (head_sbb.machine[0]=='C')
            res=lee_cpc_bin (Fichero.c_str(),&head_sbb,blocks_sbb);
        else
            res=lee_msx_bin (Fichero.c_str(),&head_sbb,blocks_sbb);

    if (extension==".ROM")
        res=ROMMSX (Fichero.c_str());

    if  (extension==".P")
        res=lee_p (Fichero.c_str(),&head_sbb,blocks_sbb);
    if  (extension==".Z81")
        res=lee_z81 (Fichero.c_str(),&head_sbb,blocks_sbb);

    if (extension==".DSK")
    {
        AnsiString comand=" \""+ Fichero +"\"";
        if (head_sbb.machine[0]=='C')
            comand=ToolCPC + comand;
        else
            comand=ToolDiskMgr + comand;
        system (comand.c_str()) ;
        ShowMessage (error_file(res));
        return ModalResult=mrCancel;
    }



    if (res==0)
    {
        filldata ();
        ShowModal();
    }
    else
    {
       // ShowMessage (error_file(res));
        AnsiString men=Fichero+"\r\n";
        men+=error_file(res);
        MessageDlg(men,  mtError	, TMsgDlgButtons() << mbOK, 0);
        ModalResult=mrCancel;
    }
    return ModalResult;
}

int   __fastcall TInputBlocksDlg::Edit(ts_sbb_header  *in_head_sbb ,ts_sbb_block   *in_blocks_sbb)
{
       head_sbb=* in_head_sbb;
       for (int i=0;i<15;i++)
            blocks_sbb[i]=in_blocks_sbb[i];
       filldata();
       ShowModal();
       return ModalResult;
}
//---------------------------------------------------------------------------
// 0 = valid  block
// 1 = warning block
// 2 = error block
int estate (ts_sbb_block *block)
{
    switch (block->block_type)
    {
        case _test_: return 0;

        case _zx_program_   : return 3;
        case _zx_char_array_: return 2;
        case _zx_num_array_ : return 2;
        case _zx_bytes_     :
        case _zx_tap_hdlss_:
        case _zx_tzx_hdlss_:
            if (block->ini==0xffff)
                return 2;      
            if ((block->ini<0x4000)&&(block->block_type==_zx_bytes_))
                return 1;
            if (block->size+block->ini> MAX_DIR_ZX)
                return 1;
            return 0;
        case _zx_48k_snap_ : return 0;
        case _zx_128k_snap_: return 0;
        case _zx_16k_page_ : return 0;
        case _zx_complete_ : return 0;

        case _cpc_basic_    : return 2;
        case _cpc_ascii_    : return 2;
        case _cpc_binary_   :
            if (block->size+block->ini>0xff00)
                return 1;
            return 0;
        case _cpc_protected_: return 2;
        case _cpc_64k_snap_ : return 0;

        case _msx_ascii_: return 2;
        case _msx_bytes_:
            if (block->size+block->ini>0xf400)
                return 1;
            return 0;
        case _msx_token_: return 2;
        case _msx_bin_:   
            if (block->size+block->ini>0xf400)
                return 1;
            return 0;;
            
        case _81_program_ : return 0;
        case _81_snap_:     return 0;
        
        default: return 2;
    }
}

void __fastcall TInputBlocksDlg:: filldata()
{
    TListItem * item;
    AnsiString s;

    int i=0;
    char n1 [20];

  
    ListView1->Items->Clear();

    ClearEdt->Text="";
    UsrCmbBx->Text="";




    while (i<16)
    {
        if (blocks_sbb[i].block_type==0)
            break;

        item=ListView1->Items->Add();
        item->Data= &blocks_sbb[i];

        item->Caption=blocks_sbb[i].blockname;


        if (blocks_sbb[i].block_type!='0')
        {
           item->SubItems->Add(n2s(blocks_sbb[i].size,enheaxadecimal));
           item->SubItems->Add(TipoBloque(blocks_sbb[i].block_type));
           item->SubItems->Add(n2s(blocks_sbb[i].ini,enheaxadecimal));
           item->SubItems->Add(n2s(blocks_sbb[i].exec,enheaxadecimal));
           item->SubItems->Add(" ");
           item->SubItems->Add(" ");
           item->SubItems->Add(" ");
        }
        else
        {
           buscaUSR(blocks_sbb[i].data,blocks_sbb[i].size);

           item->SubItems->Add(n2s(blocks_sbb[i].size,enheaxadecimal));
           item->SubItems->Add(TipoBloque(blocks_sbb[i].block_type));
           item->SubItems->Add("..");
           item->SubItems->Add("..");
           item->SubItems->Add(n2s(blocks_sbb[i].ini,enheaxadecimal)); //parma3
           //item->SubItems->Add(n2s(clearN,enheaxadecimal));
           //item->SubItems->Add(n2s(usrN,enheaxadecimal));
           item->SubItems->Add(n2s(head_sbb.clear_sp,enheaxadecimal));
           item->SubItems->Add(n2s(head_sbb.usr_pc,enheaxadecimal));

        }


        item->ImageIndex=estate (&blocks_sbb[i]) ;
        item->Checked =(item->ImageIndex<=1);


        i++;
    }


    if (head_sbb.clear_sp)
        ClearEdt->Text=n2s(head_sbb.clear_sp,enheaxadecimal);
    if (head_sbb.usr_pc)
        UsrCmbBx->Text=n2s(head_sbb.usr_pc,enheaxadecimal);
    else
    {
       head_sbb.usr_pc=blocks_sbb[i-1].exec;
       if (head_sbb.usr_pc)
          UsrCmbBx->Text=n2s(head_sbb.usr_pc,enheaxadecimal);
    }


    
    char texto[65];
    char buffer[65];
    short dir;
    if (head_sbb.machine[0]=='Z')
    {
        Memo1->Clear();
        Memo1->Lines->Add("Memory dump of printer buffer:");
        Memo1->Lines->Add("------------------------------");
        for (int f=0;f<8;f++)
        {
            dir= /*0x5b00+*/f*32;
            AnsiString linea="$5b";
            BinToHex((char*)&dir,texto,1) ;
            texto[2]=0;
            linea+=texto;
            linea+=": ";
            memcpy  (buffer,&printer_buffer[dir],32);
            BinToHex(buffer,texto,32) ;
            texto[64]=0;
            linea+=texto;
            Memo1->Lines->Add(linea);
        }
    }
    
    if (head_sbb.machine[0]=='C')
       Memo1->Visible = 0;

    if (head_sbb.machine[0]=='M')
        if (blocks_sbb[0].block_type==(char) 0xEA)
               Memo1->Text= (char *)blocks_sbb[0].data;
    if (head_sbb.machine[0]=='8')
    {
        Memo1->Clear();
        Memo1->Lines->Add("Memory dump of PRBUFF+MEMBOT:");
        Memo1->Lines->Add("------------------------------");
        for (int f=0;f<2;f++)
        {
            dir= f*32;
            AnsiString linea="";
            memcpy  (buffer,&prbuff_membot[dir],32);
            BinToHex(buffer,texto,32) ;
            texto[64]=0;
            linea+=texto;
            Memo1->Lines->Add(linea);
        }
    }

}


void __fastcall TInputBlocksDlg::OKBtnClick(TObject *Sender)
{
    int b=0;

    head_sbb.clear_sp=inputDecHex (ClearEdt->Text.c_str());
    
    head_sbb.usr_pc  =inputDecHex (UsrCmbBx->Text.c_str());

    for (int i =0; i<ListView1->Items->Count;i++)
    {
      if (blocks_sbb[i].block_type==_81_snap_)
        ListView1->Items->Item[i]->Checked=true;
      if (ListView1->Items->Item[i]->Checked )
        {
            res_blocks_sbb[b]=blocks_sbb[i];
            if ( (head_sbb.machine[0]=='Z')&& blocks_sbb[i].block_type==_zx_bytes_)
            {
             //   unsigned short dir_last= blocks_sbb[i].ini+blocks_sbb[i].size-1;
                if ((blocks_sbb[i].ini+blocks_sbb[i].size-1)>=MAX_DIR_ZX)
                {
                  zxlastbytes(&head_sbb,&res_blocks_sbb[b]);
                /*
                    res_blocks_sbb[b].size=MAX_DIR_ZX-blocks_sbb[i].ini;
                    copy_data (&res_blocks_sbb[b],blocks_sbb[i].data);

                    memcpy (complete_zx+16,blocks_sbb[i].data+(MAX_DIR_ZX-blocks_sbb[i].ini), 0xe0);

                    complete_zx[8]=dir_last-MAX_DIR_ZX;
                    complete_zx [14]=head_sbb.usr_pc&0xff;
                    complete_zx [15]=head_sbb.usr_pc>>8;
                    head_sbb.usr_pc= 0x5b00;


                    memcpy (res_blocks_sbb[b+1].blockname,"last bytes",12);
                    res_blocks_sbb[b+1].block_type=_zx_complete_;
                    res_blocks_sbb[b+1].ini=0x5b00;
                    res_blocks_sbb[b+1].size=16+0xe0;
                    res_blocks_sbb[b+1].exec=0x5b00;
                    copy_data (&res_blocks_sbb[b+1],complete_zx);
                          */
                    b++;
                }
            }
            b++;
        }
    }
    res_blocks_sbb[b].block_type=0;
}
//---------------------------------------------------------------------------


void __fastcall TInputBlocksDlg::Modify1Click(TObject *Sender)
{      
      ts_sbb_block *b= (ts_sbb_block *) ListView1->Selected->Data;
      ModificaDlg->NameEdt->Text=b->blockname;
      ModificaDlg->StartEdt->Text=n2s(b->ini,enheaxadecimal);
      ModificaDlg->SizeEdt->Text=n2s(b->size,enheaxadecimal);
      ModificaDlg->ExecEdt->Text=n2s(b->exec,enheaxadecimal);
      ModificaDlg->ShowModal();
      if (ModificaDlg->ModalResult==mrOk)
      {
        strncpy (b->blockname,ModificaDlg->NameEdt->Text.c_str(),16);
        b->ini =inputDecHex(ModificaDlg->StartEdt->Text.c_str());
        b->size=inputDecHex(ModificaDlg->SizeEdt->Text.c_str());
        b->exec=inputDecHex(ModificaDlg->ExecEdt->Text.c_str());
        filldata();
      }
}
//---------------------------------------------------------------------------

void __fastcall TInputBlocksDlg::Seedata1Click(TObject *Sender)
{
    char texto[65];
    char buffer[65];
    ModificaDlg->Memo1->Clear();
    ts_sbb_block *b= (ts_sbb_block *) ListView1->Selected->Data;
    for (int f=0;f<b->size;f+=32)
    {
            memcpy  (buffer,b->data+f,32);
            BinToHex(buffer,texto,32) ;
            texto[64]=0;
            ModificaDlg->Memo1->Lines->Add(texto);

     }

      ModificaDlg->Width=450;
      ModificaDlg->Height=350;
      ModificaDlg->Memo1->Visible=true;
      ModificaDlg->ShowModal();
      ModificaDlg->Memo1->Visible=false;
      ModificaDlg->Width=325;
      ModificaDlg->Height=204;
}
//---------------------------------------------------------------------------

void __fastcall TInputBlocksDlg::PopupMenu1Popup(TObject *Sender)
{
    try{
    ts_sbb_block *b= (ts_sbb_block *) ListView1->Selected->Data;
    
    PopupMenu1->Items->Items[0]->Enabled=true;
    PopupMenu1->Items->Items[1]->Enabled=true;
    PopupMenu1->Items->Items[2]->Enabled=true;
    PopupMenu1->Items->Items[3]->Enabled =(b->block_type==_zx_program_) ;
    PopupMenu1->Items->Items[4]->Enabled =(b->block_type==_zx_48k_snap_)
                                          &&(b->data[0x1b00+40]!=0xc3)
                                          &&Memo1->Visible ;
    }
    catch (...)
    {
     PopupMenu1->Items->Items[0]->Enabled=false;
     PopupMenu1->Items->Items[1]->Enabled=false;
     PopupMenu1->Items->Items[2]->Enabled=false;
     PopupMenu1->Items->Items[3]->Enabled=false;
     PopupMenu1->Items->Items[4]->Enabled=false;
    }

}
//---------------------------------------------------------------------------

void __fastcall TInputBlocksDlg::ConverBasic1Click(TObject *Sender)
{
      ts_sbb_block *b= (ts_sbb_block *) ListView1->Selected->Data;
      ts_sbb_block back= *b;
      zxbasic2bytes(b);
      Modify1Click(NULL);
      if (ModificaDlg->ModalResult!=mrOk)
         *(ts_sbb_block *) ListView1->Selected->Data=back;
      else
          UsrCmbBx->Text=b->exec;
}
//---------------------------------------------------------------------------

void __fastcall TInputBlocksDlg::Restore1Click(TObject *Sender)
{
      ts_sbb_block *b= (ts_sbb_block *) ListView1->Selected->Data;
      ts_sbb_block back= *b;
      restorefromscreen(b);
      Modify1Click(NULL);
      if (ModificaDlg->ModalResult!=mrOk)
         *(ts_sbb_block *) ListView1->Selected->Data=back;
}
//---------------------------------------------------------------------------

void __fastcall TInputBlocksDlg::Viewdecimalhexadecimal1Click(
      TObject *Sender)
{
        enheaxadecimal=!enheaxadecimal;
        if (enheaxadecimal)
            PopupMenu1->Items->Items[1]->Caption="Decimal Values";
        else
            PopupMenu1->Items->Items[1]->Caption="Hexadecimal Values";

       filldata();
}
//---------------------------------------------------------------------------

