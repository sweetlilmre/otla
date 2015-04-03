//---------------------------------------------------------------------------

#ifndef DirFormH
#define DirFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <FileCtrl.hpp>
//---------------------------------------------------------------------------
class TDirFrm : public TForm
{
__published:	// IDE-managed Components
        TDriveComboBox *DriveComboBox1;
        TDirectoryListBox *DirectoryListBox1;
        TButton *AceptarBtn;
        TButton *CancelBtn;
        void __fastcall CancelBtnClick(TObject *Sender);
        void __fastcall AceptarBtnClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TDirFrm(TComponent* Owner);
        void __fastcall DirectorioInicial(AnsiString  Directorio);

};
//---------------------------------------------------------------------------
extern PACKAGE TDirFrm *DirFrm;
//---------------------------------------------------------------------------
#endif
