head	1.5;
access;
symbols;
locks;
comment	@#   @;


1.5
date	96.03.27.10.46.53;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.11.06.01.42.16;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.11.05.18.53.13;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.08.08.13.43.14;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.08.02.12.55.03;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@current
@
text
@!include ..\..\dllver.mak

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
PH_GENERATE=/Fi+
PH_USE=/Si+

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
  icc /Tdp /B"/NOO" $(DEBLINK) /Fe$(EXE) /Ms /Q $(EXE_DEPENDENCIES) $(DLL_LIB) /I$(IPATH) dde4cc.lib fido2uu.Def

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

@


1.4
log
@Using FTN_Msg! Seems to be working.
@
text
@d17 5
a21 1
CC = icc $(DEB) $(OPT) /I$(IPATH) /Tdp /Ge+ /Ms /Q+
d61 1
@


1.3
log
@*** empty log message ***
@
text
@d5 1
a5 1
#DEBLINK=/B"/DEB"
@


1.2
log
@*** empty log message ***
@
text
@d51 1
a51 1
  icc /Tdp /B"/NOO" $(DEBLINK) /Fe$(EXE) /Ms /Q $(EXE_DEPENDENCIES) $(DLL_LIB) dde4cc.lib fido2uu.Def
@


1.1
log
@Initial revision
@
text
@d6 3
d17 1
a17 1
CC = icc $(DEB) /I$(IPATH) /Tdp /Ge+ /Ms /Q+
@
