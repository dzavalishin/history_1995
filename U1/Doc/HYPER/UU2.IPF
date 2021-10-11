:userdoc.
:docprof toc=123456.
:title.UU2

.* helpC generated IPF Source File
.* part of the help development kit, (c) 1992, 93 Loewy Ron

:h1 res=1.Title Page
:p.:p.
:lines align=center.
U U 2 
 
 
                              UUCP - FIDO gateway 
 
                                  Version 1.12 
 
:artwork align=center name='StarCube.BMP'.
 
 
                                Operator's guide 
 
 
 
                  Copyright (C) 1991-1994 by Dmitry Zavalishin. 
 
:elines.
.br
.br
:link reftype=hd res=3.Go to Guide:elink.
.br
:link reftype=hd res=2.Go look at the cover picture:elink.
:h1 res=2.Cover Picture
:artwork align=center name='uu2balls.BMP'.
.br
:link reftype=hd res=3.Go to Guide:elink.
:h1 res=3.UU2 Guide
.br
.br
      
.br
                                 UU2 Topics 
.br
.br
      ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ
.br
.br
.br
        Chapter I       :link reftype=hd res=4.Introduction:elink.
.br
        Chapter II      :link reftype=hd res=5.System Requirements:elink.
.br
        Chapter III     :link reftype=hd res=6.Hilights:elink.
.br
        Chapter IV      :link reftype=hd res=7.The way it works:elink.
.br
        Chapter V       :link reftype=hd res=8.Setting up:elink.
.br
        Chapter VI      :link reftype=hd res=9.Cooperation with...:elink.
.br
        Chapter VII     :link reftype=hd res=20.Utilities:elink.
.br
        Chapter VIII    :link reftype=hd res=21.User's Manual:elink.
.br
        Chapter IX      :link reftype=hd res=22.Advanced operation:elink.
.br
        Chapter X       :link reftype=hd res=23.Technical notes:elink.
.br
        Chapter XI      :link reftype=hd res=24.local specific (RelCom):elink.
.br
        Chapter XII     :link reftype=hd res=25.My thanx to:elink.
.br
:h1 res=4.Introduction
.br
.br
     1.1&colon. It is a beta version!
.br
.br
          Hi, dear user!
.br
:p.     Well, this is another beta version of UU2. It has bugs, it 
does all but the things you need it to, it doesn't work at all... 
Sure, I do my best to make it better and better, but don't judge 
me too strictly, if something is wrong! 
.br
.br
.br
     1.2&colon. Overview
.br
:p.     This product  (hereinafter  Gate)  is the  facility  that 
enables you being  the user both  of UUCP  network (such  as 
Relcom, UUNET,  etc.) and  FIDO (FidoNet,  LifNet, etc.)  to 
integrate these networks. Gate is able to transfer  messages 
from one network to another as well as integrate  electronic 
conferences. 
.br
.br
     1.3&colon. How to Get in Touch with the Author
.br
:p.     If you want  to get in  touch with  the author of  this 
thing  with  your   comments,  questions,  suggestions   and 
swearing you may&colon. 
.br
         o  Subscribe to UU2 echo.
.br
.br
         o  Send a letter via e&dash.mail&colon.
.br
.br
          Internet
.br
                    dz@phantom.msk.su
.br
.br
          UUCP
.br
                    funet.fi!kiae!dialnet!pd!dz
.br
.br
          FIDO
.br
                    Dmitry Zavalishin&comma. 2&colon.5020/32
.br
.br
         o  Call by telephone number&colon. +7 (095) 110&dash.6728
.br
.br
         o  Send a snailmail letter to&colon.
.br
.br
          Zavalishin D.K.
.br
          Russia, Moscow, Artekovskaya str. 7-4, fl.260
.br
.br
.br
 :link reftype=hd res=5.(Next chapter):elink.
.br
:h1 res=5.System Requirements
.br
.br
     2.1&colon. Hardware
.br
.br
:p.     No special hardware  required. Current  version of  UU2 
will run on  any PC  compatible machine,  including PC  XT, 
with 8086, V20 and  up to 486 processor.  I suppose it  will 
work even on not "true-compatible"  machines, but if not  - 
sorry, I had no such beast to test UU2 on. 
.br
.br
.br
     2.2&colon. Software
