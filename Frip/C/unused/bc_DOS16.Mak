#
#        The software included, file formats and basic algorithms are
#      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
#
#      Module:  OS-Dependent part of make file - DOS (16 bit), Borland C
#
#      $Log: Make_DOS16.Mak $
#
#
#

OBJDIR=bcdos16


!ifndef NODEBUG
DEB=-v
DEBLINK=debug all
#OPT=/O
!else
#OPT=/O
!endif

#OVERLAYS_CC=-of
#OVERLAYS_LD=option dynamic

EXE=frip_b16.exe

# zt - far data treshold
CC = bcc -mh $(DEB) $(OPT) $(OVERLAYS_CC) -Istl_cs -I.


goal: $(EXE)

.c.obj:
  $(CC) -dVERDEF="$(VERDEF)" -P -o$@ $<

.cpp.obj:
  $(CC) -dVERDEF="$(VERDEF)" -o$@ $<

COMPILE=$(CC) -DVERDEF="$(VERDEF)" -P -o$@ $&.c

!include DEPENDS.MAK

$(EXE): $(EXE_DEPENDENCIES) $(DLL_LIB) makedep
  bcc -P $(OVERLAYS_LD) -e$(EXE) $(DEBLINK) $(EXE_DEPENDENCIES)
  copy $(EXE) d:\tmp\frip

