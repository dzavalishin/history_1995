head	1.13;
access;
symbols
	Beta_12:1.5;
locks;
comment	@ *      @;


1.13
date	97.03.28.22.04.16;	author dz;	state Exp;
branches;
next	1.12;

1.12
date	96.12.28.03.11.55;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.07.20.23.24.50;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.01.22.04.36.41;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.01.14.01.07.53;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.01.03.23.58.54;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.12.05.04.23.40;	author dz;	state Exp;
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


1.13
log
@ostream <<
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	String
 *
 *      $Log: strng.h $
 *      Revision 1.12  1996/12/28 03:11:55  dz
 *      new string/istring/domain_s comparison ops
 *
 *      Revision 1.11  1996/09/08 19:05:31  dz
 *      Version 25
 *
 *      Revision 1.10  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.9  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.8  1996/07/20 23:24:50  dz
 *      state save
 *
 *      Revision 1.7  1996/01/22 04:36:41  dz
 *      C Set & STL support
 *
 *      Revision 1.6  1996/01/14 01:07:53  dz
 *      beta 14
 *
 *      Revision 1.5  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

#ifndef STRNG_H
#define STRNG_H

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <style.h>
#include <string.h>
#include <iostream.h>

#if defined(__WATCOMC__) || defined(__GNUC__) || defined (__BORLANDC__)
#define _Export
#endif

class istring;
class domain_s;




class _Export string
    {
public:
    static void zdie( const void *s );
    
private:
    class sdata
        {
public:
        friend class string;
//        friend int _Export operator == ( const char *, const string & );
//        friend int _Export operator == ( const string &, const char * );
//        friend int _Export operator == ( const string &, const string & );
//        friend int _Export operator > ( const string &s1, const string &s2 );
//        friend int _Export operator < ( const string &s1, const string &s2 );
        
        char	*v;
        int     refs;     // references
        int     len;      // length of DATA (not buffer) without trailing zero
        
        sdata( int alen );
        sdata( const sdata &i );
        virtual ~sdata();
        void newlength();
        void attach();
        }; // end of class sdata def.
    
    
    sdata *      d;
    
    void         detach();
    void         COW(); // copy on write
    void         newdata(int size);
                 string( sdata *ss );

protected:
    void         newlength();

public:
                 string( void );
                 string( const char * );
                 string( const unsigned char * );
                 string( const string & );
    
    virtual      ~string();
    
    const char * c_str() const;
    
    int	         copy( char *cb, int n );
    int	         copy( char *cb, int n, int pos );
    
    int	         length() const { return d->len; }
    
    int	         is_empty() const { return length() == 0; }
    
    void         load( FILE *fp );
    void         save( FILE *fp ) const;

    void         strip_crlf();
    void         strip_ws();
    void         strip_leading_ws();
    void         strip_leading( int n ); // Just delete leading n characters
    void         strip( int pos, int len  );
    
    string       substr( int pos, int len = 0 ) const;
    
    int          insert( const string *s0, const string *s1 );
    int          print( const char *fmt, ... );

    bool         parse( string &out1, char c, string &out2 ) const;
    bool         parse( string &out1, string &out2 ) const; // delimiter - space
    
    void         recode_upper_128( const char *tab );
    void         lcase(); // convert to lower case
    void         ucase();
    
    virtual bool same( const string &s ) const { return d == s.d ? Yes : No; }
    
    //friend int operator == ( const char *, const string & );
    //friend int operator == ( const string &, const char * );
    
    //virtual int  operator == ( const domain_s & ) const;
    //virtual int  operator != ( const domain_s & ) const;
    //virtual int  operator >  ( const domain_s & ) const;
    //virtual int  operator <  ( const domain_s & ) const;

    //virtual int  operator == ( const istring &  ) const;
    //virtual int  operator != ( const istring &  ) const;
    //virtual int  operator >  ( const istring &  ) const;
    //virtual int  operator <  ( const istring &  ) const;
        
    enum          srtti { srtti_string, srtti_istring, srtti_domain };
    virtual srtti rtti() const { return srtti_string; }
    
    int          operator == ( const string &   ) const;
    int          operator != ( const string &   ) const;
    int          operator >  ( const string &   ) const;
    int          operator <  ( const string &   ) const;
    int          operator >= ( const string &   ) const;
    int          operator <= ( const string &   ) const;
    
    string &     operator =  ( const string & );
    string &     operator =  ( const char *s );
    
    string &     operator += ( const string & );
    string       operator +  ( const string & ) const;

#ifndef __BORLANDC__    
                 operator const char*          ( ) const;
#endif
                 operator const unsigned char* ( ) const;
                 operator const signed char*   ( ) const;
    
    char &       operator []             ( int i );
    char         operator []             ( int i ) const;
    
    friend string _Export operator + ( const string &, const char * );
    friend string _Export operator + ( const char *, const string & );

    friend int   string_eq( const string &l, const string &r );
    friend int   string_lt( const string &l, const string &r );
    friend int   istring_eq( const string &l, const string &r );
    friend int   istring_lt( const string &l, const string &r );
    friend int   domain_eq( const string &l, const string &r );
    friend int   domain_lt( const string &l, const string &r );


    friend ostream &    operator << ( ostream & st, string ss );

    };


