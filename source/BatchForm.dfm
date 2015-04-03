object BatchFrm: TBatchFrm
  Left = 318
  Top = 255
  BorderStyle = bsDialog
  Caption = 'BatchFrm'
  ClientHeight = 345
  ClientWidth = 321
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
  object InputLbl: TLabel
    Left = 8
    Top = 48
    Width = 38
    Height = 13
    Caption = 'InputLbl'
  end
  object OutputLbl: TLabel
    Left = 8
    Top = 96
    Width = 46
    Height = 13
    Caption = 'Output dir'
  end
  object Label2: TLabel
    Left = 8
    Top = 8
    Width = 32
    Height = 13
    Caption = 'Label2'
  end
  object Memo1: TMemo
    Left = 0
    Top = 190
    Width = 321
    Height = 138
    Align = alBottom
    Lines.Strings = (
      'Memo1')
    ReadOnly = True
    TabOrder = 8
  end
  object ProccedBtn: TButton
    Left = 16
    Top = 144
    Width = 81
    Height = 25
    Caption = 'Procced'
    TabOrder = 0
    OnClick = ProccedBtnClick
  end
  object InputDirEdt: TEdit
    Left = 8
    Top = 64
    Width = 241
    Height = 21
    TabOrder = 4
    Text = 'InputDirEdt'
  end
  object InputDirBtn: TButton
    Left = 268
    Top = 56
    Width = 25
    Height = 17
    Caption = '...'
    TabOrder = 5
    OnClick = InputDirBtnClick
  end
  object OutputDirEdt: TEdit
    Left = 8
    Top = 112
    Width = 241
    Height = 21
    TabOrder = 6
    Text = 'OutputDirEdt'
  end
  object OutputDirBtn: TButton
    Left = 268
    Top = 104
    Width = 25
    Height = 17
    Caption = '...'
    TabOrder = 7
    OnClick = OutputDirBtnClick
  end
  object CancelBtn: TButton
    Left = 163
    Top = 145
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object ExtensionCmbBx: TComboBox
    Left = 8
    Top = 24
    Width = 73
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 2
    Items.Strings = (
      '.TAP'
      '.Z80'
      '.SNA'
      '.BIN'
      '.ROM'
      '.CAS'
      '.P'
      '.Z81')
  end
  object ProgressBar1: TProgressBar
    Left = 0
    Top = 328
    Width = 321
    Height = 17
    Align = alBottom
    Min = 0
    Max = 100
    TabOrder = 9
  end
  object MakeMp3ChkBx: TCheckBox
    Left = 144
    Top = 24
    Width = 137
    Height = 17
    Caption = 'Make mp3 file'
    TabOrder = 3
  end
end
