head	1.3;
access;
symbols;
locks;
comment	@.*    @;


1.3
date	96.04.20.16.12.22;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.05.10.12.03.17;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.05.05.03.32.50;	author dz;	state Exp;
branches;
next	;


desc
@cooperation
@

1.3
log
@*** empty log message ***
@
text
@.*
.*
.*  $Log: coop.ipf $
.*    Revision 1.2  1995/05/10  12:03:17  dz
.*    update
.*
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
:p.As it was said before, there's two different ways of U1 
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
<U1_dir>\uu2fido %3@@%2
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
          FTN FidoNet 2&colon.5020/23.0&atsign.fidonet.org fidonet.org z=1-6
          Magic uucp
          EchoAddress   2&colon.5020/23.1&atsign.fidonet.org 2&colon.5020/23.0&atsign.fidonet.org
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

Node manager menu&colon.

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


Edit/Node Configuration menu&colon.

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

:p.Add 2&colon.5020/23.1 to the export list of echoes you want to gate to UseNet. 




@


1.2
log
@update
@
text
@d1 329
a329 328
.*
.*
.*  $Log: coop.ipf $
.*  Revision 1.1  1995/05/10  12:55:16  dz
.*  Initial revision
.*
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
<U1_dir>\uu2fido %3@@%2
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

Node manager menu&colon.

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


Edit/Node Configuration menu&colon.

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

:p.Add 2&colon.5020/23.1 to the export list of echoes you want to gate to UseNet. 




@



1.1
log
@Initial revision
@
text
@d1 5
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
@
