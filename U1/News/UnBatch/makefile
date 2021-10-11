!include ..\..\dllver.mak

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

#Precompiled headers:  $(PH_GENERATE) $(PH_USE)
#PH_GENERATE=/Fi+
#PH_USE=/Si+

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
FTNFrom.obj Open_PKT.obj SendNews.obj SendFile.obj ToSpool.obj chg_ext.obj \
decomp.obj get_art.obj gethdr.obj kiae_unb.obj \
main.obj mktemp.obj scan.obj unbatch.obj uuparse.obj vread.obj vread_bf.obj \
vread_fi.obj vread_wi.obj


#=============================================================
#		Explicit Rules
#=============================================================

$(EXE): $(EXE_DEPENDENCIES) $(DLL_LIB) makefile
	icc /Fe$(EXE) /I$(IPATH) /Tdp /Ms /Q+ $(DEBLINK) $(XTRALINK) $(EXE_DEPENDENCIES) $(XTRA) $(DLL_LIB) Unbatch.def
# dde4cc.lib


out: ..\..\Out\OS2Dist.OUT\$(EXE)

..\..\Out\OS2Dist.OUT\$(EXE): $(EXE)
  copy $(EXE) ..\..\Out\OS2Dist.OUT
#  lxlite /d- ..\..\Out\OS2Dist.OUT\$(EXE)

clean:
	-rm $(EXE_DEPENDENCIES) *.brs UNBATCH.TRC $(EXE)


