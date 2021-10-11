head	1.3;
access;
symbols;
locks;
comment	@.*    @;


1.3
date	95.05.10.12.03.17;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.05.05.03.32.50;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.05.05.03.14.38;	author dz;	state Exp;
branches;
next	;


desc
@U1 dox
@


1.3
log
@update
@
text
@.*
.*
.*  $Log: U1.IPF $
.*  Revision 1.1  1995/05/10  12:55:16  dz
.*  Initial revision
.*
.*    Revision 1.2  1995/05/05  03:32:50  dz
.*    everything moved out
.*
.*    Revision 1.1  1995/05/05  03:14:38  dz
.*    Initial revision
.*
.*
.*

:userdoc.
:docprof toc=123456.
:title.U1 Operator's Guide

:isyn root=dz.dz Zavalishin

.im title.ipf

.im hilights.ipf

.im howis.ipf

.im setup.ipf

.im coop.ipf

.im gremote.ipf

.im userman.ipf

.* :h1 res=22.Advanced operation

.im technote.ipf

.*:h1 res=24.local specific
.*     11.1&colon. Newsserver
.*     11.2&colon. uunpack

.im thanx.ipf


:euserdoc.

@


1.2
log
@everything moved out
@
text
@d4 3
d16 2
@


1.1
log
@Initial revision
@
text
@d3 3
a5 1
.*  $Log: U1.IPF $
.*  Revision 1.1  1995/05/10  12:55:16  dz
.*  Initial revision
.*
d8 1
d14 1
a14 432

:h1 res=1.Title Page
:p.:p.
:lines align=center.
:font facename='Tms Rmn' size=60x80.
U 1
:font facename='Tms Rmn' size=24x32.

                              UUCP - FIDO gateway 
:font facename='Helv' size=12x20.
                                  Version 1.98
 
.* :artwork align=center name='StarCube.BMP'.
 
:font facename='Tms Rmn' size=24x32.
                                Operator's guide 
 
 
:font facename='Tms Rmn' size=12x20.
                  Copyright (C) 1991-1995 by Dmitry Zavalishin. 
 
:elines.

.*:link reftype=hd res=3.Go to Guide:elink.

.*:link reftype=hd res=2.Go look at the cover picture:elink.

.* :h1 res=2.Cover Picture
.* :artwork align=center name='uu2balls.BMP'.

.* :link reftype=hd res=3.Go to Guide:elink.




:h1 res=4.Introduction
:h2.It is a beta version!
:p.
Hi, dear user!
:p.
Well, this is another beta version of U1. It has bugs, it 
does all but the things you need it to, it doesn't work at all... 
Sure, I do my best to make it better and better, but don't judge 
me too strictly, if something is wrong! 
:p.Note as well, that the documentation you're reading
is in beta version too &colon.-).

:h2.Overview
:p.This product (hereinafter Gate) is the facility that 
enables you being the user both of UUCP network (such as 
Relcom, UUNET, etc.) and FIDO (FidoNet, LifNet, etc.) to 
integrate these networks. Gate is able to transfer messages 
from one network to another as well as integrate electronic 
conferences (echoes, newsgroups).

:h2.How to Get in Touch with the Author
:p.If you want to get in touch with the author of this 
thing with your comments, questions, suggestions and 
swearing you may&colon. 
:ul.
:li.Subscribe to UU2 echo.
:li.Send a letter via e&dash.mail&colon.
:dl.
:dt.Internet
:dd.dz@@phantom.ru
:dt.UUCP
:dd.funet.fi!kiae!phantom!dz
:dt.FIDO
:dd.Dmitry Zavalishin&comma. 2&colon.5020/32
:edl.
:li.Call by telephone number&colon. +7 (095) 110&dash.6728
:li.Send a snailmail letter to&colon.
Zavalishin D.K., Russia, Moscow, Artekovskaya str. 7-4, fl.260
:eul.
:p.:p.
:link reftype=hd res=5.(Next chapter):elink.

:h1 res=5.System Requirements
:h2.Hardware
:p.No special hardware required. Current version of U1
will run on any OS/2 compatible Intel x86 machine.

:h2.Software
:p.OS/2 2.0, 2.1, 2.11, Warp 3.0 or later required.

