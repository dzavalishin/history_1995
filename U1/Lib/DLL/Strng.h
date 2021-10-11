#ifndef STRNG_H
#define STRNG_H

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include "style.h"
#include <string.h>

class _Export string
	{
	char	*v;

	inline void f() { if( v != NULL ) free( v ); v = NULL; }
	inline void c() { /* if( v == NULL ) v = ""; */ }

public:
	string( void );
	string( const char * );
	string( const unsigned char * );
	string( const string & );

	~string();

//	inline const char * c_str() const { return v; }
	const char * c_str() const;

	int	copy( char *cb, int n );
	int	copy( char *cb, int n, int pos );

	int	length() const;

	int	is_empty() const { return length() == 0; }

	bool	load( FILE *fp );
	void    strip_crlf();
	void	strip_leading_ws();
	void	strip_ws();
	void	strip_leading( int n ); // Just delete leading n characters
        
        bool    parse( string &out1, string &out2 ) const;
        bool    parse( string &out1, char c, string &out2 ) const;
        
        
	string	substr( int pos, int len = 0 ) const;

        void    recode_upper_128( const char *tab );
        void    lcase() // convert to lower case
                { if( v != 0 ) strlwr( v ); }
        void    ucase()
                { if( v != 0 ) strupr( v ); }

	friend int operator == ( const char *, const string & );
	friend int operator == ( const string &, const char * );

	friend int operator == ( const string &, const string & );
	friend int operator >  ( const string &, const string & );
	friend int operator <  ( const string &, const string & );

	string & operator =  ( const string & );
	string & operator += ( const string & );
	string   operator +  ( const string & ) const;

	operator const char*          ( ) const;
	operator const unsigned char* ( ) const;
	operator const signed char*   ( ) const;

        char &operator []             ( int i ) const;

        friend string _Export operator + ( const string &, const char * );
        friend string _Export operator + ( const char *, const string & );
        
        };

int _Export operator == ( const char *, const string & );
int _Export operator == ( const string &, const char * );

int _Export operator < ( const string &, const string & );
int _Export operator > ( const string &, const string & );

string _Export operator + ( const string &, const char * );
string _Export operator + ( const char *, const string & );
//string _Export operator + ( const string &, const unsigned char * );
//string _Export operator + ( const char *, const unsigned string & );

#define String string


#endif