inline ostream &    operator << ( ostream & st, string ss )
    {
    st << ss.d->v;
    return st;
    }
    


//string _Export operator + ( const string &, const char * );
//string _Export operator + ( const char *, const string & );


//int _Export operator == ( const char *, const string & );
//int _Export operator == ( const string &, const char * );

//int operator != ( const string &a, const string &b ) { return !(a == b); };


//int _Export operator < ( const string &, const string & );
//int _Export operator > ( const string &, const string & );

//string _Export operator + ( const string &, const unsigned char * );
//string _Export operator + ( const char *, const unsigned string & );

// No! Watcom has String class, and this define will mess things up
//#define String string

// case Insensitive string
class _Export istring : public string
    {
protected:
    virtual srtti rtti() const { return srtti_istring; }

public:
    istring( void ) {}
    istring( const char *p )           : string(p) {}
    istring( const unsigned char * p ) : string(p) {}
    istring( const string &p )         : string(p) {}
    istring( const istring &p )        : string(p) {}
        

#if 0
    //virtual int operator == ( const string &   ) const;
    //virtual int operator != ( const string &   ) const;
    //virtual int operator >  ( const string &   ) const;
    //virtual int operator <  ( const string &   ) const;
    
    //virtual int operator == ( const istring &  ) const;
    //virtual int operator != ( const istring &  ) const;
    //virtual int operator >  ( const istring &  ) const;
    //virtual int operator <  ( const istring &  ) const;
    
    //virtual int operator == ( const domain_s & ) const;
    //virtual int operator != ( const domain_s & ) const;
    //virtual int operator >  ( const domain_s & ) const;
    //virtual int operator <  ( const domain_s & ) const;
    
    friend int _Export operator == ( const char *s1, const istring &s2 );
    friend int _Export operator == ( const istring &s1, const char *s2 );
#endif



    };

//int _Export operator == ( const char *, const istring & );
//int _Export operator == ( const istring &, const char * );

//int _Export operator == ( const istring &, const string & );
//int _Export operator != ( const istring &, const string & );
//int _Export operator >  ( const istring &, const string & );
//int _Export operator <  ( const istring &, const string & );


class domain_s : public string
    {
protected:
    virtual srtti rtti() const { return srtti_domain; }

public:
    domain_s();
    domain_s( const string &s );
    domain_s( const domain_s &s );
    domain_s( const char *s );

    //const char * Wild = "*";
    
    bool is_wild() const;
    
#if 0
    //virtual int operator == ( const domain_s & ) const;
    //virtual int operator != ( const domain_s & ) const;
    //virtual int operator <  ( const domain_s & ) const;
    /*
    virtual int operator == ( const string & ) const;
    virtual int operator != ( const string & ) const;
    virtual int operator <  ( const string & ) const;
    */

    virtual int operator == ( const string &   ) const;
    virtual int operator != ( const string &   ) const;
    virtual int operator >  ( const string &   ) const;
    virtual int operator <  ( const string &   ) const;
    
    virtual int operator == ( const istring &  ) const;
    virtual int operator != ( const istring &  ) const;
    virtual int operator >  ( const istring &  ) const;
    virtual int operator <  ( const istring &  ) const;
    
    virtual int operator == ( const domain_s & ) const;
    virtual int operator != ( const domain_s & ) const;
    virtual int operator >  ( const domain_s & ) const;
    virtual int operator <  ( const domain_s & ) const;
    //friend int operator == ( const char *,     const domain_s & );
    //friend int operator == ( const domain_s &, const char * );
    //friend int operator == ( const domain_s &, const domain_s & );
    //friend int operator != ( const char *,     const domain_s & );
    //friend int operator != ( const domain_s &, const char * );
    //friend int operator != ( const domain_s &, const domain_s & );
#endif

    };




  // ------------------------------------- Inlines ------------------------------------------------------

