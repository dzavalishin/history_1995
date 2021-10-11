/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	String
 *
 *      $Log: strng.h $
 *      Revision 1.13  1997/03/28 22:04:16  dz
 *      ostream <<
 *
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
