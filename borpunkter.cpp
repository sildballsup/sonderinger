//---------------------------------------------------------------------------

#include <vcl.h>
#include <iostream>
#pragma hdrstop
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <math.h>

#include "borpunkter.h"
#include "Unit1.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


//denne fila skal ineholde klasser for sonderboringsdata.
//---------------------------------------------------------------------------
// CElementer
//---------------------------------------------------------------------------
ifstream &operator>>(ifstream &fil, CBoringer &kontainer)
{
  char GmlDecimalSeparator = DecimalSeparator;
  DecimalSeparator = '.';
  //Først går jeg gjennom hele fila og finner format og typer boringer, deretter går jeg gjennom
  //fila på nytt og leser inn.
  //leser inn i en TStringList:
  TStringList *StringList1 = new TStringList;
  StringList1->Sorted = false;
  char Buf[251];
  while(!fil.eof()) {
    fil.getline(Buf, 250);
    //her bør evt \r fjernes ,dvs cariage return karakteren:
    FjernCariageReturn(Buf);
    StringList1->Add(AnsiString(Buf));
  }//end while

  //Hopper over initielle linjer som er blanke eller inneh bare ett tegn:
  const int AntTegnPrLinje = 250;
  char Linje[AntTegnPrLinje + 1];
  int LinjeNr = 0;
  strcpy(Linje, StringList1->Strings[LinjeNr].c_str());
  while((strlen(Linje)<2) && (LinjeNr < StringList1->Count)) {
    LinjeNr++;
    strcpy(Linje, StringList1->Strings[LinjeNr].c_str());
  }//end while

  //skal bestemme formatet på fila:
  kontainer.LogFormat = kontainer.BestemLogFilFormat(StringList1->Strings[LinjeNr]);
  if(kontainer.LogFormat == UkjentFormat) {
    Application->MessageBox("Ukjent filformat.  Fillesing avbrutt.",MB_OK,0);
    DecimalSeparator = GmlDecimalSeparator;
    return fil;
  }//end if

  //bestemmer type boringer.  Leter fram til headerlinjer. Alle headerlinjer har H som første tegn:
  istringstream L;
  vector<enBortype> Bortyper;             //her lagres tolkingene av type boring ved første gjennomløp.
  enBortype Bortype = UkjentBortype;
  for(;LinjeNr < StringList1->Count; LinjeNr++) {

    if(*(StringList1->Strings[LinjeNr].c_str()) != 'H') continue;

    if((kontainer.LogFormat == Geologg)) {
      strcpy(Linje, StringList1->Strings[LinjeNr].c_str());
      L.str(Linje);
      char Token[251];
      for(int i = 1;i < 4; i++) {L.getline(Token,AntTegnPrLinje,',');}   //finner den tredje verdien på linja
      int tredje = (AnsiString(Token)).ToInt();
      if(tredje==20) {Bortype = Dreietrykksondering;}
      else if(tredje==22) {Bortype = Totalsondering;}
      /* else if(tredje==10) {Bortype = Dreiesondering;}
      else if(tredje==31) {Bortype = Enkelsondering;}
      else if(tredje==60) {Bortype = Trykksondering;}
      else if(tredje==40) {Bortype = Ramsondering;}
      else if(tredje==30) {Bortype = Fjellkontrollboring;}
      else if(tredje==50) {Bortype = Vingeboring;}         */
      else {Bortype = UkjentBortype;}

      Bortyper.push_back(Bortype);
    }

    else if(kontainer.LogFormat == Geoprinter60) {               //her er jeg avh av at det ikke finnes blanke i teksten
      TStringList *StringList2 = new TStringList;
      StringList2->CommaText = StringList1->Strings[LinjeNr];
      int HM = StringList2->Values[AnsiString("HM")].ToInt();
      if(HM==23) {Bortype = Dreietrykksondering;}
      else if(HM==24) {Bortype = Totalsondering;}
      /*else if(HM==7)  {Bortype = Trykksondering;}
      else if(HM==35) {Bortype = PoretrykksutjevningVedCPT;}
      else if(HM==36) {Bortype = Fjellkontrollboring;}
      else if(HM==37) {Bortype = TotalsonderingForCPT;}    */
      else {Bortype = UkjentBortype;}

      Bortyper.push_back(Bortype);
      delete StringList2;
    }

    else if(kontainer.LogFormat == Geoprinter50) {
    //finner 3. og 8. felt i header:
      strcpy(Linje, StringList1->Strings[LinjeNr].c_str());
      L.str(Linje);
      char Token[251];
      for(int i = 1;i < 4; i++) {L.getline(Token,AntTegnPrLinje,',');}
      int tredje = (AnsiString(Token)).ToInt();
      for(int i = 1;i < 6; i++) {L.getline(Token,AntTegnPrLinje,',');}
      int femte =  (AnsiString(Token)).ToInt();
      if((tredje==6)&&(femte==5)) {Bortype = Dreietrykksondering;}
      else if(tredje==7)               {Bortype = Totalsondering;}
      /* else if     ((tredje==2)&&(femte==1)) {Bortype = Dreiesondering;}
      else if((tredje==5)&&(femte==1)) {Bortype = Enkelsondering;}
      else if((tredje==6)&&(femte==6)) {Bortype = Fjellkontrollboring;}
      else if((tredje==6)&&(femte==1)) {Bortype = Totalsondering;}     */
      else {Bortype = UkjentBortype;}

      Bortyper.push_back(Bortype);
    }
  }//end for

  //Så skal fila gjennomgåspå nytt og data leses til CBoring objekter:
  fil.clear();  //fjerner evt errorcondition i stream. hvis error virker ikke seekg() eller annet..
  fil.seekg(0);   //starter på begynnelsen av fila igjen
  vector<enBortype>::iterator p;
  CTotalsondering *pTotalsondering = 0;
  CDreietrykksondering *pDreietrykksondering = 0;
  CUkjentBortype *pUkjentBortype = 0;
  for (p=Bortyper.begin(); p != Bortyper.end(); ++p) {
    switch (*p)
    {
      case Totalsondering:
        pTotalsondering = new CTotalsondering;
        fil >> *pTotalsondering;
        kontainer.push_back(pTotalsondering);
      break;
      case Dreietrykksondering:
        pDreietrykksondering = new CDreietrykksondering;
        fil >> *pDreietrykksondering;
        kontainer.push_back(pDreietrykksondering);
      break;
      case UkjentBortype:
        Application->MessageBox("Hopper over en ukjent bortype.",MB_OK,0);
        fil >> *pUkjentBortype;
      break;

/*    case Trykksondering:
        CTrykksondering *p = new CTrykksondering;
        fil >> *bp;
        kontainer.push_back(bp);
      break;
      case Fjellkontrollsondering:
        CFjellkontroll *p= new CFjellkontroll;
        fil >> *bp;
        kontainer.push_back(bp);
      break;
      default: ;
*/
    }//end switch.
  }//end for
  delete StringList1;
  DecimalSeparator = GmlDecimalSeparator;
  return fil;
}
//---------------------------------------------------------------------------

void CBoringer::clear()
{
  CBoring *pElement = 0;
  CBoringIterator i;
  for(i=begin(); i != end(); i++)
  {
    pElement = *i;
    delete pElement;
  }//end for
  kont.clear();
}

//---------------------------------------------------------------------------

enLogFormat CBoringer::BestemLogFilFormat(AnsiString s)
{
  enLogFormat result;
  const int AntTegnPrLinje = 250;
  char Linje[AntTegnPrLinje];
  char Token[AntTegnPrLinje];

  strcpy(Linje, s.c_str());
  istringstream L;
  L.str(Linje);
  L.getline(Token,AntTegnPrLinje,',');       //plukker ut delteksten fram til første komma.
  AnsiString t(Token);
  if     (t.Pos("HAL")>0) {result = Geoprinter50;}
  else if(t.Pos('=')>0)   {result = Geoprinter60;}
  else if(t.Pos('H')>0)   {result = Geologg;}
  else {
    result = UkjentFormat;
  }
  return result;
}
//----------------------------------------------------------------------------

