
SET DEVDIR=D:\dev
SET MUMBLEDIR=D:\dev\mumble
SET DEPLOYROOT=D:\home\projects\whisper\deployments

SET DEPLOYDIR=%DEPLOYROOT%\%1
SET DEPLOYFILES=%DEPLOYDIR%\files
SET DEPLOYWHISPER=%DEPLOYFILES%\whisper
SET DEPLOYPLUGINS=%DEPLOYWHISPER%\plugins

SET /P GOON="really delete all files in target directory %1? (yes|no) "

IF NOT %GOON%==yes goto END

IF NOT EXIST %DEPLOYFILES% mkdir %DEPLOYFILES%
IF NOT EXIST %DEPLOYWHISPER% mkdir %DEPLOYWHISPER%
IF NOT EXIST %DEPLOYPLUGINS% mkdir %DEPLOYPLUGINS%
IF NOT EXIST %DEPLOYPLUGINS%\codecs mkdir %DEPLOYPLUGINS%\codecs
IF NOT EXIST %DEPLOYPLUGINS%\iconengines mkdir %DEPLOYPLUGINS%\iconengines
IF NOT EXIST %DEPLOYPLUGINS%\imageformats mkdir %DEPLOYPLUGINS%\imageformats

COPY %DEVDIR%\mumble\whisper_installer\whisper.ini %DEPLOYWHISPER%
COPY %DEVDIR%\mumble\release\mumble.exe %DEPLOYWHISPER%
COPY %DEVDIR%\mumble\release\mumble_ol.dll %DEPLOYFILES%
COPY %DEVDIR%\mumble\release\celt.0.7.0.dll %DEPLOYFILES%
COPY %DEVDIR%\mumble\release\speex.dll %DEPLOYFILES%

COPY %DEVDIR%\mumble\whisper_installer\use_sl_voice.bat %DEPLOYFILES%
COPY %DEVDIR%\mumble\whisper_installer\use_whisper_voice.bat %DEPLOYFILES%
COPY %DEVDIR%\mumble\whisper_installer\qt.txt %DEPLOYWHISPER%
COPY %DEVDIR%\mumble\whisper_installer\speex.txt %DEPLOYWHISPER%
COPY %DEVDIR%\mumble\whisper_installer\whisper.txt %DEPLOYWHISPER%
COPY %DEVDIR%\mumble\whisper_installer\readme.txt %DEPLOYWHISPER%

COPY %DEVDIR%\mumble\whisper_installer\Microsoft.VC90.CRT.manifest %DEPLOYFILES%
COPY %DEVDIR%\mumble\whisper_installer\msvcp90.dll %DEPLOYFILES%
COPY %DEVDIR%\mumble\whisper_installer\msvcr90.dll %DEPLOYFILES%

COPY %DEVDIR%\MySQL\bin\libmysql.dll %DEPLOYFILES%

COPY %DEVDIR%\QtMumble\bin\QtCore4.dll %DEPLOYFILES%
COPY %DEVDIR%\QtMumble\bin\QtDBus4.dll %DEPLOYFILES%
COPY %DEVDIR%\QtMumble\bin\QtGui4.dll %DEPLOYFILES%
COPY %DEVDIR%\QtMumble\bin\QtNetwork4.dll %DEPLOYFILES%
COPY %DEVDIR%\QtMumble\bin\QtOpenGL4.dll %DEPLOYFILES%
COPY %DEVDIR%\QtMumble\bin\QtSql4.dll %DEPLOYFILES%
COPY %DEVDIR%\QtMumble\bin\QtSvg4.dll %DEPLOYFILES%
COPY %DEVDIR%\QtMumble\bin\QtXml4.dll %DEPLOYFILES%

COPY %DEVDIR%\QtMumble\plugins\codecs\qtwcodecs4.dll %DEPLOYPLUGINS%\codecs
COPY %DEVDIR%\QtMumble\plugins\codecs\qkrcodecs4.dll %DEPLOYPLUGINS%\codecs
COPY %DEVDIR%\QtMumble\plugins\codecs\qjpcodecs4.dll %DEPLOYPLUGINS%\codecs
COPY %DEVDIR%\QtMumble\plugins\codecs\qcncodecs4.dll %DEPLOYPLUGINS%\codecs
COPY %DEVDIR%\QtMumble\plugins\iconengines\qsvgicon4.dll %DEPLOYPLUGINS%\iconengines
COPY %DEVDIR%\QtMumble\plugins\imageformats\qtiff4.dll %DEPLOYPLUGINS%\imageformats
COPY %DEVDIR%\QtMumble\plugins\imageformats\qsvg4.dll %DEPLOYPLUGINS%\imageformats
COPY %DEVDIR%\QtMumble\plugins\imageformats\qmng4.dll %DEPLOYPLUGINS%\imageformats
COPY %DEVDIR%\QtMumble\plugins\imageformats\qjpeg4.dll %DEPLOYPLUGINS%\imageformats
COPY %DEVDIR%\QtMumble\plugins\imageformats\qico4.dll %DEPLOYPLUGINS%\imageformats
COPY %DEVDIR%\QtMumble\plugins\imageformats\qgif4.dll %DEPLOYPLUGINS%\imageformats

COPY %DEVDIR%\libsndfile\libsndfile-1.dll %DEPLOYFILES%

COPY %DEVDIR%\OpenSSL\bin\libeay32.dll %DEPLOYFILES%
COPY %DEVDIR%\OpenSSL\bin\ssleay32.dll %DEPLOYFILES%

rem COPY %DEVDIR%\dbus\bin\dbus-daemon.exe %DEPLOYFILES%
rem COPY %DEVDIR%\dbus\bin\dbus-send.exe %DEPLOYFILES%
rem COPY %DEVDIR%\dbus\bin\dbus-1.dll %DEPLOYFILES%
rem COPY %DEVDIR%\dbus\bin\iconv.dll %DEPLOYFILES%
rem COPY %DEVDIR%\dbus\bin\libcharset-1.dll %DEPLOYFILES%
rem COPY %DEVDIR%\dbus\bin\libiconv-2.dll %DEPLOYFILES%
rem COPY %DEVDIR%\dbus\bin\libxml2.dll %DEPLOYFILES%
rem COPY %DEVDIR%\dbus\bin\zlib1.dll %DEPLOYFILES%

:END
