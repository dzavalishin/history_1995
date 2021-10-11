head	1.10;
access;
symbols;
locks;
comment	@ * @;


1.10
date	97.01.31.09.54.36;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	97.01.21.08.53.51;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	97.01.02.10.06.18;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	97.01.01.21.31.39;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	97.01.01.16.08.42;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.07.31.07.26.25;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.07.31.07.00.23;	author dz;	state Exp;
branches;
next	;


desc
@@


1.10
log
@PackDirect support, fixes.
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Mail processing
 *
 *      $Log: Mail.C $
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





@


1.9
log
@Flags, msgid, via lines
@
text
@d8 3
d159 1
d181 1
d206 1
d227 2
a228 3
    if( got_intl == No && got_domain == No )
        throw Ex_Fail("get_mail_addr", "No DOMAIN or INTL kludge in message, unknown zone", in );

d233 1
d239 1
d242 31
d283 2
d320 6
a325 1
    if( (attr&FF_Sent) || (attr&FF_Orphan) )        return;
@


1.8
log
@Correct order of attach/message send.
@
text
@d8 3
d55 1
d156 1
a156 1
void get_mail_info(
d160 2
a161 1
                   unsigned long &flags
d200 1
d276 1
a276 1
    string subj;
d278 1
a278 1
    get_mail_info( in, from, to, subj, attr );
d289 9
d303 2
a304 1

d312 1
a312 1
    string flags;
d314 3
a316 3
    if( audit ) flags = "ARQ"; // Each node in chain should send a note back
    if( rrq ) flags += " RRQ"; // Last node must send receipt message
    if( rec ) flags += " REC"; // This is a receipt, don't try to answer!
d320 1
a320 1
    if( fattach ) fara_send( subj, to, No /*kill*/, rif_type::File, "ATT" );
d322 1
a322 1
    fara_send( in, to, kill, rif_type::Mail, flags );
d369 1
a369 1
void msg_add_via_lines( string name, const vector <addr> &path )
d384 6
a389 2
        const string &e = string(path[i]);
        s << "\x01Via " << e.c_str() << ", relayed with FRIP\x0D";
d396 92
@


1.7
log
@msg received time/date support
@
text
@d8 3
a300 2
    
    fara_send( in, to, kill, rif_type::Mail, flags );
d302 2
d305 2
@


1.6
log
@Before changing
@
text
@d8 3
d36 1
a36 1

d250 1
a250 1
void msg_clean_bad_flags( string in )
d253 1
a253 1
    if( !s ) throw Ex_Errno("msg_clean_bad_flags", "Can't open "+in, errno );
d258 1
@


1.5
log
@Version 25
@
text
@d8 3
d270 1
a270 1
    
d272 9
@


1.4
log
@Exceptions added, not compiled
@
text
@d8 3
d26 1
d32 1
a32 1
static void send_one_msg( string in );
d34 1
a34 1
void eat_mail_in( const string &dir )
d42 1
a42 1
        try { send_one_msg( fn ); }
d55 1
a55 1
static void load_line( FILE *fp, string &out )
d71 1
a71 1
            int c = getc(fp);
d84 1
a84 1
        if( ferror(fp) )
d90 1
a90 1
            throw Ex_EOF;
d102 1
a102 1
static void get_kludge( FILE *fp, string &klk, string &klv )
d108 1
a108 1
        load_line( fp, l );
d114 1
a114 1
        if( !parse( l, klk, klv ) )   continue;
d119 1
a119 1
        debug("Kludge is '"+klk+"' => '"+klv+"'" );
d126 1
a126 1
static void get_header( istream is, fido_msg_hdr &h )
d129 1
a129 1
    is.read( &h, sizeof(h) );
d134 1
a134 1
static void put_header( ostream o, const fido_msg_hdr &h )
d136 4
a139 4
    s.seekp( 0L );
    s.write( &h, sizeof(h) );
    if( s.fail() )
        throw Ex_Errno("msg_mark_sent", "Can't write header of "+in, errno );
d167 1
a167 1
    while( get_kludge( fp, klk, klv ) )
d169 2
d175 1
a175 1
            if( !parse( klv, ito, ifm ) )
d190 3
a192 3
               (!parse( klv, dto, tail )) ||
               (!parse( tail, to, tail )) ||
               (!parse( tail, dfm, fm ))
d234 1
a234 1
    fstream s( in, ios::binary|ios:in|ios::out|ios::nocreate|ios::noreplace );
d246 1
a246 1
    fstream s( in, ios::binary|ios:in|ios::out|ios::nocreate|ios::noreplace );
d256 1
a256 1
static void send_one_msg( string in )
d283 1
a283 1
    frip_do_send( in, to, kill, rif_type::Mail, flags );
d285 1
a285 1
    if( fattach ) frip_do_send( subj, to, No /*kill*/, rif_type::File, "ATT" );
d334 1
a334 1
    fstream s( name, ios::binary|ios:in|ios::out|ios::nocreate|ios::noreplace );
d337 1
a337 1
    s.seekg( -1, ios::end ) );
@


1.3
log
@Before exceptions.
@
text
@d8 3
d28 1
a28 1
static bool send_one_msg( string in );
d30 1
a30 1
bool eat_mail_in( const string &dir )
d38 3
a40 2
        send_one_msg( fn );
        
