#=============================================================
#
#   OS2.MAK - Makefile for fido2uu/2
#
#=============================================================

OBJ=BCC_OS2.OBJ
#DEB=-v

.AUTODEPEND
.PATH.obj = $(OBJ)

#=============================================================
#		Translator Definitions
#=============================================================
CC = bcc $(DEB) +OS2.CFG
TASM = tasm.exe
TLIB = tlib.exe
TLINK = tlink
RC = brcc.exe
RB = rc.exe
LIBPATH = d:\BIN\BCOS2\LIB;..\..\lib\out\os2
INCLUDEPATH = d:\BIN\BCOS2\INCLUDE;d:\BIN\BCOS2\include\classlib;..\..\lib\out\os2


#=============================================================
#		Implicit Rules
#=============================================================
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

.asm.obj:
  $(TASM) -Mx $*.asm,$*.obj

.rc.res:
  $(RC) -r $*.rc

#=============================================================
#		List Macros
#=============================================================


EXE_DEPENDENCIES =  \
 {$(LIBPATH)}uucp.lib \
 {$(LIBPATH)}fido.lib \
 {$(LIBPATH)}uu2.lib \
 encode.obj \
 address.obj \
 uuencode.obj \
 send_err.obj \
 send_ack.obj \
 runsmail.obj \
 gatectl.obj \
 convert.obj \
 select.obj \
 mainloop.obj \
 main.obj

#=============================================================
#		Explicit Rules
#=============================================================
$(OBJ)\fido2uu.exe: OS2.CFG $(EXE_DEPENDENCIES)
  $(TLINK) $(DEB) /v /x /Toe /ap -S:100000 /L$(LIBPATH) @&&|
d:\BIN\BCOS2\LIB\C02.OBJ+
$(OBJ)\encode.obj+
$(OBJ)\address.obj+
$(OBJ)\uuencode.obj+
$(OBJ)\send_err.obj+
$(OBJ)\send_ack.obj+
$(OBJ)\runsmail.obj+
$(OBJ)\gatectl.obj+
$(OBJ)\convert.obj+
$(OBJ)\select.obj+
$(OBJ)\mainloop.obj+
$(OBJ)\main.obj
$(OBJ)\fido2uu

uucp.lib+
fido.lib+
uu2.lib+
d:\BIN\BCOS2\LIB\BIDS2.LIB+
d:\BIN\BCOS2\LIB\C2I.LIB+
d:\BIN\BCOS2\LIB\OS2.LIB

|

#d:\BIN\BCOS2\classlib\LIB\TCLASS2.LIB+


#=============================================================
#		Individual File Dependencies
#=============================================================
ENCODE.obj: OS2.CFG ENCODE.C 

ADDRESS.obj: OS2.CFG ADDRESS.C 

UUENCODE.obj: OS2.CFG UUENCODE.C 

SEND_ERR.obj: OS2.CFG SEND_ERR.C 

SEND_ACK.obj: OS2.CFG SEND_ACK.C 

RUNSMAIL.obj: OS2.CFG RUNSMAIL.C 

GATECTL.obj: OS2.CFG GATECTL.C 

CONVERT.obj: OS2.CFG CONVERT.C 

SELECT.obj: OS2.CFG SELECT.C 

MAINLOOP.obj: OS2.CFG MAINLOOP.C 

MAIN.obj: OS2.CFG MAIN.C 

#=============================================================
#		Compiler Configuration File
#=============================================================
OS2.CFG: OS2.MAK
  copy &&|
-L$(LIBPATH)
-I$(INCLUDEPATH)
-n$(OBJ)
-P
-vi-
-wuse
-wstv
-wsig
-wdef
-wcln
-wamp
-wamb
-N
| OS2.CFG






