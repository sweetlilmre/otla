
//---------------------------------------------
#include <vcl.h>

#include <inifiles.hpp>
#pragma hdrstop

#include "wavmonitorForm.h"

//---------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMonitorFrm *MonitorFrm;


  HMIXER hMixer;
  
  MIXERCAPS mxcaps;

  MIXERLINE mxl;
  MIXERCONTROL mc;
  MIXERLINECONTROLS mxlc;

  MIXERCONTROLDETAILS mxCDVolumeSpeakers; //mxcd;
  MIXERCONTROLDETAILS mxCDMuteSpeakers;

  MIXERCONTROLDETAILS_UNSIGNED mxdu;


//---------------------------------------------
__fastcall TMonitorFrm::TMonitorFrm (TComponent* Owner) : TForm(Owner)
{
}
//---------------------------------------------

void __fastcall TMonitorFrm::FormCreate(TObject *Sender)
{
  hMixer=NULL;

  TIniFile *ini;
  AnsiString IniFileName=ExtractFilePath(Application->ExeName)+"otla.ini";
  ini = new TIniFile( IniFileName );
  AudioDevice=ini->ReadInteger ( "otlaPlayer", "Audiodevice", 0);
  CloseOnChkBx->Checked=ini->ReadBool ( "otlaPlayer", "CloseOnFinished", 1);
  delete ini;

   //MMRESULT    ;
  int n= mixerGetNumDevs();
  for (int i=0;i<n;i++)
  {
    mixerGetDevCaps(  i,   &mxcaps,  sizeof ( mxcaps));
    TarjetasCmbBx->Items->Add(mxcaps.szPname);
  }
  WaveData = 0;
  // Set the volume bar position to the
  // current volume setting.

  //DWORD volume;

  //waveOutGetVolume(0, &volume);

  //VolumeBar->Position = 0xFFFF - LOWORD(volume);

  Image1->Canvas->Pen->Color=clGreen;
  Image1->Canvas->Brush->Color=clBlack;
  Image1->Canvas->Pen->Width=2;
  Image1->Canvas->FillRect(Rect(0,0,Image1->Width,Image1->Height));

  TarjetasCmbBx->ItemIndex =AudioDevice ;
  TarjetasCmbBxChange(NULL);
}
//---------------------------------------------
void __fastcall TMonitorFrm::ActualizaVolumen()
{
   HRESULT hr;


   mixerGetDevCaps(  AudioDevice ,   &mxcaps,  sizeof ( mxcaps));
	hr = mixerOpen(&hMixer, TarjetasCmbBx->ItemIndex, 0, 0, 0);
	if ( ! FAILED( hr ) )
   {
   memset(&mxl, 0, sizeof(mxl));
   mxl.cbStruct = sizeof(mxl);
   mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;//*ComponentType
   mxdu.dwValue = -1; // default

   mixerGetLineInfo((HMIXEROBJ)hMixer, &mxl, MIXER_GETLINEINFOF_COMPONENTTYPE);

   mc.cbStruct = sizeof(mc);
   mxlc.cbStruct = sizeof(mxlc);
   mxlc.dwLineID = mxl.dwLineID;
   mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
   mxlc.cControls = 1;
   mxlc.cbmxctrl = sizeof(MIXERCONTROL);
   mxlc.pamxctrl = &mc;
   hr = mixerGetLineControls((HMIXEROBJ)hMixer, &mxlc, MIXER_GETLINECONTROLSF_ONEBYTYPE);

   mxCDVolumeSpeakers.cMultipleItems = 0;
   mxCDVolumeSpeakers.cChannels = 1; mxl.cChannels;
   mxCDVolumeSpeakers.cbStruct = sizeof(mxCDVolumeSpeakers);
   mxCDVolumeSpeakers.dwControlID = mc.dwControlID;
   mxCDVolumeSpeakers.cbDetails = sizeof(mxdu);
   mxCDVolumeSpeakers.paDetails = &mxdu;

   hr = mixerGetControlDetails((HMIXEROBJ)hMixer, &mxCDVolumeSpeakers, MIXER_GETCONTROLDETAILSF_VALUE);

   int v= (mxdu.dwValue*100.0)/0x10000+0.5;
   MasterVolBar->Position= 100-v;

   //////////////////////////////

   mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_MUTE;
   hr = mixerGetLineControls((HMIXEROBJ)hMixer, &mxlc, MIXER_GETLINECONTROLSF_ONEBYTYPE);


   mxCDMuteSpeakers.cMultipleItems = 0;
   mxCDMuteSpeakers.cChannels = 1; mxl.cChannels;
   mxCDMuteSpeakers.cbStruct = sizeof(mxCDVolumeSpeakers);
   mxCDMuteSpeakers.dwControlID = mc.dwControlID;
   mxCDMuteSpeakers.cbDetails = sizeof(mxdu);
   mxCDMuteSpeakers.paDetails = &mxdu;

   hr = mixerGetControlDetails((HMIXEROBJ)hMixer, &mxCDMuteSpeakers, MIXER_GETCONTROLDETAILSF_VALUE);

   MuteChkBx->Checked= mxdu.dwValue;
   }
}
//---------------------------------------------
void __fastcall TMonitorFrm::TarjetasCmbBxChange(TObject *Sender)
{
    if (hMixer!=NULL)
       mixerClose (hMixer);
       
	HRESULT hr;

   AudioDevice= TarjetasCmbBx->ItemIndex ;

   ActualizaVolumen ();
   return ;
 /*
   mixerGetDevCaps(  AudioDevice ,   &mxcaps,  sizeof ( mxcaps));
	hr = mixerOpen(&hMixer, TarjetasCmbBx->ItemIndex, 0, 0, 0);
	if ( FAILED( hr ) )
      return ;

   memset(&mxl, 0, sizeof(mxl));
   mxl.cbStruct = sizeof(mxl);
   mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;//*ComponentType
   mxdu.dwValue = -1; // default

   mixerGetLineInfo((HMIXEROBJ)hMixer, &mxl, MIXER_GETLINEINFOF_COMPONENTTYPE);

   mc.cbStruct = sizeof(mc);
   mxlc.cbStruct = sizeof(mxlc);
   mxlc.dwLineID = mxl.dwLineID;
   mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
   mxlc.cControls = 1;
   mxlc.cbmxctrl = sizeof(MIXERCONTROL);
   mxlc.pamxctrl = &mc;
   hr = mixerGetLineControls((HMIXEROBJ)hMixer, &mxlc, MIXER_GETLINECONTROLSF_ONEBYTYPE);

   mxCDVolumeSpeakers.cMultipleItems = 0;
   mxCDVolumeSpeakers.cChannels = 1; mxl.cChannels;
   mxCDVolumeSpeakers.cbStruct = sizeof(mxCDVolumeSpeakers);
   mxCDVolumeSpeakers.dwControlID = mc.dwControlID;
   mxCDVolumeSpeakers.cbDetails = sizeof(mxdu);
   mxCDVolumeSpeakers.paDetails = &mxdu;

   hr = mixerGetControlDetails((HMIXEROBJ)hMixer, &mxCDVolumeSpeakers, MIXER_GETCONTROLDETAILSF_VALUE);

   int v= (mxdu.dwValue*100.0)/0x10000;
   MasterVolBar->Position= 100-v;

   //////////////////////////////

   mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_MUTE;
   hr = mixerGetLineControls((HMIXEROBJ)hMixer, &mxlc, MIXER_GETLINECONTROLSF_ONEBYTYPE);


   mxCDMuteSpeakers.cMultipleItems = 0;
   mxCDMuteSpeakers.cChannels = 1; mxl.cChannels;
   mxCDMuteSpeakers.cbStruct = sizeof(mxCDVolumeSpeakers);
   mxCDMuteSpeakers.dwControlID = mc.dwControlID;
   mxCDMuteSpeakers.cbDetails = sizeof(mxdu);
   mxCDMuteSpeakers.paDetails = &mxdu;

   hr = mixerGetControlDetails((HMIXEROBJ)hMixer, &mxCDMuteSpeakers, MIXER_GETCONTROLDETAILSF_VALUE);

   MuteChkBx->Checked= mxdu.dwValue;

    */
}
//---------------------------------------------
void __fastcall TMonitorFrm::MasterVolBarChange(TObject *Sender)
{
   int vol= 0x10000*(100-MasterVolBar->Position)/100.00+0.5; // default
   mxCDVolumeSpeakers.cChannels = 1;
   mxdu.dwValue =   vol;
   mxCDVolumeSpeakers.paDetails = &mxdu;

   HRESULT hr = mixerSetControlDetails((HMIXEROBJ)hMixer, &mxCDVolumeSpeakers, MIXER_GETCONTROLDETAILSF_VALUE);

}
//---------------------------------------------------------------------------

