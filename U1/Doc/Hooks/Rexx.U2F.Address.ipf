:h2 
    x=left y=top width=25% height=100%
    scroll=none group=1 clear
    res=81.Rexx.U2F.Address&colon. UU2Fido UUCP Address Hook
:link reftype=hd res=811
      vpx=25% vpy=top vpcx=75% vpcy=100%
      group=2 auto dependent.
:p.
:link reftype=hd res=811
      vpx=25% vpy=top vpcx=75% vpcy=100%
      group=2 dependent.Description:elink.
:p.
:link reftype=hd res=812
      vpx=25% vpy=top vpcx=75% vpcy=100%
      group=3 dependent.Example:elink.


:h3 res=811 x=25% y=top width=75% height=100%.Description of Rexx.U2F.Address

:dl break=all.
:dt.:hp6.Purpose:ehp6.
:dd.This hook can be used to preprocess (convert) uucp destination address
of the letter before passing it to uucp to fido address translator of U1.
Note that translating address to user@p#.f#.n#.z# form you can avoid nearly
all further translation of the address by gate.

:dt.:hp6.Call reason:ehp6.
:dd.This hook is called for each destination address of letter passed to 
UU2Fido.

:dt.:hp6.Parameters:ehp6.
:dd.The only parameter is destination uucp address.

:dt.:hp6.Return value:ehp6.
:dd.This hook must return new uucp address for message or its
first parameter if no replacement needed.

:edl.

:h3 res=812 x=25% y=top width=75% height=100%.
Sample script for Rexx.U2F.Address
:xmp.
/**
 * Address conversion hook for UU2Fido.exe. Does nothing.
 *
 * Input:  uucp address of recipient
 *
 * Output: replacement address
 *
**/

/* Load variable 'inaddr' with value of the first arg. */
Parse Arg inaddr

/* initialize output to default */
OutString = inaddr           

/* Put debug info to log file */
log 'Rexx.U2F.Address: InAddr is 'inaddr

/* return string, removing blanks on the front or back. */
Return Strip(OutString)                
:exmp.
