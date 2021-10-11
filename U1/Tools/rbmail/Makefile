#
# Sending and receiving batched mail
#
# $Header: Makefile,v 1.1 90/12/10 16:53:47 avg Exp $
# $Log:	Makefile,v $
# Revision 1.1  90/12/10  16:53:47  avg
# Initial revision
# 
#

test: rbmail.exe
	-del *.cpy
#	rbmail.exe <batch
#	rbmail.exe <BSvExu.D

DEST_EXE=d:\net\uupc\bin\rbmail.exe

install: $(DEST_EXE)

$(DEST_EXE): rbmail.exe
	copy rbmail.exe $(DEST_EXE)
        lxlite $(DEST_EXE)



RLIB=/usr/lib/relcom
RBIN=/usr/bin

CC=icc /Tdp /Ti /Q

#
# Set R=X if you have sources without RCS archive files
#
R=X
RCS=RCS
COFLAGS=-q

SHELL=/bin/sh
D=.
SD=.
CF=
UULIB=

UU=y

all: $(D)/batchmail.exe $(D)/rbmail.exe

$(D)/batchmail: batchmail.c
	cd $(D); $(CC) $(CFLAGS) -e batchmail.exe $(SD)/batchmail.c

$(D)/rbmail: rbmail.c  $(D)/$(UULIB)
	cd $(D); $(CC) $(CFLAGS) -e rbmail.exe $(SD)/rbmail.c $(UULIB)

#install: $(UU)inst

Ninst ninst:

#Yinst yinst: ${CF}mkall
#	cp $(D)/rbmail $(RBIN)/rbmail
#	chown uucp $(RBIN)/rbmail
#	chgrp uucp $(RBIN)/rbmail
#	chmod 4755 $(RBIN)/rbmail
#	cp $(D)/batchmail $(RLIB)/batchmail
#	chown uucp $(RLIB)/batchmail
#	chgrp uucp $(RLIB)/batchmail
#	chmod 4755 $(RLIB)/batchmail
#	-@set -x; \
#	[ -d /usr/spool/batchmail ] || \
#		( mkdir /usr/spool/batchmail ; \
#		  chmod 700 /usr/spool/batchmail ; \
#		  chown uucp /usr/spool/batchmail )
#

mkall:  all
Cmkall:

#
# Handling RCS archives
#
#$(R)batchmail.c: $(RCS)/batchmail.c,v ;  co $(COFLAGS) batchmail.c
#$(R)rbmail.c:    $(RCS)/rbmail.c,v ;     co $(COFLAGS) rbmail.c
#$(R)Makefile:    $(RCS)/Makefile,v ;     co $(COFLAGS) Makefile

