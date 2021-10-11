head	1.1;
access;
symbols;
locks;
comment	@.*  @;


1.1
date	95.05.10.12.03.17;	author dz;	state Exp;
branches;
next	;


desc
@@


1.1
log
@Initial revision
@
text
@:h2 res=91.Sendmail
:p.:i1.Sendmail
U1 can be used as sendmail-driven mailer.
Sendmail version 1.3.2 or later should be used.
Note though that Warp IAK sendmail has been designed to work with slip
only and sences state of the slip driver. So avoid using it if slip
is not the only way you communicating with outer world. For use with
LAN TCP/IP or any UUCP connections get yourself a copy of sendmail.exe
from the latest :link reftype=fn refid=ip_csd.IBM TCP/IP CSD:elink..
The following excerpt from
sendmail.cf shows how U1 mailer can be described to be called by sendmail.
:note.Flag -x is ignored by U1 and used just to overcome glitch in
sendmail program execution logic.
:xmp.
Muu2,  P=C&colon.\u1\uu2fido.exe, A=-x1 $u, Flags=upm, Sender=10, Recipient=12
:exmp.
:p.:p.
.* Here included :link reftype=hd res=911.complete example of sendmail.cf:elink.
To use sendmail instead of usual rmail, which is called by U1 to deliver
uucp mail, use
:link reftype=hd res=912.rmail.c:elink. filter program.

:h2 res=911 hide.Example of sendmail configuration
.* .im sendmail\sendmail.cf

:h2 res=912 hide.Cqalling sendmail instead of rmail
.im sendmail\rmail.c


:fn id=ip_csd.
:p.
To get latest CSD, ftp to :hp5.software.watson.ibm.com:ehp5.,
change directory to :hp5.tcpip/os2:ehp5. and download updates.
:note.Don't you get TCP/IP 1.2 updates mistakenly.
:efn.

@
