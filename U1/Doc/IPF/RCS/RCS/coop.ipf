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
@@cooperation
@@


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

:h1 clear res=9.Cooperation with
:p.In the following subsections I'm describing 'interesting' 
details of U1 installation together with a different 3-rd party 
software packages. 

.* :p.If you can't find your beloved mailer, uucp package or packer here,
.* please try to install U1 without additional help &colon.) and send me any 
.* information you want me to include in this chapter &colon.). 
.* :p.
.* :link reftype=hd res=10.UUCP packages:elink.
.* :p.
.* :link reftype=hd res=14.Echo-Processors:elink.
.* :p.
.* :link reftype=hd res=20.(Next chapter):elink.




.im sendmail.ipf




.* :h2 res=10.UUCP packages
.* :p.
.* :link reftype=hd res=11.Waffle 1.65:elink.
.* :link reftype=hd res=12.UUPC/1.12 Extended:elink.
.* :link reftype=hd res=13.UUPC/* by Ache:elink.

:h2 res=11 hide noprint nosearch.Waffle 1.65
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






:h2 res=12.UUPC/1.12 Extended
:p.:i1.UUPC
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

:dt.Cooperating with RMAIL.EXE

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
<U1_dir>\uu2fido %3@@@@%2
:exmp.
:p.NB! Make sure you didn't set PassUnknownBack mode in U1 setup! 

:edl.





:h2 res=13 hide noprint nosearch.UUPC/* by Ache
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


.* :h2 res=14.Echo-processors
.* :ul.
.* :li.:link reftype=hd res=15.Squish 1.01:elink.
.* :li.:link reftype=hd res=16.GEcho 1.00:elink.  
.* :li.:link reftype=hd res=17.IMail 1.21:elink.  
.* :li.:link reftype=hd res=18.FastEcho:elink.
.* :link reftype=hd res=19.TossScan 1.0:elink.
.* :eul.
.* :p.
.* See also&colon.
.* :link reftype=hd res=10.Cooperation with UUCP packages:elink.

:h2 res=15.Squish 1.01
:p.:i1.Squish
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



:h2 res=16 hide noprint nosearch.GEcho 1.00

The following is how to export echoes to gate&colon.

System data/Compression programs/8 menu&colon.

:p.:p.
:cgraphic.
          ���������������������������������������������������������������͸ 
          �                                                               � 
          ��ARC PKPAK -a       ��������������������������   8 �           � 
          � ARJ ARJ a -e -m1 -s�                              �           � 
          � LZH LHA a /mt      � Program  SNEWS               �           � 
          � PAK PAK A /L /ST   � Switches -i                  �           � 
          � ZIP PKZIP -ao      � Memory   640                 �           � 
          � ZOO ZOO a&colon.         �                              �           � 
          � SQZ SQZ a /q0 /z3  ������������������������������;           � 
          �   8                                                           � 
          �   9                                                           � 
          �  10                                                           � 
          � GUS {+}                                                       � 
          �                                                               � 
          ���������������������������������������������������������������; 
:ecgraphic.

Node manager menu&colon.

:p.:p.
:cgraphic.
          � New entry �������������������������������� Node manager � 
          �                                                         � 
          � Node address        2&colon.5020/23.1                         � 
          � Sysop name                                              � 
          � Route via           2&colon.5020/23.1                         � 
          � Packet password                                         � 
          � AreaMgr password                                        � 
          � Authorized groups                                       � 
          � Remote maintenance  No                                  � 
          � Allow rescan        No                                  � 
          � Forward requests    No                                  � 
          � Send notify list    No                                  � 
          � Compression type    SNEWS -i                            � 
          � Mail archive status Hold                                � 
          � Direct              Yes                                 � 
          �                                                         � 
          ���������������������������������������������������������; 
:ecgraphic.

Add 2&colon.5020/23.1 to the export list of all areas you want to gate to UseNet.


:h2 res=17 hide noprint nosearch.IMail 1.21

Compression Programs menu&colon.

          Program 8      snews -i

Node Export Manager menu&colon.

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

:p.In General configuration/Product codes menu add product code 'CC'. 

:p.Add node 2&colon.5020/23.1 to the export list of all echoes you want to 
export to UseNet. 


:h2 res=18 hide noprint nosearch.FastEcho

Edit/Compression Programs menu&colon.

:p.:p.
:cgraphic.
          �������������������������������������������Compression Programs͸ 
          �    Tag  Command                                               � 
          ���������������������������������������������������������������ĳ 
          � A) UU2  snews -i                                              � 
          � B) ���                                                        � 
          � C)                                                            � 
          � D)                                                            � 
          � E)                                                            � 
          � F)                                                            � 
          � G)                                                            � 
          � H)                                                            � 
          ���������������������������������������������������������������; 
:ecgraphic.


Edit/Node Configuration menu&colon.

:p.:p.
:cgraphic.
          �������������������������������������Node-Manager͸ 
          �                                                 � 
          �     Node&colon. 2&colon.5020/23.1                     4D&colon. Y � 
          �                                      Tosscan&colon. N � 
          � Your Aka&colon. 2&colon.5020/23.0               D'Bridge&colon. N � 
          �                                 ARCmail 0.60&colon. N � 
          � Password&colon.                             Direct&colon. Y � 
          �                                        Crash&colon. N � 
          �                                         Hold&colon. Y � 
          �   Packer&colon. UU2                      Umlaut&colon. None � 
          �                                                 � 
          �������������������������������������������������; 
:ecgraphic.

:p.Add 2&colon.5020/23.1 to the export list of echoes you want to gate to UseNet. 




@@


1.1
log
@@Initial revision
@@
text
@@d1 5
d7 1
a7 1
:h1 res=9.Cooperation with
d12 8
a19 8
:p.If you can't find your beloved mailer, uucp package or packer here,
please try to install U1 without additional help &colon.) and send me any 
information you want me to include in this chapter &colon.). 
:p.
:link reftype=hd res=10.UUCP packages:elink.
:p.
:link reftype=hd res=14.Echo-Processors:elink.
:p.
d21 7
a27 1
.br
d30 2
a31 2
:h2 res=10.UUCP packages
:p.
d33 1
a33 1
:link reftype=hd res=12.UUPC/1.12 Extended:elink.
d36 1
a36 1
:h3 res=11 hide noprint nosearch.Waffle 1.65
d73 2
a74 2
:h3 res=12.UUPC/1.12 Extended
:p.
d114 1
a114 1
:h3 res=13 hide noprint nosearch.UUPC/* by Ache
d137 3
a139 3
:h2 res=14.Echo-processors
:ul.
:li.:link reftype=hd res=15.Squish 1.01:elink.
d144 4
a147 4
:eul.
:p.
See also&colon.
:link reftype=hd res=10.Cooperation with UUCP packages:elink.
d149 2
a150 2
:h3 res=15.Squish 1.01
:p.
d201 1
a201 4
:p.
.* See also&colon.
.*         :link reftype=hd res=10.Cooperation with UUCP packages:elink.
.*         :link reftype=hd res=14.Cooperation with Echo processors:elink.
d204 1
a205 3
:h3 res=16 hide noprint nosearch.GEcho 1.00
.br
.br
d207 1
a207 2
.br
.br
d209 1
a209 1
.br
d228 1
a228 2
.br
.br
d230 1
a230 1
.br
d251 1
a251 2
.br
.br
d253 1
a253 8
.br
.br
     See also&colon.
.br
        :link reftype=hd res=10.Cooperation with UUCP packages:elink.
.br
        :link reftype=hd res=14.Cooperation with Echo processors:elink.
.br
d255 1
a256 3
:h3 res=17 hide noprint nosearch.IMail 1.21
.br
.br
d258 1
a258 2
.br
.br
d260 1
a260 2
.br
.br
d262 1
a262 1
.br
d275 1
a275 1
.br
d277 1
a277 1
.br
d280 1
a280 8
.br
.br
     See also&colon.
.br
        :link reftype=hd res=10.Cooperation with UUCP packages:elink.
.br
        :link reftype=hd res=14.Cooperation with Echo processors:elink.
.br
d282 1
a283 3
:h3 res=18 hide noprint nosearch.FastEcho
.br
.br
d285 1
a285 1
.br
d301 2
a302 2
.br
.br
d304 1
a304 1
.br
d320 1
a320 1
.br
d322 2
a323 21
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
strips off '@@@@' character from FIDO message headers. 
.br
.br
.br
     See also&colon.
.br
        :link reftype=hd res=10.Cooperation with UUCP packages:elink.
.br
        :link reftype=hd res=14.Cooperation with Echo processors:elink.
.br
@@
@
