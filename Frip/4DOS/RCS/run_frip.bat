head	1.2;
access;
symbols;
locks;
comment	@:: @;


1.2
date	96.07.30.08.30.49;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.07.30.08.30.13;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@*** empty log message ***
@
text
@rem $Id: run_frip.bat 1.1 1996/07/30 08:30:13 dz Exp $

rem �㦭� ��p��� � ��⠫��, ��� ����� frip
rem ������ �ਯ �� ������ ᠬ, �� ��譨� ࠧ �� ����蠥�

n:
cd \net\frip

rem move n:\spool\in\*.ri? .


rem �᫨ �� ��� ��p����� p��� ��� �p���p�� ��� ��
rem �����⢠ - p�᪮�����p�� �ਥ����騥 � ᫥���騥 ��p���
rem mkdir old
rem copy *.ri? old

frip_dos

@


1.1
log
@Initial revision
@
text
@d1 1
a1 1
rem $Id: run_frip.cmd 1.2 1996/07/30 08:27:43 dz Exp $
d5 1
d9 2
a10 3
rem ������ �ਯ �� ������ ᠬ
rem move n:\spool\in\*.rip .
rem move n:\spool\in\*.riz .
d15 1
a15 5
rem copy *.rip old

rem ������ �ਯ �� ������ ᠬ
rem pkunzip *.riz *.rip
rem del *.riz
a18 4
rem ren *.ri0 *.rip
rem if exist *.rip frip
rem ren *.ri1 *.ri0
rem if exist *.rip frip
@
