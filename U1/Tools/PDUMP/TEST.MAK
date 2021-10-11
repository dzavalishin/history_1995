.AUTODEPEND

.PATH.obj = .

#		*Translator Definitions*
CC = bcc +TEST.CFG
TASM = TASM
TLIB = tlib
TLINK = tlink
LIBPATH = D:\BOR\BC\LIB;\lib\fido
INCLUDEPATH = D:\BOR\BC\INCLUDE;\lib\fido;d:\include


#		*Implicit Rules*
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

#		*List Macros*


EXE_dependencies =  \
 main.obj

#		*Explicit Rules*
.\test.exe: test.cfg $(EXE_dependencies)
  $(TLINK) /v/x/n/c/P-/L$(LIBPATH) @&&|
c0s.obj+
.\main.obj
.\test
		# no map file
cs.lib
|


#		*Individual File Dependencies*
main.obj: test.cfg main.c 

#		*Compiler Configuration File*
test.cfg: test.mak
  copy &&|
-2
-a
-f-
-ff-
-K
-j20
-N
-v
-y
-G
-O
-Z
-vi
-H=TEST.SYM
-w-ret
-w-nci
-w-inl
-w-par
-w-cpt
-w-dup
-w-pia
-w-ill
-w-sus
-w-ext
-w-ias
-n.
-I$(INCLUDEPATH)
-L$(LIBPATH)
-P.C
| test.cfg


