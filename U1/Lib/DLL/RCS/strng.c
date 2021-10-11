head	1.1;
access;
symbols;
locks; strict;
comment	@ * @;


1.1
date	95.04.13.11.06.28;	author dz;	state Exp;
branches;
next	;


desc
@string class, simple one
@


1.1
log
@Initial revision
@
text
@#include "Strng.h"
#include "stdlib.h"
#include "string.h"


const char * 
string::c_str() const 
	{
	if( v == NULL ) return "";
	return v;
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
	f();
	if( s == NULL ) v = NULL;
	else v = strdup( (const char *)s );
	c();
	}

string::
string( const string &s )
	{
	f();
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
	return strcmp( s1, s2.v );
	}

int _Export operator == ( const string &s1, const char *s2 )
	{
	return strcmp( s1.v, s2 );
	}

int _Export operator == ( const string &s1, const string &s2 )
	{
	return strcmp( s1.v, s2.v );
	}

int _Export operator > ( const string &s1, const string &s2 )
	{
	return strcmp( s1.v, s2.v ) > 0;
	}

int _Export operator < ( const string &s1, const string &s2 )
	{
	return strcmp( s1.v, s2.v ) < 0;
	}



@
