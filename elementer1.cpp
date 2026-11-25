//---------------------------------------------------------------------------
#include <iostream>
#include <vcl.h>
#pragma hdrstop
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <math.h>

#include "elementer1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------


// CGeometriFormler
//---------------------------------------------------------------------------
long double CGeometriFormler::Avstand(CPunkt P1, CPunkt P2)
{
  long double resultat = sqrtl(powl((P2.x-P1.x),2) + powl((P2.y-P1.y),2) );
  return resultat;
}
//---------------------------------------------------------------------------
long double CGeometriFormler::Retnv(CPunkt P1, CPunkt P2)         //retning 0 nordover vinkel øker med rot. med
{                                                            //klokka. Dvs orientering av koordinatsystem som
  long double dx = P2.x - P1.x;                                   //i landmåling. x-aksen mot nord, y-aksen mot øst.
  long double dy = P2.y - P1.y;                                   //returnerer vinkel i området [0-2*pi>.
  if (Avstand(P1,P2) == 0)                                   //Identiske punkter gir retningsvinkel=0.
  {
    return 0;           //returnerer pga bare ett pkt
  }
  long double alfa = asinl(dy/Avstand(P1,P2));

  if ( (dx >= 0)&&(dy >= 0) )     //er da i 1. kvadrant
  {
    return alfa;
  }
  else if (dx < 0)      // er da i 2. eller 3. kvadrant
  {
    return (M_PI - alfa);  // er da i 4. kvadrant
  }
  else return (2*M_PI + alfa);
}
//---------------------------------------------------------------------------
long double CGeometriFormler::Vinkel(CPunkt P1, CPunkt P2, CPunkt P3)
{
  long double v = Retnv(P2,P3) - Retnv(P2,P1);                  //Beregner vinkelen med toppunkt i P2 og med
  if (v >= 0)                                              //dreining fra P2-P1 til P2-P3.
  {                                                        //Dvs vinkel i området [0 - 2*pi>
    return v;
  }
  else
  {
    return v + 2*M_PI;
  }
}
//---------------------------------------------------------------------------
long double CGeometriFormler::AbsVinkel(CPunkt P1, CPunkt P2, CPunkt P3)   //som Vinkel men her returneres absoluttverdien av
{                                                                     //vinkelen.
  long double v = fabsl(Retnv(P2,P3) - Retnv(P2,P1));
  if (v >= M_PI) {
    return v - M_PI;
  }
  return v;
}
//---------------------------------------------------------------------------
long double CGeometriFormler::AbsVinkel(long double v1, long double v2)               //returnerer vinkelen mel to retningsvinkler i absoluttverdi.
{                                                                      //forutsetter v1 0g v2 pos og i området [0 2pi>
  long double v = v2 - v1;
  if (v < 0) {
    v = v + 2*M_PI;
  }
  if (v > M_PI) {
    v = 2*M_PI - v;
  }
  return v;
}
//---------------------------------------------------------------------------
long double CGeometriFormler::Vinkel(long double v1, long double v2)                  //returnerer vinkel mel to retningsvinkler, dreining fra v1 til v2.
{
  long double v = v2 - v1;
  if (v >= 0)                                              //dreining fra P2-P1 til P2-P3.
  {                                                        //Dvs vinkel i området [0 - 2*pi>
    return v;
  }
  else
  {
    return v + 2*M_PI;
  }
}
//---------------------------------------------------------------------------

