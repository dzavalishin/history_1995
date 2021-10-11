head	1.14;
access;
symbols
	Beta_12:1.6;
locks;
comment	@ *      @;


1.14
date	96.12.28.03.11.55;	author dz;	state Exp;
branches;
next	1.13;

1.13
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.12;

1.12
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.07.31.07.26.25;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.07.20.23.24.50;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.05.20.00.35.28;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.01.22.04.35.34;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.01.03.23.58.54;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.12.05.04.23.40;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.11.26.13.58.02;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.11.16.00.54.44;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.11.12.13.12.27;	author dz;	state Exp;
branches;
next	;


desc
@@


1.14
log
@new string/istring/domain_s comparison ops
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	String class
 *
 *      $Log: strng.C $
 *      Revision 1.13  1996/09/08 19:05:31  dz
 *      Version 25
 *
 *      Revision 1.12  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.11  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.10  1996/07/31 07:26:25  dz
 *      Clean up of watcom classlib based code
 *
 *      Revision 1.9  1996/07/20 23:24:50  dz
 *      state save
 *
 *      Revision 1.8  1996/05/20 00:35:28  dz
 *      Fix in string( const char *s ) - should
 *      crash if NULL passed as arg.
 *
 *      Revision 1.7  1996/01/22 04:35:34  dz
 *      C Set & STL support
 *
 *      Revision 1.6  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
 *
\*/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include "strng.h"
#include "addr.h"
#include "except.h"


void string::recode_upper_128( const char *tab )
    {
    COW();
    
    char *s = d->v;

    while( *s )
        {
        if( *s >= 0x80 )
            *s = tab[(*s)-0x80];
        s++;
        }
    }

string string::substr( int pos, int len ) const
    {
    if( pos < 0 || len < 0 ) return string("");
    
    int mylen = length();
    
    if( (pos + len > mylen) || (len == 0) )
        len = mylen - pos;
    
    if(len < 0)        return string("");
    
    sdata *newd = new sdata(len); zdie( newd);
    
    strncpy( newd->v, d->v+pos, len );
    (newd->v)[len] = '\0';
    
    string ss( newd );
    return ss;
    }


void string::save( FILE *fp ) const
    {
    if((EOF == fputs( d->v, fp )) || (EOF == fputs( "\n", fp )))
        throw Ex_Errno( "string::save", "Write error", errno );
    }

void string::load( FILE *fp )
    {
    const bs = 4096;
    char b[bs+1];

    (*this) = "";
    bool first = Yes;

    do	
        {
        if( fgets( b, bs, fp ) == NULL )
            {
            if( feof( fp ) )
                {
                if( !first )   // Have something
                    return;
                throw Ex_EOF( "string::load" );
                }
            throw Ex_Errno( "string::load", "Read error", errno );
            }

        b[bs] = '\0';
        (*this) += string( b );
        first = No;
        } while( ! strpbrk( b, "\r\n" ) );

    return;
    }

void string::strip_crlf()
    {
    COW();
    char *ee;
    if( (ee = strpbrk( d->v, "\r\n" )) != NULL )	// Kill any CRLFs
        *ee = '\0';
    d->newlength();
    }

void string::strip_ws()
    {
    COW();

    unsigned char *vp = (unsigned char *)d->v;

    while( *vp == ' ' || *vp == '\t' )		// Skip ws
        vp++;

    strcpy( d->v, (char *)vp );

    vp = (unsigned char *)d->v+strlen(d->v);  // v points to the trainling zero all the time
    while( vp > (unsigned char *)(d->v) )
        {
        if( isspace(vp[-1]) )
            {
            vp--;
            *vp = 0;
            }
        else
            break;
        }

    d->newlength();
    }


void string::strip_leading_ws()
    {
    COW();
    char *vp = d->v;
    while( *vp == ' ' || *vp == '\t' )		// Skip ws
        vp++;
    strcpy( d->v, vp );
    d->newlength();
    }


void string::strip_leading( int n )
    {
    COW();
    char *vp = d->v;
    while( *vp && n-- )
        vp++;
    strcpy( d->v, vp );
    d->newlength();
    }

