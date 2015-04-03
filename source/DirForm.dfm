object DirFrm: TDirFrm
  Left = 329
  Top = 185
  BorderStyle = bsDialog
  Caption = 'Output dir...'
  ClientHeight = 260
  ClientWidth = 253
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object DriveComboBox1: TDriveComboBox
    Left = 16
    Top = 16
    Width = 217
    Height = 19
    DirList = DirectoryListBox1
    TabOrder = 0
    TextCase = tcUpperCase
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
    Left = 16
    Top = 224
    Width = 81
    Height = 25
    Caption = 'OK'
    TabOrder = 2
    OnClick = AceptarBtnClick
  end
  object CancelBtn: TButton
    Left = 136
    Top = 224
    Width = 89
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    TabOrder = 3
    OnClick = CancelBtnClick
  end
end
