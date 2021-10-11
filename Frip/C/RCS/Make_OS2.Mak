head	1.4;
access;
symbols
	Beta_12:1.1;
locks;
comment	@# @;


1.4
date	97.01.01.21.40.25;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.01.18.01.38.05;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.01.18.01.00.09;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	;


desc
@@


1.4
log
@*** empty log message ***
@
text
@#
#        The software included, file formats and basic algorithms are
#      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
#
#      Module:  OS-Dependent part of make file - OS/2
#
#      $Log: Make_OS2.Mak $
#      Revision 1.3  1996/01/18 01:38:05  dz
#      Split obj-dirs
#
#      Revision 1.2  1996/01/18 01:00:09  dz
#      Beta 15
#
#      Revision 1.1  1996/01/07 20:21:01  dz
#      Initial revision
#
#      Revision 1.4  1996/01/07 20:14:22  dz
#      *** empty log message ***
#
#
#

OBJDIR=os2

!ifndef NODEBUG
DEB=-d2
DEBLINK=debug all
#OPT=
!else
OPT=-ox -oe=100
!endif

EXE=frip.exe
MAP=frip.map

CC = wpp386 -w8 $(DEB) $(OPT) -zq -od -i=stl


!include Common.Mak

$(EXE): $(EXE_DEPENDENCIES) $(DLL_LIB) makedep
  wlink system os2v2 name $(EXE) $(DEBLINK) file $(OBJ) disable 1027 op map=$(MAP) op stack=800k
  copy $(EXE) d:\tmp\frip

@


1.3
log
@Split obj-dirs
@
text
@d8 3
d30 1
a30 1
#OPT=-ox -oe=100
@


1.2
log
@Beta 15
@
text
@d8 3
d19 2
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
#      $Log: makefile $
d28 1
a28 1
CC = wpp386 -wx $(DEB) $(OPT) -zq -od
@
