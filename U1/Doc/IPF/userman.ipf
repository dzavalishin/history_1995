.*
.*
.*  $Log: userman.ipf $
.*    Revision 1.2  1995/05/10  12:03:17  dz
.*    update
.*
.*
.*

:h1 res=21.User's manual
:i1.User's manual
:ul.
:li.Sending letters via gate (FIDO side)
:ol.

:li.Address
:p.Well, you going to send a letter to Internet. Recipient's
address is "jack@jpc.super.org", gate you will use is placed
at 1&colon.333/22 and it's &cdq.magic name&cdq. is UUCP. (Gate's magic name
can be set up in MAGIC parameter of configuration file.) Two
ways exist to prepare that letter.

:ul.
:li.First method&colon.
:p.FIDO destination address of letter must be &cdq.UUCP&comma. 1&colon.333/22&cdq.&comma. 
the very first line of letter text must be &cdq.To&colon. jack&atsign.jpc.super.org&cdq.. 
:li.Second method&colon.
:p.FIDO destination address  must be  &cdq.jack&atsign.jpc.super.org&comma. 1&colon.333/22&cdq.. 
This method is much more convenient, but can be used only if Internet 
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
It is desirable to make the first letters of the key words capital. 
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
:p.:i1.Formatting
Gate will automatically reformat the lines with the width more 
than 80 characters. Formatting is executed without right border 
indentation and hyphenation. Unfortunately, by now it is not possible  
to disable the formating despite of the fact, that sometimes I want 
to (to transfer wide table, for example). On the other hand, not every  
network may transfer the letters with the line's width more than 80 
symbols, so gate in some sense provides standartability to your 
letters. 

:li.UUENCODE
:p.:i1.UUENCODE
If you need to send a small file to Internet, you don't have to 
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
