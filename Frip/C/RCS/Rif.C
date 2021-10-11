head	1.8;
access;
symbols;
locks;
comment	@ * @;


1.8
date	97.03.22.15.50.07;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	97.03.05.18.12.38;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	97.01.21.08.53.51;	author dz;	state Exp;
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
date	96.07.31.07.00.23;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.07.22.02.48.05;	author dz;	state Exp;
branches;
next	;


desc
@@


1.8
log
@datetime
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	RIF Class impl.
 *
 *      $Log: Rif.C $
 *      Revision 1.7  1997/03/05 18:12:38  dz
 *      Ver. 34
 *
 *      Revision 1.6  1997/01/21 08:53:51  dz
 *      Flags remade, msgid added.
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
 *      Revision 1.2  1996/07/31 07:00:23  dz
 *      FripMail written
 *
 *      Revision 1.1  1996/07/22 02:48:05  dz
 *      Initial revision
 *
 *
 *
 *
\*/

#include "frip.h"
#include "rif.h"
#include "osname.h"

  /*
   * File structure is:
   *
   *    -- data size
   *    -- Tailer elem data
   *    -- Tailer elem size
   *       ...
   *    -- file data
   */


const fptr_size = 4;

static void putlong( ostream &o, int data )
    {
    o.write( (const char *)&data, 4 );
    if( o.bad() ) throw Ex_Fail("putlong", "write failed","");
    }

static void getlong( istream &i, int &data )
    {
    i.read( (char *)&data, 4 );
    if( i.bad() ) throw Ex_Fail("getlong", "read failed","");
    }

static void add_elem( char type, const char* buf, int len, ostream &o, int datasize )
    {
    
      //o.seekp( -fptr_size, ios::end );
    o.seekp( 0, ios::end );
    o.seekp( o.tellp()-fptr_size );
    
    len++; // include type char
    putlong( o, len );

    o.write( &type, 1 );
    o.write( buf, len-1 ); // -1 compensates for len++ above

    putlong( o, datasize );

    if( o.bad() )
        {
        char bb[2]; bb[0] = type; bb[1] = 0;
        throw Ex_Fail("add_elem", "write fail for type "+string(bb),"");
        }
    }

static void add_elem( char type, string temp, ostream &o, int datasize )
    {
    add_elem( type, temp.c_str(), temp.length()+1, o, datasize );
    }


// for files in rif formt only!
static int get_datasize( istream &i )
    {
    int datasize;
    i.seekg( -fptr_size, ios::end );
    getlong( i, datasize );
    return datasize;
    }

static void seek_to_tail( istream &i )
    {
    int ds = get_datasize( i );
    i.seekg( ds );
    if( i.bad() ) throw Ex_Fail("seek_to_tail", "seek failed","");
    }

static void  get_elem( char* buf, int len, int &olen, istream &i )
    {
    getlong( i, olen );
    if( olen > len ) throw Ex_Fail("get_elem", "elem. buffer overflow","");
    i.read( buf, olen );
    if( i.bad() ) throw Ex_Fail("get_elem", "read failed","");
    }





