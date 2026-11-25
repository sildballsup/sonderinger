// klasser i programmet titab
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
#include <Buttons.hpp>




#ifndef elementer1H
#define elementer1H
const double STORTTALL = 1000000000000000;       //har tenkt å bruke denne som radius når radius=0 i titab for klotoider
const double Omtrentnull = 0.001;                //brukes ved testing på flyttall
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
enum enElementtype {Rettlinje, Sirkelsegm, Klotoide};
//---------------------------------------------------------------------------
struct CPunkt {
public:
  double x;
  double y;
  bool operator ==(CPunkt P)const { return ((x==P.x)&&(y==P.y)); }
  CPunkt operator +(CPunkt P) { CPunkt temp; temp.x = x + P.x; temp.y = y + P.y; return temp; }
  CPunkt(double k, double l) {x = k; y = l;}
  CPunkt Ofset(double ofx, double ofy) {CPunkt temp; temp.x = x + ofx; temp.y = y + ofy; return temp;}
  CPunkt() {}
};
//---------------------------------------------------------------------------
struct CPA {
public:
  double Profil;
  double Avsett;
  CPA(double k, double l) {Profil = k; Avsett = l;}
  CPA() {}
};
//---------------------------------------------------------------------------
struct CElementPunkt : public CPunkt {
public:
  double Retning;
  double Profilnr;
  double Radius;
};
//---------------------------------------------------------------------------
class CGeometriFormler {
private:
protected:
  long double Avstand(CPunkt P1, CPunkt P2);
  long double Retnv(CPunkt P1, CPunkt P2);
  long double Vinkel(CPunkt P1, CPunkt P2, CPunkt P3);
  long double Vinkel(long double v1, long double v2);
  long double AbsVinkel(CPunkt P1, CPunkt P2, CPunkt P3);
  long double AbsVinkel(long double v1, long double v2);
  CPunkt Polar(CPunkt Anker, long double Avst, long double Retning);
  CPunkt RoterogSkaler(CPunkt P, CPunkt sentrum, long double rotv, long double skala) {       //obs her er rotasjonsvinkelen pos i retning med klokka.
    return CPunkt(((P.x-sentrum.x)*cos(-rotv)+(P.y-sentrum.y)*sin(-rotv))*skala,
                  ((P.y-sentrum.y)*cos(-rotv)-(P.x-sentrum.x)*sin(-rotv))*skala);}
  CPunkt Translater(CPunkt P, long double dx, long double dy) {return CPunkt(P.x+dx, P.y+dy);}
  long double TransfRetnv(long double retnv, long double retnvnyX);
  long double SpeilVinkelOmXAksen(long double vinkel){return 2*M_PI - vinkel;}    //forutsetter område [0 - 2*M_PI]
  CPunkt SpeilPktOmXAksen(CPunkt P) {return CPunkt(P.x,-P.y);}
public:
  CGeometriFormler() {}
  ~CGeometriFormler() {}
};
//---------------------------------------------------------------------------
class CElement : public CGeometriFormler {
private:
protected:
  double Vinkelendring();                                           //absoluttverdi av vinkelendringen.
  virtual CPunkt PtilXY(double Prnr)    {return CPunkt(0,0);}       //returnerer pkt for gitt prnr. Her bare en dummyoppførsel
  //virtual double Retn(double Prnr) {return 0;}                    //returnerer retningsvinkel for gitt prnr. Her bare en dummyoppførsel
  int Profilfortegn;                                                //flagg for om profilnr øker fra Beg til Sl. +1 for økende, -1 for avtakende profilnr.
public:
  CElement(int IElnr, double IBegPr, double IRBeg, double IRSl, double IAKlot,
           double IXBeg, double IYBeg, double IXSl, double IYSl, double IPrSl,enElementtype IElementtype);
  CElement() {}
  virtual bool Nedfellbar(CPunkt P) {return false;}               //kan pkt felles ned på elementet?    Her dummyoppførsel.
  virtual CPA XYtilPA(CPunkt P){return CPA(0,0);}           //returnerer profilnr og avsett. Her bare en dummyoppførsel
  virtual CPunkt PAtilXY(double Profil, double Avsett) {return CPunkt(0,0); }
  double Elementlengde() {return fabs(Sl.Profilnr - Beg.Profilnr); }
  bool HarProfil(double Profil);
  int Elnr;
  double AKlot;                              //klotoideparameteren
  CElementPunkt Beg;
  CElementPunkt Sl;
  enElementtype Elementtype;
  bool ok;
};
//---------------------------------------------------------------------------
class CElementer {
private:
protected:
public:
  vector<CElement*> kont;
  vector<CElement*>::iterator begin() {return kont.begin();}
  vector<CElement*>::iterator end() {return kont.end();}
  vector<CElement*>::size_type size() {return kont.size();}
  vector<CElement*>::reference front() {return kont.front();}
  vector<CElement*>::reference back() {return kont.back();}
  void reserve(long N) {kont.reserve(N);}
  void insert(vector<CElement*>::iterator i, CElement* P) {kont.insert(i, P);}
  void clear();
  //void erase(vector<CElement*>::iterator i, vector<CElement*>::iterator k) {kont.erase(i, k);}
  bool empty() {return kont.empty();}
  void push_back(CElement* P) {kont.push_back(P);}
  void Paint(TCanvas* Canvaset){}
  friend ifstream &operator>>(ifstream &stream, CElementer &kontainer);
  void FyllStringGrid(TStringGrid *StringGrid);
  CElementer(){}
  ~CElementer(){clear();}
  CPunkt PAtilXY(double Profil, double Avsett);
  CPA XYtilPA(double X, double Y);
};

