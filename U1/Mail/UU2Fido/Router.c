/************************ UUCP to FIDO gate ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	UUCP Router
 *
 *      $Log: Router.c $
 *      Revision 1.1  1995/03/07  00:20:29  dz
 *      Initial revision
 *
 *
 *         Rev 1.0   02 Dec 1993 00:25:50   dz
 *      Initial revision.
 *
 *
\*/

#include	"uu2fido.h"

//
// 1. Build list of address elements - AS IS, withno conversions
//
// 2. If we have no domain
//    a. Search in user table withno domain, if found - deliver to fido
//       as for registered user.
//    b. Fail!
//
// 3. If we have only one domain
//    a. Search in routing table. If found - deliver accordingly.
//    b. Search in user table with domain, if found - deliver to fido
//       as for registered user.
//    c. Search in FTN table, if found - deliver to fido
//       as for non-registered user.
//    d. Search in user table withno domain, if found - deliver to fido
//       as for registered user.
//    e. Fail!
//
// 4. If we have more, than one domain
//    a. Strip off everything equal to our default domain or
//       domains, defined in strip list.
//    b. If just one domain left, try 3a-3c.
//    c. Try mixing and matching parts to recover FTN-style address,
//       and, if
//       c1. got something, without zone: send it as if it matched
//           first FTN.
//       c2. it has zone: try to find corresponding FTN by zone.
//    d. Fail!
//
// How to fail:
// 1. If bounceing enabled, do that.
// 2. If PassUnknownBack enabled, re-route to domain given
//    in PassUnknownBack setup. (Simply saying, forward it)
//
// 3. Oh, no - send to postmaster!
// 4. No postmaster defined? Hang itself :(.

static bool    divide_to_pieces( DoubleList &pieces, String &user,
                                 const char *uu_dest );

static bool    route_no_domain( const char *orig_addr, String &uu_user );
static bool    route_one_domain( const char *orig_addr, String &domain, String &user );
static bool    route_many_domains( const char *orig_addr, DoubleList &pieces, String &user );

static bool    route_fail( const char *orig_addr, String &user );

// ----------------------------------------------------------------------

static bool    route_by_route_table  ( String &domain, String &user );
static bool    route_by_user_table   ( String &domain, String &user );
static bool    route_by_user_table   (                 String &user );
static bool    route_by_FTN_table    ( String &domain, String &user );
static bool    route_mix_n_match_FTN ( DoubleList &pieces, String &user );

static bool    route_strip_shit      ( DoubleList &pieces );

// ----------------------------------------------------------------------



bool
uu_route( const char *uu_dest )
    {
    DoubleList  pieces;
    int         npieces;
    String      user;

// 1. Build list of address elements - AS IS, withno conversions

    if( (npieces = divide_to_pieces( pieces, user, uu_dest )) < 0 )
        return Err;

// 2. If we have no domain

    if( npieces == 0 )
        return route_no_domain( uu_dest, user );

// 3. If we have only one domain

    if( npieces == 1 )
        return route_one_domain( uu_dest, ((String&)pieces.peekAtHead()), user );

// 4. If we have more, than one domain

    return route_many_domains( uu_dest, pieces, user );
    }


static bool
route_no_domain( const char *orig_addr, String &user )
    {
// 2. If we have no domain
//    a. Search in user table withno domain, if found - deliver to fido
//       as for registered user.

    if( route_by_user_table( user ) == Ok )
        return Ok;

//    b. Fail!
    return route_fail( orig_addr, user );
    }

static bool
route_one_domain( const char *orig_addr, String &domain, String &user )
    {
// 3. If we have only one domain
//    a. Search in routing table. If found - deliver accordingly.

    if( route_by_route_table( domain, user ) == Ok )
        return Ok;

//    b. Search in user table with domain, if found - deliver to fido
//       as for registered user.

    if( route_by_user_table( domain, user ) == Ok )
        return Ok;

//    c. Search in FTN table, if found - deliver to fido
//       as for non-registered user.

    if( route_by_FTN_table( domain, user ) == Ok )
        return Ok;

//    d. Search in user table withno domain, if found - deliver to fido
//       as for registered user.

    if( route_no_domain( orig_addr, user ) == Ok )
        return Ok;

//    e. Fail!
    return route_fail( orig_addr, user );
    }

static bool
route_many_domains( const char *orig_addr, DoubleList &pieces, String &user )
    {
// 4. If we have more, than one domain
//    a. Strip off everything equal to our default domain or
//       domains, defined in strip list.

    if( route_strip_shit( pieces ) == Err )
        return Err;

//    b. If just one domain left, try 3a-3c.

    if( pieces.getItemsInContainer() == 0 )
        return route_no_domain( orig_addr, user );

    if( pieces.getItemsInContainer() == 1 )
        return route_one_domain( orig_addr, ((String&)pieces.peekAtHead()), user );

//    c. Try mixing and matching parts to recover FTN-style address

    if( route_mix_n_match_FTN( pieces, user ) == Ok )
        return Ok;

//    d. Fail!
    return route_fail( orig_addr, user );
    }


// ----------------------------------------------------------------------


static bool
divide_to_pieces( DoubleList &pieces, String &user, const char *uu_dest )
    {
    return Err;
    }

static bool
route_strip_shit( DoubleList &pieces )
    {
    return Err;
    }

// ----------------------------------------------------------------------


static bool
route_by_route_table( String &domain, String &user )
    {
    return Err;
    }


static bool
route_by_user_table( String &domain, String &user )
    {
    return Err;
    }


static bool
route_by_user_table( String &user )
    {
    return Err;
    }


static bool
route_by_FTN_table( String &domain, String &user )
    {
    return Err;
    }


static bool
route_mix_n_match_FTN( DoubleList &pieces, String &user )
    {
    return Err;
    }


// ----------------------------------------------------------------------

static bool
route_fail( const char *orig_addr, String &user )
    {
    return Err;
    }