void rif::load( string const &s )
    {
    fstream f( s, ios::binary|ios::in );
    if( !f )     throw Ex_Fail( "rif::load", "Can't open file", s );
    
    path_v.erase  ( path_v.begin(), path_v.end() );
    created_v = datetime::now();
    flags_v.clear();
    msgid_v = "";
        
    bool got_from = No, got_to = No, got_type = No, got_name = No;

    f.seekg( -fptr_size, ios::end );
    int end_of_tail = f.tellg();
    seek_to_tail( f );

    //printf("end of tail %d, pos %d\n", end_of_tail, f.tellg() );
    
    while( f.tellg() < end_of_tail )
        {
        const bs = 2000;
        char  buf[bs];

        int len = 0;
        try { get_elem(buf, bs, len, f); } catch(...) { break; }
        
        char key = *buf;
        char *val = buf+1;

            {
            char buf[1000];
            sprintf(buf,"record %c, len %d, data <%.*s>", key, len, len, val );
            debug(buf);
            }
        
        switch( key )
            {
            case 'N': // Name
                    {
                    name_v = val;
                    got_name = Yes;
                    }
                break;
                
            case '+': // Flags
                flags_v = string(val);
                break;
                
            case 'F': // FROM
                    {
                    string vc = val;
                    vc.lcase();
                    if( vc == "myself" ) from_v = conf.aka[0];
                    else                 from_v = addr( val );
                    got_from = Yes;
                    }
                break;
                
            case 'T': // TO
                to_v = addr( val );
                got_to = Yes;
                break;
                
            case 'D': // description
                desc_v = val;
                break;
                
            case 'I': // msgid
                msgid_v = val;
                break;
                
            case 'Y': // TYPE
                    {
                    type_v = rif_type( val );
                    got_type = Yes;
                    }
                break;
                
            case 'C': // Created
                created_v = datetime::from_time_t_string( val, datetime::UTC ); //atoi( val );
                break;
                
            case 'P': // PATH
                    {
                    //string path, tail;
                    //string(val).parse( path, tail );
                    path_v.push_back( path_elem(val) );
                    }
                break;

            } // switch
        } // while
    
    if( !got_type ) type_v = rif_type::F;         // Хрен с ним, поверим
    
    if(
       got_from != Yes ||
       got_to != Yes ||
       (got_name != Yes && type_v == rif_type::F)
      )
        throw Ex_Fail( "rif::load", "Incomplete RIF", s );
    
    if( ((rif_type::rftype)type_v) == rif_type::NOP )
        throw Ex_Fail( "rif::load", "Unknown RIF type", s );
    }





static rif_type::rtts rttab[] =
    {
    rif_type::F,    "File",
    rif_type::F,    "F",
    rif_type::Mail, "Mail",
    rif_type::Mail, "M",
    rif_type::NOP,  "NOP"        // Keep this line at the end!
    };
    

rif_type::rif_type( string a )
    {
    for( int i = 0; rttab[i].val != NOP; i++ )
        if( 0 == stricmp( rttab[i].name, a ) )
            {
            val = rttab[i].val;
            return;
            }
    
    val = NOP;
    Error("Unknown RIF type: "+a);
    }

rif_type::operator string () const
    {
    for( int i = 0; rttab[i].val != NOP; i++ )
        if( val == rttab[i].val )
            return rttab[i].name;
    
    if( val != NOP ) Error("Unknown RIF type value!");
    return "NOP";
    }


  // ----------------------------------------------------------------------

void unpack_rif( string const &s )
    {
    fstream f( s, ios::binary|ios::in|ios::out|ios::nocreate );
    if( !f )     throw Ex_Fail( "unpack_rif", "Can't open file", s );

    int datasize = get_datasize( f );
    chsize( (f.rdbuf())->fd(), datasize ); // Cut off tail
    if( f.sync() == EOF ) throw Ex_Fail( "unpack_rif", "Can't sync file", s );
    f.close();
    }


void add_path_to_rif( string const &s )
    {
    fstream f( s, ios::binary|ios::in|ios::out|ios::nocreate );
    if( !f )     throw Ex_Fail( "add_path_to_rif", "Can't open file", s );
    
    int datasize = get_datasize( f );

    string temp = (string(conf.aka[0])) + " " + (datetime::now()).gmtime_time_t_string();
    add_elem( 'P', temp.c_str(), temp.length()+1, f, datasize );

    if( f.sync() == EOF ) throw Ex_Fail( "add_path_to_rif", "Can't sync file", s );
    f.close();
    }


  // ----------------------------------------------------------------------

static string rif_newid( const addr &from )
    {
    static tt = 0;

    const addr &ia = conf.match_aka(from);
    
    if( tt == 0 ) tt = time(0);

    char b[15];
    sprintf( b, " %08x", tt++ );

    return string(ia) + b;
    }


  // Shouild be called only for file containing only data!

void rif::save_to_file( ostream &f )
    {

    f.seekp( 0, ios::end );
    int datasize = f.tellp();
    //printf( "file size = %d\n", datasize );

    // or next add_item will overwrite end of file.
    putlong( f, datasize );

    // ident
    add_elem( '!', "I=AM=RIF!", f, datasize );

    if( type_v != rif_type::Mail )
        add_elem( 'N', name_v, f, datasize );

    add_elem( 'F', string(from_v), f, datasize );
    add_elem( 'T', string(to_v),   f, datasize );
    add_elem( 'Y', string(type_v), f, datasize );

    if(msgid_v.is_empty()) msgid_v = rif_newid( from_v );
    add_elem( 'I', msgid_v, f, datasize );

    if( desc_v.length() )
        add_elem( 'D', desc_v, f, datasize );

        {
        char temp[30];
        sprintf( temp, "%ld", time(0) );
        add_elem( 'C', temp, f, datasize );
        }
    
    // path
    add_elem( 'P', string(path_elem(conf.aka[0],datetime::now())), f, datasize );

    if( !flags_v.is_empty() )
        add_elem( '+', flags_v, f, datasize );
    }


