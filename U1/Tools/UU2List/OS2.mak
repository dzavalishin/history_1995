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
 main.obj

#=============================================================
#		Explicit Rules
#=============================================================
$(OUT)\uu2List.exe: OS2.cfg $(EXE_DEPENDENCIES)
  $(TLINK) -S:128000 /v /Toe /ap -S:100000 /L$(LIBPATH) @&&|
d:\BIN\BCOS2\LIB\C02.OBJ+
$(OUT)\main.obj
$(OUT)\uu2List,$(OUT)\uu2List
uucp.lib+
fido.lib+
uu2.lib+
d:\BIN\BCos2\LIB\C2I.LIB+
d:\BIN\BCos2\LIB\OS2.LIB

|


#=============================================================
#		Individual File Dependencies
#=============================================================

MAIN.obj: OS2.cfg MAIN.C 

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




