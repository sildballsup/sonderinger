//---------------------------------------------------------------------------


#include <vcl.h>
#pragma hdrstop
USERES("sonderinger.res");
USEFORM("Unit1.cpp", Form1);
USEUNIT("borpunkter.cpp");
USEFORM("Unit2.cpp", Form2);
USEUNIT("..\TMapWin\TMapWin.cpp");
USEUNIT("..\TMapWin\MapWinGraf.cpp");
USEUNIT("..\titab\elementer1.cpp");
USEUNIT("..\titab\acadentiteter.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TForm1), &Form1);
                 Application->CreateForm(__classid(TForm2), &Form2);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
