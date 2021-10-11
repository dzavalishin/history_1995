#
#        The software included, file formats and basic algorithms are
#      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
#
#      Module:  OS-Dependent part of make file - DOS (16 bit)
#
#      $Log: Make_DOS16.Mak $
#      Revision 1.7  1996/01/22 04:35:15  dz
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

