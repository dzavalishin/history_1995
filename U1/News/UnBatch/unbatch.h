/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Header
 *
 *      $Log: unbatch.h $
 *      Revision 1.5  1995/08/07 14:55:01  dz
 *      Using class App
 *
 *      Revision 1.4  1995/08/02 13:47:49  dz
 *      get_packet_file: fd -> FILE *
 *
 *      Revision 1.3  1995/04/09  18:25:15  dz
 *      IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 *      Revision 1.3  1995/04/09  18:25:15  dz
 *      IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 *      Revision 1.2  1995/04/09  10:45:11  dz
 *      rewriting for C Set
 *
 *      Revision 1.1  1995/03/11  18:32:26  dz
 *      Initial revision
 *
 *      
 *         Rev 1.3   10 Mar 1993 15:43:56   dz
 *      Crossposting support.
 *      
 *         Rev 1.2   05 Mar 1993 03:49:22   dz
 *      Direct unbatching
 *      
 *         Rev 1.1.1.0   03 Mar 1993 20:38:44   dz
 *      Temp: trying to implement direct unbatching
 *      
 *         Rev 1.1   17 May 1992 11:06:48   dz
 *      Internal packetizer, kiae-specific unbatcher
 *      
 *         Rev 1.0   21 Feb 1992 11:22:38   dz
 *      Initial revision.
 *      
 *
\*/

#include	<app.h>
#include	<ftn_msg.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<ctype.h>
#include	<errno.h>
#include	<time.h>

#include	"setup.h"
#include	"log.h"
#include        <elist.h>

#include	"uuparse.h"
#include	"vread.h"

class VReadBase;

//extern bool			eat_crlf;			// Treat CRLF as 1 byte


class UnbatchApp : public U1App
	{
	bool	process( void );
        bool    verbose;

public:
	int	Run();

protected:

	void	scan( const char *dir );
	bool	process_all( const char *batch_dir );

	bool	z_ext( const char *name );

	bool	decompress( const char *dir, const char* fn );

	bool	unbatch( const char *dir, const char *fn );
	bool	do_unbatch( const char *bn, VReadBFile & in );

	bool	process_art( VReadBase & in, uuinfo ui );			// From anywhere
	bool	get_art( VReadBase & in, long len );				// From batch
	bool	send_unknown( VReadBase & in, uuinfo &ui, echo_def &edef, const char *gl, const char *group );
	bool	scan_groups( VReadBase & in, uuinfo ui );

	void	maketemp( const char *outdir, const char *ext, int *o_fd );

	bool	getheader( char *buf, int len, VReadBase &in );

	bool    change_ext( const char *fname, const char *new_ext );

//	bool	do_send_article( int fd );
	bool	do_send_article( VReadBFile & in, const char *fn );

	bool    save_to_file( VReadBase &in, uuinfo &, echo_def &edef, bool );



	bool	packetize_news( VReadBase &in, uuinfo &ui, echo_def &edef,
		bool retain_groupname );
	bool	do_packetize_news( VReadBase &in, uuinfo &ui, echo_def &edef, bool retain_groupname );
	bool	copy_data( VReadBase &in, FILE *fp, uuinfo &ui, echo_def &edef, bool retain_groupname, int part_no );



	bool	unbatch_kiae( const char *fn, bool recode_back );
	bool	run_uunpack( const char *fn );

	FILE *	get_packet_file( void );
// 	bool	group2area( echo_def &edef, const char *gl, const char *from );

	bool	art_to_spool( VReadBase & in, uuinfo &ui );

	fido_user	FTN_From( const uuinfo &ui, bool &fsc35_mode );


	};

