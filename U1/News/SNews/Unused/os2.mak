#=============================================================
#
#	OS2.MAK - Makefile for project D:\NET\GATE\NEWS\SNEWS\snews2.prj
#		Created on 05/16/93 at 19:40
#
#=============================================================

OUT=BCC_OS2.OBJ
BC=d:\bin\bcos2
DEB=-v

.AUTODEPEND
.PATH.obj = $(OUT)

#=============================================================
#		Translator Definitions
#=============================================================
CC = bcc $(DEB) +OS2.CFG
TASM = tasm.exe
TLIB = tlib.exe
TLINK = tlink
RC = brcc.exe
RB = rc.exe
LIBPATH = $(BC)\LIB;..\..\lib\out\os2
INCLUDEPATH = $(BC)\CLASSLIB\INCLUDE;$(BC)\INCLUDE;..\..\lib\out\os2


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
 ..\..\lib\out\os2\fido.lib \
 ..\..\lib\out\os2\uucp.lib \
 ..\..\lib\out\os2\uu2.lib \
 unpack.obj \
 send.obj \
 scan.obj \
 read.obj \
 origin.obj \
 main.obj

#=============================================================
#		Explicit Rules
#=============================================================
$(OUT)\snews.exe: OS2.cfg $(EXE_DEPENDENCIES)
  $(TLINK)  $(DEB) /Toe /ap -S:10000 /L$(LIBPATH) @&&|
$(BC)\LIB\C02.OBJ+
$(OUT)\unpack.obj+
$(OUT)\send.obj+
$(OUT)\scan.obj+
$(OUT)\read.obj+
$(OUT)\origin.obj+
$(OUT)\main.obj+
d:\bin\bcos2\lib\wildargs.obj
$(OUT)\snews,$(OUT)\snews
fido.lib+
uucp.lib+
uu2.lib+
$(BC)\LIB\BIDS2.LIB+
$(BC)\LIB\C2I.LIB+
$(BC)\LIB\OS2.LIB

|


#=============================================================
#		Individual File Dependencies
#=============================================================
UNPACK.obj: OS2.cfg UNPACK.C 

SEND.obj: OS2.cfg SEND.C 

SCAN.obj: OS2.cfg SCAN.C 

READ.obj: OS2.cfg READ.C 

ORIGIN.obj: OS2.cfg ORIGIN.C 

MAIN.obj: OS2.cfg MAIN.C 

#=============================================================
#		Compiler Configuration File
#=============================================================
OS2.cfg: OS2.mak
  copy &&|
-L$(LIBPATH)
-I$(INCLUDEPATH)
-n$(OUT)
-P
-vi-
-v
-N
| OS2.cfg



