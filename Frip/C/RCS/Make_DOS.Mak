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
#      Module:  OS-Dependent part of make file - DOS
#
#      $Log: Make_DOS.Mak $
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

OBJDIR=dos

!ifndef NODEBUG
DEB=-d2
DEBLINK=debug all
#OPT=/Ox
!else
OPT=/Ox
!endif

EXE=frip_dos.exe
MAP=frip_dos.map

FIX=-5s -od
#QUIET=-zq 
#EXC=-xss
EXC=-xst
CC = wpp386 -bt=DOS $(DEB) $(OPT) $(QUIET) -i=stl $(EXC) $(FIX)

!include Common.Mak

$(EXE): $(EXE_DEPENDENCIES) $(DLL_LIB) makedep
  wlink system dos4g op stack=128k op map=$(MAP) name $(EXE) $(DEBLINK) file $(OBJ) disable 1027
  copy $(EXE) d:\tmp\frip

@


1.6
log
@Split obj-dirs
@
text
@d8 3
d23 3
a25 3
#DEB=-d2
#DEBLINK=debug all
#OPT=/O
d27 1
a27 1
#OPT=/O
d33 5
a37 1
CC = wpp386 -bt=DOS $(DEB) $(OPT) -zq -i=stl
@


1.5
log
@beta 14
@
text
@d8 3
d16 2
@


1.4
log
@Beta 12
@
text
@d7 4
a10 1
#      $Log:$
d25 1
a25 1
CC = wpp386 -bt=DOS $(DEB) $(OPT) -zq
@


1.3
log
@*** empty log message ***
@
text
@d1 10
d20 1
d27 1
a27 1
  wlink system dos4g op stack=128k name $(EXE) $(DEBLINK) file $(OBJ) disable 1027
@


1.2
log
@beta 7
@
text
@d11 1
a11 2
#Precompiled headers:  $(PH_GENERATE)
#PH_GENERATE=/fh
d13 1
a13 1
CC = wpp386 -bt=DOS $(DEB) $(OPT) -zq $(PH_GENERATE)
a14 16

#=============================================================
#		Implicit Rules
#=============================================================
.c.obj:
  $(CC) $<

.cpp.obj:
  $(CC) $<

!include Objects.Mak


#=============================================================
#		Explicit Rules
#=============================================================
a17 17

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
@d26 1
a26 18
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
d34 1
a34 1
  copy $(EXE) d:\tmp\open
d52 1
@