CPunkt CGeometriFormler::Polar(CPunkt Anker, long double Avst, long double Retning)
{                                                          //Koordinatsystem som i landmåling. Dvs
  long double dx = Avst * cosl(Retning);                         //Retning 0 nordover og pos vinkeldreinging med sola. Og
  long double dy = Avst * sinl(Retning);                         //x-aksen mot nord, y-aksen mot øst.
  CPunkt NyttPunkt;
  NyttPunkt.x = Anker.x + dx;
  NyttPunkt.y = Anker.y + dy;
  return NyttPunkt;
}
//---------------------------------------------------------------------------
long double CGeometriFormler::TransfRetnv(long double retnv, long double retnvnyX)
{                                                           //finner retningsvinkel etter at x aksen er rotert til
 long double v = retnv - retnvnyX;                               //retning retnvnyX
  if (v >= 0)
  {
    return v;
  }
  else
  {
    return v + 2*M_PI;
  }
}
//---------------------------------------------------------------------------
// CElement
//---------------------------------------------------------------------------
double CElement::Vinkelendring()             //beregner absoluttverdien av vinkelendringen, dvs aldri negativ verdi
{                                            //vinkelendring fra Beg til Sl
  double result;                             //forutsetter retningsvinkler i første omløp
  if (Beg.Radius >0)
  {
    result = Sl.Retning - Beg.Retning;
  }
  else
  {
    result = Beg.Retning - Sl.Retning;
  }

  if (result < 0)
    {
      result = 2*M_PI + result;  //pga retnvinkelen paserer 0
    }
  return result;
}
//---------------------------------------------------------------------------
CElement::CElement(int IElnr, double IBegPr, double IRBeg, double IRSl, double IAKlot,
           double IXBeg, double IYBeg, double IXSl, double IYSl, double IPrSl, enElementtype IElementtype)
{
  Elnr = IElnr;
  Beg.Profilnr = IBegPr;
  Beg.Radius = IRBeg;
  Sl.Radius = IRSl;
  AKlot = IAKlot;
  Beg.x = IXBeg;
  Beg.y = IYBeg;
  Sl.x = IXSl;
  Sl.y = IYSl;
  Sl.Profilnr = IPrSl;
  Elementtype = IElementtype;
  ok = true;
  Profilfortegn = (Sl.Profilnr > Beg.Profilnr)? 1 : -1;
}
//---------------------------------------------------------------------------
bool CElement::HarProfil(double Profil)
{
  if (   ((Profil > Beg.Profilnr) && (Profil > Sl.Profilnr))
      || ((Profil < Beg.Profilnr) && (Profil < Sl.Profilnr)) ) {
    return false;
  }
  else return true;
}
//---------------------------------------------------------------------------
// CElementer
//---------------------------------------------------------------------------
ifstream &operator>>(ifstream &fil, CElementer &kontainer)
{
  const int AntTegnPrLinje = 250;         //maks antall tegn per linje i 10-tabell fila.
  double IElnr = 0;
  double IBegPr = 0;
  double IRBeg = 0;
  double IRSl = 0;
  double IAKlot = 0;
  double IXBeg = 0;
  double IYBeg = 0;
  double IXSl = 0;
  double IYSl = 0;
  double IPrSl = 0;
  char str1[AntTegnPrLinje + 1];  //brukes til å lese en linje fra fila.  linje 1 av linjeparene.
  char str2[AntTegnPrLinje + 1];  //brukes til å lese en linje fra fila. linje 2 av linjeparene.
  istringstream L1;               //brukes til å parse en linje. linje 1 av linjeparene.
  istringstream L2;               //brukes til å parse en linje. linje 2 av linjeparene.
  char t1[AntTegnPrLinje + 1];                    //hit leses første element på linja for testing.

  //sleter først innholdet i CElementer
  kontainer.clear();             //hvert element er alokert dynamisk så det må deletes

  try {

  //skal forst teste om det er riktig antall linjer og ingen blanke innimellom
  int n=0;                                //teller for antall linjer i fila.
  while(!fil.eof()) {
    fil.getline(str1,AntTegnPrLinje);
    L1.str(str1);
    L1 >> t1;
    if(AnsiString(t1).Pos("10") == 0) {      //har da ei linje som ikke beg med 10
      throw AnsiString("Noen linjer begynner ikke med 10.\n Eller ikkeblanke linjer i slutten av fila.");
    }//end if
    if(AnsiString(t1).Pos("10") > 0) {       //teller bare linjer som beg med 10.
      n++;
    }
  }//end while

  if ((n % 2) != 0) {
    throw AnsiString("Ikke partall antall 10-linjer.");
  }//end if

  //og er en kommet helt hit så er det riktig antall 10-linjer.  Neste blir å parse to og to linjer:
  //nullstiller før bruk:
  n=0;
  fil.seekg(0);
  for(int i = 0;i<AntTegnPrLinje; i++) {
    str1[i] = 0;
    str2[i] = 0;
  }//end for

  while(!fil.eof()) {
    n++;                //teller linjene for å kunne rap linjenr med feil
    for(int i = 0;i<AntTegnPrLinje; i++) {
      str1[i] = 0;
      str2[i] = 0;
    }//end for
    fil.getline(str1,AntTegnPrLinje);
    fil.getline(str2,AntTegnPrLinje);
    if (sscanf(str1, "10 %lf %lf %lf %lf %lf",&IElnr, &IBegPr, &IRBeg, &IRSl, &IAKlot) < 5) {
      throw AnsiString("feil i linje ")+ n;
    }
    if (sscanf(str2, "10 %lf %lf %lf %lf %lf",&IXBeg, &IYBeg, &IXSl, &IYSl, &IPrSl)    < 5) {
      throw AnsiString("feil i linje ")+ (n+1);
    }

    //her skal elementtypen bestemmes og objekt opprettes og legges til kontaineren:
    //bestemmer først elementtype:
    enElementtype Elementtype;
    if (IAKlot != 0) {
      Elementtype = Klotoide;
    }
    else  if ( (IRBeg == IRSl) && (IRBeg != 0) ) {
      Elementtype = Sirkelsegm;
    }
    else Elementtype = Rettlinje;
    //så skal objekt oppretes:
    CElement *pElement = 0;
    switch (Elementtype){
      case Klotoide:
        pElement = new CKlotoide(IElnr, IBegPr, IRBeg, IRSl, IAKlot, IXBeg, IYBeg, IXSl, IYSl, IPrSl,Elementtype);
        kontainer.push_back(pElement);
      break;
      case Sirkelsegm:
        pElement = new CSirkelsegm(IElnr, IBegPr, IRBeg, IRSl, IAKlot, IXBeg, IYBeg, IXSl, IYSl, IPrSl,Elementtype);
        kontainer.push_back(pElement);
      break;
      case Rettlinje:
        pElement = new CRettlinje(IElnr, IBegPr, IRBeg, IRSl, IAKlot, IXBeg, IYBeg, IXSl, IYSl, IPrSl,Elementtype);
        kontainer.push_back(pElement);
      }//end switch
  }//end while
  }//end try

  catch (const AnsiString &s) {
    kontainer.clear();
    Application->MessageBox(s.c_str(),MB_OK,0);
  }//end catch på AnsiString
  return fil;
}
//---------------------------------------------------------------------------
void CElementer::clear()
{
  CElement *pElement = 0;
  CElementIterator i;
  for(i=begin(); i != end(); i++)
  {
    pElement = *i;
    delete pElement;
  }//end for
  kont.clear();
}
//---------------------------------------------------------------------------
void CElementer::FyllStringGrid(TStringGrid *StringGrid)
{
  //sletter først alt innhold i stringgridden:
  int I, J;
  for (I = 0; I < StringGrid->ColCount; I++)
    for (J = 1; J < StringGrid->RowCount; J++)
      StringGrid->Cells[I][J] = " ";
  //leser så inn i stringgridden:
  CElement *pElement = 0;
  CElementIterator i;
  int linjenr = 1;
  for(i=begin(); i != end(); i++, linjenr++)
  {
    pElement = *i;
    StringGrid->Cells[0][linjenr] =   IntToStr((*pElement).Elnr);
    StringGrid->Cells[1][linjenr]  = FloatToStr((*pElement).Beg.Profilnr);
    StringGrid->Cells[2][linjenr]  = FloatToStr((*pElement).Beg.Radius);
    StringGrid->Cells[3][linjenr]  = FloatToStr((*pElement).Sl.Radius);
    StringGrid->Cells[4][linjenr]  = FloatToStr((*pElement).AKlot);
    StringGrid->Cells[5][linjenr]  = FloatToStr((*pElement).Beg.x);
    StringGrid->Cells[6][linjenr]  = FloatToStr((*pElement).Beg.y);
    StringGrid->Cells[7][linjenr]  = FloatToStr((*pElement).Sl.x);
    StringGrid->Cells[8][linjenr]  = FloatToStr((*pElement).Sl.y);
    StringGrid->Cells[9][linjenr]  = FloatToStr((*pElement).Sl.Profilnr);
  }//end for
  if (size()>0) {
    StringGrid->RowCount = size() + 2;
    StringGrid->FixedRows = 1;
  }
  else {
    StringGrid->RowCount = 1;
    StringGrid->FixedRows = 0;
  }
}
//---------------------------------------------------------------------------
CPunkt CElementer::PAtilXY(double Profil, double Avsett)
{
  CElement *pElement = 0;
  CElementIterator i;
  for(i=begin(); i != end(); i++)
  {
    //finner elementet som inneholder profilnummeret
    pElement = *i;
    if ( (*pElement).HarProfil(Profil) && (*pElement).ok )
    {
      return (*pElement).PAtilXY(Profil,Avsett);
    }
  }//end for
  //kommer hit hvis ingen av elementene inneh profilnummeret, returnerer da 0,0 som dummy.
  return CPunkt(0,0);
}
//---------------------------------------------------------------------------
CPA CElementer::XYtilPA(double X, double Y)         //forelopig kan denne met bare returnere forste treff.
{                                                   //muligens er det flere treff der avst til linja er mindre
  CPunkt Pkt(X,Y);                                  //det bor undersokes i en mer rafinert utg av metoden.
  CPA Result(0,STORTTALL);                          //første gjettning på PA, en slags dumy.
  CPA PA;                                           //denne brukes løpende til midlertidig lagring av beregnet pa.
  CElement *pElement = 0;
  CElementIterator i;
  for(i=begin(); i != end(); i++)
  {
    //finner forste element som punktet kan felles ned pa.
    pElement = *i;
    if ( (*pElement).Nedfellbar(Pkt) ) {
      PA = (*pElement).XYtilPA(Pkt);
      if (fabs(PA.Avsett) < fabs(Result.Avsett)) {
        Result = PA;
      }
    }
  }//end for
  if ((Result.Profil == 0) && (Result.Avsett == STORTTALL)) {     //da kunne ikke punktet felles ned. Returnerer dummy.
    return CPA(0,0);
  }
  else {return Result;}
}
//---------------------------------------------------------------------------
// CRettlinje
//---------------------------------------------------------------------------
bool CRettlinje::Nedfellbar(CPunkt P)
{
  double X1 = Beg.x;
  double Y1 = Beg.y;
  double X2 = Sl.x;
  double Y2 = Sl.y;
  double X4 = P.x;
  double Y4 = P.y;
  long double t = ((Y4-Y1)*(Y2-Y1) + (X4-X1)*(X2-X1)) /
              (powl(X2-X1,2) + powl(Y2-Y1,2));
  if ( (t < 0) || (t > 1) )
  {
    return false;
  }
  else
  {
    return true;
  }
}
//---------------------------------------------------------------------------
CPunkt CRettlinje::Nedfellt(CPunkt P)
{
  double X1 = Beg.x;
  double Y1 = Beg.y;
  double X2 = Sl.x;
  double Y2 = Sl.y;
  double X4 = P.x;
  double Y4 = P.y;
  long double t = ((Y4-Y1)*(Y2-Y1) + (X4-X1)*(X2-X1)) /
              (powl(X2-X1,2) + powl(Y2-Y1,2));
  double X3 = X1 + t*(X2 - X1);
  double Y3 = Y1 + t*(Y2 - Y1);
  return CPunkt(X3,Y3);
}
//---------------------------------------------------------------------------
CRettlinje::CRettlinje(int IElnr, double IBegPr, double IRBeg, double IRSl, double IAKlot,
           double IXBeg, double IYBeg, double IXSl, double IYSl, double IPrSl, enElementtype IElementtype):
           CElement(IElnr,IBegPr,IRBeg,IRSl,IAKlot,IXBeg,IYBeg,IXSl,IYSl,IPrSl,IElementtype)
{
  Beg.Retning = Sl.Retning = Retnv(Beg,Sl);
}
//---------------------------------------------------------------------------
CPunkt CRettlinje::PtilXY(double Prnr)
{
  if (!HarProfil(Prnr)) {
    return CPunkt(0,0);                         //dummyoppførsel
  }
  return Polar(Beg, (Prnr - Beg.Profilnr)*Profilfortegn, Beg.Retning);
}
//---------------------------------------------------------------------------
CPA CRettlinje::XYtilPA(CPunkt P)
{
  if  (!(Nedfellbar(P)) )
  {
    return CPA(0,0);                 //dummyoppførsel
  }
  double Prnr = Beg.Profilnr + Avstand(Beg, Nedfellt(P))*Profilfortegn;
  double avsett;
  if ( fabs(Vinkel(Beg.Retning, Retnv(Nedfellt(P),P))-M_PI_2) < Omtrentnull) {
    avsett = Avstand(Nedfellt(P), P);
  }
  else
  {
    avsett = -Avstand(Nedfellt(P), P);
  }
  return CPA(Prnr, avsett);
}
//---------------------------------------------------------------------------
CPunkt CRettlinje::PAtilXY(double Prnr, double Avsett)
{
  if (!HarProfil(Prnr)) {
    return CPunkt(0,0);                         //dummyoppførsel
  }
  CPunkt Mellompkt = PtilXY(Prnr);
  long double retning = Beg.Retning + M_PI_2*Profilfortegn;
  return Polar(Mellompkt, Avsett, retning);
}
//---------------------------------------------------------------------------
// CSirkelsegm
//---------------------------------------------------------------------------
CSirkelsegm::CSirkelsegm(int IElnr, double IBegPr, double IRBeg, double IRSl, double IAKlot,
           double IXBeg, double IYBeg, double IXSl, double IYSl, double IPrSl, enElementtype IElementtype):
           CElement(IElnr,IBegPr,IRBeg,IRSl,IAKlot,IXBeg,IYBeg,IXSl,IYSl,IPrSl,IElementtype)
{
  //skal sette senterpkt og retn vinkler i beg og sl    
  //konstruerer forst fram senterpunktet S
  double Kordelengde = Avstand(Beg,Sl);
  double Korderetning = Retnv(Beg,Sl);
  CPunkt MellomPkt = Polar(Beg,Kordelengde/2,Korderetning);
  double Avst;                                               // avst fra Mellompkt til sentrum.
  if (fabs(Kordelengde-2*fabs(Beg.Radius)) < Omtrentnull ) { //da går korden gjennom sirkelsentrum
    Avst = 0;
  }
  else {
    Avst = fabs(Kordelengde / (2*tan(asin(Kordelengde/(2*fabs(Beg.Radius))))));
  }
  double RetningS;                                           //retning fra Mellompunkt til sentrum.
  if ((Beg.Radius > 0) &&  (fabs(Sl.Profilnr - Beg.Profilnr) < fabs(Beg.Radius) * M_PI) ) {
    RetningS = Korderetning + M_PI/2;
  }
  else if ( (Beg.Radius < 0) &&  (fabs(Sl.Profilnr - Beg.Profilnr) < fabs(Beg.Radius) * M_PI) ) {
    RetningS = Korderetning - M_PI/2;
  }
  else if ((Beg.Radius > 0) &&  (fabs(Sl.Profilnr - Beg.Profilnr) >= fabs(Beg.Radius) * M_PI) ) {
    RetningS = Korderetning - M_PI/2;
  }
  else if ((Beg.Radius < 0) &&  (fabs(Sl.Profilnr - Beg.Profilnr) >= fabs(Beg.Radius) * M_PI) ) {
    RetningS = Korderetning + M_PI/2;
  }
  CPunkt S = Polar(MellomPkt,Avst,RetningS);
  Sentrum = S;
                       //så beregnes og settes retningsvinklene
  if (Beg.Radius > 0)
  {
    Beg.Retning = Retnv(S,Beg) + M_PI/2;
    Sl.Retning  = Retnv(S,Sl) + M_PI/2;
  }
  else
  {
    Beg.Retning  = Retnv(S,Beg) - M_PI/2;
    Sl.Retning   = Retnv(S,Sl) - M_PI/2;
  }
//skal sikre at retningsvinklene ligger i første omløp
  if (Beg.Retning > 2*M_PI) Beg.Retning = Beg.Retning - 2*M_PI;
  if (Sl.Retning  > 2*M_PI) Sl.Retning  =  Sl.Retning - 2*M_PI;
  if (Beg.Retning < 0) Beg.Retning = Beg.Retning + 2*M_PI;
  if ( Sl.Retning < 0)  Sl.Retning =  Sl.Retning + 2*M_PI;
}
//---------------------------------------------------------------------------
bool CSirkelsegm::Nedfellbar(CPunkt Pkt) //     ligger Pkt i sirkelsegm-sektoren?
{
  double RetningBeg = Retnv(Sentrum, Beg);
  double RetningSl = Retnv(Sentrum, Sl);
  double RetningPkt = Retnv(Sentrum, Pkt);
  double Vinkel1 = min(Vinkel(RetningBeg, RetningPkt), Vinkel(RetningPkt, RetningBeg));  //vinkelendringen langs buen fra Beg til Pkt, absoluttverdi.
  double Vinkel2 = min(Vinkel(RetningPkt, RetningSl), Vinkel(RetningSl, RetningPkt));    //vinkelendringen langs buen fra Pkt til Sl, absoluttverdi.
  double BuensVinkelendring = Vinkelendring(); //  Absoluttverdi av vinklendringen langs buen.
  if ((Vinkel1 <= BuensVinkelendring) && (Vinkel2 <= BuensVinkelendring)){
    return true;
  }
  return false;
}
//---------------------------------------------------------------------------
CPunkt CSirkelsegm::Nedfellt(CPunkt P)
{
return Polar(Sentrum,fabs(Beg.Radius),Retnv(Sentrum,P) );
}
//---------------------------------------------------------------------------
CPunkt CSirkelsegm::PtilXY(double Prnr)
{
  if (!HarProfil(Prnr)) {
    return CPunkt(0,0);                         //dummyoppførsel
  }
  double retning = Retnv(Sentrum, Beg) + fabs(Prnr - Beg.Profilnr)/Beg.Radius;
  return Polar(Sentrum, fabs(Beg.Radius), retning);
}
//---------------------------------------------------------------------------
CPA CSirkelsegm::XYtilPA(CPunkt P)
{
  if (!(Nedfellbar(P)) ) {
    return CPA(0,0);                 //dummyoppførsel
  }
  double RetningBeg = Retnv(Sentrum, Beg);
  double RetningPkt = Retnv(Sentrum, P);
  double Vinkel1 = min(Vinkel(RetningBeg, RetningPkt), Vinkel(RetningPkt, RetningBeg));  //vinkelendringen langs buen fra Beg til Pkt, absoluttverdi.
  double BuensVinkelendring = Vinkelendring(); //  Absoluttverdi av vinklendringen langs buen.

  double Profil = Beg.Profilnr +
        (Sl.Profilnr - Beg.Profilnr) *  Vinkel1 / BuensVinkelendring;
  double avsett = (Beg.Radius - Avstand(Sentrum, P) * Beg.Radius/fabs(Beg.Radius)) * Profilfortegn;
  return CPA(Profil, avsett);
}  
  //---------------------------------------------------------------------------
