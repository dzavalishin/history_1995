head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	95.11.04.18.32.53;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.11.04.01.21.46;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.11.02.14.25.20;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.08.14.02.59.56;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.08.08.13.22.34;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@Now returning message to sender in a case of
address conversion errors.
Some debugging done.
@
text
@/************************ FIDO2UU ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	RFC Message Class
 *
 *      $Log: RFC_Msg.c $
 *      Revision 1.4  1995/11/04 01:21:46  dz
 *      First RFC_Msg-based version, that compiles and does something
 *
 *      Revision 1.3  1995/11/02 14:25:20  dz
 *      Before switching to rfc_msg
 *
 *      Revision 1.2  1995/08/14 02:59:56  dz
 *      *** empty log message ***
 *
 *      Revision 1.1  1995/08/08 13:22:34  dz
 *      Initial revision
 *
 *
 *
\*/

#include	"hl.h"
#include	"RFC_Msg.h"

#include	<log.h>
#include	<ctype.h>


int                     
hl::operator == (const hl &r) const 
    {
    return key_v==r.key_v && val_v==r.val_v; 
    }


hl::hl(const string &s)
	{
	char *k = strdup( s );

	if( k == NULL )
		out_of_memory();

	
	char *vp = strchr( s, ':' );
	char *vp_orig = vp;

	if( vp == NULL )
		{
		key_v = s;
//		val_v = "";
		}
	else
		{
		*vp = '\0';
		vp++;
		while( *vp == ' ' || *vp == '\t' )
			vp++;

		val_v.add( String(vp) );
		key_v = vp_orig;
		}

	free( vp );
	}


hl *
RFC_headlines::have_key( const string &key )
    {
    hl_Cursor c( *this );

    for( c.setToFirst(); c.isValid(); c.setToNext() )
        {
        if( 0 == stricmp( c.element().key(), key ) )
// BUG
// i'm removing const modifier here :(
            return (hl *) (&c.element());
        }

    return NULL;
    }


hl *
RFC_headlines::add_or_find_key( const string &key )
    {
    hl *key_hl;

    if( (key_hl = have_key( key )) == NULL )
        {
        add( hl( key ) );
        }

    return have_key( key );  // we must get pointer to that one in container
    }

void             
RFC_headlines::add_hl( const string &key, const string &val )
    {
    add_or_find_key( key )->add_val( val );
    }


bool RFC_headlines::
get( string &val, const string key )
    {
    hl *h = have_key( key );
    
    if( NULL == h )
        return No;
    
    hl_val_Cursor hvc( h->val() );

    hvc.setToFirst(); 
    
    if( !hvc.isValid())
        return No;
        
    val = hvc.element();
        
    return Yes;
    }


string RFC_headlines::
operator [] (const string &key)
    {
    string s;
    
    if( get( s, key ) ) return s;
    return string("");
    }











RFC_Msg::RFC_Msg()
    {
    froms_invalid_v = Yes;
    
    lr = sr = 0;
    
    size_v = 0;
    }

RFC_Msg::~RFC_Msg()
    {
    }


void
RFC_Msg::add_headline(const string& hl)
    {
    const char *l = hl, *cp;

//    cp = strchr( l, ':' );
    cp = l;
    while( isalpha( *cp ) || isdigit( *cp ) ||*cp == '-' ) cp++;

    if( *cp != ':' )
        {

        if( strnicmp( l, "from", 4 ) == 0 && isspace( *cp ) )
            {
            while( isspace (*cp) ) cp++;
            envelope.add_from( cp );
            return;
            }

        error( EI_None, "Incorrect headline (skipped) '%s'", l );
        return;
        }

    string key = hl.substr( 0, cp-l );

    cp++; // skip ':'
    while( isspace (*cp) ) cp++;

    string val = hl.substr( cp-l, 0 ); // len == 0: all the rest

    headline.add_hl( key, val );
    }

@


1.4
log
@First RFC_Msg-based version, that compiles and does something
@
text
@d8 3
d76 1
a76 1
        if( 0 == strcmp( c.element().key(), key ) )
@


1.3
log
@Before switching to rfc_msg
@
text
@d8 3
d103 40
d145 5
d172 1
a172 1
//          envelope.add_from( cp );
@


1.2
log
@*** empty log message ***
@
text
@d8 3
d22 1
d25 5
a29 1

d46 1
a46 1
		val_v = "";
d55 1
a55 1
		val_v = vp;
d61 80
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: UUEncode.c $
d15 1
d18 9
a26 4
//#include	<sys/types.h>
//#include	<sys/stat.h>
//#include	<share.h>
//#include	<fcntl.h>
d28 2
d31 3
d35 11
d47 3
d51 2
@
