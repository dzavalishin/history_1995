head	1.1;
access;
symbols;
locks;
comment	@#   @;


1.1
date	95.08.02.13.27.55;	author dz;	state Exp;
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

EXE=batch.exe

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
 lists.obj \
 getopt.obj \
 feeds.obj \
 cmpgroup.obj \
 cmppath.obj \
 compress.obj \
 put_art.obj \
 send.obj \
 scan.obj \
 main.obj \
 mktemp.obj

#=============================================================
#		Explicit Rules
#=============================================================
$(EXE): $(EXE_DEPENDENCIES) $(DLL_LIB)
  icc $(DEBLINK) /Fe$(EXE) /Ms /Q $(EXE_DEPENDENCIES) $(DLL_LIB) Batch.Def

out: ..\..\Out\OS2Dist.OUT\$(EXE)

..\..\Out\OS2Dist.OUT\$(EXE): $(EXE)
  copy $(EXE) ..\..\Out\OS2Dist.OUT

clean:
	-rm $(EXE_DEPENDENCIES) $(EXE)

#=============================================================
#		Individual File Dependencies
#=============================================================
mktemp.obj: mktemp.c 

lists.obj: lists.c 

getopt.obj: getopt.c 

feeds.obj: feeds.c 

cmpgroup.obj: cmpgroup.c 

cmppath.obj: cmppath.c

compress.obj: compress.c 

put_art.obj: put_art.c 

send.obj: send.c 

scan.obj: scan.c 

main.obj: main.c 

@
