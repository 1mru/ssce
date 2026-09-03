; このスクリプトは Inno Setup スクリプトウィザード によって生成されました。
; Inno Setup スクリプトファイルの作成方法の詳細については、ドキュメントを参照してください！
; 非商用利用のみ.

#define MyAppName "ssce"
#define MyAppVersion "0.0.3"
#define MyAppURL "https://1mru.github.io/ssce/"
#define MyAppExeName "ssce.exe"
#define MyAppAssocName MyAppName + " ファイル"
#define MyAppAssocExt ".myp"
#define MyAppAssocKey StringChange(MyAppAssocName, " ", "") + MyAppAssocExt
#define DoubleAmp(Value) StringChange(Value, "&", "&&")
#define EscapeConstArgument(Value) StringChange(StringChange(StringChange(Value, "%", "%25"), ",", "%2c"), "}", "%7d")

[Setup]
; 注意: AppId の値はこのアプリケーションを一意に識別します。他のアプリケーションのインストーラーで同じ AppId の値を使用しないでください。
; (新しい GUID を生成するには、IDE 内で「ツール」|「GUID の生成」をクリックしてください。)
AppId={{3025E5C2-21A6-43C2-A2DA-9A04E4537921}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={cm:NameAndVersion,{#EscapeConstArgument(MyAppName)},{#EscapeConstArgument(MyAppVersion)}}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
UninstallDisplayIcon={app}\{#MyAppExeName}
; "ArchitecturesAllowed=x64compatible" は、セットアップが x64 および Arm 版 Windows 11 以外では実行できないことを指定します。
ArchitecturesAllowed=x64compatible
; "ArchitecturesInstallIn64BitMode=x64compatible" は、x64 または Arm 版 Windows 11 で「64 ビットモード」でインストールを実行するように要求します。
; これは、ネイティブの 64 ビット Program Files フォルダーおよびレジストリの 64 ビットビューを使用することを意味します。
ArchitecturesInstallIn64BitMode=x64compatible
; 64 ビットインストーラーを使用するには、次の行のコメントを解除してください。
;SetupArchitecture=x64
ChangesAssociations=yes
DisableProgramGroupPage=yes
LicenseFile=C:\Users\1mru\ssce\LICENSE.txt
; 非管理者インストールモード (現在のユーザーのみにインストール) で実行するには、次の行のコメントを解除してください。
;PrivilegesRequired=lowest
OutputBaseFilename=ssce-setup
SolidCompression=yes
WizardStyle=modern dynamic

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\Users\1mru\ssce\bin\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
; 注意: 共有システムファイルには "Flags: ignoreversion" を使用しないでください。

[Registry]
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocExt}\OpenWithProgids"; ValueType: string; ValueName: "{#MyAppAssocKey}"; ValueData: ""; Flags: uninsdeletevalue
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKey}"; ValueType: string; ValueName: ""; ValueData: "{#MyAppAssocName}"; Flags: uninsdeletekey
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKey}\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\{#MyAppExeName},0"
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKey}\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\{#MyAppExeName}"" ""%1"""

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#DoubleAmp(MyAppName)}}"; Flags: nowait postinstall skipifsilent

