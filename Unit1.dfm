object Form1: TForm1
  Left = 15
  Top = 144
  Width = 990
  Height = 519
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object StringGrid1: TStringGrid
    Left = 0
    Top = 0
    Width = 982
    Height = 633
    Align = alTop
    ColCount = 14
    DefaultColWidth = 65
    FixedCols = 0
    RowCount = 2
    FixedRows = 0
    TabOrder = 0
    Visible = False
    OnDblClick = StringGrid1DblClick
    ColWidths = (
      103
      65
      65
      65
      65
      65
      65
      65
      65
      65
      65
      65
      65
      65)
  end
  object MainMenu1: TMainMenu
    Left = 192
    Top = 16
    object File1: TMenuItem
      Caption = '&File'
      object New1: TMenuItem
        Caption = '&New'
      end
      object Open1: TMenuItem
        Caption = '&Open...'
        OnClick = Open1Click
      end
      object Save1: TMenuItem
        Caption = '&Save'
      end
      object SaveAs1: TMenuItem
        Caption = 'Save &As...'
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object Print1: TMenuItem
        Caption = '&Print...'
      end
      object PrintSetup1: TMenuItem
        Caption = 'P&rint Setup...'
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'E&xit'
        OnClick = Exit1Click
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Filter = 'RDT og STD og DAT filer|*.RDT;*STD;*.RDT'
    Left = 288
    Top = 16
  end
end
