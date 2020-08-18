del navilime.log
mkdir "c:\Program Files\NavilIME"
copy /y NavilIME\Keyboard.dat "C:\Program Files\NavilIME"
copy /y x64\Debug\NavilIME.dll "C:\Program Files\NavilIME"
regsvr32 "c:\Program Files\NavilIME\NavilIME.dll"
