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
@kill/sent
@


1.1
log
@Initial revision
@
text
@/**
 *
 * The software included, file formats and basic algorithms are
 * copyright (C) 1995 by Dmitry Zavalishin. All rights reserved.
 *
**/

/* Сей deliver посылает с помошью сквиша */

a=arg(1)
parse var a dest file

if dest = '' | file = '' then
	do
	say 'deliver <address> <file>'
	say '(and you given me address='dest' and file='file')'
	exit 2
	end

'squish send "^'file'" 'dest

exit RC
@
