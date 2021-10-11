head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	97.03.17.08.03.42;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	97.01.21.08.53.51;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	97.01.01.16.08.42;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.12.28.13.42.00;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@delete_timed_out_routes()
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	FripApp class
 *
 *      $Log: app.h $
 *      Revision 1.4  1997/01/21 08:53:51  dz
 *      New flag type for fara, message id suppoer.
 *
 *      Revision 1.3  1997/01/01 16:08:42  dz
 *      Before changing
 *
 *      Revision 1.2  1996/12/28 13:42:00  dz
 *      Ver. 27
 *
 *      Revision 1.1  1996/09/08 19:05:31  dz
 *      Initial revision
 *
 *      
 *      
\*/

#ifndef APP_H
#define APP_H

#include "version.h"
#include "except.h"
#include "style.h"
#include "strng.h"
#include "addr.h"
#include "rip.h"
#include "rif.h"
#include "conf.h"
#include "zip.h"
#include "base.h"
#include "links.h"
#include "msgid.h"
#include "approve.h"


class FripApp : private links
    {

    msgid_base    id_base;
    zip_list      zlist;
    //links         llist;
    base          db;
    approve       ap;

    public:
        

        FripApp()
            :
        id_base( conf.dir()+"/msgid.dat" )

            {
            }

        ~FripApp()
            {
            zlist.send();
            }


        
        //void send_rips( const rip &r )     {  llist.send_rip_to_all_links( r, Yes ); }
        //void load_links()                  {  links::load();                 }
        
        //void ZipList_add( const string &ripfile, const addr &dest ) {  zlist.add( ripfile, dest );    }
        

        // --- Basic functions

        // Process routing loss
        void         frip_do_off( addr dest );
        // Send file with FARA
        void         fara_send( string name, addr dest, bool kill, rif_type t, flags f, string msgid = "" );
        // Process incoming rips/rifs
        void         main_loop( bool force_announce, bool force_ping );

        
        // --- Main workers

        void         do_help();
        void         do_ad( const string &in, rip &r );
        // process received or made in frip_do_off off type rip
        void         do_off( rip &r ); 

        void         eat_rifs_in( const string &dir );
        void         receive_one_rif( const string &in, rif &r );
        void         do_one_rif( string in );

        void         unzip_in( const string &inb );
        void         eat_rips_in( const string &dir );
        void         do_one_rip( string in );

        void         delete_timed_out_routes();
        
        // --- Mail

        void         mail_send_one_msg( string in );
        void         eat_mail_in( const string &dir );

        
        // --- Main entry point

        void         frip( int ac, char **av );


        // --- Postprocessors

        // Creates output route file
        void         create_routes();


        // --- Tools

          //void         send_to_the_link( rip r, const link &l );
          //void         do_a_link( rip r, const link &l );

        void         send_rip_to_link( rip r, const link &l, bool do_out_checks );
        void         send_rip_to_all_links( const rip &r, bool do_out_checks );
        
        void         ping();
        void         send_rescan_req();
        
    };


extern string started_in_dir;


#endif // APP_H





@


1.4
log
@New flag type for fara, message id suppoer.
@
text
@d8 3
d77 1
a77 1
        void         frip_do_off( string dest );
d99 2
@


1.3
log
@Before changing
@
text
@d8 3
d76 1
a76 1
        void         fara_send( string name, addr dest, bool kill, rif_type t, string flags );
d89 1
a89 1
        void         relay_one_rif( const string &in, rif &r );
@


1.2
log
@Ver. 27
@
text
@d8 3
a38 1
    
a44 2
    

d86 1
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: frip.C $
d15 3
d126 1
@
