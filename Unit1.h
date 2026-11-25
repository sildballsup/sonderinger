//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu1;
        TMenuItem *File1;
        TMenuItem *Exit1;
        TMenuItem *N1;
        TMenuItem *PrintSetup1;
        TMenuItem *Print1;
        TMenuItem *N2;
        TMenuItem *SaveAs1;
        TMenuItem *Save1;
        TMenuItem *Open1;
        TMenuItem *New1;
        TOpenDialog *OpenDialog1;
        TStringGrid *StringGrid1;
        void __fastcall Open1Click(TObject *Sender);
        void __fastcall StringGrid1DblClick(TObject *Sender);
        void __fastcall Exit1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
        AnsiString Filnavn1;
        CBoringer Base;
        CBoring *CurBoring;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
 