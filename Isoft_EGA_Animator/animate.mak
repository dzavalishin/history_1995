.AUTODEPEND

.PATH.obj = .

#		*Translator Definitions*
CC = tcc +ANIMATE.CFG
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
  draw_isg.obj \
  load_isg.obj \
  thru_isg.obj \
  ega.obj \
  scene.obj \
  area.obj \
  hero_cd.obj \
  list.obj \
  main.obj \
  pack.obj

#		*Explicit Rules*
animate.exe: animate.cfg $(EXE_dependencies)
  $(TLINK) /v/x/c @&&|
e:\tc\lib\c0l.obj+
.\draw_isg.obj+
.\load_isg.obj+
.\thru_isg.obj+
.\ega.obj+
.\scene.obj+
.\area.obj+
.\hero_cd.obj+
.\list.obj+
.\main.obj+
.\pack.obj
animate,
e:\tc\lib\cl.lib
|


#		*Individual File Dependencies*
draw_isg.obj: draw_isg.cpp 

load_isg.obj: load_isg.cpp 

thru_isg.obj: thru_isg.cpp 

ega.obj: ega.cpp 

scene.obj: scene.cpp 

area.obj: area.cpp 

hero_cd.obj: hero_cd.cpp 

list.obj: list.cpp 

main.obj: main.cpp 

pack.obj: pack\pack.cpp 
	$(CC) -c pack\pack.cpp

#		*Compiler Configuration File*
animate.cfg: animate.mak
  copy &&|
-ml!
-2
-f-
-ff-
-K
-v
-y
-G
-O
-Z
-V
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
-IE:\TC\INCLUDE; .\PACK
-LE:\TC\LIB
-j5
-g10
| animate.cfg


