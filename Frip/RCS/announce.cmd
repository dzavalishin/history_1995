head	1.4;
access;
symbols;
locks;
comment	@ *  @;


1.4
date	95.10.03.14.36.05;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.10.03.11.45.34;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.10.03.09.51.32;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.10.02.18.51.42;	author dz;	state Exp;
branches;
next	;


desc
@@


1.4
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
/**
 *
 * Сию хpень надо запускать когда изменились файлы *.ad
 *
 * read announcements file, generate ads as if we received 'em
 *
 * parameters: confdir (or ., if none given)
 *
 * $Log: announce.cmd $
 * Revision 1.3  1995/10/03 11:45:34  dz
 * *** empty log message ***
 *
 * Revision 1.2  1995/10/03 09:51:32  dz
 * *** empty log message ***
 *
 * Revision 1.1  1995/10/02 18:51:42  dz
 * Initial revision
 *
 *
**/

if 0 then trace intermediates

parse arg confdir .

if confdir = '' then confdir = '.'

/*

Each record contains an address (or mask) we are ready to forward mail for,
and, optionally, list of addresses (or masks) this ad shouldn't make it to.
All those exceptions are simply added to seenby list of ad.

It is assumed that all of those addresses are our akas or masks, if we
pretend to route for entire net or region (or zone, or all the domains?)

*/


rc = SysFileTree( confdir'\*.ad', 'Files', 'FO' );
if \ rc = 0 then do
   Say 'Problems reading contents of 'confdir
   return 0
   end

do j = 1 to Files.0
	Say ''
	Say 'Looking at 'Files.j
	call do_one_file Files.j, confdir
end

exit 0

do_one_file: procedure
in=arg(1)
dir=arg(2)


do while lines(in)
        line=strip(linein(in))
        if(substr(line,1,1)=';') then
                iterate
        if(line='') then
                iterate

	call do_ad line, dir

end

return



do_ad: procedure
line=arg(1)
dir=arg(2)

parse var line addr tail

addr=fcanon(addr)

say ''
say 'ad: 'addr

temp=systempFileName(dir'/tmp?????.rip')

if temp='' then
	do
	say '*** aborted: no temp file name'
	return
	end

call lineout temp, 'from myself'
call lineout temp, 'hops 0'
call lineout temp, 'created 'date('e')' 'time('n')
call lineout temp, 'ad 'addr

do forever
	parse var tail '-'except tail
	except=strip(except)
	if except == '' then leave
	except=fcanon(except)
	call lineout temp, 'seenby 'except
	say '    - 'except
end

call stream temp, 'C', 'close'

return



@


1.3
log
@*** empty log message ***
@
text
@d3 8
d16 3
@


1.2
log
@*** empty log message ***
@
text
@d8 3
d17 2
d42 1
d55 1
a55 1
        line=linein(in)
d57 2
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 * $Log:$
d93 2
@
