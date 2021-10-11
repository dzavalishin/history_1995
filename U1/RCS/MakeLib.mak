head	1.1;
access;
symbols;
locks;
comment	@# @;


1.1
date	96.03.09.01.36.22;	author dz;	state Exp;
branches;
next	;


desc
@@


1.1
log
@Initial revision
@
text
@!include dllver.mak

OUT=Lib\Out\U1_$(DLLVER).LIB
SRCD=Lib\DLL

$(OUT): Makelib.mak Version.h $(SRCD)\*.c $(SRCD)\*.h *.h
	cd $(SRCD)
#	nmake clean
	nmake install

Makelib.mak: dllver.mak

dllver.mak: version.h
	@@echo nmake dllver.mak, please
        @@exit 1

@
