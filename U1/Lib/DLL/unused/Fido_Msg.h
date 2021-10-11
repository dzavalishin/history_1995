/************************ FIDO MSG file format ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:		File format structure
 *
 *      $Log: Fido_Msg.h $
 *      Revision 1.3  1995/11/05 13:52:48  dz
 *      current.
 *
 * Revision 1.2  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 *      
 *         Rev 1.7   09 Apr 1993 10:04:36   dz
 *      Extended flags support
 *      
 *         Rev 1.6   29 Mar 1993 04:17:18   dz
 *      Trying to suport FLAGS kludge
 *      
 *         Rev 1.5   25 Feb 1993 18:31:32   dz
 *      Rewritten using fido_addr
 *      
 *         Rev 1.4   06 Nov 1992 19:20:08   dz
 *      Ver 1.12a save
 *      
 *         Rev 1.3   30 Mar 1992 21:43:08   dz
 *      FIDO structures & fido_addr class definition
 *      moved to separate header files.
 *      FSC0035 support written.
 *      
 *         Rev 1.2   04 Feb 1992 22:14:10   dz
 *      const modifers added
 *      
 *         Rev 1.1   11 Oct 1991 05:10:16   dz
 *      New methods - getl() and add_xflag()
 *
 *         Rev 1.0   12 Sep 1991 01:28:30   dz
 *      Initial revision.
 *
 *	Use 4 spaces/tab to see it properly
\*/

#ifndef __cplusplus
#error You Need C++ to compile this
// Or delete class definition & use structures only :-)
#endif


#ifndef	FIDO_MSG_H
#define	FIDO_MSG_H

#include	<style.h>
#include	<stdio.h>
#include	<string.h>

#include	<uu2io.h>

#include	"fmsg_str.h"
#include	"fidoaddr.h"


#define		MSG_FN_LEN		150
#define		FSC35RA			100

typedef ulong fmsg_attr;


const ulong		FF_ArchiveSent			   = 0x00010000ul;
const ulong		FF_Direct                  = 0x00020000ul;
const ulong		FF_Immediate               = 0x00040000ul;
const ulong		FF_ZoneGate                = 0x00080000ul;
const ulong		FF_HubHostRoute            = 0x00100000ul;
const ulong		FF_Xmail                   = 0x00200000ul;
const ulong		FF_KillFileSent            = 0x00400000ul;
const ulong		FF_TruncateFileSent        = 0x00800000ul;
const ulong		FF_Lock                    = 0x01000000ul;
const ulong		FF_ConfirmReceiptRequest   = 0x02000000ul;
const ulong		FF_FAX_HiRes               = 0x04000000ul;
const ulong		FF_FAX_CoverLetter         = 0x08000000ul;
const ulong		FF_FAX_Signature           = 0x10000000ul;
const ulong		FF_FAX_LetterHead          = 0x20000000ul;

#define FLAGS_KLUDGE_BITS	\
		(								\
		FF_Private					|   \
		FF_Crash					|   \
		FF_Recd						|   \
		FF_Sent						|   \
		FF_FileAttached				|   \
		FF_KillSent					|   \
		FF_HoldForPickup			|   \
		FF_FileRequest				|   \
		FF_ReturnReceiptRequest		|   \
		FF_ArchiveSent				|   \
		FF_Direct					|   \
		FF_Immediate				|   \
		FF_ZoneGate					|   \
		FF_HubHostRoute				|   \
		FF_Xmail					|   \
		FF_KillFileSent				|   \
		FF_TruncateFileSent			|   \
		FF_Lock						|   \
		FF_ConfirmReceiptRequest	|   \
										\
		FF_FAX_HiRes				|   \
		FF_FAX_CoverLetter			|   \
		FF_FAX_Signature			|   \
		FF_FAX_LetterHead			    \
		)

#pragma pack(1)


struct hlc				// For internal use only
	{
	char	*fido;
	bool	fmpt;
	bool	topt;
	bool	intl;
	bool	flags;
	bool	reply;
	bool	pid;
	bool	replyaddr;
	bool	replyto;
	bool	domain;
	};


