head	1.2;
access;
symbols;
locks;
comment	@:: @;


1.2
date	96.07.30.08.27.43;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.07.30.08.26.03;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@*** empty log message ***
@
text
@rem $Id: run_frip.cmd 1.1 1996/07/30 08:26:03 dz Exp $

rem �㦭� ��p��� � ��⠫��, ��� ����� frip

n:
cd \net\frip

rem move n:\spool\in\*.ri? .

rem �᫨ �� ��� ��p����� p��� ��� �p���p�� ��� ��
rem �����⢠ - p�᪮�����p�� ᫥���騥 ��p���
rem mkdir old
rem copy *.ri? old

frip

@


1.1
log
@Initial revision
@
text
@d1 2
a2 1
rem $Id$
d4 1
d8 1
a8 10
rem move n:\spool\in\*.rip .
rem move n:\spool\in\*.riz .

rem unzip -Cjn *.riz *.rip
rem �᫨ unzip �� �������� *.riz, ���p���� ��p����, 㪠���� ����:
rem for %%i in (*.zip) do unzip -Cjn %%i *.rip
rem ����, �ਯ ����⠭ �� UnZip 5.12 of 28 August 1994, by Info-ZIP.

rem move n:\spool\in\*.ri0 .
rem move n:\spool\in\*.ri1 .
d13 1
a13 4
rem copy *.rip old
rem copy *.riz old

rem del *.riz
a16 4
rem ren *.ri0 *.rip
rem if exist *.rip frip
rem ren *.ri1 *.ri0
rem if exist *.rip frip
@