void string::strip( int pos, int len  ) // not done
    {
    COW();
    if( pos < 0 || len < 0 )
        return;

    int mylen = length();

    if( (pos + len > mylen) || (len == 0) )
        len = mylen - pos;

    if(len < 0)
        return;

    strcpy( d->v+pos, d->v+pos+len );
    d->newlength();
    }
    
    

// NB!
// In multithreaded env. the following 4 functions 
// should call COW() before returning pointer.

const char * string::c_str() const
    {
    return d->v;
    }


string::operator const char*          ( ) const
    {
    return (const char*)d->v;
    }


string::operator const unsigned char* ( ) const
    {
    return (const unsigned char*)d->v;
    }

string::operator const signed char*   ( ) const
    {
    return (const signed char*)d->v;
    }

  // BUG!!
  // String length can possibly change and
  // our d->len will not be updated :(
  // Maybe we must invalidate it somehow?
char & string::operator []          ( int i )
    {
    COW(); // They might change us!
    if( i < 0 || i > length() )
        throw Ex_Arg("string::operator[]","index out of range","");
    return (d->v)[i];
    }

char  string::operator []           ( int i ) const
    {
    if( i < 0 || i > length() )
        throw Ex_Arg("string::operator[]","index out of range","");
    return (d->v)[i];
    }


string::string( void )
    {
    newdata(0);
    }

string::string( const char *s )
    {
    if( s == NULL ) newdata(0);
    else
        {
        newdata( strlen( s ) );
        strcpy( d->v, s );
        }
    }

string::string( const unsigned char *s )
    {
    if( s == NULL ) newdata(0);
    else
        {
        newdata( strlen( (const char *)s ) );
        strcpy( d->v, (const char *)s );
        }
    }

string::string( const string &s )
    {
    d = s.d;
    s.d->attach();
    }


string & string::operator = ( const string &src )
    {
    detach();
    d = src.d;
    d->attach();
    return *this;
    }


string & string::operator = ( const char *s )
    {
    detach();

    if( s == NULL ) newdata(0);
    else
        {
        newdata( strlen( s ) );
        strcpy( d->v, s );
        }

    return *this;
    }



string & string::operator += ( const string &src )
    {
    int newlen = length() + src.length();

    sdata *newd = new sdata(newlen); zdie(newd);

    strcpy( newd->v, d->v );
    strcat( newd->v, src.d->v );

    detach();
    d = newd;

    return *this;
    }


string string::operator +  ( const string &src ) const
    {
    int newlen = length() + src.length();

    sdata *newd = new sdata(newlen); zdie(newd);

    strcpy( newd->v, d->v );
    strcat( newd->v, src.d->v );

    return string(newd);
    }


string _Export operator + ( const string &src1, const char *src2 )
    {
    if( src2 == 0 ) src2 = "";
    
    int newlen = src1.length() + strlen( src2 );

    string::sdata *newd = new string::sdata(newlen); string::zdie(newd);

    strcpy( newd->v, src1.d->v );
    strcat( newd->v, src2 );

    return string(newd);
    }


string _Export operator + ( const char *src1, const string &src2 )
    {
    if( src1 == 0 ) src1 = "";
    
    int newlen = src2.length() + strlen( src1 );

    string::sdata *newd = new string::sdata(newlen); string::zdie(newd);

    strcpy( newd->v, src1 );
    strcat( newd->v, src2.d->v );

    return string(newd);
    }





/*
int _Export operator == ( const char *s1, const string &s2 )
    {
    if( s1 == 0 ) s1 = "";
    return !strcmp( s1, s2.d->v );
    }

int _Export operator == ( const string &s1, const char *s2 )
    {
    if( s2 == 0 ) s2 = "";
    return !strcmp( s1.d->v, s2 );
    }
    */

#if 0
int string::operator == ( const string &s2 ) const
    {
    return same( s2 ) || (!strcmp( d->v, s2.d->v ));
    }

int string::operator != ( const string &b ) const
    { 
    return !((*this) == b); 
    }


int string::operator > ( const string &s2 ) const
    {
    return strcmp( d->v, s2.d->v ) > 0;
    }

