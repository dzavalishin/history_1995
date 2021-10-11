!include ..\..\dllver.mak

!ifndef NODEBUG
DEB=/Ti+
DEBLINK=/B"/DEB"
#OPT=/O
!else
OPT=/O
!endif

IPATH = ..\..\lib\Out
DLL_LIB=$(IPATH)\U1_$(DLLVER).LIB
DLL_DLL=$(IPATH)\U1_$(DLLVER).DLL

EXE=fido2uu.exe

#Precompiled headers:  $(PH_GENERATE) $(PH_USE)
#PH_GENERATE=/Fi+
#PH_USE=/Si+

CC = icc $(DEB) $(OPT) /I$(IPATH) /Tdp /Ge+ /Ms /Q+ $(PH_GENERATE) $(PH_USE)


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
 address.obj \
 send_err.obj \
 send_ack.obj \
 runsmail.obj \
 gatectl.obj \
 convert.obj \
 select.obj \
 mainloop.obj \
 main.obj \
 uuencode.obj \
 encode.obj

#=============================================================
#		Explicit Rules
#=============================================================
$(EXE): $(EXE_DEPENDENCIES) $(DLL_LIB)
  icc /Tdp $(DEBLINK) /Fe$(EXE) /Ms /Q $(EXE_DEPENDENCIES) $(DLL_LIB) /I$(IPATH) fido2uu.Def
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
encode.obj: encode.c 

address.obj: address.c 

uuencode.obj: uuencode.c 

send_err.obj: send_err.c 

send_ack.obj: send_ack.c 

runsmail.obj: runsmail.c 

gatectl.obj: gatectl.c 

convert.obj: convert.c 

select.obj: select.c 

mainloop.obj: mainloop.c 

main.obj: main.c 

