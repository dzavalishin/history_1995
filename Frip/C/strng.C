/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	String class
 *
 *      $Log: strng.C $
 *      Revision 1.14  1996/12/28 03:11:55  dz
 *      new string/istring/domain_s comparison ops
 *
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