int string::operator < ( const string &s2 ) const 
    {
    return strcmp( d->v, s2.d->v ) < 0;
    }
#else
int string_eq( const string &l, const string &r )
    {
    return l.same( r ) || (!strcmp( l.d->v, r.d->v ));
    }

int istring_eq( const string &l, const string &r )
    {
    return l.same( r ) || (!stricmp( l.d->v, r.d->v ));
    }

int string_lt( const string &l, const string &r )
    {
    return (!l.same( r )) && (strcmp( l.d->v, r.d->v ) < 0);
    }

int istring_lt( const string &l, const string &r )
    {
    return (!l.same( r )) && (stricmp( l.d->v, r.d->v ) < 0);
    }


#endif


#define ARG_HACK( i )  (*((i) ? s1 : s0))

int /* __stdcall */
string::insert( const string *s0, const string *s1 )
    {
    
    int len = length() + 1; // Our current length + 1 is a minimum result len
    
    const char  *c;
    
    for( c = d->v; *c; c++ )
        {
        if( c[0] == '%' && c[1] == '%' )
            c++;
        else if( c[0] == '%' && c[1] >= '0' && c[1] <= '9' )
            {
            len -= 2;
            len += ARG_HACK( c[1] - '0' ).length();
            }
        }
    
    
    sdata *newd = new sdata(len); zdie( newd);
    
    
    char *op = newd->v;
    int   count = 0;
    
    for( c = d->v; *c; c++ )
        {
        
        if( c[0] == '%' && c[1] == '%' )
            {
            c++;
            *op++ = '%';
            continue;
            }
        else if( c[0] == '%' && c[1] >= '0' && c[1] <= '9' )
            {
            const char *ip = ARG_HACK( c[1] - '0' );
            c++;
            count++;
            while( *ip )
                *op++ = *ip++;
            }
        else
            *op++ = *c;
        }
    
    *op = 0;
    
    detach();
    d = newd;
    
    return count;
    }



int string::print( const char *fmt, ... )
    {
    va_list argptr;
    
    va_start(argptr, fmt);
    
    char buf[2048];
    int rc = vsprintf( buf, fmt, argptr);
    va_end(argptr);
    
    (*this) = buf;
    return rc;
    }



void string::zdie( const void *s )
    { 
    if( s==0 ) 
        throw Nomem_Ex();
    }









/*
// Case-Independent comparison for istring

int _Export operator == ( const char *s1, const istring &s2 )
    {
    if( s1 == 0 ) s1 = "";
    return !strcmpi( s1, s2.c_str() );
    }

int _Export operator == ( const istring &s1, const char *s2 )
    {
    if( s2 == 0 ) s2 = "";
    return !strcmpi( s1.c_str(), s2 );
    }
*/

  /*
int operator == ( const istring &s1, const string &s2 )
    {
    return s1.same( s2 ) || (!strcmpi( s1.c_str(), s2.c_str() ));
    }

int operator != ( const istring &a, const string &b )
    { 
    return !(a == b);
    }


int operator > ( const istring &s1, const string &s2 )
    {
    return strcmpi( s1.c_str(), s2.c_str() ) > 0;
    }

int operator < ( const istring &s1, const string &s2 )
    {
    return strcmpi( s1.c_str(), s2.c_str() ) < 0;
    }
*/


#if 0
int string::operator == ( const istring &s ) const { return s.operator == (*this);}
int string::operator != ( const istring &s ) const { return s.operator != (*this);}
int string::operator >  ( const istring &s )  const { return s.operator < (*this);}
int string::operator <  ( const istring &s )  const { return s.operator > (*this);}

int string::operator == ( const domain_s &s ) const { return s.operator == (*this);}
int string::operator != ( const domain_s &s ) const { return s.operator != (*this);}
int string::operator >  ( const domain_s &s ) const { return s.operator < (*this);}
int string::operator <  ( const domain_s &s ) const { return s.operator > (*this);}



int istring::operator == ( const string &s2 ) const
    {
    return same( s2 ) || (!strcmpi( c_str(), s2.c_str() ));
    }

int istring::operator != ( const string &b ) const
    { 
    return !(*this == b);
    }


