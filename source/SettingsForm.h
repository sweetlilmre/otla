//---------------------------------------------------------------------------

#ifndef SettingsFormH
#define SettingsFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TSettingsFrm : public TForm
{
__published:	// IDE-managed Components
        TOpenDialog *OpenDialog1;
        TButton *AceptarBtn;
        TButton *CancelBtn;
        TGroupBox *GroupBox1;
        TLabel *Label1;
        TLabel *Label3;
        TLabel *Label4;
        TEdit *DirZXEdt;
        TButton *DirZXBtn;
        TEdit *DirCPCEdt;
        TButton *DirCPCBtn;
        TEdit *DirMSXEdt;
        TButton *DirMSXBtn;
        TGroupBox *GroupBox2;
        TLabel *Label2;
        TLabel *Label5;
        TLabel *Label6;
        TEdit *Rom2BinEdt;
        TButton *Rom2BinBtn;
        TEdit *DiskMgrEdt;
        TButton *DiskMngrBtn;
        TEdit *CPCToolEdt;
        TButton *CPCToolBtn;
        TLabel *Label8;
        TEdit *Dir81Edt;
        TButton *Dir81Btn;
   TCheckBox *HexadecimalChkBx;
   TGroupBox *GroupBox3;
   TLabel *Label7;
   TLabel *Label9;
   TLabel *Label10;
   TEdit *WavDirEdt;
   TEdit *LameEdt;
   TButton *DirBtn;
   TButton *LameBtn;
   TComboBox *BitRateCmbBx;
   TCheckBox *ForceCubicChkBx;
   TCheckBox *InfoChkBx;
   TCheckBox *ExtensionChkBx;
        void __fastcall DirZXBtnClick(TObject *Sender);
        void __fastcall Rom2BinBtnClick(TObject *Sender);
        void __fastcall CancelBtnClick(TObject *Sender);
        void __fastcall AceptarBtnClick(TObject *Sender);
        void __fastcall DirCPCBtnClick(TObject *Sender);
        void __fastcall DirMSXBtnClick(TObject *Sender);
        void __fastcall DiskMngrBtnClick(TObject *Sender);
        void __fastcall CPCToolBtnClick(TObject *Sender);
        void __fastcall Dir81BtnClick(TObject *Sender);
   void __fastcall DirBtnClick(TObject *Sender);
   void __fastcall LameBtnClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TSettingsFrm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSettingsFrm *SettingsFrm;
//---------------------------------------------------------------------------
#endif
