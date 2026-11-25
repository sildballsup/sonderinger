//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <math.h>
#include "TMapWin.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TMapWin *)
{
        new TMapWin(NULL);
}
/*
//---------------------------------------------------------------------------
__fastcall TMapWin::TMapWin(TComponent* Owner)
        : TObject(Owner)
{
}
//---------------------------------------------------------------------------
*/
namespace Tmapwin
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TMapWin)};
                 RegisterComponents("Samples", classes, 0);
        }
}

//---------------------------------------------------------------------------
//set funksjoner
//---------------------------------------------------------------------------
void __fastcall TMapWin::setimage(TImage *i) {
  fimage = i;
  if (image != NULL) {
    width = fimage->Width;
    height = fimage->Height;
  }  
}
//---------------------------------------------------------------------------
void __fastcall TMapWin::setTLX(int u) {
  ftlx = u;
}
//---------------------------------------------------------------------------
void __fastcall TMapWin::setTLY(int u) {
  ftly = u;
}
//---------------------------------------------------------------------------
void __fastcall TMapWin::setWidth(int u) {
  fbrx = ftlx+u;
  fwidth = u;
}
//---------------------------------------------------------------------------
void __fastcall TMapWin::setHeight(int u) {
  fbry = ftly+u;
  fheight = u;
}
//---------------------------------------------------------------------------
void __fastcall TMapWin::setxlow(float z) {
  fxlow = z;
  fdx = fxhigh - fxlow;
}
//---------------------------------------------------------------------------
void __fastcall TMapWin::setxhigh(float z) {
  fxhigh = z;
  fdx = fxhigh - fxlow;
}
//---------------------------------------------------------------------------
void __fastcall TMapWin::setylow(float z) {
  fylow = z;
  fdy = fyhigh - fylow;
}
//---------------------------------------------------------------------------
void __fastcall TMapWin::setyhigh(float z) {
  fyhigh = z;
  fdy = fyhigh - fylow;  
}
//---------------------------------------------------------------------------
void __fastcall TMapWin::setxlog(bool b) {
  fxlog = b;
  if (b) {                  // set log
    if (xlow==0) {
      xlow = 1e-24;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TMapWin::setylog(bool b) {
  fylog = b;
  if (b) {                 // set log
    if (ylow==0) {
      ylow = 1e-24;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TMapWin::setfont(TFont *f) {
  ffont->Assign(f);
}
//---------------------------------------------------------------------------
//get funksjoner
//---------------------------------------------------------------------------
TCanvas *TMapWin::GetCanvas() {
    return ( (image == NULL) ? NULL : image->Canvas);
}
//---------------------------------------------------------------------------
//map funksjoner
//---------------------------------------------------------------------------
int TMapWin::mapW2SxLin(float xf) {
  return floor(ftlx+(xf-fxlow)*(width)/(fxhigh-fxlow));
}
//---------------------------------------------------------------------------
int TMapWin::mapW2SyLin(float yf) {
  return floor(fbry-(yf-fylow)*(height)/(fyhigh-fylow));
}
//---------------------------------------------------------------------------
int TMapWin::mapW2SxLog(float xf) {
  return floor(ftlx+(log(xf/fxlow)/log(fxhigh/fxlow))*(width));
}
//---------------------------------------------------------------------------
int TMapWin::mapW2SyLog(float yf) {
  return fbry - floor((log(yf/fylow)/log(fyhigh/fylow))*(height));
}
//---------------------------------------------------------------------------
float TMapWin::mapS2WxLin(int xs) {
  return fxlow + (xs-ftlx)*(fxhigh-fxlow)/(width);
}
//---------------------------------------------------------------------------
float TMapWin::mapS2WyLin(int ys) {
  return fyhigh - (ys-ftly)*(fyhigh-fylow)/(height);
}
//---------------------------------------------------------------------------
float TMapWin::mapS2WxLog(int xs) {
  return fxlow*exp(((xs-ftlx)/(width))*log(fxhigh/fxlow));
}
//---------------------------------------------------------------------------
float TMapWin::mapS2WyLog(int ys) {
  return fyhigh/exp(((ys-ftly)/(height)*log(fyhigh/fylow)));
}

//---------------------------------------------------------------------------
//konstruktor og destruktor
//---------------------------------------------------------------------------
__fastcall TMapWin::TMapWin(TComponent *AOwner) : TComponent(AOwner){
  fimage = NULL;
  fdrawclip = true;
  fprotectlog = true;
  xlow = 0; ylow = 0; xhigh = 1; yhigh = 1; xlog = false; ylog = false;
  ftlx = 0; ftly = 0;
  width = 100; height = 100;
  color = TColor(clGreen);
  ffont = new TFont();
}
//---------------------------------------------------------------------------
__fastcall TMapWin::TMapWin(TComponent *AOwner, TImage *image) : TComponent(AOwner) {
  fimage = image;
  fdrawclip = true;
  fprotectlog = true;
  xlow = 0; ylow = 0; xhigh = 1; yhigh = 1; xlog = false; ylog = false;
  ftlx = 0; ftly = 0;
  color = TColor(clGreen);
  ffont = new TFont();
  if (image != NULL) {
    width = image->Width;
    height = image->Height;
  }
  else {
    width = 100; height = 100;
  }
}
//--------------------------------------------------------------------------
__fastcall TMapWin::~TMapWin() {
  delete ffont;
}
//--------------------------------------------------------------------------
// konverteringsfunksjoner
//--------------------------------------------------------------------------
void TMapWin::FloatToScreen(float xf, float yf, int &xs, int &ys) {                        //obs noen parametre er var.. betyr det &  ????
  if (((fxlog)&&(xf<0)) || ((fylog)&&(yf<0))) {
    throw TMyPlotException("TMapWin: Number for log axis smaller than zero");
  }
  xs = (fxlog ? mapW2SxLog(xf) : mapW2SxLin(xf) );
  ys = (fylog ? mapW2SyLog(yf) : mapW2SyLin(yf) );
}
//--------------------------------------------------------------------------
void TMapWin::ScreenToFloat(int xs, int ys, float &xf, float &yf) {
  xf = (fxlog ? mapS2WxLog(xs) : mapS2WxLin(xs) );
  yf = (fylog ? mapS2WyLog(ys) : mapS2WyLin(ys) );
}

//--------------------------------------------------------------------------
//tegnefunksjoner
//--------------------------------------------------------------------------
void TMapWin::clear() {
  if ((fimage)!=NULL) {
    fimage->Canvas->Pen->Color = clWhite;
    fimage->Canvas->Brush->Color = clWhite;
    fimage->Canvas->Rectangle(ftlx,ftly,fbrx,fbry);
  }
}
//--------------------------------------------------------------------------
void TMapWin::putbox(float x1, float y1, float x2, float y2) {
  int cx1,cx2,cy1,cy2;
  if ((fimage)!=NULL) {
    FloatToScreen(x1,y1,cx1,cy1);
    FloatToScreen(x2,y2,cx2,cy2);
    fimage->Canvas->Rectangle(cx1,cy1,cx2,cy2);
  }
}
//--------------------------------------------------------------------------
void TMapWin::puttext(float x, float y, AnsiString s) {
  int cx,cy;
  if ((fimage)!=NULL) {
    FloatToScreen(x,y,cx,cy);
    fimage->Canvas->Font->Assign(font);
    fimage->Canvas->TextOut(cx,cy,s);
  }
}
//--------------------------------------------------------------------------
void TMapWin::puttext(float x, float y, AnsiString s, double Texthoyde) {
  int cx,cy;
  if ((fimage)!=NULL) {
    FloatToScreen(x,y,cx,cy);
    font->Height = Texthoyde;
    fimage->Canvas->Font->Assign(font);
    fimage->Canvas->TextOut(cx,cy,s);
  }
}
//--------------------------------------------------------------------------

void TMapWin::putpixel(float x, float y) {
  int ix,iy;
  bool cx,cy;
  if (((fxlog)&&(x < 0)) || ((fylog)&&(y < 0)))  {
    if (fprotectlog) {
      exit(0);
    }
    else {
      throw TMyPlotException("Number for log axis smaller than zero");
    }
  }
  cx = cy = false;
  ix = iy = 0;
  if (x < fxlow) {ix = ftlx;   cx = true; }
  if (x > fxhigh){ix = fbrx-1; cx = true; }
  if (y < fylow) {iy = fbry-1; cy = true; }
  if (y > fyhigh){iy = ftly;   cy = true; }

  if (!cx) {
    if (fxlog) {
      ix = mapW2SxLog(x);
    }
    else {ix = mapW2SxLin(x);}
  }

  if (!cy) {
   if (fylog) {
     iy = mapW2SyLog(y);
   }
   else {iy = mapW2SyLin(y);}
  }

  if ((cx || cy) && (!fdrawclip)) {
    exit(0);
  }
  if ((fimage)!=NULL) {
    fimage->Canvas->Pixels[ix][iy] = color;
  }
}
//--------------------------------------------------------------------------
void TMapWin::putline(float x1, float y1, float x2, float y2) {
  int cx1,cx2,cy1,cy2;
  if ((fimage)!=NULL) {
    FloatToScreen(x1,y1,cx1,cy1);
    FloatToScreen(x2,y2,cx2,cy2);
    fimage->Canvas->Pen->Color = color;
    fimage->Canvas->MoveTo(cx1,cy1);
    fimage->Canvas->LineTo(cx2,cy2);
  }
}
//--------------------------------------------------------------------------
void TMapWin::VLine(float x) { // vertical line
  int ix;
  bool cx;
  if ((fxlog)&&(x < 0)) {
    if (fprotectlog) {
      exit(0);
    }
    else {
      throw TMyPlotException("Number for log axis smaller than zero");
    }//end else
  }
  cx = false;
  ix = 0;
  if (x < fxlow) {ix = ftlx;   cx = true;}
  if (x > fxhigh){ix = fbrx-1; cx = true;}
  if (!cx) {
    if (fxlog) {
      ix = mapW2SxLog(x);
    }
    else { ix = mapW2SxLin(x);}
  }
  if ((cx)&&(!fdrawclip)) { exit(0);}
  if ((fimage)!=NULL) {
    fimage->Canvas->Pen->Color = color;
    fimage->Canvas->MoveTo(ix,ftly);
    fimage->Canvas->LineTo(ix,fbry);
  }
}
//---------------------------------------------------------------------------
void TMapWin::HLine(float y) { // horizontal line
  int iy;
  bool cy;
  if ((fylog)&&(y < 0)) {
    if (fprotectlog) {
      exit(0);
    }
    else { throw TMyPlotException("Number for log axis smaller than zero");}
  }
  cy = false;
  iy = 0;
  if (y < fylow) {iy = fbry;   cy = true;}
  if (y > fyhigh){iy = ftly-1; cy = true;}
  if (!cy) {
    if (fylog) {iy = mapW2SyLog(y);}
    else {iy = mapW2SyLin(y);}
  }
  if ((cy)&&(!fdrawclip)) { exit(0);}
  if ((fimage)!=NULL) {
    fimage->Canvas->Pen->Color = color;
    fimage->Canvas->MoveTo(ftlx,iy);
    fimage->Canvas->LineTo(fbrx,iy);
  }
}

//--------------------------------------------------------------------------
//funksjoner for konv av musekoord?
//--------------------------------------------------------------------------
float TMapWin::getX(int i) {             // i:=0..width-1
  if (fxlog) {
    return mapS2WxLog(i);
  }
  else { return mapS2WxLin(i); }
}
//--------------------------------------------------------------------------
float TMapWin::getY(int i) {             // i:=0..height-1
  if (fylog) {
    return mapS2WyLog(i);
  }
  else { return mapS2WyLin(i); }
}
//--------------------------------------------------------------------------

