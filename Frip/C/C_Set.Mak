#
#        The software included, file formats and basic algorithms are
#      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
#
#      Module:  OS/2 make file - C Set
#
#      $Log: C_Set.Mak $
#      Revision 1.3  1997/01/01 21:40:19  dz
#      *** empty log message ***
#
#      Revision 1.2  1996/01/22 19:24:32  dz
#      Mostly works
#
#      Revision 1.1  1996/01/22 04:35:15  dz
#      Initial revision
#
#
#
#


OBJDIR=os2cs

#!ifndef VERDEF
#VERDEF=undefined
#!endif

!ifndef NODEBUG
DEB=/Ti+
DEBLINK=/B"/DEB"
#OPT=
!else
OPT=/G5 /O+ /Oi30
!endif

EXE=frip.exe
MAP=frip.map

CC = icc /Q $(DEB) $(OPT) -istl_cs;. /Tdp

goal: $(EXE)
#goal: regress $(EXE)

.c.obj:
  $(CC) /fo$@ /c $<

#  $(CC) -dVERDEF="$(VERDEF)" /fo$@ /c $<

.cpp.obj:
  $(CC) /fo$@ /c $<

#  $(CC) -dVERDEF="$(VERDEF)" /fo$@ /c $<

COMPILE=$(CC) /fo$@ /c $(*B).c
#COMPILE=$(CC) -dVERDEF="$(VERDEF)" /fo$@ /c $(*B).c

frip.h: style.h strng.h addr.h rip.h conf.h base.h links.h

$(OBJDIR)\Approve.obj: Approve.c
  $(COMPILE)

$(OBJDIR)\App.obj:     App.c
  $(COMPILE)

$(OBJDIR)\flag.obj     : flag.c
  $(COMPILE)

$(OBJDIR)\msgid.obj    : msgid.c
  $(COMPILE)

$(OBJDIR)\basedata.obj : basedata.c
  $(COMPILE)

$(OBJDIR)\base.obj     : base.c
  $(COMPILE)

$(OBJDIR)\config.obj   : config.c
  $(COMPILE)

$(OBJDIR)\datetime.obj : datetime.c
  $(COMPILE)

$(OBJDIR)\mainloop.obj : mainloop.c
  $(COMPILE)

$(OBJDIR)\mail.obj     : mail.c
  $(COMPILE)

$(OBJDIR)\Mail_Flags.obj :  Mail_Flags.c 
  $(COMPILE)

$(OBJDIR)\routes.obj   : routes.c
  $(COMPILE)

$(OBJDIR)\rip.obj      : rip.c
  $(COMPILE)

$(OBJDIR)\rif.obj      : rif.c
  $(COMPILE)

$(OBJDIR)\Links.obj    : Links.c
  $(COMPILE)

$(OBJDIR)\util.obj     : util.c
  $(COMPILE)

$(OBJDIR)\parse.obj    : parse.c
  $(COMPILE)

$(OBJDIR)\path.obj     : path.c
  $(COMPILE)

$(OBJDIR)\ping.obj     : ping.c
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

$(OBJDIR)\ea.obj       : ea.c
  $(COMPILE)

$(OBJDIR)\debug.obj    : debug.c
  $(COMPILE)




EXE_DEPENDENCIES =     \
$(OBJDIR)\ea.obj       \
$(OBJDIR)\Approve.obj  \
$(OBJDIR)\App.obj      \
$(OBJDIR)\msgid.obj    \
$(OBJDIR)\flag.obj     \
$(OBJDIR)\basedata.obj \
$(OBJDIR)\base.obj     \
$(OBJDIR)\config.obj   \
$(OBJDIR)\mainloop.obj \
$(OBJDIR)\routes.obj   \
$(OBJDIR)\rip.obj      \
$(OBJDIR)\rif.obj      \
$(OBJDIR)\Links.obj    \
$(OBJDIR)\parse.obj    \
$(OBJDIR)\path.obj     \
$(OBJDIR)\ping.obj     \
$(OBJDIR)\strng.obj    \
$(OBJDIR)\sglist.obj   \
$(OBJDIR)\Zip.obj      \
$(OBJDIR)\Bink.obj     \
$(OBJDIR)\TMail.obj    \
$(OBJDIR)\base_rw.obj  \
$(OBJDIR)\Announce.obj \
$(OBJDIR)\sysdep.obj   \
$(OBJDIR)\addr.obj     \
$(OBJDIR)\util.obj     \
$(OBJDIR)\datetime.obj \
$(OBJDIR)\mail.obj     \
$(OBJDIR)\Mail_Flags.obj \
$(OBJDIR)\debug.obj

#$(OBJDIR)\frip.obj     


OBJ=\
$(OBJDIR)\flag.obj,     $(OBJDIR)\Approve.obj,  \
$(OBJDIR)\addr.obj,     $(OBJDIR)\sglist.obj,   \
$(OBJDIR)\Announce.obj, $(OBJDIR)\Bink.obj,     \
$(OBJDIR)\TMail.obj,    $(OBJDIR)\Zip.obj,      \
$(OBJDIR)\base.obj,     $(OBJDIR)\config.obj,   \
$(OBJDIR)\mainloop.obj, $(OBJDIR)\routes.obj,   \
$(OBJDIR)\rip.obj,      $(OBJDIR)\Links.obj,    \
$(OBJDIR)\util.obj,     $(OBJDIR)\parse.obj,     $(OBJDIR)\path.obj,    \
$(OBJDIR)\base_rw.obj,  $(OBJDIR)\basedata.obj, \
$(OBJDIR)\strng.obj,    $(OBJDIR)\frip.obj,     \
$(OBJDIR)\sysdep.obj,   $(OBJDIR)\debug.obj,    \
$(OBJDIR)\rif.obj,      $(OBJDIR)\ea.obj,       \
$(OBJDIR)\mail.obj,     $(OBJDIR)\Mail_Flags.obj, $(OBJDIR)\App.obj,      \
$(OBJDIR)\msgid.obj


!include Manual_Depends.LST
!include DEPENDS.LST

clean:
	-rm $(EXE_DEPENDENCIES) $(EXE) *.map
        -mv *~ bak

makedep: DEPENDS.LST

DEPENDS.LST: $(EXE_DEPENDENCIES)
	makedep


$(EXE): $(EXE_DEPENDENCIES) $(OBJDIR)\frip.obj $(DLL_LIB) makedep
  $(CC) /B"/NOE" /B"/M:Full" /Q /Fe$(EXE) /Fm$(MAP) $(DEBLINK) $(EXE_DEPENDENCIES) $(OBJDIR)\frip.obj
  copy $(EXE) d:\tmp\frip

REXE=regress.exe

$(REXE): regress.obj $(EXE_DEPENDENCIES)
  $(CC) /B"/NOE" /B"/M:Full" /Q /Fe$(REXE) $(DEBLINK) regress.obj $(EXE_DEPENDENCIES)

regress: $(REXE)

#  $(CC) /B"/NOE" /B"/M:Full" /B"/NOO" /Q /Fe$(EXE) /Fm$(MAP) $(DEBLINK) $(EXE_DEPENDENCIES)

#  wlink system os2v2 name $(EXE) $(DEBLINK) file $(OBJ) disable 1027 op map=$(MAP) op stack=800k