.br
.br
:p.     MS or PC-DOS 3.30 or later required. For those who runs 
OS/2&colon. native OS/2 2.1 version is partially complete&comma. contact me 
to get OS/2 executables. 
.br
:p.     UU2 was tested (by myself or betatesters) with&colon. MS DOS 
5.0, OS/2 2.1 DOS box, Novell Netware 3.11, Lantastic,  QEMM 
6.03 (stealth  mode), DesqView/X 1.0, FrontDoor 2.01-2.10, 
BinkleyTerm 2.50-2.55 (and some sideway betas, such as  EE 
beta D), D'Bridge, Imail 1.10-1.21, Squish 1.00-1.01, 
TossScan (patched, see section  6.2.5 at page 18), Waffle 
UUCP 1.64-1.65, UUPC/Extended 1.11, UUPC/* by Ache 2.0-5.0. 
:p.     In addition I must mention that UU2 was used in  multi- 
channel FIDO systems in a  situation when gate modules  work 
in both channels simultaneously. 
.br
.br
 :link reftype=hd res=6.(Next chapter):elink.
.br
:h1 res=6.Hilights
:p.Two address convertion schemes at once&colon. Traditional 
(Z&colon.N/F.P <&dash.&dash.> pP.fF.nN.zZ.domain) and userlist&dash.based one. 
.br
:p.Optional access restriction&colon. You can define domains which are accessible  
for non-registered FIDOnet-side user. 
.br
:p.News/echo gating in viamail/viauux/viarsh modes or via user-defined program. 
Compressed feeds supported. 
.br
:p.Complete activity log, optionally with debugging info. 
.br
:p.Statistics support&colon. all the letters passing gateway are logged together  
with their origin and destination address, direction (to or from fidonet) 
and size. Just import this information to your DBMS and prepare any 
reports you like. 
.br
:p.Remote access module&colon. broadcasts messages to all registered gate users&comma. 
sends help file on user request, etc. 
.br
:p.Easy way to add specific headlines to your letters. Need 
'Return&dash.Receipt&dash.To&colon.' or 'Distribution&colon.' headline? Just write it in the 
first line of the message text - gate will move it into the header of 
resulting uucp message. 
.br
:p.Cc, Return-Receipt supported. 
.br
:p.Automatic UUencode of (small) attached files.                                          
.br
:p.Can be easily combined with nearly any UUCP clone. 
.br
:p.Handy uucp address representation in Fido messages - in almost all the 
cases you don't need to add that stupid 'To&colon.' line to your message. 
.br
:p.Advanced FIDO standards, such as FSC-0044, FSC-0035 are implemented. 
.br
:p.Unlimited number of echo/newsgroups supported. There is no limit at 
all - adding more and more group names to list just slows conversion  
down a bit. Gate caches in memory up to 10Kb of group definitions. 
.br
:p.Optional Cyrillic code tables conversion. Additional code conversion  
tables can be added to new versions of gate. 
.br
:p.Large messages from UUCP to FIDO are PKZIPped and converted to    
file-attaches on the fly. 
.br
:p.Filtering of control messages in UseNet news (such as cancel, newgroup...) 
.br
:p.Selective headline elimination system&colon. Define which headlines you want 
to see in FIDO messages. Definitions for netmail and echomail are 
separate. Unwanted headlines can be stripped or hidden under ^A. 
.br
:p.Three log file formats - my own, BinkleyTerm and FrontDoor lookalike. 
.br
:p.Powerfull log file filter&colon. all the messages are divided into classes. 
You can turn logging of each class of messages on or off separately. 
.br
:p.Fidogate's 'Comment&dash.To&colon.' headline supported. 
.br
:p.Ability to add 'Distribution' headline to all the messages in any newsgroup. 
.br
:p.Well&comma. i'm tired to  enumerate &colon.)... Just set it up and try yourself! 
.br
.br
 :link reftype=hd res=7.(Next chapter):elink.
.br
:h1 res=7.The way it works
.br
.br
     4.1&colon. Mail transfer
.br
.br
.br
     4.1.1  ... from FIDO to UUCP
.br
:p.This is what FIDO2UU.EXE is needed for. Being executed, 
FIDO2UU.EXE performs the following steps&colon. 
.br
:p.     1. Searches NETMAIL folder of your FIDO system for 
messages, that should be processed. Messages, that will be 
processed by FIDO2UU.EXE must have destination address, that 
is equal to the address, given in Address or NewsAddress 
parameter of UU2 setup, and destination user name that is 
equal to the name given in the Address parameter of  UU2 
setup or contains character '@'. 
.br
:p.     2. Each of the messages found is converted to UUCP- 
style one as follows&colon. 
.br
:p.*  Origin address is searched in USERS.UU2 and replaced 
with corresponding UUCP one, if found. If more than 
one matching occurrence exists, first one is used. If 
origin address is not found in USERS.UU2, user is 
treated as not registered and some restrictions are 
applied. (See below). In this case address is 
converted to 
.br
.br
     FirstName_LastName@pPOINT.fNODE.nNET.zZONE.<UU2_DOMAIN>
.br
:p.Where <UU2_DOMAIN> is a value of Domain parameter, 
given in UU2 setup file. 
.br
:p.* If FIDO destination username field contains '@', it's 
value is used as UUCP destination address. Otherwise, 
message is searched for the line of form&colon. 'To&colon. 
<uucp_address>' and value of <uucp_address> is used 
as destination. 
.br
:p.* Message is searched for FIDO-style CC lines, all  of 
those lines are converted to RFC-822 form and added 
to resulting message header to inform recipient about 
addresses where copies of letter was sent to. 
.br
:p.&asterisk.  Message is searched for UU2&dash.specific XCC lines ('XCC&colon. 
<uucp_address>'). Uucp-addresses, that are given in 
those lines are added to those listed in CC headlines 
of resulting message, and copies of message are sent 
to those addresses, as well, as to primary destination address. 
.br
:p.*  Before sending message to destination or XCC address, 
validity of addresses  is checked, if  user is not  a 
registered one. Address assumed to be valid if all of 
it's parts,  @exept  for  local name  (as  defined  in 
RFC-822) are found among  the contents of Free  setup 
file parameter. 
.br
.br
&asterisk.  Example&colon.
.br
.br
   SETUP.UU2 contains&colon.
.br
       Free mycompany.com borland.com john.UUCP Frank
.br
.br
.br
   Valid addresses&colon.
.br
.br
       me@myhost.mycompany.com
.br
       john.UUCP!JohnHimSelf@borland.com
.br
       Hey@Frank%borland.com
.br
       Frank!root
.br
.br
   Invalid addresses&colon.
.br
.br
       john!root
.br
       Somebody@borland.org
.br
:p.*  Message beginning is searched for all other lines  of 
form 'Name&colon. Value'&comma. and all of them are converted to 
UUCP message headlines. 
.br
.br
.br
     4.1.2  ... from UUCP to FIDO
.br
:p.     UU2FIDO.EXE is responsible for it. Your UUCP package 
must run this program instead of it's usual mail delivery 
program (rmail or sendmail). To achieve that you need to 
rename UU2FIDO.EXE to RMAIL.EXE or SENDMAIL.EXE, according 
to what name mail delivery program has in your package. Make 
sure you have not deleted original delivery program - gate 
needs it to be able send mail to UUCP side. 
:p.     Another way to set up gate can be used if your UUCP is 
able to call external delivery program to deliver mail to 
specified domains. In this case you don't need to rename 
anything at all - just set up your UUCP package to call 
UU2FIDO.EXE. (For detailed discussion on this method please 
see section 6.1.1 at page 13). 
.br
.br
.br
     4.1.3  Viamail news entry point
.br
:p.     If one of the delivery addresses is 'newsmaster' (this 
name is configurable, of course), the letter will be sent to 
the echo-mail too. For example, (if the configuration is the 
same as it is in SETUP.UU2 file of documentation) execution 
of the command "uu2fido dz newsmaster < news_article" will 
lead to the sending of file contents to the user dz and, 
what is more, to the conversion of file contents into echo- 
packet in compliance with the contents of Newsgroups 
headline. (In fact, UU2FIDO.EXE just stores the copy of 
message in the UU2 inbound news spool. The conversion to 
echo-packet is performed by UNBATCH.EXE) 
.br
.br
.br
     4.2&colon. News transfer
.br
.br
.br
     4.2.1  ... Echo-Mail to News
.br
:p.     This is a quite sophisticated process. Snews and batch 
programs are responsible for it. To have the echo being 
sent, your echo-processor has to make the packet with echo- 
mail. The packet must be made for the  address placed in 
NEWSADDRESS parameter of configuration file and it must not 
be compressed by some compression program. In particular, 
there is a possibility to create  non-compressed packets in 
Imail 1.21 echo-processor. After starting with key -s, 
SNEWS.EXE will search for the letters which have been sent 
to NEWSADDRESS with echo-packet attached to them among 
netmail and then snews program will try to convert and 
delete the echo-packet. You may also attach snews to echo- 
processor as an additional compression program. In such a 
mode the program converts the packet and deletes it instead 
of compressing it. Nevertheless, it does  not surprise the 
echo-processor usually. The only trouble is that the 
processor in this case generates the letter to which, as it 
seems to him, the file is attached. You may clean these 
letters by running SNEWS.EXE -s, but what is more likely, 
the echo-processor will delete them in its next pass. 
:p.     The program SNEWS.EXE converts the letters of FIDO 
format into UseNet format and places them in the special 
directory, from whitch they may be taken by the program 
BATCH.EXE later on. As a result of BATCH.EXE operation the 
letters are converted into the news packets. In turn, the 
packets are sent to the  receiving systems using method, 
specified in configuration file (FEEDS.UU2) for that system. 
.br
.br
     4.2.2  ... News to Echo
.br
:p.     The recieving of news also proceeds by two stages. On 
the first stage, uu2fido (ViaMail) or rnews (@VaiUux) program 
receives the news packet and places it into inbound spool- 
directory. The program UU2FIDO.EXE considers as news 
everything, that have been sent to the address, placed in 
the configuration file in the parameter NEWSMASTER. The 
contents of the spool-directory is handled by UNBATCH.EXE. 
This program converts news to echo-packets addressed to your 
node and places them in the your FIDO software inbound files 
directory. Usually it is enough for your echo-processor to 
find it there and to import. 
.br
.br
 :link reftype=hd res=8.(Next chapter):elink.
.br
:h1 res=8.Setting things up
:p.     Ok, in general, installation of UU2 is not too 
complex... for experienced FIDOnet sysop... who understands 
UUCP technology as well &colon.). Here's the common case described 
step&dash.by&dash.step&colon. 
.br
.br
1.  Find (or create) good lovely directory to put all the junk to.
.br
.br
2.  Copy all the files there.
.br
.br
3.  Edit UU2 setup file.
:p.    Here you will need to tell the gate which thing it 
must call to send uucp mail (RmailEXE parameter). Usually  
it will be something like rmail.exe or sendmail.exe - refer 
to your UUCP package to find this out and... 
.br
:p. o 1st way&colon. rename it! You'll have to replace original 
RMAIL.EXE (or whatever you have) with UU2FIDO.EXE. So&colon. 
.br
.br
    1.  Rename RMAIL.EXE to (say) UURMAIL.EXE
.br
.br
    2.  Rename UU2FIDO.EXE to RMAIL.EXE
.br
.br
    3.  In SETUP.UU2 set RmailEXE parameter to have value UURMAIL.EXE.
.br
:p. o 2nd way&colon. Make sure your UUCP package's RMAIL.EXE will 
call uu2fido to deliver letters to domain you're going to  
use for FIDO/FTN mail. Don't rename anything, in SETUP.UU2 
set RmailEXE parameter to have value RMAIL.EXE (or whatever 
it happens to be in your UUCP package). 
.br
:p.4. Set UU2 environment variable in your AUTOEXEC file - 
it must contain the name of directory you put UU2 setup files to. 
.br
:p.5. Go to your echoprocessor setup and tell echoprocessor 
that you've got a new compression program called UU2, and that 
it is known that to compress mail packet with that program 
echoprocessor must execute 'SNEWS.EXE name.pkt' or 
'SNEWS.EXE -i anything name.pkt'. You will use this 'compression' 
program to export echoes to gate. Assign it to the node or point 
which address you have in NewsAddress setup parameter of UU2, 
than add that address to export list of any echo you wand to 
gate out. 
.br
.br
.br
.br
 :link reftype=hd res=9.(Next chapter):elink.
.br
:h1 res=9.Cooperation with
:p.   In the following subsections I'm describing 'interesting' 
details of UU2 installation together with a different 3-rd party 
software packages. 
.br
:p.   If you can't find your beloved mailer, uucp package or packer here,  
please try to install UU2 without additional help &colon.) and send me any 
information you want me to include in this chapter &colon.). 
.br
.br
.br
   :link reftype=hd res=10.UUCP packages:elink.      
.br
   :link reftype=hd res=14.Echo-Processors:elink.
.br
.br
.br
 :link reftype=hd res=20.(Next chapter):elink.
.br
:h1 res=10.Cooperatin with UUCP packages
.br
.br
.br
         :link reftype=hd res=11.Waffle 1.65:elink.    :link reftype=hd res=12.UUPC/1.11 Extended:elink.    :link reftype=hd res=13.UUPC/* by Ache:elink.
.br
:h1 res=11.Cooperation with Waffle 1.65
.br
:p.   As it was said before, there's two differrent ways of UU2 
installation exist - one with replacing RMAIL.EXE with 
UU2FIDO.EXE, and one without such replacement. 
.br
.br
     >>> Replacing rmail.exe
.br
:p.   Rename (Waffle's) RMAIL.EXE to UURMAIL.EXE, rename (UU2's) 
UU2FIDO.EXE to RMAIL.EXE and put it to the directory where 
RMAIL.EXE was&comma. in SETUP.UU2 write&colon. 
.br
.br
          RmailEXE <correct_drive_&amp._dir>&bsl.uurmail.exe
.br
          RmailExitL -1
.br
          RmailExitR 0
.br
.br
     >>> Cooperating with RMAIL.EXE
.br
.br
Don't rename anything. In SETUP.UU2 write
.br
.br
          RmailEXE <correct_drive_&amp._dir>&bsl.rmail.exe
.br
          RmailExitL -1
.br
          RmailExitR 0
.br
.br
In <waffle_dir>&bsl.uucp&bsl.paths write&colon.
.br
.br
          .fidonet.org                   | <UU2_dir>\uu2fido.exe
.br
          <UU2_domain>                   | <UU2_dir>\uu2fido.exe
.br
.br
:p.NB! Make sure you didn't set PassUnknownBack mode in UU2 setup! 
.br
.br
.br
     See also&colon.
.br
        :link reftype=hd res=10.Cooperation with UUCP packages:elink.
.br
        :link reftype=hd res=14.Cooperation with Echo processors:elink.
.br
:h1 res=12.Cooperation with UUPC/1.11 Extended
.br
.br
     >>> Replacing rmail.exe
.br
:p.     Rename (uupc's) RMAIL.EXE to UURMAIL.EXE, rename (UU2's) 
UU2FIDO.EXE to RMAIL.EXE and put it to the directory where 
RMAIL.EXE was&comma. in SETUP.UU2 write&colon. 
.br
.br
          RmailEXE <correct_drive_&amp._dir>&bsl.uurmail.exe
.br
          RmailExitL -1
.br
          RmailExitR 0
.br
.br
.br
     >>> Cooperating with RMAIL.EXE
.br
:p.NB! The following method was not tested, I'm not sure it works. 
.br
.br
Don't rename anything. In SETUP.UU2 write
.br
.br
          RmailEXE <correct_drive_&amp._dir>&bsl.rmail.exe
.br
          RmailExitL -1
.br
          RmailExitR 0
.br
.br
In <uupc_dir>&bsl.conf&bsl.hostpath write&colon.
.br
.br
          .fidonet.org                   | <UU2_dir>\uupc2uu2.bat
.br
          <UU2_domain>                   | <UU2_dir>\uupc2uu2.bat
.br
.br
In <UU2_dir>&bsl.uupc2uu2.bat write&colon.
.br
.br
          <UU2_dir>\uu2fido %3@%2
.br
.br
:p.NB! Make sure you didn't set PassUnknownBack mode in UU2 setup! 
.br
.br
     See also&colon.
.br
        :link reftype=hd res=10.Cooperation with UUCP packages:elink.
.br
        :link reftype=hd res=14.Cooperation with Echo processors:elink.
.br
:h1 res=13.Cooperation with UUPC/* by Ache
.br
.br
Versions supported&colon.
.br
.br
     3.00 - completely
.br
.br
     4.07 - no viauux news reception possible
.br
.br
     5.00 - PassUnknownBack will not work well.
.br
:p.    Installation is nearly the same as for UUPC 1.11. The only 
difference is the value of the following items in SETUP.UU2&colon. 
.br
.br
          RmailExitL          0
.br
          RmailExitR          48
.br
.br
.br
     See also&colon.
.br
        :link reftype=hd res=10.Cooperation with UUCP packages:elink.
.br
        :link reftype=hd res=14.Cooperation with Echo processors:elink.
.br
:h1 res=14.Cooperation with Echo-processors
.br
.br
      :link reftype=hd res=15.Squish 1.01:elink.  
.br
      :link reftype=hd res=16.GEcho 1.00:elink.  
.br
      :link reftype=hd res=17.IMail 1.21:elink.  
.br
      :link reftype=hd res=18.FastEcho:elink.
.br
      :link reftype=hd res=19.TossScan 1.0:elink.
.br
.br
   See also&colon.
.br
      :link reftype=hd res=10.Cooperation with UUCP packages:elink.
.br
:h1 res=15.Cooperation with Squish 1.01
.br
.br
The following is how to export echoes to gate&colon.
.br
.br
SETUP.UU2&colon.
.br
.br
          ...
.br
          Address             2&colon.5020/23.0&atsign.fidonet.org uucp
.br
          NewsAddress         2&colon.5020/23.1&atsign.fidonet.org
.br
          ...
.br
.br
ECHOES.UU2&colon.
.br
.br
          ...
.br
          fido.bermudian.sysop          Bermudian.Sysop
.br
          fido.bermudian.talks          Bermudian.Talks
.br
          ...
.br
.br
SQUISH.CFG&colon.
.br
.br
          ...
.br
          Address 2&colon.5020/23
.br
          ...
.br
          Compress compress.cfg
.br
          Routing route.cfg
.br
          ...
.br
          Pack UU2 2&colon.5020/23.1
.br
          ...
.br
          ; The following two groups are to be gated to news
.br
          ;
.br
          EchoArea Bermudian.Sysop x&colon./Somewhere/bsys &dash.&dollar.m200 2&colon.5020/23.1
.br
          EchoArea Bermudian.Talks x&colon./Somewhere/btlk &dash.&dollar.m200 2&colon.5020/23.1
.br
          ...
.br
.br
COMPRESS.CFG&colon.
.br
.br
          ...
.br
          Archiver UU2
.br
                 Extension UU2
.br
                 Ident 0,NothingOfAGreatValue
.br
                 Add x&colon.&bsl.uu2&bsl.snews %f
.br
          End Archiver
.br
          ...
.br
.br
ROUTE.CFG&colon.
.br
          Send hold file 2&colon.5020/23.1
.br
.br
     See also&colon.
.br
        :link reftype=hd res=10.Cooperation with UUCP packages:elink.
.br
        :link reftype=hd res=14.Cooperation with Echo processors:elink.
.br
:h1 res=16.Cooperation with GEcho 1.00
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
:h1 res=17.Cooperation with IMail 1.21
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
:h1 res=18.Cooperation with FastEcho
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
:h1 res=19.Cooperation with TossScan 1.0
.br
:p.   In general, UU2 shouldn't be used with TossScan since TossScan 
strips off '@' character from FIDO message headers. 
.br
.br
.br
     See also&colon.
.br
        :link reftype=hd res=10.Cooperation with UUCP packages:elink.
.br
        :link reftype=hd res=14.Cooperation with Echo processors:elink.
.br
:h1 res=20.Utilities
.br
.br
     7.1&colon. Remote control unit
.br
.br
:p.     This utility (GREMOTE.EXE), being started without any 
parameters, searches the letters which have been sent to the 
user with the name Gate and when found them, the utility 
tries to find and execute the commands contained in letter. 
It should be noted that the letters may come through Gate 
too - gremote ignores the lines of the RFC822 letter header. 
.br
.br
          Gremote understands the following commands&colon.
.br
.br
     HELP          
:p.Sends back a note concerning GREMOTE comands together with the 
contents of special file (See HELPFILE config. file option). 
.br
.br
     USERS :link refid=GREMOTE-SECURE reftype=fn.(Secure):elink.
:p.Sends the list of :link refid=REGISTERED-USERS reftype=fn.registered:elink. gate users. 
.br
.br
     ECHOES
:p.Sends the list of gated echo-, news-groups. 
.br
.br
     NOTIFY :link refid=GREMOTE-SECURE reftype=fn.(Secure):elink.
:p.Sends the letters to the all the :link refid=REGISTERED-USERS reftype=fn.registered:elink. users of 
gate with reminding, that they are gate users, what their address is and 
how they may ask for help. 
.br
.br
     BROADCAST :link refid=GREMOTE-SECURE reftype=fn.(Secure):elink.
:p.Distributes text following the comand (up to the end of the message)  
to the all the :link refid=REGISTERED-USERS reftype=fn.registered:elink. users. 
.br
.br
.br
.br
     7.2&colon. Statistics analyzer
.br
.br
.br
Still in alpha testing, sorry.
.br
.br
:fn id=GREMOTE-SECURE.
Secure (Privileged) GRemote commands:
The privileged commands may be executed only 
when a valid password is given. To use such 
commands you need to pass password in the 
Subject field of the letter.
:efn.
:fn id=REGISTERED-USERS.
Registered Users:
Registered users are the users, which are 
mentioned in the USERS.UU2 configuration file.
:efn.
:h1 res=21.User's manual
.br
.br
     8.1&colon. Sending letters via gate (FIDO side)
.br
.br
.br
     8.1.1  Address
.br
:p.     Well, you going to send a letter to Internet. Recipient's 
address is "jack@jpc.super.org", gate you will use is placed  
at 1&colon.333/22 and it's &cdq.magic name&cdq. is UUCP. (Gate's magic name  
can be set up in ADDRESS parameter of configuration file.) Two 
ways exist to prepare that letter. 
.br
.br
    First method&colon.
.br
:p.    FIDO destination address of letter must be &cdq.UUCP&comma. 1&colon.333/22&cdq.&comma. 
the very first line of letter text must be &cdq.To&colon. jack&atsign.jpc.super.org&cdq.. 
.br
.br
    Second method&colon.
.br
:p.    FIDO destination address  must be  &cdq.jack&atsign.jpc.super.org&comma. 1&colon.333/22&cdq.. 
This method is much more convinient, but can be used only if Internet 
address is short enough to fit into the FIDO username field. 
.br
.br
.br
     8.1.2  Netmail reply
.br
:p.     When replying to letter, which came from gate, you always 
have correct FIDO destination address, because UU2 makes FIDO 
letter "from" field to contain best kind of address possible, 
filling username subfield with Internet address if it is short 
enough, or with gate's magic name otherwise. In the latter case 
you need to type in &cdq.To&colon. Internet_address&cdq. in the first line of 
your reply manually (Unless your message editor supports 
FTS-0034/35 recommendations). Note that Internet address of the  
person you replying to can be found in a special kludge line 
(REPLYTO) UU2 inserts. 
.br
.br
     8.1.3  Receipt Request
.br
:p.     Letters with RRQ flag are confirmed with a fragment of 
the gate log file, which corresponds to the processing of the 
letter. In addition, resulting uucp letter will carry 
Return-Receipt-Request headline. 
.br
.br
     8.1.4  Additional headlines
.br
:p.     In the very first lines of the letter you may insert the 
lines like &cdq.name&colon. value&cdq.. They must be started from the first 
position and must not have blanks between a keyword and a colon. 
It is desireable to make the first letters of the key words capital. 
All the lines of such a form (down to the first line not answering  
to this description) are considered to be the lines of RFC822-header 
of the letter and are placed there. Their width is limited by 75 
symbols. 
.br
:p.:p.
:cgraphic.
          Example&colon. 
          | 
          |To&colon. myfriend&atsign.somewhere.us 
          |To&colon. news&atsign.kiae.su 
          |Newsgroups&colon. comp.lang.std.c++ 
          |Distribution&colon. su 
          |Return&dash.Receipt&dash.To&colon. me&atsign.mydomain.su 
          | 
          |<&dash. this is the left border of editor window &colon.) 
:ecgraphic.
.br
.br
     8.1.5  CC feature
.br
:p.     To send a letter to the several addresses in uucp network you 
may use the following way&colon. place at the very beginning of the letter   
the line 'xcc&colon. address one_more_address ...'. Abbreviation 'xcc'  
(instead of the usual 'cc') is chosen so that the line would not be 
interpreted by FIDO software. 
.br
.br
     8.1.6  Text formatting
.br
:p.     Gate will automatically reformat the lines with the width more 
than 80 characters. Formatting is executed without right border 
indentation and hyphenation. Unfortunately, by now it is not possible  
to disable the formating despite of the fact, that sometimes I want 
to (to transfer wide table, for example). On the other hand, not every  
network may transfer the letters with the line's width more than 80 
symbols, so gate in some sense provides @standartability to your 
letters. 
.br
.br
     8.1.7  UUENCODE
.br
:p.     If you need to send a small file to Internet, you don't have to 
encode it yourself and stuff into the letter. Just attach file to the 
letter, and UU2 will uuencode it for you and send along with the text 
of letter. Please don't attach large files, 'cause  UU2 does not split  
'em up into the parts before encoding and resulting letter will be too 
large to pass through. 
.br
.br
.br
     8.2&colon. Sending letters via gate (UUCP side)
.br
.br
.br
     8.2.1  Comment-To
.br
:p.     When posting an article into the newsgroup which is gated to  
FIDO with UU2, you can force gate to set FIDO echomail message 'To'  
field to anything you want using a special &cdq.Comment&dash.To&colon.&cdq. headline. 
Syntax of that headline is &cdq.Comment&dash.To&colon. User Name <address>&cdq.. User 
name given will replace the usual &cdq.To&colon. All&cdq. in resulting echo message&comma.  
and address is simply ignored. 
.br
:h1 res=22.Adanced operation
.br
.br
.br
.br
.br
.br
.br
.br
:h1 res=23.Technical Notes
.br
.br
.br
     10.1&colon. General
.br
.br
:p.The FTSC assigned product code for UU2 is CC hexadecimal. 
.br
:p.UU2 supports 3D/4D and partially 5D addresses. 
.br
:p.UU2 creates and is able to receive Type 2+ packets, provided they 
have correct capability word as well as it's validation copy. 
.br
:p.UU2 batcher doesn't add '#! cunbatch' line at the beginning of 
compressed UseNet batches. It is assumed that UU2 will be used  
with Cnews as the receiver of batches, which doesn't need 
'#! cunbatch' header. 
.br
:p.Echo&dash.News gateing&colon. text&comma. that follows origin line will be stripped. 
.br
:p.Executable files are not compressed because of overlays contained in. 
.br
:p.UU2 never gives up timeslices in multitasking environment since  
there's no situations when UU2 waits for some event to happen. 
.br
:p.I did my best to make sure UU2 will not cause any problems on the 
network or under multitasker, but nothing is guaranteed. But if you  
encounter some problem related to file locking or something else 
related to multitasking environment, please feel free to inform me. 
.br
.br
.br
     10.2&colon. Standards supported
.br
.br
:p.     UU2 supports FIDO standards FTS-0001, FTS-0004, FTS-0009,  
FSC-0035, FSC-0044, FSC-0053 and Internet standards RFC-822, 
RFC-1036, RFC-1123. 
.br
.br
.br
     10.3&colon. FIDO kludge lines
.br
.br
.br
          Kludge lines parsed&colon.
.br
:p.FMPT, TOPT,  INTL,  FLAGS, DOMAIN,  MSGID,  REPLY, PID. 
.br
.br
          Kludge lines created&colon.
.br
:p.FMPT, TOPT,  DOMAIN,  INTL, FLAGS,  MSGID,  REPLY, REPLYADDR, 
REPLYTO, PID. 
.br
:p.Please note that UU2 never attempts to find letter origin address 
in MSGID field. 
.br
.br
.br
     10.4&colon. A word on errors handling
.br
.br
:p.     In short, Gate registers errors in the protocol and makes 
the following efforts to secure your property if it failed 
to do something&colon. 
.br
.br
         -  Delivery error to UUCP
.br
:p.The letter will be generated to the sender of the initial message  
and to the postmaster. The letter contains the corresponded fragment  
of the UU2 log file and text of the original letter. 
.br
.br
         -  The recipient was not found during delievery to FIDO
.br
:p.The original letter will be sent to the postmaster. If he is also not 
found in the users' list - so much the worse for you. The letter is 
simply lost. 
.br
.br
         -  The news-group  has  not  found  in  the  list  while
.br
            converting of news into echo
.br
.br
The letter will be sent to the echo named bad_group.
.br
:p.         -  The echo-group has not found while converting of echo 
            into news 
.br
.br
The letter will be ignored.
.br
:p.     Among the all fatal errors the worst ones are those, that may 
occure during delievery of the letter from UUCP. They, usually result 
in loss of the letter, because some of UUCP packages, when delivering 
received messages, simply ignore exit codes, returned by delivery program. 
.br
.br
.br
:h1 res=24.local specific
.br
     11.1&colon. Newsserver
.br
.br
.br
     11.2&colon. uunpack
.br
.br
.br
.br
:h1 res=25.Thanx...
.br
.br
          People I'm really thankfull to...
.br
:p.     Alex Belyaev, Alex Popov, Alex Rudnev, Alex Selkov, Alex Sherbakov,   
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
.br
.br
:p.     If you feel you must be mentioned in this list, but is 
not, I am sorry, sorry, sorry... Please, forgive me - it is 
really hard to recall all the people I discussed UU2 with! 
.br
.br
:euserdoc.

