object ModificaDlg: TModificaDlg
  Left = 245
  Top = 108
  BorderStyle = bsDialog
  Caption = 'Dialog'
  ClientHeight = 176
  ClientWidth = 317
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poMainFormCenter
  DesignSize = (
    317
    176)
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 8
    Top = 8
    Width = 297
    Height = 121
    Shape = bsFrame
  end
  object Label1: TLabel
    Left = 24
    Top = 24
    Width = 64
    Height = 13
    Caption = 'Bloack Name'
  end
  object Label2: TLabel
    Left = 24
    Top = 48
    Width = 63
    Height = 13
    Caption = 'Start Address'
  end
  object Label3: TLabel
    Left = 24
    Top = 72
    Width = 20
    Height = 13
    Caption = 'Size'
  end
  object Label4: TLabel
    Left = 24
    Top = 96
    Width = 64
    Height = 13
    Caption = 'Exec address'
  end
  object OKBtn: TButton
    Left = 95
    Top = 148
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
  object CancelBtn: TButton
    Left = 183
    Top = 148
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object NameEdt: TEdit
    Left = 160
    Top = 16
    Width = 129
    Height = 21
    TabOrder = 2
    Text = 'NameEdt'
  end
  object StartEdt: TEdit
    Left = 160
    Top = 40
    Width = 129
    Height = 21
    TabOrder = 3
    Text = 'StartEdt'
  end
  object SizeEdt: TEdit
    Left = 160
    Top = 64
    Width = 129
    Height = 21
    TabOrder = 4
    Text = 'SizeEdt'
  end
  object ExecEdt: TEdit
    Left = 160
    Top = 88
    Width = 129
    Height = 21
    TabOrder = 5
    Text = 'ExecEdt'
  end
  object Memo1: TMemo
    Left = 0
    Top = 0
    Width = 313
    Height = 137
    Anchors = [akLeft, akTop, akRight, akBottom]
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Terminal'
    Font.Style = []
    Lines.Strings = (
      'Memo1')
    ParentFont = False
    ScrollBars = ssBoth
    TabOrder = 6
    Visible = False
    WordWrap = False
  end
end