enLogFormat CBoring::BestemLogFilFormat(AnsiString s)
{
  enLogFormat result;
  const int AntTegnPrLinje = 250;
  char Linje[AntTegnPrLinje];
  char Token[AntTegnPrLinje];

  strcpy(Linje, s.c_str());
  istringstream L;
  L.str(Linje);
  L.getline(Token,AntTegnPrLinje,',');       //plukker ut delteksten fram til første komma.
  AnsiString t(Token);
  if     (t.Pos("HAL")>0) {result = Geoprinter50;}
  else if(t.Pos('=')>0)   {result = Geoprinter60;}
  else if(t.Pos('H')>0)   {result = Geologg;}
  else {
    result = UkjentFormat;
  }
  return result;
}
//--------------------------------------------------------------------------

ifstream &operator>>(ifstream &stream, CTotalsondering &k)
{
  const int AntTegnPrLinje = 250;
  char Linje[AntTegnPrLinje + 1];
  stream.getline(Linje, AntTegnPrLinje);
  if(stream.eof()) return stream;

  //hopper over linjer med mindre enn 2 tegn:
  while( (strlen(Linje)<2) && (!stream.eof() )) {stream.getline(Linje, AntTegnPrLinje);}

  //skal bestemme formatet på fil
  k.LogFormat = k.BestemLogFilFormat(AnsiString(Linje));
  if(k.LogFormat == UkjentFormat) {
    Application->MessageBox("Ukjent filformat.  Fillesing som totalsondering avbrutt.",MB_OK,0);
    return stream;
  }//end if

  //så leses data inn i objekt:
  istringstream L;
  char Token[251];

//GEOLOGG FORMAT:
  if(k.LogFormat == Geologg) {

    //først leses headerdatalinjen:
    L.str(Linje);
    int Nr = 1;
    int ik = 0;  //dette er en kode i fila som jeg ikke lagrer.
    bool hoyre = true; // dette er en kode i fila jeg ikke lagrer men den gir fortegn til avsett.
    for(int Nr = 1;!L.eof(); Nr++) {
      L.getline(Token,AntTegnPrLinje,',');
      if     (Nr==1) { continue; }
      else if(Nr==2) { k.Lopenr =       AnsiString(Token).ToIntDef(99999);}
      else if(Nr==3) { k.Bortype = Totalsondering;} //lagrer ikke verdien i felt 3. verdien i felt 3 skal vare 20.
      else if(Nr==4) { k.AntParamPerDatalinje = AnsiString(Token).ToIntDef(99999);}
      else if(Nr==5) { k.AntDatalinjerEtterD  = AnsiString(Token).ToIntDef(99999);}
      else if(Nr==6) { k.Serienr = AnsiString(Token).ToIntDef(99999);}
      else if(Nr==7) { k.AarManedDag = AnsiString(Token).ToIntDef(99999);}
      else if(Nr==8) { continue;}
      else if(Nr==9) { k.LopenrSondering = AnsiString(Token).ToIntDef(99999);}
      else if(Nr==10){ k.SignBorleder = AnsiString(Token);}
      else if(Nr==11){ k.Prosjektnr = AnsiString(Token).ToIntDef(99999);}
      else if(Nr==12){ k.BorpktNr = AnsiString(Token).ToIntDef(99999);}
      else if(Nr==13){ ik = AnsiString(Token).ToIntDef(99999);}
      else if(Nr==14){ continue;}
      else if(Nr==15){
        if(ik==1) { k.Profil = AnsiString(Token).ToInt();}
        else if(ik==2) { k.X = 0.01*AnsiString(Token).ToDouble() ;}
      }
      else if(Nr==16){
        if((ik==1)&&(AnsiString(Token)=="H")) {hoyre = true;}
        if((ik==1)&&(AnsiString(Token)=="V")) {hoyre = false;}
        if(ik==2) {continue;}
      }
      else if(Nr==17) {
        if(ik==1) {
          k.Avsett = 0.01*AnsiString(Token).ToDouble();
          if(!hoyre) {k.Avsett = -k.Avsett;}
        }
        if(ik==2) {k.Y = 0.01*AnsiString(Token).ToInt();}
      }
      else if(Nr==18) { k.Z = 0.01*AnsiString(Token).ToIntDef(99999);}
      else if(Nr==19) { k.Forboringsdybde = 0.01*AnsiString(Token).ToIntDef(99999);}
      else if(Nr==20) { k.Vanndybde = 0.01*AnsiString(Token).ToIntDef(99999);}
      else if(Nr==21) { k.Sonderingsdybde = 0.01*AnsiString(Token).ToIntDef(99999);}
      else if(Nr==22) {k.SerienrSonde = AnsiString(Token).ToIntDef(99999);}
    }//end for

    //så leses registreringene:
    bool Slag = false;
    double ForigeMatekraft=0;       //når det er registrert / eller % istede for matekraft på et nivå bruker jeg den forige registrerte veriden (det er slik det blir gjort i geoplot)
    double ankerdybde = -0.1;      //ja det er riktig
    while(!stream.eof()) {         //her starter løkka for innlesing av datalinjer
      int ch = stream.peek();
      switch (ch)
      {
        case 'H':
          return stream;      //da returnerer funksjonen. ferdig å lese en boring.
        case  EOF:
          return stream;      //da returnerer funksjonen. ferdig å lese en boring.
        case 'D':
          stream.getline(Token,AntTegnPrLinje,',');    //plukker vekk d-en.
          ankerdybde = ankerdybde + 0.1;          //her øker etter hvert ankerdybden.
          break;
        case '?':
          break;
        default:
          ankerdybde = ankerdybde + 0.1;          //her øker etter hvert ankerdybden.
      }//end switch


      //Kommet hit så har jeg en ren datalinje foran meg å lese inn:
      stream.getline(Linje, AntTegnPrLinje);      //her leses datalinjene.
      FjernCariageReturn(Linje);
      if(strlen(Linje)<2) {continue;}             //her hopper jeg over evt linje med bare ett tegn.
      AnsiString Felt1 = "0";
      AnsiString Felt2 = "0";
      AnsiString Felt3 = "0";
      AnsiString Felt4 = "0";
      AnsiString Felt5 = "0";
      AnsiString Felt6 = "0";
      AnsiString Felt7 = "0";
      AnsiString Felt8 = "0";

      L.clear();
      L.str(Linje);
      bool ereof = L.eof();
      for(int Nr = 1; !L.eof(); Nr++) {
        L.getline(Token,AntTegnPrLinje,',');
        if(Nr==1) { Felt1 = Token;}
        else if(Nr==2) { Felt2 = Token;}
        else if(Nr==3) { Felt3 = Token;}
        else if(Nr==4) { Felt4 = Token;}
        else if(Nr==5) { Felt5 = Token;}
        else if(Nr==6) { Felt6 = Token;}
        else if(Nr==7) { Felt7 = Token;}
        else if(Nr==8) { Felt8 = Token;}
      }//end for.
      double antsek = Felt5.ToIntDef(100000);
      if (antsek==0) {antsek=0.0001;}
      double Synkhastighet = 2/antsek;
      double Spyletrykk = 10*Felt6.ToIntDef(0);
      double rpm = Felt7.ToIntDef(0);
      double Kommentarkode =Felt8.ToIntDef(99999);

      CTotalsonderingsdata d;
      d.Dybde = ankerdybde + 0.025;
      d.Matekraft = (ErTall(Felt1)? 0.01*Felt1.ToInt() : ForigeMatekraft);
      ForigeMatekraft = d.Matekraft;
      if(Felt1=="/") {Slag = true;}
      if(Felt1=="%") {Slag = false;}
      d.Slag = Slag;
      d.Synkhastighet = Synkhastighet;
      d.rpm = rpm;
      d.Kommentarkode = Kommentarkode;
      d.Spyletrykk = Spyletrykk;
      k.push_back(d);

      d.Dybde = ankerdybde + 0.050;
      d.Matekraft = (ErTall(Felt2)? 0.01*Felt2.ToInt() : ForigeMatekraft);
      ForigeMatekraft = d.Matekraft;
      if(Felt1=="/") {Slag = true;}
      if(Felt1=="%") {Slag = false;}
      d.Slag = Slag;
      d.Synkhastighet = Synkhastighet;
      d.rpm = rpm;
      d.Kommentarkode = Kommentarkode;
      d.Spyletrykk = Spyletrykk;
      k.push_back(d);

      d.Dybde = ankerdybde + 0.075;
      d.Matekraft = (ErTall(Felt3)? 0.01*Felt3.ToInt() : ForigeMatekraft);
      ForigeMatekraft = d.Matekraft;
      if(Felt1=="/") {Slag = true;}
      if(Felt1=="%") {Slag = false;}
      d.Slag = Slag;
      d.Synkhastighet = Synkhastighet;
      d.rpm = rpm;
      d.Kommentarkode = Kommentarkode;
      d.Spyletrykk = Spyletrykk;
      k.push_back(d);

      d.Dybde = ankerdybde + 0.10;
      d.Matekraft = (ErTall(Felt4)? 0.01*Felt4.ToInt() : ForigeMatekraft);
      ForigeMatekraft = d.Matekraft;
      if(Felt1=="/") {Slag = true;}
      if(Felt1=="%") {Slag = false;}
      d.Slag = Slag;
      d.Synkhastighet = Synkhastighet;
      d.rpm = rpm;
      d.Kommentarkode = Kommentarkode;
      d.Spyletrykk = Spyletrykk;

      k.push_back(d);

    }//end while
  }//end if på Geologg

//GEOPRINTER50 FORMAT:
  else if(k.LogFormat == Geoprinter50) {
    //først leses headerdatalinjen:
    L.str(Linje);
    int Nr = 1;
    int ik = 0;  //dette er en kode i fila som jeg ikke lagrer.
    for(int Nr = 1;!L.eof(); Nr++) {
      L.getline(Token,AntTegnPrLinje,',');
      if     (Nr==1) { continue; }
      else if(Nr==2) { k.Lopenr =       AnsiString(Token).ToInt();}
      else if(Nr==3) { k.Bortype = Totalsondering; } //data i felt 3 lagres ikke.  Data i felt 3 skal være 06 for dreietrykksond for dette formatet.
      else if(Nr==4) { k.Forboringsdybde = AnsiString(Token).ToDouble();}
      else if(Nr==5) { k.AarManedDag  = AnsiString(Token).ToInt();}
      else if(Nr==6) { continue;}
      else if(Nr==7) { k.Metodeparameter2 = AnsiString(Token).ToInt();}  //skaleringsfaktor for matekraft.
      else if(Nr==8) { continue;} //lagres ikke.  Skal være 05 for dreietrykk for dette formatet.
      else if(Nr==9) { ik = AnsiString(Token).ToInt();}
      else if(Nr==10){ k.Prosjektnr = AnsiString(Token);}
      else if(Nr==11){
        if(ik==0) {continue;}
        else if(ik==1) {k.Profil = AnsiString(Token).ToDouble();}
        else if(ik==2) {k.X      = AnsiString(Token).ToDouble();}
        else if(ik==3) {k.LopenrSondering = AnsiString(Token).ToInt();}
      }
      else if(Nr==12) {
        if(ik==0) {continue;}
        else if(ik==1) {k.Avsett = AnsiString(Token).ToDouble();}
        else if(ik==2) {k.Y =      AnsiString(Token).ToDouble();}
        else if(ik==3) {continue; }
      }
      else if(Nr==13) {k.Z = AnsiString(Token).ToDouble();}
    }//end for

    //så leses registreringene:
    double ankerdybde = 0;
    double gmlankerdybde = 0;
    double antsek = 0;  //en mellomvariabel for å unngå div med null i ber av hastighet
    while(!stream.eof()) {         //her starter løkka for innlesing av datalinjer
      int ch = stream.peek();
      switch (ch)
      {
        case 'H':
          return stream;      //da returnerer funksjonen. ferdig å lese en boring.
        case  EOF:
          return stream;      //da returnerer funksjonen. ferdig å lese en boring.
      }//end switch

      //Kommet hit så har jeg en ren datalinje foran meg å lese inn:
      stream.getline(Linje, AntTegnPrLinje);      //her leses datalinjene.
      if(strlen(Linje)<2) {continue;}             //her hopper jeg over evt linje med bare ett tegn.
      AnsiString Felt1 = "0";
      AnsiString Felt2 = "0";
      AnsiString Felt3 = "0";
      AnsiString Felt4 = "0";
      AnsiString Felt5 = "0";
      AnsiString Felt6 = "0";
      AnsiString Felt7 = "0";
      AnsiString Felt8 = "0";
      AnsiString Felt9 = "0";

      L.clear();
      L.str(Linje);
      for(int Nr = 1; !L.eof(); Nr++) {
        L.getline(Token,AntTegnPrLinje,',');
        if(Nr==1) { Felt1 = Token;}
        if(Nr==2) { Felt2 = Token;}
        if(Nr==3) { Felt3 = Token;}
        if(Nr==4) { Felt4 = Token;}
        if(Nr==5) { Felt5 = Token;}
        if(Nr==6) { Felt6 = Token;}
        if(Nr==7) { Felt7 = Token;}
        if(Nr==8) { Felt8 = Token;}
        if(Nr==9) { Felt9 = Token;}
      }//end for.
      double rpm = Felt8.ToDouble();
      double Kommentarkode;
      if ((Felt9 == "?")||(Felt9 == "0")) Kommentarkode = 99999;      //dummy
      else Kommentarkode = Felt9.ToInt();
      gmlankerdybde = ankerdybde;                                      //alt dette med gmlankerdybde, synkifelt6 og stoppdybde er for å få med den spesielle tolkingen på siste linje der i boringen.
      ankerdybde = Felt1.ToDouble();
      bool SynkIFelt6 = ((ankerdybde - gmlankerdybde)<0.2);
      double stoppdybde = 0;
      if(SynkIFelt6) {stoppdybde = gmlankerdybde + Felt6.ToDouble();}

      CTotalsonderingsdata d;
      //første reg, tolker felt2:
      if(Felt2.ToInt()<144) {
        d.Dybde = ankerdybde -0.16;
        d.Matekraft = k.Metodeparameter2 * Felt2.ToInt();
        d.Synkhastighet = Felt7.ToInt();
        d.Spyletrykk = 0;
      }
      else {
        d.Matekraft = 0;
        antsek = Felt2.ToIntDef(100144) - 144;
        if(antsek==0) {antsek = 0.001;}
        d.Synkhastighet = 2/antsek;     //litt usikker på beregningen her.  feltdata er sekunder..
        d.Spyletrykk = Felt7.ToInt();
      }
      d.rpm = rpm;
      d.Kommentarkode = Kommentarkode;
      d.Moment = 0; //registreres ikke i dette formatet;
      if((!SynkIFelt6)||(d.Dybde <= stoppdybde)) {k.push_back(d);}

      //andre reg, tolker felt3:
      if(Felt3.ToInt()<144) {
        d.Dybde = ankerdybde -0.12;
        d.Matekraft = k.Metodeparameter2 * Felt3.ToInt();
        d.Synkhastighet = Felt7.ToInt();
        d.Spyletrykk = 0;
      }
      else {
        d.Matekraft = 0;
        antsek = Felt3.ToIntDef(100144) - 144;
        if(antsek==0) {antsek = 0.001;}
        d.Synkhastighet = 2/antsek;     //litt usikker på beregningen her.  feltdata er sekunder..
        d.Spyletrykk = Felt7.ToInt();
      }
      d.Moment = 0;
      d.rpm = rpm;
      d.Kommentarkode = Kommentarkode;
      if((!SynkIFelt6)||(d.Dybde <= stoppdybde)) {k.push_back(d);}

      //tredje reg, tolker felt4:
      if(Felt4.ToInt()<144) {
        d.Dybde = ankerdybde -0.08;
        d.Matekraft = k.Metodeparameter2 * Felt4.ToInt();
        d.Synkhastighet = Felt7.ToInt();
        d.Spyletrykk = 0;
      }
      else {
        d.Matekraft = 0;
        antsek = Felt4.ToIntDef(100144) - 144;
        if(antsek==0) {antsek = 0.001;}
        d.Synkhastighet = 2/antsek;     //litt usikker på beregningen her.  feltdata er sekunder..
        d.Spyletrykk = Felt7.ToInt();
      }
      d.Moment = 0;
      d.rpm = rpm;
      d.Kommentarkode = Kommentarkode;
      if((!SynkIFelt6)||(d.Dybde <= stoppdybde)) {k.push_back(d);}

      //fjerde reg, tolker felt5:
      if(Felt5.ToInt()<144) {
        d.Dybde = ankerdybde -0.04;
        d.Matekraft = k.Metodeparameter2 * Felt5.ToInt();
        d.Synkhastighet = Felt7.ToInt();
        d.Spyletrykk = 0;
      }
      else {
        d.Matekraft = 0;
        antsek = Felt5.ToIntDef(100144) - 144;
        if(antsek==0) {antsek = 0.001;}
        d.Synkhastighet = 2/antsek;     //litt usikker på beregningen her.  feltdata er sekunder..
        d.Spyletrykk = Felt7.ToInt();
      }
      d.Moment = 0;
      d.rpm = rpm;
      d.Kommentarkode = Kommentarkode;
      if((!SynkIFelt6)||(d.Dybde <= stoppdybde)) {k.push_back(d);}

      //femte reg, tolker felt6:
      if(Felt6.ToInt()<144) {
        d.Dybde = ankerdybde -0.00;
        d.Matekraft = k.Metodeparameter2 * Felt6.ToInt();
        d.Synkhastighet = Felt7.ToInt();
        d.Spyletrykk = 0;
      }
      else {
        d.Matekraft = 0;
        antsek = Felt6.ToIntDef(100144) - 144;
        if(antsek==0) {antsek = 0.001;}
        d.Synkhastighet = 2/(Felt6.ToInt() - 144);     //litt usikker på beregningen her.  feltdata er sekunder..
        d.Spyletrykk = Felt7.ToInt();
      }
      d.Moment = 0;
      d.rpm = rpm;
      d.Kommentarkode = Kommentarkode;
      if((!SynkIFelt6)||(d.Dybde <= stoppdybde)) {k.push_back(d);}
    }//end while
  }//end else  på Geoprinter50

//GEOPRINTER60 FORMAT:
  else if(k.LogFormat == Geoprinter60) {
    //først leses headerdatalinjen:
    TStringList *StringList1 = new TStringList;
    StringList1->Sorted = false;
    StringList1->CommaText = AnsiString(Linje);
    k.Lopenr =          StringList1->Values["HA"].ToIntDef(0);
    k.LopenrSondering = StringList1->Values["HB"].ToIntDef(0);
    k.Serienr =         StringList1->Values["HC"];
    k.AarManedDag =     StringList1->Values["HD"].ToIntDef(0);
    k.Bortype = Totalsondering;
    k.Oppdragsgiver =   StringList1->Values["HE"];
    k.Firmanavn =       StringList1->Values["HF"];
    k.Grunnvannsniva = (ErTall(StringList1->Values["HG"]) ? StringList1->Values["HG"].ToDouble()  : 0);
    k.Avsett =         (ErTall(StringList1->Values["HH"]) ? StringList1->Values["HH"].ToDouble()  : 0);
    if (k.Avsett == 0) {
      k.Avsett =       (ErTall(StringList1->Values["HV"]) ? -StringList1->Values["HV"].ToDouble() : 0);
    }//end if
    k.Starttidspunkt = (ErTall(StringList1->Values["HI"]) ? StringList1->Values["HI"].ToDouble()  : 0);
    k.Oppdragsnr =      StringList1->Values["HJ"];
    k.BorpktNr =        StringList1->Values["HK"];//.ToIntDef(0);
    k.Profil =          (ErTall(StringList1->Values["HL"]) ? StringList1->Values["HL"].ToDouble() : 0);
    k.SerienrSonde =    StringList1->Values["HN"].ToIntDef(0);
    k.Forboringsdybde =(ErTall(StringList1->Values["HO"])  ? StringList1->Values["HO"].ToDouble() : 0);
    k.Vanndybde =      (ErTall(StringList1->Values["HP"])  ? StringList1->Values["HP"].ToDouble() : 0);
    k.SignBorleder =    StringList1->Values["HQ"];
    k.FriTekst =        StringList1->Values["HT"];
    k.Barometertrykk  =(ErTall(StringList1->Values["HU"])  ? StringList1->Values["HU"].ToDouble() : 0);
    k.Koordinatsystem = StringList1->Values["HW"];
    k.X =              (ErTall(StringList1->Values["HX"])  ? StringList1->Values["HX"].ToDouble() : 0);
    k.Y =              (ErTall(StringList1->Values["HY"])  ? StringList1->Values["HY"].ToDouble() : 0);
    k.Z =              (ErTall(StringList1->Values["HZ"])  ? StringList1->Values["HZ"].ToDouble() : 0);

    //så leses registreringene:

    while(!stream.eof()) {         //her starter løkka for innlesing av datalinjer
      int ch = stream.peek();
      switch (ch)
      {
        case 'H':
          return stream;      //da returnerer funksjonen. ferdig å lese en boring.
        case  EOF:
          return stream;      //da returnerer funksjonen. ferdig å lese en boring.
      }//end switch

      //Kommet hit så har jeg en ren datalinje foran meg å lese inn:
      stream.getline(Linje, AntTegnPrLinje);      //her leses datalinjene.
      if(strlen(Linje)<2) {continue;}             //her hopper jeg over evt linje med bare ett tegn.
      StringList1->Clear();
      StringList1->CommaText = AnsiString(Linje);
      CTotalsonderingsdata d;
      d.Dybde =         StringList1->Values["D"].ToDouble();
      double debugdybde = StringList1->Values["D"].ToDouble();
      d.Matekraft =     (ErTall(StringList1->Values["A"])? StringList1->Values["A"].ToDouble() : 0);
      d.Synkhastighet = (ErTall(StringList1->Values["B"]) ?StringList1->Values["B"].ToDouble() : 0);
      d.rpm =           StringList1->Values["R"].ToDouble();
      d.Kommentarkode = (ErTall(StringList1->Values["K"])? StringList1->Values["K"].ToDouble() : 99999); //99999 som dummy hvis kommentar mangler
      d.Spyletrykk =    (ErTall(StringList1->Values["I"])? StringList1->Values["I"].ToDouble() : 0  );
      if(StringList1->Values["C"] ==  "-0.23107")
        { int a = 1;}
      if((ErTall(StringList1->Values["C"]))&&(StringList1->Values["C"] != 0) ) {d.Synkhastighet = 200/(StringList1->Values["C"].ToIntDef(100000));}   //ToIntDef legger inn en dummy i tilfelle negative tall, ikke helt bra behandling av problemet men..
      d.Spyling =       StringList1->Values["AR"].ToIntDef(0);
      d.OktRotasjon =   StringList1->Values["AQ"].ToIntDef(0);
      d.Slag =          StringList1->Values["AP"].ToIntDef(1);
      d.Moment =        StringList1->Values["V"].ToIntDef(0);

      k.push_back(d);
    }//end while på løkke for inlesing av datalinjer
  }//end else på Geoprinter60

  return stream;
}
//---------------------------------------------------------------------------

