//---------------------------------------------------------------------------

#ifndef OptionsFormH
#define OptionsFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TOptionsFrm : public TForm
{
__published:	// IDE-managed Components
        TEdit *WavDirEdt;
        TLabel *Label1;
        TLabel *Label2;
        TEdit *LameEdt;
        TOpenDialog *OpenDialog1;
        TButton *DirBtn;
        TButton *LameBtn;
        TComboBox *BitRateCmbBx;
        TLabel *Label3;
        TButton *AceptarBtn;
        TButton *CancelBtn;
        TCheckBox *ForceCubicChkBx;
    TCheckBox *HexadecimalChkBx;
        TCheckBox *InfoChkBx;
        void __fastcall DirBtnClick(TObject *Sender);
        void __fastcall LameBtnClick(TObject *Sender);
        void __fastcall CancelBtnClick(TObject *Sender);
        void __fastcall AceptarBtnClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TOptionsFrm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TOptionsFrm *OptionsFrm;
//---------------------------------------------------------------------------
#endif