int istring::operator > ( const string &s2 ) const
    {
    return strcmpi( c_str(), s2.c_str() ) > 0;
    }

int istring::operator < ( const string &s2 ) const
    {
    return strcmpi( c_str(), s2.c_str() ) < 0;
    }


int istring::operator == ( const istring &s2 ) const { return *this == ((string&)s2); }
int istring::operator != ( const istring &s2 ) const { return *this != ((string&)s2); }
int istring::operator > ( const istring &s2 )  const { return *this > ((string&)s2); }
int istring::operator < ( const istring &s2 )  const { return *this < ((string&)s2); }

int istring::operator == ( const domain_s &s ) const { return s.operator == (*this);}
int istring::operator != ( const domain_s &s ) const { return s.operator != (*this);}
int istring::operator >  ( const domain_s &s ) const { return s.operator < (*this);}
int istring::operator <  ( const domain_s &s ) const { return s.operator > (*this);}

#endif

@


1.13
log
@Version 25
@
text
@d8 3
d44 1
d221 13
a233 2
char &
string::operator []          ( int i ) const
d360 1
a360 1

d372 1
d374 1
d395 20
d417 2
d509 1
a509 1

d523 1
d525 1
d546 1
d549 1
a549 2


d555 27
d584 4
d589 4
d594 1
@


1.12
log
@Exceptions added, not compiled
@
text
@d8 3
a34 1
#include "Strng.h"
d40 3
d79 1
a79 1
bool string::save( FILE *fp ) const
d81 2
a82 1
    return ((EOF == fputs( d->v, fp )) || (EOF == fputs( "\n", fp ))) ? Err : Ok;
d85 1
a85 1
bool string::load( FILE *fp )
d100 2
a101 3
                    return Ok;
                else
                    return Err;
d103 1
a103 1
            return Err;
d111 1
a111 1
    return Ok;
d221 1
a221 4
        {
        printf("string index out of range, fatal exit.\n");
        exit(35);
        }
d459 1
a459 1
        { printf("Out of memory!\n"); exit( 34 ); }
d461 58
@


1.11
log
@Before exceptions.
@
text
@d8 3
@


1.10
log
@Clean up of watcom classlib based code
@
text
@d8 3
@


1.9
log
@state save
@
text
@d8 3
a336 15
#if 0
int string::copy( char *cb, int n )
    {
    if( n < (strlen( d->v )+1) ) n = strlen( d->v ) + 1;
    strncpy( cb, d->v, n );
    return n;
    }

int string::copy( char *cb, int n, int pos )
    {
    if( n+pos < (strlen( d->v )+1) ) n = strlen( d->v ) + 1 - pos;
    strncpy( cb, d->v+pos, n );
    return n;
    }
#endif
@


1.8
log
@Fix in string( const char *s ) - should
crash if NULL passed as arg.
@
text
@d7 5
a11 1
 *      $Log: strng.c $
d20 1
d27 1
d30 1
a30 2
void
string::recode_upper_128( const char *tab )
a31 2
//    if( v == 0 ) return;
    
d44 1
a44 2
string
string::substr( int pos, int len ) const
d46 1
a46 3
    //	string s;
    if( pos < 0 || len < 0 )
        return string("");
d53 1
a53 2
    if(len < 0)
        return string("");
a54 2
    //	s.f();
    //	s.v = (char *)malloc( len + 1 );
a60 1
    
d65 1
a65 2
bool	
string::save( FILE *fp ) const
d70 61
a130 10
bool	
string::load( FILE *fp )
	{
	const bs = 4096;
	char b[bs+1];

        //        detach();
        //        d = new sdata(0); zdie( d );
        (*this) = "";
        
d132 2
a133 51
	do	{
		if( fgets( b, bs, fp ) == NULL )
			return Err;

		b[bs] = '\0';

		(*this) += string( b );

		} while( ! strpbrk( b, "\n" ) );

	return Ok;
	}

void    
string::strip_crlf()
	{
        COW();
        char *ee;

	if( (ee = strpbrk( d->v, "\r\n" )) != NULL )	// Kill any CRLFs
		*ee = '\0';

        d->newlength();
	}

