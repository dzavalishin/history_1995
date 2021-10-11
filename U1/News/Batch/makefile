!include ..\..\dllver.mak

!ifndef NODEBUG
DEB=/Ti+
#DEBLINK=/B"/DEB"
!endif

IPATH = ..\..\lib\Out
DLL_LIB=$(IPATH)\U1_$(DLLVER).LIB
DLL_DLL=$(IPATH)\U1_$(DLLVER).DLL

EXE=batch.exe

#Precompiled headers
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
 lists.obj \
 feeds.obj \
 cmpgroup.obj \
 cmppath.obj \
 compress.obj \
 put_art.obj \
 send.obj \
 scan.obj \
 main.obj \
 mktemp.obj

# getopt.obj \


#=============================================================
#		Explicit Rules
#=============================================================
$(EXE): $(EXE_DEPENDENCIES) $(DLL_LIB)
  icc $(DEBLINK) /Fe$(EXE) /Ms /Q /I$(IPATH) /Tdp $(EXE_DEPENDENCIES) $(DLL_LIB) Batch.Def

# dde4cc.lib

out: ..\..\Out\OS2Dist.OUT\$(EXE)

..\..\Out\OS2Dist.OUT\$(EXE): $(EXE)
  copy $(EXE) ..\..\Out\OS2Dist.OUT
#  lxlite /d- ..\..\Out\OS2Dist.OUT\$(EXE)

clean:
	-rm $(EXE_DEPENDENCIES) $(EXE)

#=============================================================
#		Individual File Dependencies
#=============================================================
mktemp.obj: mktemp.c 

lists.obj: lists.c 

#getopt.obj: getopt.c 

feeds.obj: feeds.c 

cmpgroup.obj: cmpgroup.c 

cmppath.obj: cmppath.c

compress.obj: compress.c 

put_art.obj: put_art.c 

send.obj: send.c 

scan.obj: scan.c 

main.obj: main.c 

