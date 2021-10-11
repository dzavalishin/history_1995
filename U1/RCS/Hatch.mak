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
@!include version.mak

alpha:
   copy $(DIR)\$(FILE) $(DIR)\AU1_$(VER).Zip
   N:\Net\NEF\nef -cN:\Net\NEF\NEF.CFG match $(DIR)\AU1_$(VER).Zip UU2 "U1 $(VER) Alpha"

@
