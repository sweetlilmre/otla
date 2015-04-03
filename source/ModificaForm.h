//----------------------------------------------------------------------------
#ifndef ModificaFormH
#define ModificaFormH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
//----------------------------------------------------------------------------
class TModificaDlg : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
        TLabel *Label1;
        TEdit *NameEdt;
        TEdit *StartEdt;
        TLabel *Label2;
        TEdit *SizeEdt;
        TLabel *Label3;
        TEdit *ExecEdt;
        TLabel *Label4;
        TMemo *Memo1;
private:
public:
	virtual __fastcall TModificaDlg(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TModificaDlg *ModificaDlg;
//----------------------------------------------------------------------------
#endif    
