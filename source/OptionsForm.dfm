object OptionsFrm: TOptionsFrm
  Left = 294
  Top = 118
  Width = 387
  Height = 232
  Caption = 'Options'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 40
    Width = 46
    Height = 13
    Caption = 'Output dir'
  end
  object Label2: TLabel
    Left = 8
    Top = 104
    Width = 26
    Height = 13
    Caption = 'Lame'
  end
  object Label3: TLabel
    Left = 8
    Top = 128
    Width = 35
    Height = 13
    Caption = 'BitRate'
  end
  object WavDirEdt: TEdit
    Left = 64
    Top = 40
    Width = 265
    Height = 21
    TabOrder = 0
    Text = 'WavDirEdt'
  end
  object LameEdt: TEdit
    Left = 64
    Top = 96
    Width = 265
    Height = 21
    TabOrder = 1
    Text = 'LameEdt'
  end
  object DirBtn: TButton
    Left = 344
    Top = 40
    Width = 25
    Height = 17
    Caption = '...'
    TabOrder = 2
    OnClick = DirBtnClick
  end
  object LameBtn: TButton
    Left = 344
    Top = 96
    Width = 25
    Height = 17
    Caption = '...'
    TabOrder = 3
    OnClick = LameBtnClick
  end
  object BitRateCmbBx: TComboBox
    Left = 64
    Top = 120
    Width = 105
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 4
    Items.Strings = (
      '160'
      '192'
      '224'
      '256'
      '320')
  end
  object AceptarBtn: TButton
    Left = 96
    Top = 159
    Width = 81
    Height = 25
    Caption = 'OK'
    TabOrder = 5
    OnClick = AceptarBtnClick
  end
  object CancelBtn: TButton
    Left = 200
    Top = 159
    Width = 89
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    TabOrder = 6
    OnClick = CancelBtnClick
  end
  object ForceCubicChkBx: TCheckBox
    Left = 176
    Top = 120
    Width = 185
    Height = 17
    Caption = 'Force Cubic Wavform for mp3'
    TabOrder = 7
  end
  object HexadecimalChkBx: TCheckBox
    Left = 8
    Top = 8
    Width = 289
    Height = 17
    Caption = 'View some numbers in hexadecimal'
    TabOrder = 8
  end
  object InfoChkBx: TCheckBox
    Left = 64
    Top = 64
    Width = 209
    Height = 17
    Caption = 'Put info in output file name'
    TabOrder = 9
  end
  object OpenDialog1: TOpenDialog
    Filter = '*.exe|*.exe'
    Left = 24
    Top = 152
  end
end
