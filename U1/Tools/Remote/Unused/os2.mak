OUT=BCC_OS2.OBJ
BC=d:\bin\bcos2
#DEB=-v

#.AUTODEPEND
#.PATH.obj = $(OUT)

#		*Translator Definitions*
CC = bcc +OS2.CFG $(DEB)
TASM = TASM
TLIB = tlib
TLINK = tlink
LIBPATH = $(BC)\LIB;..\..\LIB\OUT\OS2
INCLUDEPATH = $(BC)\INCLUDE;..\..\LIB\OUT\OS2;..\..;$(BC)\INCLUDE\CLASSLIB


#		*Implicit Rules*
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

#		*List Macros*


EXE_dependencies =  \
 main.obj \
 mainloop.obj \
 select.obj \
 address.obj \
 send_err.obj \
 process.obj \
 e_help.obj \
 e_list.obj \
 e_notify.obj \
 e_stat.obj \
 e_broad.obj 
# {$(LIBPATH)}fido.lib \
# {$(LIBPATH)}uucp.lib \
# {$(LIBPATH)}uu2.lib

#		*Explicit Rules*
$(OUT)\gremote.exe: os2.cfg $(EXE_dependencies)
  $(TLINK) $(DEB) /x /Toe /ap -S:100000 /L$(LIBPATH) @&&|
 c02.obj+
$(OUT)\main.obj+
$(OUT)\mainloop.obj+
$(OUT)\select.obj+
$(OUT)\address.obj+
$(OUT)\send_err.obj+
$(OUT)\process.obj+
$(OUT)\e_help.obj+
$(OUT)\e_list.obj+
$(OUT)\e_notify.obj+
$(OUT)\e_stat.obj+
$(OUT)\e_broad.obj
$(OUT)\gremote
		# no map file
fido.lib+
uucp.lib+
uu2.lib+
bids2.lib+
c2i.lib +
os2.lib
|


#		*Individual File Dependencies*
$(OUT)\main.obj: os2.cfg main.c 

$(OUT)\mainloop.obj: os2.cfg mainloop.c 

$(OUT)\select.obj: os2.cfg select.c 

$(OUT)\address.obj: os2.cfg address.c 

$(OUT)\send_err.obj: os2.cfg send_err.c 

$(OUT)\process.obj: os2.cfg process.c 

$(OUT)\e_help.obj: os2.cfg e_help.c 

$(OUT)\e_list.obj: os2.cfg e_list.c 

$(OUT)\e_notify.obj: os2.cfg e_notify.c 

$(OUT)\e_stat.obj: os2.cfg e_stat.c 

$(OUT)\e_broad.obj: os2.cfg e_broad.c 

#		*Compiler Configuration File*
os2.cfg: os2.mak
  copy &&|
-a
-f-
-ff-
-j20
-v
-y
-G
-O
-vi-
-wbbf
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
-n$(OUT)
-I$(INCLUDEPATH)
-L$(LIBPATH)
-P.C
-N
-H-
| os2.cfg






#-H=$(OUT)\GREMOTE.SYM
