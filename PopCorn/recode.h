/************************ PopCorn server ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	:	Code table convertor header
 *
 *      $Log: recode.h $
 *      Revision 1.1  1996/09/17 23:20:43  dz
 *      Initial revision
 *
 *
 *
 *
 *
\*/


#ifndef	RECODE_H
#define	RECODE_H

#include <style.h>

//#pragma pack(1)

class _Export recoder
    {
    private:
        char	*tab;

    public:
        recoder( const string & tabname );

        uchar	rc( uchar );					// Recode character
        void	rs( uchar * );                 // Recode string
        void    recoder::rs( string &s );      // Recode string
    };

//inline string code_to_koi8( string code = "CP866" ) { return code+"-KOI8"; }
//inline string code_from_koi8( string code = "CP866"  ) { return "KOI8-"+code; }

inline string code_to_koi8( string code = "CP1251" ) { return code+"-KOI8"; }
inline string code_from_koi8( string code = "CP1251"  ) { return "KOI8-"+code; }

//#pragma pack()

#endif

