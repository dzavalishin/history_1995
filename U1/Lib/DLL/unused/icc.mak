#OUT=ICC_OS2.OBJ
#BC=d:\bin\bcos2
DEB=/Ti+

#.AUTODEPEND
#.PATH.obj = $(OUT)

#=============================================================
#		Translator Definitions
#=============================================================
CC = icc $(DEB) /Tdp /Ge- /Ms /Q+

#TLINK = tlink
#LIBPATH = $(BC)\LIB;$(BC)\classlib\lib;..\..\lib\out\os2
#INCLUDEPATH = $(BC)\INCLUDE;$(BC)\classlib\INCLUDE;..\..\lib\out\os2


#=============================================================
#		Implicit Rules
#=============================================================
.c.obj:
  $(CC) /c $<

.cpp.obj:
  $(CC) /c $<

#=============================================================
#		List Macros
#=============================================================


EXE_DEPENDENCIES =  \
 _Dll_Init.obj strng.obj \
 msg_rw.obj msg_new.obj msg_gs.obj msg_date.obj msg_att.obj \
 msg.obj flags_kl.obj fidoaddr.obj fa2ua.obj fa2strng.obj \
 spool.obj hl_match.obj rfc.obj ftn.obj elist.obj elist_c.obj \
 freedom.obj log.obj recode.obj setup.obj set_tab.obj stat.obj \
 new_usr.obj fuserdef.obj userlist.obj \
 dateconv.obj datetok.obj getindat.obj rfc_date.obj \
 ua2fa.obj uucpaddr.obj uutraver.obj


#=============================================================
#		Explicit Rules
#=============================================================
u1.dll: $(EXE_DEPENDENCIES)
  link.cmd

#  icc $(DEB) /Feu1.dll /Ge- /Ms /Q+ U1.Def $(EXE_DEPENDENCIES)

u1.lib: u1.dll
	IMPLIB /NoLogo u1.lib u1.dll

CheckLib.obj: CheckLib.c
	icc $(DEB) /c /Tdp /Ge+ /Ms /Q+ CheckLib.c

test.exe: CheckLib.obj u1.lib
	icc $(DEB) /B"/DEB" /Fetest.exe /Ms /Q+ CheckLib.obj U1.Lib


test: test.exe




