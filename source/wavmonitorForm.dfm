object MonitorFrm: TMonitorFrm
  Left = 233
  Top = 122
  BorderStyle = bsDialog
  Caption = 'Player Monitor'
  ClientHeight = 187
  ClientWidth = 380
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 136
    Width = 18
    Height = 13
    Caption = '------'
  end
  object SoundCardLbl: TLabel
    Left = 16
    Top = 35
    Width = 53
    Height = 13
    Caption = 'SoundCard'
  end
  object Label4: TLabel
    Left = 80
    Top = 3
    Width = 30
    Height = 13
    Caption = '----------'
    WordWrap = True
  end
  object Label5: TLabel
    Left = 80
    Top = 18
    Width = 30
    Height = 13
    Caption = '----------'
  end
  object OpenBtn: TButton
    Left = 16
    Top = 5
    Width = 49
    Height = 18
    Caption = 'File...'
    TabOrder = 0
    OnClick = OpenBtnClick
  end
  object PlayBtn: TButton
    Left = 288
    Top = 32
    Width = 41
    Height = 25
    Caption = 'Play'
    Enabled = False
    TabOrder = 2
    OnClick = PlayBtnClick
  end
  object TarjetasCmbBx: TComboBox
    Left = 80
    Top = 35
    Width = 201
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 1
    OnChange = TarjetasCmbBxChange
  end
  object MasterVolBar: TTrackBar
    Left = 16
    Top = 64
    Width = 41
    Height = 73
    BorderWidth = 2
    Max = 100
    Orientation = trVertical
    PageSize = 1
    Frequency = 20
    Position = 0
    SelEnd = 0
    SelStart = 0
    TabOrder = 5
    ThumbLength = 15
    TickMarks = tmBoth
    TickStyle = tsAuto
    OnChange = MasterVolBarChange
  end
  object ProgressBar1: TProgressBar
    Left = 72
    Top = 136
    Width = 305
    Height = 17
    Min = 0
    Max = 100
    Smooth = True
    Step = 1
    TabOrder = 8
  end
  object Panel1: TPanel
    Left = 72
    Top = 64
    Width = 305
    Height = 70
    BevelOuter = bvLowered
    Caption = '-----'
    TabOrder = 9
    object Image1: TImage
      Left = 1
      Top = 1
      Width = 303
      Height = 68
      Align = alClient
    end
  end
  object MuteChkBx: TCheckBox
    Left = 16
    Top = 56
    Width = 57
    Height = 9
    Caption = 'Mute'
    TabOrder = 4
    OnClick = MuteChkBxClick
  end
  object StopBtn: TButton
    Left = 336
    Top = 32
    Width = 41
    Height = 25
    Caption = 'Stop'
    TabOrder = 3
    OnClick = StopBtnClick
  end
  object CloseOnChkBx: TCheckBox
    Left = 8
    Top = 160
    Width = 161
    Height = 17
    Caption = 'Close on finished'
    TabOrder = 6
  end
  object CloseBtn: TButton
    Left = 248
    Top = 160
    Width = 121
    Height = 25
    Caption = 'Close'
    TabOrder = 7
    OnClick = CloseBtnClick
  end
  object OpenDialog1: TOpenDialog
    Left = 120
    Top = 32
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 95
    OnTimer = Timer1Timer
    Left = 160
    Top = 32
  end
end