CPunkt CSirkelsegm::PAtilXY(double Prnr, double Avsett)
{
  if (!HarProfil(Prnr)) {
    return CPunkt(0,0);                         //dummyoppførsel
  }
  CPunkt Mellompkt = PtilXY(Prnr);
  double retning = (Profilfortegn == 1)? Retnv(Mellompkt,Sentrum) : Retnv(Sentrum,Mellompkt) ;
  return Polar(Mellompkt, Avsett * Beg.Radius/fabs(Beg.Radius), retning);
}
//---------------------------------------------------------------------------
// CEnhetsklotoide
//---------------------------------------------------------------------------

double CEnhetsKlot::MatFunk1(double n)
{
  double Result = 1;
  for (int i=1;i<(n+1);i++)
  {
    Result *= i;
  }
  return Result;
}
//---------------------------------------------

double CEnhetsKlot::FsinCl()
{
  double Result = 0;
  for (int i=1;i<20;++i)
  {
    Result = Result + ( powl(-1, (i+1)) )
    * (2*i-1) / (4*i-3)
    * ( powl(Tau, (2*i-1)) / MatFunk1(2*i-1) );
    }//for
  return Result;
}
//----------------------------------------------

double CEnhetsKlot::FcosCl()
{
  double Result = 1;
  for (int i=1;i<20;++i)
  {
    Result = Result + powl(-1, i)
    * (2*i) / (4*i-1)
    * ( powl(Tau, 2*i) / MatFunk1(2*i) );
  }//for
  return Result;
}
//-----------------------------------------------

