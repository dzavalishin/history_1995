head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	95.08.08.13.22.34;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.12.21.20.45;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.24.01;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@App class used
@
text
@/************************ UUCP to FIDO convertor ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :       Header
 *
 *      $Log: CC_Bag.h $
 *      Revision 1.2  1995/04/12  21:20:45  dz
 *      IBM C Set version seems to be working...
 *
 *      Revision 1.1  1995/03/11  18:24:01  dz
 *      Initial revision
 *
 *      
 *
 *
\*/

#ifndef CC_BAG
#define CC_BAG

#ifdef __BORLANDC__
#include <bags.h>


typedef TBagAsVector<string>		CC_Bag;
typedef TBagAsVectorIterator<string>	CC_BagIterator;
#endif

#ifdef __IBMCPP__
#include <ibagsls.h>


//typedef IBag<string>		CC_Bag;

//typedef IBagOnSortedTabularSequence<string>		CC_Bag;
typedef IBagOnSortedLinkedSequence<string>		CC_Bag;

//typedef IBag<string>::Cursor	CC_BagIterator;

typedef CC_Bag::Cursor	CC_BagIterator;

#endif



#endif
@


1.2
log
@IBM C Set version seems to be working...
@
text
@d8 5
a12 2
 * Revision 1.1  1995/03/11  18:24:01  dz
 * Initial revision
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:    $
d19 1
d25 17
@
