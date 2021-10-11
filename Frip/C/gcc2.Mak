#
#        The software included, file formats and basic algorithms are
#      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
#
#      Module:  OS/2 make file - C Set
#
#      $Log: gcc2.Mak $
#      Revision 1.1  1997/01/01 21:40:25  dz
#      Initial revision
#
#
#
#
#


OBJDIR=gcc2

#!ifndef VERDEF
#VERDEF=undefined
#!endif

!ifndef NODEBUG
DEB=-g
#DEBLINK=/B"/DEB"
#OPT=
!else
OPT=-O2
!endif

EXE=frip_gcc2.exe
MAP=frip_gcc2.map

CC = gcc $(DEB) $(OPT) -I stl_gc -I . -x c++


goal: $(EXE)

.c.obj:
  $(CC) -o $@ -c $<

#  $(CC) -dVERDEF="$(VERDEF)" -o $@ -c $<

.cpp.obj:
  $(CC) -o $@ -c $<

#  $(CC) -dVERDEF="$(VERDEF)" -o $@ -c $<

COMPILE=$(CC) -o $@ -c $(*B).c
#COMPILE=$(CC) -dVERDEF="$(VERDEF)" -o $@ -c $(*B).c

frip.h: style.h strng.h addr.h rip.h conf.h base.h links.h

$(OBJDIR)\Approve.obj: Approve.c
  $(COMPILE)

$(OBJDIR)\flag.obj     : flag.c
  $(COMPILE)

$(OBJDIR)\basedata.obj : basedata.c
  $(COMPILE)

$(OBJDIR)\base.obj     : base.c
  $(COMPILE)

$(OBJDIR)\config.obj   : config.c
  $(COMPILE)

$(OBJDIR)\mainloop.obj : mainloop.c
  $(COMPILE)

$(OBJDIR)\routes.obj   : routes.c
  $(COMPILE)

$(OBJDIR)\rip.obj      : rip.c
  $(COMPILE)

$(OBJDIR)\Links.obj    : Links.c
  $(COMPILE)

$(OBJDIR)\util.obj     : util.c
  $(COMPILE)

$(OBJDIR)\parse.obj    : parse.c
  $(COMPILE)

$(OBJDIR)\strng.obj    : strng.c
  $(COMPILE)

$(OBJDIR)\sglist.obj   : sglist.c
  $(COMPILE)

$(OBJDIR)\Zip.obj      : Zip.c
  $(COMPILE)

$(OBJDIR)\Bink.obj     : Bink.c
  $(COMPILE)

$(OBJDIR)\TMail.obj    : TMail.c
  $(COMPILE)

$(OBJDIR)\base_rw.obj  : base_rw.c
  $(COMPILE)

$(OBJDIR)\Announce.obj : Announce.c
  $(COMPILE)

$(OBJDIR)\frip.obj     : frip.c
  $(COMPILE)

$(OBJDIR)\sysdep.obj   : sysdep.c
  $(COMPILE)

$(OBJDIR)\addr.obj     : addr.c
  $(COMPILE)

$(OBJDIR)\debug.obj    : debug.c
  $(COMPILE)




EXE_DEPENDENCIES =     \
$(OBJDIR)\Approve.obj  \
$(OBJDIR)\flag.obj     \
$(OBJDIR)\basedata.obj \
$(OBJDIR)\base.obj     \
$(OBJDIR)\config.obj   \
$(OBJDIR)\mainloop.obj \
$(OBJDIR)\routes.obj   \
$(OBJDIR)\rip.obj      \
$(OBJDIR)\Links.obj    \
$(OBJDIR)\parse.obj    \
$(OBJDIR)\strng.obj    \
$(OBJDIR)\sglist.obj   \
$(OBJDIR)\Zip.obj      \
$(OBJDIR)\Bink.obj     \
$(OBJDIR)\TMail.obj    \
$(OBJDIR)\base_rw.obj  \
$(OBJDIR)\Announce.obj \
$(OBJDIR)\frip.obj     \
$(OBJDIR)\sysdep.obj   \
$(OBJDIR)\addr.obj     \
$(OBJDIR)\util.obj     \
$(OBJDIR)\debug.obj

OBJ=\
$(OBJDIR)\flag.obj,     $(OBJDIR)\Approve.obj,  \
$(OBJDIR)\addr.obj,     $(OBJDIR)\sglist.obj,   \
$(OBJDIR)\Announce.obj, $(OBJDIR)\Bink.obj,     \
$(OBJDIR)\TMail.obj,    $(OBJDIR)\Zip.obj,      \
$(OBJDIR)\base.obj,     $(OBJDIR)\config.obj,   \
$(OBJDIR)\mainloop.obj, $(OBJDIR)\routes.obj,   \
$(OBJDIR)\rip.obj,      $(OBJDIR)\Links.obj,    \
$(OBJDIR)\util.obj,     $(OBJDIR)\parse.obj,    \
$(OBJDIR)\base_rw.obj,  $(OBJDIR)\basedata.obj, \
$(OBJDIR)\strng.obj,    $(OBJDIR)\frip.obj,     \
$(OBJDIR)\sysdep.obj,   $(OBJDIR)\debug.obj


!include DEPENDS.LST

clean:
	-rm $(EXE_DEPENDENCIES) $(EXE)

makedep: DEPENDS.LST

DEPENDS.LST: $(EXE_DEPENDENCIES)
	makedep


$(EXE): $(EXE_DEPENDENCIES) $(DLL_LIB) makedep
  $(CC) -Zexe -o $(EXE) $(DEBLINK) $(EXE_DEPENDENCIES)
  copy $(EXE) d:\tmp\frip

# $(CC) /B"/NOE" /B"/M:Full" /B"/NOO" /Q /Fe$(EXE) /Fm$(MAP) $(DEBLINK) $(EXE_DEPENDENCIES)
#  wlink system os2v2 name $(EXE) $(DEBLINK) file $(OBJ) disable 1027 op map=$(MAP) op stack=800k


