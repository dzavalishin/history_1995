/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	String
 *
 *      $Log: Strng.H $
 *      Revision 1.2  1996/07/07 10:01:14  dz
 *      *** empty log message ***
 *
 *      Revision 1.1  1996/06/30 22:58:25  dz
 *      Initial revision
 *
 *      Revision 1.5  1996/06/14 04:36:51  dz
 *      print
 *
 *      Revision 1.4  1996/06/03 20:55:02  dz
 *      Before debugging
 *
 *      Revision 1.3  1996/05/24 20:38:18  dz
 *      temp
 *
 *      Revision 1.2  1996/05/22 19:46:04  dz
 *      compiled
 *
 *      Revision 1.1  1996/05/17 13:25:40  dz
 *      Initial revision
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
#include <stdarg.h>

//#include <compiler.h>

#ifndef __cplusplus
#error Need C++
#endif

#define ISTRING 0

#if ISTRING
class istring;
#endif

class _Export string
    {
    public:
        static void zdie( const void *s );

    private:
        class sdata
            {
            public:
                friend class string;
                friend int _Export operator == ( const char *, const string & );
                friend int _Export operator == ( const string &, const char * );

                char	*v;
                int     refs;     // references
                int     len;      // length of DATA (not buffer) without trailing zero
        
                void attach() { refs++; }

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
                    }
        
                void newlength() { len = strlen( v ); }
            }; // end of class sdata def.
    
    
        sdata   *d;
    
        void detach()
            {
            (d->refs)--;
            if( (d->refs) <= 0 )
                delete d;
            }

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

        
        string( sdata *ss ) : d(ss) {}


    public:
        string( void );
        string( const char * );
        string( const unsigned char * );
        string( const string & );

        virtual ~string() { detach(); }

        const char * c_str() const;

        //int	copy( char *cb, int n );
        //int	copy( char *cb, int n, int pos );

        int	length() const { return d->len; }

        int	is_empty() const { return length() == 0; }

        void	load( FILE *fp );
        void	save( FILE *fp ) const;

        void    strip_crlf();
        void    strip_ws();
        void	strip_leading_ws();
        void	strip_leading( int n ); // Just delete leading n characters
        
        void    strip( int pos, int len = 1 ); // Cut off characters
        
        void    clear() { detach(); newdata(0); }
        
        string	substr( int pos, int len = 0 ) const;

        int     insert( const string *s0, const string *s1 );
        
        int     print( const char *fmt, ... );

        
        void    recode_upper_128( const char *tab );
        void    lcase() // convert to lower case
            { COW(); strlwr( d->v ); }
        void    ucase()
            { COW(); strupr( d->v ); }

        bool    parse( string &out1, string &out2 ) const;
        bool    parse( string &out1, char c, string &out2 ) const;

        bool same( const string &s ) const { return d == s.d ? Yes : No; }

#if ISTRING
        virtual int operator == ( const istring &s ) const;  // { return s.operator == (*this);}
        virtual int operator != ( const istring &s ) const;  // { return s.operator != (*this);}
        virtual int operator >  ( const istring &s )  const; // { return s.operator < (*this);}
        virtual int operator <  ( const istring &s )  const; // { return s.operator > (*this);}
#endif
        
        virtual int operator == ( const string & ) const;
        virtual int operator != ( const string & ) const;
        virtual int operator >  ( const string & ) const;
        virtual int operator <  ( const string & ) const;

        friend int operator == ( const char *, const string & );
        friend int operator == ( const string &, const char * );

        string & operator =  ( const string & );
        string & operator =  ( const char *s );

        string & operator += ( const string & );
        string   operator +  ( const string & ) const;

#ifndef __BORLANDC__
        operator const char*          ( ) const;
#endif
        operator const unsigned char* ( ) const;
        operator const signed char*   ( ) const;

        char &operator []             ( int i ) const;

        friend string _Export operator + ( const string &, const char * );
        friend string _Export operator + ( const char *, const string & );

    };



int _Export operator == ( const char *, const string & );
int _Export operator == ( const string &, const char * );

string _Export operator + ( const string &, const char * );
string _Export operator + ( const char *, const string & );



#if ISTRING
// case Insensitive string
class _Export istring : public string
    {
    friend int operator == ( const char *, const istring & );
    friend int operator == ( const istring &, const char * );
    
    // a lot of warnings!
#if 0
    virtual int operator == ( const string & ) const;
    virtual int operator != ( const string & ) const;
    virtual int operator >  ( const string & ) const;
    virtual int operator <  ( const string & ) const;
#endif
    };

int _Export operator == ( const char *, const istring & );
int _Export operator == ( const istring &, const char * );
#endif



#endif
