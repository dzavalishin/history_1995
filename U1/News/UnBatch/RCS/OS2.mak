head	1.1;
access;
symbols;
locks;
comment	@#   @;


1.1
date	95.03.11.18.32.26;	author dz;	state Exp;
branches;
next	;


desc
@UU2 News Unbatcher
@


1.1
log
@Initial revision
@
text
@OUT=bcc_os2.obj
BC=d:\bin\bcos2
#DEB=-v

.AUTODEPEND
.PATH.obj = BCC_OS2.OBJ

#=============================================================
#		Translator Definitions
#=============================================================
CC = bcc +OS2.CFG -v $(DEB)
TASM = tasm.exe
TLIB = tlib.exe
TLINK = tlink
RC = brcc.exe
RB = rc.exe
LIBPATH = $(BC)\LIB;$(BC)\ClassLib\LIB;..\..\lib\out\os2
INCLUDEPATH = $(BC)\INCLUDE;$(BC)\ClassLib\INCLUDE;..\..\lib\out\os2


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
vread_wi.obj \
vread_fi.obj \
vread_bf.obj \
vread.obj \
uuparse.obj \
unbatch.obj \
sendnews.obj \
scan.obj \
open_pkt.obj \
mktemp.obj \
main.obj \
kiae_unb.obj \
gr2area.obj \
get_art.obj \
getopt.obj \
gethdr.obj \
decomp.obj \
chg_ext.obj \
ToSpool.obj FTNFrom.obj

#=============================================================
#		Explicit Rules
#=============================================================
$(OUT)\unbatch.exe: OS2.cfg $(EXE_DEPENDENCIES)
  $(TLINK) $(DEB) /Toe /ap /L$(LIBPATH) /v -S:10000 @@&&|
$(BC)\LIB\C02.OBJ+
$(OUT)\vread_wi.obj+
$(OUT)\vread_fi.obj+
$(OUT)\vread_bf.obj+
$(OUT)\vread.obj+
$(OUT)\uuparse.obj+
$(OUT)\unbatch.obj+
$(OUT)\sendnews.obj+
$(OUT)\scan.obj+
$(OUT)\open_pkt.obj+
$(OUT)\mktemp.obj+
$(OUT)\main.obj+
$(OUT)\kiae_unb.obj+
$(OUT)\gr2area.obj+
$(OUT)\get_art.obj+
$(OUT)\getopt.obj+
$(OUT)\gethdr.obj+
$(OUT)\decomp.obj+
$(OUT)\chg_ext.obj+
$(OUT)\toSpool.obj+
$(OUT)\FTNFrom.obj
$(OUT)\unbatch,$(OUT)\unbatch
uucp.lib+
fido.lib+
uu2.lib+
$(BC)\LIB\BIDS2.LIB+
$(BC)\LIB\C2I.LIB+
$(BC)\LIB\OS2.LIB

|


#=============================================================
#		Individual File Dependencies
#=============================================================
VREAD_WI.obj: OS2.cfg VREAD_WI.C 

VREAD_FI.obj: OS2.cfg VREAD_FI.C 

VREAD_BF.obj: OS2.cfg VREAD_BF.C 

VREAD.obj: OS2.cfg VREAD.C 

UUPARSE.obj: OS2.cfg UUPARSE.C 

UNBATCH.obj: OS2.cfg UNBATCH.C 

SENDNEWS.obj: OS2.cfg SENDNEWS.C 

SCAN.obj: OS2.cfg SCAN.C 

OPEN_PKT.obj: OS2.cfg OPEN_PKT.C 

MKTEMP.obj: OS2.cfg MKTEMP.C 

MAIN.obj: OS2.cfg MAIN.C 

KIAE_UNB.obj: OS2.cfg KIAE_UNB.C 

GR2AREA.obj: OS2.cfg GR2AREA.C 

GET_ART.obj: OS2.cfg GET_ART.C 

GETOPT.obj: OS2.cfg GETOPT.C 

GETHDR.obj: OS2.cfg GETHDR.C 

DECOMP.obj: OS2.cfg DECOMP.C 

CHG_EXT.obj: OS2.cfg CHG_EXT.C 

ToSpool.obj: OS2.cfg ToSpool.c

FTNFrom.obj: OS2.cfg FTNFrom.c

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
