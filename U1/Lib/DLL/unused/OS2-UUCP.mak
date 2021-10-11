OBJS = os2_obj
OUT=os2.lib

.AUTODEPEND
.PATH.obj = $(OBJS)

#		*Translator Definitions*
CC = bcc +OS2.CFG
TASM = TASM
TLIB = tlib
TLINK = tlink
LIBPATH = f:\BIN\BCOS2\LIB
INCLUDEPATH = f:\BIN\BCOS2\INCLUDE;..\FIDO;..\U1;.
#;D:\BIN\BCOS2\CLASSLIB\INCLUDE


#		*Implicit Rules*
.c.obj:
  $(CC) -c -n$(OBJS) {$< }

.cpp.obj:
  $(CC) -c -n$(OBJS) {$< }

#		*List Macros*


LIB_dependencies =  \
dateconv.obj    datetok.obj     getindat.obj    rfc_date.obj    \
ua2fa.obj       uucpaddr.obj    uutraver.obj

#		*Explicit Rules*
$(OUT): os2.cfg $(LIB_dependencies)
  - del $(OUT)
  $(TLIB) $<  @&&|
-+$(OBJS)\dateconv.obj &
-+$(OBJS)\datetok.obj  &
-+$(OBJS)\getindat.obj &
-+$(OBJS)\rfc_date.obj &
-+$(OBJS)\ua2fa.obj    &
-+$(OBJS)\uucpaddr.obj &
-+$(OBJS)\uutraver.obj
|

install: $(OUT_DIR)\uucp.lib

$(OUT_DIR)\uucp.lib: $(OUT)
     copy $(OUT) $(OUT_DIR)\uucp.lib
     copy *.h $(OUT_DIR)



#		*Individual File Dependencies*

dateconv.obj: os2.cfg   dateconv.c

datetok.obj: os2.cfg    datetok.c

getindat.obj: os2.cfg   getindat.c

rfc_date.obj: os2.cfg   rfc_date.c

ua2fa.obj: os2.cfg      ua2fa.c

uucpaddr.obj: os2.cfg   uucpaddr.c

uutraver.obj: os2.cfg   uutraver.c


#		*Compiler Configuration File*
os2.cfg: os2.mak
  copy &&|
-a
-f-
-ff-
-K-
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
-w-ucp
-wuse
-weas
-wpre
-n.
-I$(INCLUDEPATH)
-L$(LIBPATH)
-P.C
| os2.cfg



