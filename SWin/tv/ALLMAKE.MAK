#
#	Common part of make files for IDA.
#

# .AUTODEPEND

#--------------------------- Directories --------------------------
BCOS2_DIR=c:\bcos2\\
BCDOS_DIR=c:\bcc\\

#########################
!ifdef __WATCOMC__			# Watcom
IMPLIB=implib
ASM=wasm
CC=wpp386
LINKER=ld $(_LDFLAGS)
!ifdef __MSDOS__			# Watcom DOS 32
__DOS32__=1
CFLAGS=@$(IDA)d32watp.cfg $(_CFLAGS)
CCL=ld -c -l=dos4g /\"option stack=8192\" $(_CFLAGS) $(_LDFLAGS)
LINKSYS=dos4g
MAP=.mp3
!elif $d(__DLL__)			# Watcom OS/2 DLL
__OS2WATDLL__=1
CFLAGS=@$(IDA)os2watd.cfg $(_CFLAGS)
LINKSYS=os2v2 dll
MAP=.mpd
!else					# Watcom OS/2 prog
__OS2WATPRG__=1
CCL=ld -c $(_CFLAGS) $(_LDFLAGS)
CFLAGS=@$(IDA)os2watp.cfg $(_CFLAGS)
LINKSYS=os2v2
MAP=.mpw
!endif
!else					# Borland C++
!ifdef __MSDOS__			# Borland C++ under MSDOS
__DOS16__=1
LINKER=tlink /m $(_LDFLAGS)
IMPLIB=implib
ASM=tasm
CC=bcc
CFLAGS=+$(IDA)idacmpb.cfg $(_CFLAGS)
CCL=$(CC) $(CFLAGS) $(_LDFLAGS)
C_STARTUP=$(BCDOS_DIR)lib\c0l
C_LIB=cl
MAP=.map
!else					# Borland C++ under OS/2
LINKER=tlink /c/m $(_LDFLAGS)
IMPLIB=$(BCOS2_DIR)bin\implib
ASM=tasm
CC=bcc
CFLAGS=+$(IDA)os2cmpb.cfg $(_CFLAGS)
CCL=$(CC) $(CFLAGS) $(_LDFLAGS)
C_STARTUP=$(BCOS2_DIR)lib\c02
CD_STARTUP=$(BCOS2_DIR)lib\c02d
C_LIB=c2mt os2
MAP=.mp2
!endif
!endif

#########################
I=$(IDA)include\\
H=$(IDA)help\\		# Helps dir

# File extensions
#	object
!ifdef __WATCOMC__			# Watcom
AFLAGS=/d__WATCOM__ /3r /mf /e $(_AFLAGS)
!ifdef __MSDOS__			# Watcom MSDOS 32
L=$(IDA)lib.w32\\
TL=$(IDA)tvision\lib.w32\\
R=$(IDA)bin\\
B=3.exe
RS=$(R)os2\\
BS=.exe
F=w32\\
T=.aw3
IDP=.d32
IDPSTUB=$(L)inirt386$(O)
!elif $d(__DLL__)			# Watcom OS/2 DLL
L=$(IDA)lib.owd\\
TL=$(IDA)tvision\lib.owd\\
R=$(IDA)bin\os2\\
B=.exe
RS=$(R)
BS=.exe
F=owd\\
T=.awd
IDP=.dll
!else					# Watcom OS/2 prog
L=$(IDA)lib.osw\\
TL=$(IDA)tvision\lib.osw\\
R=$(IDA)bin\os2\\
B=.exe
BS=.exe
RS=$(R)
F=osw\\
T=.arw
IDP=.dll
!endif					# endif
O=.obj
A=.lib
API=.apw
DEF=.dfw
OUTSW=/fe=
OUTDLL=/bd
!else					# Borland C++
!ifdef __MSDOS__			# Borland C++ MSDOS
AFLAGS=/t/ml/z/m5$(_AFLAGS)
L=$(IDA)lib\\
TL=$(IDA)tvision\lib\\
R=$(IDA)bin\\
RS=$(R)
F=
O=.obj
A=.lib
T=.ar
B=.exe
BS=.exe
API=.api
DEF=.def
IDP=.idp
IDPSTUB=$(L)idpstub$(O) 
OVROFF=-Yo-
OVRON=-Yo
OUTSW=-e
OUTDLL=/Twd
!else					# Borland C++ under OS/2
AFLAGS=/D__FLAT__ /t/ml/zi/m5$(_AFLAGS)
L=$(IDA)lib.os2\\
TL=$(IDA)tvision\lib.os2\\
R=$(IDA)bin\os2\\
F=os2\\
O=.obj
A=.lib
T=.ar2
B=2.exe
RS=$(R)
BS=2.exe
API=.ap2
DEF=.df2
IDP=.bll
IDPSTUB=$(CD_STARTUP)
IDP_OS2_LIB=$(C_LIB)
OUTSW=-n -e
OUTDLL=/Tod
!endif
!endif

!if 0		# this is for makedep
F=
CFLAGS=+$(IDA)idacmpb.cfg
!endif

#	help source
HS=.hls
#	help headers
HH=.hhp

HC=$(RS)hc$(BS)		# Help Compiler
!ifdef __DLL__
AR=$(R)ard$(B) ruv	# Librarian
!else
AR=$(R)ar$(B) ruv	# Librarian
!endif
RM=$(R)rm		# File Remover

IDALIB=$(L)ida$(A)

HLIB=$(H)_ida.hlp

########################################################################
!ifdef __WATCOMC__
.cpp$(O):
        $(CC) $(CFLAGS) $<
.c$(O):
        $(CC) $(CFLAGS) $<
.asm$(O):
        $(ASM) $(AFLAGS) /fo=$(F)$* $*
!else
.cpp$(O):
        $(CC) $(CFLAGS) -c {$< }
.c$(O):
        $(CC) $(CFLAGS) -c {$< }
.asm$(O):
        $(ASM) $(AFLAGS) $*,$(F)$*
!endif

.hls.hhp:
	$(HC) -t $(HLIB) -i$@ $?
$(API)$(DEF):
	$(R)api2df$(BS) $? $@
$(DEF)$(A):
	$(IMPLIB) $@ $? >nul
########################################################################
