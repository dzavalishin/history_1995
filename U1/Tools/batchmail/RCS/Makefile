head	1.1;
access;
symbols;
locks;
comment	@# @;


1.1
date	96.09.22.11.11.44;	author dz;	state Exp;
branches;
next	;


desc
@@


1.1
log
@Initial revision
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

CFLAGS = $(DEB) /I$(IPATH) /Tdp /Ge+ /Ms /Q+ 

EXE=batchmail.exe

goal: mkmf $(EXE)

mkmf:
    mkmf -s

test: $(EXE)
#	-del *.cpy
#	rbmail.exe <batch
#	rbmail.exe <BSvExu.D


install: $(EXE)
	copy $(EXE) n:\net\uupc\bin


OBJS= load_stdin.obj main.obj


$(EXE): $(OBJS)
     $(CC) $(CFLAGS) -Fe$(EXE) $(OBJS) REXX.LIB BatchMail.Def

..\..\Out\OS2Dist.OUT\$(EXE): $(EXE)
  copy $(EXE) ..\..\Out\OS2Dist.OUT
  lxlite /d- ..\..\Out\OS2Dist.OUT\$(EXE)

clean:
	-rm $(OBJS)
        -mv *.?~ bak
        -mv *.brs bak



###
main.obj: batchmail.h ../../version.h
@
