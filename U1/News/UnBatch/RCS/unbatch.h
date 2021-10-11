head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	95.08.07.14.55.01;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.08.02.13.47.49;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.04.09.18.25.15;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.09.10.45.11;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.32.26;	author dz;	state Exp;
branches;
next	;


desc
@UU2 News Unbatcher
@


1.5
log
@Using class App
@
text
@/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Header
 *
 *      $Log: unbatch.h $
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
#include	<fido_msg.h>
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

@


1.4
log
@get_packet_file: fd -> FILE *
@
text
@d8 2
a9 2
 * Revision 1.3  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
d11 2
a12 2
 * Revision 1.3  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
d14 2
a15 2
 * Revision 1.2  1995/04/09  10:45:11  dz
 * rewriting for C Set
d17 5
a21 2
 * Revision 1.1  1995/03/11  18:32:26  dz
 * Initial revision
d42 1
a42 1
#include	<style.h>
d52 1
a52 1
#include    <elist.h>
d55 1
a55 1
//#include	"vread.h"
d59 17
a75 3
/****************************************************************************
						Global variables
****************************************************************************/
d77 1
a77 1
extern bool			eat_crlf;			// Treat CRLF as 1 byte
d79 2
a80 3
/****************************************************************************
							Functions
****************************************************************************/
d82 4
d87 1
a87 6
void	scan( const char *dir );
bool	z_ext( const char *name );
bool	decompress( const char *dir, const char* fn );
bool	unbatch( const char *dir, const char *fn );
bool	process_art( VReadBase & in, uuinfo ui );			// From anywhere
bool	get_art( VReadBase & in, long len );				// From batch
d89 1
a89 3
void	maketemp( const char *outdir, const char *ext, int *o_fd );
bool	getheader( char *buf, int len, VReadBase &in );
bool    change_ext( const char *fname, const char *new_ext );
d91 1
a91 1
bool	do_send_article( int fd );
d93 8
a100 1
bool	packetize_news( VReadBase &in, uuinfo &ui, echo_def &edef,
d102 10
d113 1
a113 1
bool	unbatch_kiae( const char *fn, bool recode_back );
d115 1
a115 2
FILE *		get_packet_file( void );
// bool	group2area( echo_def &edef, const char *gl, const char *from );
a116 1
bool	art_to_spool( VReadBase & in, uuinfo &ui );
d118 1
a118 1
fido_user	FTN_From( const uuinfo &ui, bool &fsc35_mode );
@


1.3
log
@IBM C Set version. Phew, it was not a piece of cake to get here...
@
text
@d8 6
d85 1
a85 1
int		get_packet_file( void );
@


1.2
log
@rewriting for C Set
@
text
@d8 3
d80 1
a80 1
//bool	group2area( echo_def &edef, const char *gl, const char *from );
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/news/unbatch/vcs/unbatch.h_v  $
d59 1
@
