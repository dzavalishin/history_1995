@echo off
echo .
echo .		This bat file compiles TVision for MS DOS 16 bit.
echo .
echo .
echo .		Don't forget to change paths in IDACMPB.CFG
echo .		and variable BCDOS_DIR in ALLMAKE.MAK.
echo .

set IDA=D:\IDASRC\
cd source
make