void __fastcall TMonitorFrm::MuteChkBxClick(TObject *Sender)
{
   mxCDMuteSpeakers.cChannels = 1;
   mxdu.dwValue =   MuteChkBx->Checked;
   mxCDMuteSpeakers.paDetails = &mxdu;

   HRESULT hr = mixerSetControlDetails((HMIXEROBJ)hMixer, &mxCDMuteSpeakers, MIXER_GETCONTROLDETAILSF_VALUE);

}
//---------------------------------------------------------------------------

void __fastcall TMonitorFrm::FormDestroy(TObject *Sender)
{

  TIniFile *ini;
  AnsiString IniFileName=ExtractFilePath(Application->ExeName)+"otla.ini";
  ini = new TIniFile( IniFileName );
  ini->WriteInteger ( "otlaPlayer", "Audiodevice", AudioDevice);
  ini->WriteBool ( "otlaPlayer", "CloseOnFinished", CloseOnChkBx->Checked);
  delete ini;

  // Be sure to delete the buffer. 

  if (WaveData)
    delete[] WaveData;
    
  if (hMixer!=NULL)
       mixerClose (hMixer);
}
//---------------------------------------------
int   TMonitorFrm::OpenWaveFile(AnsiString Filename)
{
 // Declare the structures we'll need.
  MMCKINFO ChunkInfo;
  MMCKINFO FormatChunkInfo;
  MMCKINFO DataChunkInfo;

  Label4->Caption= Filename;
  // Zero out the ChunkInfo structure.
  memset(&ChunkInfo, 0, sizeof
	(MMCKINFO));

  // Open the file.
  HMMIO handle = mmioOpen(
  Filename.c_str(), 0,
	  MMIO_READ);
  if (!handle) {
    MessageBox(0,
     "Error opening file.",
	"Error Message", 0);
    return -1;
  }

  // Find the RIFF chunk.
  DWORD Res = mmioDescend(handle, &ChunkInfo, 0, MMIO_FINDRIFF);
  CheckMMIOError(Res);

  // Descend into the format chunk.
  FormatChunkInfo.ckid = mmioStringToFOURCC("fmt", 0);
  Res = mmioDescend(handle, &FormatChunkInfo, &ChunkInfo, MMIO_FINDCHUNK);
  CheckMMIOError(Res);

  // Read the wave format.
  memset(&WaveFmt, 0, sizeof(WAVEFORMATEX));

  // mmioRead and mmioWrite return the number of
  // bytes read or written so don't call the
  // CheckMMIOError fucntion for those.
  mmioRead(handle, (char*)&WaveFmt, FormatChunkInfo.cksize);

  // Ascend out of the format chunk.
  Res = mmioAscend(handle, &FormatChunkInfo, 0);
  CheckMMIOError(Res);

  // Descend into the data chunk.
  DataChunkInfo.ckid = mmioStringToFOURCC("data", 0);
  Res = mmioDescend(handle, &DataChunkInfo, &ChunkInfo,	MMIO_FINDCHUNK);
  CheckMMIOError(Res);

  // Read the data into a buffer.
  DataSize = DataChunkInfo.cksize;
  if (WaveData)
    delete[] WaveData;
  WaveData = new char[DataSize+400];
  mmioRead(handle, WaveData, DataSize);

  // Close the file
  mmioClose(handle, 0);

 // Memo1->Lines->Add(String	(DataSize) +     " bytes of wave data loaded 	from file.");
  PlayBtn->Enabled = true;

  if (WaveFmt.nBlockAlign)
      ProgressBar1->Max= DataSize / WaveFmt.nBlockAlign;
  else
      ProgressBar1->Max= 0;

  AnsiString saux;
  saux=saux.sprintf("%i Hz %i bit %s",(int)WaveFmt.nSamplesPerSec,
                             (int)WaveFmt.wBitsPerSample,
                             WaveFmt.nChannels==1?"mono":"stereo") ;
  
  Label5->Caption= saux;

  return 0;

}
void __fastcall TMonitorFrm::OpenBtnClick(TObject *Sender)
{
  // Show the Open Dialog.
  if (!OpenDialog1->Execute()) return;

  OpenWaveFile ( OpenDialog1->FileName );
}
//---------------------------------------------
void TMonitorFrm::CheckMMIOError (DWORD code)
{
  // Report an mmio error, if 	one occurred.
  if (code == 0) return;
  char buff[256];
  wsprintf(buff, 
    "MMIO Error. Error Code: %d", code);
  Application-> MessageBox(buff, "MMIO Error", 0);
}
//---------------------------------------------

