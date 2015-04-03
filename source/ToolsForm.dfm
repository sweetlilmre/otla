object ToolsFrm: TToolsFrm
  Left = 414
  Top = 190
  Width = 385
  Height = 306
  Caption = 'Settings'
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
    Left = 96
    Top = 217
    Width = 81
    Height = 25
    Caption = 'OK'
    TabOrder = 0
    OnClick = AceptarBtnClick
  end
  object CancelBtn: TButton
    Left = 216
    Top = 217
    Width = 89
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    TabOrder = 1
    OnClick = CancelBtnClick
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 353
    Height = 113
    Caption = 'Directories'
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
    Top = 120
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
  object OpenDialog1: TOpenDialog
    Left = 24
    Top = 216
  end
end