inline string::sdata::sdata( int alen )
    {
    if( (len=alen) < 0 ) len = 0;
    v = new char[ len+1 ]; zdie(v);
    *v = 0;
    refs = 1;
    }
        
inline string::sdata::sdata( const sdata &i )
    {
    len = i.len;
    v = new char[ len+1 ]; zdie(v);
    strncpy( v, i.v, len+1 );
    refs = 1;
    }
        
inline string::sdata::~sdata()
    {
    delete [] v;
      /* free( this ); */
    }

inline void string::sdata::newlength() { len = strlen( v ); }
inline void string::sdata::attach() { refs++; }


  // ------------------------------------- Inlines ------------------------------------------------------

inline void string::detach()
    {
    (d->refs)--;
    if( (d->refs) <= 0 )
        delete d;
    }

inline void string::COW() // copy on write
    {
    if( d->refs == 1 ) return;  // I own it, so no need to copy
      // get my own copy of sdata
    sdata *newd = new sdata(*d); zdie(newd);
    detach();
    d = newd;
    }
    
inline void string::newdata(int size)
    {
    d = new sdata(size); zdie(d);
    }

        
inline string::string( sdata *ss ) : d(ss) {}
inline string::~string() { detach(); }


inline void string::newlength() { d->newlength(); }

inline void string::lcase() // convert to lower case
    { COW(); strlwr( d->v ); }

inline void string::ucase()
    { COW(); strupr( d->v ); }




  // ------------------------------------- Inlines ------------------------------------------------------

inline string::srtti max_rtti( string::srtti a, string::srtti b )
    {
    if( a == string::srtti_domain || b == string::srtti_domain ) return string::srtti_domain;
    if( a == string::srtti_istring || b == string::srtti_istring ) return string::srtti_istring;
    return string::srtti_string;
    }

inline int string::operator == ( const string &r ) const
    {
    switch( max_rtti(rtti(),r.rtti()) )
        {
        default:
        case srtti_string:    return string_eq( *this, r );
        case srtti_istring:   return istring_eq( *this, r );
        case srtti_domain:    return domain_eq( *this, r );
        }
    }
    
inline int string::operator != ( const string &r ) const
    {
    switch( max_rtti(rtti(),r.rtti()) )
        {
        default:
        case srtti_string:    return !string_eq( *this, r );
        case srtti_istring:   return !istring_eq( *this, r );
        case srtti_domain:    return !domain_eq( *this, r );
        }
    }

inline int string::operator >  ( const string &r ) const
    {
    switch( max_rtti(rtti(),r.rtti()) )
        {
        default:
        case srtti_string:    return string_lt( r, *this );
        case srtti_istring:   return istring_lt( r, *this );
        case srtti_domain:    return domain_lt( r, *this );
        }
    }

inline int string::operator <  ( const string &r ) const
    {
    switch( max_rtti(rtti(),r.rtti()) )
        {
        default:
        case srtti_string:    return string_lt( *this, r );
        case srtti_istring:   return istring_lt( *this, r );
        case srtti_domain:    return domain_lt( *this, r );
        }
    }

inline int string::operator >= ( const string &r ) const
    {
    return (*this == r) || (*this > r);
    }

inline int string::operator <= ( const string &r ) const
    {
    return (*this == r) || (*this < r);
    }


inline int operator == ( const char *s1, const istring &s2 ) { return string(s1).operator == (s2); }
inline int operator == ( const istring &s1, const char *s2 ) { return s1.operator == (string(s2)); }
inline int operator == ( const istring &s1, const istring &s2 ) { return s1.operator == (string(s2)); }

inline int operator == ( const char *s1, const string &s2 ) { return string(s1).operator == (s2); }
inline int operator == ( const string &s1, const char *s2 ) { return s1.operator == (string(s2)); }


#endif
@


1.12
log
@new string/istring/domain_s comparison ops
@
text
@d8 3
d44 1
d113 1
a113 1
    