double CEnhetsKlot::BerPX()
{
  if (Tau==0) {
    return 0;
  }

  double X = (2/sqrt(2*Tau)) * sinCl;
  return X;
}
//-----------------------------------------------

double CEnhetsKlot::BerPY()
{
  if (Tau==0) {
    return 0;
  }

  double Y = (2/sqrt(2*Tau)) * (1-cosCl);
  return Y;
}
//------------------------------------------------

double CEnhetsKlot::BerMX()
{
  if (Tau==0) {     //dummyoppførsel for å ungå div med null
    return 0;
  }

  double Result = (1/sqrt(2*Tau)) * ( 2*sinCl - sin(Tau) ); //ja det er riktig, det skal stå sin(..
  return Result;
}
//------------------------------------------------

double CEnhetsKlot::BerMY()
{
  if (Tau==0) {     //dummyoppførsel for å ungå div med null
    return 0;
  }

  double Result = 2 * (1/sqrt(2*Tau)) * ( 1 - cosCl + cos(Tau));
  return Result;
}
//------------------------------------------------

bool CEnhetsKlot::SetTau(double T)
{
  Tau = T;
  sinCl = FsinCl();
  cosCl = FcosCl();
  PPkt.x = BerPX();
  PPkt.y = BerPY();
  MPkt.x = BerMX();
  MPkt.y = BerMY();
  return true;
}
//------------------------------------------------

