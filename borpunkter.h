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
//---------------------------------------------------------------------------
#ifndef borpunkterH
#define borpunkterH
//---------------------------------------------------------------------------
//merk at disse metodene leser filer med . som desimalseparator.  Det går bra så lange global variabel settes først:  DecimalSeparator = '.'  Det gjøres i cpp fila ved streaming fra fil til CBoringer.  desuten lagres gammel state og settes tilbake ved retur av funksjonen.

using namespace std;

enum enLogFormat {Geoprinter50, Geoprinter60, Geologg, UkjentFormat};

enum enBortype {UkjentBortype, Dreiesondering, Proveserie, Provebelastning, Enkelsondering,
               Dreietrykksondering, Trykksondering, Skruplateforsok, Ramsondering,
               Setningsmaling, SPT, Fjellkontrollboring, Poretrykksmaling,
               InSituPermeabilitetsmaling, Vingeboring, ElektriskSondering,
               Helningsmaling, Totalsondering, PoretrykksutjevningVedCPT, TotalsonderingForCPT};

AnsiString GiStringForBortype(enBortype t);
//---------------------------------------------------------------------------
class CBoring {
private:

protected:

public:
  virtual void FyllStringGrid(TStringGrid* StringGrid){}
  virtual double GiBoretLengdeIFjell() {}
  virtual int GiSluttkode(){}
  virtual double GiDybdeTilFjell() {}
  virtual double GiBoretDybde() {}
  virtual double GiForboretDybde() {}
  virtual double GiBoretDybdeILosmasser() {}
  int Metodeparameter2;
  int Lopenr;
  enBortype Bortype;
  int AntParamPerDatalinje;
  int AntDatalinjerEtterD;
  AnsiString Serienr;
  int AarManedDag;
  int LopenrSondering;
  AnsiString SignBorleder;
  AnsiString Prosjektnr;
  AnsiString Oppdragsnr;
  AnsiString BorpktNr;
  double Profil;
  double Avsett;
  double X;
  double Y;
  double Z;
  double Forboringsdybde;
  double Vanndybde;
  double Sonderingsdybde;
  int SerienrSonde;
  AnsiString SerienrFeltdatainnsamlingsenhet;
  AnsiString Oppdragsgiver;
  AnsiString Firmanavn;
  double Grunnvannsniva;
  double Starttidspunkt;
  AnsiString FriTekst;
  AnsiString Koordinatsystem;
  enLogFormat BestemLogFilFormat(AnsiString s);
  enLogFormat LogFormat;
  double Barometertrykk;
};
//-------------------------------------------------------------------------
typedef vector<CBoring*>::iterator CBoringIterator;
//-------------------------------------------------------------------------
class CDreietrykkdata {
private:

protected:

public:
  double Dybde;
  double Matekraft;
  double Synkhastighet;
  double Moment;
  double rpm;
  int Kommentarkode;
  bool OktRotasjon;
};
//-------------------------------------------------------------------------
class CTotalsonderingsdata {
private:

protected:

public:
  double Dybde;
  double Matekraft;
  double Synkhastighet;
  double rpm;
  int Kommentarkode;
  double Spyletrykk;
  bool Spyling;
  bool OktRotasjon;
  bool Slag;
  double Moment;
};
//-------------------------------------------------------------------------
class CDreietrykksondering : public CBoring {
private:

protected:

public:
  void FyllStringGrid(TStringGrid* StringGrid);
  double GiBoretDybdeILosmasser() {return GiBoretDybde();}
  double GiBoretLengdeIFjell() {return 0;}
  int GiSluttkode(){return back().Kommentarkode;}
  double GiDybdeTilFjell() {if(GiSluttkode() == 93) return GiBoretDybde(); else return 99999;}
  double GiBoretDybde() {return back().Dybde;}
  double GiForboretDybde() {return Forboringsdybde;}
  vector<CDreietrykkdata> kont;
  vector<CDreietrykkdata>::iterator begin() {return kont.begin();}
  vector<CDreietrykkdata>::iterator end() {return kont.end();}
  vector<CDreietrykkdata>::size_type size() {return kont.size();}
  vector<CDreietrykkdata>::reference front() {return kont.front();}
  vector<CDreietrykkdata>::reference back() {return kont.back();}
  void reserve(long N) {kont.reserve(N);}
  void insert(vector<CDreietrykkdata>::iterator i, CDreietrykkdata P) {kont.insert(i, P);}
  bool empty() {return kont.empty();}
  void push_back(CDreietrykkdata P) {kont.push_back(P);}
  void Paint(TCanvas* Canvaset){}
  friend ifstream &operator>>(ifstream &stream, CDreietrykksondering &kontainer);
  //void FyllStringGrid(TStringGrid *StringGrid);
  CDreietrykksondering(){}
  ~CDreietrykksondering(){}
};

