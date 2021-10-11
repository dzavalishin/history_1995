head	1.3;
access;
symbols;
locks;
comment	@# @;


1.3
date	97.01.01.21.40.19;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.01.22.19.24.32;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.01.22.04.35.15;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@*** empty log message ***
@
text
@#
#        The software included, file formats and basic algorithms are
#      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
#
#      Module:  OS/2 make file - C Set
#
#      $Log: C_Set.Mak $
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

!ifndef VERDEF
VERDEF=undefined
!endif

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


goal: regress $(EXE)

.c.obj:
  $(CC) -dVERDEF="$(VERDEF)" /fo$@@ /c $<

.cpp.obj:
  $(CC) -dVERDEF="$(VERDEF)" /fo$@@ /c $<

COMPILE=$(CC) -dVERDEF="$(VERDEF)" /fo$@@ /c $(*B).c

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
$(OBJDIR)\mail.obj,     $(OBJDIR)\App.obj,      \
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


@


1.2
log
@Mostly works
@
text
@d8 3
d33 2
a34 2
EXE=frip_cs.exe
MAP=frip_cs.map
d36 1
a36 1
CC = icc /Q $(DEB) $(OPT) -istl_cs /Tdp
d39 1
a39 1
goal: $(EXE)
d54 3
d60 3
d72 3
d78 3
d87 3
d99 6
d135 3
d145 1
d147 2
d156 1
d159 2
a167 1
$(OBJDIR)\frip.obj     \
d171 2
d175 3
d186 1
a186 1
$(OBJDIR)\util.obj,     $(OBJDIR)\parse.obj,    \
d189 4
a192 1
$(OBJDIR)\sysdep.obj,   $(OBJDIR)\debug.obj
d195 1
d199 2
a200 1
	-rm $(EXE_DEPENDENCIES) $(EXE)
d208 2
a209 2
$(EXE): $(EXE_DEPENDENCIES) $(DLL_LIB) makedep
  $(CC) /B"/NOE" /B"/M:Full" /B"/NOO" /Q /Fe$(EXE) /Fm$(MAP) $(DEBLINK) $(EXE_DEPENDENCIES)
d211 9
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
#      $Log: Common.Mak $
d167 1
a167 1
  $(CC) /Fe$(EXE) /Fm$(MAP) $(DEBLINK) $(EXE_DEPENDENCIES)
@
