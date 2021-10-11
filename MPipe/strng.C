/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	String class
 *
 *      $Log: strng.C $
 *      Revision 1.4  1996/07/07 10:01:14  dz
 *      *** empty log message ***
 *
 *      Revision 1.3  1996/07/06 05:25:19  dz
 *      *** empty log message ***
 *
 *      Revision 1.2  1996/07/02 04:33:28  dz
 *      writing
 *
 *      Revision 1.1  1996/06/30 22:58:25  dz
 *      Initial revision
 *
 *
 *
\*/

#include "Strng.h"
//#include "MpUtil.h"
#include "MpExcept.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void string::recode_upper_128( const char *tab )
    {
    COW();
    
    unsigned char *s = (unsigned char *)d->v;

    while( *s )
        {
        if( *s >= 0x80 )
            *s = tab[(*s)-0x80];
        s++;
        }
    }

string string::substr( int pos, int len ) const
    {
    if( pos < 0 || len < 0 )  return string("");

    int mylen = length();

    if( (pos + len > mylen) || (len == 0) )
        len = mylen - pos;

    if(len < 0)               return string("");
    
    sdata *newd = new sdata(len); zdie( newd);
    
    strncpy( newd->v, d->v+pos, len );
    (newd->v)[len] = '\0';
    
    string ss( newd );
    return ss;
    }


void string::save( FILE *fp ) const
    {
    if((EOF == fputs( d->v, fp )) || (EOF == fputs( "\n", fp )))
        throw Mp_Ex_Fail( "string::save", "unable to write", "" );
;
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
            if( feof(fp) )
                {
                if( !first )   // Have something
                    return;
                throw Mp_Ex_EOF( "string::load" );
                }

            throw Mp_Ex_Fail( "string::load", "unable to read", "" );
            }

        b[bs] = '\0';
        (*this) += string( b );
        first = No;
        } while( ! strpbrk( b, "\r\n" ) );

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
    
    // NB! v points to the trainling zero all the time
    vp = (unsigned char *)d->v+strlen(d->v);
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
        throw Mp_Ex_Fail( "string::strip", "pos or len < 0", *this );

    int mylen = length();

    if( (pos + len > mylen) || (len == 0) )
        len = mylen - pos;

    if(len < 0)
        throw Mp_Ex_Fail( "string::strip", "pos > string length", *this );

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


string::operator const char* ( ) const
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

char & string::operator []          ( int i ) const
    {
    COW();
    if( i < 0 || i > length() )
        throw Mp_Ex_Fail( "string::operator []", "invalid index", *this );
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




int string::insert( const string *s0, const string *s1 )
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
            len += (*((c[1]-'0') ? s1 : s0)).length();
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
            const char *ip = (*((c[1]-'0') ? s1 : s0));
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
        throw Mp_Ex_NoMemory("string::zdie");
    }


#if ISTRING

// Case-Independent comparison for istring

int _Export operator == ( const char *s1, const istring &s2 )
    {
    if( s1 == 0 ) s1 = "";
    return !strcmpi( s1, s2.d->v );
    }

int _Export operator == ( const istring &s1, const char *s2 )
    {
    if( s2 == 0 ) s2 = "";
    return !strcmpi( s1.d->v, s2 );
    }

int istring::operator == ( const string &s2 ) const
    {
    return same( s2 ) || (!strcmpi( d->v, s2.d->v ));
    }

int istring::operator != ( const string &b ) const
    { 
    return !((*this) == b); 
    }


int istring::operator > ( const string &s2 ) const
    {
    return strcmpi( d->v, s2.d->v ) > 0;
    }

int istring::operator < ( const string &s2 ) const
    {
    return strcmpi( d->v, s2.d->v ) < 0;
    }

int string::operator == ( const istring &s ) const { return s.operator == (*this);}
int string::operator != ( const istring &s ) const { return s.operator != (*this);}
int string::operator >  ( const istring &s )  const { return s.operator < (*this);}
int string::operator <  ( const istring &s )  const { return s.operator > (*this);}


#endif














