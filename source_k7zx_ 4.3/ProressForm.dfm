object ProgresoFrm: TProgresoFrm
  Left = 212
  Top = 195
  Width = 598
  Height = 367
  Caption = 'Converting files...'
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
    Left = 32
    Top = 8
    Width = 32
    Height = 13
    Caption = 'Label1'
  end
  object InfoLbl: TLabel
    Left = 288
    Top = 8
    Width = 32
    Height = 13
    Caption = 'InfoLbl'
  end
  object Button1: TButton
    Left = 248
    Top = 304
    Width = 81
    Height = 25
    Caption = '-'
    TabOrder = 0
    OnClick = Button1Click
  end
  object ProgressBar1: TProgressBar
    Left = 16
    Top = 8
    Width = 249
    Height = 25
    Min = 0
    Max = 100
    TabOrder = 1
  end
  object FilesLV: TListView
    Left = 0
    Top = 48
    Width = 585
    Height = 249
    Columns = <
      item
        Caption = 'file'
        Width = 280
      end
      item
        Caption = 'program'
      end
      item
        Caption = 'usr'
      end
      item
        Caption = 'clear'
      end
      item
        Caption = 'blocks'
      end
      item
        Caption = 'length'
      end
      item
        Caption = 'res'
      end>
    GridLines = True
    Items.Data = {
      2E0000000100000000000000FFFFFFFFFFFFFFFF040000000000000001610162
      016301640165FFFFFFFFFFFFFFFF}
    ReadOnly = True
    TabOrder = 2
    ViewStyle = vsReport
  end
end
