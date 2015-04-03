//---------------------------------------------------------------------------

#ifndef SettingdFormH
#define SettingdFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ImgList.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
extern char conversion;
extern char variosbloques;
extern int bitrate;


      
extern char *info_tecnica  [];
extern char *info_kbps [];
extern char *tecnica[];


extern AnsiString info ;

class TSettingsFrm : public TForm
{
__published:	// IDE-managed Components
        TButton *AceptarBtn;
        TGroupBox *OndaGB;
        TLabel *Label1;
        TLabel *Label2;
        TCheckBox *InvertidaChkBx;
        TComboBox *FormaCmbBx;
        TCheckBox *CDChk;
        TComboBox *FrecuenciaCmbBx;
        TCheckBox *MaxVolChkBx;
        TCheckBox *FinalChkBx;
        TCheckBox *InvRChkBx;
        TCheckBox *DivisorFChkBx;
        TCheckBox *AcelerarChk;
        TComboBox *ConversionCmbBx;
        TImageList *ImageList1;
        TGroupBox *GroupBox1;
        TCheckBox *CargadorChk;
        TCheckBox *TapeErrorChkBx;
        TRadioButton *UnBloqueRB;
        TRadioButton *BloquesRB;
        TRadioButton *OriginalRB;
        TGroupBox *GroupBox2;
        TComboBox *MetodoCmbBx;
        TComboBox *MPBCmbBx;
        TCheckBox *KolmogorovChk;
        TGroupBox *MaNonTrppoGB;
        TGroupBox *NormalGB;
        TRadioButton *AndanteRB;
        TRadioButton *AllegroRB;
        TRadioButton *VivaceRB;
        TRadioButton *PrestoRB;
        TLabel *Label4;
        TMemo *Memo1;
        TLabel *Label5;
        TLabel *Label6;
        TButton *CancelarBtn;
        TMemo *Memo2;
        TImageList *BorderIL;
        TImage *Image1;
        TLabel *Label3;
        TRadioButton *MaestosoRB;
        TLabel *Label7;
        TComboBox *RatioMACmbBx;
        TComboBox *RatioNORCmbBx;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label10;
        TRadioButton *ScherzandoRB;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall AceptarBtnClick(TObject *Sender);
        void __fastcall MPBCmbBxDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
        void __fastcall MetodoCmbBxChange(TObject *Sender);
        void __fastcall FrecuenciaCmbBxChange(TObject *Sender);
        void __fastcall ConversionCmbBxChange(TObject *Sender);
        void __fastcall FormaCmbBxDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
        void __fastcall CancelarBtnClick(TObject *Sender);
        void __fastcall MPBCmbBxChange(TObject *Sender);
        void __fastcall CDChkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TSettingsFrm(TComponent* Owner);
        AnsiString  __fastcall Speed(int f,int mpb);
};
//---------------------------------------------------------------------------
extern PACKAGE TSettingsFrm *SettingsFrm;
//---------------------------------------------------------------------------
#endif
