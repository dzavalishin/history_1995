head	1.9;
access;
symbols;
locks;
comment	@ *  @;


1.9
date	95.11.01.03.45.17;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	95.10.28.19.19.33;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	95.10.27.23.48.42;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	95.10.27.21.54.46;	author dz;	state Exp;
branches
	1.6.1.1;
next	1.5;

1.5
date	95.10.04.15.01.37;	author dz;	state Exp;
branches;
next	1.4;

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

1.6.1.1
date	95.10.27.22.58.11;	author dz;	state Exp;
branches;
next	;


desc
@@


1.9
log
@beta 4
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
 * read rip files, generate ads, update database, create routing file
 *
 * parameters: confdir (or ., if none given)
 *
 * $Log: frip.cmd $
 * Revision 1.8  1995/10/28 19:19:33  dz
 * Now we automatically add all of our AKAs to outgoig
 * rip seenbys, so that we'll never get it back to our
 * other AKA, as it happened before.
 *
 * Revision 1.7  1995/10/27 23:48:42  dz
 * Colouring, misc. details.
 *
 * Revision 1.6.1.1  1995/10/27 22:58:11  dz
 * Version from 256 - this one needs
 * config file and does not loose everything
 * when wildcard ad received. Wildcards are
 * assumed as such only if our akas does not
 * match those wildcards.
 *
 * Revision 1.6  1995/10/27 21:54:46  dz
 * SysLoadFuncs
 *
 * Revision 1.5  1995/10/04 15:01:37  dz
 * When generating routing file, treat .0 as .*
 *
 * Revision 1.4  1995/10/03 14:36:05  dz
 * seenby processing fixed - now we really don't send
 * rips to addresses listed in seenbys.
 *
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

call set_colors

Say ansi.cyan||'Frip - FTN Networks Automatic Routing Builder: '||ansi.intense||'Beta 4'ansi.normal

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

parse arg confdir .

if confdir = '' then confdir = '.'

if 0 then call announce confdir
if 0 then trace all

call parse_config

call load_base

rc = SysFileTree( confdir'\*.rip', 'Files', 'FO' );
if \ rc = 0 then do
   call Error 'Problems reading contents of 'confdir
   return 0
   end

do j = 1 to Files.0
	fj=strip(Files.j)
	call BoldMsg 'Looking at "'fj'"'
	if 1=do_one_file( fj, confdir ) then
		do
/*		call stream fj, 'C', 'close' */
		call SysMkdir confdir'\old'
		'@@move 'fj' 'confdir'\old >\dev\nul'
                if RC <> 0 then 
                   call Error "Can't move "fj' to 'confdir'\old, deleting'
                call SysFileDelete fj /* in case it was not move */
		end
end

call create_routes confdir

call save_base

exit 0





do_one_file: procedure expose base. aka. ansi.
in=arg(1)
dir=arg(2)

/* clear all of the stem */
rip.=''

rc=stream( in, 'c', 'open read' )
rc=stream( in, 'c', 'seek =1' )

if 0 then trace intermediates

do while lines(in)
        line=linein(in)
        if(substr(line,1,1)=';') then
                iterate

	parse var line key val
	if key='seenby' | key='path' then
		call value 'rip.'key, value('rip.'key)||'|'||val
	else
		call value 'rip.'key, val
end

if 0 then trace off

call stream in, 'C', 'close'

if rip.created='' | rip.from='' | rip.ad='' then
	do
	call Warning 'Incorrect rip 'in', skipped'
	return 0
	end

/* don't add myself - old way of thinking. Now we do.
if rip.from='myself' then return
*/

if rip.from='myself' then rip.from=aka.1


call BoldMsg '    Announce of 'ansi.green||ansi.intense||rip.ad||ansi.normal||ansi.green||' from 'rip.from
call msg '    created 'rip.created

/* here we can filter all the incoming rips - for now skip it */

if check_base() == 0 then
	do 
	call msg 'rip gives us no good, ignored'
	return 1
	end

call update_base

rip.hops=rip.hops+1

call send_rips dir

return 1






load_base:
bf=confdir'/RIP_Base'

base.0=0
c=1

rc=stream( bf, 'C', 'open read' )

