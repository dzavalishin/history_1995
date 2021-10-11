OBJS = OS2_obj
OUT=OS2.lib
BC=d:\bin\bcos2

.AUTODEPEND
.PATH.obj = $(OBJS)

#		*Translator Definitions*
CC = bcc +OS2.CFG
TASM = TASM
TLIB = tlib
TLINK = tlink
LIBPATH = $(BC)\LIB
INCLUDEPATH = $(BC)\include;..\FIDO;..\UUCP;.;..\..
#;$(BC)\CLASSLIB\INCLUDE


#		*Implicit Rules*
.c.obj:
  $(CC) -c -n$(OBJS) {$< }

.cpp.obj:
  $(CC) -c -n$(OBJS) {$< }

#		*List Macros*


LIB_dependencies =  \
 spool.obj \
 hl_match.obj \
 rfc.obj \
 ftn.obj \
 elist.obj \
 elist_c.obj \
 freedom.obj \
 log.obj \
 recode.obj \
 setup.obj \
 set_tab.obj \
 stat.obj \
 new_usr.obj \
 fuserdef.obj \
 userlist.obj

#		*Explicit Rules*
$(OUT): OS2.cfg $(LIB_dependencies)
  - del $<
  $(TLIB) $<  @&&|
-+$(OBJS)\spool.obj &
-+$(OBJS)\hl_match.obj &
-+$(OBJS)\rfc.obj &
-+$(OBJS)\ftn.obj &
-+$(OBJS)\elist.obj &
-+$(OBJS)\elist_c.obj &
-+$(OBJS)\freedom.obj &
-+$(OBJS)\log.obj &
-+$(OBJS)\recode.obj &
-+$(OBJS)\setup.obj &
-+$(OBJS)\set_tab.obj &
-+$(OBJS)\stat.obj &
-+$(OBJS)\new_usr.obj &
-+$(OBJS)\fuserdef.obj &
-+$(OBJS)\userlist.obj
|

install: $(OUT_DIR)\uu2.lib

$(OUT_DIR)\uu2.lib: $(OUT)
    copy $(OUT) $<
    copy *.h $(OUT_DIR)

#               *Individual File Dependencies*
spool.obj: OS2.cfg spool.c

hl_match.obj: OS2.cfg hl_match.c

rfc.obj: OS2.cfg rfc.c

ftn.obj: OS2.cfg ftn.c

elist.obj: OS2.cfg elist.c

elist_c.obj: OS2.cfg elist_c.c

freedom.obj: OS2.cfg freedom.c

log.obj: OS2.cfg log.c

recode.obj: OS2.cfg recode.c

setup.obj: OS2.cfg setup.c

set_tab.obj: OS2.cfg set_tab.c

stat.obj: OS2.cfg stat.c

new_usr.obj: OS2.cfg new_usr.c

fuserdef.obj: OS2.cfg fuserdef.c

userlist.obj: OS2.cfg userlist.c

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
-wpin
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
| OS2.cfg




