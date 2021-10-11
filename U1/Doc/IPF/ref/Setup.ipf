:p.
U1 setup file is a plain text file called :hp5.Setup.U1:ehp5.,
which is placed in a directory pointed by :hp2.U1:ehp2. environment variable.
:p.
For example, if your setup file is :hp5.f&colon.\Network\U1\Setup.U1:ehp5.
you must include :hp2.SET U1=f&colon.\Network\U1:ehp2. for gate to work
properly.
:p.
Setup file can contain :hp6.comments:ehp6. and :hp6.parameters:ehp6..
Any text from semicolon character to the end of line is a comment and
is ignored by U1. Each parameter is beginning with a :hp6.keyword:ehp6..
Keywords can be written in any case (even mixed).
:warning.Maximal total line length is 200 characters.
:p.
Among all the parameters :hp6.flags:ehp6. are those, that can have
value 'Yes' or 'No' only. Note that default is 'Yes', if flag keyword
is used alone in a setup file.
:p.Example&colon.
:xmp.
Announce    Yes  ; Turn option on
Announce    No   ; Turn option off
Announce         ; Turn option on
Announce    XYZ  ; Turn option on, issue error message
:exmp.

.im ref\required.ipf

.im ref\optional.ipf

.im ref\newsecho.ipf

.im ref\grempar.ipf