:p.U1 was tested (by myself or betatesters) with&colon.
Novell Netware 3.11, Lantastic, FrontDoor 2.01-2.10, 
BinkleyTerm 2.50-2.55 (and some sideway betas, such as EE 
beta D and E), D'Bridge, Imail 1.10-1.21, Squish 1.00-1.01, 
UUPC/Extended 1.12k. 
:p.In addition I must mention that U1 was used in multichannel 
FIDO systems in a situation when gate modules work 
in both channels simultaneously. 

:p.
:link reftype=hd res=6.(Next chapter):elink.


:h1 res=6.Hilights
:ul.
:li.Two address convertion schemes at once&colon. Traditional 
(Z&colon.N/F.P <&dash.&dash.> pP.fF.nN.zZ.domain) and userlist&dash.based one. 

:li.Optional access restriction&colon. You can define domains which are accessible
for non-registered FIDOnet-side user. 

:li.News/echo gating in viamail/viauux/viarsh modes or via user-defined program. 
Compressed feeds supported. 

:li.Complete activity log, optionally with debugging info. 

:li.Statistics support&colon. all the letters passing gateway are logged together
with their origin and destination address, direction (to or from fidonet) 
and size. Just import this information to your DBMS and prepare any 
reports you like. 

:li.Remote access module&colon. broadcasts messages to all registered gate users&comma. 
sends help file on user request, etc. 

:li.Easy way to add specific headlines to your letters. Need 
'Return&dash.Receipt&dash.To&colon.' or 'Distribution&colon.' headline? Just write it in the 
first line of the message text - gate will move it into the header of 
resulting uucp message. 

:li.Cc, Return-Receipt supported. 

:li.Automatic UUencode of (small) attached files.

:li.Can be easily combined with nearly any UUCP clone. 

:li.Handy uucp address representation in Fido messages - in almost all the 
cases you don't need to add that stupid 'To&colon.' line to your message. 

:li.Advanced FIDO standards, such as FSC-0044, FSC-0035 are implemented. 

:li.Unlimited number of echo/newsgroups supported. There is no limit at 
all - adding more and more group names to list just slows conversion
down a bit.

:li.Optional Cyrillic code tables conversion. Additional code conversion
tables can be added to new versions of gate. 

:li.Large messages from UUCP to FIDO are PKZIPped and converted to  
file-attaches on the fly. 

:li.Filtering of control messages in UseNet news (such as cancel, newgroup...) 

:li.Selective headline elimination system&colon. Define which headlines you want 
to see in FIDO messages. Definitions for netmail and echomail are 
separate. Unwanted headlines can be stripped or hidden under ^A. 

:li.Three log file formats - my own, BinkleyTerm and FrontDoor lookalike. 

:li.Powerfull log file filter&colon. all the messages are divided into classes. 
You can turn logging of each class of messages on or off separately. 

:li.Fidogate's 'Comment&dash.To&colon.' headline supported. 

:li.Ability to add 'Distribution' headline to all the messages in any newsgroup. 

:li.Well&comma. i'm tired to enumerate &colon.)... Just set it up and try yourself! 
:eul.
:p.
:link reftype=hd res=7.(Next chapter):elink.


:h1 res=7.The way it works
:h2.Mail transfer
:h3.Mail from FIDO to UUCP
:p.This is what FIDO2UU.EXE is needed for. Being executed, 
FIDO2UU.EXE performs the following steps&colon. 
:ol.
:li.Searches NETMAIL folder of your FIDO system for 
messages, that should be processed. Messages, that will be 
processed by FIDO2UU.EXE must have destination address, that 
is equal to the address, given in any FTN
parameter of U1 setup, and destination user name that is 
equal to the name given in the Magic parameter of U1 
setup or contains character '@@'. 

:li.Each of the messages found is converted to UUCP- 
style one as follows&colon. 

:li.Origin address is searched in USERS.U1 and replaced 
with corresponding UUCP one, if found. If more than 
one matching occurrence exists, first one is used. If 
origin address is not found in USERS.U1, user is 
treated as not registered and some restrictions are 
applied. (See below). In this case address is 
converted to 
:xmp.
     FirstName_LastName@@pPOINT.fNODE.nNET.zZONE.<U1_DOMAIN>
:exmp.
:p.Where <U1_DOMAIN> is choosen from FTN setup parameter, that is
most closely matches FidoNet address of the letter's sender.

