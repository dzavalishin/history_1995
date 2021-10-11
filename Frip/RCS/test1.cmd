head	1.2;
access;
symbols;
locks;
comment	@:: @;


1.2
date	95.10.03.14.36.05;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.10.03.11.45.34;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@*** empty log message ***
@
text
@/* */

do forever 

call go '2_5020_251.0@@fidonet.org'
call go '2_5020_144.0@@fidonet.org'
call go '2_5020_180.0@@fidonet.org'
call go '2_5020_269.0@@fidonet.org'
call go '2_5020_50.0@@fidonet.org'
call go '2_5020_69.0@@fidonet.org'
call go '2_50_0.0@@fidonet.org'
call go '2_5020_32.0@@fidonet.org'

call syssleep 10+random(1,15)

end

go: procedure
dir='u:\dz\frip\'arg(1)

old=directory( dir )

call frip

rc=directory(old)

return


@


1.1
log
@Initial revision
@
text
@d5 1
a5 2
call syssleep 10+random(1,15)

d12 3
d19 1
a19 1
dir=arg(1)
d25 1
a25 1
directory(old)
@
