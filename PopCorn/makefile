
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

OBJS= ffolder.obj httpproto.obj mailbox.obj mailboxes.obj main.obj parse.obj pop3proto.obj recode.obj runsmail.obj smtpproto.obj strng.obj tcp.obj uupcconf.obj

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
#*      $Log: makefile $
#*      Revision 1.6  1996/09/17 23:20:43  dz
#*      First completely working POP3/ESMTP version
#*
#*
#*


###
ffolder.obj: ffolder.h
httpproto.obj: popcorn.h strng.h version.h passwd.h except.h http.h tcp.h recode.h
mailbox.obj: popcorn.h strng.h version.h passwd.h except.h ffolder.h outfile.h
mailboxes.obj: popcorn.h strng.h version.h passwd.h except.h
main.obj: popcorn.h strng.h version.h passwd.h except.h pop3.h tcp.h recode.h smtp.h http.h
parse.obj: strng.h
pop3proto.obj: popcorn.h strng.h version.h passwd.h except.h pop3.h tcp.h recode.h
recode.obj: recode.h except.h strng.h recode.tbl
runsmail.obj: popcorn.h strng.h version.h passwd.h except.h
smtpproto.obj: popcorn.h strng.h version.h passwd.h except.h smtp.h tcp.h recode.h
strng.obj: strng.h except.h strng.h
tcp.obj: popcorn.h strng.h version.h passwd.h except.h tcp.h
uupcconf.obj: popcorn.h strng.h version.h passwd.h except.h
