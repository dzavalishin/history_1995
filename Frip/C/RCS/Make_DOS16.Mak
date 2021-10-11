head	1.7;
access;
symbols
	Beta_12:1.4;
locks;
comment	@# @;


1.7
date	96.01.22.04.35.15;	author dz;	state Exp;
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
#      Module:  OS-Dependent part of make file - DOS (16 bit)
#
#      $Log: Make_DOS16.Mak $
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

OBJDIR=dos16


!ifndef NODEBUG
DEB=-d2
DEBLINK=debug all
#OPT=/O
!else
#OPT=/O
!endif

OVERLAYS_CC=-of
OVERLAYS_LD=option dynamic

EXE=frip_d16.exe

# zt - far data treshold
CC = wpp -mh -zt100 -bt=DOS $(DEB) $(OPT) $(OVERLAYS_CC) -zq -i=stl


!include Common.Mak

$(EXE): $(EXE_DEPENDENCIES) $(DLL_LIB) makedep
  wlink system dos $(OVERLAYS_LD) op m=dos16 op stack=40k name $(EXE) $(DEBLINK) begin autosection file $(OBJ) end disable 1027
  copy $(EXE) d:\tmp\frip

@


1.6
log
@Split obj-dirs
@
text
@d8 3
d24 2
a25 2
#DEB=-d2
#DEBLINK=debug all
d43 1
a43 1
  wlink system dos $(OVERLAYS_LD) op m=dos16 op stack=60k name $(EXE) $(DEBLINK) begin autosection file $(OBJ) end disable 1027
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
d28 1
a28 1
CC = wpp -mh -zt100 -bt=DOS $(DEB) $(OPT) $(OVERLAYS_CC) -zq
@


1.3
log
@*** empty log message ***
@
text
@d1 10
d19 3
d25 1
a25 1
CC = wpp -mh -zt100 -bt=DOS $(DEB) $(OPT) -zq
d31 1
a31 1
  wlink system dos op stack=60k name $(EXE) $(DEBLINK) file $(OBJ) disable 1027
@


1.2
log
@beta 7
@
text
@a10 3
#Precompiled headers:  $(PH_GENERATE)
#PH_GENERATE=/fh

d12 1
a13 8
CC = wpp -mh -zt100 -bt=DOS $(DEB) $(OPT) -zq $(PH_GENERATE)


#=============================================================
#		Implicit Rules
#=============================================================
.c.obj:
  $(CC) $<
d15 1
a15 2
.cpp.obj:
  $(CC) $<
a16 6
!include Objects.Mak


#=============================================================
#		Explicit Rules
#=============================================================
a20 16
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
@d28 1
a28 18
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
d36 1
a36 1
  copy $(EXE) d:\tmp\open
@
