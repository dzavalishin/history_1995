head	1.1;
access;
symbols;
locks;
comment	@# @;


1.1
date	95.03.07.00.20.29;	author dz;	state Exp;
branches;
next	;


desc
@@


1.1
log
@Initial revision
@
text
@#=============================================================
#
#	OS2.MAK - Makefile for project D:\NET\GATE\UU2FIDO\uu2fido.prj
#		Created on 05/16/93 at 20:24
#
#=============================================================

.AUTODEPEND

OUT=BCC_OS2.Obj

.PATH.obj = $(OUT)

#=============================================================
#		Translator Definitions
#=============================================================
CC = bcc +OS2.CFG
TASM = tasm.exe
TLIB = tlib.exe
TLINK = tlink
RC = brcc.exe
RB = rc.exe
LIBPATH = d:\BIN\BCOS2\LIB;d:\BIN\BCOS2\classlib\LIB;..\..\lib\out\os2
INCLUDEPATH = d:\BIN\BCOS2\INCLUDE;d:\BIN\BCOS2\include\classlib;../../lib/out/os2


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
 ..\..\lib\out\os2\uucp.lib \
 ..\..\lib\out\os2\fido.lib \
 ..\..\lib\out\os2\uu2.lib \
 uuparse.obj \
 sendspool.obj \
 sendlett.obj \
 sendfile.obj \
 send.obj \
 runsmail.obj \
 main.obj \
 address.obj

#=============================================================
#		Explicit Rules
#=============================================================
$(OUT)\uu2fido.exe: OS2.cfg $(EXE_DEPENDENCIES)
  $(TLINK) -S:128000 /v /Toe /ap -S:100000 /L$(LIBPATH) @@&&|
d:\BIN\BCOS2\LIB\C02.OBJ+
$(OUT)\uuparse.obj+
$(OUT)\sendspool.obj+
$(OUT)\sendlett.obj+
$(OUT)\sendfile.obj+
$(OUT)\send.obj+
$(OUT)\runsmail.obj+
$(OUT)\main.obj+
$(OUT)\address.obj
$(OUT)\uu2fido,$(OUT)\uu2fido
uucp.lib+
fido.lib+
uu2.lib+
d:\BIN\BCos2\LIB\C2I.LIB+
d:\BIN\BCos2\LIB\OS2.LIB

|

#d:\BIN\BCos2\classlib\LIB\TCLASS2.LIB+
#d:\BIN\BCos2\LIB\BIDS2.LIB


#=============================================================
#		Individual File Dependencies
#=============================================================
UUPARSE.obj: OS2.cfg UUPARSE.C 

SENDSPOOL.obj: OS2.cfg SENDSPOOL.C 

SENDLETT.obj: OS2.cfg SENDLETT.C 

SENDFILE.obj: OS2.cfg SENDFILE.C 

SEND.obj: OS2.cfg SEND.C 

RUNSMAIL.obj: OS2.cfg RUNSMAIL.C 

MAIN.obj: OS2.cfg MAIN.C 

ADDRESS.obj: OS2.cfg ADDRESS.C 

#=============================================================
#		Compiler Configuration File
#=============================================================
OS2.cfg: OS2.mak
  copy &&|
-L$(LIBPATH)
-I$(INCLUDEPATH)
-nbcc_os2.obj
-P
-vi-
-v
-wuse
-wstv
-wsig
-wdef
-wcln
-wamp
-wamb
-N
| OS2.cfg




@
