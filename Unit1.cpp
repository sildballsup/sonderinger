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
#include "Unit1.h"
#include "Unit2.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Open1Click(TObject *Sender)
{
  if(OpenDialog1->Execute()) {
    Filnavn1 = OpenDialog1->FileName;
  }
  ifstream fil(Filnavn1.c_str(),ios::nocreate,0);
    if (!fil) {
    return;
  }
  Base.clear();
  fil>>Base;
  Base.FyllStringGrid(Form1->StringGrid1);
  StringGrid1->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::StringGrid1DblClick(TObject *Sender)
{
//klikk i headerlinje gir sortering av linjene på denne kolonnen
  if ((StringGrid1->Row)==0)
  {
    switch (StringGrid1->Col)
    {

      case 0: Base.SortEtterBortype();
              Base.FyllStringGrid(Form1->StringGrid1);
              break;
      case 1: Base.SortEtterBorpktNr();
              Base.FyllStringGrid(Form1->StringGrid1);
              break;
      case 2: Base.SortEtterLopenr();
              Base.FyllStringGrid(Form1->StringGrid1);
              break;
      case 3: Base.SortEtterProfil();
              Base.FyllStringGrid(Form1->StringGrid1);
              break;
      case 4: Base.SortEtterAvsett();
              Base.FyllStringGrid(Form1->StringGrid1);
              break;
      case 5: Base.SortEtterX();
              Base.FyllStringGrid(Form1->StringGrid1);
              break;
      case 6: Base.SortEtterY();
              Base.FyllStringGrid(Form1->StringGrid1);
              break;
      case 7: Base.SortEtterZ();
              Base.FyllStringGrid(Form1->StringGrid1);
              break;
    }//end switch
  }//end if

  else
  {
//setter cur boring og tegner opp form2
  CurBoring = Base.kont[StringGrid1->Row - 1];
  if (Form2->Tegning1 != NULL) delete Form2->Tegning1;  //sletter gammel tegning

  //skriver inn headerdata i form2:
  Form2->Edit1->Text = FloatToStrF(CurBoring->X,ffFixed,15,3);
  Form2->Edit2->Text = FloatToStrF(CurBoring->Y,ffFixed,15,3);
  Form2->Edit3->Text = FloatToStrF(CurBoring->Z,ffFixed,15,3);
  Form2->Edit4->Text = FloatToStrF(CurBoring->Profil,ffFixed,15,3);
  Form2->Edit5->Text = FloatToStrF(CurBoring->Avsett,ffFixed,15,3);
  Form2->Edit6->Text = FloatToStrF(CurBoring->Forboringsdybde,ffFixed,15,3);
  Form2->Edit7->Text = CurBoring->Prosjektnr;
  Form2->Edit8->Text = CurBoring->Oppdragsnr;
  Form2->ComboBox3->Text = CurBoring->GiSluttkode();

  switch (CurBoring->Bortype)
  {
    case Dreietrykksondering: Form2->Tegning1 = new CDreietrykkopptegningEntiteter( (CDreietrykksondering*)CurBoring  );

    break;
    case Totalsondering: Form2->Tegning1 = new CTotalsonderingopptegningEntiteter( (CTotalsondering*)CurBoring  );
    break;
    default: ;
  }//end switch

  Form2->Tegning1->FyllDiagramAltTilCombobox(Form2->ComboBox1);
  Form2->Tegning1->Oppdater();
  Form2->Tegning1->Paint(*Form2->MapWin1);
  Form2->Show();
  CurBoring->FyllStringGrid(Form2->StringGrid1);
  }//end else
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Exit1Click(TObject *Sender)
{
  Close();        
}
//---------------------------------------------------------------------------