bool CEnhetsKlot::SetLengde(double L)
{
  SetTau(0.5 * pow(L,2));
  return true;
}
//-----------------------------------------------

bool CEnhetsKlot::SetRadius(double R)
{
  if (R == 0) {
    SetTau(0);
  }
  else {
    SetTau(0.5 / pow(R,2));
  }
  return true;
}
//------------------------------------------------

bool CEnhetsKlot::SetPPkt(CPunkt P) // Gjelder for tau <=1 og TanCl <= 0.3430182285
{ // med minst 8 siffer nøyaktighet i Tau.
  double TauVerdi;
  if(fabs(P.x) < 0.000009) {
    TauVerdi = 0;
  }
  else {
    double TanCl = P.y/P.x;
    TauVerdi = 2.9999999616 * TanCl
    - 0.7714221291 * pow(TanCl, 3)
    + 0.4732321515 * pow(TanCl, 5)
    - 0.32278317 * pow(TanCl, 7)
    + 0.1980138 * pow(TanCl, 9);
  }//end else
  if (fabsl(TauVerdi) >1) {                  //er da utenfor gyldighetsområdet og jeg setter en dummy
    TauVerdi = 0;
    return false;
  }
  SetTau(TauVerdi);
  return true;
}
//------------------------------------------------

bool CEnhetsKlot::SetMPkt(CPunkt M) //Gjelder for CotHb <=0.8335160378
{
  double TauVerdi;
  double CotHb=0;
  if (MPkt.x ==0) {
    TauVerdi = 0;
  }
  else {
    CotHb = MPkt.y/MPkt.x;
    double ledd1, ledd2; //mellomverdier i beregningen av Tau
    ledd1 = 1.0000000052 * CotHb
    - 1.1328519856 * pow(CotHb, 3)
    + 0.3283815154 * pow(CotHb, 5)
    - 0.0152468524 * pow(CotHb, 7);
    ledd2 = 1.0
    - 1.3328517446 * pow(CotHb, 2)
    + 0.5151607337 * pow(CotHb, 4)
    - 0.0516106533 * pow(CotHb, 6);
    TauVerdi = ledd1/ledd2;
  }//end else
  if (CotHb >0.8335160378) {                  //er da utenfor gyldighetsområdet og jeg setter en dummy
    TauVerdi = 0;
    return false;
  }
  SetTau(TauVerdi);
  return true;
}
//------------------------------------------------
// CKlotoide
//------------------------------------------------
//------------------------------------------------
CKlotoide::CKlotoide(int IElnr, double IBegPr, double IRBeg, double IRSl, double IAKlot,
           double IXBeg, double IYBeg, double IXSl, double IYSl, double IPrSl, enElementtype IElementtype)
           : CElement(IElnr,IBegPr,IRBeg,IRSl,IAKlot,IXBeg,IYBeg,IXSl,IYSl,IPrSl,IElementtype)
{


    //Her settes div parametre som beskriver klotoiden. Av disse brukes flaggene Speilvendt og
  //Snudd og parametrene Vendpunkt og Rotasjon til å regne fra/til enhetsklotoiden i transf metodene i
  //CEnkelklotoide.

    //Benytter kopier av Beg og Sl, for å takle komplikasjonene pga Snudd og Speilvendt:
  CElementPunkt BegMel = Beg;
  CElementPunkt SlMel = Sl;
  CElementPunkt Mel;

  //Skal sette flagget Snudd
  if (BegMel.Radius == 0) {
    Snudd = false;
  }
  else if (SlMel.Radius == 0) {
    Snudd = true;
  }
  else if (fabs(SlMel.Radius) > fabs(BegMel.Radius)) {
    Snudd = true;
  }
  else {
    Snudd = false;
  }

  //hvis snudd så snur jeg rekkefølge av punktene og snur fortegn på radius.
  if (Snudd){
    Mel = BegMel;
    BegMel = SlMel;
    SlMel = Mel;
    BegMel.Radius = -BegMel.Radius;
    SlMel.Radius = -SlMel.Radius;
  }

  //Skal sette flagget Speilvendt. Vet nå allerede at Mel er en ikkesnudd klotoide.
  if ((BegMel.Radius < 0) || (SlMel.Radius < 0)) {
    Speilvendt = true;
  }
  else {
    Speilvendt = false;
  }

  //hvis speilvendt så speilvender jeg, det innebærer også skifte av fortegn på radius.
  if (Speilvendt)
  {
    BegMel.y = -BegMel.y;
    SlMel.y = -SlMel.y;
    BegMel.Radius = -BegMel.Radius;
    SlMel.Radius = -SlMel.Radius;
  }

  //Skal finne Rotasjon. Dette er alså rotasjon etter at transf er gjort for Snudd og Speilvendt.
  //world_klotoidevinkel = transf_for_snudd_og_rot(enhklotvinkel + rotasjon)

                        //Lagrer først unna verdiene lagret i Vendepunkt og Rotasjon og Snudd og Speilvendt
  CPunkt GmlVendepunkt = Vendepunkt;
  double GmlRotasjon = Rotasjon;
  bool GmlSnudd = Snudd;
  bool GmlSpeilvendt = Speilvendt;

  Vendepunkt = CPunkt(0,0);
  Rotasjon = 0;
  Snudd = Speilvendt = false;

                        //så finnes BegMel og SlMel transformert til enhetsklotoiden: PA og PB
  CPunkt PA,PB;
                        //først P1 til PA:
  if (BegMel.Radius == 0) {
    PA = CPunkt(0,0);
  }
  else {
    CEnhetsKlot::SetRadius(BegMel.Radius/AKlot);
    PA = GetPPkt();
  }
                        //så P2 til PB
  if (SlMel.Radius == 0) {
    PB = CPunkt(0,0);
  }
  else {
    CEnhetsKlot::SetRadius(SlMel.Radius/AKlot);
    PB = GetPPkt();

  }
                        //beregner rotasjonen
  double Rot = Retnv(BegMel,SlMel) - Retnv(PA,PB);
                        //Setter så inn igjen den gamle verdien i Vendepunkt og Rotasjon og Snudd og Speilvendt.
  Vendepunkt = GmlVendepunkt;
  Rotasjon = GmlRotasjon;
  Snudd = GmlSnudd;
  Speilvendt = GmlSpeilvendt;
                        //setter rotasjonen.
  Rotasjon = Rot;

  //Skal finne Vendepunkt. Dette er altså vendepunktet for den klotoiden som er transformert for Snudd og Speilvendt.
  CEnhetsKlot::SetRadius(SlMel.Radius/AKlot);
  CPunkt P = CEnhetsKlot::GetPPkt();
  double AvstSlVendepkt = sqrt(pow(P.x,2) + pow(P.y,2)) * AKlot;
  double RetningSlVendepkt = Retnv(P, CPunkt(0,0)) + Rotasjon;
  Vendepunkt = Polar(SlMel, AvstSlVendepkt, RetningSlVendepkt);
  //alle transformasjonsparametrene er nå satt!!
  //beregner og setter Beg.Retning og Sl.Retning.
  SetPPkt(Beg);
  Beg.Retning = GetRetning();
  SetPPkt(Sl);
  Sl.Retning = GetRetning();
  //skal sette flagget ok = false hvis tau kan bli større enn 1:
  float minsteradius;
  if      (Beg.Radius == 0) { minsteradius = fabs(Sl.Radius);}
  else if (Sl.Radius ==  0) { minsteradius = fabs(Beg.Radius);}
  else                      { minsteradius = min(fabs(Beg.Radius), fabs(Sl.Radius));}
  if( (minsteradius/AKlot)<0.7071)  {
    ok = false;
    Application->MessageBox("en eller flere av klotoidene kan ikke beregnes fordi den har for stort klotoideparameter/radius forhold. Klotoiden vises i lista men settes innaktiv i beregningene.",MB_OK,0);
  }
  }