string get_nameext(const char * ffn )
    {
    char ndr[_MAX_DRIVE], ndi[_MAX_DIR], nfn[_MAX_FNAME], nex[_MAX_EXT];
    _splitpath( (char *)ffn, ndr, ndi, nfn, nex );
    
    return string(nfn)+string(nex);
    }

void rif::create_in_dir( const string &dir, string &newname, const string &data, bool kill )
    {
    string tempn = tempFileName(dir+"/rif?????.rif");
    name(get_nameext(data)); // Tell carrier the original filename to restore to
    
    if( kill )  frip_rename(data, tempn);
    else        frip_copy(data, tempn);
    
    fstream temp( tempn, ios::binary|ios::in|ios::out );
    
    if( !temp )
        throw Ex_Fail( "rif::create_in_dir", "Can't reopen file", tempn );

    newname = tempn;
    
    save_to_file( temp );
    }




@


1.7
log
@Ver. 34
@
text
@d8 3
d125 1
a125 1
    created_v = time(0);
d198 1
a198 4
                    {
                    created_v = atoi( val );
                    //got_type = Yes;
                    }
@


1.6
log
@Flags remade, msgid added.
@
text
@d8 3
d181 4
@


1.5
log
@Version 25
@
text
@d8 3
d120 2
a121 1
    flags_v = "";
d158 1
a158 1
                flags_v = val;
d196 3
a198 3
                    string path, tail;
                    string(val).parse( path, tail );
                    path_v.push_back( addr( path ) );
d277 1
a277 1
    string temp = (string)(conf.aka[0]);
d287 13
d324 3
d337 1
a337 1
    add_elem( 'P', string(conf.aka[0]), f, datasize );
d339 1
a339 1
    if( flags_v.length() )
@


1.4
log
@Exceptions added, not compiled
@
text
@d8 3
d62 1
a62 1
    if( Err == putlong( o, len ) ) return Err;
d78 1
a78 1
    add_elem( 'F', temp.c_str(), temp.length()+1, f, datasize );
d83 1
a83 1
int void get_datasize( istream &i )
d93 1
a93 3
    int ds;
    if( get_datasize( i, ds ) == Err ) return Err;
    //printf("seeking to %ld\n", ds );
d133 1
a133 1
        try { get_elem(buf, bs, len, f) } catch { break; }
d193 1
a193 1
                    parse( val, path, tail );
d271 1
a271 2
    int datasize = 0;
    if( get_datasize( f, datasize ) ) return Err;
@


1.3
log
@Before exceptions.
@
text
@d8 3
d39 1
a39 1
bool putlong( ostream &o, int data )
d42 1
a42 1
    return o.bad() ? Err : Ok;
d45 1
a45 1
bool getlong( istream &i, int &data )
d48 1
a48 1
    return i.bad() ? Err : Ok;
d51 1
a51 1
bool add_elem( char type, const char* buf, int len, ostream &o, int datasize )
d64 1
a64 1
    if( Err == putlong( o, datasize ) ) return Err;
d69 1
a69 1
        Error("add_elem for type "+string(bb)+" failed");
d71 1
d73 3
a75 1
    return o.bad() ? Err : Ok;
d78 1
d80 1
a80 1
bool get_datasize( istream &i, int &datasize )
d82 1
d85 1
a85 3
    //printf("got datasize %ld\n", datasize );
    
    return i.bad() ? Err : Ok;
d88 1
a88 1
bool seek_to_tail( istream &i )
d94 1
a94 2
    
    return i.bad() ? Err : Ok;
d97 1
a97 1
bool get_elem( char* buf, int len, int &olen, istream &i )
d99 2
a100 2
    if( Err == getlong( i, olen ) ) return Err;
    if( olen > len ) return Err;
d102 1
a102 1
    return i.bad() ? Err : Ok;
d109 1
a109 1
bool rif::load( string const &s )
d112 1
a112 6
    
    if( !f )
        {
        Error( "Can't open "+s );
        return Err;
        }
