/************************ UUCP to FIDO convertor ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :       Header
 *
 *      $Log: RFC_Bags.h $
 *      Revision 1.1  1995/11/02 14:25:20  dz
 *      Initial revision
 *
 *      Revision 1.1  1995/08/14 02:59:23  dz
 *      Initial revision
 *
 *
 *      
 *
 *
\*/

#ifndef RFC_BAGS
#define RFC_BAGS

#include <ieqseq.h>

//typedef IBagOnSortedLinkedSequence<hl>      hl_Bag;
//typedef hl_Bag::Cursor                      hl_BagIterator;

typedef IEqualitySequence<string>           txt_Bag;
typedef txt_Bag::Cursor                     txt_Cursor;


typedef IEqualitySequence<string>           env_from_Bag;
typedef env_from_Bag::Cursor                env_from_Cursor;

typedef IEqualitySequence<string>           env_to_Bag;
typedef env_to_Bag::Cursor                  env_to_Cursor;





#endif