//------------------------------------------------

CPunkt CKlotoide::PAtilXY(double Prnr, double Avsett)
{
  if (!HarProfil(Prnr) || !ok) {
    return CPunkt(0,0);                         //dummyoppførsel
  }
  SetProfil(Prnr);
  CPunkt CLPunkt = GetPPkt();
//beregner så polart til søkt avsett ut fra cl.
  double retningcltilpkt = (Profilfortegn > 0)? TransfRetnv(Retning(Prnr),-M_PI_2) : TransfRetnv(Retning(Prnr),M_PI_2); 
  return Polar(CLPunkt, Avsett, retningcltilpkt);
}
//------------------------------------------------
/*
double CKlotoide::Retning(double Prnr)
{
  SetProfil(Prnr);
  return GetRetning();
}
*/
//------------------------------------------------
void CKlotoide::SetProfil(double Profilnr)
{
  //denne setter punktet PPkt i klotoiden tilsvarende SetLengde() og SetRadius().
  SetPPkt(Beg);
  double BegLengde = GetLengde();
  SetPPkt(Sl);
  double SlLengde = GetLengde();
  double lengde = (Profilnr - Beg.Profilnr) * (SlLengde - BegLengde) / (Sl.Profilnr - Beg.Profilnr) + BegLengde;
  if (Snudd){
    lengde = (Profilnr - Sl.Profilnr) * (BegLengde - SlLengde) / (Beg.Profilnr - Sl.Profilnr) + SlLengde;
  }
  SetLengde(lengde);
}
//------------------------------------------------
bool CKlotoide::Nedfellbar(CPunkt P)
{
  if(!ok) return false;            //ok=false hvis klot kan få tau>1.
  //hvis P sammenfaller med Beg eller Sl returneres true:
  if ((P==Beg) || (P==Sl)) return true;
  //beregner vinkel fra Beg til P og fra Sl til P hvis vinklene er henholdsvis større og mindre enn 90 grader, er
  //det ikke nedfellbart.
  double RetnFraBegPkt = Retnv(Beg, P);
  double VinkelFraBeg = min(Vinkel(RetnFraBegPkt,Beg.Retning), Vinkel(Beg.Retning, RetnFraBegPkt));
  if (VinkelFraBeg > M_PI_2){
    return false;
  }
  double RetnFraSlPkt = Retnv(Sl, P);
  double VinkelFraSl = min(Vinkel(RetnFraSlPkt,Sl.Retning), Vinkel(Sl.Retning, RetnFraSlPkt));
  if (VinkelFraSl < M_PI_2){
    return false;
  }
  return true;
}
//------------------------------------------------
CPA CKlotoide::XYtilPA(CPunkt P)
{
  if (!ok) {return CPA(0,0);}
  //her skal jeg iterere fram fra vendepunktsenden av klotoiden.
  const double NOYAKTIGHET = 0.0001;     //itererer så lenge steplengden er større en denne.
  double Startprofil;
  double ElLengde;
  double Profil;
  double Ankerprofil;

  if (Snudd) {
    Startprofil = Sl.Profilnr;
    ElLengde = Beg.Profilnr - Sl.Profilnr;
  }
  else {
    Startprofil = Beg.Profilnr;
    ElLengde = Sl.Profilnr - Beg.Profilnr;
  }
  Ankerprofil = Startprofil;
  double Steplengde = ElLengde/40;     //tilstrekkelig små step til at det ikke blir flere løsninger i ett steppintervall.

 /* Profil = Ankerprofil;
  double w = Retning(Profil);
  double e = TransfRetnv(Retning(Profil),M_PI);
  double q = (Snudd ? TransfRetnv(Retning(Profil),M_PI) : Retning(Profil) );
 */

  while(fabs(Steplengde) > NOYAKTIGHET) {
    for (Profil = Ankerprofil;
      (min(Vinkel( (Snudd ? TransfRetnv(Retning(Profil),M_PI): Retning(Profil) ), Retnv(GetPPkt(),P)), Vinkel(Retnv(GetPPkt(),P),(Snudd ? TransfRetnv(Retning(Profil),M_PI) : Retning(Profil))   ))) < M_PI_2         //dette monsteret:(Snudd ? TransfRetnv(Retning(Profil),M_PI) : Retning(Profil)) er retningsvinkelen i cl i Profil = Profil i linjeretning mot økende krumning
       && (fabs(Profil - Startprofil)< fabs(ElLengde));) {
      Profil = Profil + Steplengde;
      SetProfil(Profil);
    }//end for
    
    Ankerprofil = ((Profil == Ankerprofil) ? Ankerprofil : (Profil - Steplengde));
    Steplengde = Steplengde/2;
  }//end while
  Profil = (Profil + Ankerprofil)/2;

  //så beregnes avsett
  SetProfil(Profil);
  CPunkt NedfelltP = GetPPkt();
  double framretning = Retning(Profil);
  double Avsett = Avstand(NedfelltP,P);

  if ( (Vinkel(framretning,Retnv(NedfelltP,P)) - M_PI) < Omtrentnull) {
    Avsett = Avsett * Profilfortegn;
  }
  else {
    Avsett = -Avsett * Profilfortegn;
  }
  return CPA(Profil,Avsett);
}
//------------------------------------------------------------------------
double CKlotoide::GetRetning()         //retning i punktet PPkt langs linja fra Beg til Sl.
{
  double retning = TransfRetningFraEnhklotSpace(GetTau() );
  if (Snudd) {
    retning = TransfRetnv(retning, M_PI);
  }
  return retning;
}
//-----------------------------------------------