:li.If FIDO destination username field contains '@@', it's 
value is used as UUCP destination address. Otherwise, 
message is searched for the line of form 'To&colon. 
<uucp_address>' and value of <uucp_address> is used 
as destination. 

:li.Message is searched for FIDO-style CC lines, all of 
those lines are converted to RFC-822 form and added 
to resulting message header to inform recipient about 
addresses where copies of letter was sent to. 

:li.Message is searched for U1&dash.specific XCC lines ('XCC&colon. 
<uucp_address>'). Uucp-addresses, that are given in 
those lines are added to those listed in CC headlines 
of resulting message, and copies of message are sent 
to those addresses, as well, as to primary destination address. 

:li.Before sending message to destination or XCC address, 
validity of addresses is checked, if user is not a 
registered one. Address assumed to be valid if all of 
it's parts, exept for local name (as defined in 
RFC-822) are found among the contents of Free setup 
file parameter. 
:p.
Example&colon.
:p.
SETUP.U1 contains&colon.
:xmp.
    Free mycompany.com borland.com john.UUCP Frank
:exmp.
:p.
Valid addresses&colon.
:xmp.
    me@@myhost.mycompany.com
    john.UUCP!JohnHimSelf@@borland.com
    Hey@@Frank%borland.com
    Frank!root
:exmp.
:p.
Invalid addresses&colon.
:xmp.
    john!root
    Somebody@@borland.org
:exmp.

:li.Message beginning is searched for all other lines of 
form 'Name&colon. Value'&comma. and all of them are converted to 
UUCP message headlines. 

:eol.
:h3.Mail from UUCP to FIDO
:p.UU2FIDO.EXE is responsible for it. Your UUCP package 
must run this program instead of it's usual mail delivery 
program (rmail or sendmail). To achieve that you need to 
rename UU2FIDO.EXE to RMAIL.EXE or SENDMAIL.EXE, according 
to what name mail delivery program has in your package. Make 
sure you have not deleted original delivery program - gate 
needs it to be able send mail to UUCP side. 

:p.Another way to set up gate can be used if your UUCP is 
able to call external delivery program to deliver mail to 
specified domains. In this case you don't need to rename 
anything at all - just set up your UUCP package to call 
UU2FIDO.EXE. (For detailed discussion on this method please 
see :link reftype=hd res=12.cooperation with UUPC 1.12:elink.). 

:h3.Viamail news entry point
:p.If one of the delivery addresses is 'newsmaster' (this 
name is configurable, of course), the letter will be sent to 
the echo-mail too. For example, (if the configuration is the 
same as it is in SETUP.U1 file of documentation) execution 
of the command "uu2fido dz newsmaster < news_article" will 
lead to the sending of file contents to the user dz and, 
what is more, to the conversion of file contents into echo- 
packet in compliance with the contents of Newsgroups 
headline. (In fact, UU2FIDO.EXE just stores the copy of 
message in the U1 inbound news spool. The conversion to 
echo-packet is performed by UNBATCH.EXE) 


:h2.News transfer

:h3.Echo-Mail to News
:p.This is a quite sophisticated process. SNEWS and BATCH 
programs are responsible for it. To have the echo being
sent, your echo-processor has to make the packet with echo- 
mail. The packet must be made for the first address placed in 
ECHOADDRESS parameter of configuration file and it must not 
be compressed by some compression program. In particular, 
there is a possibility to create non-compressed packets in 
Imail 1.21 echo-processor. After starting with key -s, 
SNEWS.EXE will search for the netmail letters which have been sent 
to ECHOADDRESS with echo-packet attached to them
and then snews program will try to convert and 
delete the echo-packet. You may also attach snews to echo- 
processor as an additional compression program. In such a 
mode the program converts the packet and deletes it instead 
of compressing it. Nevertheless, it does not surprise the 
echo-processor usually. The only trouble is that the 
processor in this case generates the letter to which, as it 
seems to him, the file is attached. You may clean these 
letters by running SNEWS.EXE -s, but what is more likely, 
the echo-processor will delete them in its next pass. 