d181 12
d194 1
a194 1
    };
@


1.11
log
@Version 25
@
text
@d8 3
d47 4
d62 2
a63 2
        friend int _Export operator == ( const char *, const string & );
        friend int _Export operator == ( const string &, const char * );
d72 5
a76 28
        void attach() { refs++; }
#if 0
        void detach() { refs--; if( refs <= 0 ) (*this).~sdata(); }
#endif
    
        sdata( int alen )
            {
            if( (len=alen) < 0 ) len = 0;
            v = new char[ len+1 ]; zdie(v);
            *v = 0;
            refs = 1;
            }
        
        sdata( const sdata &i )
            {
            len = i.len;
            v = new char[ len+1 ]; zdie(v);
            strncpy( v, i.v, len+1 );
            refs = 1;
            }
        
        virtual ~sdata()
            {
            delete [] v;
            /* free( this ); */
            }
        
        void newlength() { len = strlen( v ); }
d80 1
a80 3
    //    char	*v;
    
    sdata   *d;
d82 4
a85 6
    void detach() 
        { 
        (d->refs)--; 
        if( (d->refs) <= 0 ) 
            delete d; 
        }
d87 2
a88 13
    void COW() // copy on write
        {
        if( d->refs == 1 ) return;  // I own it, so no need to copy
        // get my own copy of sdata
        sdata *newd = new sdata(*d); zdie(newd);
        detach();
        d = newd;
        }
    
    void newdata(int size)
        {
        d = new sdata(size); zdie(d);
        }
a89 4
        
    string( sdata *ss ) : d(ss) {}
    
    
d91 4
a94 4
    string( void );
    string( const char * );
    string( const unsigned char * );
    string( const string & );
d96 1
a96 1
    virtual ~string() { detach(); }
a97 1
    //	inline const char * c_str() const { return v; }
d100 2
a101 2
    int	copy( char *cb, int n );
    int	copy( char *cb, int n, int pos );
d103 1
a103 1
    int	length() const { return d->len; }
d105 1
a105 1
    int	is_empty() const { return length() == 0; }
d107 2
a108 2
    void	load( FILE *fp );
    void	save( FILE *fp ) const;
d110 5
a114 5
    void    strip_crlf();
    void    strip_ws();
    void	strip_leading_ws();
    void	strip_leading( int n ); // Just delete leading n characters
    void    strip( int pos, int len  );
d116 1
a116 1
    string	substr( int pos, int len = 0 ) const;
d118 5
a122 2
    int /*__stdcall*/ insert( const string *s0, const string *s1 );
    int         print( const char *fmt, ... );
d124 3
a126 5
    void    recode_upper_128( const char *tab );
    void    lcase() // convert to lower case
        { COW(); strlwr( d->v ); }
    void    ucase()
        { COW(); strupr( d->v ); }
d128 1
a128 1
    bool same( const string &s ) const { return d == s.d ? Yes : No; }
d130 2
a131 2
    friend int operator == ( const char *, const string & );
    friend int operator == ( const string &, const char * );
d133 9
a141 4
    virtual int operator == ( const istring &s ) const;
    virtual int operator != ( const istring &s ) const;
    virtual int operator >  ( const istring &s ) const;
    virtual int operator <  ( const istring &s ) const;
d143 9
a151 4
    virtual int operator == ( const string & ) const;
    virtual int operator != ( const string & ) const;
    virtual int operator >  ( const string & ) const;
    virtual int operator <  ( const string & ) const;
d153 2
a154 2
    string & operator =  ( const string & );
    string & operator =  ( const char *s );
d156 2
a157 2
    string & operator += ( const string & );
    string   operator +  ( const string & ) const;
d160 1
a160 1
    operator const char*          ( ) const;
d162 2
a163 2
    operator const unsigned char* ( ) const;
    operator const signed char*   ( ) const;
d165 2
a166 1
    char &operator []             ( int i ) const;
d170 7
a177 2
    bool        parse( string &out1, char c, string &out2 ) const;
    bool        parse( string &out1, string &out2 ) const; // delimiter - space
d180 2
d184 2
a185 2
int _Export operator == ( const char *, const string & );
int _Export operator == ( const string &, const char * );
a192 2
string _Export operator + ( const string &, const char * );
string _Export operator + ( const char *, const string & );
d202 3
d206 6
a211 2
    friend int operator == ( const char *, const istring & );
    friend int operator == ( const istring &, const char * );
