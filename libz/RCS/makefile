head	1.3;
access;
symbols;
locks;
comment	@# @;


1.3
date	96.09.23.03.32.01;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.09.23.03.28.28;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.23.03.19.42;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@*** empty log message ***
@
text
@!include dllver.mak

!ifndef NODEBUG
DEB=-Ti
DEBLINK=-B"/DEB"
#OPT=-O
!else
OPT=-O
!endif

MAKE=make /nologo 

#STL=;D:\usr\Include\stl\cset 

#IPATH = .;..;..\Out
IPATH = .

#DLL_LIB=ZLIB_$(DLLVER).LIB
#DLL_DLL=ZLIB_$(DLLVER).DLL

THEDLL=U1_$(DLLVER).DLL
THELIB=U1_$(DLLVER).LIB
DEFDLL=U1_$(DLLVER)
DEF=ZLIB.DEF



#EXE=test.exe

#Precompiled headers:  $(PH_GENERATE) $(PH_USE)
#PH_GEN=/Fi+
#PH_USE=/Si+

#SYSTEM_LINKAGE
#SYS_LINK=/Ms

COMMON_CC=$(DEB) $(OPT) /I$(IPATH) /Tdp /Q+ $(SYS_LINK) $(PH_GEN) $(PH_USE) -Le -Lj -Ls

CC =     icc /Ge- $(COMMON_CC)
EXE_CC = icc /Ge+ $(COMMON_CC)



OBJS = zsema.obj zstrng.obj



#=============================================================
#		Explicit Rules
#=============================================================

all: mkmf lib



mkmf:
    mkmf -s



#=============================================================
#               DLL
#=============================================================


install: $(THEDLL) $(THELIB) *.h
	copy $(THEDLL) C:\Local\DLL
	attrib +r C:\Local\DLL\$(THEDLL)
	copy $(THELIB) ..\out
	copy *.h ..\out

dll: $(THELIB)

lib: $(THELIB)


#unlock:
#	attrib -r C:\Local\DLL\$(THEDLL)

#again: unlock all

#reinstall: unlock all

clean:
	-rm $(OBJS) $(THEDLL) $(THELIB)
        -mkdir bak
        -move *~ bak


$(THEDLL): $(OBJS) $(DEF) makefile
	icc /Tdp /I$(IPATH) /B"/NOE" $(DEBLINK) /Ge- /Fe$(THEDLL) /Ms /Q $(OBJS) REXX.LIB $(DEF)


_dll_init.obj : _dll_init.c
	icc /Ti /O+ /Q+ /Gi+ /Gs+ /Ss /Ms /Ge- /c _dll_init.c

$(THELIB): $(THEDLL)
	IMPLIB /NoLogo $(THELIB) $(THEDLL)

$(DEF): makefile ..\dllver.mak
	cat >$(DEF) <<
LIBRARY $(DEFDLL) INITINSTANCE TERMINSTANCE
DESCRIPTION 'dz general library $(VER)'
DATA MULTIPLE READWRITE LOADONCALL
CODE LOADONCALL
PROTMODE
STACKSIZE 81920
HEAPSIZE 81920
<<



###
zsema.obj: zsema.h zexcept.h zstrng.h zstyle.h
zstrng.obj: zstrng.h zexcept.h
@


1.2
log
@*** empty log message ***
@
text
@d1 1
a1 1
!include ..\dllver.mak
d44 1
a44 1
OBJS =  
d114 2
a115 8








@


1.1
log
@Initial revision
@
text
@d4 3
a6 3
DEB=/Ti+
DEBLINK=/B"/DEB"
#OPT=/O
d8 1
a8 1
OPT=/O
d15 2
a16 1
IPATH = .;..;..\Out
d18 2
a19 2
DLL_LIB=U1_$(DLLVER).LIB
DLL_DLL=U1_$(DLLVER).DLL
d22 1
d24 2
a25 1
THELIB=U1_$(DLLVER).LIB
d28 1
a28 1
EXE=test.exe
a42 5
#=============================================================
#		Implicit Rules
#=============================================================
.c.obj:
  $(CC) /c $<
