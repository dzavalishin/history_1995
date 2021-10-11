head	1.2;
access;
symbols;
locks;
comment	@# @;


1.2
date	96.03.09.01.39.35;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.04.06.23.33.30;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@*** empty log message ***
@
text
@!include ..\..\dllver.mak


THEDLL=U1_$(DLLVER).DLL
DEFDLL=U1_$(DLLVER)
THELIB=U1_$(DLLVER).LIB

!ifndef NODEBUG
DEBLINK=/O"/DEB" /Ti
!endif

#ASM=/Fa
#LST=/Fl

CC = icc
CFLAGS = /O+ /Q+ /Ge- /Gi+ /Gs+ /Tdp /Ms /Ti $(ASM) $(LST)

LD = link386
LDFLAGS = /NOI /NOE /NoLogo $(LD_DEB)

EXE_DEPENDENCIES =  \
 _Dll_Init.obj strng.obj app.obj ffolder.obj\
 fidoaddr.obj fa2ua.obj fa2strng.obj \
 spool.obj hl_match.obj rfc.obj ftn.obj elist.obj elist_c.obj \
 freedom.obj log.obj recode.obj setup.obj set_tab.obj stat.obj \
 new_usr.obj fuserdef.obj userlist.obj \
 dateconv.obj datetok.obj getindat.obj rfc_date.obj \
 ua2fa.obj uucpaddr.obj uutraver.obj \
 FTN_Msg.obj FTN_Att.obj FTN_Date.obj FTN_Deb.obj FTN_New.obj \
 FTN_Rw.obj FTN_Flags.obj \
 RexxCall.obj RexxScom.obj RexxFunc.obj



#=============================================================
#		Explicit Rules
#=============================================================

all : makedep install # test.exe

install: $(THEDLL) $(THELIB) *.h
	copy $(THEDLL) C:\Local\DLL
#	attrib +r N:\Local\DLL\$(THEDLL)
	copy $(THELIB) ..\out
	copy *.h ..\out

clean:
	-rm $(EXE_DEPENDENCIES) $(THEDLL) $(THELIB)

makedep: DEPENDS.LST

DEPENDS.LST: $(EXE_DEPENDENCIES)
	makedep

$(THEDLL): $(EXE_DEPENDENCIES) U1.DEF makefile
   icc /Tdp /I$(IPATH) /B"/NOO" /B"/NOE" $(DEBLINK) /Ge- /Fe$(THEDLL) /Ms /Q $(EXE_DEPENDENCIES) REXX.LIB dde4cc.lib u1.def



#$(THEDLL): $(EXE_DEPENDENCIES) U1.DEF makefile
#	$(LD) $(LDFLAGS) @@<<
#_Dll_Init.obj+
#app.obj+
#ffolder.obj+
#strng.obj+
#fidoaddr.obj+
#fa2ua.obj+
#fa2strng.obj+
#spool.obj+
#hl_match.obj+
#rfc.obj+
#ftn.obj+
#elist.obj+
#elist_c.obj+
#freedom.obj+
#log.obj+
#recode.obj+
#setup.obj+
#set_tab.obj+
#stat.obj+
#new_usr.obj+
#fuserdef.obj+
#userlist.obj+
#dateconv.obj+
#datetok.obj+
#getindat.obj+
#FTN.obj+
#FTN_Att.obj+
#FTN_Date.obj+
#FTN_Deb.obj+
#FTN_New.obj+
#FTN_Rw.obj+
#FTN_Flags.obj+
#RexxCall.obj+
#RexxScom.obj+
#RexxFunc.obj+
#rfc_date.obj+
#ua2fa.obj+
#uucpaddr.obj+
#uutraver.obj
#$(THEDLL)
#u1dll.map
#REXX.LIB+
#dde4cc.lib
#u1.def
#<<

_dll_init.obj : _dll_init.c
	icc /O+ /Q+ /Gi+ /Gs+ /Ss /Ms /Ge- /c _dll_init.c

$(THELIB): $(THEDLL)
	IMPLIB /NoLogo $(THELIB) $(THEDLL)


checklib.obj : checklib.c
	icc /O+ /Q+ /Tx+ /Ti+ /Tm+ /Gi+ /Gs+ /Ms /Tdp /c checklib.c

test.exe: checklib.obj $(THELIB)
	$(LD) $(LDFLAGS) /M @@<<
checklib
test
test
$(THELIB)
test
<<





U1.DEF: makefile ..\..\dllver.mak ..\..\version.h
	cat >U1.DEF <<
LIBRARY $(DEFDLL) INITINSTANCE TERMINSTANCE
DESCRIPTION 'U1 Runtime Environment Version $(VER)'
DATA MULTIPLE READWRITE LOADONCALL
CODE LOADONCALL
PROTMODE
STACKSIZE 8192
HEAPSIZE 8192
<<









#RexxCall.obj: RexxCall.c

#RexxScom.obj: RexxScom.c

#rexxfunc.obj: rexxfunc.c


!include depends.lst

@


1.1
log
@Initial revision
@
text
@d1 158
a158 1
!include os2.mak@
