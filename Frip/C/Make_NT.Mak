#
#        The software included, file formats and basic algorithms are
#      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
#
#      Module:  OS-Dependent part of make file - Win NT / Win 95
#
#      $Log: Make_NT.Mak $
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

#  -4s - can't because of lib absence
CC = wpp386 -bt=NT $(CCWIN) $(DEB) $(OPT) -zq -i=stl -xs -4s

!include Common.Mak

$(EXE): $(EXE_DEPENDENCIES) $(DLL_LIB) makedep
  wlink system $(TARGET) name $(EXE) $(DEBLINK) file $(OBJ) disable 1027 op map=$(MAP) Library kernel32.lib
  copy $(EXE) d:\tmp\frip

