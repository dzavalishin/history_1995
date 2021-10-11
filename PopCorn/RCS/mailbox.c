head	1.2;
access;
symbols
	Ver_075:1.2;
locks;
comment	@ * @;


1.2
date	96.09.17.23.20.43;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.13.12.38.48;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@First completely working POP3/ESMTP version
@
text
@/************************ PopCorn server ***************************\
 *
 *	Copyright (C) 1991-1996 by Dmitry Zavalishin
 *
 *	Module 	: MailBox class
 *
 *      $Log:  $
 *
 *
 *
 *
\*/

#include "PopCorn.h"
#include "ffolder.h"
#include "except.h"
#include "outfile.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>


MailBox::MailBox(const string &user) : user_v(user)
    {
    lock_v = 0;
    last_msg_no = 0;
    }

MailBox::~MailBox()
    {
    if(lock_v) unlock();
    if(transaction_v) unroll();
    }
        
void MailBox::lock()
    {
      // BUG!! Lock it!
    lock_v = Yes;

    parse_uupc_box();
    
    string fdir = "mail/"+user_v;
    string fuupc = uupc_mailbox_file_name( user_v );

    try { do_move( fuupc, fdir+"/uupc_box" ); }   catch( ... ) {}

    parse_uupc_box();

    printf("Loading dir %s\n", fdir.c_str() );
    
    
    ffolder ff(fdir);
    while( 1 )
        {
        string fn;
        
        try { fn = ff.next(); }
        catch( XEOF ) { break; }
        
        //printf("adding msg %s\n", fn.c_str() );

        minfo m( fn );

        //update_last_msg_no(fn);
        
        mi_v.push_back( m );
        }
    
    }


#if 0
void MailBox::update_last_msg_no( const string &fn )
    {
    const char *npart = strrchr(fn,'/');
    const char *npart1 = strrchr(npart,'\\');
    if( npart1 == 0 ) npart1 = npart;
    if( npart1 == 0 ) return;
    int mn = atoi( npart ) + 1;
    last_msg_no = max( mn, last_msg_no );
    }
#endif
string MailBox::gen_new_msg_fname()
    {
    char buf[200];
    for( int i = 40; i; i-- )
        {
        sprintf( buf, "mail/%s/%d", user_v.c_str(), last_msg_no );
        if( access( buf, 0 ) != 0 )   return buf;
        last_msg_no++;
        }
    for( i = 20; i; i-- )
        {
        sprintf( buf, "mail/%s/%d", user_v.c_str(), last_msg_no );
        if( access( buf, 0 ) != 0 )   return buf;
        last_msg_no += 10;
        }
    throw Fail( "MailBox::gen_new_msg_fname", "can't generate message file name", buf );
    }

void MailBox::parse_uupc_box()
    {
    string fbox = "mail/"+user_v+"/uupc_box";
    printf("Parsing box %s: ", fbox.c_str() );
    FILE *fp = fopen(fbox, "r" );
      //if( fp == NULL ) throw Fail( "MailBox::parse_uupc_box", "can't open file", fbox );
    if( fp == NULL )
        {
        printf( "Can't open\n" );
        return;
        }
    
    string line;

    outfile of;
    of.open( gen_new_msg_fname() );

    bool first_line = Yes;
    
    while(1)
        {
        try { line.load(fp); } catch( XEOF ) { break; }
        line.strip_crlf();
        
        if( line == "" )
            {
            if(first_line) continue;
            printf( "." ); fflush(stdout);
            of.close();
            of.open( gen_new_msg_fname() );
            }
        else
            of.puts(line);

        first_line = No;
        }
    
    if(!first_line) of.close();
    printf( "Done\n" );
    fclose(fp);
    chmod(fbox,0666);
    unlink(fbox);
    }


void MailBox::unlock()
    {
      // BUG!! UnLock it!
    lock_v = No;
    }
        
void MailBox::kill( int msg )
    {
    mi_v[msg].kill();
    }
        
void MailBox::commit() // Changes are confirmed. Do them.
    {
    for( int i = mi_v.size(); i--; )
        mi_v[i].really_kill();
    }

void MailBox::unroll() // Changes must be thrown away
    {
    for( int i = mi_v.size(); i--; )
        mi_v[i].unroll();
    }

int MailBox::messages() const
    {
    return mi_v.size();
    }


void minfo::load()
    {
    struct stat stb;

    if( stat(fn_v, &stb) )
        throw Fail( "minfo::load", "can't stat file", fn_v );

    if( !(stb.st_mode & S_IFREG) )
        throw Fail( "minfo::load", "not a regular file", fn_v );

    size_v = stb.st_size;

#if 0
    
    FILE *fp = fopen(fn_v, "r" );
    if( fp == NULL ) throw Fail( "minfo::load", "can't open file", fn_v );
    
    string line;
    while(1)
        {
        try { line.load(fp); } catch( XEOF ) { break; }

        line.strip_crlf();

        if( line.length() == 0 )
            break; // end of header

        if( isspace( line[0] ) )
            continue; // continuation. we'll ignore. BUG!!
        
        line.strip_ws();
        
        string key, val;
        line.parse( key, ':', val );

        key.lcase();
        val.strip_ws();

        if( key == "message-id" )
            id_v = val;
        
        if( key == "subject" )
            subj_v = val;

        }
    
    fclose(fp);
#endif
    }
@


1.1
log
@Initial revision
@
text
@d1 12
d15 3
d19 3
d23 2
a24 1
MailBox::MailBox()
d27 1
d38 32
d72 75
d149 2
d155 1
d160 2
d166 2
d170 4
d176 49
@
