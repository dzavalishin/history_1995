/************************ UUCP to FIDO convertor ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :       Header
 *
 *      $Log: CC_Bag.h $
 *      Revision 1.3  1995/08/08 13:22:34  dz
 *      App class used
 *
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
