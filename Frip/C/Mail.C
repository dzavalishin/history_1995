/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Mail processing
 *
 *      $Log: Mail.C $
 *      Revision 1.10  1997/01/31 09:54:36  dz
 *      PackDirect support, fixes.
 *
 *      Revision 1.9  1997/01/21 08:53:51  dz
 *      Flags, msgid, via lines
 *
 *      Revision 1.8  1997/01/02 10:06:18  dz
 *      Correct order of attach/message send.
 *
 *      Revision 1.7  1997/01/01 21:31:39  dz
 *      msg received time/date support
 *
 *      Revision 1.6  1997/01/01 16:08:42  dz
 *      Before changing
 *
 *      Revision 1.5  1996/09/08 19:05:31  dz
 *      Version 25
 *
 *      Revision 1.4  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.3  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.2  1996/07/31 07:26:25  dz
 *      Clean up of watcom classlib based code
 *
 *      Revision 1.1  1996/07/31 07:00:23  dz
 *      Initial revision
 *
 *
 *
 *
\*/

#include "frip.h"
#include "filer.h"
#include "mail.h"
#include "rif.h"
#include "base.h"
#include "datetime.h"

//static void send_one_msg( string in );

void FripApp::eat_mail_in( const string &dir )
    {
    BoldMsg("Packing mail in "+dir);
    
    for( filer maildir(dir,"*.msg"); maildir.valid(); maildir.next() )
        {
        string fn = dir + "/" + maildir.current();
        //debug( "Looking at: " + fn );
        try { mail_send_one_msg( fn ); }
        catch( Ex_EOF ex )     { if( conf.debug() ) ex.print(); }
        catch( General_Ex ex ) { ex.print(); }

        if( Stop_Frip ) 
            {
            Error("Frip execution shut down by signal or ^C");
            break;
            }
        }
    }



static void load_line( istream &is, string &out )
    {
    const bs = 4096;
    char b[bs+1];
    
    out = "";
    
    bool first = Yes;
    bool cr = No;
    
    do	
        {
        char *bsp = b;

        while( bsp-b < (bs-1) )
            {
            int c = is.get();
            if( c == EOF ) break;
            if( c == '\n' ) continue;
            if( c == '\r' )
                {
                cr = Yes;
                break;
                }
            *bsp++ = c;
            }

        *bsp = 0;
        
        if( is.fail() )
            throw Ex_Errno("mail load_line","Error reading data",errno );

        if( *b == 0 )
            {
            if( !first )    return;   // Have something
            throw Ex_EOF("mail load_line");
            }
        
        b[bs] = '\0';
        out += string( b );
        first = No;
        } while( !cr );
    }




static void get_kludge( istream &is, string &klk, string &klv )
    {
    string l;

    while(1)
        {
        load_line( is, l );
        if( l[0] != 0x01 )            continue;
        l.strip( 0, 1 );

        //Msg("Kludge "+l );

        if( !l.parse( klk, klv ) )    continue;

        if( klk[klk.length()-1] == ':' )
            klk.strip(klk.length()-1, 1);

        //debug("Kludge is '"+klk+"' => '"+klv+"'" );
        return;
        }
        
    }


static void get_header( istream &is, fido_msg_hdr &h )
    {
    is.seekg( 0L );
    is.read( (char *)&h, sizeof(h) );
    if( is.gcount() != sizeof(h) || is.fail() )
        throw Ex_Errno("get_header", "Can't read *.msg header", errno );
    }

static void put_header( ostream &o, const fido_msg_hdr &h )
    {
    o.seekp( 0L );
    o.write( (const char *)&h, sizeof(h) );
    if( o.fail() )
        throw Ex_Errno("msg_mark_sent", "Can't write *.msg header", errno );
    }



