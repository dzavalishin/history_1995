.AUTODEPEND

.PATH.obj = .

#		*Translator Definitions*
CC = tcc +PACK.CFG
TASM = TASM
TLINK = tlink


#		*Implicit Rules*
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

.asm.obj:
  $(TASM) {$< }


#		*List Macros*


EXE_dependencies =  \
  pack.obj

#		*Explicit Rules*
pack.exe: pack.cfg $(EXE_dependencies)
  $(TLINK) /v/x/n/c @&&|
e:\tc\lib\c0t.obj+
.\pack.obj
pack,
e:\tc\lib\graphics.lib+
e:\tc\lib\cs.lib
|


#		*Individual File Dependencies*
pack.obj: pack.cpp 

#		*Compiler Configuration File*
pack.cfg: pack.mak
  copy &&|
-mt
-f-
-ff-
-K
-N
-v
-y
-G
-O
-Z

-V
-P
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
-wucp
-n.
-IE:\TC\INCLUDE
-LE:\TC\LIB
-j5
| pack.cfg


