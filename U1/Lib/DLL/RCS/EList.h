head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	95.11.05.13.52.48;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.13.11.06.28;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.04.17;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@current.
@
text
@/************************ UU2 EchoList interface ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	EchoList fil interface
 *
 *      $Log: EList.h $
 * Revision 1.2  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *      
 *         Rev 1.0   07 Jun 1993 17:57:06   dz
 *      Initial revision.
 *
 *
 *
\*/


#ifndef	ELIST_H
#define	ELIST_H

#include "strng.h"

#pragma pack(1)


class _Export echo_def
	{
private:
	String			newsgroup_v;				// UseNet newsgroup name
	String			echo_v;						// FIDO echo name

	bool            in_file_v;                	// Each incoming form uucp message goes to file
	String          in_file_dir_v;   			// Where files will be saved

	String			rules_file_v;				// Echo rules file
	bool			send_rules_v;				// Send rules, or not
	bool			fido_rules_v;				// Send rules to fido side too

	bool			file_echo_v;				// Is it an file-echo?
	String			file_hold_dir_v;			// Where to seek for TIC files

	String			fido_premoderator_v;		// Address of...
	String			uucp_premoderator_v;		// Address of...

	String			approved_v;					// Add 'Approved' headl.
    String          distrib_v;                  // Add 'Distribution' headl.

	bool			pass_to_fido_v;				// Pass mail uucp->fido
	bool			pass_to_uucp_v;				// Pass mail fido->uucp
	bool			pass_fu_reg_only_v;			// Pass fido->uucp from registered ones only

	bool			xpost_guard_v;				// Refuse to gate message to THIS echo if it has more than one newsgroup

	bool			parse_param( char *def );	// Destroys value of def!

	void			clear( void );				// It does not clear!

protected:
	inline int		size( void );


public:
					echo_def( void );
					~echo_def( void );

	bool			find_by_area( const char *area );
	bool			find_by_group( const char *group );

	void			set_passthrough( const char *group_or_area );

	// Value extractors
	const String&	area ( void ) const	      { return echo_v; }
	const String&	group( void ) const	      { return newsgroup_v; }

	bool			in_file( void ) const     { return in_file_v; }
	const String&	in_file_dir( void ) const { return in_file_dir_v; }

	const String&	rules_file( void ) const  { return rules_file_v; }
	bool			send_rules( void ) const  { return send_rules_v; }
	bool			fido_rules( void ) const  { return fido_rules_v; }

	bool			file_echo( void ) const   { return file_echo_v; }
	const String&	file_hold_dir( void ) const { return file_hold_dir_v; }

	const String&	fido_premod( void ) const { return fido_premoderator_v; }
	const String&	uucp_premod( void ) const { return uucp_premoderator_v; }

	const String&	approved( void ) const    { return approved_v; }
    const String&   distrib( void ) const     { return distrib_v; }

	bool			pass_to_fido( void )	  { return pass_to_fido_v; }
	bool			pass_to_uucp( void )	  { return pass_to_uucp_v; }
    bool            pass_fu_reg_only( void)   { return pass_fu_reg_only_v; }

	bool			xpost_guard( void ) { return xpost_guard_v; }

	// Comparison
	inline bool		group_is( const char *a ) const;
	inline bool		area_is ( const char *a ) const;

	// Assignment
	echo_def&		operator = ( echo_def &r );

	// Config. parser
	bool			parse( const char *def );
	};

inline bool echo_def::
group_is( const char *a ) const
	{
	return stricmp( a, (const char *) newsgroup_v.c_str() ) == 0 ? Yes : No;

// No!	return a == newsgroup_v ? Yes : No;
	}

inline bool echo_def::
area_is( const char *a )  const
	{
	return stricmp( a, (const char *) echo_v.c_str() )  == 0 ? Yes : No;

// No!	return a == echo_v ? Yes : No;
	}

inline int	echo_def::
size( void )
	{
	int s = 0;

//	if( newsgroup_v != NULL )	s += strlen( newsgroup_v );
//	if( echo_v != NULL )		s += strlen( echo_v );
//	if( in_file_dir_v != NULL)	s += strlen( in_file_dir_v );

#if 0
	if( newsgroup_v != NULL )	s += newsgroup_v.length();
	if( echo_v != NULL )		s += echo_v.length();
	if( in_file_dir_v != NULL)	s += in_file_dir_v.length();
#endif

	return s;
	}

#pragma pack()

#endif

@


1.2
log
@debug
@
text
@d8 3
d60 2
d102 2
@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/uu2/vcs/elist.h_v  $
d31 1
a31 1
class echo_def
d134 1
d138 1
@
