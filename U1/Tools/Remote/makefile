!include ..\..\dllver.mak

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
  icc /Tdp /I$(IPATH) $(DEBLINK) /Fe$(EXE) /Ms /Q $(EXE_DEPENDENCIES) $(DLL_LIB) REXX.LIB gremote.Def

out: ..\..\Out\OS2Dist.OUT\$(EXE)

..\..\Out\OS2Dist.OUT\$(EXE): $(EXE)
  copy $(EXE) ..\..\Out\OS2Dist.OUT
#  lxlite /d- ..\..\Out\OS2Dist.OUT\$(EXE)

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
#    Revision 1.3  1995/11/06 14:28:09  dz
#    Current
#
#    Revision 1.2  1995/08/02 13:50:28  dz
#    update
#
#    Revision 1.1  1995/07/01  21:21:09  dz
#    Initial revision
#
#