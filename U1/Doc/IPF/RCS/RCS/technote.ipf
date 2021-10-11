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
@head	1.2;
access;
symbols;
locks;
comment	@@.*    @@;


1.2
date	95.05.10.12.03.17;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.05.05.03.32.50;	author dz;	state Exp;
branches;
next	;


desc
@@@@


1.2
log
@@update
@@
text
@@.*
.*
.*  $Log: $
.*
.*

:h1 res=23.Technical Notes
:p.
See following pages.

:h2 res=231.General Technical Notes
:i1.product code
:i1.Type 2+ packets
:i1.#! cunbatch
:i1.network

:ul.
:li.The FTSC assigned product code for U1 is CC hexadecimal. 

:li.U1 supports 3D/4D and partially 5D addresses. 

:li.U1 creates and is able to receive Type 2+ packets, provided they
have correct capability word as well as it's validation copy. 

:li.U1 batcher doesn't add '#! cunbatch' line at the beginning of
compressed UseNet batches. It is assumed that U1 will be used
with Cnews as the receiver of batches, which doesn't need
'#! cunbatch' header. 

:li.Echo&dash.News gateing&colon. text&comma. that follows origin line will be stripped.

:li.U1 never gives up timeslices since
there's no situations when U1 waits for some event to happen.

:li.I did my best to make sure U1 will not cause any problems on the
network, but nothing is guaranteed. But if you
encounter some problem related to file locking or something else
related to multitasking environment, please feel free to inform me.

:eul.



:h2 res=232.Standards supported
:p.:i1.FTS
:i1.FSC
:i1.RFC
U1 supports FIDO standards
:ul compact.
:li.FTS-0001
:li.FTS-0004
:li.FTS-0009
:li.FSC-0035
:li.FSC-0044
:li.FSC-0053
:eul.
:p.and Internet standards
:ul compact.
:li.RFC-822
:li.RFC-1036
:li.RFC-1123
:eul.

:h2 res=233.FIDO kludge lines
:p.:i1.Kludge lines
Kludge lines parsed&colon.
:ul compact.
:li.FMPT
:li.TOPT
:li.INTL
:li.FLAGS
:li.DOMAIN
:li.MSGID
:li.REPLY
:li.PID
:eul.

:p.Kludge lines created&colon.
:ul compact.
:li.FMPT
:li.TOPT
:li.INTL
:li.FLAGS
:li.DOMAIN
:li.MSGID
:li.REPLY
:li.PID
:li.REPLYADDR
:li.REPLYTO
:eul.

:p.Please note that U1 never attempts to find letter origin address
in MSGID field. 



:h2 res=234.A word on errors handling
:p.:i1.Errors handling
In short, Gate registers errors in the log file and makes
the following efforts to secure your property if it failed
to do something&colon.

:dl break=all.
:dt.Delivery error to UUCP
:dd.The letter will be generated to the sender of the initial message
and to the postmaster. The letter contains the corresponded fragment 
of the U1 log file and text of the original letter.

:dt.The recipient was not found during delievery to FIDO
:dd.The original letter will be sent to the postmaster. If he is also not
found in the users' list - so much the worse for you. The letter is
simply lost.

:dt.The news-group has not found in the list while converting of news into echo
:dd.The letter will be sent to the echo named bad_group.

:dt.The echo-group has not found while converting of echo into news
:dd.The letter will be ignored.

:edl.

:p.Among the all fatal errors the worst ones are those, that may
occure during delievery of the letter from UUCP. They, usually result
in loss of the letter, because some of UUCP packages, when delivering
received messages, simply ignore exit codes, returned by delivery program.
@@


1.1
log
@@Initial revision
@@
text
@@d1 6
d8 3
d12 4
d45 4
a48 1
:p.U1 supports FIDO standards
d65 2
a66 1
:p.Kludge lines parsed&colon.
d98 2
a99 1
:p.In short, Gate registers errors in the log file and makes
@@
@