do while lines(bf)
        line=strip(linein(bf))
        if(substr(line,1,1)=';'|line='') then
                iterate

	base.c=line
	c=c+1
	
end

base.0=c-1

call stream bf, 'C', 'close'

return






save_base:
bf=confdir'\RIP_Base'

call SysFileDelete confdir'\RIP_Base.Old'
'@@ren 'bf' RIP_Base.Old >\dev\nul'
if RC <> 0 then call Error "Can't rename "bf' to RIP_Base.Old'

call stream bf, 'C', 'open'
call stream bf, 'C', 'seek =0'

i=1
do while i < base.0+1
        call lineout bf, base.i
	i=i+1
end

call stream bf, 'C', 'close'

return




check_base:

/* if base is empty - anything looks great */
if base.0=0 then return 1

got_a_match=0

do i = 1 to base.0
	if match_base(base.i) then 
		do
		got_a_match=1
		if better(base.i) > 0 then return 1
		end
end

if 0 then say 'got_a_match ' got_a_match

return \got_a_match





better:
bline=arg(1)

/*

format of base line:

route-for route-via date time hops

*/


parse var bline route_for route_via date_d'/'date_m'/'date_y time_h':'time_m':'time_s hops
parse var rip.created cdate_d'/'cdate_m'/'cdate_y ctime_h':'ctime_m':'ctime_s

today=date('e')
parse var today tdate_d'/'tdate_m'/'tdate_y 
thistime=time('n')
parse var thistime ttime_h':'ttime_m':'ttime_s

if date_y < cdate_y-tdate_y then return 0
if date_m < cdate_m-tdate_m then return 0
if date_d < cdate_d-tdate_d then return 0
if time_h < ctime_h-ttime_h then return 0
if time_m < ctime_m-ttime_m then return 0

if rip.hops > hops then return 0

return 1







update_base:

do i = 1 to base.0
	if match_base(base.i) then 
		do
		/* remove lines that worse than ours */
		if better(base.i) <= 0 then base.i=';'
		end
end

base.0=base.0+1
i=base.0

if 0 then say 'rip.created 'rip.created rip.from

parse var rip.created cdate_d'/'cdate_m'/'cdate_y ctime_h':'ctime_m':'ctime_s

today=date('e')
parse var today tdate_d'/'tdate_m'/'tdate_y 
thistime=time('n')
parse var thistime ttime_h':'ttime_m':'ttime_s

time_s=ttime_s-ctime_s
if time_s<0 then 
	do
	time_s=time_s+60; 
	ttime_m=ttime_m-1; 
	end

time_m=ttime_m-ctime_m
if time_m<0 then 
	do 
	time_m=time_m+60; 
	ttime_h=ttime_h-1; 
	end

time_h=ttime_h-ctime_h
if time_h<0 then 
	do 
	time_h=time_h+24; 
	tdate_d=tdate_d-1; 
	end

/* yeah i know that not all of the months have 31 day */
date_d=tdate_d-cdate_d
if date_d<0 then 
	do 
	date_d=date_d+31; 
	tdate_m=tdate_m-1; 
	end

date_m=tdate_m-cdate_m
if date_m<0 then 
	do 
	date_m=date_m+12; 
	tdate_y=tdate_y-1; 
	end

date_y=tdate_y-cdate_y


base.i=rip.ad rip.from date_d'/'date_m'/'date_y time_h':'time_m':'time_s rip.hops

return



match_base:
bline=arg(1)
parse var bline bl_route_for .

if 0 then say 'match_base( 'rip.ad' vs 'bline' )'

rip_mask_mode=1
if our_aka_matches(rip.ad) then
   do
   /* Say 'Ad 'rip.ad" matches our aka -> will'nt be treated as mask" */
   rip_mask_mode=0
   end

base_mask_mode=1
if our_aka_matches(bl_route_for) then
   do
   /* Say 'Base line 'bl_route_for" matches our aka -> will'nt be treated as mask" */
   base_mask_mode=0
   end



parse var rip.ad rzone':'rnet'/'rnode'.'rpoint'@@'rdomain
parse var bl_route_for bzone':'bnet'/'bnode'.'bpoint'@@'bdomain

