:h2 
    x=left y=top width=25% height=100%
    scroll=none group=1 clear
    res=83.Rexx.Gremote.Cmd&colon. U1 Remote Control Command Execution Hook
:link reftype=hd res=831
      vpx=25% vpy=top vpcx=75% vpcy=100%
      group=2 auto dependent.
:p.
:link reftype=hd res=831
      vpx=25% vpy=top vpcx=75% vpcy=100%
      group=2 dependent.Description:elink.
:p.
:link reftype=hd res=832
      vpx=25% vpy=top vpcx=75% vpcy=100%
      group=3 dependent.Example:elink.


:h3 res=831 x=25% y=top width=75% height=100%.Description of Rexx.Gremote.Cmd

:dl break=all.
:dt.:hp6.Purpose:ehp6.
:dd.This hook can be used to extend command set of gremote.exe remote
control unit.

:dt.:hp6.Call reason:ehp6.
:dd.During parsing the letter from user GRemote checks command verb
against internal table first, then, if no match found, this hook is called.

:dt.:hp6.Parameters:ehp6.
:dd.The first parameter is command verb,
second is tail of the command line.

:dt.:hp6.Return value:ehp6.
:dd.This hook must return 0 if command verb is correct, nonzero otherwise.

:edl.

:h3 res=832 x=25% y=top width=75% height=100%.
Sample script for Rexx.Gremote.Cmd
:xmp.

/*****************************************************/
/* Command hook for U1 GRemote.exe                   */
/*                                                   */
/*                                                   */
/* Input:  command, parameters                       */
/*                                                   */
/* Output: 0 if successfull, nonzero otherwise.      */
/*                                                   */
/*****************************************************/

/* Get command and parameters */
cmd=arg(1)
par=arg(2)

/* GRemote will send user a reply letter. */
/* The following operator adds            */
/* a line of text to that letter          */
call ans "Rexx subsystem online..."
call ans "Processing command "cmd" with parameters "par

/* Log what do we do                      */
address "U1" log "Rexx processing command "cmd' with parameters 'par

/* Read the following line of user's      */
/* letter to the variable 'line'          */
address "gremote" "GetLetterLine" "line"

/* print onscreen what do we got          */
say 'command is 'cmd', parameters: 'par
say 'next line of letter is: 'line

return 0



ans: procedure
address "gremote" "PutLetterLine" arg(1)
return


:exmp.
