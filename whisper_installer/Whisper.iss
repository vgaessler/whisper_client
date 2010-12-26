[Setup]
AppName=Whisper
AppVerName=Whisper 0.2.7
DefaultDirName={reg:HKLM\SOFTWARE\Linden Research%2c Inc.\SecondLife,|{pf}\SecondLife}
DirExistsWarning=no
DefaultGroupName=Desktop
UninstallDisplayIcon={app}\mumble.exe
Compression=lzma
SolidCompression=yes
OutputDir=.
SourceDir=D:\home\projects\whisper\deployments\client_0.2.7
OutputBaseFilename=whisper_setup_0.2.7
AppendDefaultDirName=no

[Dirs]
Name: "{app}\SLVoice_orig"; AfterInstall: SaveVoice

[Files]
Source: "files\whisper\*"; DestDir: "{app}\whisper"; Flags: ignoreversion recursesubdirs
Source: "files\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs
Source: "files\whisper\readme.txt"; DestDir: "{app}"; Flags: ignoreversion isreadme

[Code]
var appdir: String;

procedure SaveVoice();
begin
  appdir := ExpandConstant('{app}');

  if (not FileExists(appdir + '\SLVoice_orig\SLVoice.exe')) then
  begin
    FileCopy(appdir + '\SLVoice.exe', appdir + '\SLVoice_orig\SLVoice.exe', True);
  end;
end;

[Run]
Filename: "{app}\use_whisper_voice.bat"; WorkingDir: "{app}"; StatusMsg: "Setting voice to mumble..."; Flags: skipifdoesntexist

[UninstallRun]
Filename: "{app}\use_sl_voice.bat"; WorkingDir: "{app}"; StatusMsg: "Resetting voice to original..."; Flags: skipifdoesntexist

[UninstallDelete]
Type: filesandordirs; Name: "{app}\SLVoice_orig";