static void get_mail_info(
                   string in,
                   addr &from, addr &to,
                   string &subj,
                   unsigned long &flags,
                   string &msgid
                  )
    {
    ifstream is( in, ios::binary );
    if( !is ) throw Ex_Errno("get_mail_addr", "Can't open "+in, errno );
    
    fido_msg_hdr h;
    get_header( is, h );
    flags = h.attr; // We should add data from ^AFLAGS here, but we're too laaaazy yet :)

    h.subject[sizeof(h.subject)-1] = 0; // zero-terminate
    subj = h.subject;
    
    int fmpt = 0, topt = 0;
    addr intlfrom, intlto;
    addr domfrom, domto;
    string flags_s;

    bool got_intl = No, got_domain = No;
    
    string klk, klv;
    while( 1 )
        {
        try { get_kludge( is, klk, klv ); } catch(Ex_EOF) { break; }
        
        klk.lcase();
        if( klk == "intl" )
            {
            string ito, ifm;
            if( !klv.parse( ito, ifm ) )
                {
                Error("Unparsable intl "+klv );
                continue;
                }
            intlfrom = ifm;
            intlto = ito;
            got_intl = Yes;
            }
        else if( klk == "fmpt" ) fmpt = atoi( klv );
        else if( klk == "topt" ) topt = atoi( klv );
        else if( klk == "msgid" ) msgid = klv;
        else if( klk == "flags" ) flags_s = klv;
        else if( klk == "domain" )
            {
            string dto, to, dfm, fm, tail;
            if(
               (!klv.parse( dto, tail )) ||
               (!tail.parse( to, tail )) ||
               (!tail.parse( dfm, fm ))
              )
                {
                Error("Unparsable intl "+klv );
                continue;
                }
            domfrom = fm; domfrom.domain( dfm );
            domto = to;   domto.domain( dto );
            got_domain = Yes;
            }
        }
    
    is.close();

    bool got_zone = No;
    
    if( got_domain )
        {
        from = domfrom;
        to = domto;
        got_zone = Yes;
        }
    else if( got_intl )
        {
        from = intlfrom;
        to = intlto;
        got_zone = Yes;
        }

    if( !got_zone )
        {
          // try to get main zone from msgid
        try
            {
            string mi_address, mi_etc;
            msgid.parse( mi_address, mi_etc );
            
            addr miad( mi_address );
            
              //from.zone( miad.zone() );
              //to.zone( miad.zone() );

            from = miad;
            to   = miad;

            from.node(h.origNode);
            to.node(h.destNode);
            from.net(h.origNet);
            to.net(h.destNet);
            
            got_zone = Yes;
            }
        catch(...)
            {
            }
        }

    if(!got_zone)
        throw Ex_Fail("get_mail_addr", "No DOMAIN, INTL or MSGID kludge in message, unknown zone", in );
    
    if(
       h.origNode != from.node() ||
       h.origNet  != from.net() ||
       h.destNode != to.node() ||
       h.destNet  != to.net()
      )
        Warning("Zonegateing is nor supported neither required with FRIP");

    from.point( fmpt );
    to.point( topt );

    flags |= mail_scan_flags(flags_s);
    }

void msg_mark_sent( string in )
    {
    fstream s( in, ios::binary|ios::in|ios::out|ios::nocreate );
    if( !s ) throw Ex_Errno("msg_mark_sent", "Can't open "+in, errno );
    
    fido_msg_hdr h;
    get_header( s, h );
    h.attr |= FF_Sent;
    put_header( s, h );
    }


void msg_mark_received( string in )
    {
    fstream s( in, ios::binary|ios::in|ios::out|ios::nocreate );
    if( !s ) throw Ex_Errno("msg_mark_received", "Can't open "+in, errno );
    
    fido_msg_hdr h;
    get_header( s, h );
    h.attr &= ~(FF_Sent|FF_Crash|FF_Recd|FF_Orphan|FF_Local|FF_HoldForPickup);
    h.date_arrived = (datetime::now()).localtime_msgtime();
    put_header( s, h );
    }


void FripApp::mail_send_one_msg( string in )
    {
    addr from, to;
    unsigned long attr;
    string subj, msgid;

    get_mail_info( in, from, to, subj, attr, msgid );

    if( (attr&FF_Sent) || (attr&FF_Orphan) || (attr&FF_FileRequest) || (attr&FF_FileUpdateReq) )
        return;

    if( (!conf.fripmail_pack_direct()) && (attr&FF_Direct) )
        return;
    
    // It's for us, don't send
    if( our_aka_matches(to) )                       return;

    Msg("Message from "+string(from)+" to "+string(to));

      // exception may be thrown from find_route to indicate no-route situation
    addr route_via = db.find_route( to, Yes /* for rif */ );

    for( int i = conf.aka.count(); i--; )
        if( conf.aka[i] == route_via )
            {
            debug("Can't deliver to "+string(to)+": routed to our node");
            return;
            }

    /*
     // this prevents points from sending mail via their boss :)
    if( our_aka_matches(route_via) )
        {
        debug("Can't deliver to "+string(to)+": routed to our node");
        return;
        }
    */
        
    bool kill    = (attr&FF_KillSent) ? Yes : No;
    bool attach  = (attr&FF_FileAttached) ? Yes : No;
    bool audit   = (attr&FF_AuditRequest) ? Yes : No;
    bool rrq     = (attr&FF_ReturnReceiptRequest) ? Yes : No;
    bool rec     = (attr&FF_IsReturnReceipt) ? Yes : No;
    bool fattach = (attr&FF_FileAttached) ? Yes : No;

    flags the_flags;

    if( audit ) the_flags += "ARQ"; // Each node in chain should send a note back
    if( rrq )   the_flags += "RRQ"; // Last node must send receipt message
    if( rec )   the_flags += "REC"; // This is a receipt, don't try to answer!

    // try to send attach first, or we'll end generating a message    
    // each run if attach can't be sent
    if( fattach ) fara_send( subj, to, No /*kill*/, rif_type::File, string("ATT") );

    fara_send( in, to, kill, rif_type::Mail, the_flags );

    if( kill )        unlink( in );
    else              msg_mark_sent( in );
    }





