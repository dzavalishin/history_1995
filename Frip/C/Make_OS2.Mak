#
#        The software included, file formats and basic algorithms are
#      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
#
#      Module:  OS-Dependent part of make file - OS/2
#
#      $Log: Make_OS2.Mak $
#      Revision 1.4  1997/01/01 21:40:25  dz
#      *** empty log message ***
#
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

EXE=frip_w.exe
MAP=frip_w.map

#DISABLE_OPT=-od 
CC = wpp386 -w8 -xs $(DEB) $(OPT) -zq -i=stl $(DISABLE_OPT) -bt=os2v2


!include Common.Mak

$(EXE): $(EXE_DEPENDENCIES) $(DLL_LIB) makedep
  wlink system os2v2 name $(EXE) $(DEBLINK) file $(OBJ) disable 1027 op map=$(MAP) op stack=800k
  copy $(EXE) d:\tmp\frip

