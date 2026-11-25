//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <vector>
#include "TMapWin.h"
#include "MapWinGraf.h"
#include "elementer1.h"
#include "borpunkter.h"
#include "acadentiteter.h"






//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
void CEntiteter::clear()
{
  CEntitet *pEntitet = 0;
  CEntitetIterator i;
  for(i=begin(); i != end(); i++)
  {
    pEntitet = *i;
    delete pEntitet;
  }//end for
  kont.clear();
}
//---------------------------------------------------------------------------
/*
void CEntiteter::Paint(TMapWinGraf &MWG) {
  MWG.clear();
  //tegner vertikale linjer:
  TColor temp = MWG.color;
  MWG.color = clGray;
  MWG.VLine(0);
  MWG.color = temp;

  CEntitet *pEntitet = 0;
  CEntitetIterator i;
  for(i=begin(); i != end(); i++)
  {
    pEntitet = *i;
    if(LagErPaslatt[pEntitet->Lag]) {
      pEntitet->Paint(MWG);
    }
  }//end for
  //saa tegnes det inn akser med paskrift:
  double dx = MWG.dx;
  double dy = MWG.dy;
  double stasjavstX;
  if (fabs(dx)>1000) stasjavstX = 1000;
  else if (fabs(dx)>100) stasjavstX = 100;
  else if (fabs(dx)>10)  stasjavstX = 10;
  else if (fabs(dx)>1)   stasjavstX = 1;
  else stasjavstX = 0.1;
  double stasjavstY;
  if (fabs(dy)>1000) stasjavstY = 1000;
  else if (fabs(dy)>500) stasjavstY = 100;
  else if (fabs(dy)>200) stasjavstY = 50;
  else if (fabs(dy)>50)  stasjavstY = 10;
  else if (fabs(dy)>20)  stasjavstY = 5;
  else if (fabs(dy)>10)  stasjavstY = 2;
  else if (fabs(dy)>5)   stasjavstY = 1;
  else if (fabs(dy)>2)   stasjavstY = 1;
  else if (fabs(dy)>1)   stasjavstY = 0.2;
  else if (fabs(dy)>0.5) stasjavstY = 0.1;
  else stasjavstY = 0.1;
  MWG.putxakse(MWG.xlow + 0.05*dx, MWG.ylow + 0.05*dy, dx, stasjavstX, 0.05*dy, true);
  MWG.putyakse(MWG.xlow, MWG.ylow, dy, stasjavstY, 0.01*dx, true);
}
*/
//---------------------------------------------------------------------------

void CEntiteter::Paint(TMapWin &MW) {
  MW.clear();
  CEntitet *pEntitet = 0;
  CEntitetIterator i;
  for(i=begin(); i != end(); i++)
  {
    pEntitet = *i;
    if(LagErPaslatt[pEntitet->Lag]) {
      pEntitet->Paint(MW);
    }
  }//end for
}
//---------------------------------------------------------------------------


