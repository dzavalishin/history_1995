!include ..\..\dllver.mak

!ifndef NODEBUG
DEB=/Ti+
DEBLINK=/B"/DEB"
!endif

IPATH = ..\..\lib\Out
DLL_LIB=$(IPATH)\U1_$(DLLVER).LIB
DLL_DLL=$(IPATH)\U1_$(DLLVER).DLL

EXE=uu2fido.exe

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
 sendlett.obj \
 sendfile.obj \
 send.obj \
 sendspool.obj \
 main.obj \
 runsmail.obj \
 address.obj

# rfc_msg.obj \
# rfc_load.obj \
# uuparse.obj \
# CallRexx.obj \
# RexxScom.obj \
# rexxfunc.obj \


#=============================================================
#		Explicit Rules
#=============================================================
$(EXE): $(EXE_DEPENDENCIES) $(DLL_LIB) makedep 
  icc /Tdp /I$(IPATH) $(DEBLINK) /Fe$(EXE) /Ms /Q $(EXE_DEPENDENCIES) $(DLL_LIB) uu2fido.Def
# dde4cc.lib

# dde4cc.lib 

out: ..\..\Out\OS2Dist.OUT\$(EXE)

..\..\Out\OS2Dist.OUT\$(EXE): $(EXE)
  copy $(EXE) ..\..\Out\OS2Dist.OUT
#  lxlite /d- ..\..\Out\OS2Dist.OUT\$(EXE)

clean:
	-rm $(EXE_DEPENDENCIES) $(EXE)

makedep: DEPENDS.LST

DEPENDS.LST: $(EXE_DEPENDENCIES)
	makedep




#=============================================================
#		Individual File Dependencies
#=============================================================
#rfc_msg.h: hl.h rfc_bags.h hl_bag.h

#uuparse.obj: uuparse.c uu2fido.h 

sendspool.obj: sendspool.c  uu2fido.h 

sendlett.obj: sendlett.c  uu2fido.h 

sendfile.obj: sendfile.c  uu2fido.h 

send.obj: send.c  uu2fido.h 

runsmail.obj: runsmail.c  uu2fido.h 

main.obj: main.c  uu2fido.h

address.obj: address.c  uu2fido.h 

#rfc_load.obj: rfc_load.c uu2fido.h 

#rfc_msg.obj: rfc_msg.c uu2fido.h rfc_msg.h

!include depends.lst
