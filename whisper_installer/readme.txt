Whisper V 0.2.7 Readme
======================
Whisper is an SLVoice.exe implementation based on the Mumble voice system.

Prerequisites
-------------
- Windows XP, Vista, or 7
- SL 1.22.11 or SL 1.23.5 based viewer. 
- OpenSim 0.6.9

Installation
------------
- Execute the installer. 
- The software must be installed in the viewer's program directory. Example:
  C:\Program Files\Imprudence
- During installation the original SLVoice.exe will be replaced by the mumble based
  whisper SLVoice.exe.

Usage
--------
During installation, the original SLVoice.exe was saved in the SLVoice_orig directory.
In order to switch back to the original SLVoice.exe, it the original SLVoice.exe must
be copied back to the program directory. There are two batch files that allow to do that.

- use_whisper_voice.bat
- user_SL_voice.bat

On Windows Vista and 7 they must be executed with admin rights.

You can keep shortcuts to these scripts on the desktop in order to switch between the 
voice systems. When running these scripts, make sure SLVoice.exe is not running anymore.
That should be the case about 15 seconds after stopping the viewer.

The .ini file whisper/whisper.ini has two parameters:
debug=yes|no             when switched on, the mumble client window will be visible
log_level=basic|debug    debug: full debug information is written to the log file
                         the log file is located in the <appdata>/whisper/whisper.log

Features
--------
- private parcel voice 
- speaker indication

Open Features
-------------
- positional sound
- private chat
- compilation for Mac
- compilaton for Linux is implemented but only superficially tested
- compatibility with SL viewer 2.x
- compatibility with OpenSim 0.7.x
- Automatic switch between Mumble and Freeswitch

Known Limitations
-----------------
Client:
- In some occasions, the client is unmuted on startup with the Talk button showing muted.
  As soon as the Talk button is pressed, the state is corrected.
- Microphone volume cannot be set in the device settings. Use Windows settings instead.
- Device settings can be operated but has no effect. Use Windows settings instead.
- In rare occasions, an avatar loses speaker indication, but can hear and talk normally.
  This can be fixed with a new login

Server:
- The region server and murmur must not be separated by firewalls. In order to do that, we 
  need to run Glacier2. Glacier2 is not fully integrated yet.
- All the parcels are running in the same murmur virtual server.
- When entering a parcel or region without mumble voice, the user can still hear voice from
  the parcel he/she came from.  

Troubleshooting
---------------
After starting the viewer: the Talk button does not appear:
    - Another instance of the viewer was started before and is running. In this case, 
      only that viewer will have voice.
    - Voice is not enabled in the preferences

The button appears but is greyed out:
    - Whisper was installed in the wrong directory
    - You are on a sim that does not have Mumble support

The button can be pressed but you hear nothing:
    - Check the Windows settings the output device.
    - Make sure your speakers are not muted (in Windows and in the viewer).
    - Make sure parcel voice is enabled on the parcel where you are. After enabling 
      voice on a parcel, you will need to leave the parcel and re-enter it.

Your talk button is active but others cannot hear you
    - Check the Windows settings the input device. 
    - Make sure the microphone is not muted.

When visiting a non-Mumble sim, your Talk button is greyed out
    - You need to switch back to the original SLVoice.exe. To do so you can run the script 
      use_sl_voice.bat located in the program directory.

Support
-------
For support please check out the forums: http://whisper.vcomm.ch/forum
 
Credits
-------
I am very grateful to the following people for their collaboration in whisper:
- Eduardo Valencia and DOT for their help in a large part of the development
  (http://www.dot-virtualworlds.com)
- Brian Becker for the implementation of the OpenSim whisper module
- Geronimo Rowies for the development in client software
- Snoopy Pfeffer for professional OpenSim hosting support.
  (http://3dmetaverse.com)
- Neil Canham for his support in testing the software.
  (http://www.flyingisland.co.uk)