void    
string::strip_ws()
	{
        COW();

	unsigned char *vp = (unsigned char *)d->v;

	while( *vp == ' ' || *vp == '\t' )		// Skip ws
		vp++;

	strcpy( d->v, (char *)vp );

	vp = (unsigned char *)d->v+strlen(d->v);  // v points to the trainling zero all the time
        while( vp > (unsigned char *)(d->v) )
		{
		if( isspace(vp[-1]) )
			{
			vp--;
			*vp = 0;
			}
		else
			break;
		}
        
        d->newlength();
	}
d136 1
a136 2
void	
string::strip_leading_ws()
a139 1
    
a141 1
    
d147 1
a147 2
void	
string::strip_leading( int n )
a149 1

a150 1

a152 1

d157 10
d168 17
a184 5
const char * 
string::c_str() const 
	{
	return d->v;
	}
d188 3
a190 3
	{
	return (const char*)d->v;
	}
d194 3
a196 3
	{
	return (const unsigned char*)d->v;
	}
d199 3
a201 3
	{
	return (const signed char*)d->v;
	}
d205 24
a228 22
	{
	return (d->v)[i];
	}


string::
string( void )
	{
        //	v = NULL;
        newdata(0);
	}

string::
string( const char *s )
	{
	if( s == NULL ) newdata(0);
        else 
            {
            newdata( strlen( s ) );
            strcpy( d->v, s );
            }
	}
d230 9
a238 10
string::
string( const unsigned char *s )
	{
	if( s == NULL ) newdata(0);
        else
            {
            newdata( strlen( (const char *)s ) );
            strcpy( d->v, (const char *)s );
            }
	}
d240 1
a240 2
string::
string( const string &s )
d247 1
a247 2
string & 
string::operator = ( const string &src )
d252 14
a270 14
string & 
string::operator = ( const char *s )
	{
        detach();
        
        if( s == NULL ) newdata(0);
        else
            {
            newdata( strlen( s ) );
            strcpy( d->v, s );
            }
        
	return *this;
	}
d272 3
d276 1
d278 2
a279 4
string & 
string::operator += ( const string &src )
	{
	int newlen = length() + src.length();
d281 2
a282 1
        sdata *newd = new sdata(newlen); zdie(newd);
d284 2
a285 2
	strcpy( newd->v, d->v );
        strcat( newd->v, src.d->v );
a286 5
        detach();
        d = newd;
        
	return *this;
	}
d288 3
d292 1
a292 4
string
string::operator +  ( const string &src ) const
	{
	int newlen = length() + src.length();
d294 2
a295 1
        sdata *newd = new sdata(newlen); zdie(newd);
d297 2
a298 5
	strcpy( newd->v, d->v );
        strcat( newd->v, src.d->v );

	return string(newd);
	}
a331 7
/*
string::
~string()
	{
	f();
	}
*/
d334 7
d342 7
a348 25
int
string::copy( char *cb, int n )
	{
	if( n < (strlen( d->v )+1) ) n = strlen( d->v ) + 1;
	strncpy( cb, d->v, n );
	return n;
	}

int	
string::copy( char *cb, int n, int pos )
	{
	if( n+pos < (strlen( d->v )+1) ) n = strlen( d->v ) + 1 - pos;
	strncpy( cb, d->v+pos, n );
	return n;
	}

/*
int	
string::length() const
	{
	if( v == NULL ) return 0;
	return strlen( v );
	}
*/

d351 4
a354 6
	{
        // if( s1 == 0 && s2.v == 0 ) return 1;
        // if( s1 == 0 || s2.v == 0 ) return 0;
        if( s1 == 0 ) s1 = "";
        return !strcmp( s1, s2.d->v );
	}
d357 4
a360 6
	{
        // if( s1.v == 0 && s2 == 0 ) return 1;
        //  if( s1.v == 0 || s2 == 0 ) return 0;
        if( s2 == 0 ) s2 = "";
	return !strcmp( s1.d->v, s2 );
	}
d363 3
a365 5
	{
        // if( s1.v == 0 && s2.v == 0 ) return 1;
        // if( s1.v == 0 || s2.v == 0 ) return 0;
	return same( s2 ) || (!strcmp( d->v, s2.d->v ));
	}
