/************************ Gate remote control ***************************\
 *
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :	Send statistics
 *
 *      $Log: E_Stat.c $
 *      Revision 1.2  1995/08/02 13:52:37  dz
 *      tail available (command string)
 *
 *  Revision 1.1  1995/07/01  21:19:55  dz
 *  Initial revision
 *
 *      
 *         Rev 1.1   28 Nov 1992 23:41:48   dz
 *      up to date
 *      
 *         Rev 1.0   04 Feb 1992 00:21:58   dz
 *      Initial revision.
 *
 *
\*/

#include	"gremote.h"
#include	"../../version.h"
#include	"cmds.h"


//#include	<dir.h>
#include	<time.h>
#include	<errno.h>

#include	<stat.h>


#if 0

void
print_count( char *out, char *hl, StatCount &sc ) 
	{
	sprintf( out, "%s%ld bytes, %ld letters, %ld files.\n",
		hl,
		sc.Bytes,  sc.Letters, sc.Files
		);

	}


static void
doit( char *fname, fido_msg &reply, bool all ) {
	FILE	*fp;
	char	buf[200];
/*
	fp = fopen( fname, "rb" );
	if( fp == NULL ) {
		reply.puts("Can't open statistics file!" );
		return;
		}

	if( all )
		sprintf( buf,
			"\r\nStatistics, gathered for all users in %s :\r\n",
			fname
			);
	else
		sprintf( buf,
			"\r\nStatistics, gathered for your address in %s :\r\n",
			fname
			);
	reply.puts( buf );

	FidoUserEntry	s;
	fido_user		to;

	reply.get_to( to );

	rewind( fp );
	while( fread( &s, sizeof(s), 1, fp ) == 1 ) {

		if( (!all) && (
			s.Address.net()	    !=	to.net()		||
			s.Address.node()	!=	to.node()		||
			s.Address.point()	!=	to.point()
			)
			) continue;

		sprintf( buf, "%s, %d:%d/%d.%d\n",
			s.Address.name(),
			s.Address.zone(),				s.Address.net(),
			s.Address.node(),				s.Address.point()
//			s.has_domain() ? "@" : "",	s.domain()
			);
		reply.puts( buf );

		print_count( buf, "To FIDO: ", s.ToFido );
		reply.puts( buf );
		print_count( buf, "To UUCP: ", s.ToUu );
		reply.puts( buf );

		sprintf( buf, "First  : %s",	ctime( &s.first_used ));
		reply.puts( buf );

		sprintf( buf, "Last   : %s",	ctime( &s.last_used ));
		reply.puts( buf );

		sprintf( buf, "Period : %ld days\r\n\r\n\r\n",
			( s.last_used-s.first_used ) / (24L*60L*60L) );
		reply.puts( buf );
		}

	fclose( fp );
*/
	}



/****************************************************************************
						Entry point
****************************************************************************/


//#pragma argsused

bool
exec_stat( const string &tail, fido_msg &reply, fido_msg &orig ) 
	{
	char	fn[120];
	bool	all = No;

	if( !stricmp( strtok( NULL, " \t" ), "all" ) )
		all = Yes;

	sprintf( fn, "%s\\fidointl.stt", (const char *)conf.stat_dir.c_str() );
	doit( fn, reply, all );


	return Ok;
	}



#endif



