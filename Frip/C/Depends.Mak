#
#        The software included, file formats and basic algorithms are
#      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
#
#      Module:  OS-Independent part of make file
#
#      $Log: Depends.Mak $
#      Revision 1.1  1997/01/01 21:40:25  dz
#      Initial revision
#
#
#
#

frip.h: style.h strng.h addr.h rip.h conf.h base.h links.h


sysdep.c: dep_os2_i.c dep_nt_i.c dep_dos_i.c dep_unix_i.c

$(OBJDIR)\Approve.obj: Approve.c
  $(COMPILE)

$(OBJDIR)\App.obj: App.c
  $(COMPILE)

$(OBJDIR)\flag.obj     : flag.c
  $(COMPILE)

$(OBJDIR)\datetime.obj     : datetime.c
  $(COMPILE)

$(OBJDIR)\msgid.obj     : msgid.c
  $(COMPILE)

$(OBJDIR)\basedata.obj : basedata.c
  $(COMPILE)

$(OBJDIR)\base.obj     : base.c
  $(COMPILE)

$(OBJDIR)\config.obj   : config.c
  $(COMPILE)

$(OBJDIR)\mainloop.obj : mainloop.c
  $(COMPILE)

$(OBJDIR)\mail.obj     : mail.c
  $(COMPILE)

$(OBJDIR)\Mail_Flags.obj : Mail_Flags.c
  $(COMPILE)

$(OBJDIR)\ping.obj     : ping.c
  $(COMPILE)

$(OBJDIR)\path.obj     : path.c
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
$(OBJDIR)\Approve.obj  \
$(OBJDIR)\App.obj      \
$(OBJDIR)\msgid.obj    \
$(OBJDIR)\ea.obj       \
$(OBJDIR)\flag.obj     \
$(OBJDIR)\basedata.obj \
$(OBJDIR)\base.obj     \
$(OBJDIR)\config.obj   \
$(OBJDIR)\datetime.obj \
$(OBJDIR)\mainloop.obj \
$(OBJDIR)\mail.obj     \
$(OBJDIR)\Mail_Flags.obj \
$(OBJDIR)\routes.obj   \
$(OBJDIR)\rip.obj      \
$(OBJDIR)\rif.obj      \
$(OBJDIR)\Links.obj    \
$(OBJDIR)\util.obj     \
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
$(OBJDIR)\frip.obj     \
$(OBJDIR)\sysdep.obj   \
$(OBJDIR)\addr.obj     \
$(OBJDIR)\debug.obj

OBJ=\
$(OBJDIR)\flag.obj,     $(OBJDIR)\Approve.obj,  \
$(OBJDIR)\msgid.obj,    $(OBJDIR)\App.obj,      \
$(OBJDIR)\addr.obj,     $(OBJDIR)\sglist.obj,   \
$(OBJDIR)\Announce.obj, $(OBJDIR)\Bink.obj,     \
$(OBJDIR)\TMail.obj,    $(OBJDIR)\Zip.obj,      \
$(OBJDIR)\base.obj,     $(OBJDIR)\config.obj,   \
$(OBJDIR)\mainloop.obj, $(OBJDIR)\routes.obj,   \
$(OBJDIR)\rip.obj,      $(OBJDIR)\Links.obj,    $(OBJDIR)\path.obj,     \
$(OBJDIR)\util.obj,     $(OBJDIR)\parse.obj,    $(OBJDIR)\ping.obj,     \
$(OBJDIR)\base_rw.obj,  $(OBJDIR)\basedata.obj, $(OBJDIR)\datetime.obj, \
$(OBJDIR)\strng.obj,    $(OBJDIR)\frip.obj,     \
$(OBJDIR)\sysdep.obj,   $(OBJDIR)\debug.obj,    \
$(OBJDIR)\rif.obj,      $(OBJDIR)\mail.obj,     $(OBJDIR)\Mail_Flags.obj, \
$(OBJDIR)\ea.obj


!include Manual_Depends.LST
!include DEPENDS.LST

clean:
	-rm $(EXE_DEPENDENCIES) $(EXE) *.map
        -mv *~ bak

makedep: DEPENDS.LST

DEPENDS.LST: $(EXE_DEPENDENCIES)
	makedep