d374 3
a376 5
	{
        // if( s2.v == 0 ) return 1;
        // if( s1.v == 0 ) return 0;
	return strcmp( d->v, s2.d->v ) > 0;
	}
d379 3
a381 6
	{
        // if( s2.v == 0 ) return 0;
        // if( s1.v == 0 ) return 1;
	return strcmp( d->v, s2.d->v ) < 0;
	}

a383 2
//#define ARG_HACK( i )  (*((s0)+(i)))
//#pragma aux string::insert PARM CALLER
a389 1
//debug("insert: v= "+string(v));
a405 2
//debug("insert: v= "+string(v));
    //    char *o = (char *)malloc( len + 1 );
a411 1
//debug("insert: v= "+string(v));
a424 1
//debug("inserting arg "+string(ip));
a432 1
//debug("insert: v= "+string(v));
a435 2
    //    f();
    //    v = o;
d441 6
d448 1
d450 3
d454 11
a464 17
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    @


1.7
log
@C Set & STL support
@
text
@d8 3
d209 5
a213 2
	else newdata( strlen( s ) );
        strcpy( d->v, s );
@


1.6
log
@Beta 12
@
text
@d7 4
a10 1
 *      $Log:$
d121 1
a121 1
        while( vp > d->v )
d405 1
a405 1
int __stdcall 
@


1.5
log
@Fixed memory hog in operator =
@
text
@d1 11
d368 1
a368 1
int _Export operator == ( const string &s1, const string &s2 )
d372 1
a372 1
	return s1.same( s2 ) || (!strcmp( s1.d->v, s2.d->v ));
d375 7
a381 1
int _Export operator > ( const string &s1, const string &s2 )
d385 1
a385 1
	return strcmp( s1.d->v, s2.d->v ) > 0;
d388 1
a388 1
int _Export operator < ( const string &s1, const string &s2 )
d392 1
a392 1
	return strcmp( s1.d->v, s2.d->v ) < 0;
@


1.4
log
@before COW strings
@
text
@d10 1
a10 1
    if( v == 0 ) return;
d12 3
a14 1
    char *s = v;
d26 24
a49 20
	{
	string s;
	if( pos < 0 || len < 0 || v == 0 )
		return s;

	int mylen = length();

	if( (pos + len > mylen) || (len == 0) )
		len = mylen - pos;

	if(len < 0)
		return s;

	s.f();
	s.v = (char *)malloc( len + 1 );
	strncpy( s.v, v+pos, len );
	s.v[len] = '\0';

	return s;
	}
d55 1
a55 1
    return ((EOF == fputs( v == 0 ? "" : v, fp )) || (EOF == fputs( "\n", fp ))) ? Err : Ok;
d64 4
a67 1
	f();
d85 2
a86 1
	char *ee;
d88 1
a88 1
	if( v != NULL && (ee = strpbrk( v, "\r\n" )) != NULL )	// Kill any CRLFs
d91 1
d97 1
a97 1
	if( v == NULL ) return;
d99 1
a99 1
	unsigned char *vp = (unsigned char *)v;
d104 1
a104 1
	strcpy( v, (char *)vp );
d106 2
a107 2
	vp = (unsigned char *)v+strlen(v);  // v points to the trainling zero all the time
        while( vp > v )
d117 2
a118 1

d124 10
a133 10
	{
	if( v == NULL ) return;

	char *vp = v;

	while( *vp == ' ' || *vp == '\t' )		// Skip ws
		vp++;

	strcpy( v, vp );
	}
d139 1
a139 1
    if( v == NULL ) return;
d141 1
a141 1
    char *vp = v;
d146 2
a147 1
    strcpy( v, vp );
d154 1
a154 2
	if( v == NULL ) return "";
	return v;
d160 1
a160 2
	if( v == NULL ) return (const char*)"";
	return (const char*)v;
d166 1
a166 2
	if( v == NULL ) return (const unsigned char*)"";
	return (const unsigned char*)v;
d171 1
a171 2
	if( v == NULL ) return (const signed char*)"";
	return (const signed char*)v;
d177 1
a177 1
	return v[i];
