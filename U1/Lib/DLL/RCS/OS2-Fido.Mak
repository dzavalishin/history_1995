head	1.1;
access;
symbols;
locks; strict;
comment	@# @;


1.1
date	95.04.06.23.33.30;	author dz;	state Exp;
branches;
next	;


desc
@@


1.1
log
@Initial revision
@
text
@OBJS = os2_obj
OUT=os2.lib

.AUTODEPEND
.PATH.obj = $(OBJS)

#		*Translator Definitions*
CC = bcc +OS2.CFG
TASM = TASM
TLIB = tlib
TLINK = tlink
LIBPATH = f:\bin\BCOS2\LIB
INCLUDEPATH = f:\bin\BCOS2\INCLUDE;..\UUCP;..\U1;.
#;d:\bin\BCOS2\CLASSLIB\INCLUDE


#		*Implicit Rules*
.c.obj:
  $(CC) -c -n$(OBJS) {$< }

.cpp.obj:
  $(CC) -c -n$(OBJS) {$< }

#		*List Macros*


LIB_dependencies =  \
 msg_rw.obj \
 msg_new.obj \
 msg_gs.obj \
 msg_date.obj \
 msg_att.obj \
 msg.obj \
 flags_kl.obj \
 fidoaddr.obj \
 fa2ua.obj \
 fa2strng.obj
# tic_save.obj \
# tic_kw.obj \
# tic_fill.obj \
# tic_clr.obj \
# tic.obj 

#		*Explicit Rules*
$(OUT): os2.cfg $(LIB_dependencies)
  - del $(OUT)
  $(TLIB) $<  @@&&|
-+$(OBJS)\msg_rw.obj &
-+$(OBJS)\msg_new.obj &
-+$(OBJS)\msg_gs.obj &
-+$(OBJS)\msg_date.obj &
-+$(OBJS)\msg_att.obj &
-+$(OBJS)\msg.obj &
-+$(OBJS)\flags_kl.obj &
-+$(OBJS)\fidoaddr.obj &
-+$(OBJS)\fa2ua.obj &
-+$(OBJS)\fa2strng.obj
|

#-+$(OBJS)\tic_save.obj &
#-+$(OBJS)\tic_kw.obj &
#-+$(OBJS)\tic_fill.obj &
#-+$(OBJS)\tic_clr.obj &
#-+$(OBJS)\tic.obj &


install: $(OUT_DIR)\fido.lib

$(OUT_DIR)\fido.lib: $(OUT)
     copy $(OUT) $(OUT_DIR)\fido.lib
     copy *.h $(OUT_DIR)

#       *Individual File Dependencies*


TIC_SAVE.obj: OS2.cfg TIC_SAVE.C

TIC_KW.obj: OS2.cfg TIC_KW.C

TIC_FILL.obj: OS2.cfg TIC_FILL.C

TIC_CLR.obj: OS2.cfg TIC_CLR.C

TIC.obj: OS2.cfg TIC.C

MSG_RW.obj: OS2.cfg MSG_RW.C

MSG_NEW.obj: OS2.cfg MSG_NEW.C

MSG_GS.obj: OS2.cfg MSG_GS.C

MSG_DATE.obj: OS2.cfg MSG_DATE.C

MSG_ATT.obj: OS2.cfg MSG_ATT.C

MSG.obj: OS2.cfg MSG.C

FLAGS_KL.obj: OS2.cfg FLAGS_KL.C

FIDOADDR.obj: OS2.cfg FIDOADDR.C

FA2UA.obj: OS2.cfg FA2UA.C

FA2STRNG.obj: OS2.cfg FA2STRNG.C


#		*Compiler Configuration File*
OS2.cfg: OS2.mak
  copy &&|
-a
-f-
-ff-
-j20
-N
-v
-y
-G
-O
-Oc
-Oe
-Ob
-Z
-vi-
-wbbf
-w-pin
-wamb
-wamp
-wasm
-wpro
-wcln
-wdef
-wsig
-wnod
-wstv
-wuse
-weas
-wpre
-n.
-I$(INCLUDEPATH)
-L$(LIBPATH)
-P.C
-H=$(OBJS)\headers.sym
| os2.cfg


@
