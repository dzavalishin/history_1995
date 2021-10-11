/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Main
 *
 *      $Log: frip.h $
 *      Revision 1.19  1997/03/05 18:12:38  dz
 *      Ver. 34
 *
 *      Revision 1.18  1997/01/21 08:53:51  dz
 *      EXT_ROUTER off
 *
 *      Revision 1.17  1997/01/07 10:02:56  dz
 *      Ver 29
 *
 *      Revision 1.16  1997/01/02 10:06:18  dz
 *      RIFRIZ capability
 *
 *      Revision 1.15  1996/12/28 03:11:55  dz
 *      new string/istring/domain_s comparison ops
 *
 *      Revision 1.13  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.12  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.11  1996/07/31 07:00:23  dz
 *      FripMail written
 *
 *      Revision 1.10  1996/07/22 02:48:05  dz
 *      ver 17
 *
 *      Revision 1.9  1996/07/20 23:24:50  dz
 *      state save
 *
 *      Revision 1.8  1996/01/22 05:03:34  dz
 *      C Set fixes
 *
 *      Revision 1.7  1996/01/22 04:36:41  dz
 *      C Set & STL support
 *
 *      Revision 1.6  1996/01/14 01:06:31  dz
 *      beta 14
 *
 *      Revision 1.5  1996/01/13 09:48:05  dz
 *      *** empty log message ***
 *
 *      Revision 1.4  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/


#ifndef FRIP_H
#define FRIP_H



// RIF means we can receive RIF mail
// RIFRIZ means we will unpack RIF from RIZ

  // can't turn more than one capa now - capas hadling code is not correct in versiobs up to 29
  // lets wait until 29 will be more popular
//#define FRIP_CAPAS "RIF RIFRIZ"
#define FRIP_CAPAS "RIF"

#define USE_STL   1

// generate external routing table. Is not supported any more.
#define EXT_ROUTER 0

#define TESTALLOC 1
#define DUMPALLOC 0

#if TESTALLOC
void heap_dump(const char *);
#define __HD__(A) heap_dump(A)
#else
#define __HD__(A)
#endif


#ifdef __IBMCPP__
#  define INCL_DOSFILEMGR
#  define INCL_DOSERRORS
#endif


#include <io.h>
#include <stdio.h> 
#ifndef __GNUC__
#  include <direct.h>
#endif
#include <fstream.h>

#include <vector.h>
#include <time.h>

#include "app.h"
#include "style.h"


#ifdef __IBMCPP__
#  include <os2.h>
#endif


  // Exits

enum exit_code
    {
    Exit_Hard_Fail      = 128,
    Exit_Fail           =  64,
    Exit_Config         =  32,
    Exit_Exception      =  16,
    Exit_Packer_Error   =   8,
    Exit_Cmdline_Error  =   4,
    Exit_Cmd_Error      =   2,
    Exit_Ok             =   0
    };

void add_exit_code( exit_code e );


  // OS Type

class OS_Type
    {
    public:
        OS_Type();
        enum ot { OS2, Unix, DOS, Win32_95, Win32_NT };

    private:
        ot   v;
        char *name_v;

    public:

        ot   type()            { return v; }
        const char *name()     { return name_v; }

        bool is_nt()           { return v == Win32_NT ? Yes : No; }
        bool is_95()           { return v == Win32_95 ? Yes : No; }
        
    };


extern OS_Type os_type;

//void main_loop();
bool our_aka_matches( const addr &a );
bool is_our_aka( const addr &a );

void parse_config();
//void create_routes();

//void send_rips( const rip &r );
//void load_links();

bool need_announce();
void announce();

bool need_ping();
void mark_ping();
void ping();


string tempFileName( const string &mask );
void tempFile( fstream &f, string &outname, const string &mask, int mode );



void   BinkAttach(  const string &file, const addr &to, bool hold );
void   HPFSAttach(  const string &file, const addr &to, bool hold );
void   TMailAttachFAT( const string &file, const addr &to, bool hold );
void   TMailAttachHPFS( const string &file, const addr &to, bool hold );


void deliver( const addr &a, const string &fn_a );
//void ZipList_add( const string &ripfile, const addr &dest );
//void ZipList_send();





extern volatile bool Stop_Frip;       // Cancel all activities ASAP!



  // file copy/move, etc

void frip_copy( string from, string to );
void frip_rename( string from, string to );
void frip_sleep( int sec );

  // file descriptions

string get_desc( string file );
void set_desc( string file, string desc );

  // file names

string newdir( const char *name, string dir );
void mkpath( const string &path );


  // OS/2 EAs
  // do nothing ifndef __OS2__. Anybody wants to implement it for NT?
void set_ea_ASCII( const char *fn, const char* ea_name, string data );
void set_ea_MVMT_ASCII( const char *fn, const char* ea_name, const vector <string> &data );
void set_ea_MVST_ASCII( const char *fn, const char* ea_name, const vector <string> &data );


  // rip processing

void do_off( rip &r );

  // FARA

//void frip_do_send( string name, addr dest, bool kill, rif_type t, string flags );
//void eat_mail_in( const string &dir );





#endif // FRIP_H
