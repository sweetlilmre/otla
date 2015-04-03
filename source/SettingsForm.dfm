object SettingsFrm: TSettingsFrm
  Left = 394
  Top = 146
  BorderStyle = bsDialog
  Caption = 'Settings'
  ClientHeight = 466
  ClientWidth = 369
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
  object AceptarBtn: TButton
    Left = 72
    Top = 417
    Width = 81
    Height = 25
    Caption = 'OK'
    TabOrder = 0
    OnClick = AceptarBtnClick
  end
  object CancelBtn: TButton
    Left = 200
    Top = 417
    Width = 89
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    TabOrder = 1
    OnClick = CancelBtnClick
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 64
    Width = 353
    Height = 113
    Caption = 'Directories for .SBB files'
    TabOrder = 2
    object Label1: TLabel
      Left = 24
      Top = 16
      Width = 45
      Height = 13
      Caption = 'Zx output'
    end
    object Label3: TLabel
      Left = 24
      Top = 40
      Width = 52
      Height = 13
      Caption = 'Cpc output'
    end
    object Label4: TLabel
      Left = 24
      Top = 64
      Width = 52
      Height = 13
      Caption = 'Msx output'
    end
    object Label8: TLabel
      Left = 24
      Top = 88
      Width = 42
      Height = 13
      Caption = '81output'
    end
    object DirZXEdt: TEdit
      Left = 96
      Top = 16
      Width = 209
      Height = 21
      TabOrder = 0
      Text = 'DirZXEdt'
    end
    object DirZXBtn: TButton
      Left = 312
      Top = 16
      Width = 25
      Height = 17
      Caption = '...'
      TabOrder = 1
      OnClick = DirZXBtnClick
    end
    object DirCPCEdt: TEdit
      Left = 96
      Top = 40
      Width = 209
      Height = 21
      TabOrder = 2
      Text = 'DirCPCEdt'
    end
    object DirCPCBtn: TButton
      Left = 312
      Top = 40
      Width = 25
      Height = 17
      Caption = '...'
      TabOrder = 3
      OnClick = DirCPCBtnClick
    end
    object DirMSXEdt: TEdit
      Left = 96
      Top = 64
      Width = 209
      Height = 21
      TabOrder = 4
      Text = 'DirMSXEdt'
    end
    object DirMSXBtn: TButton
      Left = 312
      Top = 64
      Width = 25
      Height = 17
      Caption = '...'
      TabOrder = 5
      OnClick = DirMSXBtnClick
    end
    object Dir81Edt: TEdit
      Left = 96
      Top = 88
      Width = 209
      Height = 21
      TabOrder = 6
      Text = 'Dir81Edt'
    end
    object Dir81Btn: TButton
      Left = 312
      Top = 88
      Width = 25
      Height = 17
      Caption = '...'
      TabOrder = 7
      OnClick = Dir81BtnClick
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 176
    Width = 353
    Height = 89
    Caption = 'Tools'
    TabOrder = 3
    object Label2: TLabel
      Left = 8
      Top = 24
      Width = 81
      Height = 13
      Caption = 'MSX ROM=>BIN'
    end
    object Label5: TLabel
      Left = 48
      Top = 48
      Width = 43
      Height = 13
      Caption = 'MSX dsk'
    end
    object Label6: TLabel
      Left = 48
      Top = 72
      Width = 41
      Height = 13
      Caption = 'CPC tool'
    end
    object Rom2BinEdt: TEdit
      Left = 96
      Top = 16
      Width = 209
      Height = 21
      TabOrder = 0
      Text = 'Rom2BinEdt'
    end
    object Rom2BinBtn: TButton
      Left = 312
      Top = 16
      Width = 25
      Height = 17
      Caption = '...'
      TabOrder = 1
      OnClick = Rom2BinBtnClick
    end
    object DiskMgrEdt: TEdit
      Left = 96
      Top = 40
      Width = 209
      Height = 21
      TabOrder = 2
      Text = 'DiskMgrEdt'
    end
    object DiskMngrBtn: TButton
      Left = 312
      Top = 40
      Width = 25
      Height = 17
      Caption = '...'
      TabOrder = 3
      OnClick = DiskMngrBtnClick
    end
    object CPCToolEdt: TEdit
      Left = 96
      Top = 64
      Width = 209
      Height = 21
      TabOrder = 4
      Text = 'CPCToolEdt'
    end
    object CPCToolBtn: TButton
      Left = 312
      Top = 64
      Width = 25
      Height = 17
      Caption = '...'
      TabOrder = 5
      OnClick = CPCToolBtnClick
    end
  end
  object HexadecimalChkBx: TCheckBox
    Left = 8
    Top = 40
    Width = 289
    Height = 17
    Caption = 'View some numbers in hexadecimal'
    TabOrder = 4
  end
  object GroupBox3: TGroupBox
    Left = 8
    Top = 272
    Width = 353
    Height = 137
    Caption = 'Wav settings'
    TabOrder = 5
    object Label7: TLabel
      Left = 8
      Top = 24
      Width = 46
      Height = 13
      Caption = 'Output dir'
    end
    object Label9: TLabel
      Left = 8
      Top = 88
      Width = 26
      Height = 13
      Caption = 'Lame'
    end
    object Label10: TLabel
      Left = 8
      Top = 112
      Width = 35
      Height = 13
      Caption = 'BitRate'
    end
    object WavDirEdt: TEdit
      Left = 64
      Top = 24
      Width = 241
      Height = 21
      TabOrder = 0
      Text = 'WavDirEdt'
    end
    object LameEdt: TEdit
      Left = 64
      Top = 80
      Width = 241
      Height = 21
      TabOrder = 1
      Text = 'LameEdt'
    end
    object DirBtn: TButton
      Left = 312
      Top = 24
      Width = 25
      Height = 17
      Caption = '...'
      TabOrder = 2
      OnClick = DirBtnClick
    end
    object LameBtn: TButton
      Left = 312
      Top = 80
      Width = 25
      Height = 17
      Caption = '...'
      TabOrder = 3
      OnClick = LameBtnClick
    end
    object BitRateCmbBx: TComboBox
      Left = 64
      Top = 104
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
    object ForceCubicChkBx: TCheckBox
      Left = 176
      Top = 104
      Width = 169
      Height = 17
      Caption = 'Force Cubic Wavform for mp3'
      TabOrder = 5
    end
    object InfoChkBx: TCheckBox
      Left = 64
      Top = 48
      Width = 281
      Height = 17
      Caption = 'Complete output file name with wav parameters info'
      TabOrder = 6
    end
  end
  object ExtensionChkBx: TCheckBox
    Left = 8
    Top = 16
    Width = 345
    Height = 17
    Caption = 'Associate .sbb extension with otla.exe'
    TabOrder = 6
  end
  object OpenDialog1: TOpenDialog
    Left = 16
    Top = 88
  end
end