:p.The program SNEWS.EXE converts the letters of FIDO 
format into UseNet format and places them in the special 
directory, from whitch they may be taken by the program 
BATCH.EXE later on. As a result of BATCH.EXE operation the 
letters are converted into the news packets. In turn, the 
packets are sent to the receiving systems using method, 
specified in configuration file (FEEDS.U1) for that system. 

:h3.News to Echo
:p.The recieving of news also proceeds by two stages. On 
the first stage, uu2fido (ViaMail) or rnews (ViaUux) program 
receives the news packet and places it into inbound spool- 
directory. The program UU2FIDO.EXE considers as news 
everything, that have been sent to the address, placed in 
the configuration file in the parameter NEWSMASTER. The 
contents of the spool-directory is handled by UNBATCH.EXE. 
This program converts news to echo-packets addressed to your 
node and places them in the your FIDO software inbound files 
directory. Usually it is enough for your echo-processor to 
find it there and to import. 
:p.
:link reftype=hd res=8.(Next chapter):elink.





:h1 res=8.Setting things up
:p.Ok, in general, installation of U1 is not too 
complex... for experienced FIDOnet sysop... who understands 
UUCP technology as well &colon.). Here's the common case described 
step&dash.by&dash.step&colon. 

:ol.
:li.Find (or create) good lovely directory to put all the junk to.
:li..Copy all the files there.
:li.Edit U1 setup file.
:p.Here you will need to tell the gate which thing it 
must call to send uucp mail (RmailEXE parameter). Usually
it will be something like rmail.exe or sendmail.exe - refer 
to your UUCP package to find this out and... 
:ul.
:li.1st way&colon. rename it! You'll have to replace original 
RMAIL.EXE (or whatever you have) with UU2FIDO.EXE. So&colon. 

:ol.
:li.Rename RMAIL.EXE to (say) UURMAIL.EXE
:li.Rename UU2FIDO.EXE to RMAIL.EXE
:li.In SETUP.U1 set RmailEXE parameter to have value UURMAIL.EXE.
:eol.

:li.2nd way&colon. Make sure your UUCP package's RMAIL.EXE will 
call uu2fido to deliver letters to domain you're going to
use for FIDO/FTN mail. Don't rename anything, in SETUP.U1 
set RmailEXE parameter to have value RMAIL.EXE (or whatever 
it happens to be in your UUCP package). 
:eul.

:li.Set U1 environment variable in your AUTOEXEC file - 
it must contain the name of directory you put U1 setup files to. 

:li.Go to your echoprocessor setup and tell echoprocessor 
that you've got a new compression program called U1, and that 
it is known that to compress mail packet with that program 
echoprocessor must execute 'SNEWS.EXE name.pkt' or 
'SNEWS.EXE -i anything name.pkt'. You will use this 'compression' 
program to export echoes to gate. Assign it to the node or point 
which address you have in EchoAddress setup parameter of U1, 
than add that address to export list of any echo you wand to 
gate out. 
:eol.

:p.

:link reftype=hd res=9.(Next chapter):elink.


:h1 res=9.Cooperation with
:p.In the following subsections I'm describing 'interesting' 
details of U1 installation together with a different 3-rd party 
software packages. 

:p.If you can't find your beloved mailer, uucp package or packer here,
please try to install U1 without additional help &colon.) and send me any 
information you want me to include in this chapter &colon.). 
:p.
:link reftype=hd res=10.UUCP packages:elink.
:p.
:link reftype=hd res=14.Echo-Processors:elink.
:p.
:link reftype=hd res=20.(Next chapter):elink.
.br


