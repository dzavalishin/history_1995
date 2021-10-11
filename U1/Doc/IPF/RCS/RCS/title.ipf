head	1.1;
access;
symbols;
locks;
comment	@.*  @;


1.1
date	95.05.10.12.55.16;	author dz;	state Exp;
branches;
next	;


desc
@U1 Dox part
@


1.1
log
@Initial revision
@
text
@head	1.3;
access;
symbols;
locks;
comment	@@.*    @@;


1.3
date	95.05.10.12.03.17;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.05.05.03.32.50;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.05.05.03.16.55;	author dz;	state Exp;
branches;
next	;


desc
@@@@


1.3
log
@@update
@@
text
@@.*
.*
.*  $Log: title.ipf $
.*    Revision 1.2  1995/05/05  03:32:50  dz
.*    *** empty log message ***
.*
.*    Revision 1.1  1995/05/05  03:16:55  dz
.*    Initial revision
.*
.*
.*

:h1 res=1.Title Page
:p.:p.
:lines align=center.
:font facename='Tms Rmn' size=60x80.
U 1
:font facename='Tms Rmn' size=24x32.

                              UUCP - FIDO gateway 
:font facename='Helv' size=12x20.
                                  Version 1.98
 
:font facename='Tms Rmn' size=24x32.
                                Operator's guide 
:artwork align=center name='StarCube.BMP'.
 
 
:font facename='Tms Rmn' size=12x20.
                  Copyright (C) 1991-1995 by Dmitry Zavalishin. 
 
:elines.

.*:link reftype=hd res=3.Go to Guide:elink.

.*:link reftype=hd res=2.Go look at the cover picture:elink.

.* :h1 res=2.Cover Picture
.* :artwork align=center name='uu2balls.BMP'.

.* :link reftype=hd res=3.Go to Guide:elink.









:h1 res=4 x=left y=top width=25% height=100%
    scroll=none group=1 clear.Introduction

:p.:i1.Introduction
:link reftype=hd res=41 dependent auto.
:link reftype=hd res=41 dependent.That's Beta!:elink.
:p.
:link reftype=hd res=42 dependent.Overview:elink.
:p.
:link reftype=hd res=43 dependent.Reaching the author:elink.
:p.
:link reftype=hd res=44 dependent.Getting the last version:elink.
:p.
:link reftype=hd res=45 dependent.System Requirements:elink.
:p.
:link reftype=hd res=46 dependent.Registration:elink.


:h2 res=41 x=25% y=top width=75% height=100% group=2.It is a beta version!
:p.
Hi, dear user!
:p.:i1.Beta
Well, this is another beta version of U1. It has bugs, it 
does all but the things you need it to, it doesn't work at all... 
Sure, I do my best to make it better and better, but don't judge 
me too strictly, if something is wrong! 
:p.Note as well, that the documentation you're reading
is in the beta testing too &colon.-). Part of it I wrote myself, and part was
translated from Russian by Natasha Razdaeva
(:link reftype=hd res=25.Thanx!:elink.)
some time ago. Hope I'll find some time later to rewrite obsolete parts from
scratch to be sure everything is in sync with current state of code.
:p.:i1.Version numbering
A word on version numbers. U1 version number has major part, minor part and modification letter.
For example, version 1.98t has major number :hp2.1:ehp2.,
minor number :hp2.98:ehp2. and letter :hp2.t:ehp2.. Each time
I change code significantly or release alpha version, modification
letter is changed. Each time the beta version is released or i'm out of
letters &colon.-), minor number is increased. Major number changes once a
final release is done.

:h2 res=42 x=25% y=top width=75% height=100% group=2.Overview
:p.
This product (hereinafter Gate) is the facility that 
enables you being the user both of UUCP network (such as 
Relcom, UUNET, etc.) and FIDO (FidoNet, LifNet, etc.) to 
integrate these networks. Gate is able to transfer messages 
from one network to another as well as integrate electronic 
conferences (echoes, newsgroups).

