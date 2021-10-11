head	1.1;
access;
symbols;
locks;
comment	@#   @;


1.1
date	95.08.02.13.33.53;	author dz;	state Exp;
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

EXE=snews.exe

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
 unpack.obj \
 send.obj \
 read.obj \
 origin.obj \
 main.obj \
 scan.obj

#=============================================================
#		Explicit Rules
#=============================================================
$(EXE): $(EXE_DEPENDENCIES) $(DLL_LIB)
  icc $(DEBLINK) /B"/NOE" /Fe$(EXE) /Ms /Q $(EXE_DEPENDENCIES) SETARGV.OBJ $(DLL_LIB) SNews.Def


out: ..\..\Out\OS2Dist.OUT\$(EXE)

..\..\Out\OS2Dist.OUT\$(EXE): $(EXE)
  copy $(EXE) ..\..\Out\OS2Dist.OUT

clean:
	-rm $(EXE_DEPENDENCIES) $(EXE)

#=============================================================
#		Individual File Dependencies
#=============================================================
unpack.obj: unpack.c 

send.obj: send.c 

scan.obj: scan.c 

read.obj: read.c 

origin.obj: origin.c 

main.obj: main.c 
@
