head	1.1;
access;
symbols;
locks;
comment	@#   @;


1.1
date	95.03.11.18.26.07;	author dz;	state Exp;
branches;
next	;


desc
@@


1.1
log
@Initial revision
@
text
@#=============================================================
#
#	OS2.MAK - Makefile for project D:\UU2\NEWS\BATCH\batch2.prj
#		Created on 01/15/94 at 20:04
#
#=============================================================

.AUTODEPEND

.PATH.obj = BCC_OS2.OBJ

#=============================================================
#		Translator Definitions
#=============================================================
CC = bcc +OS2.CFG
TASM = tasm.exe
TLIB = tlib.exe
TLINK = tlink
RC = brcc.exe
RB = rc.exe
LIBPATH = d:\BIN\BCOS2\LIB;d:\BIN\BCOS2\classlib\LIB;..\..\lib\OUT\OS2
INCLUDEPATH = d:\BIN\BCOS2\INCLUDE;d:\BIN\BCOS2\classlib\INCLUDE;..\..\lib\OUT\OS2


#=============================================================
#		Implicit Rules
#=============================================================
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

.asm.obj:
  $(TASM) -Mx $*.asm,$*.obj

.rc.res:
  $(RC) -r $*.rc

#=============================================================
#		List Macros
#=============================================================


EXE_DEPENDENCIES =  \
 {$(LIBPATH)}uu2.lib \
 {$(LIBPATH)}uucp.lib \
 {$(LIBPATH)}fido.lib \
 mktemp.obj \
 lists.obj \
 getopt.obj \
 feeds.obj \
 cmpgroup.obj \
 cmppath.obj \
 compress.obj \
 put_art.obj \
 send.obj \
 scan.obj \
 main.obj

#=============================================================
#		Explicit Rules
#=============================================================
BCC_OS2.OBJ\batch.exe: os2.cfg $(EXE_DEPENDENCIES)
  $(TLINK) /v /Toe /ap /L$(LIBPATH) -S:10000 @@&&|
d:\BIN\BCOS2\LIB\C02.OBJ+
BCC_OS2.OBJ\mktemp.obj+
BCC_OS2.OBJ\lists.obj+
BCC_OS2.OBJ\getopt.obj+
BCC_OS2.OBJ\feeds.obj+
BCC_OS2.OBJ\cmpgroup.obj+
BCC_OS2.OBJ\CMPPATH.obj+
BCC_OS2.OBJ\compress.obj+
BCC_OS2.OBJ\put_art.obj+
BCC_OS2.OBJ\send.obj+
BCC_OS2.OBJ\scan.obj+
BCC_OS2.OBJ\main.obj
BCC_OS2.OBJ\batch,BCC_OS2.OBJ\batch
uu2.lib+
uucp.lib+
fido.lib+
d:\BIN\BCOS2\LIB\BIDS2.LIB+
d:\BIN\BCOS2\LIB\C2I.LIB+
d:\BIN\BCOS2\LIB\OS2.LIB

|


#=============================================================
#		Individual File Dependencies
#=============================================================
MKTEMP.obj: OS2.cfg MKTEMP.C 

LISTS.obj: OS2.cfg LISTS.C 

GETOPT.obj: OS2.cfg GETOPT.C 

FEEDS.obj: OS2.cfg FEEDS.C 

CMPGROUP.obj: OS2.cfg CMPGROUP.C 

CMPPATH.obj: OS2.cfg cmppath.c

COMPRESS.obj: OS2.cfg COMPRESS.C 

PUT_ART.obj: OS2.cfg PUT_ART.C 

SEND.obj: OS2.cfg SEND.C 

SCAN.obj: OS2.cfg SCAN.C 

MAIN.obj: OS2.cfg MAIN.C 

#=============================================================
#		Compiler Configuration File
#=============================================================
!if 1
OS2.cfg: OS2.mak
  copy &&|
-L$(LIBPATH)
-I$(INCLUDEPATH)
-nBCC_OS2.OBJ
-P
-v
-wnod
-wuse
-wstv
-wsig
-wdef
-wcln
-wamp
-wamb
-N
| OS2.cfg




!endif@
