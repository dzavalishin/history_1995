.*
.*
.*  $Log: howis.ipf $
.*    Revision 1.2  1995/05/10  12:03:17  dz
.*    update
.*
.*
.*

:h1 res=7.The way it works
:p.
Following pages describe in general the way U1 works.

:h2.Mail transfer
:p.:i1.Mail transfer
Programs UU2Fido.exe and Fido2UU.exe are e-mail
(as opposed to news and echoes) gateway parts.

:h3.Mail from FIDO to UUCP
:p.:i1.Fido2UU.exe
This is what FIDO2UU.EXE is needed for. Being executed, 
FIDO2UU.EXE performs the following steps&colon. 
:ol.
:li.Searches NETMAIL folder of your FIDO system for 
messages, that should be processed. Messages, that will be 
processed by FIDO2UU.EXE must have destination address, that 
is equal to the address, given in any FTN
parameter of U1 setup, and destination user name that is 
equal to the name given in the Magic parameter of U1 
setup or contains character '@'. 

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
     FirstName_LastName@pPOINT.fNODE.nNET.zZONE.<U1_DOMAIN>
:exmp.
:p.Where <U1_DOMAIN> is chosen from FTN setup parameter, that is
most closely matches FidoNet address of the letter's sender.

:li.If FIDO destination username field contains '@', it's 
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
it's parts, except for local name (as defined in 
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
    me@myhost.mycompany.com
    john.UUCP!JohnHimSelf@borland.com
    Hey@Frank%borland.com
    Frank!root
:exmp.
:p.
Invalid addresses&colon.
:xmp.
    john!root
    Somebody@borland.org
:exmp.

:li.Message beginning is searched for all other lines of 
form 'Name&colon. Value'&comma. and all of them are converted to 
UUCP message headlines. 

:eol.
:h3.Mail from UUCP to FIDO
:p.:i1.UU2Fido.exe
UU2FIDO.EXE is responsible for it. Your UUCP package 
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
:p.:i1.News transfer
News/echo gateway is more complex than e-mail and has twice more parts: 
RNews.exe and Unbatch.exe for news to echo conversion and SNews.exe and
Batch.exe - for echo to news.

:h3.Echo-Mail to News
:p.:i1.SNews.exe
:i1.Batch.exe
This is a quite sophisticated process. SNEWS and BATCH 
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
directory, from which they may be taken by the program 
BATCH.EXE later on. As a result of BATCH.EXE operation the 
letters are converted into the news packets. In turn, the 
packets are sent to the receiving systems using method, 
specified in configuration file (FEEDS.U1) for that system. 

:h3.News to Echo
:p.:i1.RNews.exe
:i1.Unbatch.exe
The receiving of news also proceeds by two stages. On 
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

