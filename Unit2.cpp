//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <math.h>
using namespace std;

#include "TMapWin.h"
#include "MapWinGraf.h"
#include "elementer1.h"


#include "borpunkter.h"
#include "acadentiteter.h"
#include "Unit2.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TMapWin"
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button2Click(TObject *Sender)
{
  Form2->Hide();
}
//---------------------------------------------------------------------------
void TForm2::Oppdater()        //data for CurBoring skrives inn i formen:
{
  Edit1->Text = FloatToStrF(CurBoring->X,ffFixed,15,3);
  Edit2->Text = FloatToStrF(CurBoring->Y,ffFixed,15,3);
  Edit3->Text = FloatToStrF(CurBoring->Z,ffFixed,15,3);
  Edit4->Text = FloatToStrF(CurBoring->Profil,ffFixed,15,3);
  Edit5->Text = FloatToStrF(CurBoring->Avsett,ffFixed,15,3);
  Edit6->Text = FloatToStrF(CurBoring->Forboringsdybde,ffFixed,15,3);
  Edit7->Text = CurBoring->Prosjektnr;
  Edit8->Text = CurBoring->Oppdragsnr;


  CurBoring->FyllStringGrid(StringGrid1);
}
//---------------------------------------------------------------------------

void __fastcall TForm2::ScrollBar1Change(TObject *Sender)
{
  MapWin1->yhigh = -1 + ScrollBar1->Position;
  MapWin1->ylow = 25 + ScrollBar1->Position;
  Tegning1->Paint(*MapWin1);
}
//---------------------------------------------------------------------------


void __fastcall TForm2::ComboBox1Change(TObject *Sender)
{
  Tegning1->SetValgtDiagram(Standard); //((enDtDiagram)ComboBox1->ItemIndex);         //maa caste her, ikke saa fint men..
  Tegning1->Oppdater();
  Tegning1->Paint(*MapWin1);
}
//---------------------------------------------------------------------------

