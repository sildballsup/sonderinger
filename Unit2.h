//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Tabnotbk.hpp>
#include <Grids.hpp>
#include "TMapWin.h"
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
        TTabbedNotebook *TabbedNotebook1;
        TLabel *Label1;
        TComboBox *ComboBox1;
        TPanel *Panel1;
        TButton *Button1;
        TButton *Button2;
        TButton *Button3;
        TButton *Button4;
        TButton *Button5;
        TImage *Image1;
        TGroupBox *GroupBox1;
        TGroupBox *GroupBox2;
        TGroupBox *GroupBox3;
        TGroupBox *GroupBox4;
        TEdit *Edit1;
        TEdit *Edit2;
        TEdit *Edit3;
        TButton *Button6;
        TComboBox *ComboBox2;
        TLabel *Label2;
        TEdit *Edit4;
        TEdit *Edit5;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label8;
        TEdit *Edit7;
        TLabel *Label9;
        TLabel *Label10;
        TStringGrid *StringGrid1;
        TComboBox *ComboBox3;
        TEdit *Edit8;
        TLabel *Label11;
        TMapWin *MapWin1;
        TScrollBar *ScrollBar1;
        TLabel *Label12;
        TEdit *Edit6;
        TButton *Button7;
        TButton *Button8;
        TLabel *Label5;
        TButton *Button9;
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall ScrollBar1Change(TObject *Sender);
        void __fastcall ComboBox1Change(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm2(TComponent* Owner);
        CBoring *CurBoring;
        void Oppdater();
        CBorprofilEntiteter *Tegning1;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
