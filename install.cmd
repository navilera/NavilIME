copy /y NavilIME\Keyboard.dat "C:\Program Files\NavilIME"
copy /y x64\Release\NavilIME.dll "C:\Program Files\NavilIME"
regsvr32 "c:\Program Files\NavilIME\NavilIME.dll"

copy /y NavilIME\Keyboard.dat "C:\Program Files (x86)\NavilIME"
copy /y Win32\Release\NavilIME.dll "C:\Program Files (x86)\NavilIME"
regsvr32 "c:\Program Files (x86)\NavilIME\NavilIME.dll"
