OUT=BCC_OS2.OBJ

.AUTODEPEND
.PATH.obj =$(OUT)

#=============================================================
#		Translator Definitions
#=============================================================
CC = bcc +OS2.CFG
TASM = tasm.exe
TLIB = tlib.exe
TLINK = tlink
RC = brcc.exe
RB = rc.exe
LIBPATH = d:\BIN\BCOS2\LIB;..\..\lib\out\os2;d:\BIN\BCOS2\classlib\lib
INCLUDEPATH = d:\BIN\BCOS2\INCLUDE;..\..\lib\out\os2;d:\BIN\BCOS2\classlib\INCLUDE;..\..


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
 ..\..\lib\out\os2\uu2.lib \
 ..\..\lib\out\os2\fido.lib \
 uuencode.obj \
 enc_part.obj \
 process.obj \
 scan.obj \
 main.obj

#=============================================================
#		Explicit Rules
#=============================================================
$(OUT)\file2uu.exe: OS2.CFG $(EXE_DEPENDENCIES)
  $(TLINK) /v /Toe /ap /L$(LIBPATH) @&&|
d:\BIN\BCOS2\LIB\C02.OBJ+
$(OUT)\uuencode.obj+
$(OUT)\enc_part.obj+
$(OUT)\process.obj+
$(OUT)\scan.obj+
$(OUT)\main.obj
$(OUT)\file2uu,$(OUT)\file2uu
..\..\lib\out\os2\uucp.lib+
..\..\lib\out\os2\uu2.lib+
..\..\lib\out\os2\fido.lib+
d:\BIN\BCOS2\classlib\lib\TCLASS2.LIB+
d:\BIN\BCOS2\LIB\C2.LIB+
d:\BIN\BCOS2\LIB\OS2.LIB

|


#=============================================================
#		Individual File Dependencies
#=============================================================
UUENCODE.obj: OS2.CFG UUENCODE.C 

ENC_PART.obj: OS2.CFG ENC_PART.C 

PROCESS.obj: OS2.CFG PROCESS.C 

SCAN.obj: OS2.CFG SCAN.C 

MAIN.obj: OS2.CFG MAIN.C 

#=============================================================
#		Compiler Configuration File
#=============================================================
OS2.CFG: OS2.mak
  copy &&|
-L$(LIBPATH)
-I$(INCLUDEPATH)
-n$(OUT)
-P
-vi-
-v
| OS2.CFG


