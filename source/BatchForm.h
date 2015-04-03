//---------------------------------------------------------------------------

#ifndef BatchFormH
#define BatchFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TBatchFrm : public TForm
{
__published:	// IDE-managed Components
   TMemo *Memo1;
   TButton *ProccedBtn;
   TLabel *InputLbl;
   TEdit *InputDirEdt;
   TButton *InputDirBtn;
   TLabel *OutputLbl;
   TEdit *OutputDirEdt;
   TButton *OutputDirBtn;
   TButton *CancelBtn;
   TComboBox *ExtensionCmbBx;
   TLabel *Label2;
   TProgressBar *ProgressBar1;
   TCheckBox *MakeMp3ChkBx;
   void __fastcall InputDirBtnClick(TObject *Sender);
   void __fastcall OutputDirBtnClick(TObject *Sender);
   void __fastcall ProccedBtnClick(TObject *Sender);
private:	// User declarations
   void __fastcall ConverToSbb( );
   void __fastcall ConverToWav( );
   int  __fastcall BuildSbb(AnsiString File );
   int  __fastcall N_Files(AnsiString mask );
   char Maquina;
   int Tipo;
   int M_y_E;
public:		// User declarations
   __fastcall TBatchFrm(TComponent* Owner);

   void __fastcall Batch ( char m , int t );


};
//---------------------------------------------------------------------------
extern PACKAGE TBatchFrm *BatchFrm;
//---------------------------------------------------------------------------
#endif