d132 1
a132 2
        if( get_elem(buf, bs, len, f) || f.fail() )
            break;
d207 1
a207 5
        {
        Error("Incomplete RIF: "+s );
        return Err;
        }
    
d210 1
a210 6
        {
        Error("Unknown RIF type: "+s);
        return Err;
        }

    return Ok;
d253 1
a253 1
bool unpack_rif( string const &s )
d256 1
a256 6
    
    if( !f )
        {
        Error( "Can't open "+s );
        return Err;
        }
d258 1
a258 3
    int datasize = 0;
    if( get_datasize( f, datasize ) ) return Err;
    if( f.sync() == EOF ) return Err;
d260 1
a261 1
    return Ok;
d265 1
a265 1
bool add_path_to_rif( string const &s )
d268 1
a268 6
    
    if( !f )
        {
        Error( "Can't open "+s );
        return Err;
        }
d276 1
a276 1
    if( f.sync() == EOF ) return Err;
a277 1
    return Ok;
d287 1
a287 1
bool rif::save_to_file( ostream &f )
d295 1
a295 1
    if( Err == putlong( f, datasize ) ) return Err;
d297 2
a298 2
    char rif_ident[] = "I=AM=RIF!";
    add_elem( '!', rif_ident, sizeof(rif_ident), f, datasize );
d301 1
a301 1
        add_elem( 'N', name_v.c_str(), name_v.length()+1, f, datasize );
d303 3
a305 10
    string temp;
    
    temp = (string)from_v;
    add_elem( 'F', temp.c_str(), temp.length()+1, f, datasize );

    temp = (string)to_v;
    add_elem( 'T', temp.c_str(), temp.length()+1, f, datasize );
    
    temp = (string)type_v;
    add_elem( 'Y', temp.c_str(), temp.length()+1, f, datasize );
d308 1
a308 1
        add_elem( 'D', desc_v.c_str(), desc_v.length()+1, f, datasize );
d310 5
a314 4
    char temp1[30];
    sprintf( temp1, "%ld", time(0) );
    temp = temp1;
    add_elem( 'C', temp.c_str(), temp.length()+1, f, datasize );
d316 2
a317 2
    temp = (string)(conf.aka[0]);
    add_elem( 'P', temp.c_str(), temp.length()+1, f, datasize );
d320 1
a320 3
        add_elem( '+', flags_v.c_str(), flags_v.length()+1, f, datasize );
    
    return f.fail() ? Err : Ok;
d332 1
a332 1
bool rif::create_in_dir( const string &dir, string &newname, const string &data, bool kill )
a334 7
    
    if( tempn=="" )
        {
        Error( "rif generation aborted - no temp file name" );
        return Err;
        }
    
d337 2
a338 2
    if( kill )  { if( frip_rename(data, tempn) == Err ) return Err; }
    else        { if( frip_copy(data, tempn)   == Err ) return Err; }
d343 1
a343 4
        {
        Error( "rif generation aborted - can't reopen file: "+tempn );
        return Err;
        }
d347 1
a347 1
    return save_to_file( temp );
@


1.2
log
@FripMail written
@
text
@d8 3
d106 1
a106 1
    fstream f( s, ios::bin|ios::in );
d116 1
d153 4
d265 1
a265 1
    fstream f( s, ios::bin|ios::in|ios::out|ios::nocreate );
d284 1
a284 1
    fstream f( s, ios::bin|ios::in|ios::out|ios::nocreate );
d300 1
d320 3
d347 3
d378 1
a378 1
    fstream temp( tempn, ios::bin|ios::in|ios::out );
@


1.1
log
@Initial revision
@
text
@d8 3
d74 1
a74 1
    printf("got datasize %ld\n", datasize );
d83 1
a83 1
    printf("seeking to %ld\n", ds );
d120 1
a120 1
    printf("end of tail %d, pos %d\n", end_of_tail, f.tellg() );
d134 5
a138 1
        printf("record %c, len %d, data <%.*s>\n", key, len, len, val );
d195 5
a199 1
    if( got_from != Yes || got_to != Yes || got_name != Yes )
d221 1
d223 2
a224 1
    rif_type::Mail, "MAIL",
d310 3
a312 2
    
    add_elem( 'N', name_v.c_str(), name_v.length()+1, f, datasize );
d375 1
@