ifstream &operator>>(ifstream &stream, CDreietrykksondering &k)
{
  const int AntTegnPrLinje = 250;
  char Linje[AntTegnPrLinje + 1];
  stream.getline(Linje, AntTegnPrLinje);
  if(stream.eof()) return stream;

  //hopper over linjer med mindre enn 2 tegn:
  FjernCariageReturn(Linje);
  while( (strlen(Linje)<2) && (!stream.eof() )) {stream.getline(Linje, AntTegnPrLinje);   FjernCariageReturn(Linje);}

  //skal bestemme formatet på fil
  k.LogFormat = k.BestemLogFilFormat(AnsiString(Linje));
  if(k.LogFormat == UkjentFormat) {
    Application->MessageBox("Ukjent filformat.  Fillesing som totalsondering avbrutt.",MB_OK,0);
    return stream;
  }//end if

  //så leses data inn i objekt:
  istringstream L;
  char Token[251];

//GEOLOGG FORMAT:
  if(k.LogFormat == Geologg) {

    //først leses headerdatalinjen:
    L.str(Linje);
    int Nr = 1;
    int ik = 0;  //dette er en kode i fila som jeg ikke lagrer.
    bool hoyre = true; // dette er en kode i fila jeg ikke lagrer men den gir fortegn til avsett.
    for(int Nr = 1;!L.eof(); Nr++) {
      L.getline(Token,AntTegnPrLinje,',');
      if     (Nr==1) { continue; }
      else if(Nr==2) { k.Lopenr =       AnsiString(Token).ToIntDef(99999);}
      else if(Nr==3) { k.Bortype = Dreietrykksondering;} //lagrer ikke verdien i felt 3. verdien i felt 3 skal vare 20.
      else if(Nr==4) { k.AntParamPerDatalinje = AnsiString(Token).ToIntDef(99999);}
      else if(Nr==5) { k.AntDatalinjerEtterD  = AnsiString(Token).ToIntDef(99999);}
      else if(Nr==6) { k.Serienr = AnsiString(Token).ToIntDef(99999);}
      else if(Nr==7) { k.AarManedDag = AnsiString(Token).ToIntDef(99999);}
      else if(Nr==8) { continue;}
      else if(Nr==9) { k.LopenrSondering = AnsiString(Token).ToIntDef(99999);}
      else if(Nr==10){ k.SignBorleder = AnsiString(Token);}
      else if(Nr==11){ k.Prosjektnr = AnsiString(Token).ToIntDef(99999);}
      else if(Nr==12){ k.BorpktNr = AnsiString(Token).ToIntDef(99999);}
      else if(Nr==13){ ik = AnsiString(Token).ToInt();}
      else if(Nr==14){ continue;}
      else if(Nr==15){
        if(ik==1) { k.Profil = AnsiString(Token).ToInt();}
        else if(ik==2) { k.X = 0.01*AnsiString(Token).ToDouble() ;}
      }
      else if(Nr==16){
        if((ik==1)&&(AnsiString(Token)=="H")) {hoyre = true;}
        if((ik==1)&&(AnsiString(Token)=="V")) {hoyre = false;}
        if(ik==2) {continue;}
      }
      else if(Nr==17) {
        if(ik==1) {
          k.Avsett = 0.01*AnsiString(Token).ToDouble();
          if(!hoyre) {k.Avsett = -k.Avsett;}
        }
        if(ik==2) {k.Y = 0.01*AnsiString(Token).ToInt();}
      }
      else if(Nr==18) { k.Z = 0.01*AnsiString(Token).ToIntDef(99999);}
      else if(Nr==19) { k.Forboringsdybde = 0.01*AnsiString(Token).ToIntDef(99999);}
      else if(Nr==20) { k.Vanndybde = 0.01*AnsiString(Token).ToIntDef(99999);}
      else if(Nr==21) { k.Sonderingsdybde = 0.01*AnsiString(Token).ToIntDef(99999);}
      else if(Nr==22) {k.SerienrSonde = AnsiString(Token).ToIntDef(99999);}
    }//end for

    //så leses registreringene:
    double ForigeMatekraft=0;       //når det er registrert / eller % istede for matekraft på et nivå bruker jeg den forige registrerte veriden (det er slik det blir gjort i geoplot)
    double ankerdybde = -0.1;      //ja det er riktig
    while(!stream.eof()) {         //her starter løkka for innlesing av datalinjer
      int ch = stream.peek();
      switch (ch)
      {
        case 'H':
          return stream;      //da returnerer funksjonen. ferdig å lese en boring.
        case  EOF:
          return stream;      //da returnerer funksjonen. ferdig å lese en boring.
        case 'D':
          stream.getline(Token,AntTegnPrLinje,',');    //plukker vekk d-en.
          ankerdybde = ankerdybde + 0.1;          //her øker etter hvert ankerdybden.
          break;
        case '?':
          break;
        default:
          ankerdybde = ankerdybde + 0.1;          //her øker etter hvert ankerdybden.
      }//end switch


      //Kommet hit så har jeg en ren datalinje foran meg å lese inn:
      stream.getline(Linje, AntTegnPrLinje);      //her leses datalinjene.
      FjernCariageReturn(Linje);
      if(strlen(Linje)<2) {continue;}             //her hopper jeg over evt linje med bare ett tegn.
      AnsiString Felt1 = "0";
      AnsiString Felt2 = "0";
      AnsiString Felt3 = "0";
      AnsiString Felt4 = "0";
      AnsiString Felt5 = "0";
      AnsiString Felt6 = "0";
      AnsiString Felt7 = "0";
      AnsiString Felt8 = "0";

      L.clear();
      L.str(Linje);
      for(int Nr = 1; !L.eof(); Nr++) {
        L.getline(Token,AntTegnPrLinje,',');
        if(Nr==1) { Felt1 = Token;}
        if(Nr==2) { Felt2 = Token;}
        if(Nr==3) { Felt3 = Token;}
        if(Nr==4) { Felt4 = Token;}
        if(Nr==5) { Felt5 = Token;}
        if(Nr==6) { Felt6 = Token;}
        if(Nr==7) { Felt7 = Token;}
        if(Nr==8) { Felt8 = Token;}
      }//end for.
      double Synkhastighet = Felt5.ToInt();
      double Moment = Felt6.ToInt();
      double rpm = Felt7.ToInt();
      double Kommentarkode;
      if (Felt8 == "?") Kommentarkode = 99999;      //dummy
      else Kommentarkode = Felt8.ToInt();

      CDreietrykkdata d;

      d.Dybde = ankerdybde + 0.025;
      d.Matekraft = (ErTall(Felt1)? 0.01*Felt1.ToInt() : ForigeMatekraft);
      ForigeMatekraft = d.Matekraft;
      d.Synkhastighet = Synkhastighet;
      d.rpm = rpm;
      d.Kommentarkode = Kommentarkode;
      d.Moment = Moment;
      k.push_back(d);

      d.Dybde = ankerdybde + 0.050;
      d.Matekraft = (ErTall(Felt2)? 0.01*Felt2.ToInt() : ForigeMatekraft);
      ForigeMatekraft = d.Matekraft;
      d.Synkhastighet = Synkhastighet;
      d.rpm = rpm;
      d.Kommentarkode = Kommentarkode;
      d.Moment = Moment;
      k.push_back(d);

      d.Dybde = ankerdybde + 0.075;
      d.Matekraft = (ErTall(Felt3)? 0.01*Felt3.ToInt() : ForigeMatekraft);
      ForigeMatekraft = d.Matekraft;
      d.Synkhastighet = Synkhastighet;
      d.rpm = rpm;
      d.Kommentarkode = Kommentarkode;
      d.Moment = Moment;
      k.push_back(d);

      d.Dybde = ankerdybde + 0.10;
      d.Matekraft = (ErTall(Felt4)? 0.01*Felt4.ToInt() : ForigeMatekraft);
      ForigeMatekraft = d.Matekraft;
      d.Synkhastighet = Synkhastighet;
      d.rpm = rpm;
      d.Kommentarkode = Kommentarkode;
      d.Moment = Moment;
      k.push_back(d);

    }//end while
  }//end if på Geologg

//GEOPRINTER50 FORMAT:
  else if(k.LogFormat == Geoprinter50) {
    //først leses headerdatalinjen:
    L.str(Linje);
    int Nr = 1;
    int ik = 0;  //dette er en kode i fila som jeg ikke lagrer.
    for(int Nr = 1;!L.eof(); Nr++) {
      L.getline(Token,AntTegnPrLinje,',');
      if     (Nr==1) { continue; }
      else if(Nr==2) { k.Lopenr =       AnsiString(Token).ToInt();}
      else if(Nr==3) { k.Bortype = Dreietrykksondering; } //data i felt 3 lagres ikke.  Data i felt 3 skal være 06 for dreietrykksond for dette formatet.
      else if(Nr==4) { k.Forboringsdybde = AnsiString(Token).ToDouble();}
      else if(Nr==5) { k.AarManedDag  = AnsiString(Token).ToInt();}
      else if(Nr==6) { continue;}
      else if(Nr==7) { k.Metodeparameter2 = AnsiString(Token).ToInt();}  //skaleringsfaktor for matekraft.
      else if(Nr==8) { continue;} //lagres ikke.  Skal være 05 for dreietrykk for dette formatet.
      else if(Nr==9) { ik = AnsiString(Token).ToInt();}
      else if(Nr==10){ k.Prosjektnr = AnsiString(Token);}
      else if(Nr==11){
        if(ik==0) {continue;}
        else if(ik==1) {k.Profil = AnsiString(Token).ToDouble();}
        else if(ik==2) {k.X      = AnsiString(Token).ToDouble();}
        else if(ik==3) {k.LopenrSondering = AnsiString(Token).ToInt();}
      }
      else if(Nr==12) {
        if(ik==0) {continue;}
        else if(ik==1) {k.Avsett = AnsiString(Token).ToDouble();}
        else if(ik==2) {k.Y =      AnsiString(Token).ToDouble();}
        else if(ik==3) {continue; }
      }
      else if(Nr==13) {k.Z = AnsiString(Token).ToDouble();}
    }//end for

    //så leses registreringene:
    double ankerdybde = 0;
    while(!stream.eof()) {         //her starter løkka for innlesing av datalinjer
      int ch = stream.peek();
      switch (ch)
      {
        case 'H':
          return stream;      //da returnerer funksjonen. ferdig å lese en boring.
        case  EOF:
          return stream;      //da returnerer funksjonen. ferdig å lese en boring.
      }//end switch

      //Kommet hit så har jeg en ren datalinje foran meg å lese inn:
      stream.getline(Linje, AntTegnPrLinje);      //her leses datalinjene.
      FjernCariageReturn(Linje);
      if(strlen(Linje)<2) {continue;}             //her hopper jeg over evt linje med bare ett tegn.
      AnsiString Felt1 = "0";
      AnsiString Felt2 = "0";
      AnsiString Felt3 = "0";
      AnsiString Felt4 = "0";
      AnsiString Felt5 = "0";
      AnsiString Felt6 = "0";
      AnsiString Felt7 = "0";
      AnsiString Felt8 = "0";
      AnsiString Felt9 = "0";

      L.clear();
      L.str(Linje);
      for(int Nr = 1; !L.eof(); Nr++) {
        L.getline(Token,AntTegnPrLinje,',');
        if(Nr==1) { Felt1 = Token;}
        if(Nr==2) { Felt2 = Token;}
        if(Nr==3) { Felt3 = Token;}
        if(Nr==4) { Felt4 = Token;}
        if(Nr==5) { Felt5 = Token;}
        if(Nr==6) { Felt6 = Token;}
        if(Nr==7) { Felt7 = Token;}
        if(Nr==8) { Felt8 = Token;}
        if(Nr==9) { Felt9 = Token;}
      }//end for.
      double rpm = Felt8.ToDouble();
      double Kommentarkode;
        if ((Felt9 == "?")||(Felt9 == "0")) Kommentarkode = 99999;      //dummy
      else Kommentarkode = Felt9.ToInt();
      ankerdybde = Felt1.ToDouble();

      CDreietrykkdata d;

      d.Dybde = ankerdybde -0.16;
      d.Matekraft = k.Metodeparameter2 * Felt2.ToInt();
      d.Synkhastighet = Felt7.ToInt();
      d.rpm = rpm;
      d.Kommentarkode = Kommentarkode;
      d.Moment = 0; //registreres ikke i dette formatet;
      k.push_back(d);

      d.Dybde = ankerdybde -0.12;
      d.Matekraft = k.Metodeparameter2 * Felt3.ToInt();
      d.Synkhastighet = Felt7.ToInt();
      d.Moment = 0;
      d.rpm = rpm;
      d.Kommentarkode = Kommentarkode;
      k.push_back(d);

      d.Dybde = ankerdybde -0.08;
      d.Matekraft = k.Metodeparameter2 * Felt4.ToInt();
      d.Synkhastighet = Felt7.ToInt();
      d.Moment = 0;
      d.rpm = rpm;
      d.Kommentarkode = Kommentarkode;
      k.push_back(d);

      d.Dybde = ankerdybde -0.04;
      d.Matekraft = k.Metodeparameter2 * Felt5.ToInt();
      d.Synkhastighet = Felt7.ToInt();
      d.Moment = 0;
      d.rpm = rpm;
      d.Kommentarkode = Kommentarkode;
      k.push_back(d);

      d.Dybde = ankerdybde -0.00;
      d.Matekraft = k.Metodeparameter2 * Felt6.ToInt();
      d.Synkhastighet = Felt7.ToInt();
      d.Moment = 0;
      d.rpm = rpm;
      d.Kommentarkode = Kommentarkode;
      k.push_back(d);
    }//end while
  }//end else  på Geoprinter50

//GEOPRINTER60 FORMAT:
  else if(k.LogFormat == Geoprinter60) {
    //først leses headerdatalinjen:
    TStringList *StringList1 = new TStringList;
    StringList1->Sorted = false;
    StringList1->CommaText = AnsiString(Linje);

    k.Lopenr =          StringList1->Values["HA"].ToIntDef(0);
    k.LopenrSondering = StringList1->Values["HB"].ToIntDef(0);
    k.Serienr =         StringList1->Values["HC"];
    k.AarManedDag =     StringList1->Values["HD"].ToIntDef(0);
    k.Bortype = Dreietrykksondering;
    k.Oppdragsgiver =   StringList1->Values["HE"];
    k.Firmanavn =       StringList1->Values["HF"];
    k.Grunnvannsniva = (ErTall(StringList1->Values["HG"]) ? StringList1->Values["HG"].ToDouble()  : 0);
    k.Avsett =         (ErTall(StringList1->Values["HH"]) ? StringList1->Values["HH"].ToDouble()  : 0);
    if (k.Avsett == 0) {
      k.Avsett =       (ErTall(StringList1->Values["HV"]) ? -StringList1->Values["HV"].ToDouble() : 0);
    }//end if
    k.Starttidspunkt = (ErTall(StringList1->Values["HI"]) ? StringList1->Values["HI"].ToDouble()  : 0);
    k.Oppdragsnr =      StringList1->Values["HJ"];
    k.BorpktNr =        StringList1->Values["HK"];//.ToIntDef(0);
    k.Profil =          (ErTall(StringList1->Values["HL"]) ? StringList1->Values["HL"].ToDouble() : 0);
    k.SerienrSonde =    StringList1->Values["HN"].ToIntDef(0);
    k.Forboringsdybde =(ErTall(StringList1->Values["HO"])  ? StringList1->Values["HO"].ToDouble() : 0);
    k.Vanndybde =      (ErTall(StringList1->Values["HP"])  ? StringList1->Values["HP"].ToDouble() : 0);
    k.SignBorleder =    StringList1->Values["HQ"];
    k.FriTekst =        StringList1->Values["HT"];
    k.Barometertrykk  =(ErTall(StringList1->Values["HU"])  ? StringList1->Values["HU"].ToDouble() : 0);
    k.Koordinatsystem = StringList1->Values["HW"];
    k.X =              (ErTall(StringList1->Values["HX"])  ? StringList1->Values["HX"].ToDouble() : 0);
    k.Y =              (ErTall(StringList1->Values["HY"])  ? StringList1->Values["HY"].ToDouble() : 0);
    k.Z =              (ErTall(StringList1->Values["HZ"])  ? StringList1->Values["HZ"].ToDouble() : 0);

    //så leses registreringene:
    while(!stream.eof()) {         //her starter løkka for innlesing av datalinjer
      int ch = stream.peek();
      switch (ch)
      {
        case 'H':
          return stream;      //da returnerer funksjonen. ferdig å lese en boring.
        case  EOF:
          return stream;      //da returnerer funksjonen. ferdig å lese en boring.
      }//end switch

      //Kommet hit så har jeg en ren datalinje foran meg å lese inn:
      stream.getline(Linje, AntTegnPrLinje);      //her leses datalinjene.
      FjernCariageReturn(Linje);
      if(strlen(Linje)<2) {continue;}             //her hopper jeg over evt linje med bare ett tegn.
      StringList1->Clear();
      StringList1->CommaText = AnsiString(Linje);
      CDreietrykkdata d;
      d.Matekraft =     StringList1->Values["A"].ToDouble();
      d.Dybde =         StringList1->Values["D"].ToDouble();
      d.Matekraft =     (ErTall(StringList1->Values["A"])? StringList1->Values["A"].ToDouble() : 0);
      d.Synkhastighet = (ErTall(StringList1->Values["B"]) ?StringList1->Values["B"].ToDouble() : 0);
      d.rpm =           StringList1->Values["R"].ToDouble();
      d.Kommentarkode = (ErTall(StringList1->Values["K"])? StringList1->Values["K"].ToDouble() : 99999); //99999 som dummy hvis kommentar mangler
      d.OktRotasjon =   StringList1->Values["AQ"].ToIntDef(0);
      d.Moment =        StringList1->Values["V"].ToIntDef(0);

      k.push_back(d);
    }//end while på løkke for inlesing av datalinjer
  }//end else på Geoprinter60

  return stream;
}
//---------------------------------------------------------------------------