d44 1
a44 58
.cpp.obj:
  $(CC) /c $<

#=============================================================
#		List Macros
#=============================================================

EXE_DEPENDENCIES = test.obj

DLL_DEPENDENCIES =  \
_Dll_Init.obj \
rfc_msgid.obj \
Stat.obj \
FreeDom.obj \
Ign_Name.obj \
RexxCall.obj \
RexxFunc.obj \
RexxScom.obj \
spool.obj \
io_file.obj \
dt_DateTok.obj \
dt_GetInDat.obj \
dt_timeline.obj \
dt_tm2time.obj \
RFC_Load.obj \
RFC_Msg.obj \
rfc_hl.obj \
Recode.obj \
Tic.obj \
Tic_Clr.obj \
Tic_Fill.obj \
Tic_kw.obj \
Tic_Save.obj \
EList.obj \
ftn_tab.obj \
UserList.obj \
App.obj  \
FA2UA.obj  \
FTN_Msg.obj  \
FTN_Msg_Att.obj  \
FTN_Msg_Date.obj  \
FTN_Msg_Deb.obj  \
FTN_Msg_Flags.obj  \
FTN_Msg_New.obj  \
FTN_Msg_Rw.obj  \
Ffolder.obj  \
Fidoaddr.obj  \
Parse.obj  \
Set_Tab.obj \
Setup_file.obj  \
Setup_item.obj  \
Setup_item_kw.obj  \
Setup_kw.obj  \
Setup_storage.obj  \
UA2FA.obj  \
UUCPAddr.obj  \
log.obj  \
strng.obj 
d52 1
a52 6
all: makedep
	$(MAKE) lib 
	$(MAKE) $(EXE)

test.obj: test.c
    $(EXE_CC) test.c
a53 5
$(EXE): $(DLL_LIB) $(EXE_DEPENDENCIES)
	icc /Tdp /B"/NOE" \
      $(DEBLINK) /I$(IPATH) \
      /Fe$(EXE) /Ms /Q $(EXE_DEPENDENCIES) $(DLL_LIB) \
      REXX.LIB test.Def
a54 1
#  icc /Tdp /B"/NOE" $(DEBLINK) /I.;..;$(IPATH);$(STL) /Fe$(EXE) /Ms /Q  $(DLL_LIB) /I$(IPATH) test.Def
d56 2
a57 1
#out: ..\..\Out\OS2Dist.OUT\$(EXE)
a58 16
#..\..\Out\OS2Dist.OUT\$(EXE): $(EXE)
#  copy $(EXE) ..\..\Out\OS2Dist.OUT
#  lxlite /d- ..\..\Out\OS2Dist.OUT\$(EXE)


makedep: DEPENDS.LST

DEPENDS.LST: $(DLL_DEPENDENCIES) $(EXE_DEPENDENCIES)
	makedep


#=============================================================
#		Individual File Dependencies
#=============================================================

!include depends.lst
d77 2
a78 2
unlock:
	attrib -r C:\Local\DLL\$(THEDLL)
d80 1
a80 1
again: unlock all
d82 1
a82 1
reinstall: unlock all
d85 3
a87 2
	-rm $(EXE_DEPENDENCIES) $(DLL_DEPENDENCIES) $(THEDLL) $(THELIB)
	-move *~ bak
d90 2
a91 2
$(THEDLL): $(DLL_DEPENDENCIES) U1.DEF makefile
	icc /Tdp /I$(IPATH) /B"/NOE" $(DEBLINK) /Ge- /Fe$(THEDLL) /Ms /Q $(DLL_DEPENDENCIES) REXX.LIB u1.def
d100 2
a101 3

U1.DEF: makefile ..\dllver.mak ..\version.h
	cat >U1.DEF <<
d103 1
a103 1
DESCRIPTION 'U1 Runtime Environment Version $(VER)'
d113 1
a113 2

!include depends.lst
@
