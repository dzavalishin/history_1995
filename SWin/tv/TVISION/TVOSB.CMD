@echo off
echo .
echo .		This cmd file compiles TVision for OS/2 using Borland C++.
echo .
echo .
echo .		Don't forget to change paths in OS2CMPB.CFG (see end of file)
echo .		and BCOS2_DIR variable in ALLMAKE.MAK
echo .

set IDA=D:\IDASRC\
cd source
make
