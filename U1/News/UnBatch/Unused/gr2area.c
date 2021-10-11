/************************ UUCP to FIDO gate ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :       Newsgroup to area name
 *
 *      $Log: gr2area.c $
 *      Revision 1.3  1995/08/02 13:41:14  dz
 *      group[50] --> group[500] :-)
 *      kludge, of course. string must be used.
 *
 * Revision 1.2  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 * Revision 1.1  1995/03/11  18:32:26  dz
 * Initial revision
 *
 *
 *
 *
\*/

#include        "unbatch.h"
#include        "..\..\version.h"

#include		<new_usr.h>
#include		<elist.h>
#include		<strng.h>


	/*************************************************************
					Extract username from address
	*************************************************************/

bool
group2area( echo_def &edef, const char *grlist, const char *from )
	{
	char	group[500], *p;
	char	*gbuf;

	group[0] = '\0';

	gbuf = strdup( grlist );
	if( gbuf == NULL )
		{
		error( EI_None, "Out of memory" );
		return Err;
		}

	char	*gl = gbuf;

	if( !strlen( gl ) )
		{
		error( EI_None, "No newsgroup name found");
		free( gbuf );
		return Err;
		}

	while( *gl )
		{
		bool	breakout = No;

		while( *gl && (*gl == ' ' || *gl == '\t'))
			gl++;

		p = strchr( gl, ',' );
		if( p )
			{
			*p = '\0';
			strcpy( group, gl );
			gl = p+1;
			}
		else
			{
			strcpy( group, gl );
			breakout = Yes;
			}

		p = strpbrk( group, "\t " );
		if( p )
			*p = '\0';

		if( edef.find_by_group( group ) == Ok )
			{
			debug("Area : %s <- %s", (const char *)edef.area().c_str(), (const char *)edef.group().c_str() );

			// Store newsgroup name/ user address into the
			// special file so SendRules program will find
			// it, check if it is the first letter to this
			// group from that user, and send him rules of
			// this group.

			rules_sender::add_address( group, from );
			free( gbuf );
			return Ok;
			}

		if( breakout )
			break;
		} /* while */

// NB!
// MSG_??? must be suported too!

	if( conf.news_bad_group_msg == MSG_Pass )
		{
		log( "n", "Passing group name: %s", group );
		edef.set_passthrough( group );
		free( gbuf );
		return Ok;
		}

	error( EI_None, "Unknown groups: %s", gl );
	free( gbuf );
	return Err;
	}