d43 1
a43 1
            Error("Frip execution shut down by signal or ^C, unprocessed rips left.");
a46 2
    
    return Ok;
d51 1
a51 1
bool load_line( FILE *fp, string &out )
d81 1
a81 1
            return Err;
d85 2
a86 4
            if( !first )   // Have something
                return Ok;
            else
                return Err;
a92 2
    
    return Ok;
d98 1
a98 1
static bool get_kludge( FILE *fp, string &klk, string &klv )
d102 1
a102 1
    while( (!feof(fp)) && (Err != load_line( fp, l )) )
d104 2
a105 3
        if( l[0] != 0x01 )
            continue;

d110 1
a110 2
        if( !parse( l, klk, klv ) )
            continue;
d115 2
a116 2
        //Msg("Kludge is '"+klk+"' => '"+klv+"'" );
        return Yes;
a118 1
    return No;
d121 19
a139 1
bool get_mail_addr(
d146 3
a148 4
    FILE *fp = fopen( in, "rb" );

    fseek( fp, 0L, SEEK_SET );

d150 1
a150 7

    if( 1 != fread( &h, sizeof(h), 1, fp ) )
        {
        fclose( fp );
        return Err;
        }

d198 1
a198 1
    fclose( fp );
d201 1
a201 4
        {
        Error("No DOMAIN or INTL kludge in message, unknown zone");
        return Err;
        }
a223 2

    return Ok;
d226 1
a226 1
bool msg_mark_sent( string in )
d228 3
a230 2
    FILE *fp = fopen( in, "r+b" );

d232 1
a232 8
    
    fseek( fp, 0L, SEEK_SET );
    if( 1 != fread( &h, sizeof(h), 1, fp ) )
        {
        fclose( fp );
        return Err;
        }
    
d234 1
a234 10
    
    fseek( fp, 0L, SEEK_SET );
    if( 1 != fwrite( &h, sizeof(h), 1, fp ) )
        {
        fclose( fp );
        return Err;
        }
    
    fclose( fp );
    return Ok;
d238 1
a238 1
bool msg_clean_bad_flags( string in )
d240 2
a241 1
    FILE *fp = fopen( in, "r+b" );
d244 1
a244 8
    
    fseek( fp, 0L, SEEK_SET );
    if( 1 != fread( &h, sizeof(h), 1, fp ) )
        {
        fclose( fp );
        return Err;
        }
    
d246 1
a246 10
    
    fseek( fp, 0L, SEEK_SET );
    if( 1 != fwrite( &h, sizeof(h), 1, fp ) )
        {
        fclose( fp );
        return Err;
        }
    
    fclose( fp );
    return Ok;
d250 1
a250 1
static bool send_one_msg( string in )
d256 1
a256 5
    if( Err == get_mail_addr( in, from, to, subj, attr ) )
        {
        Error("Can't get addresses for "+in+", skipped");
        return Err;
        }
d258 1
a258 6
    if( (attr&FF_Sent) || (attr&FF_Orphan) )
        {
        //debug("Sent or orphan message "+in+", skipped");
        return Err;
        }
    
d260 1
a260 1
    if( our_aka_matches(to) ) return Err;
d277 1
a277 2
    if( frip_do_send( in, to, kill, rif_type::Mail, flags ) )
        return Err;
d279 1
a279 2
    if( fattach && frip_do_send( subj, to, No /*kill*/, rif_type::File, "ATT" ) )
        return Err;
a282 2

    return Ok;
d312 1
a312 1
    while(1)
d321 1
d326 1
a326 1
bool msg_add_via_lines( string name, const vector <addr> &path )
d328 2
d331 2
a332 6
    FILE *fp = fopen( name, "r+b" );
    if( fp == NULL )
        {
        Error("Can't reopen "+name+" to add via lines");
        return Err;
        }
d334 3
a336 12

    if( EOF == fseek( fp, -1, SEEK_END ) )
        {
        Error("Can't seek in "+name+" to add via lines");
        fclose( fp );
        return Err;
        }
    
    if( 0 == getc(fp) )                // Message ends with \0?
        fseek( fp, -1, SEEK_END );     // Overwrite that zero
    else
        fseek( fp, 0, SEEK_END );      // Go to end of message
d341 2
a342 2
        string e = string(path[i]);
        fprintf( fp, "\x01Via %s, relayed with FRIP\x0D", e.c_str() );
d346 2
a347 10
    fwrite( "\0", 1, 1, fp );          // add trailing zero

    if( ferror( fp ) )
        {
        Error("Can't seek in "+name+" to add via lines");
        fclose( fp );
        return Err;
        }
    fclose( fp );
    return Ok;
@


1.2
log
@Clean up of watcom classlib based code
@
text
@d8 3
d34 2
a35 3
        debug( "Looking at: " + fn );
        if( send_one_msg( fn ) == Ok )
            unlink( fn );
d126 6
a131 1
bool get_mail_addr( string in, addr &from, addr &to )
d145 5
d225 54
d282 2
d285 1
a285 1
    if( Err == get_mail_addr( in, from, to ) )
d291 6
d301 13
d315 1
a315 1
    if( frip_do_send( in, to, Yes /* kill */, rif_type::Mail ) )
d317 6
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: MainLoop.C $
a19 4
//#include "approve.h"

//#include <io.h>
//#include <fcntl.h>
@