ifstream &operator>>(ifstream &stream, CUkjentBortype &k)
{
  const int AntTegnPrLinje = 250;
  char Linje[AntTegnPrLinje + 1];
  stream.getline(Linje, AntTegnPrLinje);
  if(stream.eof()) return stream;

//hopper over linjer med mindre enn 2 tegn:
  while( (strlen(Linje)<2) && (!stream.eof() )) {
    stream.getline(Linje, AntTegnPrLinje);
    }//end while

//hopper over de andre linjene:
  while(!stream.eof()) {
    int ch = stream.peek();
    switch (ch)
    {
      case 'H':
        return stream;      //da returnerer funksjonen. ferdig å lese en boring.
      case  EOF:
        return stream;      //da returnerer funksjonen. ferdig å lese en boring.
      default:
        stream.getline(Linje, AntTegnPrLinje);
    }//end switch
  }//end while
}
//---------------------------------------------------------------------------
bool ErTall(const AnsiString &s)
{
 try {
   double a = s.ToDouble();
 }
 catch (EConvertError &err) {
   return false;
 }
 return true;
}
//---------------------------------------------------------------------------

void CBoringer::FyllStringGrid(TStringGrid *StringGrid)     //denne sender data til borpunktliste
{
  //sletter først alt innhold i stringgridden:
  int I, J;
  for (I = 0; I < StringGrid->ColCount; I++)
    for (J = 1; J < StringGrid->RowCount; J++)
      StringGrid->Cells[I][J] = " ";
  //fyller inn i kolonneheaderene:
      StringGrid->Cells[0][0] = "Metode navn";
      StringGrid->Cells[1][0] = "Hullnummer";
      StringGrid->Cells[2][0] = "Løpenummer";
      StringGrid->Cells[3][0] = "Profil";
      StringGrid->Cells[4][0] = "Avsett";
      StringGrid->Cells[5][0] = "X";
      StringGrid->Cells[6][0] = "Y";
      StringGrid->Cells[7][0] = "Z";
      StringGrid->Cells[8][0] = "Forboret";
      StringGrid->Cells[9][0] = "Løsmasser";
      StringGrid->Cells[10][0] = "Fjell";
      StringGrid->Cells[11][0] = "Sluttkode";
      StringGrid->Cells[12][0] = "Dato";

  //leser så inn i stringgridden:
  CBoring *pBoring = 0;
  CBoringIterator i;
  int linjenr = 1;
  for(i=begin(); i != end(); i++, linjenr++)
  {
    pBoring = *i;
    StringGrid->Cells[0][linjenr] =   GiStringForBortype((*pBoring).Bortype);
    StringGrid->Cells[1][linjenr] =   ( (*pBoring).BorpktNr == 99999) ? AnsiString(".....") : AnsiString((*pBoring).BorpktNr);
    StringGrid->Cells[2][linjenr] =   IntToStr((*pBoring).LopenrSondering);
    StringGrid->Cells[3][linjenr] =   FloatToStrF((*pBoring).Profil,ffFixed,15,3);
    StringGrid->Cells[4][linjenr] =   FloatToStrF((*pBoring).Avsett,ffFixed,15,3);
    StringGrid->Cells[5][linjenr] =   FloatToStrF((*pBoring).X,ffFixed,15,3);
    StringGrid->Cells[6][linjenr] =   FloatToStrF((*pBoring).Y,ffFixed,15,3);
    StringGrid->Cells[7][linjenr] =   FloatToStrF((*pBoring).Z,ffFixed,15,3);
    StringGrid->Cells[8][linjenr] =   FloatToStrF((*pBoring).Forboringsdybde,ffFixed,15,3);
    StringGrid->Cells[9][linjenr] =   FloatToStrF((*pBoring).GiBoretDybdeILosmasser(),ffFixed,15,3);
    StringGrid->Cells[10][linjenr] =   FloatToStrF((*pBoring).GiBoretLengdeIFjell(),ffFixed,15,3);
    AnsiString s = ((*pBoring).GiSluttkode()== 99999) ? AnsiString(".....") : AnsiString((*pBoring).GiSluttkode() );
    StringGrid->Cells[11][linjenr] =  s;
    StringGrid->Cells[12][linjenr] =  IntToStr((*pBoring).AarManedDag);

  }//end for
  if (size()>0) {
    StringGrid->RowCount = size() + 2;
//    StringGrid->FixedRows = 1;
  }
  else {
    StringGrid->RowCount = 1;
    StringGrid->FixedRows = 0;
  }
}

