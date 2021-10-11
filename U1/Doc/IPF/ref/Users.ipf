:p.
Users.U1 file contains list of registered gate users. Name of this file must
be given in
:link reftype=hd res=8118.UserList:elink. setup file parameter.
:p.
Setup file can contain :hp6.comments:ehp6. and :hp6.user definitions:ehp6..
Any text from semicolon character to the end of line is a comment and
is ignored by U1.
:p.
User definition general form is:
:fig.
uucp_name [parameters] fido_address fido_name
:efig.
:p.
Parameters start with '-' or '/' and have form
:fig.
-key=value
:efig.
:p.
Currently these parameters are defined:
:dl compact tsize=20.
:dt.-from=domain
:dd.used to define domain, used in uucp From: field
:dt.-filedir=path
:dd.directory to put fileattaches for this user to
:edl.

:note.uucp_name can have domain as well, but, currently, it will be ignored by U1.
:p.
Examples:
:xmp.

; Entry for postmaster
postmaster          2&colon.5020/32.0     Postmaster
; Forward mail to UUCP to postmaster too
uucp                2&colon.5020/32.0     Postmaster

; dz@isoft.msk.su, default domain used
dz                  2&colon.5020/32.0     Dmitry_Zavalishin
; same as above
dz@                 2&colon.5020/32.0     Dmitry_Zavalishin

; dz-1@dialnet.msk.su, explicit domain used
dz-1@dialnet.msk.su 2&colon.5020/23.4     Dmitry_Zavalishin_1

; dz-2@pd.isoft.msk.su, partially defined domain - pasted to default
dz-2@pd.            2&colon.5020/23.77    Dmitry_Zavalishin_2

; Entry for newsmaster
newsmaster          2&colon.5020/32.0     NewsMaster
newsmaster          2&colon.5020/23.55    NewsMaster
newsmaster          2&colon.5020/23.4     NewsMaster


:exmp.
