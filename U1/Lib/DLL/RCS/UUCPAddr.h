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
date	95.03.11.18.07.42;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@debug
@
text
@/************************ UUCP interface library ***************************\
 *
 *  Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   UUCP address class definition
 *
 *      $Log: UUCPAddr.h $
 * Revision 1.1  1995/03/11  18:07:42  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:07:42  dz
 * Initial revision
 *
 *      
 *         Rev 1.1   07 Jun 1993 18:04:10   dz
 *      update
 *      
 *         Rev 1.0   09 Nov 1992 23:00:20   dz
 *      Initial revision.
 *
 *
\*/

#ifndef UUCPADDR_H
#define UUCPADDR_H

#include    <style.h>
#include    <string.h>
#include    <stdlib.h>
//#include    <dbllist.h>

#pragma pack(1)


class   fido_addr;
class   fido_user;
class   uucp_addr;

    /*************************************************************
                             Definition
    *************************************************************/

class _Export   uucp_addr
    {

//Û
//Û     Internal types
//Û

public:

    enum el_type
    {
    UU_name         = 1,    // Local part (user name)       host2!USER@@host1
    UU_via          = 2,    // Source routing (via) domain  @@DOMAIN1:user@@d2
    UU_domain       = 3,    // Destination domain           user@@DOMAIN
    UU_bang         = 4     // Bang path host               HOST!user
    };

    typedef
        bool (*tr_func)
            ( el_type t, const char *el, int len, void *arg );

        /*********************************************
                        Data members
        *********************************************/


    unsigned char   *addr_v;

        /*********************************************
                          Methods
        *********************************************/

public:

    inline              uucp_addr( void );              // Default constructor
    inline              uucp_addr( const uucp_addr & ); // Copy constructor
    inline              uucp_addr( const char *str );   // From string
    ~uucp_addr( void ) { clear(); }                     // Destructor

    inline void         addr( const char *s );
    inline const char*  addr( void ) const;
//  const char*         name( void ) const;

    bool                operator == ( const uucp_addr &a) const;
    inline bool         operator != ( const uucp_addr &a ) const;
    inline uucp_addr &  operator =  ( const uucp_addr &a );
    inline uucp_addr &  operator =  ( const char *v );
    inline              operator const char * () const;

    bool                convert_to_fido( fido_user &,
                                        char *uname, int uname_len ) const;

    inline bool         valid( void ) const;
    inline void         clear( void );

    // Go through the address elements, calling finction
    bool                traverse( tr_func func, void *arg ) const;


    };




    /*************************************************************
                           Inline methods
    *************************************************************/

    /*************************************************************
                           C'tor/D'tor
    *************************************************************/

inline uucp_addr::uucp_addr( void )
    {   addr_v = NULL;                  }

inline uucp_addr::uucp_addr( const uucp_addr &v )
    {   addr_v = (uchar *) strdup( (const char *)(v.addr_v) );    }

inline uucp_addr::uucp_addr( const char *v )
    {   addr_v = (uchar *) strdup( (const char *)v );           }

inline void uucp_addr::
addr( const char *s )
    {
    clear();
    addr_v = (uchar *) strdup( s );
    }

inline uucp_addr & uucp_addr::
operator =  ( const uucp_addr &a )
    {
    clear();
    addr_v = (uchar *)strdup( (const char *)(a.addr_v) );
    return *this;
    }

inline uucp_addr & uucp_addr::
operator =  ( const char *v )
    {
    clear();
    addr_v = (uchar *)strdup( v );
    return *this;
    }

inline const char* uucp_addr::
addr( void ) const
    {
    return (const char *)addr_v;
    }

inline uucp_addr::
operator const char * () const
    {
    return (const char *)addr_v;
    }

/*--------------------------------------------------------------------------*/

inline bool uucp_addr::
operator != ( const uucp_addr &a ) const
    {
    return (bool) ! ((int) ( *this == a ));
    }


/*--------------------------------------------------------------------------*/

inline bool uucp_addr::
valid( void ) const
    {
    if( addr_v != NULL )    return Yes;
    return No;
    }

inline void uucp_addr::
clear( void )
    {
    if( addr_v )
        free( addr_v );
    addr_v = NULL;
    }


#pragma pack()

#endif // UUCPADDR_H
@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/uucp/vcs/uucpaddr.h_v  $
d29 1
a29 1
#include    <alloc.h>
d43 1
a43 1
class   uucp_addr
d57 1
a57 1
    UU_bang         = 4,    // Bang path host               HOST!user
d119 1
a119 1
    {   addr_v = strdup( v.addr_v );    }
d122 1
a122 1
    {   addr_v = strdup( v );           }
d128 1
a128 1
    addr_v = strdup( s );
d135 1
a135 1
    addr_v = strdup( a.addr_v );
d143 1
a143 1
    addr_v = strdup( v );
d150 1
a150 1
    return addr_v;
d188 1
a188 1
#endif UUCPADDR_H
@
