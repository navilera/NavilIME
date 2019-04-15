# NavilIME
Windows Hangul Input Method Editor based on TSF

NavilIME is a Hangul input method editor for Windows based on TSF (Text Service Framework) using the libhangul. NavilIME supports following Hangul keyboard layouts.

* Seobul 3-18Na
* Seobul 3-90
* Seobul 3-91 final
* Seobul no-shift
* Seobul yeot-Hangul
* Dobul standard
* Dobul yeot-Hangul
* Hangul roman

## How to build NavilIME
Open the NavilIME.sln file using the VisualStudio. You can select one of two build options which are the Release and the Debug option.

You can use the Debug build option when your purpose is development. You have to modify the DEBUG_LOGFILE configuration value in the Global.h for writing log file without any potential issue.

NavilIME can be built both of x86 and x64 build option.

## How to install NavilIME as manully
Make a following folder.

C:\Program Files\NavilIME
C:\Program Files (x86)\NavilIME

Launch a Windows command with administrator and run the install.cmd.

You can install NavilIME as manually if you want. Copy a Keyboard.dat and a NavilIME.dll into the installation folder. Register NavilIME using regsvr32 tool. Followings are step by step command list.

```
copy /y NavilIME\Keyboard.dat "C:\Program Files\NavilIME"
copy /y x64\Release\NavilIME.dll "C:\Program Files\NavilIME"
regsvr32 "c:\Program Files\NavilIME\NavilIME.dll"

copy /y NavilIME\Keyboard.dat "C:\Program Files (x86)\NavilIME"
copy /y Win32\Release\NavilIME.dll "C:\Program Files (x86)\NavilIME"
regsvr32 "c:\Program Files (x86)\NavilIME\NavilIME.dll"
```

## Installation using automatic installer
Just download installer from [https://github.com/navilera/NavilIME/tree/master/Release/Installer].
Then run the latest installer EXE file.

## How to uninstall NavilIME
Launch a Windows command with administrator and run the uninstall.cmd. Then delete the installation folder as manually. Following is real command for uninstall NavilIME.

```
regsvr32 /u "c:\Program Files\NavilIME\NavilIME.dll"
regsvr32 /u "c:\Program Files (x86)\NavilIME\NavilIME.dll"
```

## How configure a keyboard layout
Modify Keyboard.dat from the installation folder (default are C:\Program Files\NavilIME and c:\Program Files (x86)\NavilIME). Write an one of following list.

* 318Na
* 2
* 2y
* 32
* 39
* 3f
* 3s
* 3y
* ro