double CKlotoide::Retning(double Prnr)
{
  SetProfil(Prnr);
  return GetRetning();
}
//------------------------------------------------------------------------
CPunkt CKlotoide::TransfPktTilEnhklotSpace(CPunkt P)
{
  if (Speilvendt) {
    P = SpeilPktOmXAksen(P);
  }
  CPunkt TranslatertPkt = Translater(P,-Vendepunkt.x,-Vendepunkt.y);
  return RoterogSkaler(TranslatertPkt,CPunkt(0,0),-Rotasjon,1/AKlot);
}
//------------------------------------------------
CPunkt CKlotoide::TransfPktFraEnhklotSpace(CPunkt P)
{
  CPunkt RSPkt = RoterogSkaler(P,CPunkt(0,0),Rotasjon,AKlot);
  CPunkt TranslatertPkt = Translater(RSPkt,Vendepunkt.x,Vendepunkt.y);
  if (Speilvendt) {
    TranslatertPkt = SpeilPktOmXAksen(TranslatertPkt);
  }
  return TranslatertPkt;
}
//------------------------------------------------
bool CKlotoide::SetLengde(double L)      //alltid L> = 0
{
  CEnhetsKlot::SetLengde(L / AKlot);
  return true;
}
//------------------------------------------------
bool CKlotoide::SetRadius(double R)
{
  if (Snudd) {
    R = -R;
  }
  if (Speilvendt) {
    R = -R;
  }
  if (R==0){R = STORTTALL;}    //R=0 er jo ikke riktig når det er rettlinje, dum feil i titabformatet.. retter på det her.
  if (R < 0) return false;     //R skal være større enn 0 for at formler for enh klotoiden skal fungere
  CEnhetsKlot::SetRadius(R/AKlot);
  return true;
}
//------------------------------------------------
bool CKlotoide::SetPPkt(CPunkt P)
{
  CEnhetsKlot::SetPPkt(TransfPktTilEnhklotSpace(P));
  return true;
}
//------------------------------------------------
bool CKlotoide::SetMPkt(CPunkt M)
{
  CEnhetsKlot::SetMPkt(TransfPktTilEnhklotSpace(M));
  return true;
}
//------------------------------------------------
double CKlotoide::GetLengde()
{
  return CEnhetsKlot::GetLengde() * AKlot;
}
//------------------------------------------------
double CKlotoide::GetRadius()
{
  double R = CEnhetsKlot::GetRadius() * AKlot;
  if (Speilvendt){
    R = -R;
  }
  if (Snudd){
    R = -R;
  }
  return R;
}
//------------------------------------------------
CPunkt CKlotoide::GetPPkt()
{
  return TransfPktFraEnhklotSpace(CEnhetsKlot::GetPPkt());
}
//------------------------------------------------
CPunkt CKlotoide::GetMPkt()
{
  return TransfPktFraEnhklotSpace(CEnhetsKlot::GetMPkt());
}
//------------------------------------------------

double CKlotoide::TransfRetningFraEnhklotSpace(double retning)
{
  retning = TransfRetnv(retning,-Rotasjon);
  if (Speilvendt){
    retning = SpeilVinkelOmXAksen(retning);
  }
  return retning;
}
//------------------------------------------------
double CKlotoide::TransfRetningTilEnhklotSpace(double retning)
{
  if (Speilvendt){
    retning = SpeilVinkelOmXAksen(retning);
  }
  retning = TransfRetnv(retning,Rotasjon);
  return retning;
}
//------------------------------------------------






























