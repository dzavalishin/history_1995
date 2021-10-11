head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	95.11.06.02.33.43;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.11.05.18.50.12;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.11.05.13.52.48;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.13.11.06.28;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.04.17;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@Fixes & cleanups caused by FTN_Msg introduction
@
text
@#ifndef STRNG_H
#define STRNG_H

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <style.h>
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
	void	strip_leading( int n ); // Just delete leading n characters

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
@


1.4
log
@RFC_Msg used
@
text
@d38 1
d65 4
a68 1
	};
d76 4
@


1.3
log
@current.
@
text
@d62 2
@


1.2
log
@debug
@
text
@d6 3
d35 12
d54 8
a61 1
	string & operator = ( const string & );
@


1.1
log
@Initial revision
@
text
@d1 2
a2 1
#include <cstring.h>
d4 2
a5 2
//class _CLASSTYPE String : public string
// {};
d7 3
d11 38
d50 3
@
