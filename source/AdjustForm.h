//---------------------------------------------------------------------------

#ifndef AdjustFormH
#define AdjustFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class TAdjustFrm : public TForm
{
__published:	// IDE-managed Components
   TButton *CloseBtn;
   TGroupBox *GroupBox1;
   TButton *PlayProgramBtn;
   TButton *MakeProgramBtn;
   TGroupBox *GroupBox2;
   TComboBox *FrecuenciaCmbBx;
   TComboBox *FormaCmbBx;
   TButton *MakeSignalBtn;
   TCheckBox *InvertChkBx;
   TComboBox *TestSignalCmbBx;
   TButton *PlaySignalBtn;
   TImageList *wavImgLst;
   void __fastcall FormaCmbBxDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
   void __fastcall MakeProgramBtnClick(TObject *Sender);
   void __fastcall MakeSignalBtnClick(TObject *Sender);
   void __fastcall CloseBtnClick(TObject *Sender);
   void __fastcall FrecuenciaCmbBxChange(TObject *Sender);
   void __fastcall PlayProgramBtnClick(TObject *Sender);
   void __fastcall PlaySignalBtnClick(TObject *Sender);
private:	// User declarations
   AnsiString ProgramWav;
public:		// User declarations
   __fastcall TAdjustFrm(TComponent* Owner);
   char Maquina;
   AnsiString OutputDir;
   AnsiString SignalName;
};
//---------------------------------------------------------------------------
extern PACKAGE TAdjustFrm *AdjustFrm;
//---------------------------------------------------------------------------
#endif