if bdomain \= '*' then
	do
	parse var rdomain rd1'.'rd2
	parse var bdomain bd1'.'bd2

	if rd2='' then bd2=''
	if bd1='' then bd2=''

	if rd2 \= bd2 | rd1 \= bd1 then return 0
	end

if bzone \= '*' & rzone \= bzone then return 0
if bnet  \= '*' & rnet  \= bnet  then return 0
if bnode \= '*' & rnode \= bnode then return 0

/* node is allways a default route for its points */
if bpoint \= '*' & bpoint \= 0 & rpoint \= bpoint then return 0

if rip_mask_mode=0 then 
   do
   if rdomain='*' & bdomain <> '*' then return 0
   if rzone='*'   & bzone <> '*'   then return 0
   if rnet='*'    & bnet <> '*'    then return 0
   if rnode='*'   & bnode <> '*'   then return 0
   if rpoint='*'  & bpoint <> '*'  then return 0
   end

if base_mask_mode=0 then 
   do
   if bdomain='*' & rdomain <> '*' then return 0
   if bzone='*'   & rzone <> '*'   then return 0
   if bnet='*'    & rnet <> '*'    then return 0
   if bnode='*'   & rnode <> '*'   then return 0
   if bpoint='*'  & rpoint <> '*'  then return 0
   end


return 1



create_routes: procedure expose base. aka. ansi.
dir=arg(1)
ro=dir'\routes.inc'

call BoldMsg 'Creating route file...'

call sysfiledelete ro

call stream ro, 'C', 'open write'

do i = 1 to base.0
	parse var base.i ro_route_for ro_route_via .
	parse var ro_route_for bzone':'bnet'/'bnode'.'bpoint'@@'bdomain

	if bpoint = 0 then bpoint = '*'
	if (bzone='*')|(bnet='*')|(bnode='*')|(bdomain='*')|(bpoint='*') then iterate

	call put_route ro, ro_route_for, ro_route_via
end

do i = 1 to base.0
	parse var base.i ro_route_for ro_route_via .
	parse var ro_route_for bzone':'bnet'/'bnode'.'bpoint'@@'bdomain

	if bpoint = 0 then bpoint = '*'
	if (bzone='*')|(bnet='*')|(bnode='*')|(bdomain='*')|(bpoint\='*') then iterate

	call put_route ro, ro_route_for, ro_route_via
end

do i = 1 to base.0
	parse var base.i ro_route_for ro_route_via .
	parse var ro_route_for bzone':'bnet'/'bnode'.'bpoint'@@'bdomain

	if bpoint = 0 then bpoint = '*'
	if (bzone='*')|(bnet='*')|(bnode\='*')|(bdomain='*')|(bpoint\='*') then iterate

	call put_route ro, ro_route_for, ro_route_via
end

do i = 1 to base.0
	parse var base.i ro_route_for ro_route_via .
	parse var ro_route_for bzone':'bnet'/'bnode'.'bpoint'@@'bdomain

	if bpoint = 0 then bpoint = '*'
	if (bzone='*')|(bnet\='*')|(bnode\='*')|(bdomain='*')|(bpoint\='*') then iterate

	call put_route ro, ro_route_for, ro_route_via
end

do i = 1 to base.0
	parse var base.i ro_route_for ro_route_via .
	parse var ro_route_for bzone':'bnet'/'bnode'.'bpoint'@@'bdomain

	if bpoint = 0 then bpoint = '*'
	if (bzone\='*')|(bnet\='*')|(bnode\='*')|(bdomain='*')|(bpoint\='*') then iterate

	call put_route ro, ro_route_for, ro_route_via
end

do i = 1 to base.0
	parse var base.i ro_route_for ro_route_via .
	parse var ro_route_for bzone':'bnet'/'bnode'.'bpoint'@@'bdomain

	if bpoint = 0 then bpoint = '*'
	if (bzone\='*')|(bnet\='*')|(bnode\='*')|(bdomain\='*')|(bpoint\='*') then iterate

	call put_route ro, ro_route_for, ro_route_via
end


call stream ro, 'C', 'close'


return

put_route: procedure expose aka.
fn=arg(1)
rofor=arg(2)
rovia=arg(3)

if 0 then Say fn '-' rofor '-' rovia

/* don't be stupid and route node via itself */
if match_addr(rofor,rovia) then return 0


