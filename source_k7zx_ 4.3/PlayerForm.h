//---------------------------------------------------------------------------

#ifndef PlayerFormH
#define PlayerFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <CheckLst.hpp>
#include <FileCtrl.hpp>
#include <Grids.hpp>
#include <ValEdit.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <MPlayer.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TPlayerFrm : public TForm
{
__published:	// IDE-managed Components
        TDirectoryListBox *DirectoryListBox1;
        TFileListBox *FileListBox1;
        TDriveComboBox *DriveComboBox1;
        TLabel *Label6;
        TButton *File2WAVBtn;
        TButton *EmularBtn;
        TButton *PlayBtn;
        TButton *StopBtn;
        TCheckBox *InfoChkBx;
        TMainMenu *MainMenu1;
        TMenuItem *Settings1;
        TMenuItem *About1;
        TEdit *OutputEdt;
        TButton *File2MP3Btn;
        TEdit *LameEdt;
        TLabel *Label2;
        TLabel *Label7;
        TGroupBox *GroupBox1;
        TLabel *Label4;
        TLabel *Label3;
        TLabel *Label5;
        TEdit *UsrEdt;
        TEdit *ClearEdt;
        TEdit *NombreEdt;
        TGroupBox *SnapGB;
        TLabel *TipoSnapLbl;
        TLabel *PCLbl;
        TLabel *SPLbl;
        TLabel *CurrentLbl;
        TCheckListBox *TrozosCLB;
        TCheckBox *PokeadorChkBx;
        TValueListEditor *PokesVLE;
        TLabel *Label1;
        TButton *DirBtn;
        TButton *LameBtn;
        TOpenDialog *OpenDialog1;
        TCheckBox *SelectedChkBx;
        TLabel *BitRateLbl;
        TStatusBar *StatusBarra;
        TMediaPlayer *MediaPlayer1;
        TTimer *Timer1;
        TProgressBar *ProgressBar1;
        TLabel *LoaderLbl;
        TLabel *DuraLbl;
        TMemo *Memo1;
        void __fastcall Settings1Click(TObject *Sender);
        void __fastcall About1Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FileListBox1Change(TObject *Sender);
        void __fastcall DirBtnClick(TObject *Sender);
        void __fastcall EmularBtnClick(TObject *Sender);
        void __fastcall File2WAVBtnClick(TObject *Sender);
        void __fastcall PlayBtnClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall LameBtnClick(TObject *Sender);
        void __fastcall File2MP3BtnClick(TObject *Sender);
        void __fastcall StopBtnClick(TObject *Sender);
        void __fastcall SelectedChkBxClick(TObject *Sender);
        void __fastcall MediaPlayer1Notify(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
private:	// User declarations
       bool play;
       bool mp3;
       AnsiString file_final;
       void __fastcall FillProgres( );
        AnsiString file_out;
       void __fastcall VerBarra(bool ver );

public:		// User declarations
        __fastcall TPlayerFrm(TComponent* Owner);
        int __fastcall TPlayerFrm::makewav();
};
//---------------------------------------------------------------------------
extern PACKAGE TPlayerFrm *PlayerFrm;
//---------------------------------------------------------------------------
#endif
