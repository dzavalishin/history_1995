head	1.3;
access;
symbols;
locks;
comment	@#   @;


1.3
date	95.11.05.18.51.07;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.11.05.17.37.36;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.11.04.01.23.00;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@rfc_msg
,.
@
text
@!include ..\..\dllver.mak

!ifndef NODEBUG
DEB=/Ti+
DEBLINK=/B"/DEB"
!endif

IPATH = ..\..\lib\Out
DLL_LIB=$(IPATH)\U1_$(DLLVER).LIB
DLL_DLL=$(IPATH)\U1_$(DLLVER).DLL

EXE=uu2fido.exe

CC = icc $(DEB) /I$(IPATH) /Tdp /Ge+ /Ms /Q+


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
 sendlett.obj \
 sendfile.obj \
 send.obj \
 sendspool.obj \
 main.obj \
 runsmail.obj \
 rfc_msg.obj \
 rfc_load.obj \
 address.obj

# uuparse.obj \
# CallRexx.obj \
# RexxScom.obj \
# rexxfunc.obj \


#=============================================================
#		Explicit Rules
#=============================================================
$(EXE): $(EXE_DEPENDENCIES) $(DLL_LIB) makedep 
  icc /Tdp /I$(IPATH) /B"/NOO" $(DEBLINK) /Fe$(EXE) /Ms /Q $(EXE_DEPENDENCIES) dde4cc.lib $(DLL_LIB) uu2fido.Def

# dde4cc.lib 

out: ..\..\Out\OS2Dist.OUT\$(EXE)

..\..\Out\OS2Dist.OUT\$(EXE): $(EXE)
  copy $(EXE) ..\..\Out\OS2Dist.OUT

clean:
	-rm $(EXE_DEPENDENCIES) $(EXE)

makedep: DEPENDS.LST

DEPENDS.LST: $(EXE_DEPENDENCIES)
	makedep




#=============================================================
#		Individual File Dependencies
#=============================================================
rfc_msg.h: hl.h rfc_bags.h hl_bag.h

#uuparse.obj: uuparse.c uu2fido.h 

sendspool.obj: sendspool.c  uu2fido.h 

sendlett.obj: sendlett.c  uu2fido.h 

sendfile.obj: sendfile.c  uu2fido.h 

send.obj: send.c  uu2fido.h 

runsmail.obj: runsmail.c  uu2fido.h 

main.obj: main.c  uu2fido.h rfc_msg.h

address.obj: address.c  uu2fido.h 

rfc_load.obj: rfc_load.c uu2fido.h 

rfc_msg.obj: rfc_msg.c uu2fido.h rfc_msg.h

!include depends.lst
@


1.2
log
@Using FTN_Msg
@
text
@a37 7
 FTN.obj \
 FTN_Att.obj \
 FTN_Date.obj \
 FTN_Deb.obj \
 FTN_New.obj \
 FTN_Rw.obj \
 FTN_Flags.obj \
@


1.1
log
@Initial revision
@
text
@a31 2
 rfc_msg.obj \
 rfc_load.obj \
d38 9
a46 1
 uuparse.obj \
d49 1
d84 1
a84 1
uuparse.obj: uuparse.c uu2fido.h 
@
