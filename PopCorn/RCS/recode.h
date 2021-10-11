head	1.1;
access;
symbols
	Ver_075:1.1;
locks;
comment	@ * @;


1.1
date	96.09.17.23.20.43;	author dz;	state Exp;
branches;
next	;


desc
@@


1.1
log
@Initial revision
@
text
@/************************ PopCorn server ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	:	Code table convertor header
 *
 *      $Log: Recode.h $
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

inline string code_to_koi8( string code = "CP866" ) { return code+"-KOI8"; }
inline string code_from_koi8( string code = "CP866"  ) { return "KOI8-"+code; }

//#pragma pack()

#endif

@
