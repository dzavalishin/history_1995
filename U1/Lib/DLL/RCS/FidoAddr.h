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
@/************************ FIDO interface library ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	FIDO address class definition
 *
 *      $Log: FidoAddr.h $
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 *      
 *         Rev 1.0   25 Feb 1993 18:28:18   dz
 *      (In fact) OS2 port - uints replaced with ushort
 *      
 *         Rev 1.1   06 Nov 1992 19:20:10   dz
 *      Ver 1.12a save
 *
 *         Rev 1.0   30 Mar 1992 21:39:46   dz
 *      Initial revision.
 *
 *
\*/

#ifndef	FIDOADDR_H
#define	FIDOADDR_H

#include	<style.h>
#include	<string.h>
#include	<strng.h>

#pragma pack(1)

	/*************************************************************
							 Definition
	*************************************************************/

class	uucp_addr;

class _Export	fido_addr
	{

private:
	char    domain_v[38];
	ushort	zone_v;
	ushort	net_v;
	ushort	node_v;
	ushort	point_v;

protected:

    static ushort   def_zone_v;
    static String   def_domain_v;

    bool    parse_znnpd( const char *&ap );
    void    fix_z()    { if( zone_v == 0 ) zone_v = def_zone_v; }

public:
    ushort  zone()      const   { return zone_v; }
    ushort  net()       const   { return net_v; }
    ushort  node()      const   { return node_v; }
    ushort  point()     const   { return point_v; }

    void    zone( ushort i )    { zone_v = i; fix_z(); }
	void	net( ushort i )		{ net_v = i; }
	void	node( ushort i )	{ node_v = i; }
	void	point( ushort i )	{ point_v = i; }



    static ushort def_zone()                     { return def_zone_v; }
    static void   def_zone( ushort z )           { def_zone_v = z; }

    static const  String &  def_domain()         { return def_domain_v; }
    static void   def_domain( const String &d )  { def_domain_v = d; }

    void    fix_domain();



    fido_addr( void );                                                              // Default constructor
	fido_addr( const fido_addr & );					// Copy constructor

	inline void		   	domain( const char *s );
    inline const char*  domain( void ) const;
	inline bool			has_domain( void ) const;

    bool                operator == ( const fido_addr &a) const;
	inline bool 		operator != ( const fido_addr &a ) const;
	fido_addr & 		operator =  ( const fido_addr &a );
//                      operator const char * () const;
                        operator String () const;

    bool                aparse( const char *&ap );      // Parse everything exept for username

	virtual inline bool	valid( void ) const;
	virtual inline void	clear( void );

	bool				convert_to_uucp( uucp_addr &ua, const char *domain ) const;

	};



// ---------------------------------------------------------------------
//
//  FIDO user class
//
// ---------------------------------------------------------------------


class _Export	fido_user : public fido_addr
	{

	char	name_v[38];

public:
	fido_user( void );								// Default constructor
	fido_user( const fido_user & );					// Copy constructor
        fido_user( const fido_addr &, const char *name );    // Full c'tor

	inline void		   	name( const char *s );
	inline const char*	name( void ) const;

	bool 				operator == ( const fido_user &a) const;
	inline bool			operator != ( const fido_user &a ) const;

	fido_user & 		operator =  ( const fido_user &a );
//						operator const char * () const;
						operator String () const;

	bool 				parse ( const char * );		// Parses name as a single word
	bool 				fparse( const char * );		// Parses name up to end of string
	bool 				cparse( const char * );		// Parses address in Golded CC: format (Name Addr)
	bool 				aparse( const char *&ap );	// Parse everything exept for username

	virtual inline bool	valid( void ) const;
	virtual inline void	clear( void );

	bool				convert_to_uucp( uucp_addr &ua, const char *domain ) const;

	// Processing

	void				normalize_name( void );

	};


	/*************************************************************
						   Inline methods
	*************************************************************/


inline void fido_addr::
domain( const char *s )
	{
	strncpy( domain_v, s, 38 );
	domain_v[37] = '\0';
	}

inline const char* fido_addr::
domain( void ) const
	{
	return domain_v;
	}

inline bool fido_addr::
has_domain( void ) const
	{
	return domain_v[0] == '\0' ? No : Yes;
	}

inline void fido_user::
name( const char *s )
	{
	strncpy( name_v, s, 38 );
	name_v[37] = '\0';
	}

inline const char* fido_user::
name( void ) const
	{
	return name_v;
	}

/*--------------------------------------------------------------------------*/

inline bool fido_addr::
operator != ( const fido_addr &a ) const
	{
	return (bool) ! ((int) ( *this == a ));
	}

inline bool fido_user::
operator != ( const fido_user &a ) const
	{
	return (bool) ! ((int) ( *this == a ));
	}

/*--------------------------------------------------------------------------*/

inline bool fido_addr::
aparse( const char *&ap )		// Parse everything exept for username
		{
		bool b = parse_znnpd( ap );
        fix_z();
		return b;
		}

/*--------------------------------------------------------------------------*/

inline bool fido_addr::
valid( void ) const
	{
	if( net_v != 0 )	return Yes;
	return No;
	}

inline bool fido_user::
valid( void ) const
	{
	if( fido_addr::valid() && strlen( name_v ) )	return Yes;
	return No;
	}

inline void fido_addr::
clear( void )
	{
	zone_v = net_v = node_v = point_v = 0;
	domain_v[0] = '\0';
	}

inline void fido_user::
clear( void )
	{
	fido_addr::clear();
	name_v[0] = '\0';
	}


/*--------------------------------------------------------------------------*/


inline void  fido_addr::
fix_domain()
    {
//    if( zone_v == 0 ) zone_v = def_zone_v;
    if( strlen( domain_v ) == 0 )
        {
//        strncpy( domain_v, def_domain_v, 37 );
        def_domain_v.copy( domain_v, 37 );
//        domain_v[36] = '\0';
        }
    }


#pragma pack()

#endif // FIDOADDR_H
@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/fido/vcs/fidoaddr.h_v  $
d42 1
a42 1
class	fido_addr
d114 1
a114 1
class	fido_user : public fido_addr
d261 1
a261 1
#endif FIDOADDR_H
@
