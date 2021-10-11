head	1.1;
access;
symbols;
locks;
comment	@ * @;


1.1
date	97.01.01.16.08.42;	author dz;	state Exp;
branches;
next	;


desc
@@


1.1
log
@Initial revision
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Persistent storage classes
 *
 *      $Log: app.h $
 *
 *      
 *      
\*/

#ifndef PERS_H
#define PERS_H

#include "style.h"

class persistant_storage
    {
    public:
        class key
            {
            key( string name, string type, int size ) :
            name_v(name), type_v(type), size_v(size)
                {}
            
            string    name_v;
            string    type_v;
            int       size_v;

            friend class persistant_storage;
            };
        
    public:
        persistant_storage( string fname );
        ~persistant_storage();

        void      get( key what, void *buf );
        void      put( key what, const void *buf );

        void      clear( key what );

        void      flush();
    };



class persistent_type
    {
    private:
        bool                      must_load;
        bool                      must_save;

        persistant_storage::key   the_key;
        persistant_storage        storage;

    protected:
        
        persistent_type(string name, string type, int size, persistant_storage &pst )
            :
        the_key( name, type, size ), storage(pst)
            {
            must_load = Yes;
            must_save = No;
            }

        ~persistent_type() { save(); }

        void load() = 0;
        void load( void *val )
            {
            if(!must_load) return;
            storage.get( the_key, val );
            must_load = No;
            }
        
        void save() = 0;
        void save( const void *val )
            {
            if(!must_save) return;
            storage.put( the_key, val );
            must_save = No;
            }

        void need_save() { must_save = Yes; }
        
    public:
        void      flush();
    };

  //
  // Four-byte integer
  //

class persistent_int : public persistent_type
    {
    private:
        int       value;
        
    public:
        persistent_int( conat char *name, persistant_storage &pst  )
            :
        persistent_type( name, "int4", 4, pst )
            {
            load(&value);
            }

        ~persistent_int()
            {
            save(&value);
            }
        
        int operator = ( int val )
            {
            value = val;
            need_save();
            return val;
            }

        operator int () const
            {
            return value;
            }

        void set( int val )
            {
            value = val;
            need_save();
            }

        int get() const
            {
            return value;
            }
    };








#endif // PERS_H

@
