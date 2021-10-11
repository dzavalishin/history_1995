#include "Strng.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void
string::recode_upper_128( const char *tab )
    {
    char *s = v;

    while( *s )
        {
        if( *s >= 0x80 )
            *s = tab[(*s)-0x80];
        s++;
        }
    }

string
string::substr( int pos, int len ) const
	{
	string s;
	int mylen = length();

	if( pos < 0 || len < 0 )
		return s;

	if( (pos + len > mylen) || (len == 0) )
		len = mylen - pos;

	if( len < 0 )
		return s;

	s.f();
	s.v = (char *)malloc( len + 1 );
	strncpy( s.v, v+pos, len );
	s.v[len] = '\0';

	return s;
	}

bool	
string::load( FILE *fp )
	{
	const bs = 4096;
	char b[bs+1];

	f();

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
	char *ee;

	if( v != NULL && (ee = strpbrk( v, "\r\n" )) != NULL )	// Kill any CRLFs
		*ee = '\0';

	}

void	
string::strip_leading_ws()
	{
	if( v == NULL ) return;

	char *vp = v;

	while( *vp == ' ' || *vp == '\t' )		// Skip ws
		vp++;

	strcpy( v, vp );
	}

void
    string::strip_ws()
    {
    if( v == NULL ) return;
    
    unsigned char *vp = (unsigned char *)v;
    
    while( *vp == ' ' || *vp == '\t' )		// Skip ws
        vp++;
    
    strcpy( v, (char *)vp );
    
    vp = (unsigned char *)v+strlen(v);  // vp points to the trainling zero all the time
    while( vp > (unsigned char *)(v) )
        {
        if( isspace(vp[-1]) )
            {
            vp--;
            *vp = 0;
            }
        else
            break;
        }
    
    }


void	
string::strip_leading( int n )
    {
    if( v == NULL ) return;

    char *vp = v;

    while( *vp && n-- )
        vp++;

    strcpy( v, vp );
    }


const char * 
string::c_str() const 
	{
	if( v == NULL ) return "";
	return v;
	}


string::operator const char*          ( ) const
	{
	if( v == NULL ) return (const char*)"";
	return (const char*)v;
	}


string::operator const unsigned char* ( ) const
	{
	if( v == NULL ) return (const unsigned char*)"";
	return (const unsigned char*)v;
	}

string::operator const signed char*   ( ) const
	{
	if( v == NULL ) return (const signed char*)"";
	return (const signed char*)v;
	}

char &
string::operator []          ( int i ) const
    {
    //COW();
    return v[i];
    }


string::
string( void )
	{
	v = NULL;
	}

string::
string( const char *s )
	{
	if( s == NULL ) v = NULL;
	else v = strdup( s );
	c();
	}

string::
string( const unsigned char *s )
	{
	if( s == NULL ) v = NULL;
	else v = strdup( (const char *)s );
	c();
	}

string::
string( const string &s )
	{
	if( s.v == NULL ) v = NULL;
	else v = strdup( s.v );
	c();
	}


string & 
string::operator = ( const string &src )
	{
	f();
	if( src.v == NULL ) v = NULL;
	else v = strdup( src.v );
	c();

	return *this;
	}


string & 
string::operator += ( const string &src )
	{
	int newlen = length() + src.length();

	char *cp = (char *) malloc( newlen+1 );

	if( v != NULL )	strcpy( cp, v );
	else *cp = '\0';
        
        if( src.v != NULL ) strcat( cp, src.v );

	f();
	v = cp;

	return *this;
	}


string
string::operator +  ( const string &src ) const
	{
	int newlen = length() + src.length();

	char *cp = (char *) malloc( newlen+1 );

	if( v != NULL )	strcpy( cp, v );
	else *cp = '\0';

	if( src.v != NULL ) strcat( cp, src.v );

	string s;
	s.f();
	s.v = cp;

	return s;
	}


string _Export operator + ( const string &src1, const char *src2 )
    {
    int newlen = src1.length() + strlen( src2 );
    
    char *cp = (char *) malloc( newlen+1 );
    
    if( src1.v != NULL )	strcpy( cp, src1.v );
    else *cp = '\0';
    
    if( src2 != NULL ) strcat( cp, src2 );
    
    string s;
    s.f();
    s.v = cp;
    
    return s;
    }


string _Export operator + ( const char *src1, const string &src2 )
    {
    int newlen = src2.length() + strlen( src1 );
    
    char *cp = (char *) malloc( newlen+1 );
    
    if( src1 != NULL )	strcpy( cp, src1 );
    else *cp = '\0';
    
    if( src2.v != NULL ) strcat( cp, src2.v );
    
    string s;
    s.f();
    s.v = cp;
    
    return s;
    }




string::
~string()
	{
	f();
	}




int
string::copy( char *cb, int n )
	{
	if( n < (strlen( v )+1) ) n = strlen( v ) + 1;
	strncpy( cb, v, n );
	return n;
	}

int	
string::copy( char *cb, int n, int pos )
	{
	if( n+pos < (strlen( v )+1) ) n = strlen( v ) + 1 - pos;
	strncpy( cb, v+pos, n );
	return n;
	}

int	
string::length() const
	{
	if( v == NULL ) return 0;
	return strlen( v );
	}



int _Export operator == ( const char *s1, const string &s2 )
	{
	return !strcmp( s1, s2.v );
	}

int _Export operator == ( const string &s1, const char *s2 )
	{
	return !strcmp( s1.v, s2 );
	}

int _Export operator == ( const string &s1, const string &s2 )
	{
	return !strcmp( s1.v, s2.v );
	}

int _Export operator > ( const string &s1, const string &s2 )
	{
	return strcmp( s1.v, s2.v ) > 0;
	}

int _Export operator < ( const string &s1, const string &s2 )
	{
	return strcmp( s1.v, s2.v ) < 0;
	}