typedef vector<CDreietrykkdata>::iterator CDreietrykkdataIterator;

typedef vector<CTotalsonderingsdata>::iterator CTotalsonderingsdataIterator;

//---------------------------------------------------------------------------
class CTotalsondering : public CBoring {
private:

protected:

public:
  void FyllStringGrid(TStringGrid* StringGrid);
  double GiBoretDybdeILosmasser() {return GiBoretDybde() - GiBoretLengdeIFjell();}
  double GiBoretLengdeIFjell();
  int GiSluttkode(){return back().Kommentarkode;}
  double GiDybdeTilFjell();
  double GiBoretDybde() {return back().Dybde;}
  double GiForboretDybde() {return Forboringsdybde;}
  vector<CTotalsonderingsdata> kont;
  vector<CTotalsonderingsdata>::iterator begin() {return kont.begin();}
  vector<CTotalsonderingsdata>::iterator end() {return kont.end();}
  vector<CTotalsonderingsdata>::size_type size() {return kont.size();}
  vector<CTotalsonderingsdata>::reference front() {return kont.front();}
  vector<CTotalsonderingsdata>::reference back() {return kont.back();}
  void reserve(long N) {kont.reserve(N);}
  void insert(vector<CTotalsonderingsdata>::iterator i, CTotalsonderingsdata P) {kont.insert(i, P);}
  bool empty() {return kont.empty();}
  void push_back(CTotalsonderingsdata P) {kont.push_back(P);}
  void Paint(TCanvas* Canvaset){}
  friend ifstream &operator>>(ifstream &stream, CTotalsondering &k);
  //void FyllStringGrid(TStringGrid *StringGrid);
  CTotalsondering(){}
  ~CTotalsondering(){}
};
//---------------------------------------------------------------------------
class CUkjentBortype : public CBoring {
private:

protected:

public:
  friend ifstream &operator>>(ifstream &stream, CUkjentBortype &k);
  //void FyllStringGrid(TStringGrid *StringGrid);
  CUkjentBortype(){}
  ~CUkjentBortype(){}
};
//---------------------------------------------------------------------------
//ser ut til at disse comparefunctions må stå utenfor CBoringer klassen for å fungere:

bool SammenlignBortypeFunk(CBoring* a, CBoring* b);

bool SammenlignLopenrFunk(CBoring* a, CBoring* b);

bool SammenlignBorpktNrFunk(CBoring* a, CBoring* b);

bool SammenlignBorpktProfil(CBoring* a, CBoring* b);

bool SammenlignBorpktAvsett(CBoring* a, CBoring* b);

bool SammenlignBorpktX(CBoring* a, CBoring* b);

bool SammenlignBorpktY(CBoring* a, CBoring* b);

bool SammenlignBorpktZ(CBoring* a, CBoring* b);
//---------------------------------------------------------------------------
class CBoringer {
private:

protected:

public:
  enLogFormat BestemLogFilFormat(AnsiString s);
  enLogFormat LogFormat;
  vector<CBoring*> kont;
  vector<CBoring*>::iterator begin() {return kont.begin();}
  vector<CBoring*>::iterator end() {return kont.end();}
  vector<CBoring*>::size_type size() {return kont.size();}
  vector<CBoring*>::reference front() {return kont.front();}
  vector<CBoring*>::reference back() {return kont.back();}
  void reserve(long N) {kont.reserve(N);}
  void insert(vector<CBoring*>::iterator i, CBoring* P) {kont.insert(i, P);}
  void clear();
  bool empty() {return kont.empty();}
  void push_back(CBoring* P) {kont.push_back(P);}
  void Paint(TCanvas* Canvaset){}
  friend ifstream &operator>>(ifstream &stream, CBoringer &kontainer);
  void FyllStringGrid(TStringGrid* StringGrid);
  void SortEtterBortype()   {sort(begin(), end(), SammenlignBortypeFunk);}
  void SortEtterLopenr()   {sort(begin(), end(), SammenlignLopenrFunk);}
  void SortEtterBorpktNr() {sort(begin(), end(), SammenlignBorpktNrFunk);}
  void SortEtterProfil() {sort(begin(), end(), SammenlignBorpktProfil);}
  void SortEtterAvsett() {sort(begin(), end(), SammenlignBorpktAvsett);}
  void SortEtterX() {sort(begin(), end(), SammenlignBorpktX);}
  void SortEtterY() {sort(begin(), end(), SammenlignBorpktY);}
  void SortEtterZ() {sort(begin(), end(), SammenlignBorpktZ);}
  CBoringer(){}
  ~CBoringer(){clear();}
};
//----------------------------------------------------------------------
bool ErTall(const AnsiString &s);
//----------------------------------------------------------------------
void FjernCariageReturn(char *pc);
//----------------------------------------------------------------------



#endif
