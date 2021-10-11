/************************ U1 ***************************\
 *
 *  Copyright (C) 1991-1995 by Infinity Soft
 *
 *	Module 	:	Headline Class
 *
 *      $Log: hl.h $
 *      Revision 1.2  1995/11/02 14:25:20  dz
 *      Before switching to rfc_msg
 *
 *      Revision 1.1  1995/08/14 02:59:23  dz
 *      Initial revision
 *
 *
 *
\*/

#ifndef HL_H
#define HL_H

#include "strng.h"
#include <ieqseq.h>

typedef IEqualitySequence<string>  hl_val_Bag;
typedef hl_val_Bag::Cursor         hl_val_Cursor;


class hl	// headLine
	{
        string                  key_v;
        hl_val_Bag              val_v;


public:
                                hl(void) {};
                                hl(const string &to_parse);
                                hl(const string &key, const string &val);
                                hl(const hl &h) : key_v(h.key_v), val_v(h.val_v) {};
                               ~hl() {}

        void                    key( const string &k) { key_v = k; }
//	void                    val( const string &v) { val_v = v; }
        void                    val( const hl_val_Bag &v) { val_v = v; }
        void                    add_val( const string &v) { val_v.add(v); }

        const string &          key() const { return key_v; }
        const hl_val_Bag &      val() const { return val_v; }

        int                     operator == (const hl &r) const;
//      int                     operator == (const hl &r) const {  return key_v==r.key_v && val_v==r.val_v; }
//	int operator >  (const hl &r) const {  return key_v > r.key_v; }
//	int operator <  (const hl &r) const {  return key_v < r.key_v; }

        hl &                    operator = (const hl &h) { key_v = h.key_v; val_v = h.val_v; return *this; };

//	operator string   ( ) const { return key_v + string(": ") + val_v; }
	};


typedef IEqualitySequence<hl>      hl_Bag;
typedef hl_Bag::Cursor             hl_Cursor;


#endif

