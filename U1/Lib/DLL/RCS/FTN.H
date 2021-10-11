head	1.2;
access;
symbols;
locks;
comment	@ * @;


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


1.2
log
@debug
@
text
@/************************ UUCP to FIDO convertor ***************************\
 *
 *  Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Setup: FTN array
 *
 *      $Log: FTN.H $
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *
 *
 *
\*/



#ifndef FTN_H
#define FTN_H

#include "strng.h"


#include "fidoaddr.h"

//#include  <uu2dir.h>
#include    <strng.h>

#pragma pack(1)


struct ftn_def
    {
    String      shortname;          // Say, fidonet
    String      fullname;           // Say, fidonet.org, taken from next el.
    fido_addr   fido_a;             // Our address in fido
    String      uucp_d;             // Internet domain for this address
    uint        min_zone;           // Lowest zone number for this ftn
    uint        max_zone;           // Highest zone number ---""---
    };


const int   _max_ftn = 20;

class _Export ftn
    {

    static ftn_def      ftn_v[_max_ftn];
    static int          ftn_count;      // How many slots used in FTN array

public:
    static bool                 add_def( const char *def ); // Parse definition
    static bool                 valid();        // Can we continue?


    // Get default FTN
    static const ftn_def &      def();

    // Find AKA matching to given address, or default
    static const fido_addr      match_aka( const fido_addr &his_a );

    // Find FTN record by FIDO or UUCP address. Inexact comparison.
    static bool                 match( ftn_def &out, const fido_addr &in );
    static bool                 match( ftn_def &out, const uucp_addr &in );

    // Find FTN record by FIDO or UUCP address. Exact comparison.
    static bool                 find( ftn_def &out, const fido_addr &in );
//  static bool                 find( ftn_def &out, const uucp_addr &in );
    };


#pragma pack()

#endif

@


1.1
log
@Initial revision
@
text
@d7 3
a9 1
 *      $Log:   Q:/lib/uu2/vcs/setup.h_v  $
d11 2
d15 2
d48 1
a48 1
class ftn
@
