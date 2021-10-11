head	1.2;
access;
symbols;
locks;
comment	@ *  @;


1.2
date	95.10.03.14.36.05;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.10.02.18.51.42;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
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
 * canonisation of address - right now ftn only
 *
 * args: address to convert, default address (must be at least z:n/node)
 *
 *       address to convert must have at least a node number
 *
**/

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


1.1
log
@Initial revision
@
text
@a0 1
/* canonisation of address - right now ftn only */
d2 8
@
