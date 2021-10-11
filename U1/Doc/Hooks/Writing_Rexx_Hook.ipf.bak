:userdoc.
:docprof toc=123456.
:title.U1 Rexx Hooks

.im Title_Page.ipf

:h1 res=2.What is REXX hook?
:p.
There it a lot of things in a gate, that differrent people like to be done
some unusual way. Examples are address translation, data compression,
accounting, etc etc. 

Instead of adding to the U1 all of the possible ways
of doing those things and, thus, making it grow bigger, it's configs to
become clumsier, I decided to let users control the way gate works themselves
by writing scripts in REXX that will intercept some data, processed by gate 
and convert that data the way user likes. 

The best example of such hook is UU2Fido
uucp address hook, which is called before gateing letter and given 
uucp address this letter is addressed to. 

Hook script can convert that address to anything
user seems to be appropriate and then return new address for gate to use
instead of the original one. If no hook provided, address will be passed as is
to U1 uucp to fido address convertor.

.* :artwork name='u2f_hook.met' fit.

:h1 res=3.How can I write my own REXX hook?
:fn id=rexxbook.
:p.
To reach that book open the :hp2.'Information':ehp2. folder on your
desktop and double-click :hp2.'Rexx Information':ehp2. object in it.
:efn.
:fn id=rexxext.
:p.
usually with an extension of .Rexx
:efn.
:p.
Every OS/2 system has a simple 
:link reftype=fn refid=rexxbook.book on REXX language:elink..
Please make sure you're familiar enough with REXX 
before trying to use REXX hooks. 
Each hook is 
:link reftype=hd res=8.described separately:elink.
 below, please read carefully about hook arguments, return value and situation,
in which that hook is called. REXX hook is a 
:link reftype=fn refid=rexxext.rexx program file:elink., 
which name is given in corresponding U1 setup file parameter. 
Most hooks are started with arguments and all of them must return some value. 

:dl break=all.

:dt.:hp6.Hook parameters:ehp6.
:dd.
The following line of REXX code will retrieve first argument of REXX hook into
the variable 'First_Arg':
:xmp.
Parse Arg First_Arg
:exmp.

:dt.:hp6.Return values:ehp6.
:dd.
The following line of REXX code will return result of script to
the caller and terminate execution of script:
:xmp.
/**
 * Assume that variable 'result' contains the value we
 * want to return.
**/
Return result
:exmp.
:p.
If no return value is needed for the hook you're writing, return 0 (zero) on
success and -1 on failure.
:xmp.
/**
 * We're failed, failed, failed &colon.(
**/
Return -1
:exmp.

:edl.

:h1 res=5.Additional REXX operators
.im Operators.ipf

.* :h1 res=6.Reporting warnings, errors and total failure
.* :p.
.*  Three additional operators available for REXX hook writers to report 
.* warnings, non-fatal and fatal errors: 'warning', 'error' and 'fatal'.
.* All of them are differrent from 'log' operator, because they're not only
.* writing corresponding information to log file (unconditionally, btw),
.* but display it onscreen too.

:h1 res=7.Sample (NULL) script (Rexx.U2F.Address)
:xmp.
/*********************************************************************/
/* address conversion hook for U1 UU2Fido.exe                        */
/*                                                                   */
/* Input:  uucp address of recepient                                 */
/*                                                                   */
/* Output: replacement address                                       */
/*                                                                   */
/*********************************************************************/

/* Here's we are loading variable 'inaddr' with value of the first argument */
Parse Arg inaddr

/* initialize output to default */
OutString = inaddr           

/* Put debug info to log file */
log 'Rexx.U2F.Address: InAddr is 'inaddr


/* return string, removing any blanks on the front or back.   */
Return Strip(OutString)                

/* end */
:exmp.



:h1 res=8.Detailed hooks descriptions
.im hooklist.ipf


:euserdoc.
