//---------------------------------------------------------------------------

#ifndef acadentiteterH
#define acadentiteterH
//---------------------------------------------------------------------------
//denne fila skal inneholde klasser som tillsvarer tegningsentiteter i acad
// og en kontainerklasse for disse ( CEntiteter)
//CDreietrykkopptegningEntiteter er avledet av CEntiteter.
// pga CDreietrykkopptegningEntiteter må borpunkter.h includeres
//pga at CEntiteter benytter TMapWinGraf som igjen benytter TMapWin så må de også includeres
//elementer1 må includes fordi CPunkt benyttes.

using namespace std;
class CEntitet;
class CEntiteter;
class CDreietrykkopptegningEntiteter;
class CEntPunkt;
class CLinje;
class CVertex;
class CPolylinje;
class CArc;
class CSirkel;
class CText;


class CEntitet {
private:
protected:
public:
  int Lag;
  TColor Farge;
  int Linjetype;
  int Linjetypeskala;
  virtual void FinnXYMaxMin(double &XMax, double &YMax, double &XMin, double &YMin){}
  virtual void Paint(TMapWin &MW){}
};
//--------------------------------------------------------------------------
class CEntiteter {
private:
protected:
public:
  CPunkt CurPos;
  int CurLag;
  int CurFarge;
  int CurLinjetype;
  int CurLinjetypeskala;
  vector<CEntitet*> kont;
  vector<CEntitet*>::iterator begin() {return kont.begin();}
  vector<CEntitet*>::iterator end() {return kont.end();}
  vector<CEntitet*>::size_type size() {return kont.size();}
  vector<CEntitet*>::reference front() {return kont.front();}
  vector<CEntitet*>::reference back() {return kont.back();}
  void reserve(long N) {kont.reserve(N);}
  void insert(vector<CEntitet*>::iterator i, CEntitet* P) {kont.insert(i, P);}
  void clear();
  bool empty() {return kont.empty();}
  void push_back(CEntitet* P) {kont.push_back(P);}
  //virtual void Paint(TMapWinGraf &MWG);
  virtual void Paint(TMapWin &MW);
  CEntiteter(){  for(int i=0; i <11; i++) LagErPaslatt[i] = true;
                 CurLag = CurFarge = CurLinjetype = CurLinjetypeskala = 1;
                 CurPos = CPunkt(0,0);}
  ~CEntiteter(){clear();}
  bool LagErPaslatt[11];
  void SlaPaLag(int Lagnr) { if( (Lagnr>-1) && (Lagnr<11) ) {LagErPaslatt[Lagnr]=true;};  }
  void SlaAvLag(int Lagnr) { if( (Lagnr>-1) && (Lagnr<11) ) {LagErPaslatt[Lagnr]=false;}; }
  void FinnXYMaxMin(double &XMax, double &YMax, double &XMin, double &YMin);
  void SetViewMaxMin(TMapWinGraf &MWG);
  void AdEntPunkt(CPunkt P, int lag, TColor farge, int linjetype, int linjetypeskala){}
  CLinje *AdLinje(CPunkt Fra, CPunkt Til, int lag, TColor farge, int linjetype, int linjetypeskala);
  CPolylinje *AdPolylinje(int lag, TColor farge, int linjetype, int linjetypeskala, CVertex *pVertex, int AntVertex);
  void AdArc(CPunkt Senter, double radius, double startvinkel, double sluttvinkel,int lag, TColor farge, int linjetype, int linjetypeskala) {}
  void AdSirkel(CPunkt Senter, double radius, int lag, TColor farge, int linjetype, int linjetypeskala) {}
  CText *AdText(AnsiString tekst, double hoyde, CPunkt Startpkt, int lag, TColor farge, int linjetype, int linjetypeskala);
  virtual void Oppdater(){}
};
//---------------------------------------------------------------------------
typedef vector<CEntitet*>::iterator CEntitetIterator;
//---------------------------------------------------------------------------
class CBorprofilEntiteter : public CEntiteter {
private:
protected:
public:
  int ValgtDiagram;     //brukes bare i avledete klasser for borprofilopptegning der denne var bestemmer branching til riktig opptegningsfunksjon.
  virtual void FyllDiagramAltTilCombobox(TComboBox *Cb){}
  virtual void SetValgtDiagram(int DiagramNr) {}
  CBorprofilEntiteter() {ValgtDiagram = 0;}
  ~CBorprofilEntiteter() {}
};
//---------------------------------------------------------------------------
class CDreietrykkopptegningEntiteter : public CBorprofilEntiteter {
private:
protected:
public:
  CDreietrykksondering *DataKilde;
  void SetValgtDiagram(int Nr) {ValgtDiagram = Nr;}
  void FyllDiagramAltTilCombobox(TComboBox *Cb) {
    Cb->Clear();
    Cb->Items->Add("Standard");
    Cb->Items->Add("Standard med tekstete koder");
    Cb->ItemIndex = 0;  //forste item dvs Standard er naa valgt i Cb. 
  }
  void Oppdater();

