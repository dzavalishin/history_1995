head	1.1;
access;
symbols;
locks;
comment	@#   @;


1.1
date	95.08.02.13.39.57;	author dz;	state Exp;
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
#BROWSER=/Fb+
DEB=/Ti+ /Gh $(BROWSER)
#DEBLINK=/B"/DEB" /B"/NOE" $(BROWSER)
#XTRA=DDE4XTRA.OBJ
#XTRALINK=/Gh
!endif


IPATH = ..\..\lib\Out
DLL_LIB=$(IPATH)\U1_$(DLLVER).LIB
DLL_DLL=$(IPATH)\U1_$(DLLVER).DLL

EXE=unbatch.exe

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
FTNFrom.obj Open_PKT.obj SendNews.obj SendFile.obj ToSpool.obj chg_ext.obj \
decomp.obj get_art.obj gethdr.obj getopt.obj gr2area.obj kiae_unb.obj \
main.obj mktemp.obj scan.obj unbatch.obj uuparse.obj vread.obj vread_bf.obj \
vread_fi.obj vread_wi.obj


#=============================================================
#		Explicit Rules
#=============================================================

$(EXE): $(EXE_DEPENDENCIES) $(DLL_LIB) makefile
	icc /Fe$(EXE) /Ms /Q+ /B"/NOO" $(DEBLINK) $(XTRALINK) $(EXE_DEPENDENCIES) $(XTRA) $(DLL_LIB) Unbatch.def


out: ..\..\Out\OS2Dist.OUT\$(EXE)

..\..\Out\OS2Dist.OUT\$(EXE): $(EXE)
  copy $(EXE) ..\..\Out\OS2Dist.OUT

clean:
	-rm $(EXE_DEPENDENCIES) *.brs UNBATCH.TRC $(EXE)


@
