head	1.6;
access;
symbols
	Beta_12:1.3;
locks;
comment	@# @;


1.6
date	97.01.01.21.40.25;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.01.18.01.38.05;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.01.13.09.48.05;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.12.05.04.26.11;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.11.26.21.33.48;	author dz;	state Exp;
branches;
next	;


desc
@@


1.6
log
@*** empty log message ***
@
text
@#
#        The software included, file formats and basic algorithms are
#      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
#
#      Module:  OS-Independent part of make file
#
#      $Log: Common.Mak $
#      Revision 1.5  1996/01/18 01:38:05  dz
#      Split obj-dirs
#
#      Revision 1.4  1996/01/13 09:48:05  dz
#      *** empty log message ***
#
#      Revision 1.3  1996/01/07 20:21:01  dz
#      Beta 12
#
#
#

goal: $(EXE)

.c.obj:
  $(CC) -dVERDEF="$(VERDEF)" -fo=$@@ $<

.cpp.obj:
  $(CC) -dVERDEF="$(VERDEF)" -fo=$@@ $<

COMPILE=$(CC) -dVERDEF="$(VERDEF)" -fo=$@@ $(*B).c

!include DEPENDS.MAK
@


1.5
log
@Split obj-dirs
@
text
@d8 3
d30 1
a30 120
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
$(OBJDIR)\util.obj     \
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


@


1.4
log
@*** empty log message ***
@
text
@d8 3
d20 1
a20 1
  $(CC) -dVERDEF="$(VERDEF)" $<
d23 1
a23 1
  $(CC) -dVERDEF="$(VERDEF)" $<
d25 1
d29 92
a120 8
EXE_DEPENDENCIES =  \
Approve.obj flag.obj basedata.obj \
base.obj config.obj mainloop.obj routes.obj \
rip.obj Links.obj \
util.obj parse.obj strng.obj \
sglist.obj Zip.obj Bink.obj TMail.obj \
base_rw.obj \
Announce.obj frip.obj sysdep.obj addr.obj debug.obj
d123 11
a133 7
flag.obj, Approve.obj,\
addr.obj, sglist.obj, Announce.obj, Bink.obj, TMail.obj,\
Zip.obj, base.obj, config.obj, mainloop.obj, routes.obj, \
rip.obj, Links.obj,\
util.obj, parse.obj, \
base_rw.obj, basedata.obj, \
strng.obj, frip.obj, sysdep.obj, debug.obj
@


1.3
log
@Beta 12
@
text
@d7 4
a10 1
#      $Log:$
d26 1
d31 1
a31 1
base_rw.obj basedata.obj \
d35 1
@


1.2
log
@*** empty log message ***
@
text
@d1 10
a22 1
sglist.obj Zip.obj Bink.obj TMail.obj\
d25 4
a28 2
util.obj parse.obj\
strng.obj Announce.obj frip.obj
d31 1
a31 1
sglist.obj, Announce.obj, Bink.obj, TMail.obj,\
d35 2
a36 1
strng.obj, frip.obj
@


1.1
log
@Initial revision
@
text
@d4 1
a4 1
  $(CC) $<
d7 1
a7 1
  $(CC) $<
d12 17
d36 2
@
