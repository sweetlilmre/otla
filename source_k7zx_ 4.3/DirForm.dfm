object DirFrm: TDirFrm
  Left = 222
  Top = 158
  Width = 261
  Height = 316
  Caption = 'Output dir...'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object DriveComboBox1: TDriveComboBox
    Left = 16
    Top = 16
    Width = 217
    Height = 19
    TabOrder = 0
  end
  object DirectoryListBox1: TDirectoryListBox
    Left = 16
    Top = 40
    Width = 209
    Height = 169
    ItemHeight = 16
    TabOrder = 1
  end
  object AceptarBtn: TButton
    Left = 40
    Top = 232
    Width = 81
    Height = 25
    Caption = 'OK'
    TabOrder = 2
    OnClick = AceptarBtnClick
  end
  object CancelBtn: TButton
    Left = 136
    Top = 232
    Width = 89
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    TabOrder = 3
    OnClick = CancelBtnClick
  end
end
