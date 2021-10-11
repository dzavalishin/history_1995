:h3 clear res=819 x=left y=top width=25% height=100%.Gremote parameters
:p.
The following parameters must be defined for gremote to work.

:p.:link reftype=hd res=8191 dependent auto.
:p.:link reftype=hd res=8191 dependent.HelpFile:elink.
:p.:link reftype=hd res=8192 dependent.PassWord:elink.


:h4 res=8191 x=25% y=top width=75% height=100% group=6.HelpFile
:p.
Gremote sends this file in response to ':link reftype=hd res=20.Help:elink.' command.

:xmp.
HelpFile    u&colon.\gate\gatehelp.txt
:exmp.

:h4 res=8192 x=25% y=top width=75% height=100% group=6.Password
:p.
Password for privileged gremote commands.

:xmp.
PassWord    Oh-John!-Oh-No!
:exmp.