d214 52
a267 1
    virtual int operator >  ( const string & ) const;
d269 22
d292 51
d344 62
a405 2
    friend int _Export operator == ( const char *s1, const istring &s2 );
    friend int _Export operator == ( const istring &s1, const char *s2 );
d407 10
d418 4
d423 4
a426 1
    };
a427 2
int _Export operator == ( const char *, const istring & );
int _Export operator == ( const istring &, const char * );
d429 3
a431 4
int _Export operator == ( const istring &, const string & );
int _Export operator != ( const istring &, const string & );
int _Export operator >  ( const istring &, const string & );
int _Export operator <  ( const istring &, const string & );
d433 2
@


1.10
log
@Exceptions added, not compiled
@
text
@d8 3
d43 2
d168 5
d217 30
@


1.9
log
@Before exceptions.
@
text
@d8 3
d138 2
a139 2
    bool	load( FILE *fp );
    bool	save( FILE *fp ) const;
d185 2
@


1.8
log
@state save
@
text
@d8 3
@


1.7
log
@C Set & STL support
@
text
@d8 3
d30 1
a30 1
#ifdef __WATCOMC__
d37 1
a37 1
    static void zdie( const void *s ) { if( s==0 ) { printf("Out of memory!\n"); exit( 34 ); } }
d139 1
d144 1
d167 2
a168 1
    
d170 1
@


1.6
log
@beta 14
@
text
@d8 3
d139 1
a139 1
    int __stdcall insert( const string *s0, const string *s1 );
@


1.5
log
@Beta 12
@
text
@d7 4
a10 1
 *      $Log:$
d187 2
a188 2

#define String string
@


1.4
log
@Fixed memory hog in operator =
@
text
@d1 11
d37 3
a39 3
        friend int _Export operator == ( const string &, const string & );
        friend int _Export operator > ( const string &s1, const string &s2 );
        friend int _Export operator < ( const string &s1, const string &s2 );
d66 1
a66 1
        ~sdata()
d111 1
a111 1
    ~string() { detach(); }
d146 4
a149 4
    friend int operator == ( const string &, const string & );
    friend int operator != ( const string &, const string & );
    friend int operator >  ( const string &, const string & );
    friend int operator <  ( const string &, const string & );
d173 1
a173 1
int operator != ( const string &a, const string &b ) { return !(a == b); };
d176 2
a177 2
int _Export operator < ( const string &, const string & );
int _Export operator > ( const string &, const string & );
@


1.3
log
@before COW strings
@
text
@d15 7
a21 6
	{
	char	*v;

	inline void f() { if( v != NULL ) free( v ); v = NULL; }
//	inline void c() { /* if( v == NULL ) v = ""; */ }

d23 10
a32 26
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
	bool	save( FILE *fp ) const;

	void    strip_crlf();
	void    strip_ws();
	void	strip_leading_ws();
	void	strip_leading( int n ); // Just delete leading n characters

	string	substr( int pos, int len = 0 ) const;
d34 1
a34 3
        int __stdcall insert(
                             const string *s0 /*= string()*/,
                             const string *s1 /*= string()*/
d36 1
a36 8
                             const string &s2 /*= string()*/,
                             const string &s3 /*= string()*/,
                             const string &s4 /*= string()*/,
                             const string &s5 /*= string()*/,
                             const string &s6 /*= string()*/,
                             const string &s7 /*= string()*/,
                             const string &s8 /*= string()*/,
                             const string &s9 /*= string()*/
d38 16
a53 1
                            );
d55 34
a88 8
        void    recode_upper_128( const char *tab );
        void    lcase() // convert to lower case
                { if( v != 0 ) strlwr( v ); }
        void    ucase()
                { if( v != 0 ) strupr( v ); }

	friend int operator == ( const char *, const string & );
	friend int operator == ( const string &, const char * );
a89 17
	friend int operator == ( const string &, const string & );
	friend int operator != ( const string &, const string & );
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
d91 65
a155 1
        };
@


1.2
log
@beta 7
@
text
@d43 1
d50 2
a51 1
                             const string *s0 /*= string()*/, ... );
a52 1
                             const string &s1 /*= string()*/,
d61 1
a62 1
#endif
@


1.1
log
@Initial revision
@
text
@d47 16
a62 1

@