:h2 res=10.UUCP packages
:p.
.* :link reftype=hd res=11.Waffle 1.65:elink.
:link reftype=hd res=12.UUPC/1.12 Extended:elink.
.* :link reftype=hd res=13.UUPC/* by Ache:elink.

:h3 res=11 hide noprint nosearch.Waffle 1.65
:p.As it was said before, there's two differrent ways of U1 
installation exist - one with replacing RMAIL.EXE with 
UU2FIDO.EXE, and one without such replacement. 
:dl break=all.
:dt.Replacing rmail.exe
:dd. Rename (Waffle's) RMAIL.EXE to UURMAIL.EXE, rename (U1's) 
UU2FIDO.EXE to RMAIL.EXE and put it to the directory where 
RMAIL.EXE was&comma. in SETUP.U1 write&colon. 
:xmp.
RmailEXE <correct_drive_&amp._dir>&bsl.uurmail.exe
RmailExitL -1
RmailExitR 0
:exmp.
:p.
:dt.Cooperating with RMAIL.EXE
:dd.Don't rename anything. In SETUP.U1 write
:xmp.
RmailEXE <correct_drive_&amp._dir>&bsl.rmail.exe
RmailExitL -1
RmailExitR 0
:exmp.
:p.
In <waffle_dir>&bsl.uucp&bsl.paths write&colon.
:xmp.
&per.fidonet.org                   | <U1_dir>\uu2fido.exe
<U1_domain>                   | <U1_dir>\uu2fido.exe
:exmp.

:p.NB! Make sure you didn't set PassUnknownBack mode in U1 setup! 
:edl.

d16 1
d18 1
d20 1
d22 1
d24 1
a24 12
:h3 res=12.UUPC/1.12 Extended
:p.
:dl break=all.
:dt.Replacing rmail.exe
:dd.Rename (uupc's) RMAIL.EXE to UURMAIL.EXE, rename (U1's) 
UU2FIDO.EXE to RMAIL.EXE and put it to the directory where 
RMAIL.EXE was&comma. in SETUP.U1 write&colon. 
:xmp.
RmailEXE <correct_drive_&amp._dir>&bsl.uurmail.exe
RmailExitL -1
RmailExitR 0
:exmp.
d26 1
a26 1
:dt.Cooperating with RMAIL.EXE
d28 1
a28 19
:p.NB! The following method was not tested, I'm not sure it works. 
:dd.Don't rename anything. In SETUP.U1 write
:xmp.
RmailEXE <correct_drive_&amp._dir>&bsl.rmail.exe
RmailExitL -1
RmailExitR 0
:exmp.
:p.
In <uupc_dir>&bsl.conf&bsl.hostpath write&colon.
:xmp.
&asterisk..fidonet.org                  | <U1_dir>\uupc2uu2.cmd
<U1_domain>                   | <U1_dir>\uupc2uu2.cmd
:exmp.
:p.
In <U1_dir>&bsl.uupc2uu2.cmd write&colon.
:xmp.
<U1_dir>\uu2fido %3@@%2
:exmp.
:p.NB! Make sure you didn't set PassUnknownBack mode in U1 setup! 
d30 1
a30 1
:edl.
a31 539




:h3 res=13 hide noprint nosearch.UUPC/* by Ache
;p.
Versions supported&colon.
:dl.
:dt.3.00
:dd.completely
:dt.4.07
:dd.no viauux news reception possible
:dt.5.00
:dd.PassUnknownBack will not work well.
:edl.

:p.Installation is nearly the same as for UUPC 1.12. The only
difference is the value of the following items in SETUP.U1&colon.
:xmp.
          RmailExitL          0
          RmailExitR          48
:exmp.
.*     See also&colon.
.*        :link reftype=hd res=10.Cooperation with UUCP packages:elink.
.*        :link reftype=hd res=14.Cooperation with Echo processors:elink.


:h2 res=14.Echo-processors
:ul.
:li.:link reftype=hd res=15.Squish 1.01:elink.
.* :li.:link reftype=hd res=16.GEcho 1.00:elink.  
.* :li.:link reftype=hd res=17.IMail 1.21:elink.  
.* :li.:link reftype=hd res=18.FastEcho:elink.
.* :link reftype=hd res=19.TossScan 1.0:elink.
:eul.
:p.
See also&colon.
:link reftype=hd res=10.Cooperation with UUCP packages:elink.

:h3 res=15.Squish 1.01
:p.
The following is how to export echoes to gate&colon.
:p.
SETUP.U1&colon.
:xmp.
          ...
          Address             2&colon.5020/23.0&atsign.fidonet.org uucp
          NewsAddress         2&colon.5020/23.1&atsign.fidonet.org
          ...
:exmp.
:p.
ECHOES.U1&colon.
:xmp.
          ...
          fido.bermudian.sysop          Bermudian.Sysop
          fido.bermudian.talks          Bermudian.Talks
          ...
:exmp.
:p.
SQUISH.CFG&colon.
:xmp.
          ...
          Address 2&colon.5020/23
          ...
          Compress compress.cfg
          Routing route.cfg
          ...
          Pack U1 2&colon.5020/23.1
          ...
          ; The following two groups are to be gated to news
          ;
          EchoArea Bermudian.Sysop x&colon./Somewhere/bsys &dash.&dollar.m200 2&colon.5020/23.1
          EchoArea Bermudian.Talks x&colon./Somewhere/btlk &dash.&dollar.m200 2&colon.5020/23.1
          ...
:exmp.
:p.
COMPRESS.CFG&colon.
:xmp.
          ...
          Archiver U1
                 Extension U1
                 Ident 0,NothingOfAGreatValue
                 Add x&colon.&bsl.uu2&bsl.snews %f
          End Archiver
          ...
:exmp.
:p.
ROUTE.CFG&colon.
:xmp.
          Send hold file 2&colon.5020/23.1
:exmp.
:p.
.* See also&colon.
.*         :link reftype=hd res=10.Cooperation with UUCP packages:elink.
.*         :link reftype=hd res=14.Cooperation with Echo processors:elink.



:h3 res=16 hide noprint nosearch.GEcho 1.00
.br
.br
The following is how to export echoes to gate&colon.
.br
.br
System data/Compression programs/8 menu&colon.
.br
:p.:p.
:cgraphic.
          ีอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ 
          ณ                                                               ณ 
          ณþARC PKPAK -a       ีอออออออออออออออออออออออออ   8 ธ           ณ 
          ณ ARJ ARJ a -e -m1 -sณ                              ณ           ณ 
          ณ LZH LHA a /mt      ณ Program  SNEWS               ณ           ณ 
          ณ PAK PAK A /L /ST   ณ Switches -i                  ณ           ณ 
          ณ ZIP PKZIP -ao      ณ Memory   640                 ณ           ณ 
          ณ ZOO ZOO a&colon.         ณ                              ณ           ณ 
          ณ SQZ SQZ a /q0 /z3  ิออออออออออออออออออออออออออออออพ           ณ 
          ณ   8                                                           ณ 
          ณ   9                                                           ณ 
          ณ  10                                                           ณ 
          ณ GUS {+}                                                       ณ 
          ณ                                                               ณ 
          ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ 
:ecgraphic.
.br
.br
Node manager menu&colon.
.br
:p.:p.
:cgraphic.
          ี New entry ออออออออออออออออออออออออออออออออ Node manager ธ 
          ณ                                                         ณ 
          ณ Node address        2&colon.5020/23.1                         ณ 
          ณ Sysop name                                              ณ 
          ณ Route via           2&colon.5020/23.1                         ณ 
          ณ Packet password                                         ณ 
          ณ AreaMgr password                                        ณ 
          ณ Authorized groups                                       ณ 
          ณ Remote maintenance  No                                  ณ 
          ณ Allow rescan        No                                  ณ 
          ณ Forward requests    No                                  ณ 
          ณ Send notify list    No                                  ณ 
          ณ Compression type    SNEWS -i                            ณ 
          ณ Mail archive status Hold                                ณ 
          ณ Direct              Yes                                 ณ 
          ณ                                                         ณ 
          ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ 
:ecgraphic.
.br
.br
Add 2&colon.5020/23.1 to the export list of all areas you want to gate to UseNet.
.br
.br
     See also&colon.
.br
        :link reftype=hd res=10.Cooperation with UUCP packages:elink.
.br
        :link reftype=hd res=14.Cooperation with Echo processors:elink.
.br


:h3 res=17 hide noprint nosearch.IMail 1.21
.br
.br
Compression Programs menu&colon.
.br
.br
          Program 8      snews -i
.br
.br
Node Export Manager menu&colon.
.br
:p.:p.
:cgraphic.
          Address        2&colon.5020/23.1 
          Password 
          Groups 
          Direct         Y 
          Remote Maint   N 
          Status         Hold 
          Program        snews -i 
          Capability     Type 2+ 
          CW Handling    Force 
:ecgraphic.
.br
:p.In General configuration/Product codes menu add product code 'CC'. 
.br
:p.Add node 2&colon.5020/23.1 to the export list of all echoes you want to 
export to UseNet. 
.br
.br
     See also&colon.
.br
        :link reftype=hd res=10.Cooperation with UUCP packages:elink.
.br
        :link reftype=hd res=14.Cooperation with Echo processors:elink.
.br


:h3 res=18 hide noprint nosearch.FastEcho
.br
.br
Edit/Compression Programs menu&colon.
.br
:p.:p.
:cgraphic.
          ีออออออออออออออออออออออออออออออออออออออออออCompression Programsอธ 
          ณ    Tag  Command                                               ณ 
          ณฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤณ 
          ณ A) UU2  snews -i                                              ณ 
          ณ B) ฐฐฐ                                                        ณ 
          ณ C)                                                            ณ 
          ณ D)                                                            ณ 
          ณ E)                                                            ณ 
          ณ F)                                                            ณ 
          ณ G)                                                            ณ 
          ณ H)                                                            ณ 
          ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ 
:ecgraphic.
.br
.br
Edit/Node Configuration menu&colon.
.br
:p.:p.
:cgraphic.
          ีออออออออออออออออออออออออออออออออออออNode-Managerอธ 
          ณ                                                 ณ 
          ณ     Node&colon. 2&colon.5020/23.1                     4D&colon. Y ณ 
          ณ                                      Tosscan&colon. N ณ 
          ณ Your Aka&colon. 2&colon.5020/23.0               D'Bridge&colon. N ณ 
          ณ                                 ARCmail 0.60&colon. N ณ 
          ณ Password&colon.                             Direct&colon. Y ณ 
          ณ                                        Crash&colon. N ณ 
          ณ                                         Hold&colon. Y ณ 
          ณ   Packer&colon. UU2                      Umlaut&colon. None ณ 
          ณ                                                 ณ 
          ิอออออออออออออออออออออออออออออออออออออออออออออออออพ 
:ecgraphic.
.br
:p.Add 2&colon.5020/23.1 to the export list of echoes you want to gate to UseNet. 
.br
.br
     See also&colon.
.br
        :link reftype=hd res=10.Cooperation with UUCP packages:elink.
.br
        :link reftype=hd res=14.Cooperation with Echo processors:elink.
.br
:h3 res=19 hide noprint nosearch.TossScan 1.0
.br
:p.   In general, U1 shouldn't be used with TossScan since TossScan 
strips off '@@' character from FIDO message headers. 
.br
.br
.br
     See also&colon.
.br
        :link reftype=hd res=10.Cooperation with UUCP packages:elink.
.br
        :link reftype=hd res=14.Cooperation with Echo processors:elink.
.br






:h1 res=20.Remote control unit
:p.This utility (GREMOTE.EXE), being started without any 
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
contents of special file (See HELPFILE config. file option).

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








:h1 res=21.User's manual
:ul.
:li.Sending letters via gate (FIDO side)
:ol.

:li.Address
:p.Well, you going to send a letter to Internet. Recipient's
address is "jack@@jpc.super.org", gate you will use is placed
at 1&colon.333/22 and it's &cdq.magic name&cdq. is UUCP. (Gate's magic name
can be set up in MAGIC parameter of configuration file.) Two
ways exist to prepare that letter.

:ul.
:li.First method&colon.
:p.FIDO destination address of letter must be &cdq.UUCP&comma. 1&colon.333/22&cdq.&comma. 
the very first line of letter text must be &cdq.To&colon. jack&atsign.jpc.super.org&cdq.. 
:li.Second method&colon.
:p.FIDO destination address  must be  &cdq.jack&atsign.jpc.super.org&comma. 1&colon.333/22&cdq.. 
This method is much more convinient, but can be used only if Internet 
address is short enough to fit into the FIDO username field. 
:eul.

:li.Netmail reply
:p.When replying to letter, which came from gate, you always 
have correct FIDO destination address, because U1 makes FIDO 
letter "from" field to contain best kind of address possible, 
filling username subfield with Internet address if it is short 
enough, or with gate's magic name otherwise. In the latter case 
you need to type in &cdq.To&colon. Internet_address&cdq. in the first line of 
your reply manually (Unless your message editor supports 
FTS-0034/35 recommendations). Note that Internet address of the  
person you replying to can be found in a special kludge line 
(REPLYTO) U1 inserts. 

:li.Receipt Request
:p.Letters with RRQ flag are confirmed with a fragment of 
the gate log file, which corresponds to the processing of the 
letter. In addition, resulting uucp letter will carry 
Return-Receipt-Request headline. 

:li.Additional headlines
:p.In the very first lines of the letter you may insert the 
lines like &cdq.name&colon. value&cdq.. They must be started from the first 
position and must not have blanks between a keyword and a colon. 
It is desireable to make the first letters of the key words capital. 
All the lines of such a form (down to the first line not answering  
to this description) are considered to be the lines of RFC822-header 
of the letter and are placed there. Their width is limited by 75 
symbols. 
:p.:p.
Example&colon. 
:cgraphic.
          | 
          |To&colon. myfriend&atsign.somewhere.us 
          |To&colon. news&atsign.kiae.su 
          |Newsgroups&colon. comp.lang.std.c++ 
          |Distribution&colon. su 
          |Return&dash.Receipt&dash.To&colon. me&atsign.mydomain.su 
          | 
          |<&dash. this is the left border of editor window &colon.) 
:ecgraphic.

:li.CC feature
:p.To send a letter to the several addresses in uucp network you 
may use the following way&colon. place at the very beginning of the letter   
the line 'xcc&colon. address one_more_address ...'. Abbreviation 'xcc'  
(instead of the usual 'cc') is chosen so that the line would not be 
interpreted by FIDO software. 

:li.Text formatting
:p.Gate will automatically reformat the lines with the width more 
than 80 characters. Formatting is executed without right border 
indentation and hyphenation. Unfortunately, by now it is not possible  
to disable the formating despite of the fact, that sometimes I want 
to (to transfer wide table, for example). On the other hand, not every  
network may transfer the letters with the line's width more than 80 
symbols, so gate in some sense provides standartability to your 
letters. 

:li.UUENCODE
:p.If you need to send a small file to Internet, you don't have to 
encode it yourself and stuff into the letter. Just attach file to the 
letter, and U1 will uuencode it for you and send along with the text 
of letter. Please don't attach large files, 'cause U1 does not split  
'em up into the parts before encoding and resulting letter will be too 
large to pass through. 

:eol.
:li.Sending letters via gate (UUCP side)
:ol.
:li.Comment-To
:p.When posting an article into the newsgroup which is gated to  
FIDO with U1, you can force gate to set FIDO echomail message 'To'  
field to anything you want using a special &cdq.Comment&dash.To&colon.&cdq. headline. 
Syntax of that headline is &cdq.Comment&dash.To&colon. User Name <address>&cdq.. User 
name given will replace the usual &cdq.To&colon. All&cdq. in resulting echo message&comma.  
and address is simply ignored. 
:eol.
:eul.




.* :h1 res=22.Adanced operation


:h1 res=23.Technical Notes
:h2 res=231.General Technical Notes

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
:p.U1 supports FIDO standards
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
:p.Kludge lines parsed&colon.
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
:p.In short, Gate registers errors in the log file and makes
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


d36 1
a36 18


:h1 res=25.Thanx...
:p.People I'm really thankfull to...

:p.Alex Belyaev, Alex Popov, Alex Rudnev, Alex Selkov, Alex Sherbakov,
Alex Snigerev, Alex Young, Andew Bloschintsev, Andrew Chernov (Ache),
Andrew Kovalev, Andy Nickolayev, Boris Mostovoy, Boris Tyshkievitch,
Dmitry Baronov, Dmitry Khrustalev, Egons Bush, Eric Fletcher (Eugeny
Chupriyanov), Eugene Crosser, Eugene Peskin, Fyodor Evseev, Gena Rez,
Gennady Zvonov, Grubina Olga (Russian dox edition), Harry Bush, Igor
Sviridov, Joseph Urban, Kira Stratonnokova (my wife!), Kirill Lebedew,
Mark Patterson, Max Mikheenkov, Michael Bravo, Mike Skulkin, Natasha
Razdaeva (partial dox translation to english), Nikki Chumakov, Oleg
Nesterov, Oleg Polovinkin, Peter Zeltins, Sergey Ryzshkov (Initiation!),
Sergic Dobridnuk, Vad Potapov, Vadim Getmanshchuck, Valery Bardin and
all the Relcom team (Relcom network!), Yuri PQ, Yuri Yashnev (Financial
support), Serge Terekhov
a37 3
:p.If you feel you must be mentioned in this list, but is
not, I am sorry, sorry, sorry... Please, forgive me - it is
really hard to recall all the people I discussed U1 with!
@