// Find unused message number in box

int find_last_msg( const string &dir )
    {
    int last = 1;
    
    for( filer maildir(dir,"*.msg"); maildir.valid(); maildir.next() )
        {
        string fn = maildir.current();
        int mn = atoi(fn);
        if( mn > last ) last = mn+1;
        }

    return last;
    }


string new_msg_name( const string &dir )
    {
    static last = -1;

    if( last == -1 ) last = find_last_msg(dir);

    for(int i = 1000; i--;)
        {
        char mno[20];
        sprintf( mno, "/%d.msg", last++ );
        string nn = dir+mno;
        if( !access( nn, 0 ) )
            continue;
        return nn;
        }
    throw Ex_Fail("new_msg_name","I'm tired to search for new msg name","):(");
    }



void msg_add_via_lines( string name, const vector <path_elem> &path )
    {
    fstream s( name, ios::binary|ios::in|ios::out|ios::nocreate );
    if( !s ) throw Ex_Errno("msg_add_via_lines", "Can't reopen "+name, errno );

    s.seekg( -1, ios::end );
    if( s.fail() ) throw Ex_Errno("msg_add_via_lines", "Can't seek in "+name, errno );

      // Message ends with \0?
    if( 0 == s.get() )  s.seekp( -1, ios::end );     // Overwrite that zero
    else                s.seekp(  0, ios::end );     // Go to end of message

    int pathlen = path.size();
    for( int i = 0; i < pathlen; i++ )
        {
        string a = string(path[i].address());
        string t = path[i].time().localtime_via_string();
        if( path[i].time().gmtime() == 0 )
            s << "\x01Via " << a.c_str() << ", relayed with FRIP\x0D";
        else
            s << "\x01Via " << a.c_str() << ", relayed with FRIP " << t.c_str() << "\x0D";
        }


    s.put( '\0' );          // add trailing zero
    if( s.fail() ) throw Ex_Errno("msg_add_via_lines", "Error writing to "+name, errno );
    }


static void msg_put_kludge( ostream &s, string key, string val )
    {
    s << "\x01" << key.c_str() << " " << val.c_str() << "\x0D";
    }

static void msg_put_kludge( ostream &s, string key, int val )
    {
    char b[15];
    sprintf( b, "%d", val );
    msg_put_kludge( s, key, b );
    }

static string newid()
    {
    static tt = 0;

    if( tt == 0 ) tt = time(0);

    char b[15];
    sprintf( b, "%08x", tt++ );

    return b;
    }

void msg_create_new( string contents, const addr &to, string fromname, string toname, string subject, string reply_msgid )
    {
    const addr &from = conf.match_aka( to );

    int tries = 10;
    
    while(1)
        {
        string newname = new_msg_name( conf.fripmail() );
    
        fstream s( newname, ios::binary|ios::in|ios::out|ios::noreplace );

        if( !s )
            {
            if( tries-- ) continue;

            throw Ex_Errno("msg_create_new", "Can't create "+newname, errno );
            }

        fido_msg_hdr h;
        h.attr = FF_Private|FF_KillSent|FF_Local|FF_IsReturnReceipt;
        h.date_arrived = (datetime::now()).localtime_msgtime();
        h.date_written = h.date_arrived;
        strncpy( h.from, fromname.c_str(), sizeof(h.from) );
        h.from[sizeof(h.from)-1] = 0;
    
        strncpy( h.to, toname.c_str(), sizeof(h.to) );
        h.to[sizeof(h.to)-1] = 0;
    
        strncpy( h.subject, subject.c_str(), sizeof(h.subject) );
        h.subject[sizeof(h.subject)-1] = 0;

        h.date;

        h.timesRead = h.cost = 0;

        h.destNode = to.node();
        h.origNode = from.node();
        h.destNet  = to.net();
        h.origNet  = from.net();

        h.replyTo = h.nextReply = 0;
    
        put_header( s, h );

        msg_put_kludge( s, "PID:", "Frip " FRIP_VER_STR );
        msg_put_kludge( s, "MSGID:", string(from)+" "+newid() );
        if( !reply_msgid.is_empty() ) msg_put_kludge( s, "REPLY:", reply_msgid );
        
        char b[80];
        sprintf( b, "%d:%d/%d %d:%d/%d", to.zone(), to.net(), to.node(), from.zone(), from.net(), from.node() );
        msg_put_kludge( s, "INTL", b );
    
        if( to.point() )      msg_put_kludge( s, "TOPT", to.point() );
        if( from.point() )    msg_put_kludge( s, "FMPT", from.point() );

        
        s << contents.c_str() << "\x0D";
        s.put( '\0' );          // add trailing zero

        break;
        }

    }