  void OppdaterMedStandarddiagram(); //tegner inn i denne kontaineren fra datakilden

  //void SetDataKilde(CDreietrykksondering *p) {DataKilde = p; Oppdater();}
  CDreietrykkopptegningEntiteter() {ValgtDiagram = 0;}
  CDreietrykkopptegningEntiteter(CDreietrykksondering* p) {
    DataKilde = p;
    ValgtDiagram = 0;
  }
};

//--------------------------------------------------------------------------
class CTotalsonderingopptegningEntiteter : public CBorprofilEntiteter {
private:
protected:
public:
  CTotalsondering *DataKilde;

  void Oppdater(){}                   //tegner inn i denne kontaineren fra datakilden
  //void SetDataKilde(CDreietrykksondering *p) {DataKilde = p; Oppdater();}
  CTotalsonderingopptegningEntiteter() {}
  CTotalsonderingopptegningEntiteter(CTotalsondering* p) {DataKilde = p;}
};

//--------------------------------------------------------------------------

class CEntPunkt : public CEntitet {
private:
protected:
public:
  double X;
  double Y;
  double Z;
  void Paint(TMapWin &MW);
  void FinnXYMaxMin(double &XMax, double &YMax, double &XMin, double &YMin)
    {XMax=max(XMax,X); YMax=max(YMax,Y); XMin=min(XMin,X); YMin=min(YMin,Y);}
  CEntPunkt() {}
  CEntPunkt(CPunkt P, int lag, TColor farge, int linjetype, int linjetypeskala)
    {X = P.x; Y = P.y; Z = 0;
     Lag = lag; Farge = farge; Linjetype = linjetype; Linjetypeskala = linjetypeskala; }
};
//--------------------------------------------------------------------------
class CLinje : public CEntitet {
private:
protected:
public:
  double StartX;
  double StartY;
  double StartZ;
  double EndX;
  double EndY;
  double EndZ;
  void Paint(TMapWin &MW);
  void FinnXYMaxMin(double &XMax, double &YMax, double &XMin, double &YMin)
     {XMax=max(XMax,max(StartX,EndX)); YMax=max(YMax,max(StartY,EndY));
      XMin=min(XMin,min(StartX,EndX)); YMin=min(YMin,min(StartY,EndY));}
  CLinje() {}
  CLinje(CPunkt Fra, CPunkt Til, int lag, TColor farge, int linjetype, int linjetypeskala)
    { StartX = Fra.x; StartY = Fra.y; EndX = Til.x; EndY = Til.y;
      Lag = lag; Farge = farge; Linjetype = linjetype; Linjetypeskala = linjetypeskala; }
};
//-------------------------------------------------------------------------
class CVertex {
private:
protected:
public:
  long int Nr;
  double X;
  double Y;
  double Z;
  bool operator<(const CVertex &a)const {return (X < a.X);}
  CVertex(double argX, double argY) {X=argX; Y=argY;}
  CVertex() {}
};
//------------------------------------------------------------------------
class CPolylinje : public CEntitet {
private:
protected:
public:
  vector<CVertex> kont;
  bool Lukket;   //hvis lukket går linje fra siste til første pkt.
  void Paint(TMapWin &MW);
  bool empty() {return kont.empty();}
  void push_back(CVertex P) {kont.push_back(P);}
  void clear(){}
  void FinnXYMaxMin(double &XMax, double &YMax, double &XMin, double &YMin);
  CPolylinje() {}
  CPolylinje(int lag, TColor farge, int linjetype, int linjetypeskala,CVertex *pVertex = NULL, int AntVertex = 0, bool lukket = false);
  ~CPolylinje(){}
};
//------------------------------------------------------------------------
typedef vector<CVertex>::iterator CVertexIterator;
//------------------------------------------------------------------------
class CArc : public CEntitet {
private:
protected:
public:
  double SenterX;
  double SenterY;
  double SenterZ;
  double Radius;
  double StartVinkel;
  double SluttVinkel;
  void FinnXYMaxMin(double &XMax, double &YMax, double &XMin, double &YMin)
    {XMax=max(XMax,SenterX); YMax=max(YMax,SenterY); XMin=min(XMin,SenterX); YMin=min(YMin,SenterY);}
  CArc() {}
  CArc(CPunkt Senter, double radius, double startvinkel, double sluttvinkel,int lag, TColor farge, int linjetype, int linjetypeskala)
    {SenterX = Senter.x; SenterY = Senter.y; SenterZ = 0; Radius = radius; StartVinkel = startvinkel, SluttVinkel = sluttvinkel;
     Lag = lag; Farge = farge; Linjetype = linjetype; Linjetypeskala = linjetypeskala; }
};
//-----------------------------------------------------------------------
class CSirkel : public CEntitet {
private:
protected:
public:
  double SenterX;
  double SenterY;
  double SenterZ;
  double Radius;
  void FinnXYMaxMin(double &XMax, double &YMax, double &XMin, double &YMin)
    {XMax=max(XMax,SenterX); YMax=max(YMax,SenterY); XMin=min(XMin,SenterX); YMin=min(YMin,SenterY);}
  CSirkel() {}
  CSirkel(CPunkt Senter, double radius, int lag, TColor farge, int linjetype, int linjetypeskala)
    {SenterX = Senter.x; SenterY = Senter.y; SenterZ = 0; Radius = radius;
     Lag = lag; Farge = farge; Linjetype = linjetype; Linjetypeskala = linjetypeskala; }
};
//----------------------------------------------------------------------
enum enStyle {Standard};
enum enJustify {Left,Center,Right,Aligned,Middle,Fit,TopLeft,TopCenter,TopRight
                ,MiddleLeft,MiddleCenter,MiddleRight,BottomLeft,BottomCenter,BottomRight};

class CText : public CEntitet {
private:
protected:
public:
  AnsiString Tekst;
  enStyle Style;
  enJustify Justify;
  double Hoyde;
  double Rotasjon;
  double Breddefaktor;
  double Obliquing;
  double X;
  double Y;
  double Z;
  bool OppNed;
  bool Baklengs;
  void FinnXYMaxMin(double &XMax, double &YMax, double &XMin, double &YMin)
    {XMax=max(XMax,X); YMax=max(YMax,Y); XMin=min(XMin,X); YMin=min(YMin,Y);}
  void Paint(TMapWin &MW);
  CText() {}
  CText(AnsiString tekst, double hoyde, CPunkt Startpkt, int lag, TColor farge, int linjetype, int linjetypeskala)
    { Tekst = tekst; Hoyde = hoyde; X = Startpkt.x; Y = Startpkt.y; Z = 0;
      Lag = lag; Farge = farge; Linjetype = linjetype; Linjetypeskala = linjetypeskala;
      Style = Standard; Justify = Left; Rotasjon = 0; Breddefaktor = 1; Obliquing = 1; OppNed = false; Baklengs = false;}
};
//-------------------------------------------------------------------------




#endif
