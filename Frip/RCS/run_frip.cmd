head	1.1;
access;
symbols;
locks; strict;
comment	@:: @;


1.1
date	95.10.03.14.36.05;	author dz;	state Exp;
branches;
next	;


desc
@@


1.1
log
@Initial revision
@
text
@rem нужно пеpейти в каталог, где живет frip
n:
cd \net\frip

move n:\spool\in\*.rip .
rem move n:\spool\in\*.ri0 .
rem move n:\spool\in\*.ri1 .
rem ren *.ri0 *.rip
rem ren *.ri1 *.rip

call frip

@
