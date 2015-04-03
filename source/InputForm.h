//----------------------------------------------------------------------------
#ifndef InputFormH
#define InputFormH
//----------------------------------------------------------------------------
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Classes.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Windows.hpp>
#include <vcl\System.hpp>
#include <CheckLst.hpp>
#include <ComCtrls.hpp>

#include "sbb.h"
#include <Menus.hpp>
#include <ImgList.hpp>
//----------------------------------------------------------------------------

class TInputBlocksDlg : public TForm
{
__published:
	TButton *OKBtn;
	TButton *CancelBtn;
        TListView *ListView1;
        TMemo *Memo1;
        TLabel *ClearLbl;
        TLabel *UsrLbl;
        TEdit *ClearEdt;
        TComboBox *UsrCmbBx;
        TPopupMenu *PopupMenu1;
        TMenuItem *Modify1;
        TMenuItem *Seedata1;
        TMenuItem *ConverBasic1;
        TImageList *ImageList1;
        TMenuItem *Restore1;
        TLabel *Label1;
    TMenuItem *Viewdecimalhexadecimal1;
        void __fastcall OKBtnClick(TObject *Sender);
        void __fastcall Modify1Click(TObject *Sender);
        void __fastcall Seedata1Click(TObject *Sender);
        void __fastcall PopupMenu1Popup(TObject *Sender);
        void __fastcall ConverBasic1Click(TObject *Sender);
        void __fastcall Restore1Click(TObject *Sender);
    void __fastcall Viewdecimalhexadecimal1Click(TObject *Sender);

private:
	     void  __fastcall filldata();
        int __fastcall ROMMSX (const char * file);
        ts_sbb_block blocks_sbb[16];
public:
        ts_sbb_header  head_sbb;
        ts_sbb_block   *res_blocks_sbb ;
	virtual __fastcall TInputBlocksDlg(TComponent* AOwner);
	int   __fastcall Abre(AnsiString Fichero);
	int   __fastcall Edit(ts_sbb_header  *in_head_sbb ,ts_sbb_block   *in_blocks_sbb);
        bool enheaxadecimal;
};
//----------------------------------------------------------------------------
extern PACKAGE TInputBlocksDlg *InputBlocksDlg;
//----------------------------------------------------------------------------
#endif    
