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



:h1 res=8.Setting things up
:p.Ok, in general, installation of U1 is not too 
complex... for experienced FIDOnet sysop... who understands 
UUCP technology as well &colon.). Here's the common case described 
step&dash.by&dash.step&colon. 

:ol.
:li.Find (or create) good lovely directory to put all the junk to.
:li.Copy all the files there. You may like to put a .DLL file to 
one of your LIBPATH directory.
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

:h2 res=81.Setup.U1 reference
.im ref\setup.ipf

:h2 res=82 group=1.Users.U1 reference
.im ref\users.ipf


.* :link reftype=hd res=9.(Next chapter):elink.
@@


1.1
log
@@Initial revision
@@
text
@@d1 5
d8 1
d17 2
a18 1
:li..Copy all the files there.
d56 7
@@
@
