OUT=BCC_OS2.OBJ

.AUTODEPEND
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
LIBPATH = d:\BIN\BCOS2\LIB;d:\BIN\BCOS2\Classlib\LIB
INCLUDEPATH = d:\BIN\BCOS2\INCLUDE;d:\BIN\BCOS2\Classlib\INCLUDE;..\..\lib\out\os2;..\..


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
 getflo.obj \
 getfile.obj \
 scanmail.obj \
 main.obj

#=============================================================
#		Explicit Rules
#=============================================================
$(OUT)\getfatt.exe: OS2.CFG $(EXE_DEPENDENCIES)
  $(TLINK) /v /Toe /ap /L$(LIBPATH) @&&|
d:\BIN\BCOS2\LIB\C02.OBJ+
$(OUT)\getflo.obj+
$(OUT)\getfile.obj+
$(OUT)\scanmail.obj+
$(OUT)\main.obj
$(OUT)\getfatt,$(OUT)\getfatt
..\..\lib\out\os2\fido.lib+
..\..\lib\out\os2\uucp.lib+
..\..\lib\out\os2\uu2.lib+
d:\BIN\BCOS2\Classlib\LIB\TCLASS2.LIB+
d:\BIN\BCOS2\LIB\C2.LIB+
d:\BIN\BCOS2\LIB\OS2.LIB

|


#=============================================================
#		Individual File Dependencies
#=============================================================
GETFLO.obj: OS2.CFG GETFLO.C 

GETFILE.obj: OS2.CFG GETFILE.C 

SCANMAIL.obj: OS2.CFG SCANMAIL.C 

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


