head	1.19;
access;
symbols
	Beta_12:1.4;
locks;
comment	@ *      @;


1.19
date	97.03.05.18.12.38;	author dz;	state Exp;
branches;
next	1.18;

1.18
date	97.01.21.08.53.51;	author dz;	state Exp;
branches;
next	1.17;

1.17
date	97.01.07.10.02.56;	author dz;	state Exp;
branches;
next	1.16;

1.16
date	97.01.02.10.06.18;	author dz;	state Exp;
branches;
next	1.15;

1.15
date	96.12.28.03.11.55;	author dz;	state Exp;
branches;
next	1.14;

1.14
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.13;

1.13
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.12;

1.12
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	96.07.31.07.00.23;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.07.22.02.48.05;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.07.20.23.24.50;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.01.22.05.03.34;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.01.22.04.36.41;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.01.14.01.06.31;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.01.13.09.48.05;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.12.05.04.23.40;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.11.16.00.54.44;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.11.12.13.12.27;	author dz;	state Exp;
branches;
next	;


desc
@@


1.19
log
@Ver. 34
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Main
 *
 *      $Log: frip.h $
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
@


1.18
log
@EXT_ROUTER off
@
text
@d8 3
d130 2
d169 1
a169 1
  // file copy/move
d173 1
@


1.17
log
@Ver 29
@
text
@d8 3
d67 2
@


1.16
log
@RIFRIZ capability
@
text
@d8 3
a52 1
//#define FRIP_CAPAS "RIF"
d57 4
a60 1
#define FRIP_CAPAS "RIF RIFRIZ"
@


1.15
log
@new string/istring/domain_s comparison ops
@
text
@d8 3
d50 6
a55 1
#define FRIP_CAPAS "RIF"
@


1.14
log
@Version 25
@
text
@@


1.13
log
@Exceptions added, not compiled
@
text
@d8 3
a45 1
#include "version.h"
d79 2
a80 8
#include "style.h"
#include "strng.h"
#include "addr.h"
#include "rip.h"
#include "rif.h"
#include "conf.h"
#include "base.h"
#include "links.h"
d107 1
a107 1
void main_loop();
d110 1
a110 1
void create_routes();
d112 2
a113 2
void send_rips( const rip &r );
void load_links();
d124 1
d135 2
a136 1
void ZipList_add( const string &ripfile, const addr &dest );
d153 1
a153 1
void get_desc( string file, string &desc );
d175 2
a176 2
int frip_do_send( string name, addr dest, bool kill, rif_type t, string flags );
void eat_mail_in( const string &dir );
@


1.12
log
@Before exceptions.
@
text
@d8 3
d91 18
a108 3
bool parse( const string &in, string &out1, char c, string &out2 );
bool parse( const string &in, string &out1, string &out2 ); // delimiter - space
//bool match_addr( addr a1, addr a2 );
d111 1
a111 1
bool main_loop();
d113 2
a114 2
bool parse_config();
bool create_routes();
d116 2
a117 2
bool send_rips( const rip &r );
bool load_links();
d120 1
a120 1
bool announce();
d123 2
a124 2
bool mark_ping();
bool ping();
a128 1
bool deliver( const addr &a, const string &fn_a );
d131 4
a134 4
bool   BinkAttach(  const string &file, const addr &to, bool hold );
bool   HPFSAttach(  const string &file, const addr &to, bool hold );
bool   TMailAttachFAT( const string &file, const addr &to, bool hold );
bool   TMailAttachHPFS( const string &file, const addr &to, bool hold );
d137 1
a138 2
bool ZipList_send();
bool unzip_all();
a140 12
class filer
    {
#ifdef __WATCOMC__
    DIR    *dirp;
    dirent *direntp;
#endif
#ifdef __IBMCPP__
    HDIR    h;
    bool    restart_flag;
    bool    valid_v;
    bool    opened_flag;
    string  result;
a141 2
    bool    do_restart( string &name );
    bool    do_continue( string &name );
a142 21
#endif
    string  mask;
    
    bool    fnmatch( const char *fn, const char *mask );
public:
    
#ifdef __WATCOMC__
    filer() { dirp = direntp = 0; }
#endif
#ifdef __IBMCPP__
    filer();
#endif
    filer( string dir, string mask);
    ~filer();
    
    bool    valid();
    void    next();
    
    string  current();
    
    };
