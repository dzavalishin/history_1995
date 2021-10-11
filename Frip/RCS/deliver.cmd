head	1.3;
access;
symbols;
locks;
comment	@ *  @;


1.3
date	95.10.03.14.36.05;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.10.03.11.45.55;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.10.02.16.18.10;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@*** empty log message ***
@
text
@/**
 *
 * The software included, file formats and basic algorithms are
 * copyright (C) 1995 by Dmitry Zavalishin. All rights reserved.
 *
**/

/* Это тестовый deliver! Замените на настоящий пpи инсталляции! */

a=arg(1)
parse var a dest file

if dest = '' | file = '' then
	do
	say 'deliver <address> <file>'
	say '(and you given me address='dest' and file='file')'
	exit 2
	end

dest='u:\dz\frip\'translate(dest,'___',':/\');

rc=sysmkdir( dest )
'move 'file' 'dest

exit RC
@


1.2
log
@*** empty log message ***
@
text
@d1 8
a8 1
/* */
d23 1
a23 1
'move 'file' u:\dz\frip\'dest
@


1.1
log
@Initial revision
@
text
@d3 2
a4 1
parse arg dest file
d9 1
d13 1
a13 1
dest=translate(dest,'___',':/\');
d15 1
a15 1
'mkdir 'dest
@
