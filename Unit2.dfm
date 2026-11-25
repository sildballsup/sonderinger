object Form2: TForm2
  Left = 228
  Top = 179
  Width = 743
  Height = 480
  Caption = 'Form2'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 408
    Top = 16
    Width = 61
    Height = 13
    Caption = 'Velg diagram'
  end
  object TabbedNotebook1: TTabbedNotebook
    Left = 0
    Top = 32
    Width = 385
    Height = 369
    TabFont.Charset = DEFAULT_CHARSET
    TabFont.Color = clBtnText
    TabFont.Height = -11
    TabFont.Name = 'MS Sans Serif'
    TabFont.Style = []
    TabOrder = 0
    object TTabPage
      Left = 4
      Top = 24
      Caption = 'Generelt'
      object GroupBox1: TGroupBox
        Left = 0
        Top = 184
        Width = 377
        Height = 153
        Caption = 'Diverse'
        TabOrder = 0
        object Label9: TLabel
          Left = 8
          Top = 16
          Width = 38
          Height = 13
          Caption = 'Prosjekt'
        end
        object Label10: TLabel
          Left = 8
          Top = 56
          Width = 45
          Height = 13
          Caption = 'Sluttkode'
        end
        object Label11: TLabel
          Left = 248
          Top = 16
          Width = 55
          Height = 13
          Caption = 'Oppdragsnr'
        end
        object Edit7: TEdit
          Left = 8
          Top = 32
          Width = 113
          Height = 21
          TabOrder = 0
          Text = 'Edit7'
        end
        object ComboBox3: TComboBox
          Left = 8
          Top = 72
          Width = 361
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 1
          Items.Strings = (
            '90 avsluttet på ønsket dybde uten stopp'
            '91'
            '92'
            '93'
            '94'
            '95'
            '96'
            '97'
            ' ')
        end
        object Edit8: TEdit
          Left = 248
          Top = 32
          Width = 121
          Height = 21
          TabOrder = 2
          Text = 'Edit8'
        end
      end
      object GroupBox2: TGroupBox
        Left = 0
        Top = 8
        Width = 121
        Height = 169
        Caption = 'Koordinat'
        TabOrder = 1
        object Label6: TLabel
          Left = 8
          Top = 24
          Width = 7
          Height = 13
          Caption = 'X'
        end
        object Label7: TLabel
          Left = 8
          Top = 56
          Width = 7
          Height = 13
          Caption = 'Y'
        end
        object Label8: TLabel
          Left = 40
          Top = 88
          Width = 7
          Height = 13
          Caption = 'Z'
        end
        object Edit1: TEdit
          Left = 32
          Top = 16
          Width = 81
          Height = 21
          TabOrder = 0
          Text = 'Edit1'
        end
        object Edit2: TEdit
          Left = 32
          Top = 48
          Width = 81
          Height = 21
          TabOrder = 1
          Text = 'Edit2'
        end
        object Edit3: TEdit
          Left = 64
          Top = 80
          Width = 49
          Height = 21
          TabOrder = 2
          Text = 'Edit3'
        end
        object Button6: TButton
          Left = 8
          Top = 104
          Width = 81
          Height = 25
          Caption = 'Pek i kart'
          TabOrder = 3
        end
        object Button7: TButton
          Left = 8
          Top = 136
          Width = 81
          Height = 25
          Caption = 'Beregn fra P/A'
          TabOrder = 4
        end
      end
      object GroupBox3: TGroupBox
        Left = 128
        Top = 8
        Width = 113
        Height = 169
        Caption = 'Diverse'
        TabOrder = 2
        object Label2: TLabel
          Left = 8
          Top = 40
          Width = 23
          Height = 13
          Caption = 'Dato'
        end
        object Label12: TLabel
          Left = 8
          Top = 96
          Width = 39
          Height = 13
          Caption = 'Forboret'
        end
        object ComboBox2: TComboBox
          Left = 8
          Top = 56
          Width = 89
          Height = 21
          ItemHeight = 13
          TabOrder = 0
          Text = 'ComboBox2'
        end
        object Edit6: TEdit
          Left = 56
          Top = 88
          Width = 41
          Height = 21
          TabOrder = 1
          Text = 'Edit6'
        end
      end
      object GroupBox4: TGroupBox
        Left = 248
        Top = 8
        Width = 129
        Height = 169
        Caption = 'Profil/ avsett'
        TabOrder = 3
        object Label3: TLabel
          Left = 8
          Top = 24
          Width = 23
          Height = 13
          Caption = 'Profil'
        end
        object Label4: TLabel
          Left = 8
          Top = 56
          Width = 30
          Height = 13
          Caption = 'Avsett'
        end
        object Label5: TLabel
          Left = 8
          Top = 120
          Width = 73
          Height = 13
          Caption = 'Profillinje: ingen'
        end
        object Edit4: TEdit
          Left = 48
          Top = 16
          Width = 73
          Height = 21
          TabOrder = 0
          Text = 'Edit4'
        end
        object Edit5: TEdit
          Left = 48
          Top = 48
          Width = 73
          Height = 21
          TabOrder = 1
          Text = 'Edit5'
        end
        object Button8: TButton
          Left = 8
          Top = 80
          Width = 113
          Height = 25
          Caption = 'Beregn fra koordinat'
          TabOrder = 2
        end
        object Button9: TButton
          Left = 8
          Top = 136
          Width = 113
          Height = 25
          Caption = 'Velg profillinje'
          TabOrder = 3
        end
      end
    end
    object TTabPage
      Left = 4
      Top = 24
      Caption = 'Data'
      object StringGrid1: TStringGrid
        Left = 0
        Top = 0
        Width = 377
        Height = 341
        Align = alClient
        ColCount = 7
        DefaultColWidth = 52
        DefaultRowHeight = 15
        TabOrder = 0
        ColWidths = (
          52
          52
          52
          52
          52
          52
          52)
      end
    end
  end
  object ComboBox1: TComboBox
    Left = 392
    Top = 32
    Width = 281
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 1
    OnChange = ComboBox1Change
    Items.Strings = (
      '')
  end
  object Panel1: TPanel
    Left = 392
    Top = 56
    Width = 281
    Height = 345
    Caption = 'Panel1'
    TabOrder = 2
    object Image1: TImage
      Left = 1
      Top = 1
      Width = 279
      Height = 344
      Align = alTop
    end
  end
  object Button1: TButton
    Left = 8
    Top = 416
    Width = 105
    Height = 25
    Caption = 'OK lagre'
    TabOrder = 3
    OnClick = Button2Click
  end
  object Button2: TButton
    Left = 128
    Top = 416
    Width = 75
    Height = 25
    Caption = 'Avbryt'
    TabOrder = 4
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 312
    Top = 416
    Width = 75
    Height = 25
    Caption = 'Tegn i plan'
    TabOrder = 5
  end
  object Button4: TButton
    Left = 600
    Top = 416
    Width = 75
    Height = 25
    Caption = 'Hjelp'
    TabOrder = 6
  end
  object Button5: TButton
    Left = 512
    Top = 416
    Width = 75
    Height = 25
    Caption = 'Tegn diagram'
    TabOrder = 7
  end
  object ScrollBar1: TScrollBar
    Left = 672
    Top = 56
    Width = 17
    Height = 345
    Kind = sbVertical
    PageSize = 0
    TabOrder = 8
    OnChange = ScrollBar1Change
  end
  object MapWin1: TMapWin
    image = Image1
    width = 279
    height = 344
    xlow = -100
    ylow = 19
    xhigh = 1500
    yhigh = -1
    drawclipped = True
    protectlog = True
    color = clGreen
    font.Charset = ANSI_CHARSET
    font.Color = clWindowText
    font.Height = -11
    font.Name = 'Arial'
    font.Style = []
    Left = 624
    Top = 368
  end
end
