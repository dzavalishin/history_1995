@echo off
echo .
echo .		This cmd file compiles TVision for OS/2 using Watcom C++.
echo .
echo .
echo .		Don't forget to change paths in OS2WATP.CFG (see end of line)
echo .		Don't forget to change paths in OS2WATD.CFG (see end of line)
echo .

set IDA=D:\IDASRC\
cd source

rem compiling program
make -D__WATCOMC__
if errorlevel == 1 pause

rem compiling dll
make -D__WATCOMC__ -D__DLL__


