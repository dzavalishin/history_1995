.AUTODEPEND

.PATH.obj = OBJ

#		*Translator Definitions*
CC = tcc +TIF2ISG.CFG
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
  tiff.obj \
  tif2isg.obj \
  ..\pack\pack.obj

#		*Explicit Rules*
tif2isg.exe: tif2isg.cfg $(EXE_dependencies)
  $(TLINK) /v/x/n/c @&&|
/o- e:\tc\lib\c0l.obj+
obj\tiff.obj+
obj\tif2isg.obj+
..\pack\pack.obj
tif2isg,
e:\tc\lib\graphics.lib+
e:\tc\lib\cl.lib
|


#		*Individual File Dependencies*
tiff.obj: tiff.cpp 

tif2isg.obj: tif2isg.cpp 

#		*Compiler Configuration File*
tif2isg.cfg: tif2isg.mak
  copy &&|
-ml
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
-wucp
-nOBJ
-IE:\TC\INCLUDE; ..\PACK
-LE:\TC\LIB
-j5
-g10
| tif2isg.cfg


