@echo off
echo .
echo .		This cmd file compiles TVision for MS DOS 32 bit.
echo .
echo .
echo .		Don't forget to change paths in D32WATP.CFG (see end of line)
echo .

set IDA=D:\IDASRC\
cd source
make -D__WATCOMC__ -D__MSDOS__