void CEntiteter::FinnXYMaxMin(double &XMax, double &YMax, double &XMin, double &YMin) {
  CEntitet *pEntitet = 0;
  CEntitetIterator i;
  double XMax1, YMax1, XMin1, YMin1;   //mellomlager for hver entitet
  XMax1 = YMax1 = -1000000;
  XMin1 = YMin1 =  1000000;
  for(i=begin(); i != end(); i++)
  {
    pEntitet = *i;
    if(LagErPaslatt[pEntitet->Lag]) {
      pEntitet->FinnXYMaxMin(XMax1,YMax1,XMin1,YMin1);
      XMax = max(XMax, XMax1);
      YMax = max(YMax, YMax1);
      XMin = min(XMin, XMin1);
      YMin = min(YMin, YMin1);
    }
  }//end for
}
//---------------------------------------------------------------------------
void CEntiteter::SetViewMaxMin(TMapWinGraf &MWG) {
  MWG.clear();
  double xlow  =  1000000;
  double ylow  =  1000000;
  double xhigh = -1000000;
  double yhigh = -1000000;
  FinnXYMaxMin(xhigh, yhigh, xlow, ylow);
  double dx = xhigh-xlow;
  double dy = yhigh-ylow;
  dy = (dy > 0.01 ? dy : 0.01);
  MWG.xlow = (xhigh+xlow)*0.5 - dx*0.6;
  MWG.xhigh =(xhigh+xlow)*0.5 + dx*0.6;
  MWG.ylow = (yhigh+ylow)*0.5 - dy*0.6;
  MWG.yhigh =(yhigh+ylow)*0.5 + dy*0.6;
  MWG.fXScrollBarStart = MWG.xlow;
  MWG.fXScrollBarSlutt = MWG.xhigh;
  MWG.fYScrollBarStart = MWG.ylow;
  MWG.fYScrollBarSlutt = MWG.yhigh;

  MWG.ScrollX(MWG.XMidtPros);
  MWG.ScrollY(MWG.YMidtPros);
}
//---------------------------------------------------------------------------
  CLinje *CEntiteter::AdLinje(CPunkt Fra, CPunkt Til, int lag, TColor farge, int linjetype, int linjetypeskala) {
  CLinje *L = new CLinje(Fra, Til, lag, farge,linjetype,linjetypeskala);
  push_back(L);
  return L;
}
//---------------------------------------------------------------------------
CText *CEntiteter::AdText(AnsiString tekst, double hoyde, CPunkt Startpkt, int lag, TColor farge, int linjetype, int linjetypeskala) {
  CText *T = new CText(tekst, hoyde, Startpkt, lag, farge,linjetype,linjetypeskala);
  push_back(T);
  return T;
}
//---------------------------------------------------------------------------
CPolylinje *CEntiteter::AdPolylinje(int lag, TColor farge, int linjetype, int linjetypeskala, CVertex *pVertex = NULL, int AntVertex = 0) {
  CPolylinje *P = new CPolylinje(lag, farge, linjetype, linjetypeskala, pVertex, AntVertex);
  push_back(P);
  return P;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void CEntPunkt::Paint(TMapWin &MW) {
  MW.putpixel(X,Y);
}
//---------------------------------------------------------------------------
void CLinje::Paint(TMapWin &MW) {
  MW.putline(StartX,StartY,EndX,EndY);
}
//---------------------------------------------------------------------------
void CText::Paint(TMapWin &MW) {
  MW.puttext(X,Y,Tekst,Hoyde);
}
//---------------------------------------------------------------------------
void CPolylinje::Paint(TMapWin &MW) {
  CVertex Forige = *kont.begin();
  CVertex Forste = Forige;
  CVertex Dette;
  for(CVertexIterator i = kont.begin()+1; i != kont.end(); i++) {
      Dette = *i;
      MW.putline(Forige.X, Forige.Y, Dette.X, Dette.Y);
      Forige = Dette;
    }//end for
    if(Lukket) {MW.putline(Dette.X, Dette.Y, Forste.X, Forste.Y);}
}
//---------------------------------------------------------------------------
void CPolylinje::FinnXYMaxMin(double &XMax, double &YMax, double &XMin, double &YMin) {
  for(CVertexIterator i = kont.begin(); i != kont.end(); i++) {
      XMax = max(XMax, i->X);
      YMax = max(YMax, i->Y);
      XMin = min(XMin, i->X);
      YMin = min(YMin, i->Y);
    }//end for
}
//---------------------------------------------------------------------------
CPolylinje::CPolylinje(int lag, TColor farge, int linjetype, int linjetypeskala,CVertex *pVertex, int AntVertex, bool lukket) {
  Lag = lag;
  Farge = farge;
  Linjetype = linjetype;
  Linjetypeskala = linjetypeskala;
  Lukket = lukket;
  if ( (pVertex != NULL)&&(AntVertex > 0) )
  {
    CVertex *p;
    for (p=pVertex; p != pVertex+AntVertex; p++)
      push_back(*p);
  }//end if
}
//---------------------------------------------------------------------------
void CDreietrykkopptegningEntiteter::Oppdater() {
  switch (ValgtDiagram)
  {
    case 0:   //"Standard" valgt :
      OppdaterMedStandarddiagram();
    break;
    case 1:   //"Standarddiagram med tekstete koder" valgt.  Den har jeg ikke skrevet ferdig enda.  Derfor skjer forelopig ingenting her.
      ;
    default:
      OppdaterMedStandarddiagram();
  }//end switch
}
//---------------------------------------------------------------------------
void CDreietrykkopptegningEntiteter::OppdaterMedStandarddiagram() {
  if (!DataKilde) return;
  clear();
  double Bordybde = DataKilde->GiBoretDybde();
  double HorLinjeavst = 250;
  double Text1Hoyde = 16; // 0 5 10 20 30 og kommentarkoder
  double Text2Hoyde = 22; //FDT KN
  double DimBorsymbol = 60;
  double OfsetBorsymbol = 110;
  double DimXXX = 10;
  double OfsetText1 = 65;
  double OfsetText2 = 100;
  double OfsetKomentarkode1 = 20; //kommentarkoden narmest
  double OfsetKomentarkode2 = 220;
  CPunkt ToppBorstolpe(0,0);  //koordinat
  CPunkt BunnBorstolpe(0,0 + Bordybde);
  CPunkt Fra;
  CPunkt Til;

  //tegner diagrammet til kontaineren:
  AdLinje(ToppBorstolpe, BunnBorstolpe, 1,clWhite,1,1);

  Fra = ToppBorstolpe.Ofset(HorLinjeavst, 0);
  Til = BunnBorstolpe.Ofset(HorLinjeavst, 0);
  AdLinje(Fra, Til, 1, clWhite,1,1);

  Fra = ToppBorstolpe.Ofset(2 * HorLinjeavst, 0);
  Til = BunnBorstolpe.Ofset(2 * HorLinjeavst, 0);
  AdLinje(Fra, Til, 1, clWhite,1,1);

  Fra = ToppBorstolpe.Ofset(3 * HorLinjeavst, 0);
  Til = BunnBorstolpe.Ofset(3 * HorLinjeavst, 0);
  AdLinje(Fra, Til, 1, clWhite,1,1);

  Fra = ToppBorstolpe.Ofset(4 * HorLinjeavst, 0);
  Til = BunnBorstolpe.Ofset(4 * HorLinjeavst, 0);
  AdLinje(Fra, Til, 1, clWhite,1,1);

  //og en bunnlinje:
  Fra = BunnBorstolpe.Ofset(-HorLinjeavst*0.1, 0);
  AdLinje(Fra, Til, 1, clWhite,1,1);

  Fra = BunnBorstolpe.Ofset(-Text1Hoyde, OfsetText1/100 + Text1Hoyde/100);
  AdText("0", Text1Hoyde, Fra, 1, clWhite,1,1);

  Fra = Fra.Ofset(HorLinjeavst, 0);
  AdText("5", Text1Hoyde, Fra, 1, clWhite,1,1);

  Fra = Fra.Ofset(HorLinjeavst, 0);
  AdText("10", Text1Hoyde, Fra, 1, clWhite,1,1);

  Fra = Fra.Ofset(HorLinjeavst, 0);
  AdText("20", Text1Hoyde, Fra, 1, clWhite,1,1);

  Fra = Fra.Ofset(HorLinjeavst, 0);
  AdText("30", Text1Hoyde, Fra, 1, clWhite,1,1);

  Fra = BunnBorstolpe.Ofset(HorLinjeavst, OfsetText1/100 + Text1Hoyde/100 + OfsetText2/100 + Text2Hoyde/100);
  AdText("FDT kN", Text2Hoyde, Fra, 1, clWhite,1,1);

  //hvis stoppkode 93 tegnes XXX under borstolpen:
  if (DataKilde->GiSluttkode() == 93)
  {
    Fra = BunnBorstolpe.Ofset(-3*DimXXX, 0);
    AdText("XXX", DimXXX, Fra, 1, clWhite,1,1);
  }


  //tegner polylinje for selve sonderingsdataene:
  CPolylinje *p;
  double Dybde;
  double FDT;
  double MappetFDT;

  p = AdPolylinje(1, clWhite, 1, 1);

  vector<CDreietrykkdata>::iterator i;
  for (i = DataKilde->begin(); i != DataKilde->end(); ++i)
  {
    Dybde = (*i).Dybde;
    FDT = (*i).Matekraft;
    MappetFDT = (FDT < 5 ? FDT*HorLinjeavst/5 : HorLinjeavst + (FDT-5)*HorLinjeavst/10);
    p->push_back(CVertex(MappetFDT, Dybde));
  }//end for
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
