:h3 clear res=811 x=left y=top width=25% height=100%.Required parameters
:p.
The following parameters must be defined.
:p.:link reftype=hd res=8111 dependent auto.
:p.:link reftype=hd res=8111 dependent.FTN:elink.
:p.:link reftype=hd res=8112 dependent.Magic:elink.
:p.:link reftype=hd res=8113 dependent.NetMail:elink.
:p.:link reftype=hd res=8114 dependent.NetFile:elink.
:p.:link reftype=hd res=8115 dependent.InFiles:elink.
:p.:link reftype=hd res=8116 dependent.Domain:elink.
:p.:link reftype=hd res=8117 dependent.Postmaster:elink.
:p.:link reftype=hd res=8118 dependent.UserList:elink.
:p.:link reftype=hd res=8119 dependent.TZ:elink.

:h4 res=8111 x=25% y=top width=75% height=100% group=3.FTN
:p.
FTN Address/Mapping definition(s)
:p.
This record describes
:link reftype=fn refid=ftn.FTN:elink.
gate operates in, sets gate
address in that network and defines UUCP address mapping.
:p.
Format:
:xmp.
FTN Name AKA UUCP-Domain [z=FZ-TZ]
:exmp.
:dl tsize=20.
:dt.Name
:dd.Short name of Fido Technology Network described
:dt.AKA
:dd.Our address in that network, INCLUDING DOMAIN
:dt.UUCP-Domain
:dd.domain to use for this FTN on UUCP side
:dt.FZ-TZ
:dd.Lower and upper value of zone number - used for domain guessing.
:edl.
:p.There may be two FTN records, which differ only in AKA part.
:xmp.
FTN fidonet 2&colon.5020/32.256@fidonet.org fidonet.org z=1-6
FTN fidonet 2&colon.5020/32.512@fidonet.org fidonet.org z=1-6
:exmp.
:fn id=ftn.
:p.
FTN Stands for FidoNet Technology Network.
:efn.


:h4 res=8112 x=25% y=top width=75% height=100% group=3.Magic
:p.
Magic name of our gate. Usually - UUCP.
Used to send mail to be gated to, if uucp address is too
long to fit in 36 characters.
:xmp.
Magic UUCP
:exmp.

:h4 res=8113 x=25% y=top width=75% height=100% group=3.Netmail
:p.
Your FIDO mailer netmail directory. Netmail folder must be in
*.MSG format for gate to work properly.

:xmp.
NetMail    n&colon.\mail
:exmp.

:h4 res=8114 x=25% y=top width=75% height=100% group=3.NetFile
:p.
Directory where your FIDO mailer stores received files and
where your echo-processor searches for inbound echo-packets.

:xmp.
NetFile    n&colon.\files\in
:exmp.

:h4 res=8115 x=25% y=top width=75% height=100% group=3.InFiles
:p.
Additional directories to scan file attaches in

:xmp.
InFiles    n&colon.\files\in\insecure,n&colon.\files\in\unlisted
:exmp.

:h4 res=8116 x=25% y=top width=75% height=100% group=3.Domain
:p.
Your Internet domain. If you have no one, use machine-name.UUCP

:xmp.
Domain    isoft.msk.su
:exmp.

:h4 res=8117 x=25% y=top width=75% height=100% group=3.PostMaster
:p.
UUCP-name of postmaster - person, who is responsible for gate
operation. Copies of gate error report letters will be sent
to him/her. Also he will receive all the letters, which address
was not understood by gate (If not in PassUnknownBack mode).

:xmp.
PostMaster    postmaster
:exmp.

:p.
:warning.There must be corresponding entry in :link reftype=hd res=8118.userlist:elink., for example&colon.
:xmp.
postmaster   2&colon.5020/32 Post_Master
:exmp.

:h4 res=8118 x=25% y=top width=75% height=100% group=3.UserList
:p.
File with list of registered gate users.
See :link reftype=hd res=82.userlist reference:elink..

:xmp.
UserList    u&colon.\gate\users.u1
:exmp.

:h4 res=8119 x=25% y=top width=75% height=100% group=3.TZ
:p.
Time zone in format +/-HHMM. Moscow is +0300/+0400.
:xmp.
TZ    +0300
:exmp.