:h2 res=43 x=25% y=top width=75% height=100% group=2.How to Get in Touch with the Author
:p.:i1.dz
If you want to get in touch with the author of this 
thing with your comments, questions, suggestions and 
swearing you may&colon. 
:ul.
:li.Subscribe to UU2 echo.
:li.Send a letter via e&dash.mail&colon.
:dl.
:dt.Internet
:dd.dz@@@@phantom.ru
:dt.UUCP
:dd.funet.fi!kiae!phantom!dz
:dt.FIDO
:dd.Dmitry Zavalishin&comma. 2&colon.5020/32
:edl.
:li.Call by telephone number&colon. +7 (095) 110&dash.6728
:li.Send a snailmail letter to&colon.
Zavalishin D.K., Russia, Moscow, Artekovskaya str. 7-4, fl.260
:eul.
:p.
:artwork align=center name='dz.BMP'.
.ce Hi there!


:h2 res=44 x=25% y=top width=75% height=100% group=2.Downloading the latest version
:p.:i1.Downloading U1
Here's how can you get this stuff&colon. 
:ul.
:li.Subscribe to UU2 fileecho and receive updates automatically.
:li.Check ftp.kiae.su/pub/os2/fido.
:li.Check ftp.cdrom.com and hobbes.nmsu.edu.
:li.F'Req U1 from 2&colon.5020/32.
:warning. FIDO node 2&colon.5020/32 is my home node and shares phone line with me.
Please teach your mailer to call me from 21&colon.00 to 3&colon.00 GMT :hp6.ONLY:ehp6..
:eul.
:p.:p.


:h2 res=45 x=25% y=top width=75% height=100% group=2.System Requirements
:ul.
:li.Hardware
:p.No special hardware required. Current version of U1
will run on any OS/2 compatible Intel x86 machine.

:li.Software
:p.:i1.Software requirements
OS/2 2.0, 2.1, 2.11, Warp 3.0 or later required.
Rexx support must be installed.

:p.U1 was tested (by myself or betatesters) with&colon.
Novell Netware 3.11, Lantastic, FrontDoor 2.01-2.10, 
BinkleyTerm 2.50-2.59 (and some sideway betas, such as EE 
beta D and E), D'Bridge, Imail 1.10-1.21, Squish 1.00-1.11, 
UUPC/Extended 1.12k.
:p.In addition I must mention that U1 was used in multichannel 
FIDO systems in a situation when gate modules work 
in both channels simultaneously. 

:li.You
:p.:i1.Knowledge required.
I hope you're familiar enough with FTN and UUCP concepts. &colon.-)

:eul.

:p.



:h2 res=46 x=25% y=top width=75% height=100% group=2.Registration
:i1.Registration.
:p.Current (1.98-1.99) alpha or beta versions are free for use as long, as
version 2.0 is not released. I'm not sure how much will it cost in future,
but it is planned to be released as shareware, or, maybe, bookware &colon.-),
magazine_subscription_ware &colon.-), etc.
:p.:link reftype=hd res=43.I'm:elink. open to your comments and suggestions!
@@


1.2
log
@@*** empty log message ***
@@
text
@@d4 3
a23 2
.* :artwork align=center name='StarCube.BMP'.
 
d26 1
d51 19
a69 2
:h1 res=4.Introduction
:h2.It is a beta version!
d72 1
a72 1
:p.
d78 13
a90 1
is in beta version too &colon.-).
d92 3
a94 2
:h2.Overview
:p.This product (hereinafter Gate) is the facility that 
d101 3
a103 2
:h2.How to Get in Touch with the Author
:p.If you want to get in touch with the author of this 
d121 16
a137 1
.* :link reftype=hd res=5.(Next chapter):elink.
d139 2
a140 1
:h1 res=5.System Requirements
d147 3
a149 1
:p.OS/2 2.0, 2.1, 2.11, Warp 3.0 or later required.
d153 3
a155 3
BinkleyTerm 2.50-2.55 (and some sideway betas, such as EE 
beta D and E), D'Bridge, Imail 1.10-1.21, Squish 1.00-1.01, 
UUPC/Extended 1.12k. 
d160 4
a167 1
.* :link reftype=hd res=6.(Next chapter):elink.
d170 7
@@


1.1
log
@@Initial revision
@@
text
@@d3 3
a5 1
.*  $Log: U1.IPF $
d8 1
d41 73
@@
@
