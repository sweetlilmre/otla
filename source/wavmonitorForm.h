
//---------------------------------------------
#ifndef PlayWavUH
#define PlayWavUH
//---------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <mmsystem.h>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------
class TMonitorFrm : public TForm
{
__published:  // IDE-managed Components
  TButton *PlayBtn;
  TButton *OpenBtn;
    TLabel *Label1;
    TOpenDialog *OpenDialog1;
    TTimer *Timer1;
    TComboBox *TarjetasCmbBx;
    TTrackBar *MasterVolBar;
    TProgressBar *ProgressBar1;
    TPanel *Panel1;
    TImage *Image1;
    TCheckBox *MuteChkBx;
    TLabel *SoundCardLbl;
    TLabel *Label4;
    TButton *StopBtn;
    TLabel *Label5;
    TCheckBox *CloseOnChkBx;
    TButton *CloseBtn;
  void __fastcall OpenBtnClick
	(TObject *Sender);
  void __fastcall PlayBtnClick
	(TObject *Sender);
  void __fastcall FormCreate
	(TObject *Sender);
  void __fastcall FormDestroy
	(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall TarjetasCmbBxChange(TObject *Sender);
    void __fastcall MasterVolBarChange(TObject *Sender);
    void __fastcall MuteChkBxClick(TObject *Sender);
    void __fastcall StopBtnClick(TObject *Sender);
    void __fastcall CloseBtnClick(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:  // User declarations
  unsigned  char* WaveData;
  WAVEFORMATEX WaveFmt;
  HWAVEOUT WaveHandle;
  int DataSize;
  WAVEHDR WaveHeader;
  void CheckWaveError(DWORD code);
  void CheckMMIOError(DWORD code);
  void OnWaveDone(TMessage& msg);
  bool Paused;

  int AudioDevice;         
   void __fastcall ActualizaVolumen();

public:    // User declarations
__fastcall TMonitorFrm(TComponent* Owner);
  BEGIN_MESSAGE_MAP
    MESSAGE_HANDLER(
      MM_WOM_DONE, TMessage,
	OnWaveDone)
  END_MESSAGE_MAP(TForm)


  int OpenWaveFile(AnsiString Filename);
};
//---------------------------------------------
extern PACKAGE TMonitorFrm *MonitorFrm;
//---------------------------------------------
#endif