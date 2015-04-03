//---------------------------------------------------------------------------

#ifndef MainFormH
#define MainFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <CheckLst.hpp>
#include <Dialogs.hpp>
#include <Grids.hpp>
#include <Outline.hpp>
#include <FileCtrl.hpp>
#include <ComCtrls.hpp>
#include <ValEdit.hpp>
#include <Menus.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------

extern AnsiString error_men;


class TForm1 : public TForm
{
__published:	// IDE-managed Components
     TGroupBox *MethodGB;
     TRadioButton *MilksRB;
     TRadioButton *SlowRB;
     TRadioButton *DeltaRB;
     TRadioButton *UltraRB;
     TGroupBox *SpeedGB;
     TRadioButton *r1_75RB;
     TRadioButton *r2_50RB;
     TRadioButton *r2_75RB;
     TRadioButton *r3_50RB;
     TRadioButton *r2_00RB;
     TRadioButton *r2_25RB;
     TRadioButton *r3_00RB;
     TRadioButton *r4_00RB;
        TGroupBox *EsquemaGB;
     TRadioButton *UnBloqueRB;
        TEdit *ClearEdt;
     TLabel *Label3;
     TLabel *Label4;
        TEdit *UsrEdt;
     TRadioButton *BloquesRB;
        TCheckBox *BasicxxChkBx;
        TCheckListBox *TrozosCLB;
     TDirectoryListBox *DirectoryListBox1;
     TFileListBox *FileListBox1;
     TDriveComboBox *DriveComboBox1;
        TCheckBox *Sync1ChkBx;
        TButton *ConvertirBtn;
        TRadioButton *r1_33RB;
        TRadioButton *r1_50RB;
        TRadioButton *r1_25RB;
        TRadioButton *EscurridoRB;
        TRadioButton *NonPlusRB;
        TMemo *Memo1;
        TButton *EmularBtn;
        TButton *PlayBtn;
        TButton *StopBtn;
        TEdit *NombreEdt;
        TLabel *Label5;
        TButton *TodosBtn;
        TLabel *Label6;
        TCheckBox *TapeErrorChkBx;
        TComboBox *EscurridoCmbBx;
        TCheckBox *InfoChkBx;
        TValueListEditor *PokesVLE;
     TCheckBox *PokeadorChkBx;
        TRadioButton *FiRB;
        TRadioButton *r5_00RB;
        TRadioButton *r6_00RB;
        TGroupBox *OndaGB;
        TLabel *Label1;
        TCheckBox *InvertidaChkBx;
        TComboBox *FormaCmbBx;
        TCheckBox *CDChk;
        TComboBox *FrecuenciaCmbBx;
        TCheckBox *MaxVolChkBx;
        TCheckBox *FinalChkBx;
        TRadioButton *r7_00RB;
        TLabel *Label2;
        TRadioButton *TurboRB;
        TRadioButton *PWMRB;
        TRadioButton *Manch_ieeeRB;
        TRadioButton *r8_00RB;
        TComboBox *ManchesterOLDCmbBx;
        TCheckBox *InvRChkBx;
        TCheckBox *DivisorFChkBx;
        TGroupBox *SnapGB;
        TLabel *TipoSnapLbl;
        TLabel *PCLbl;
        TLabel *SPLbl;
        TLabel *CurrentLbl;
        TRadioButton *OriginalRB;
        TRadioButton *FSKRB;
        TTabControl *TabControl1;
        TLabel *Label7;
        TMainMenu *MainMenu1;
        TMenuItem *About1;
        TCheckBox *AcelerarChk;
        TCheckBox *CargadorChk;
        TRadioButton *ROMRB;
        TImageList *ImageList1;
        TCheckBox *KolmogorovChk;
        TRadioButton *Lento4000RB;
        TRadioButton *Lento6000RB;
        TRadioButton *Manch_difRB;
        TRadioButton *Lento11RB;
        TRadioButton *Lento8000RB;
     void __fastcall UltraRBClick(TObject *Sender);
        void __fastcall ConvertirBtnClick(TObject *Sender);
        void __fastcall CargadorChkClick(TObject *Sender);
        void __fastcall FileListBox1Change(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall EmularBtnClick(TObject *Sender);
        void __fastcall PlayBtnClick(TObject *Sender);
        
        int __fastcall interfaz();
        void __fastcall StopBtnClick(TObject *Sender);
        void __fastcall TodosBtnClick(TObject *Sender);
        void __fastcall FrecuenciaCmbBxChange(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall CDChkClick(TObject *Sender);
        void __fastcall DivisorFChkBxClick(TObject *Sender);
        void __fastcall TabControl1Change(TObject *Sender);
        void __fastcall About1Click(TObject *Sender);
        void __fastcall FormaCmbBxDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
        void __fastcall FormDblClick(TObject *Sender);
        
private:	// User declarations
public:		// User declarations
     __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