d144 1
a145 43
class statistics
    {
    private:
        int      no_good_v;
        int      new_addr_v;
        int      better_repl_v;
        int      timeout_repl_v;
        int      filtered_v;
        int      rips_total_v;

        int      rifs_total_v;
        int      rifs_for_us_v;
        int      rifs_forw_v;
        int      rifs_held_v;

    public:
        statistics()
            {
            no_good_v = 0;
            new_addr_v = 0;
            better_repl_v = 0;
            timeout_repl_v = 0;
            filtered_v = 0;
            rips_total_v = 0;
            
            rifs_total_v = 0;
            rifs_for_us_v = 0; 
            rifs_forw_v = 0;   
            rifs_held_v = 0;
            }
        
        void     rip_no_good()      { no_good_v++; rips_total_v++; }
        void     rip_new_addr()     { new_addr_v++; rips_total_v++; }
        void     rip_better_repl()  { better_repl_v++; rips_total_v++; }
        void     rip_timeout_repl() { timeout_repl_v++; rips_total_v++; }
        void     rip_filtered()     { filtered_v++; rips_total_v++; }

        void     rif_for_us()       { rifs_for_us_v++; rifs_total_v++; }
        void     rif_forw()         { rifs_forw_v++; rifs_total_v++; }
        void     rif_held()         { rifs_held_v++; rifs_total_v++; }
        
    void     report();
    };
a146 3
extern statistics stats;
extern volatile bool Stop_Frip;       // Cancel all activities ASAP!
void Do_Stop_Frip();
d150 2
a151 2
bool frip_copy( string from, string to );
bool frip_rename( string from, string to );
d155 2
a156 2
bool get_desc( string file, string &desc );
bool set_desc( string file, string desc );
d166 3
a168 3
bool set_ea_ASCII( const char *fn, const char* ea_name, string data );
bool set_ea_MVMT_ASCII( const char *fn, const char* ea_name, const vector <string> &data );
bool set_ea_MVST_ASCII( const char *fn, const char* ea_name, const vector <string> &data );
d173 1
a173 1
bool do_off( rip &r );
d178 1
a178 1
bool eat_mail_in( const string &dir );
@


1.11
log
@FripMail written
@
text
@d8 3
d42 2
d241 1
a241 1
int frip_do_send( string name, addr dest, bool kill, rif_type t = rif_type::F );
@


1.10
log
@ver 17
@
text
@d8 3
d37 2
d65 2
a66 3
#ifndef USE_STL
#  include <wcvector.h>
#endif
d68 1
d73 1
d161 39
a199 21
    int      no_good_v;
    int      new_addr_v;
    int      better_repl_v;
    int      timeout_repl_v;
    int      filtered_v;
public:
    statistics()
        {
        no_good_v = 0;
        new_addr_v = 0;
        better_repl_v = 0;
        timeout_repl_v = 0;
        filtered_v = 0;
        }
    
    void     no_good()      { no_good_v++; }
    void     new_addr()     { new_addr_v++; }
    void     better_repl()  { better_repl_v++; }
    void     timeout_repl() { timeout_repl_v++; }
    void     filtered()     { filtered_v++; }
    
d216 25
@


1.9
log
@state save
@
text
@d8 3
d183 6
d190 2
@


1.8
log
@C Set fixes
@
text
@d8 3
d53 3
a55 1
#include <direct.h> 
@


1.7
log
@C Set & STL support
@
text
@d8 3
d113 9
a121 1
	HDIR		h;
@


1.6
log
@beta 14
@
text
@d8 3
d39 5
d49 3
a51 1
#include <wcvector.h>
d61 4
d105 1
d108 4
d117 6
@


1.5
log
@*** empty log message ***
@
text
@d8 3
d22 15
a139 9
#define TESTALLOC 1
#define DUMPALLOC 0

#if TESTALLOC
void heap_dump(const char *);
#define __HD__(A) heap_dump(A)
#else
#define __HD__(A)
#endif
@


1.4
log
@Beta 12
@
text
@d7 4
a10 1
 *      $Log:$
d49 5
@


1.3
log
@before COW strings
@
text
@d1 11
d60 1
d110 13
@


1.2
log
@beta 7
@
text
@d26 1
a26 1
bool our_aka_matches( addr &a );
d38 11
a48 4
bool   BinkAttach(  const string &file, const addr &to );
bool   HPFSAttach(  const string &file, const addr &to );
bool   TMailAttachFAT( const string &file, const addr &to );
bool   TMailAttachHPFS( const string &file, const addr &to );
d69 29
@


1.1
log
@Initial revision
@
text
@d22 1
a22 1
bool match_addr( addr a1, addr a2 );
d38 4
@
