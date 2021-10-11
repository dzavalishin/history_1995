:h3 clear res=816 x=left y=top width=25% height=100%.News/echo parameters
:p.
The following parameters must be defined for news/echo gateing to work.

:p.:link reftype=hd res=8161 dependent auto.
:p.:link reftype=hd res=8161 dependent.EchoAddress:elink.
:p.:link reftype=hd res=8162 dependent.NewsMaster:elink.
:p.:link reftype=hd res=8163 dependent.Origin:elink.
:p.:link reftype=hd res=8164 dependent.Organization:elink.
:p.:link reftype=hd res=8165 dependent.News headlines elimination:elink.
:p.:link reftype=hd res=8166 dependent.GateId:elink.
:p.:link reftype=hd res=8167 dependent.InNewsSpool:elink.
:p.:link reftype=hd res=8168 dependent.OutNewsSpool:elink.
:p.:link reftype=hd res=8169 dependent.NewsPath:elink.
:p.:link reftype=hd res=8170 dependent.EchoList:elink.
:p.:link reftype=hd res=8171 dependent.FeedList:elink.
:p.:link reftype=hd res=8172 dependent.CnewsFeed:elink.
:p.:link reftype=hd res=8173 dependent.EchoMsgSize:elink.
:p.:link reftype=hd res=8174 dependent.PktSize:elink.
:p.:link reftype=hd res=8175 dependent.NoCrossPost:elink.
:p.:link reftype=hd res=8176 dependent.NewsBadGroupMSG:elink.
:p.:link reftype=hd res=8177 dependent.NewsControlMSG:elink.

:h4 res=8161 x=25% y=top width=75% height=100% group=5.EchoAddress
:p.
Address pair, used by news gate. Will be used for
echo packet headers, FIDO PATH/SEEN-BY lines. 
First address is an address of newsgate. It will be used
in 'From' field of .PKT files, generated by gate. Second one
is an address these .PKTs will have in 'To' field.

:xmp.
EchoAddress   2&colon.5020/128.0@fidonet.org 2&colon.5020/32.256@fidonet.org
:exmp.

:h4 res=8162 x=25% y=top width=75% height=100% group=5.NewsMaster
:p.
Uucp username of ViaMail receiver. Your newsfeed must send
news to this address in your domain in ViaMail mode.
There must be corresponding entry in user table.

:xmp.
NewsMaster    newsmaster
:exmp.

:h4 res=8163 x=25% y=top width=75% height=100% group=5.Origin
:p.
Origin line added to echo messages by gate. If not defined,
origin line is built from Usenet message Organization headline.

:xmp.
Origin    Infinity Soft Labs
:exmp.

:h4 res=8164 x=25% y=top width=75% height=100% group=5.Organization
:p.
Used to form Organization headline in outgoing news articles.

:xmp.
Organization    Infinity Soft Labs
:exmp.

:h4 res=8165 x=25% y=top width=75% height=100% group=5.News headlines elimination
:p.
Selective headlines elimination. See
:link reftype=hd res=8126.Mail{Pass/Hide/Kill}HL:elink..
Note that NewsHideHl is implemented, unlike MailHideHL.


:xmp.
NewsPassHL *
NewsHideHL X-* Organization Resent-* Distribution
NewsKillHL Path Lines Newsgroups From Subject Message-Id Reply-To Date Sender
:exmp.

:h4 res=8166 x=25% y=top width=75% height=100% group=5.GateId
:p.
Optional. :hp7.Don't:ehp7. define if not sure that you need it.
:p.
This string is appended to Organization headline and used to
check for dupes from uucp side. (Yes, path headline is 
generated and checked too, but when U1 used with newsserver,
this method doesn't work because newsservers strips path
headlines)
:p.
Default value for this parameter contains your domain name.

:xmp.
GateId (isoft_gate)
:exmp.

:h4 res=8167 x=25% y=top width=75% height=100% group=5.InNewsSpool
:p.
Optional. Don't define if not sure that you need it.
:p.
Used to hold incoming news articles.
:p.
Default value&colon. %U1%\news.in
:p.
Directory will be created automatically, if not exists.

:xmp.
InNewsSpool    u&colon.\gate\news.in
:exmp.

:h4 res=8168 x=25% y=top width=75% height=100% group=5.OutNewsSpool
:p.
Optional. Don't define if not sure that you need it.
:p.
Used to hold outgoing news articles.
:p.
Default value&colon. %U1%\news.out
:p.
Directory will be created automatically, if not exists.

:xmp.
OutNewsSpool    u&colon.\gate\news.out
:exmp.

:h4 res=8169 x=25% y=top width=75% height=100% group=5.NewsPath
:p.
Thing gate will put in 'Path&colon.' headline of outgoing
news articles and check for in incoming ones to find dupes.
You can set complex things here like 'isoft!isoft.msk.su' -
all the string will be put to 'Path&colon.' headline of outgoing messages,
but incoming message's path will be checked only against first
component of this parameter's value (in our example - 'isoft').
Please don't include 'newsgate' or something like username -
U1 adds '!newsgate' to path line automatically.

:xmp.
NewsPath    isoft!isoft.msk.su
:exmp.

:h4 res=8170 x=25% y=top width=75% height=100% group=5.EchoList
:p.
List of gated newsgroup/echo names&colon. used for group name translation

:xmp.
EchoList    u&colon.\gate\echoes.u1
:exmp.

:h4 res=8171 x=25% y=top width=75% height=100% group=5.FeedList
:p.
List of uucp nodes we send gated news to. See example.

:xmp.
FeedList    u&colon.\gate\feeds.u1
:exmp.

:h4 res=8172 x=25% y=top width=75% height=100% group=5.CnewsFeed
:p.
Makes U1 to act as a simple Cnews-type system - feed
received cnews articles back to uucp.

:xmp.
CnewsFeed
:exmp.

:h4 res=8173 x=25% y=top width=75% height=100% group=5.EchoMsgSize
:p.
Max. size of echomail message gate will create.
:p.
Default - 12 Kbytes.

:xmp.
EchoMsgSize    12000
:exmp.

:h4 res=8174 x=25% y=top width=75% height=100% group=5.PktSize
:p.
Recommended size of .PKT files created by gate

:xmp.
PktSize    400000
:exmp.

:h4 res=8175 x=25% y=top width=75% height=100% group=5.NoCrossPost
:p.
Turn off crossposting. Messages from usenet to FIDO
will be posted only to the first area found in Echoes.U1

:xmp.
NoCrossPost
:exmp.

:h4 res=8176 x=25% y=top width=75% height=100% group=5.NewsBadGroupMSG
:p.
Defines the way of processing of messages, which
have no known newsgroup tag in their Newsgroups&colon. headline
:p.
Possible values&colon.
:p.
:dl compact.
:dt.Separate
:dd.Put message to special echo. (UU2.BAD_GROUP)
:dt.Kill
:dd.Don't process (delete) it.
:dt.Pass
:dd.Pass unchanged. (Use last group name from Newsgroups headline as echo tag)
:edl.

:xmp.
NewsBadGroupMSG    Separate
:exmp.

:h4 res=8177 x=25% y=top width=75% height=100% group=5.NewsControlMSG
:p.
Defines the way of processing of messages, which
have Control&colon. headline.
:p.
Possible values&colon.
:p.
:dl compact.
:dt.Separate
:dd.Put message to special echo. (UU2.CONTROL)
:dt.Kill
:dd.Don't process (delete) it.
:dt.Pass
:dd.Pass unchanged.
:edl.
:xmp.
NewsControlMSG    Separate
:exmp.

