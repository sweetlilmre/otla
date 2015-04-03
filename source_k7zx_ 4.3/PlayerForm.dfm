object PlayerFrm: TPlayerFrm
  Left = 348
  Top = 207
  Width = 588
  Height = 448
  Caption = 'k7zx 4.3'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label6: TLabel
    Left = 192
    Top = 232
    Width = 32
    Height = 13
    Caption = 'Label6'
  end
  object Label2: TLabel
    Left = 72
    Top = 280
    Width = 75
    Height = 13
    Caption = 'Output directory'
  end
  object Label7: TLabel
    Left = 72
    Top = 320
    Width = 94
    Height = 13
    Caption = 'LAME mp3 encoder'
  end
  object BitRateLbl: TLabel
    Left = 224
    Top = 320
    Width = 49
    Height = 13
    Caption = 'BitRateLbl'
  end
  object LoaderLbl: TLabel
    Left = 194
    Top = 229
    Width = 82
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'LoaderLbl'
    Color = clCaptionText
    ParentColor = False
    Visible = False
  end
  object DuraLbl: TLabel
    Left = 320
    Top = 228
    Width = 41
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'DuraLbl'
    Visible = False
  end
  object DirectoryListBox1: TDirectoryListBox
    Left = 8
    Top = 32
    Width = 177
    Height = 193
    FileList = FileListBox1
    ItemHeight = 16
    TabOrder = 0
  end
  object FileListBox1: TFileListBox
    Left = 192
    Top = 5
    Width = 177
    Height = 220
    ItemHeight = 13
    Mask = '*.sbb,*.tap;*.tzx;*.sna;*.z80;*.hex'
    TabOrder = 1
    OnChange = FileListBox1Change
  end
  object DriveComboBox1: TDriveComboBox
    Left = 8
    Top = 8
    Width = 177
    Height = 19
    DirList = DirectoryListBox1
    TabOrder = 2
  end
  object File2WAVBtn: TButton
    Left = 8
    Top = 296
    Width = 57
    Height = 25
    Caption = '=> WAV'
    TabOrder = 3
    OnClick = File2WAVBtnClick
  end
  object EmularBtn: TButton
    Left = 304
    Top = 256
    Width = 57
    Height = 25
    Caption = '=>Emu'
    Enabled = False
    TabOrder = 4
    OnClick = EmularBtnClick
  end
  object PlayBtn: TButton
    Left = 192
    Top = 256
    Width = 49
    Height = 25
    Caption = 'Play'
    Enabled = False
    TabOrder = 5
    OnClick = PlayBtnClick
  end
  object StopBtn: TButton
    Left = 240
    Top = 256
    Width = 49
    Height = 25
    Caption = 'Stop'
    Enabled = False
    TabOrder = 6
    OnClick = StopBtnClick
  end
  object InfoChkBx: TCheckBox
    Left = 9
    Top = 258
    Width = 136
    Height = 17
    Caption = 'Info in output file name'
    TabOrder = 7
  end
  object StatusBarra: TStatusBar
    Left = 0
    Top = 378
    Width = 580
    Height = 23
    Panels = <
      item
        Width = 77
      end
      item
        Width = 90
      end
      item
        Width = 60
      end
      item
        Width = 250
      end
      item
        Width = 25
      end>
    SimplePanel = False
  end
  object OutputEdt: TEdit
    Left = 72
    Top = 296
    Width = 289
    Height = 21
    TabOrder = 9
    Text = 'OutputEdt'
  end
  object File2MP3Btn: TButton
    Left = 8
    Top = 336
    Width = 57
    Height = 25
    Caption = '=> MP3'
    TabOrder = 10
    OnClick = File2MP3BtnClick
  end
  object LameEdt: TEdit
    Left = 72
    Top = 336
    Width = 289
    Height = 21
    TabOrder = 11
    Text = 'LameEdt'
  end
  object GroupBox1: TGroupBox
    Left = 376
    Top = 0
    Width = 193
    Height = 369
    Caption = 'File Info'
    TabOrder = 12
    object Label4: TLabel
      Left = 9
      Top = 77
      Width = 104
      Height = 15
      Caption = 'RANDOMIZE USR'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Fixedsys'
      Font.Style = []
      ParentFont = False
    end
    object Label3: TLabel
      Left = 9
      Top = 52
      Width = 40
      Height = 15
      Caption = 'CLEAR'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Fixedsys'
      Font.Style = []
      ParentFont = False
    end
    object Label5: TLabel
      Left = 9
      Top = 24
      Width = 64
      Height = 25
      AutoSize = False
      Caption = 'Program:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Fixedsys'
      Font.Style = []
      ParentFont = False
    end
    object Label1: TLabel
      Left = 8
      Top = 96
      Width = 32
      Height = 13
      Caption = 'Blocks'
    end
    object UsrEdt: TEdit
      Left = 136
      Top = 72
      Width = 49
      Height = 21
      TabOrder = 0
    end
    object ClearEdt: TEdit
      Left = 80
      Top = 45
      Width = 49
      Height = 21
      TabOrder = 1
    end
    object NombreEdt: TEdit
      Left = 80
      Top = 19
      Width = 105
      Height = 21
      MaxLength = 10
      TabOrder = 2
    end
    object SnapGB: TGroupBox
      Left = 8
      Top = 42
      Width = 177
      Height = 65
      Caption = 'SnapGB'
      TabOrder = 3
      Visible = False
      object TipoSnapLbl: TLabel
        Left = 80
        Top = 8
        Width = 60
        Height = 13
        Caption = 'TipoSnapLbl'
        Visible = False
      end
      object PCLbl: TLabel
        Left = 16
        Top = 16
        Width = 28
        Height = 13
        Caption = 'PCLbl'
      end
      object SPLbl: TLabel
        Left = 16
        Top = 32
        Width = 28
        Height = 13
        Caption = 'SPLbl'
      end
      object CurrentLbl: TLabel
        Left = 16
        Top = 48
        Width = 48
        Height = 13
        Caption = 'CurrentLbl'
      end
    end
    object TrozosCLB: TCheckListBox
      Left = 8
      Top = 112
      Width = 177
      Height = 97
      ItemHeight = 13
      Items.Strings = (
        '1'
        '2'
        '3'
        '4'
        '5'
        '6'
        '7')
      TabOrder = 4
    end
    object PokeadorChkBx: TCheckBox
      Left = 8
      Top = 272
      Width = 161
      Height = 17
      Caption = 'Make POKEs'
      TabOrder = 5
    end
    object PokesVLE: TValueListEditor
      Left = 40
      Top = 288
      Width = 145
      Height = 73
      KeyOptions = [keyEdit, keyAdd, keyDelete, keyUnique]
      Strings.Strings = (
        '35136=0')
      TabOrder = 6
      TitleCaptions.Strings = (
        'Address'
        'Value')
      ColWidths = (
        65
        74)
    end
    object Memo1: TMemo
      Left = 8
      Top = 216
      Width = 177
      Height = 49
      Lines.Strings = (
        'Memo1')
      ReadOnly = True
      ScrollBars = ssVertical
      TabOrder = 7
      WordWrap = False
    end
  end
  object DirBtn: TButton
    Left = 160
    Top = 280
    Width = 17
    Height = 17
    Caption = '...'
    TabOrder = 13
    OnClick = DirBtnClick
  end
  object LameBtn: TButton
    Left = 184
    Top = 320
    Width = 17
    Height = 17
    Caption = '...'
    TabOrder = 14
    OnClick = LameBtnClick
  end
  object SelectedChkBx: TCheckBox
    Left = 8
    Top = 240
    Width = 153
    Height = 17
    Caption = 'Convert only selected file'
    Checked = True
    State = cbChecked
    TabOrder = 15
    OnClick = SelectedChkBxClick
  end
  object MediaPlayer1: TMediaPlayer
    Left = 120
    Top = 336
    Width = 253
    Height = 33
    AutoRewind = False
    Visible = False
    TabOrder = 16
    OnNotify = MediaPlayer1Notify
  end
  object ProgressBar1: TProgressBar
    Left = 192
    Top = 241
    Width = 169
    Height = 10
    Min = 0
    Max = 100
    Position = 50
    Smooth = True
    TabOrder = 17
    Visible = False
  end
  object MainMenu1: TMainMenu
    Left = 104
    object Settings1: TMenuItem
      Caption = 'Settings'
      OnClick = Settings1Click
    end
    object About1: TMenuItem
      Caption = 'About'
      OnClick = About1Click
    end
  end
  object OpenDialog1: TOpenDialog
    Ctl3D = False
    Left = 136
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 200
    OnTimer = Timer1Timer
    Left = 344
    Top = 312
  end
end