typedef vector<CElement*>::iterator CElementIterator;
//---------------------------------------------------------------------------
class CRettlinje : public CElement {
private:
protected:
  CPunkt PtilXY(double Prnr);
  CPunkt Nedfellt(CPunkt P);
public:
  CRettlinje(int IElnr, double IBegPr, double IRBeg, double IRSl, double IAKlot,
           double IXBeg, double IYBeg, double IXSl, double IYSl, double IPrSl, enElementtype IElementtype);
  CRettlinje() {}
  bool Nedfellbar(CPunkt P);
  CPA XYtilPA(CPunkt P);
  CPunkt PAtilXY(double Prnr, double Avsett);
};
//---------------------------------------------------------------------------
class CSirkelsegm : public CElement {
private:
protected:
  CPunkt Sentrum;
  CPunkt PtilXY(double Prnr);
  CPunkt Nedfellt(CPunkt P);
public:
  CSirkelsegm(int IElnr, double IBegPr, double IRBeg, double IRSl, double IAKlot,
           double IXBeg, double IYBeg, double IXSl, double IYSl, double IPrSl, enElementtype IElementtype);
  CSirkelsegm() {}
  bool Nedfellbar(CPunkt P);
  CPA XYtilPA(CPunkt P);
  CPunkt PAtilXY(double Prnr, double Avsett);
};
//--------------------------------------------------------------------------
class CEnhetsKlot {                     //Merk at i denne klassen er x-aksen mot nord
private:                                //og y-aksen mot øst. Og positiv radius krummer til høyre.
  double Tau;
  CPunkt PPkt, MPkt; // PPkt er punkt på klotoiden, MPkt er sentrum i radius
  double MatFunk1(double n); //denne beregner n! erstatt den evt med en standardfunksjon
  double FsinCl();
  double FcosCl();
  double BerPY();
  double BerMY();
protected:
  double BerMX();
  double BerPX();
  double sinCl, cosCl; // Mellomvariabler.
public:
  bool SetLengde(double L);
  bool SetTau(double T);               //her startes mye av beregningene
  bool SetRadius(double R);
  bool SetPPkt(CPunkt P);
  bool SetMPkt(CPunkt M);
  double GetLengde() { return sqrt(2*Tau); }
  double GetTau() { return Tau; }
  double GetRadius() { return 1/sqrt(2*Tau);}
  CPunkt GetPPkt() { return PPkt; }
  CPunkt GetMPkt() { return MPkt; }
  double GetSinCl() {return sinCl;}
  CEnhetsKlot(){}
  ~CEnhetsKlot(){}
};
//---------------------------------------------------------------------------
//forutsetter: 1)Klotoiden vender ikke.
//nytt i forh til CEnkelKlotoide er:
//punktene Beg og Sl og at punkt på klotoiden får et profilnr, og et flagg for om klot er snudd.
//og en ny konstruktor

//CEnkelKlotoide, har klotoideparameter AKlot (arvet fra CElement), flagget Speilvendt og en retningsvinkelen i
//vedepunktet=Rotasjon, i denne retningen har klotoiden positiv radius
//klotoiden speilvendes evt før Rotasjon beregnes.

class CKlotoide : public CElement, public CEnhetsKlot {
private:
protected:
  bool Snudd;                        //true hvis krumningen avtar fra Beg til Sl.
  bool Speilvendt;                   //true hvis klotoiden er speilvendt, dvs krummer til venstre for L > 0.
  double Rotasjon;                   //rotasjonsvinkel for å passe inn enhetklotoiden, obs etter evt speilvending.
  CPunkt Vendepunkt;                 //R=uendelig punktet. Ligger kanskje ikke på linja men benyttes uansett som utg pkt for polare beregninger. Obs dett er vendepunkt for klot transf for snudd og speilvendt.
  CPunkt TransfPktTilEnhklotSpace(CPunkt P);
  CPunkt TransfPktFraEnhklotSpace(CPunkt P);
  double TransfRetningTilEnhklotSpace(double retning);
  double TransfRetningFraEnhklotSpace(double retning);
  bool SetLengde(double L);
  bool SetRadius(double R);
  bool SetPPkt(CPunkt P);
  bool SetMPkt(CPunkt M);
  double GetLengde();
  double GetRadius();
  CPunkt GetPPkt();
  CPunkt GetMPkt();
  void SetProfil(double Profilnr);
  double GetRetning();                //returnerer retning i punktet PPkt langs linja fra Beg til Sl.
  double Retning(double Prnr);       //retning framover i linja
  bool Nedfellbar(CPunkt P);
  CPunkt Nedfellt(CPunkt P);
  CPA XYtilPA(CPunkt P);
  CPunkt PAtilXY(double Prnr, double Avsett);
  CPunkt PtilXY(double Prnr){return PAtilXY(Prnr, 0);}
public:
  CKlotoide(int IElnr, double IBegPr, double IRBeg, double IRSl, double IAKlot,
           double IXBeg, double IYBeg, double IXSl, double IYSl, double IPrSl, enElementtype IElementtype);

  CKlotoide() {}
  ~CKlotoide() {}
};

#endif
 