//---------------------------------------------------------------------------

#ifndef mainFormH
#define mainFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TotlaFrm : public TForm
{
__published:	// IDE-managed Components
   TMainMenu *MainMenu1;
   TMenuItem *File1;
   TMenuItem *New1;
   TMenuItem *NewSave1;
   TMenuItem *Open1;
   TMenuItem *Save1;
   TMenuItem *Saveas1;
   TMenuItem *Quit1;
   TMenuItem *Covert1;
   TMenuItem *soundPlay1;
   TMenuItem *wav1;
   TMenuItem *mp31;
   TMenuItem *tzx1;
   TMenuItem *Settings1;
   TMenuItem *Tools1;
   TMenuItem *Batchsbb1;
   TMenuItem *Batchwav1;
   TMenuItem *Adjustaudio1;
   TMenuItem *Help1;
   TMenuItem *Editblocks1;
   TMenuItem *Wavplayer1;
   TGroupBox *BuilderGB;
   TListView *BlocksLV;
   TGroupBox *HeaderGB;
   TLabel *NameLbl;
   TLabel *ClearLbl;
   TLabel *UsrLbl;
   TLabel *PokeLbl;
   TLabel *AditionalLbl;
   TLabel *BlocksLbl;
   TLabel *Label2;
   TLabel *LocateLbl;
   TLabel *ModelLbl;
   TEdit *NameEdt;
   TEdit *ClearEdt;
   TEdit *UsrEdt;
   TEdit *PokeEdt;
   TEdit *AditionalInfoEdt;
   TButton *AddBtn;
   TEdit *BloksEdt;
   TEdit *OriginEdt;
   TEdit *ReubicaEdt;
   TComboBox *MaquinaCmbBx;
   TComboBox *ModelCmbBx;
   TImageList *MachineImgLst;
   TOpenDialog *OpenDialog1;
   TGroupBox *PlayerGB;
   TGroupBox *WavGB;
   TLabel *Label3;
   TLabel *Label4;
   TLabel *bpsLbl;
   TLabel *Label9;
   TCheckBox *InvertChkBx;
   TComboBox *FormaCmbBx;
   TComboBox *FrecuenciaCmbBx;
   TComboBox *MuestrasCmbBx;
   TEdit *pausaEdt;
   TGroupBox *LoaderGB;
   TCheckBox *CheckLoadErrorChkBx;
   TCheckBox *ReleMotorChkBx;
   TCheckBox *MakeLoaderChkBx;
   TCheckBox *AccelerateCHkBx;
   TButton *WavBtn;
   TButton *Mp3Btn;
   TButton *TzxBtn;
   TButton *PlayBtn;
   TImageList *wavImgLst;
   TSaveDialog *SaveDialog1;
   TCheckBox *EnableIntChkBx;
   TStatusBar *StatusBar1;
   TMenuItem *About1;
   TEdit *FileEdt;
   TMenuItem *N1;
   TMenuItem *Wiki1;
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall MaquinaCmbBxChange(TObject *Sender);
   void __fastcall MaquinaCmbBxDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
   void __fastcall New1Click(TObject *Sender);
   void __fastcall NewSave1Click(TObject *Sender);
   void __fastcall AddBtnClick(TObject *Sender);
   void __fastcall Open1Click(TObject *Sender);
   void __fastcall FrecuenciaCmbBxChange(TObject *Sender);
   void __fastcall FormaCmbBxDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
   void __fastcall Settings1Click(TObject *Sender);
   void __fastcall PlayBtnClick(TObject *Sender);
   void __fastcall WavBtnClick(TObject *Sender);
   void __fastcall Mp3BtnClick(TObject *Sender);
   void __fastcall TzxBtnClick(TObject *Sender);
   void __fastcall Editblocks1Click(TObject *Sender);
   void __fastcall Wavplayer1Click(TObject *Sender);
   void __fastcall Save1Click(TObject *Sender);
   void __fastcall Saveas1Click(TObject *Sender);
   void __fastcall Batchsbb1Click(TObject *Sender);
   void __fastcall Batchwav1Click(TObject *Sender);
   void __fastcall Adjustaudio1Click(TObject *Sender);
   void __fastcall About1Click(TObject *Sender);
   void __fastcall Wiki1Click(TObject *Sender);
   void __fastcall NameEdtChange(TObject *Sender);
private:	// User declarations
                     
      AnsiString    elFichero;   
      bool Nuevo;
      bool Modificado;

      void __fastcall UpdateHeader();


      void __fastcall ClearData();
      void __fastcall FillData();
      void __fastcall CambiaMaquina(char maquina);


      AnsiString  wavFile;
      void set_wav_vars ();
      AnsiString __fastcall MakeNameWav (AnsiString elFichero) ;


      int  __fastcall AutoSaveSBB () ;

     // AnsiString Warning;

      //  char maquina;
        bool associate_sbb;
        bool view_hex;

        
        int bit_rate_mp3;
        bool force_cubic;
        AnsiString LameExe;
        AnsiString OutPutDir;
        bool output_info;

        bool show_info;


      int __fastcall OpenFileSBB(AnsiString FileToOpen);


public:		// User declarations
   __fastcall TotlaFrm(TComponent* Owner);
   
        AnsiString IniFileName;
        AnsiString OutPutZX;
        AnsiString OutPutCPC;
        AnsiString OutPutMSX;
        AnsiString OutPut81;

        AnsiString OutPutSBB;        
        AnsiString InPutDir;

        void __fastcall LameProcess(AnsiString WavIn, char maq , char * title );

};
//---------------------------------------------------------------------------
extern PACKAGE TotlaFrm *otlaFrm;
//---------------------------------------------------------------------------
#endif