void __fastcall TMonitorFrm::PlayBtnClick(TObject *Sender)
{
  // Query the device and see if it can play
  // this wave format. If so, open the device.
  if (WaveHandle)
  {
     if (Paused)
       waveOutRestart (WaveHandle);
    else
       waveOutPause (WaveHandle);
    Paused=!Paused;
  }
  else
  {

  int Res = waveOutOpen(&WaveHandle, AudioDevice, &WaveFmt,  0, 0, WAVE_FORMAT_QUERY);
  CheckWaveError(Res);
  if (Res) return;
  Res = waveOutOpen(&WaveHandle, AudioDevice , &WaveFmt,
    MAKELONG(Handle, 0), 0, CALLBACK_WINDOW);
  CheckWaveError(Res);
  
  Res = waveOutSetVolume(WaveHandle , MAKELONG(0xFFFF, 0xFFFF));
  CheckWaveError(Res);
  
  // Set up the wave header.
  memset(&WaveHeader, 0, sizeof(WaveHeader));
  WaveHeader.lpData = WaveData;
  WaveHeader.dwBufferLength = DataSize;
                               /*
  // If the LoopEdit contains a value greater
  // than 1 then set the loop count and flags.
  int loops = LoopEdit->Text.ToIntDef(0);
  if (loops > 1) {
    WaveHeader.dwLoops = loops;
    WaveHeader.dwFlags = 
      WHDR_BEGINLOOP | WHDR_ENDLOOP;
  }    */

  // Prepare the wave header.
  Res = waveOutPrepareHeader(WaveHandle, &WaveHeader, sizeof(WAVEHDR));
  CheckWaveError(Res);

  // Start playback.
  Res = waveOutWrite(WaveHandle, &WaveHeader, sizeof(WAVEHDR));
  CheckWaveError(Res);
//  Memo1->Lines->Add("Playback started...");
  Timer1->Enabled=true;
  Paused=false;
  }
  TarjetasCmbBx->Enabled=false;
  OpenBtn->Enabled=false;
  if (Paused)
      PlayBtn->Caption="Play";
  else      
      PlayBtn->Caption="Pause";
}
//---------------------------------------------