/* If 'route for' or 'route via'matches our address, don't put a route for it! */
do i = 1 to aka.0
   if match_addr(rofor,aka.i) then return 0
   if match_addr(rovia,aka.i) then return 0
end


/* return lineout( fn, 'route 'rovia' 'rofor1 ) */
return lineout( fn, rline(rovia,rofor) )



send_rips: procedure expose rip. aka. ansi.
dir=arg(1)
links=dir'\links.cfg'

rc=stream( links, 'C', 'open read' )

do while lines(links)
        line=strip(linein(links))
        if(substr(line,1,1)=';'|line='') then
                iterate

	call do_a_link line, dir
end

call stream links, 'C', 'close'


return 1

do_a_link: procedure expose rip. aka. ansi.

if 0 then trace intermediates


link=arg(1)
dir=arg(2)

parse var link l_myaddr l_hisaddr rest

passme=1

do forever
	parse var rest elem rest

	elem=strip(elem)
	if elem='' then leave

	parse var elem '+'add
	if add\='' then
		if match_addr( rip.ad, add ) then passme=1

	parse var elem '-'del
	if del\='' then
		if match_addr( rip.ad, del ) then passme=0

end

if passme=0 then
	do
	call msg '    This announce will not come to 'l_hisaddr' due to restriction'
	return
	end


seen=rip.seenby
do forever
	parse var seen '|'seenby_el'|'seenby1
	seenby1=strip(seenby1)
	seenby_el=strip(seenby_el)
	if seenby_el='' then leave
	if match_addr( l_hisaddr, seenby_el ) then
		do
		call msg '    This announce will not come to 'l_hisaddr' due to seenby'
		return
		end
	if seenby1\='' then seen='|'seenby1
	else leave
end

call msg '    This announce will come to 'l_hisaddr


call sysmkdir dir'\out'
temp=systempFileName(dir'\out\rip?????.rip')

if temp='' then
	do
	call Error 'rip generation aborted - no temp file name'
	return
	end


l_myaddr=fcanon(l_myaddr)
l_hisaddr=fcanon(l_hisaddr)

call lineout temp, 'from 'l_myaddr
call lineout temp, 'to 'l_hisaddr
call lineout temp, 'hops 'rip.hops
call lineout temp, 'created 'rip.created
call lineout temp, 'ad 'rip.ad

call lineout temp, 'path 'l_myaddr
mypath=rip.path
do forever
	parse var mypath '|'path_el'|'path1
	if path1\='' then mypath='|'path1
	else mypath=''
	path_el=strip(path_el)
	if path_el='' then leave
	call lineout temp, 'path 'path_el
end

/* call lineout temp, 'seenby 'l_myaddr -- here was single seenby generation */
/* Now we put all the addresses of us into the seenbys */
do i = 1 to aka.0
   call lineout temp, 'seenby 'aka.i
end

seenby=rip.seenby
do forever
	parse var seenby '|'seenby_el'|'seenby1
	seenby1=strip(seenby1)
	if seenby1\='' then seenby='|'seenby1
	else seenby=''
	seenby_el=strip(seenby_el)
	if seenby_el='' then leave
	call lineout temp, 'seenby 'seenby_el
end

trace off

call stream temp, 'C', 'close'

call msg '      - delivering 'temp' to 'l_hisaddr
call deliver l_hisaddr' 'temp

return




match_addr: procedure

if 0 then trace intermediates

a1=fcanon(arg(1))
a2=fcanon(arg(2))

parse var a1 zone1':'net1'/'node1'.'point1'@@'domain1
parse var a2 zone2':'net2'/'node2'.'point2'@@'domain2

if domain2 \= '*' then
	do
	parse var domain1 d11'.'d12
	parse var domain2 d21'.'d22

	if d12='' then d22=''
	if d22='' then d12=''

	if d12 \= d22 | d11 \= d21 then return 0
	end

if zone2 \= '*' & zone1 \= zone2 then return 0
if net2  \= '*' & net1  \= net2  then return 0
if node2 \= '*' & node1 \= node2 then return 0

/* node is allways a default route for its points */
if point2 \= '*' & point2 \= 0 & point1 \= point2 then return 0

if 0 then trace off


return 1


parse_config:

in='frip.cfg'

