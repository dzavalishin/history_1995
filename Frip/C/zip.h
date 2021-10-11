/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Compression/expansion interface
 *
 *      $Log: zip.h $
 *      Revision 1.1  1996/09/08 19:05:31  dz
 *      Initial revision
 *
 *
 *
\*/

#ifndef ZIP_H
#define ZIP_H

//#include "frip.h"
//#include <process.h>

#include "string.h"
#include "addr.h"
#include <vector.h>

class RipZip
    {
    string                         zipfile_v;
    vector            <string>     ripnames_v;
    addr                           dest_v;
    
    public:
        
        RipZip() {}
        RipZip( const addr & );
        
        
        void                           add( const string &rip_name )
            {
            ripnames_v.push_back( rip_name );
            }
        void                           zip();
        void                           do_zip();
        const string &                 name() const { return zipfile_v; }
        const addr &                   dest() const { return dest_v;    }
        
        int operator ==                ( const RipZip &r ) const
            {
            return dest_v == r.dest_v && ripnames_v == r.ripnames_v
                && zipfile_v == r.zipfile_v;
            }
    };


class zip_list
    {
    private:
        vector  <RipZip>  zips_v;
        
    public:
        void              add( const string &ripfile, const addr &dest );
        void              send();
        
        ~zip_list() { send(); }
        
        
    };



#endif // ZIP_H

