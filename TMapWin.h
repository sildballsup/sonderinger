//---------------------------------------------------------------------------

#ifndef TMapWinH
#define TMapWinH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

class TMyPlotException : public Exception {
public:
  TMyPlotException(char *c) : Exception(c) {};  
};
//---------------------------------------------------------------------------
//type
  //float=extended;    hvanåenn det betyr.. det stodi delphikoden.. men jeg har erstattet de med float
//---------------------------------------------------------------------------
  
class PACKAGE TMapWin : public TComponent
{
private:
   TImage *fimage;
   int ftlx,ftly,fbrx,fbry,fwidth,fheight;
   float fxlow,fxhigh,fylow,fyhigh,fdx,fdy;
   bool fdrawclip,fxlog,fylog,fprotectlog;
   TColor fcolor;
   TFont *ffont;
protected:
   void __fastcall setimage(TImage *i);
   void __fastcall setTLX(int u);
   void __fastcall setTLY(int u);
   void __fastcall setWidth(int u);
   void __fastcall setHeight(int u);
   void __fastcall setxlow(float z);
   void __fastcall setxhigh(float z);
   void __fastcall setylow(float z);
   void __fastcall setyhigh(float z);
   void __fastcall setxlog(bool b);
   void __fastcall setylog(bool b);
   void __fastcall setfont(TFont *f);
   int mapW2SxLin(float xf);
   int mapW2SyLin(float yf);
   int mapW2SxLog(float xf);
   int mapW2SyLog(float yf);
   float mapS2WxLin(int xs);
   float mapS2WyLin(int ys);
   float mapS2WxLog(int xs);
   float mapS2WyLog(int ys);
public:
   virtual __fastcall TMapWin(TComponent *AOwner);
   virtual __fastcall TMapWin(TComponent *AOwner, TImage *image);    // must exist or NIL
   virtual __fastcall ~TMapWin();
   virtual void FloatToScreen(float xf, float yf, int &xs, int &ys);
   virtual void ScreenToFloat(int xs, int ys, float &xf, float &yf);
   void clear();
   void putbox(float x1, float y1, float x2, float y2);
   void puttext(float x, float y, AnsiString s);
   void puttext(float x, float y, AnsiString s, double Texthoyde);   
   virtual void putpixel(float x, float y);
   virtual void putline(float x1, float y1, float x2, float y2);     // not yet
   virtual void VLine(float x);                                      // vertical line
   virtual void HLine(float y);                                      // horizontal line
   float getX(int i);                                                // i:=0..width-1
   float getY(int i);                                                // i:=0..height-1
   TCanvas *GetCanvas();
__published:
   __property TImage *image = {read = fimage,write = setimage};
   __property int imleft = {read = ftlx,write = setTLX};         // with respect to image
   __property int imtop =  {read = ftly,write = setTLY};         // with respect to image
   __property int width = {read = fwidth,write = setWidth};      // with respect to image
   __property int height = {read = fheight,write = setHeight};   // with respect to image
   __property float xlow = {read = fxlow,write = setxlow};
   __property float ylow = {read = fylow,write = setylow};
   __property float xhigh = {read = fxhigh,write = setxhigh};
   __property float yhigh = {read = fyhigh,write = setyhigh};
   __property float dx = {read = fdx};
   __property float dy = {read = fdy};
   __property bool drawclipped = {read = fdrawclip,write = fdrawclip};
   __property bool xlog = {read = fxlog,write = setxlog};
   __property bool ylog = {read = fylog,write = setylog};
   __property bool protectlog = {read = fprotectlog,write = fprotectlog};
   __property TColor color = {read = fcolor, write = fcolor};
   __property TFont *font = {read = ffont, write = setfont};
};
//---------------------------------------------------------------------------
#endif