void TMonitorFrm::CheckWaveError (DWORD code)
{
  // Report a wave out error, if one occurred.
  char buff[256];
  if (code == 0) return;

  waveOutGetErrorText(code, buff, sizeof(buff));
  MessageBox(Handle, buff, "Wave Error", MB_OK);
}
//---------------------------------------------

void TMonitorFrm::OnWaveDone (TMessage& msg)
{
  // We only care about the WOM_DONE message.
  // When we get this message we know that the
  // sound has finished playing. We can then
  // unprepare the header and close the device.

  if (msg.Msg == WOM_DONE)
  {
    Timer1->Enabled=false;
   // Memo1->Lines->Add("Playback finished.");
    StopBtnClick(NULL);
    Timer1->Enabled=false;       
    TarjetasCmbBx->Enabled=true;
    OpenBtn->Enabled=true;
    if (CloseOnChkBx->Checked )
      Close();
  }
  if (msg.Msg == WOM_OPEN)
  {
     ProgressBar1->Visible=true;
  }
  if (msg.Msg == WOM_CLOSE)
  {
     ProgressBar1->Visible=false;
  }
}
//---------------------------------------------


void __fastcall TMonitorFrm::Timer1Timer(TObject *Sender)
{
  ActualizaVolumen ();

  MMTIME mt;
  mt.wType=TIME_MS;
  float tf;
  waveOutGetPosition(WaveHandle, &mt, sizeof(mt));

  ProgressBar1->Position=mt.u.sample;

  if (mt.wType==TIME_MS)
    tf=mt.u.ms/1000.0;
  else
     // if (mt.wType==TIME_BYTES)
    tf=mt.u.sample/44100.0;

  AnsiString saux;
  saux.printf("Time :%.1f",tf) ;
  Label1->Caption= saux;

  DWORD aux=0;
  waveOutGetPitch(WaveHandle, &aux);
 // Label2->Caption=(double )aux;
  waveOutGetPlaybackRate(WaveHandle, &aux);
 // Label3->Caption=(double )aux;    
  
  Image1->Canvas->FillRect(Rect(0,0,Image1->Width,Image1->Height));
  Image1->Canvas->MoveTo (0,33);
  if (WaveFmt.nBlockAlign==1)
    for (int x=0;x<Image1->Width/3;x++)
    {
    Image1->Canvas->LineTo (x*3-3,2+(0x100-WaveData[mt.u.sample+x])/4);
      Image1->Canvas->LineTo (x*3,2+(0x100-WaveData[mt.u.sample+x])/4);
    }
  else    
    for (int x=0;x<Image1->Width/3;x++)
    {
    Image1->Canvas->LineTo (x*3-3,2+(0x80+(char)WaveData[1+(mt.u.sample+x)*WaveFmt.nBlockAlign])/4);
      Image1->Canvas->LineTo (x*3,2+(0x80+(char)WaveData[1+(mt.u.sample+x)*WaveFmt.nBlockAlign])/4);
    }

}
//---------------------------------------------------------------------------

void __fastcall TMonitorFrm::StopBtnClick(TObject *Sender)
{
   if (WaveHandle)
   {
    MMRESULT res= waveOutReset(  WaveHandle   );
    int Res = waveOutUnprepareHeader(WaveHandle, &WaveHeader, sizeof(WAVEHDR));
    CheckWaveError(Res);
    Res = waveOutClose(WaveHandle);
    CheckWaveError(Res);       
    WaveHandle=0;
   } 
    TarjetasCmbBx->Enabled=true;
    OpenBtn->Enabled=true;
    PlayBtn->Caption="Play";
    Paused=true; 
    Timer1->Enabled=false;
    ProgressBar1->Position=0;
    Label1->Caption= "Time:";
}
//---------------------------------------------------------------------------


void __fastcall TMonitorFrm::CloseBtnClick(TObject *Sender)
{
      StopBtnClick (NULL);
      Close ();
}
//---------------------------------------------------------------------------




void __fastcall TMonitorFrm::FormClose(TObject *Sender,
      TCloseAction &Action)
{           

  StopBtnClick(NULL);

}
//---------------------------------------------------------------------------