d184 2
a185 1
	v = NULL;
d191 3
a193 3
	if( s == NULL ) v = NULL;
	else v = strdup( s );
///	c();
d199 6
a204 3
	if( s == NULL ) v = NULL;
	else v = strdup( (const char *)s );
//	c();
d209 4
a212 5
	{
	if( s.v == NULL ) v = NULL;
	else v = strdup( s.v );
//	c();
	}
d217 11
d229 9
a237 5
	f();
	if( src.v == NULL ) v = NULL;
	else v = strdup( src.v );
//	c();

d242 1
d248 1
a248 1
	char *cp = (char *) malloc( newlen+1 );
d250 5
a254 2
	if( v != NULL )	strcpy( cp, v );
	else *cp = '\0';
a255 5
        if( src.v != NULL ) strcat( cp, src.v );

	f();
	v = cp;

d265 1
a265 4
	char *cp = (char *) malloc( newlen+1 );

	if( v != NULL )	strcpy( cp, v );
	else *cp = '\0';
d267 2
a268 1
	if( src.v != NULL ) strcat( cp, src.v );
d270 1
a270 5
	string s;
	s.f();
	s.v = cp;

	return s;
d276 2
d279 7
a285 13
    
    char *cp = (char *) malloc( newlen+1 );
    
    if( src1.v != NULL )	strcpy( cp, src1.v );
    else *cp = '\0';
    
    if( src2 != NULL ) strcat( cp, src2 );
    
    string s;
    s.f();
    s.v = cp;
    
    return s;
d291 2
d294 7
a300 13
    
    char *cp = (char *) malloc( newlen+1 );
    
    if( src1 != NULL )	strcpy( cp, src1 );
    else *cp = '\0';
    
    if( src2.v != NULL ) strcat( cp, src2.v );
    
    string s;
    s.f();
    s.v = cp;
    
    return s;
d305 1
a305 1

d311 1
a311 1

d318 2
a319 2
	if( n < (strlen( v )+1) ) n = strlen( v ) + 1;
	strncpy( cb, v, n );
d326 2
a327 2
	if( n+pos < (strlen( v )+1) ) n = strlen( v ) + 1 - pos;
	strncpy( cb, v+pos, n );
d331 1
d338 1
a338 1

d343 4
a346 3
        if( s1 == 0 && s2.v == 0 ) return 1;
        if( s1 == 0 || s2.v == 0 ) return 0;
        return !strcmp( s1, s2.v );
d351 4
a354 3
        if( s1.v == 0 && s2 == 0 ) return 1;
        if( s1.v == 0 || s2 == 0 ) return 0;
	return !strcmp( s1.v, s2 );
d359 3
a361 3
        if( s1.v == 0 && s2.v == 0 ) return 1;
        if( s1.v == 0 || s2.v == 0 ) return 0;
	return !strcmp( s1.v, s2.v );
d366 3
a368 3
        if( s2.v == 0 ) return 1;
        if( s1.v == 0 ) return 0;
	return strcmp( s1.v, s2.v ) > 0;
d373 3
a375 3
        if( s2.v == 0 ) return 0;
        if( s1.v == 0 ) return 1;
	return strcmp( s1.v, s2.v ) < 0;
a386 9
#if 0
               const string &s0,
               ,
               const string &s2,  const string &s3,
               const string &s4, const string &s5, const string &s6,
               const string &s7, const string &s8,
               const string &s9
              )
#endif
d394 1
a394 1
    for( c = v; *c; c++ )
d406 6
a411 2
    char *o = (char *)malloc( len + 1 );
    char *op = o;
d415 1
a415 1
    for( c = v; *c; c++ )
d440 4
a443 3
    f();
    
    v = o;
@


1.3
log
@beta 9
@
text
@d4 1
d82 27
@


1.2
log
@beta 7
@
text
@d337 2
a338 1
#define ARG_HACK( i )  (*((s0)+(i)))
d340 1
a340 1
//#pragma aux string::insert PARM CALLER
d343 1
a343 1
string::insert( const string *s0, ... )
d345 1
a345 1
               const string &s0,  const string &s1
@


1.1
log
@Initial revision
@
text
@d337 90
@
