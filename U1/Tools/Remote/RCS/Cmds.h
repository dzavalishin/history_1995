head	1.3;
access;
symbols;
locks;
comment	@ *  @;


1.3
date	95.11.06.14.28.09;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.08.02.14.02.52;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.07.01.21.19.55;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@Current
@
text
@/************************ Gate remote control ***************************\
 *
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :       Commnds
 *
 *      $Log: Cmds.h $
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



#endif //	CMDS@


1.2
log
@tails added
@
text
@d8 3
d33 1
a33 1
	bool	(*func)( const string &tail, fido_msg &reply, fido_msg &orig );
d44 8
a51 8
bool		exec_help		( const string &tail, fido_msg &reply, fido_msg &orig );
bool		exec_hello		( const string &tail, fido_msg &reply, fido_msg &orig );
bool		exec_users		( const string &tail, fido_msg &reply, fido_msg &orig );
bool		exec_echoes		( const string &tail, fido_msg &reply, fido_msg &orig );
bool		exec_notify		( const string &tail, fido_msg &reply, fido_msg &orig );
bool		exec_stat		( const string &tail, fido_msg &reply, fido_msg &orig );
bool		exec_broadcast		( const string &tail, fido_msg &reply, fido_msg &orig );
bool		exec_rexx		( const string &verb, const string &tail, fido_msg &reply, fido_msg &orig );
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   C:/net/gate/fido2uu/vcs/fido2uu.h_v  $
d27 2
a28 1
struct cmd_tab {
d30 1
a30 1
	bool	(*func)( fido_msg &reply, fido_msg &orig );
d41 8
a48 7
bool		exec_help		( fido_msg &reply, fido_msg &orig );
bool		exec_hello		( fido_msg &reply, fido_msg &orig );
bool		exec_users		( fido_msg &reply, fido_msg &orig );
bool		exec_echoes		( fido_msg &reply, fido_msg &orig );
bool		exec_notify		( fido_msg &reply, fido_msg &orig );
bool		exec_stat		( fido_msg &reply, fido_msg &orig );
bool		exec_broadcast	( fido_msg &reply, fido_msg &orig );
@
