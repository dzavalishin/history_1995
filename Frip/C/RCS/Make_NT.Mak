head	1.7;
access;
symbols
	Beta_12:1.4;
locks;
comment	@# @;


1.7
date	97.01.01.21.40.25;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.01.18.01.38.05;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.01.14.01.06.31;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.12.05.04.26.11;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.11.16.00.56.34;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.11.13.00.10.25;	author dz;	state Exp;
branches;
next	;


desc
@@


1.7
log
@*** empty log message ***
@
text
@#
#        The software included, file formats and basic algorithms are
#      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
#
#      Module:  OS-Dependent part of make file - Win NT / Win 95
#
#      $Log: Make_NT.Mak $
#      Revision 1.6  1996/01/18 01:38:05  dz
#      Split obj-dirs
#
#      Revision 1.5  1996/01/14 01:06:31  dz
#      beta 14
#
#      Revision 1.4  1996/01/07 20:21:01  dz
#      Beta 12
#
#
#

OBJDIR=nt


!ifndef NODEBUG
#DEB=-d2
#DEBLINK=debug all
#OPT=/O
!else
#OPT=/O
!endif

!ifdef NTWIN
CCWIN=-bw
TARGET=nt_win
!else
TARGET=nt
!endif

EXE=frip_nt.exe
MAP=frip_nt.map

CC = wpp386 -bt=NT $(CCWIN) $(DEB) $(OPT) -zq -i=stl -xs

!include Common.Mak

$(EXE): $(EXE_DEPENDENCIES) $(DLL_LIB) makedep
  wlink system $(TARGET) name $(EXE) $(DEBLINK) file $(OBJ) disable 1027 op map=$(MAP)
  copy $(EXE) d:\tmp\frip

@


1.6
log
@Split obj-dirs
@
text
@d8 3
d41 1
a41 1
CC = wpp386 -bt=NT $(CCWIN) $(DEB) $(OPT) -zq -i=stl
@


1.5
log
@beta 14
@
text
@d8 3
d16 3
@


1.4
log
@Beta 12
@
text
@d7 4
a10 1
#      $Log:$
d32 1
a32 1
CC = wpp386 -bt=NT $(CCWIN) $(DEB) $(OPT) -zq
@


1.3
log
@*** empty log message ***
@
text
@d1 10
d27 1
d34 1
a34 1
  wlink system $(TARGET) name $(EXE) $(DEBLINK) file $(OBJ) disable 1027
@


1.2
log
@beta 7
@
text
@a8 2


d18 1
a18 5
#Precompiled headers:  $(PH_GENERATE)
#PH_GENERATE=/fh

CC = wpp386 -bt=NT $(CCWIN) $(DEB) $(OPT) -zq $(PH_GENERATE)

d20 1
a20 5
#=============================================================
#		Implicit Rules
#=============================================================
.c.obj:
  $(CC) $<
a21 8
.cpp.obj:
  $(CC) $<

!include Objects.Mak

#=============================================================
#		Explicit Rules
#=============================================================
a25 16
#out: ..\..\Out\OS2Dist.OUT\$(EXE)

#..\..\Out\OS2Dist.OUT\$(EXE): $(EXE)
#  copy $(EXE) ..\..\Out\OS2Dist.OUT

clean:
	-rm $(EXE_DEPENDENCIES) $(EXE)


makedep: DEPENDS.LST

DEPENDS.LST: $(EXE_DEPENDENCIES)
	makedep


!include DEPENDS.LST
@


1.1
log
@Initial revision
@
text
@d35 1
a35 19
#=============================================================
#		List Macros
#=============================================================


EXE_DEPENDENCIES =  \
announce.obj \
base.obj config.obj mainloop.obj routes.obj \
rip.obj Links.obj \
util.obj parse.obj\
strng.obj frip.obj

OBJ=\
announce.obj, \
base.obj, config.obj, mainloop.obj, routes.obj, \
rip.obj, Links.obj,\
util.obj, parse.obj, \
strng.obj, frip.obj

d42 1
a42 1
  copy $(EXE) d:\tmp\open
@