aka.0=0


do while lines(in)
        line=strip(linein(in))
        if(substr(line,1,1)=';') then
                iterate
        if(line='') then
                iterate

	call do_cfg_line

end

if aka.0=0 then
   do
   call Error "No akas given in configuration, can't proceed!"
   exit
   end

if 0 then
   do i = 1 to aka.0
      say 'Aka: 'aka.i
   end

return

do_cfg_line:

parse var line cfg_verb cfg_val
cfg_verb=translate(strip(cfg_verb))
cfg_val=strip(cfg_val)

if cfg_verb='ADDRESS' | cfg_verb='AKA' then
    do
    aka.0=aka.0+1
    i=aka.0
    aka.i=fcanon(cfg_val)
    end

return


our_aka_matches: procedure expose aka. ansi.
a=arg(1)

if 0 then trace intermediates

if aka.0=0 | aka.0=='AKA.0' then 
   do
   Call Error "Can't match ad against my address"
   return
   exit
   end

do i = 1 to aka.0
   if match_addr(aka.i,a) then return 1
end

return 0


set_colors: procedure expose ansi.

ansi_esc=x2c('1b')||'['
ansi.lighgray=ansi_esc'37m'
ansi.cyan=ansi_esc'36m'
ansi.magenta=ansi_esc'35m'
ansi.blue=ansi_esc'34m'
ansi.brown=ansi_esc'33m'
ansi.green=ansi_esc'32m'
ansi.red=ansi_esc'31m'
ansi.black=ansi_esc'30m'

ansi.invisible=ansi_esc'8m'
ansi.reverse=ansi_esc'7m'
ansi.darkgray_bg=ansi_esc'5m'
ansi.blue=ansi_esc'4m'
ansi.intense=ansi_esc'1m'
ansi.normal=ansi_esc'0m'

return

Error: procedure expose ansi.
Say ansi.red||ansi.intense||'Error: '||ansi.normal||arg(1)
return

Warning: procedure expose ansi.
Say ansi.brown||ansi.intense||'Warning: '||ansi.normal||arg(1)
return


Msg: procedure expose ansi.
Say ansi.green||ansi.intense||'*** '||ansi.normal||ansi.green||arg(1)||ansi.normal
return

BoldMsg: procedure expose ansi.
Say ansi.brown||ansi.intense||'*** '||ansi.normal||ansi.green||arg(1)||ansi.normal
return




fcanon: procedure
parse arg addr default_ftn

dz=0
parse var default_ftn dz':'dn'/'rest

dpoint=0
parse var rest dnode'.'dpoint'@@'ddomain
if ddomain == '' then parse var rest dnode'@@'ddomain
if ddomain == '' then parse var rest dnode'.'dpoint

if dz=0 then dz=2
if ddomain='' then ddomain='fidonet.org'

parse var addr z':'rest

parse var addr z':'rest
if rest='' then 
	z=dz
else
	addr=rest

parse var addr net'/'rest
if rest='' then 
	net=dn
else
	addr=rest

point=0
domain=''
parse var addr node'.'rest
if node='' then node=-1
else 
	do
	if rest='' then
		do
		parse var addr node'@@'rest
		if rest \= '' then domain=rest
		end
	else
		do
		parse var rest point'@@'rest
		if rest \= '' then domain=rest
		end
	end

if domain='' then domain=ddomain


/*
say "canonical: "z':'net'/'node'.'point'@@'domain
*/

return z':'net'/'node'.'point'@@'domain




@


1.8
log
@Now we automatically add all of our AKAs to outgoig
rip seenbys, so that we'll never get it back to our
other AKA, as it happened before.
@
text
@d14 5
d53 1
a53 1
Say ansi.cyan||'Frip - FTN Networks Automatic Routing Builder: '||ansi.intense||'Beta 3'ansi.normal
d77 1
a77 1
	call Msg 'Looking at "'fj'"'
d133 8
a140 1
call msg 'Announce of 'rip.ad' from 'rip.from
a284 6
/* don't add myself - old way of thinking. Now we do.
if rip.from='myself' then return
*/

if rip.from='myself' then rip.from=aka.0

