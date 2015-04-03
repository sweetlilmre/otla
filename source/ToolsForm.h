//---------------------------------------------------------------------------

#ifndef ToolsFormH
#define ToolsFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TToolsFrm : public TForm
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
        void __fastcall DirZXBtnClick(TObject *Sender);
        void __fastcall Rom2BinBtnClick(TObject *Sender);
        void __fastcall CancelBtnClick(TObject *Sender);
        void __fastcall AceptarBtnClick(TObject *Sender);
        void __fastcall DirCPCBtnClick(TObject *Sender);
        void __fastcall DirMSXBtnClick(TObject *Sender);
        void __fastcall DiskMngrBtnClick(TObject *Sender);
        void __fastcall CPCToolBtnClick(TObject *Sender);
        void __fastcall Dir81BtnClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TToolsFrm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TToolsFrm *ToolsFrm;
//---------------------------------------------------------------------------
#endif
