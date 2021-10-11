head	1.2;
access;
symbols;
locks;
comment	@ * @;


1.2
date	95.04.13.11.06.28;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.00.54;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@debug
@
text
@/************************ UU2 FILE2UU ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	.TIC file handler header
 *
 *      $Log: Tic.h $
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 *      
 *         Rev 1.1   07 Jun 1993 17:41:36   dz
 *      Appropriate consts were added, new methods written
 *      
 *         Rev 1.0   19 Jan 1993 20:57:52   dz
 *      Initial revision.
 *
 *
\*/

#ifndef	TIC_H
#define	TIC_H

#include    "style.h"
#include    "fidoaddr.h"
#include    "tic_kw.h"

//#include    "dbllist.h"
#include    "dlistimp.h"
#include    "strng.h"
#include    "time.h"
#include    "stdio.h"

#define TIC_MAXAREA 20
#define TIC_MAXFN   14
#define TIC_MAXPW   8

#pragma pack(1)

class _Export tic_file
	{
	bool		err;				// Contents invalid

	// Transport info
	fido_addr   from_v;                 // From:
	fido_addr	origin_v;				// Origin:
	fido_addr	receipt_v;				// RtRcpt: (default - to origin)
	bool		send_receipt_v;
	DoubleList	path_v;					// Path:
	DoubleList	seenby_v;				// Seenby:


	// Authentification info
	time_t		date_v;					// Date:
	long		size_v;					// Size:
	long		crc_v;					// Crc:
	char		passwd_v[TIC_MAXPW+1];	// Pw:

	// File info
	fido_addr	source_v;				// Source: (author address)
	char		name_v[TIC_MAXFN+1];	// File:
	DoubleList	replaces_v;				// Replaces: (files to delete)
	char		area_v[TIC_MAXAREA+1];	// Area:
	time_t		release_v;				// Release:
	DoubleList	desc_v;					// Desc:

	bool		fill_in( tic_kw::tic_token, const char *val );

public:
			tic_file();
			tic_file( const char *load_from );
			~tic_file();

	bool	invalid()	{ return err; }

	void	clear( void );
	bool	load_from( const char *fn );

	bool	save_to( const char *fn );
	bool	save_to( FILE *fp );

    // ---------------- Get values ----------------

    // One-by-one
    const fido_addr &   from( void ) const      { return from_v;        }
    const fido_addr &   origin( void ) const    { return origin_v;      }
    const fido_addr &   receipt( void ) const   { return receipt_v;     }
    bool                send_receipt( void ) const    { return send_receipt_v;}
    const DoubleList&   path( void ) const      { return path_v;        }
    const DoubleList&   seenby( void ) const    { return seenby_v;      }


	// Authentification info
    time_t              date( void ) const      { return date_v;        }
    long                size( void ) const      { return size_v;        }
    long                crc( void ) const       { return crc_v;         }
    const char      *   passwd( void ) const    { return passwd_v;      }

	// File info
    const fido_addr &   source( void ) const    { return source_v;      }
    const char      *   name( void ) const      { return name_v;        }
    const DoubleList&   replaces( void ) const  { return replaces_v;    }
    const char      *   area( void ) const      { return area_v;        }
    time_t              release( void ) const   { return release_v;     }
    const DoubleList&   desc( void ) const      { return desc_v;        }

    // ---------------- Set values ----------------

    // One-by-one
    void   from( const fido_addr &v )           { from_v = v;           }
	void   origin( const fido_addr &o )         { origin_v = o;      }
	void   receipt( const fido_addr &r )        { receipt_v = r; send_receipt_v = Yes;   }
    void   add_to_path( const String& s )       { path_v.addAtTail( (Object &)*s );        }
    void   add_to_seenby( const String& s )     { seenby_v.addAtTail( (Object &)*s );      }


	// Authentification info
    void   date( time_t t )                     { date_v = t;        }
    void   size( long s )                       { size_v = s;        }
    void   crc( long c )                        { crc_v = c;         }
    void   passwd( const char *p )              { strncpy( passwd_v, p, TIC_MAXPW );      }

	// File info
    void   source( const fido_addr &s )         { source_v = s;      }
    void   name( const char *n )                { strncpy( name_v, n, TIC_MAXFN );        }
    void   add_to_replaces( const String &r )   { replaces_v.addAtTail( (Object &)*r );   }
    void   area( const char * a )               { strncpy( area_v, a, TIC_MAXAREA );        }
    void   release( time_t t )                  { release_v = t;     }
    void   desc( const String &d )              { desc_v.addAtTail( (Object &)*d );       }

	};

#pragma pack()


#endif


@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/fido/vcs/tic.h_v  $
d43 1
a43 1
class tic_file
@
