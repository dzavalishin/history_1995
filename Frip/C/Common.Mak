#
#        The software included, file formats and basic algorithms are
#      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
#
#      Module:  OS-Independent part of make file
#
#      $Log: Common.Mak $
#      Revision 1.6  1997/01/01 21:40:25  dz
#      *** empty log message ***
#
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
  $(CC) -fo=$@ $<

#  $(CC) -dVERDEF="$(VERDEF)" -fo=$@ $<

.cpp.obj:
  $(CC) -fo=$@ $<

#  $(CC) -dVERDEF="$(VERDEF)" -fo=$@ $<


COMPILE=$(CC) -fo=$@ $(*B).c

#COMPILE=$(CC) -dVERDEF="$(VERDEF)" -fo=$@ $(*B).c

!include DEPENDS.MAK
