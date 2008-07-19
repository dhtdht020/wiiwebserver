@ECHO off
echo Setting WIILOAD
set WIILOAD=tcp:192.168.0.3
echo Wiiload ..
wiiload.exe hl2.elf
echo Done :-)
pause