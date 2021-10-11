#
#        The software included, file formats and basic algorithms are
#      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
#
#      Module:  OS-Dependent part of make file - DOS
#
#      $Log: Make_DOS.Mak $
#      Revision 1.7  1997/01/01 21:40:25  dz
#      *** empty log message ***
#
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
CC = wpp386 -bt=DOS $(DEB) $(OPT) $(QUIET) -i=stl $(EXC) $(FIX) -4s

!include Common.Mak

$(EXE): $(EXE_DEPENDENCIES) $(DLL_LIB) makedep
  wlink system dos4g op stack=128k op map=$(MAP) name $(EXE) $(DEBLINK) file $(OBJ) disable 1027
  copy $(EXE) d:\tmp\frip

