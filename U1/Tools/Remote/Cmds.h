/************************ Gate remote control ***************************\
 *
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :       Commnds
 *
 *      $Log: Cmds.h $
 *      Revision 1.3  1995/11/06 14:28:09  dz
 *      Current
 *
 *      Revision 1.2  1995/08/02 14:02:52  dz
 *      tails added
 *
 *  Revision 1.1  1995/07/01  21:19:55  dz
 *  Initial revision
 *
 *
 *
\*/


#ifndef	CMDS
#define	CMDS




#ifndef FIDO_MSG_H
#include	<fido_msg.h>
#endif


struct cmd_tab 
	{
	char	*cmd;
	bool	(*func)( const string &tail, FTN_Msg &reply, FTN_Msg &orig );
	bool	privil;
	char	*help;
	};

extern cmd_tab		cmd[];
extern bool			helped_once;
extern bool			secure;



bool		exec_help		( const string &tail, FTN_Msg &reply, FTN_Msg &orig );
bool		exec_hello		( const string &tail, FTN_Msg &reply, FTN_Msg &orig );
bool		exec_users		( const string &tail, FTN_Msg &reply, FTN_Msg &orig );
bool		exec_echoes		( const string &tail, FTN_Msg &reply, FTN_Msg &orig );
bool		exec_notify		( const string &tail, FTN_Msg &reply, FTN_Msg &orig );
bool		exec_stat		( const string &tail, FTN_Msg &reply, FTN_Msg &orig );
bool		exec_broadcast		( const string &tail, FTN_Msg &reply, FTN_Msg &orig );
bool		exec_rexx		( const string &verb, const string &tail, FTN_Msg &reply, FTN_Msg &orig );



#endif //	CMDS