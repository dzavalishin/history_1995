:h3 clear res=812 x=left y=top width=25% height=100%.Optional parameters
:p.
The following parameters are optional.
:p.:link reftype=hd res=8121 dependent auto.
:p.:link reftype=hd res=8121 dependent.UUCP:elink.
:p.:link reftype=hd res=8122 dependent.PercentInFrom:elink.
:p.:link reftype=hd res=8123 dependent.LogStyle:elink.
:p.:link reftype=hd res=8124 dependent.Log:elink.
:p.:link reftype=hd res=8125 dependent.LogLevel:elink.
:p.:link reftype=hd res=8126 dependent.Mail headlines elimination:elink.
:p.:link reftype=hd res=8127 dependent.Debug:elink.
:p.:link reftype=hd res=8128 dependent.Announce:elink.
:p.:link reftype=hd res=8129 dependent.Free:elink.
:p.:link reftype=hd res=8130 dependent.PassUnknownBack:elink.
:p.:link reftype=hd res=8131 dependent.Code_FU:elink.
:p.:link reftype=hd res=8132 dependent.Code_UF:elink.
:p.:link reftype=hd res=8133 dependent.Code_UF_flag:elink.
:p.:link reftype=hd res=8134 dependent.RmailExe:elink.
:p.:link reftype=hd res=8135 dependent.RmailExitL:elink.
:p.:link reftype=hd res=8136 dependent.RmailExitR:elink.
:p.:link reftype=hd res=8137 dependent.UuxExe:elink.
:p.:link reftype=hd res=8138 dependent.ZipLimit:elink.
:p.:link reftype=hd res=8139 dependent.KickUUCPFlag:elink.
:p.:link reftype=hd res=8140 dependent.StoreFiles:elink.
:p.:link reftype=hd res=8141 dependent.FidoFromFieldSize:elink.

