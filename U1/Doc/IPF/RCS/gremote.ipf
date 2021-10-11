head	1.2;
access;
symbols;
locks;
comment	@.*    @;


1.2
date	95.05.10.12.03.17;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.05.05.03.32.50;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@update
@
text
@.*
.*
.*  $Log: gremote.ipf $
.*  Revision 1.1  1995/05/10  12:55:16  dz
.*  Initial revision
.*
.*
.*

:h1 group=1 res=20.Remote control unit
:p.:i1.Gremote
This utility (GREMOTE.EXE), being started without any 
parameters, searches the letters which have been sent to the 
user with the name Gate and when found them, the utility
tries to find and execute the commands contained in letter.
It should be noted that the letters may come through Gate
too - gremote ignores the lines of the RFC822 letter header.
:p.
Gremote understands the following commands&colon.
:dl break=none tsize=20.
:dt.HELP          
:dd.Sends back a note concerning GREMOTE comands together with the 
contents of special file (See :link reftype=hd res=8191.HelpFile:elink. config. file option).

:dt.USERS
:dd.:link refid=GREMOTE-SECURE reftype=fn.(Secure):elink.
Sends the list of :link refid=REGISTERED-USERS reftype=fn.registered:elink.
gate users. 

:dt.ECHOES
:dd.Sends the list of gated echo-, news-groups. 

:dt.NOTIFY
:dd.:link refid=GREMOTE-SECURE reftype=fn.(Secure):elink.
Sends the letters to the all the
:link refid=REGISTERED-USERS reftype=fn.registered:elink. users of
gate with reminding, that they are gate users, what their address is and 
how they may ask for help. 

:dt.BROADCAST
:dd.:link refid=GREMOTE-SECURE reftype=fn.(Secure):elink.
Distributes text following the comand (up to the end of the message)  
to the all the :link refid=REGISTERED-USERS reftype=fn.registered:elink. users.
:edl.

:fn id=GREMOTE-SECURE.
:p.
Secure (Privileged) GRemote commands:
The privileged commands may be executed only 
when a valid password is given. To use such 
commands you need to pass password in the 
Subject field of the letter.
:efn.
:fn id=REGISTERED-USERS.
:p.
Registered Users:
Registered users are the users, which are 
mentioned in the USERS.U1 configuration file.
:efn.
@


1.1
log
@Initial revision
@
text
@d1 9
a9 2
:h1 res=20.Remote control unit
:p.This utility (GREMOTE.EXE), being started without any 
d20 1
a20 1
contents of special file (See HELPFILE config. file option).
@