d412 2
d484 1
a484 1
put_route: procedure
d489 5
a493 1
parse var rofor bzone':'bnet'/'bnode'.'bpoint'@@'bdomain
d495 5
a499 4
if bzone='*' then bzone='All'
if bnet ='*' then bnet ='All'
if bnode='*' then bnode='All'
if bpoint='*' then bpoint='All'
a500 1
rofor1=bzone':'bnet'/'bnode'.'bpoint
d502 2
a503 1
return lineout( fn, 'route 'rovia' 'rofor1 )
d771 66
@


1.7
log
@Colouring, misc. details.
@
text
@d14 3
d596 5
a600 1
call lineout temp, 'seenby 'l_myaddr
d693 1
a693 1
cfg_verb=strip(cfg_verb)
d696 1
a696 1
if cfg_verb='address' | cfg_verb='aka' then
d700 1
a700 1
    aka.i=cfg_val
@


1.6
log
@SysLoadFuncs
@
text
@d14 10
d43 4
d57 2
d63 1
a63 1
   Say 'Problems reading contents of 'confdir
d69 1
a69 1
	Say 'Looking at "'fj'"'
d74 4
a77 1
		'@@move 'fj' 'confdir'\old'
d91 1
a91 1
do_one_file: procedure expose base.
d121 1
a121 1
	say 'Incorrect rip 'in', skipped'
d125 2
a126 2
Say 'Announce of 'rip.ad' from 'rip.from
Say '    created 'rip.created
d132 1
a132 1
	say 'rip gives us no good, ignored'
d182 2
a183 1
'@@ren 'bf' RIP_Base.Old'
d270 1
a270 1
/* don't add myself */
d272 3
d339 16
d376 19
d399 1
a399 1
create_routes: procedure expose base.
d491 1
a491 1
send_rips: procedure expose rip.
d510 1
a510 1
do_a_link: procedure expose rip.
d540 1
a540 1
	say '    This announce will not come to 'l_hisaddr' due to restriction'
d553 1
a553 1
		say '    This announce will not come to 'l_hisaddr' due to seenby'
d560 1
a560 1
say '    This announce will come to 'l_hisaddr
d568 1
a568 1
	say '*** rip generation aborted: no temp file name'
d610 1
a610 1
Say '      - delivering 'temp' to 'l_hisaddr
d652 99
@


1.6.1.1
log
@Version from 256 - this one needs
config file and does not loose everything
when wildcard ad received. Wildcards are
assumed as such only if our akas does not
match those wildcards.
@
text
@a13 3
 * Revision 1.6  1995/10/27 21:54:46  dz
 * SysLoadFuncs
 *
a32 2
Say 'Frip - FTN Networks Automatic Routing Builder: Beta 3'

a42 2
call parse_config

d58 1
a58 3
		'@@move 'fj' 'confdir'\old >\dev\nul'
                if RC <> 0 then Say "Can't move "fj' to 'confdir'\old, deleting'
                call SysFileDelete fj /* in case it was not move */
d72 1
a72 1
do_one_file: procedure expose base. aka.
d250 1
a250 1
/* don't add myself - old way of thinking. Now we do.
a251 3
*/

if rip.from='myself' then rip.from=aka.0
a315 16
rip_mask_mode=1
if our_aka_matches(rip.ad) then
   do
   Say 'Ad 'rip.ad" matches our aka -> will'nt be treated as mask"
   rip_mask_mode=0
   end

base_mask_mode=1
if our_aka_matches(bl_route_for) then
   do
   Say 'Base line 'bl_route_for" matches our aka -> will'nt be treated as mask"
   base_mask_mode=0
   end



a336 19
if rip_mask_mode=0 then 
   do
   if rdomain='*' & bdomain <> '*' then return 0
   if rzone='*'   & bzone <> '*'   then return 0
   if rnet='*'    & bnet <> '*'    then return 0
   if rnode='*'   & bnode <> '*'   then return 0
   if rpoint='*'  & bpoint <> '*'  then return 0
   end

if base_mask_mode=0 then 
   do
   if bdomain='*' & rdomain <> '*' then return 0
   if bzone='*'   & rzone <> '*'   then return 0
   if bnet='*'    & rnet <> '*'    then return 0
   if bnode='*'   & rnode <> '*'   then return 0
   if bpoint='*'  & rpoint <> '*'  then return 0
   end


