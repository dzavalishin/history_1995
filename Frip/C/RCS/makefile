head	1.6;
access;
symbols
	Beta_12:1.5;
locks;
comment	@# @;


1.6
date	96.12.28.03.11.55;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.01.07.20.14.22;	author dz;	state Exp;
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


1.6
log
@new string/istring/domain_s comparison ops
@
text
@!include C_Set.Mak 

@


1.5
log
@Beta 12
@
text
@d1 1
a1 1
!include Make_OS2.Mak
@


1.4
log
@*** empty log message ***
@
text
@d1 1
a1 29
#
#        The software included, file formats and basic algorithms are
#      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
#
#      Module:  OS-Dependent part of make file - OS/2
#
#      $Log:$
#
#

!ifndef NODEBUG
DEB=-d2
DEBLINK=debug all
#OPT=
!else
#OPT=-ox -oe=100
!endif

EXE=frip.exe
MAP=frip.map

CC = wpp386 -wx $(DEB) $(OPT) -zq -od


!include Common.Mak

$(EXE): $(EXE_DEPENDENCIES) $(DLL_LIB) makedep
  wlink system os2v2 name $(EXE) $(DEBLINK) file $(OBJ) disable 1027 op map=$(MAP) op stack=800k
  copy $(EXE) d:\tmp\frip
@


1.3
log
@*** empty log message ***
@
text
@d1 10
d16 1
a16 1
OPT=-ox -oe=100
d20 1
d22 1
a22 1
CC = wpp386 -wx $(DEB) $(OPT) -zq
d28 1
a28 1
  wlink system os2v2 name $(EXE) $(DEBLINK) file $(OBJ) disable 1027
@


1.2
log
@beta 7
@
text
@d4 1
a4 1
#OPT=/O
d6 1
a6 1
#OPT=/O
d11 1
a11 2
#Precompiled headers:  $(PH_GENERATE)
#PH_GENERATE= /fh
a12 1
CC = wpp386 -wx $(DEB) $(OPT) -zq $(PH_GENERATE)
d14 1
a15 15
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
a19 18
#out: ..\..\Out\OS2Dist.OUT\$(EXE)

#..\..\Out\OS2Dist.OUT\$(EXE): $(EXE)
#  copy $(EXE) ..\..\Out\OS2Dist.OUT

clean:
	-rm $(EXE_DEPENDENCIES) $(EXE)

makedep: DEPENDS.LST

DEPENDS.LST: $(EXE_DEPENDENCIES)
	makedep


frip.h: style.h strng.h addr.h rip.h conf.h base.h links.h


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
Announce.obj \
base.obj config.obj mainloop.obj routes.obj \
rip.obj Links.obj \
util.obj parse.obj\
strng.obj frip.obj

OBJ=\
Announce.obj, \
base.obj, config.obj, mainloop.obj, routes.obj, \
rip.obj, Links.obj,\
util.obj, parse.obj, \
strng.obj, frip.obj
d34 1
a34 1
  copy $(EXE) d:\tmp\open
a43 1

d48 3
@