:h4 res=8121 x=25% y=top width=75% height=100% group=4.UUCP
:p.
uucp package type. Valid types&colon.
:dl compact.
:dt.ache30
:dd.uupc/*, coop. Demos, vers. 3.0
:dt.uupc11r
:dd.uupc/ext 1.11R or later
:dt.waffle
:dd.waffle 1.64 or later
:dt.sendmail
:dd.IBM TCP/IP sendmail (experimental)
:edl.
:xmp.
Uucp                    Ache30
:exmp.

:h4 res=8122 x=25% y=top width=75% height=100% group=4.PercentInFrom
:p.
If set to Yes, From_ line will have '@' replaced with '%'
in it. Some packages require it, some - not. Default to yes.

:xmp.
PercentInFrom No
:exmp.

:h4 res=8123 x=25% y=top width=75% height=100% group=4.LogStyle
:p.
Defines the format of log file. Possible values&colon.
:dl compact tsize=20.
:dt.Default
:dd.Selects own format of log
:dt.Binkley
:dd.BinkleyTerm lookalike mode
:dt.FrontDoor
:dd.you see...
:edl.
:warning.This one must be placed :hp7.BEFORE:ehp7. :link reftype=hd res=8124.Log:elink. keyword.

:xmp.
LogStyle                Binkley
:exmp.

:h4 res=8124 x=25% y=top width=75% height=100% group=4.Log
:p.
Name of gate operation log file.

:xmp.
Log                     u&colon.\gate\u1.log
:exmp.

:h4 res=8125 x=25% y=top width=75% height=100% group=4.LogLevel
:p.
Used to select information you want to see in log file. Turn
debug mode on to get :hp5.complete:ehp5. log, then look at it and select
key-characters of records you like to see.
:p.
Meaning of characters&colon.
:p.
:dl compact.
:dt.a
:dd.Address conversions
:dt.e
:dd.Echomail info
:dt.n
:dd.News info
:dt.b
:dd.batched news
:dt.c
:dd.compression
:dt.f
:dd.file attaches
:dt.u
:dd.user info
:dt.x
:dd.Rexx log operator output
:p.
:dt.#
:dd.chitchat (nothing to do, all done, etc)
:dt.%
:dd.Timings, metrics
:dt.$
:dd.Mail logging
:edl.

:xmp.
LogLevel                au%$
:exmp.

:h4 res=8126 x=25% y=top width=75% height=100% group=4.Mail headlines elimination
:p.
Following parameters define which RFC headlines to eliminate
when converting messages from UUCP to FIDO.
:p.
Meaning of example below&colon. first line enables all headlines,
second one tells to hide under ^A everything that starts
with "Receiv" or "Resent-" (not implemented yet), and last
tells to eliminate "Message-Id" and "From_" anyway.

:note.To eliminate leading UUCP From line, use "From_" as shown below.

:xmp.
MailPassHL *
MailHideHL Receiv* Resent-*
MailKillHL Message-ID From_
:exmp.

:p.
Default values used if no corresponding parameter given in config. file are:

:xmp.
MailPassHL *
MailHideHL X-* Message-ID
MailKillHL Fido* Status Resent-* Nntp-*
:exmp.


:h4 res=8127 x=25% y=top width=75% height=100% group=4.Debug
:p.
Use Debug flag to get detailed logging. Produces :hp5.very:ehp5. large
logs.

:xmp.
Debug
:exmp.

:h4 res=8128 x=25% y=top width=75% height=100% group=4.Announce
:p.
With this flag most U1 modules will display it's names
when started.
:xmp.
Announce	Yes
:exmp.

:h4 res=8129 x=25% y=top width=75% height=100% group=4.Free
:p.
This keyword enables use of gate by persons not registered
in userlist. If used alone, enables access to any
uucp/internet address. To limit access list permitted
domains and/or uucp machine names after keyword.
:xmp.
Free    su ua by ew msk kiae demos dialnet isoft
:exmp.
:p.
In this example the following addresses will be valid&colon.
:xmp.
user@subdomain.msk.su
kiae!msk.su!user
user@smthng%msk.su@somewhere.kiae.su
@msk.su&colon.user@hq.demos.su
:exmp.
:p.
Next ones will be bounced&colon.
:xmp.
service%nic.ddn.mil@kiae.su
demos!fuug.fi!henry@ford.us
:exmp.


:h4 res=8130 x=25% y=top width=75% height=100% group=4.PassUnknownBack
:p.
This flag can be used if you want to use gate together with
usual uucp environment. In PassUnknownBack mode gate passes
letters it cannot deliver to FIDO back to uucp.
:xmp.
PassUnknownBack
:exmp.

:h4 res=8131 x=25% y=top width=75% height=100% group=4.Code_FU
:p.
(For cyrillic code table conversion)
Code table name to use for fido/uucp conversion. (Used by FIDO2UU)
:xmp.
Code_FU                 None
:exmp.

:h4 res=8132 x=25% y=top width=75% height=100% group=4.Code_UF
:p.
Code table name to use for uucp/fido conversion.
(Used by UU2FIDO &amp. UNBATCH)
:p.
Use KOI8-CP866 for russian language and KOI8U-CP866U for ukrainian.
:xmp.
Code_UF                KOI8-CP866
:exmp.

:h4 res=8133 x=25% y=top width=75% height=100% group=4.Code_UF_flag
:p.
Without this keyword uucp-fido code table converter will be used
every time UU2Fido called.
This keyword specifies mode, in which code conversion
will only take place if UU2Fido called with -u flag. You will need
this mode if you interested in cyrillic characters code translation
and use U1 with UUPC/* in PassUnknownBack mode.
:xmp.
Code_UF_flag
:exmp.

:h4 res=8134 x=25% y=top width=75% height=100% group=4.RmailExe
:p.
Name of uucp mail delivery agent.
:xmp.
RmailExe                u&colon.\uupc\uusmail.exe
:exmp.

:h4 res=8135 x=25% y=top width=75% height=100% group=4.RmailExitL
:p.
Rmail must return this exit code if letter delivered to local uucp mailbox.
(This is an error condition for gate. Use -1 to disable.)
:xmp.
RmailExitL              0    ; For UUPC/Ache
RmailExitL              -1   ; For other UUCP packages
:exmp.

:h4 res=8136 x=25% y=top width=75% height=100% group=4.RmailExitR
:p.
Exit code that rmail returns on successful delivery of the letter.
:xmp.
RmailExitR              48   ; For UUPC/Ache
RmailExitR              0    ; For other UUCP packages
:exmp.

:h4 res=8137 x=25% y=top width=75% height=100% group=4.UuxExe
:p.
Name of uucp remote execution program
:xmp.
UuxExe                  u&colon.\uupc\uux.exe
:exmp.

:h4 res=8138 x=25% y=top width=75% height=100% group=4.ZipLimit
:p.
Maximal size of message to pass to FIDO netmail without
conversion to file-attach.

:xmp.
ZipLimit                16384
:exmp.

:h4 res=8139 x=25% y=top width=75% height=100% group=4.KickUUCPFlag
:p.
Name of file to create when something new sent to UUCP side

:xmp.
KickUUCPFlag           u&colon.\net\bink\btexit77.01
KickUUCPFlag           u&colon.\net\bink\fdexit77.01
:exmp.

:h4 res=8140 x=25% y=top width=75% height=100% group=4.StoreFiles
:p.
Directory where you hold your outbound files. Used to hold
file-attaches created by uu2fido.
:p.
Default value&colon. %U1%\gate.out
:p.
Directory will be created automatically, if not exists.

:xmp.
StoreFiles              u&colon.\gate\gate.out
:exmp.

:h4 res=8141 x=25% y=top width=75% height=100% group=4.FidoFromFieldSize
:p.
Sets maximal length of contents of FIDO message 'from name' field.
If uucp address to be placed in from field is longer than this
parameter's value, uucp address will be replaced with gate magic name.
:p.
Default value&colon. 35.

:xmp.
FidoFromFieldSize       36
:exmp.

