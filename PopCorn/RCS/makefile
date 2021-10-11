head	1.6;
access;
symbols
	Ver_075:1.6;
locks;
comment	@# @;


1.6
date	96.09.17.23.20.43;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.09.17.14.28.50;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.09.14.19.37.27;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.09.13.12.38.48;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.09.13.07.39.24;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.13.07.24.48;	author dz;	state Exp;
branches;
next	;


desc
@@


1.6
log
@First completely working POP3/ESMTP version
@
text
@
EXE=PopCorn.exe

all: $(EXE) mkmf
    
mkmf:
   mkmf -s

#LD_PROFILE=/B"/DE /NOE" cppopa3.obj
#OP_PROFILE=-Gh

OP_DEBUG=-Ti
#OP_OPTIMIZE=-O
OP_SOCK=-DOS2 -Gm -Gt -Sm -Ss
CFLAGS= -Tdp -Q $(OP_PROFILE) $(OP_OPTIMIZE) $(OP_DEBUG) $(OP_SOCK)
#vmt placement -Wvft+

OBJS= ffolder.obj mailbox.obj mailboxes.obj main.obj parse.obj pop3proto.obj recode.obj runsmail.obj smtpproto.obj strng.obj tcp.obj uupcconf.obj

$(EXE): $(OBJS)
   icc -Fe$(EXE) $(OP_DEBUG) $(OBJS) $(LD_PROFILE) so32dll.lib tcp32dll.lib

clean:
	-rm $(OBJS)
        -mv *.?~ bak
        -mv *.brs bak






#************************ PopCorn server ***************************\
#*
#*	Copyright (C) 1991-1996 by Dmitry Zavalishin
#*
#*	Module 	: 
#*
#*      $Log:  $
#*
#*


###
ffolder.obj: ffolder.h
mailbox.obj: popcorn.h strng.h version.h passwd.h except.h ffolder.h outfile.h
mailboxes.obj: popcorn.h strng.h version.h passwd.h except.h
main.obj: popcorn.h strng.h version.h passwd.h except.h pop3.h tcp.h recode.h smtp.h
parse.obj: strng.h
pop3proto.obj: popcorn.h strng.h version.h passwd.h except.h pop3.h tcp.h recode.h
recode.obj: recode.h except.h strng.h recode.tbl
runsmail.obj: popcorn.h strng.h version.h passwd.h except.h
smtpproto.obj: popcorn.h strng.h version.h passwd.h except.h smtp.h tcp.h recode.h
strng.obj: strng.h except.h strng.h
tcp.obj: popcorn.h strng.h version.h passwd.h except.h tcp.h
uupcconf.obj: popcorn.h strng.h version.h passwd.h except.h
@


1.5
log
@*** empty log message ***
@
text
@d18 1
a18 1
OBJS= ffolder.obj mailbox.obj mailboxes.obj main.obj parse.obj pop3proto.obj recode.obj runsmail.obj smtpproto.obj strng.obj tcp.obj
d33 9
d46 1
a46 1
mailbox.obj: popcorn.h strng.h version.h passwd.h ffolder.h except.h outfile.h
d48 1
a48 1
main.obj: popcorn.h strng.h version.h passwd.h except.h pop3.h tcp.h smtp.h
d50 2
a51 2
pop3proto.obj: popcorn.h strng.h version.h passwd.h pop3.h tcp.h except.h
recode.obj: recode.h recode.tbl
d53 1
a53 1
smtpproto.obj: popcorn.h strng.h version.h passwd.h smtp.h tcp.h except.h
d56 1
@


1.4
log
@*** empty log message ***
@
text
@d4 4
a7 2
all: $(EXE)

d18 1
a18 3
OBJS= parse.obj strng.obj tcp.obj main.obj \
      mailbox.obj mailboxes.obj \
      pop3proto.obj smtpproto.obj
d36 11
a46 4
parse.obj: strng.h d:/ibmcpp/include/sys/types.h d:/ibmcpp/include/stdlib.h d:/ibmcpp/include/stdarg.h d:/usr/include/stl/cset/style.h d:/ibmcpp/include/string.h
pop3proto.obj: popcorn.h strng.h d:/ibmcpp/include/sys/types.h d:/ibmcpp/include/stdlib.h d:/ibmcpp/include/stdarg.h d:/usr/include/stl/cset/style.h d:/ibmcpp/include/string.h version.h
strng.obj: strng.h d:/ibmcpp/include/sys/types.h d:/ibmcpp/include/stdlib.h d:/ibmcpp/include/stdarg.h d:/usr/include/stl/cset/style.h d:/ibmcpp/include/string.h except.h strng.h d:/ibmcpp/include/ctype.h
tcp.obj: popcorn.h strng.h d:/ibmcpp/include/sys/types.h d:/ibmcpp/include/stdlib.h d:/ibmcpp/include/stdarg.h d:/usr/include/stl/cset/style.h d:/ibmcpp/include/string.h version.h
@


1.3
log
@tcp works somehow
@
text
@d16 3
a18 2
OBJS= parse.obj pop3proto.obj strng.obj tcp.obj main.obj \
      mailbox.obj mailboxes.obj
@


1.2
log
@*** empty log message ***
@
text
@d12 2
a13 1
CFLAGS= -Tdp -Q $(OP_PROFILE) $(OP_OPTIMIZE) $(OP_DEBUG)
@


1.1
log
@Initial revision
@
text
@d15 2
a16 1
OBJS= parse.obj pop3proto.obj strng.obj tcp.obj main.obj
@