class _Export	fido_msg
	{
private:
	char				fname[MSG_FN_LEN];		// File name of msg

	FILE				*fp;
	fido_msg_hdr		hdr;					// Message header


	bool				error;					// Instance defunct.
	bool				hflush;					// Header must be flushed
	bool				rewrite;				// Message must be rebuilt

	fido_user			from;
	fido_user			to;

	char				kl_reply[40];
	char				kl_pid[80];
//	char				kl_flags[80];

	fmsg_attr			flags;

	char				fsc35_reply_addr[FSC35RA];	// Say, "dz@isoft.msk.su"
	fido_user			fsc35_reply_to;				// Say, "uucp, 2:5020/23"

        const char                      *via_addr;
        bool                            put_via();

private:
	bool		do_attach( void );
	bool		rewrite_msg( void );
	void		load_kludges( void );
	void		get_kludge( const char *buf, hlc &state );

	void		reset_hdr_addr( void );				// Fill reserved bytes

	bool		print_flags( char *buf, fmsg_attr f );
	fmsg_attr	scan_flags( const char *buf );


public:
	fido_msg( void );							// Constructor
	~fido_msg( void );							// D'tor



	bool	new_msg( const char *dir );			// Create new one

	bool	attach( const char *dir, int num );	// Attach to file
	bool	attach( const char *file );		// Attach to file

	bool	detach( void );						// Detach from file (deletes
												// file if KillSent && Sent)



	bool	err( void ) { return error; }		// Returns Yes in case of error
	void	flush( void ) { hflush = Yes; }		// Force header flush-out

	long	get_date( void );					// Get msg date in Unix format
	void	set_date( long );					// Set msg date in Unix format
	void	set_date( char * );					// Set msg date in text format
/*
	void	add_xflag( const char *k ) {
		strcat( kl_flags, " " );
		strcat( kl_flags, k );
		}
*/
// Text management methods	- return Err/Ok

	bool	clear_txt( void );					// Clear message text
	void	clear_hdr( void );					// Clear message header

	bool	rewind( void );						// Reset file ptr to msg start

	bool	puts( const char *s );				// Add string to msg
	bool	gets( char *s );					// Get string from msg

	bool	getl( char *s );					// Get line ( < 80 bytes )

	bool	puta( const char *s );				// Add attribute string
	bool	geta( char *s );					// Get attribute string

// Message header access

	// Get/set source address
	void	get_from( fido_user &f )			{ f = from; }
	void	set_from( const fido_user &f )		{ from = f;	rewrite = Yes; }

	// Get/set dest. address
	void	get_to	( fido_user &t )			{ t = to; }
	void	set_to	( const fido_user &t )		{ to = t;	rewrite = Yes; }

	void	get_attr( fmsg_attr &a )			{ a = flags; }
	void	set_attr( const fmsg_attr a )		{ flags = a; flush(); }
	void	add_attr( const fmsg_attr a )		{ flags |= a; flush(); }
	void	del_attr( const fmsg_attr a )		{ flags &= ~a; flush(); }

	fmsg_attr	attr()							{ return flags; }
	void		attr( const fmsg_attr a )		{ flags = a; flush(); }

	// Get/set subject line
	void	get_subj( char *s )				{ strncpy( s, hdr.subject, 72 ); }
	void	set_subj( const char *s )			{ strncpy( hdr.subject, s, 72 ); hdr.subject[71] = '\0'; flush(); }

	void	get_fsc35_reply_addr( char *buf )			{	strcpy( buf, fsc35_reply_addr );	}
	void	set_fsc35_reply_addr( const char *buf )	{	strncpy( fsc35_reply_addr, buf, FSC35RA ); fsc35_reply_addr[FSC35RA-1] = '\0';	}

	void	get_fsc35_reply_to( fido_user &fa )			{	fa = fsc35_reply_to;	}
	void	set_fsc35_reply_to( const fido_user &fa )	{	fsc35_reply_to = fa;	}

        void    set_via_addr( const char *a ) { via_addr = a; }

// Debug methods

	void	print_hdr( void );
	void	print_msg( void );
	};

#pragma pack()


//	Misc.

int _Export	find_last_msg( const char *dir );



#endif //	FIDO_MSG_H
