#
# $Log: os2.mak $
#   Revision 1.1  1995/03/11  06:14:08  dz
#   Initial revision
#
#
#

OUT=BCC_OS2.OBJ
BC=d:\bin\bcos2
#DEB=-v

.AUTODEPEND
.PATH.obj = $(OUT)

CC = bcc +OS2.CFG $(DEB)
TLINK = tlink
LIBPATH = $(BC)\LIB;$(BC)\classlib\lib;..\..\lib\out\os2
INCLUDEPATH = $(BC)\INCLUDE;$(BC)\classlib\INCLUDE;..\..\lib\out\os2


#		Implicit Rules
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
 process.obj \
 main.obj

#=============================================================
#		Explicit Rules
#=============================================================
bc_os2.obj\rnews.exe: OS2.cfg $(EXE_DEPENDENCIES)
  $(TLINK) $(DEB) -x /Toe /ap -S:100000 /L$(LIBPATH) @&&|
C02.OBJ+
$(OUT)\process.obj+
$(OUT)\main.obj
$(OUT)\rnews,$(OUT)\rnews
uucp.lib+
fido.lib+
uu2.lib+
C2I.LIB+
OS2.LIB

|
#BIDS2.LIB+
#$(OUT)\process.obj+
#uucp.lib+
#fido.lib+
#uu2.lib+



#=============================================================
#		Individual File Dependencies
#=============================================================
PROCESS.obj: OS2.cfg PROCESS.C 

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
-wuse
-wstv
-wsig
-wdef
-wcln
-wamb
-N
| OS2.cfg

#-N