//--------------------------------------------------------------------------
bool SammenlignBortypeFunk(CBoring* a, CBoring* b) {
  return (b->Bortype > a->Bortype);
}
//--------------------------------------------------------------------------
bool SammenlignBorpktNrFunk(CBoring *a, CBoring *b) {
  if (ErTall(a->BorpktNr)&& ErTall(b->BorpktNr))
  {
    return ( b->BorpktNr.ToInt() > a->BorpktNr.ToInt() );
  }
  else return (a->BorpktNr > b->BorpktNr);
}
//--------------------------------------------------------------------------
bool SammenlignLopenrFunk(CBoring* a, CBoring* b) {
  return (b->LopenrSondering > a->LopenrSondering);
}
//--------------------------------------------------------------------------
bool SammenlignBorpktProfil(CBoring* a, CBoring* b) {
  return (b->Profil > a->Profil);
}
//--------------------------------------------------------------------------
bool SammenlignBorpktAvsett(CBoring* a, CBoring* b) {
  return (b->Avsett > a->Avsett);
}
//--------------------------------------------------------------------------
bool SammenlignBorpktX(CBoring* a, CBoring* b) {
  return (b->X > a->X);
}
//--------------------------------------------------------------------------
bool SammenlignBorpktY(CBoring* a, CBoring* b) {
  return (b->Y > a->Y);
}
//--------------------------------------------------------------------------
bool SammenlignBorpktZ(CBoring* a, CBoring* b) {
  return (b->Z > a->Z);
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

AnsiString GiStringForBortype(enBortype t)
{
  switch (t)
  {
    case UkjentBortype:   return "UkjentBortype";
    case Dreiesondering:  return "Dreiesondering";
    case Proveserie:      return "Proveserie";
    case Provebelastning: return "Provebelastning";
    case Enkelsondering:  return "Enkelsondering";
    case Dreietrykksondering: return "Dreietrykksondering";
    case Trykksondering:  return "Trykksondering";
    case Skruplateforsok: return "Skruplateforsok";
    case Ramsondering:    return "Ramsondering";
    case Setningsmaling:  return "Setningsmaling";
    case SPT:             return "SPT";
    case Fjellkontrollboring: return "Fjellkontrollboring";
    case Poretrykksmaling: return "Poretrykksmaling";
    case InSituPermeabilitetsmaling: return "InSituPermeabilitetsmaling";
    case Vingeboring:     return "Vingeboring";
    case ElektriskSondering: return "ElektriskSondering";
    case Helningsmaling:   return "Helningsmaling";
    case Totalsondering:   return "Totalsondering";
    case PoretrykksutjevningVedCPT: return "PoretrykksutjevningVedCPT";
    case TotalsonderingForCPT: return "TotalsonderingForCPT";

    default : return "UkjentBortype" ;
  }//end switch
}
//--------------------------------------------------------------------

double CTotalsondering::GiDybdeTilFjell()
{
  CTotalsonderingsdata Totalsonderingsdata;
  CTotalsonderingsdataIterator i;
  bool IFjell = false;
  double Fjelldybde = 99999;
  for(i=begin(); i != end(); i++)
  {
    Totalsonderingsdata = *i;
    if((Totalsonderingsdata.Kommentarkode == 41)&&(GiSluttkode()==94)) {
      IFjell = true;
      Fjelldybde = Totalsonderingsdata.Dybde;
    }
    if(Totalsonderingsdata.Kommentarkode == 42) {
      IFjell = false;
      Fjelldybde = 99999;
    }
  }
  if(IFjell == true) return Fjelldybde;
  else if(GiSluttkode()==93) return GiBoretDybde();
  else return 99999;
}
//---------------------------------------------------------------------
double CTotalsondering::GiBoretLengdeIFjell()
{
  return (GiDybdeTilFjell()==99999)? 0 : (GiBoretDybde() - GiDybdeTilFjell() );
}
//---------------------------------------------------------------------
void CDreietrykksondering::FyllStringGrid(TStringGrid* StringGrid)
{
  //sletter først alt innhold i stringgridden:
  int I, J;
  for (I = 0; I < StringGrid->ColCount; I++) {
    for (J = 1; J < StringGrid->RowCount; J++) {
      StringGrid->Cells[I][J] = " ";
    }
  }
  //fyller inn i kolonneheaderene:
  StringGrid->Cells[1][0] = "Dybde(m)";
  StringGrid->Cells[2][0] = "Kraft(kN)";
  StringGrid->Cells[3][0] = "Bortid(sek/m)";
  StringGrid->Cells[4][0] = "Rotasjon(o/min)";
  StringGrid->Cells[5][0] = "Kode";

  //leser så inn i stringgridden:
  CDreietrykkdataIterator d;
  int linjenr = 1;
  for(d=begin(); d != end(); d++, linjenr++)
  {
    StringGrid->Cells[0][linjenr] =   IntToStr(linjenr);
    StringGrid->Cells[1][linjenr] =   FloatToStrF(d->Dybde,ffFixed,15,2);
    StringGrid->Cells[2][linjenr] =   FloatToStrF(d->Matekraft,ffFixed,15,1);
    StringGrid->Cells[3][linjenr] =   FloatToStrF(d->Synkhastighet,ffFixed,15,1);
    StringGrid->Cells[4][linjenr] =   FloatToStrF(d->rpm,ffFixed,15,0);
    int komment = (d->Kommentarkode==99999)? 0 : d->Kommentarkode;
    StringGrid->Cells[5][linjenr] =   IntToStr(komment);
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
//---------------------------------------------------------------------
void CTotalsondering::FyllStringGrid(TStringGrid* StringGrid)
{
  //sletter først alt innhold i stringgridden:
  int I, J;
  for (I = 0; I < StringGrid->ColCount; I++) {
    for (J = 1; J < StringGrid->RowCount; J++) {
      StringGrid->Cells[I][J] = " ";
    }
  }
  //fyller inn i kolonneheaderene:
  StringGrid->Cells[1][0] = "Dybde(m)";
  StringGrid->Cells[2][0] = "Kraft(kN)";
  StringGrid->Cells[3][0] = "Bortid(sek/m)";
  StringGrid->Cells[4][0] = "Spyletrykk(Mpa)";
  StringGrid->Cells[5][0] = "Rotasjon(o/min)";
  StringGrid->Cells[6][0] = "Kode";

  //leser så inn i stringgridden:
  CTotalsonderingsdataIterator d;
  int linjenr = 1;
  float bortid;
  for(d=begin(); d != end(); d++, linjenr++)
  {
    StringGrid->Cells[0][linjenr] =   IntToStr(linjenr);
    StringGrid->Cells[1][linjenr] =   FloatToStrF(d->Dybde,ffFixed,15,2);
    StringGrid->Cells[2][linjenr] =   FloatToStrF(d->Matekraft,ffFixed,15,1);
    bortid = (d->Synkhastighet == 0 ? 1000 : 1000/(d->Synkhastighet));
    StringGrid->Cells[3][linjenr] =   FloatToStrF(bortid,ffFixed,15,1);
    StringGrid->Cells[4][linjenr] =   FloatToStrF(d->Spyletrykk,ffFixed,15,2);
    StringGrid->Cells[5][linjenr] =   FloatToStrF(d->rpm,ffFixed,15,0);
    int komment = (d->Kommentarkode==99999)? 0 : d->Kommentarkode;
    StringGrid->Cells[6][linjenr] =   IntToStr(komment);
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
//---------------------------------------------------------------------
void FjernCariageReturn(char *pc)
{
  char c[251];
  int ic = 0;
  int ipc = 0;
  while ( pc[ipc] != '\0' ) {
    if( pc[ipc] != '\r') {
      c[ic] = pc[ipc];
      ic++;
    }
    ipc++;
  }//end while
  c[ic] = '\0';
  strcpy(pc,c);
}
//----------------------------------------------------------------------


