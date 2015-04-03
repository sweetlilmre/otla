//---------------------------------------------------------------------------

#ifndef ProressFormH
#define ProressFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>

//---------------------------------------------------------------------------
class TProgresoFrm : public TForm
{
__published:	// IDE-managed Components
        TButton *Button1;
        TProgressBar *ProgressBar1;
        TListView *FilesLV;
        TLabel *Label1;
        TLabel *InfoLbl;
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:	    // User declarations
        __fastcall TProgresoFrm(TComponent* Owner);
        void __fastcall Proces(TStrings *files,AnsiString out);
        void  __fastcall ProcesHI ();
        void  __fastcall ProcesNORMAL ();
        AnsiString OutputDir;
        AnsiString Lame;
        AnsiString  extra_info;
        bool mp3;
};
//---------------------------------------------------------------------------
extern PACKAGE TProgresoFrm *ProgresoFrm;
//---------------------------------------------------------------------------
#endif