d341 1
a341 1
create_routes: procedure expose base. aka.
d433 1
a433 1
send_rips: procedure expose rip. aka.
d452 1
a452 1
do_a_link: procedure expose rip. aka.
a591 66


parse_config:

in='frip.cfg'

aka.0=0


do while lines(in)
        line=strip(linein(in))
        if(substr(line,1,1)=';') then
                iterate
        if(line='') then
                iterate

	call do_cfg_line

end

if aka.0=0 then
   do
   Say "No akas given in configuration, can't proceed!"
   exit
   end

if 0 then
   do i = 1 to aka.0
      say 'Aka: 'aka.i
   end

return

do_cfg_line:

parse var line cfg_verb cfg_val
cfg_verb=strip(cfg_verb)
cfg_val=strip(cfg_val)

if cfg_verb='address' | cfg_verb='aka' then
    do
    aka.0=aka.0+1
    i=aka.0
    aka.i=cfg_val
    end

return


our_aka_matches: procedure expose aka.
a=arg(1)

if 0 then trace intermediates

if aka.0=0 | aka.0=='AKA.0' then 
   do
   Say "Can't match ad against my address"
   return
   exit
   end

do i = 1 to aka.0
   if match_addr(aka.i,a) then return 1
end

return 0
@


1.5
log
@When generating routing file, treat .0 as .*
@
text
@d14 3
d32 3
@


1.4
log
@seenby processing fixed - now we really don't send
rips to addresses listed in seenbys.
@
text
@d14 4
d347 1
d357 1
d367 1
d377 1
d387 1
d397 1
@


1.3
log
@*** empty log message ***
@
text
@d3 6
d14 3
d30 1
a30 1
if 1 then call announce confdir
d480 1
@


1.2
log
@*** empty log message ***
@
text
@d8 3
d21 1
a21 1
if 0 then call announce confdir
d63 2
d77 2
d87 2
d237 1
a237 1
if 1 then say 'rip.created 'rip.created rip.from
d428 4
d449 1
a449 1
	parse var elem '+'del
d457 1
a457 1
	say 'This announce will not come to 'l_hisaddr
a460 1
say 'This announce will come to 'l_hisaddr
d462 19
a480 2
call sysmkdir dir'/out'
temp=systempFileName(dir'/out/rip?????.rip')
d499 1
a499 1
path=rip.path
d501 3
a503 2
	parse var path '|'path_el'|'path1
	if path1\='' then path='|'path1
d509 2
d514 1
d516 1
d522 1
d526 2
a527 1

d535 3
d561 3
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 * $Log:$
d19 2
a20 1
trace all
d30 3
a32 2
	Say 'Looking at 'Files.j
	if 1=do_one_file( Files.j, confdir ) then
d34 3
a36 2
		'mkdir 'confdir'\old'
		'move 'files.j' 'confdir'\old'
d40 2
d46 4
d54 1
a54 1
/* clear all the stem */
d57 3
d67 1
a67 1
		call value('rip.'key,value('rip.'key)||'|'||val)
d69 1
a69 1
		call value('rip.'key,val)
d72 8
d93 6
a98 1
call send_rips
a99 1
return 0
d103 1
a103 1
bf=dir'/RIP_Base'
d108 2
d111 2
a112 2
        line=linein(bf)
        if(substr(line,1,1)=';') then
d126 5
d132 4
a135 1
bf=dir'/RIP_Base'
d142 1
a142 1
        lineout(bf,base.i)
d151 2
d160 1
a160 1
do i = 1 to base.0+1
d168 2
d174 2
d202 1
a202 1
if rip.hops > hops then return o
d209 3
d214 1
a214 1
do i = 1 to base.0+1
d223 1
a223 1
if rip.from='|myself' then return
d228 2
d273 3
d286 2
d291 1
a291 1
if rdomain \= '*' then
d302 3
a304 3
if rzone \= '*' & rzone \= bzone then return 0
if rnet  \= '*' & rnet  \= bnet  then return 0
if rnode \= '*' & rnode \= bnode then return 0
d307 1
a307 1
if rpoint \= '*' & rpoint \= 0 & rpoint \= bpoint then return 0
d313 61
d376 38
a414 1
send_rips:
d417 109
@
