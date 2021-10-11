!include ..\..\dllver.mak

!ifndef NODEBUG
DEB=/Ti+
#DEBLINK=/B"/DEB"
!endif

IPATH = ..\..\lib\Out
DLL_LIB=$(IPATH)\U1_$(DLLVER).LIB
DLL_DLL=$(IPATH)\U1_$(DLLVER).DLL

EXE=snews.exe

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
 Unpack.obj \
 Send.obj \
 SendUtil.obj \
 Read.obj \
 Origin.obj \
 Main.obj \
 Scan.obj

#=============================================================
#		Explicit Rules
#=============================================================
$(EXE): $(EXE_DEPENDENCIES) $(DLL_LIB)
  icc $(DEBLINK) /B"/NOE" /Fe$(EXE) /I$(IPATH) /Tdp /Ms /Q $(EXE_DEPENDENCIES) SETARGV.OBJ $(DLL_LIB) SNews.Def

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
unpack.obj: unpack.c 

send.obj: send.c 

SendUtil.obj: SendUtil.c

scan.obj: scan.c 

read.obj: read.c 

origin.obj: origin.c 

main.obj: main.c 
