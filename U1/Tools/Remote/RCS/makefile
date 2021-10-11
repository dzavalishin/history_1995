head	1.3;
access;
symbols;
locks;
comment	@#   @;


1.3
date	95.11.06.14.28.09;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.08.02.13.50.28;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.07.01.21.21.09;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@Current
@
text
@!include ..\..\dllver.mak

!ifndef NODEBUG
DEB=/Ti+
#DEBLINK=/B"/DEB"
!endif

IPATH = ..\..\lib\Out
DLL_LIB=$(IPATH)\U1_$(DLLVER).LIB
DLL_DLL=$(IPATH)\U1_$(DLLVER).DLL

EXE=gremote.exe

#Precompiled headers: 
PH_GENERATE=/Fi+
PH_USE=/Si+

CC = icc $(DEB) /I$(IPATH) /Tdp /Ge+ /Ms /Q+ $(PH_GENERATE) $(PH_USE)


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
 e_rexx.obj \
 e_help.obj \
 e_list.obj \
 e_stat.obj \
 e_broad.obj \
 e_notify.obj \
 mainloop.obj \
 select.obj \
 address.obj \
 send_err.obj \
 process.obj \
 RexxGr.obj \
 main.obj

# ffolder.obj \

#=============================================================
#		Explicit Rules
#=============================================================
$(EXE): $(EXE_DEPENDENCIES) $(DLL_LIB)
  icc /Tdp /B"/NOO" /I$(IPATH) $(DEBLINK) /Fe$(EXE) /Ms /Q $(EXE_DEPENDENCIES) $(DLL_LIB) dde4cc.lib REXX.LIB gremote.Def

out: ..\..\Out\OS2Dist.OUT\$(EXE)

..\..\Out\OS2Dist.OUT\$(EXE): $(EXE)
  copy $(EXE) ..\..\Out\OS2Dist.OUT

clean:
	-rm $(EXE_DEPENDENCIES) $(EXE)


#=============================================================
#		Individual File Dependencies
#=============================================================
main.obj: main.c GRemote.h

mainloop.obj: mainloop.c GRemote.h

select.obj: select.c GRemote.h

address.obj: address.c GRemote.h

send_err.obj: send_err.c GRemote.h

process.obj: process.c GRemote.h Cmds.h

RexxGr.obj: RexxGr.c GRemote.h Cmds.h

e_rexx.obj: e_rexx.c GRemote.h Cmds.h

e_help.obj: e_help.c GRemote.h Cmds.h

e_list.obj: e_list.c GRemote.h Cmds.h

e_notify.obj: e_notify.c GRemote.h Cmds.h

e_stat.obj: e_stat.c GRemote.h Cmds.h

e_broad.obj: e_broad.c GRemote.h Cmds.h

#ffolder.obj: ffolder.c ffolder.h


#
#
#    $Log: makefile $
#    Revision 1.2  1995/08/02 13:50:28  dz
#    update
#
#   Revision 1.1  1995/07/01  21:21:09  dz
#   Initial revision
#
#@


1.2
log
@update
@
text
@d5 1
a5 1
DEBLINK=/B"/DEB"
d14 5
a18 1
CC = icc $(DEB) /I$(IPATH) /Tdp /Ge+ /Ms /Q+
d36 6
a41 2
 ffolder.obj \
 main.obj \
d48 3
a50 6
 e_rexx.obj \
 e_help.obj \
 e_list.obj \
 e_notify.obj \
 e_stat.obj \
 e_broad.obj 
d56 1
a56 1
  icc /Tdp /B"/NOO" $(DEBLINK) /Fe$(EXE) /Ms /Q $(EXE_DEPENDENCIES) $(DLL_LIB) dde4cc.lib REXX.LIB gremote.Def
d96 1
a96 1
ffolder.obj: ffolder.c ffolder.h
d102 3
@


1.1
log
@Initial revision
@
text
@d5 1
a5 1
#DEBLINK=/B"/DEB"
d32 1
d39 2
d51 1
a51 1
  icc /Tdp /B"/NOO" $(DEBLINK) /Fe$(EXE) /Ms /Q $(EXE_DEPENDENCIES) $(DLL_LIB) dde4cc.lib gremote.Def
d77 4
d91 1
a91 1

d96 4
a99 1
#    $Log$
@
