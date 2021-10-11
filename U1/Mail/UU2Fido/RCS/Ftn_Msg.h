head	1.1;
access;
symbols;
locks;
comment	@ *      @;


1.1
date	95.11.05.17.37.36;	author dz;	state Exp;
branches;
next	;


desc
@@


1.1
log
@Initial revision
@
text
@/************************ FIDO MSG file format ***************************\
 *
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module:   FTN Message class
 *
 *      $Log: Fido_Msg.h $
 *      Revision 1.2  1995/04/13  11:06:28  dz
 *      debug
 *
 *      Revision 1.1  1995/03/11  18:00:54  dz
 *      Initial revision
 *
 *      Revision 1.1  1995/03/11  18:00:54  dz
 *      Initial revision
 *      
 *         Rev 1.7   09 Apr 1993 10:04:36   dz
 *      Extended flags support
 *      
 *         Rev 1.6   29 Mar 1993 04:17:18   dz
 *      Trying to suport FLAGS kludge
 *      
 *         Rev 1.5   25 Feb 1993 18:31:32   dz
 *      Rewritten using fido_addr
 *      
 *         Rev 1.4   06 Nov 1992 19:20:08   dz
 *      Ver 1.12a save
 *      
 *         Rev 1.3   30 Mar 1992 21:43:08   dz
 *      FIDO structures & fido_addr class definition
 *      moved to separate header files.
 *      FSC0035 support written.
 *      
 *         Rev 1.2   04 Feb 1992 22:14:10   dz
 *      const modifers added
 *      
 *         Rev 1.1   11 Oct 1991 05:10:16   dz
 *      New methods - getl() and add_xflag()
 *
 *         Rev 1.0   12 Sep 1991 01:28:30   dz
 *      Initial revision.
 *
 *    Use 4 spaces/tab to see it properly
\*/

#ifndef    FTN_MSG_H
#define    FTN_MSG_H

// to prevent use of fido_msg.h
#define	FIDO_MSG_H


#include <style.h>
#include <stdio.h>
#include <string.h>

#include <uu2io.h>

#include "ftn_str.h"
#include <fidoaddr.h>

#include <ieqseq.h>

typedef IEqualitySequence<string>           FTN_Bag;
typedef FTN_Bag::Cursor                     FTN_Cursor;


//#define        MSG_FN_LEN        150
//#define        FSC35RA            100


#pragma pack(1)


struct hlc                // For internal use only
    {
    char    *fido;
    bool    fmpt;
    bool    topt;
    bool    intl;
    bool    flags;
    bool    reply;
    bool    pid;
    bool    replyaddr;
    bool    replyto;
    bool    domain;
    };


class _Export    FTN_Msg
    {
private:
    FTN_Bag       text;
    FTN_Cursor    cursor;                 // Default text cursor

    string        fname;                  // File name of msg

    FILE         *fp;
    fido_msg_hdr  hdr;                    // Message header


    bool          error;                  // Instance defunct.
//    bool          hflush;                 // Header must be flushed
//    bool          rewrite;                // Message must be rebuilt

    fido_user     from;
    fido_user     to;

    string        kl_reply;
    string        kl_pid;

    fmsg_attr     flags;

    string        fsc35_reply_addr;       // Say, "dz@@isoft.msk.su"
    fido_user     fsc35_reply_to;         // Say, "uucp, 2:5020/23"

    string        via_addr;
    bool          put_via();

private:
    bool          do_attach( void );
    bool          rewrite_msg( void );
    void          load_kludges( void );
    bool          is_known_kludge( const string &k );
    
    void          get_kludge( const string &buf, hlc &state );

    void          reset_hdr_addr( void ); // Fill reserved bytes

    bool          print_flags( string &buf, fmsg_attr f );
    fmsg_attr     scan_flags( const string &buf );

//    bool          fputs( const string &s );   // Add string to msg

public:
    FTN_Msg( void );                     // Constructor
    ~FTN_Msg( void );                    // D'tor


    bool          new_msg( const string &dir );           // Create new one

    bool          attach( const string &dir, int num );   // Attach to file
    bool          attach( const string &file );           // Attach to file
    
    // Detach from file (deletes
    // file if KillSent && Sent)
    bool          detach( void );    

    bool          err( void ) { return error; }           // Returns Yes in case of error
//    void          flush( void ) { hflush = Yes; }         // Force header flush-out

    long          get_date( void );                       // Get msg date in Unix format
    void          set_date( long );                       // Set msg date in Unix format
    void          set_date( const string & );             // Set msg date in text format

// Text management methods    - return Err/Ok

    bool          clear_text( void );                     // Clear message text
    void          clear_hdr( void );                      // Clear message header

    bool          rewind( void );                         // Reset file ptr to msg start

    bool          puts( const string &s );                // Add string to msg
    bool          gets( string &s );                      // Get string from msg

//    bool    getl( string &s );                          // Get line ( < 80 bytes )

    bool          puta( const string &s );                // Add attribute string
    bool          geta( string &s );                      // Get attribute string

// Message header access

    // Get/set source address
    void          get_from( fido_user &f )            { f = from; }
    void          set_from( const fido_user &f )        { from = f; }

    // Get/set dest. address
    void          get_to    ( fido_user &t )            { t = to; }
    void          set_to    ( const fido_user &t )        { to = t; }

    void          get_attr( fmsg_attr &a )            { a = flags; }
    void          set_attr( const fmsg_attr a )        { flags = a; }
    void          add_attr( const fmsg_attr a )        { flags |= a; }
    void          del_attr( const fmsg_attr a )        { flags &= ~a; }

    fmsg_attr     attr()                            { return flags; }
    void          attr( const fmsg_attr a )        { flags = a; }

    // Get/set subject line
    void          get_subj( string &s )                { s = hdr.subject; }
    void          set_subj( const string &s )            { strncpy( hdr.subject, s, 71 ); hdr.subject[71] = '\0'; }

    void          get_fsc35_reply_addr( string &buf )            { buf = fsc35_reply_addr; }
    void          set_fsc35_reply_addr( const string &buf )    {  fsc35_reply_addr = buf; }

    void          get_fsc35_reply_to( fido_user &fa )            {    fa = fsc35_reply_to;    }
    void          set_fsc35_reply_to( const fido_user &fa )    {    fsc35_reply_to = fa;    }

    void          set_via_addr( const string &a ) { via_addr = a; }

// Debug methods

    void          print_hdr( void );
    void          print_msg( void );
    };

#pragma pack()


//    Misc.

int _Export    find_last_msg( const string &dir );



#endif //    FTN_MSG_H
@
