head	1.3;
access;
symbols;
locks;
comment	@# @;


1.3
date	96.07.10.18.27.24;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.07.09.16.03.58;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.07.09.02.12.58;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@something goes through, framer/frame classes
are work somehow.
@
text
@!ifndef NODEBUG
DEB=-Ti -Tx
DEBLINK=/B"/DEB"
#OPT=/O
!else
#OPT=/O
!endif

IPATH = .
#LPATH = ..\..\lib
#DLL_LIB=$(LPATH)\U1_$(DLLVER).LIB
#DLL_DLL=$(LPATH)\U1_$(DLLVER).DLL

EXE=MPipe.exe
DEF=MPipe.Def

#Precompiled headers:  $(PH_GENERATE) $(PH_USE)
#PH_GENERATE=/Fi+
#PH_USE=/Si+
BRO=-Fb
#/Ms 
ETC=/W3# /Ol #/Wvft

CC = icc $(ETC) $(DEB) $(BRO) $(OPT) /I$(IPATH) /Tdp /Ge+ /Gm /Q $(PH_GENERATE) $(PH_USE)


#=============================================================
#		Implicit Rules
#=============================================================
.c.obj:
  $(CC) /c $<

.cpp.obj:
  $(CC) /c $<

#=============================================================
#		List Macros
#=============================================================


EXE_DEPENDENCIES =  \
 MpChanType.obj \
 MpChan.obj \
 MpPkUtil.obj \
 MpBuff.obj \
 MpSess.obj \
 MpUtil.obj \
 MpFrame.obj \
 MpWin.obj \
 MpPort.obj \
 MpRecv.obj \
 MpSend.obj \
 MpPQueue.obj \
 MpPacket.obj \
 Mp.obj \
 strng.obj

#=============================================================
#		Explicit Rules
#=============================================================
#/Ms 

LINKFLAGS=/B"/NOO" $(DEBLINK)

$(EXE): $(EXE_DEPENDENCIES) $(DLL_LIB) $(DEF) makedep
   $(CC) $(LINKFLAGS) /Fe$(EXE) $(EXE_DEPENDENCIES) $(DEF) 
# dde4cc.lib 

#  icc /Tdp /B"/NOO" $(DEBLINK) /Fe$(EXE) /Q $(EXE_DEPENDENCIES) $(DLL_LIB) /I$(IPATH) dde4cc.lib $(DEF)

#out: ..\..\Out\OS2Dist.OUT\$(EXE)

#..\..\Out\OS2Dist.OUT\$(EXE): $(EXE)
#  copy $(EXE) ..\..\Out\OS2Dist.OUT
#  lxlite /d- ..\..\Out\OS2Dist.OUT\$(EXE)

clean:
	-rm $(EXE_DEPENDENCIES) $(EXE)
        -mv *.?~ bak
        -mv *.brs bak


#=============================================================
#		Individual File Dependencies
#=============================================================

makedep: DEPENDS.LST

DEPENDS.LST: $(DLL_DEPENDENCIES) $(EXE_DEPENDENCIES)
	makedep

!include depends.lst


#=============================================================
#               DEF
#=============================================================




$(DEF): makefile
	cat >$(DEF) <<
NAME MPipe WINDOWCOMPAT
DESCRIPTION 'MPipe Protocol driver'
STACKSIZE 128000
HEAPSIZE 512000
<<




@


1.2
log
@debugging depacketizer
@
text
@d22 1
a22 1
ETC=# /Ol #/Wvft
@


1.1
log
@Initial revision
@
text
@d6 1
a6 1
OPT=/O
d22 1
a22 1
ETC=#/Wvft
d24 1
a24 1
CC = icc $(ETC) $(DEB) $(BRO) $(OPT) /I$(IPATH) /Tdp /Ol /Ge+ /Gm /Q $(PH_GENERATE) $(PH_USE)
d66 2
a67 1
   $(CC) $(LINKFLAGS) /Fe$(EXE) $(EXE_DEPENDENCIES) dde4cc.lib $(DEF)
@
