.AUTODEPEND

.PATH.obj = BCC_DOS.OBJ

#		*Translator Definitions*
CC = bcc +DOS.CFG
TASM = TASM
TLIB = tlib
TLINK = tlink
LIBPATH = D:\BIN\BC.DOS\LIB;\UU2\LIB\OUT\DOS;D:\BIN\BC.DOS\CLASSLIB\LIB
INCLUDEPATH = D:\BIN\BC.DOS\INCLUDE;\UU2\LIB\OUT\DOS;D:\BIN\BC.DOS\CLASSLIB\INCLUDE;..\..


#		*Implicit Rules*
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

#		*List Macros*


EXE_dependencies =  \
 main.obj \
 scanmail.obj \
 getflo.obj \
 getfile.obj \
 {$(LIBPATH)}fido.lib \
 {$(LIBPATH)}uu2.lib \
 {$(LIBPATH)}uucp.lib

#		*Explicit Rules*
bcc_dos.obj\getfatt.exe: DOS.CFG $(EXE_dependencies)
  $(TLINK) /v/x/n/c/P-/L$(LIBPATH)/o @&&|
/o- c0l.obj+
bcc_dos.obj\main.obj+
bcc_dos.obj\scanmail.obj+
bcc_dos.obj\getflo.obj+
bcc_dos.obj\getfile.obj
bcc_dos.obj\getfatt
		# no map file
fido.lib+
uu2.lib+
uucp.lib+
overlay.lib+
tclassl.lib+
cl.lib
|


#		*Individual File Dependencies*
main.obj: DOS.CFG main.c 

scanmail.obj: DOS.CFG scanmail.c 

getflo.obj: DOS.CFG getflo.c 

getfile.obj: DOS.CFG getfile.c 

#		*Compiler Configuration File*
DOS.CFG: dos.mak
  copy &&|
-ml
-f-
-ff-
-j20
-N
-v
-y
-G
-O
-Y
-h
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
-wuse
-weas
-wpre
-nBCC_DOS.OBJ
-I$(INCLUDEPATH)
-L$(LIBPATH)
-P.C
| DOS.CFG


