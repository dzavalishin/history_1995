/*********************************************************************/
/* Command hook for U1 GRemote.exe                                   */
/*                                                                   */
/*                                                                   */
/* Input:  command, parameters                                       */
/*                                                                   */
/* Output: 0 if successfull, nonzero otherwise.                      */
/*                                                                   */
/*********************************************************************/
/*

 there's two subcommands exist in "gremote" command space
 (use address "gremote" to access):

	GetLetterLine <var_name> - reads next line of user's message 
                                   to variable <var_name>.

	PutLetterLine "text" - write line to the reply letter

*/

cmd=arg(1)
par=arg(2)

call ans "Rexx subsystem online... processing command "cmd' with parameters 'par
address "U1" log "Rexx processing command "cmd' with parameters 'par

/*
address "gremote" "GetLetterLine" "line"
say 'next line: 'line
*/

return 0



ans: procedure
address "gremote" "PutLetterLine" arg(1)
return

