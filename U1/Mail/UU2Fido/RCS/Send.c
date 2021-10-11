head	1.7;
access;
symbols;
locks;
comment	@ * @;


1.7
date	95.11.05.17.37.36;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	95.11.05.13.50.34;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	95.11.04.18.32.53;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.11.04.01.21.46;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.11.02.14.25.20;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.08.02.13.09.50;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.07.00.20.29;	author dz;	state Exp;
branches;
next	;


desc
@@


1.7
log
@Using FTN_Msg
@
text
@/************************ UUCP to FIDO gate ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *  Module  :   FIDO message handling
 *
 *      $Log: Send.c $
 *      Revision 1.6  1995/11/05 13:50:34  dz
 *      Before fido_msg rewrite
 *
 *      Revision 1.5  1995/11/04 18:32:53  dz
 *      Now returning message to sender in a case of
 *      address conversion errors.
 *      Some debugging done.
 *
 *      Revision 1.4  1995/11/04 01:21:46  dz
 *      First RFC_Msg-based version, that compiles and does something
 *
 *      Revision 1.3  1995/11/02 14:25:20  dz
 *      Before switching to rfc_msg
 *
 *      Revision 1.2  1995/08/02 13:09:50  dz
 *      RRq support, etc...
 *
 *      Revision 1.1  1995/03/07  00:20:29  dz
 *      Initial revision
 *
 *      
 *         Rev 1.12   12 Apr 1993 12:08:38   dz
 *      stdin replaced with in - support for uupc 5.06
 *      
 *         Rev 1.11   13 Mar 1993 23:29:34   dz
 *      Variable fido from field length
 *      
 *         Rev 1.10   11 Nov 1992 06:27:50   dz
 *      Minor reorganization of code
 *      
 *         Rev 1.9   23 Oct 1992 14:47:20   dz
 *      Received: ... for dest_addr
 *      
 *         Rev 1.8   05 Aug 1992 16:05:46   dz
 *      Ziplimit bug fix: int replaced with long
 *      
 *         Rev 1.7   29 Jun 1992 03:36:18   dz
 *      packetizer discontinued
 *      
 *         Rev 1.6   18 Jun 1992 11:26:24   dz
 *      Code to put newsarticles in spool written
 *      
 *         Rev 1.5   17 Apr 1992 11:30:18   dz
 *      ziplimit - unsigned comparison
 *      
 *         Rev 1.4   07 Apr 1992 12:28:48   dz
 *      Reply-To
 *      pass unknown back
 *      fsc35
 *      
 *         Rev 1.3   21 Oct 1991 05:15:58   dz
 *      Using set_date( time_t ) to prevent incorrect date field format
 *      
 *         Rev 1.2   11 Oct 1991 05:36:54   dz
 *      A lot of cosmetical changes
 *      News/echo gating support added.
 *      
 *         Rev 1.1   23 Sep 1991 17:27:10   dz
 *      Statistics added.
 *      Fido 'From' name is formed using UUCP 'from', if posiible.
 *      Code, used to prevent 'Intl' FIDO kludge removed.
 *      
 *         Rev 1.0   28 Aug 1991 00:25:30   dz
 *      Initial revision.
 *
 *
\*/

#include    "uu2fido.h"



bool Uu2FidoApp::
send( RFC_Msg &msg, const String &uu_dest )
    {
    FTN_Msg     fm;
    fido_user   fido_to;                        // Where to send in FIDO
    String	filedir;
    bool        error = No;

    debug("send( msg, %s )", (const char *)uu_dest );

    /*************************************************************
      Is it a viamail news or kiae-type batch to newsmaster? 
                       If so - put in spool.
    *************************************************************/

    if( is_batch( msg, uu_dest ) || is_news( msg, uu_dest ) )
        return spool_news();

    /*************************************************************
                        Convert address
    *************************************************************/

    switch( map_addr( msg, fido_to, filedir, uu_dest )) // Can't find out
        {                                               // where to send?
        case Yes:   // Address remapped successfully
            break;
            
        case No:    // Address was not remapped, but that's Ok
            return Ok;
            
        case Err:  // Too bad. We screwed seriously.
            return Err;
        }

    /*************************************************************
                  Not news - plain letter - send.
    *************************************************************/

    if( fm.new_msg( conf.netmail_dir.c_str() ) == Err )     // Create .MSG
        fatal( EC_UU2_Error, EI_Full, "Can't create message file" );    // Message lost!

    // NB!!
    // Don't move! send_file modifies attributes!
    
    bool has_rrc = msg.headline["Return-Receipt-To"].length() > 0 ? Yes : No;
    fm.add_attr	( 
       	FF_Private|FF_Local|FF_KillSent|(has_rrc ? FF_ReturnReceiptRequest : 0)
		);
    
    fm.set_subj( msg.headline["subject"] );
    fm.set_date( msg.get_gm_time() );

    fido_user fido_from = fido_user( ftn::match_aka( fido_to ), conf.magic.c_str() );

    string viafrom = (string) ftn::match_aka( fido_to );
    fm.set_via_addr( (const char *)viafrom );
    
    string uu_reply_to = msg.reply_address();
    int reptolen = uu_reply_to.length();            // Length of 'reply-to:'

    if( reptolen > 0 && reptolen < conf.from_field_size ) // Short enogth?
        fido_from.name( uu_reply_to );                    // Yeah, get it!

    fm.set_from( fido_from );
    fm.set_to  ( fido_to   );

    fm.set_fsc35_reply_to( fido_user( ftn::match_aka( fido_to ), conf.magic.c_str() ) );
    fm.set_fsc35_reply_addr( uu_reply_to );


    /*************************************************************
                          Copy contents
    *************************************************************/

    fm.rewind();

    debug( "2zip|~2zip: size %lu, limit %lu",
            (ulong)msg.size(), (ulong)conf.zip_limit );

    if( (((ulong)msg.size()) > ((ulong)conf.zip_limit)) /* || ui.uuencoded */ )
        {
        if( send_file( msg, fm, uu_dest, filedir ) == Err )  // Form attachment
            error = Yes;
        }
    else
        {
        if( send_letter( msg, fm, uu_dest ) == Err )    // Send as a letter
            error = Yes;
        }


    if( error )
        {
        fm.add_attr( FF_Sent|FF_KillSent );
        fm.detach();
        return Err;
        }
    else
        {
        log( "au", "Message to : %s", (const char *)((String) fido_to).c_str() );

        add_stat_fido(
            fido_to,
            (const uchar *)uu_reply_to,
            No /* BUG! */,                  // USSR
            No /* ui.uuencoded */,                   // File
            Yes,                            // To FIDO
            msg.size()
            );

        fm.detach();
        return Ok;
        }


    }




	/*************************************************************
						   Util.
	*************************************************************/


// BUG!
// This function is a piece of old junk and must be rewritten
// completely, possibly using traverse
static void
uucp_name( char *name, const char *to )
	{
	char	*p;

	strncpy( name, to, 100 );
	name[99] = '\0';
	if( (p = strpbrk( name, "@@%" ) ) != NULL )
		*p = '\0';

	if( (p = strrchr( name, '!' ) ) != NULL )
		strcpy( name, p+1 );

	strlwr( name );
	}


        /*************************************************************
                     Is this letter to be imported in echo?
	*************************************************************/

bool Uu2FidoApp::
is_news( RFC_Msg &msg, const string &uu_dest )
    {
    char	shrt[100];

    uucp_name( shrt, uu_dest );
    if( stricmp( shrt, conf.newsmaster.c_str() ) )
            return No;

    string dummy;
    if( msg.headline.get( dummy, "newsgroups" ) != Yes )
            return No;

    return Yes;
    }


	/*************************************************************
			Is it a kiae-type news batch to newsmaster?
	*************************************************************/

bool Uu2FidoApp::   
is_batch( RFC_Msg &msg, const string &uu_dest )
	{
	char	shrt[100];

	uucp_name( shrt, uu_dest );
	if( stricmp( shrt, conf.newsmaster.c_str() ) )
		return No;
        
        string x_batch;
        if( msg.headline.get( x_batch, "X-Batch" ) != Yes )
            return No;
        
	x_batch.lcase();
	if( strstr( x_batch, "compress" ) || strstr( x_batch, "pack" ) )
		{
		log( "B", "X-Batch == compress or pack" );
		return Yes;
		}

	return No;
	}



@


1.6
log
@Before fido_msg rewrite
@
text
@d8 3
d83 1
a83 1
    fido_msg    fm;
d161 1
a161 1
        if( send_file( msg, &fm, uu_dest, filedir ) == Err )  // Form attachment
@


1.5
log
@Now returning message to sender in a case of
address conversion errors.
Some debugging done.
@
text
@d8 5
d85 1
a85 2
    debug("send()" );

d99 10
a108 12
    if( remap_addr( fido_to, filedir, uu_dest ) == Err ) // Can't find out
        {                                                // where to send?

// NB!!
// We can't pass it back if we're called from rmail, or
// it will call us again... 1: goto 1;

        if( conf.pass_unknown_back )
            return run_uusmail( msg, uu_dest ); // Pass it back to uucp!
        else
            //            return Err;
            return reply_error( msg, uu_dest ); // Send notofocation letter
a117 1

a119 1

a125 7
/*
    fm.add_attr	( 
		FF_Private|FF_Local|FF_KillSent| 
		(ui.has_return_receipt_to ? FF_ReturnReceiptRequest : 0) 
		);
*/
    
a128 29
    
#if 0
    fido_user   fido_from = fido_user( ftn::match_aka( fido_to ), conf.magic.c_str() );
    
#pragma error here must be used address parts of headlines, not complete values
    string replyto = msg.headline["Reply-To"];
    string from = msg.headline["From"];
    int reptolen = replyto.length();                // Length of 'reply-to:'
    string uu_reply_to;

    if( reptolen > 0 )                              // Have Reply-To?
        {
        if( reptolen < conf.from_field_size )       // Short enogth?
            fido_from.name( replyto );              // Yeah, get it!

        uu_reply_to = replyto;
        }
    else
        {
        int fromlen = from.length();                // Length of 'from:' contents

        // Have place in header?
        if( fromlen > 0 && fromlen < conf.from_field_size )
            fido_from.name( from );                 // Yeah, get it!

        uu_reply_to = from;
        }
#else
    
d139 1
a139 5
    
#endif
    
    
    
a150 1

a167 3



a173 1

a175 3

//      printf("\rMessage to %-36s... ", fido_to.name );

d202 3
a218 2

//	debug("uucp_name() = '%s'", name );
a225 5



//bool
//is_news( uuinfo &ui, char *uu_dest )
d228 1
a228 1
{
d240 1
a240 1
}
a245 5



//bool
//is_batch( uuinfo &ui, char *uu_dest )
@


1.4
log
@First RFC_Msg-based version, that compiles and does something
@
text
@d8 3
a77 3

//    uuinfo      ui;                             // uuparse stores result here

a83 13
                        Analyze UUCP letter
    *************************************************************/


//    if( uuparse( msg, &ui ) == Err )                 // Parse headlines
//        return Err;

//    fflush( in );
    //    ui.textsize = filelength( fileno( in ));
//    ui.textsize = msg.size();  // а надо?


    /*************************************************************
a94 1

d96 1
a96 1
        {                                           // where to send?
a102 2
            {
//            rewind_to_text();
a103 1
            }
d105 2
a106 1
            return Err;
d167 2
a168 3
    // this will convert fido_user type to fido_addr (no username)
    // and convert address to string from
    fm.set_via_addr( (string) ((fido_addr&) fido_from) );
@


1.3
log
@Before switching to rfc_msg
@
text
@d8 3
d69 2
a70 2
bool
send( char *uu_dest )
d76 1
a76 1
    uuinfo      ui;                             // uuparse stores result here
d88 2
a89 2
    if( uuparse( &ui ) == Err )                 // Parse headlines
        return Err;
d91 3
a93 2
    fflush( in );
    ui.textsize = filelength( fileno( in ));
d101 1
a101 1
    if( is_batch( ui, uu_dest ) || is_news( ui, uu_dest ) )
d119 1
a119 1
            return run_uusmail( ui.from, uu_dest ); // Pass it back to uucp!
d136 7
d147 4
d152 2
a153 3
    fm.set_subj( ui.subject );
    fm.set_date( ui.gm_time );

d155 6
a160 3

    int reptolen = strlen( ui.replyto );            // Length of 'reply-to:'
    char *uu_reply_to;
d165 1
a165 1
            fido_from.name( ui.replyto );           // Yeah, get it!
d167 1
a167 1
        uu_reply_to = ui.replyto;
d171 1
a171 1
        int fromlen = strlen( ui.from );            // Length of 'from:' contents
d175 1
a175 1
            fido_from.name( ui.from );              // Yeah, get it!
d177 1
a177 1
        uu_reply_to = ui.from;
d179 18
a196 1

d212 1
a212 1
            (ulong)ui.textsize, (ulong)conf.zip_limit );
d214 1
a214 1
    if( (((ulong)ui.textsize) > ((ulong)conf.zip_limit)) || ui.uuencoded )
d216 1
a216 1
        if( send_file( &fm, uu_dest, ui, filedir ) == Err )  // Form attachment
d221 1
a221 1
        if( send_letter( &fm, uu_dest ) == Err )    // Send as a letter
d245 1
a245 1
            (uchar *)ui.from,
d247 1
a247 1
            ui.uuencoded,                   // File
d249 1
a249 1
            ui.textsize
d286 2
a287 2
	/*************************************************************
				Is this letter to be imported in echo?
a291 8
bool
is_news( uuinfo &ui, char *uu_dest )
	{
	char	shrt[100];

	uucp_name( shrt, uu_dest );
	if( stricmp( shrt, conf.newsmaster.c_str() ) )
		return No;
d293 14
a306 2
	if( strlen( ui.newsgroups ) == 0 )
		return No;
d308 2
a309 2
	return Yes;
	}
d318 5
a322 2
bool
is_batch( uuinfo &ui, char *uu_dest )
d329 7
a335 3

	strlwr( ui.x_batch );
	if( strstr( ui.x_batch, "compress" ) || strstr( ui.x_batch, "pack" ) )
d343 2
@


1.2
log
@RRq support, etc...
@
text
@d8 5
a12 2
 * Revision 1.1  1995/03/07  00:20:29  dz
 * Initial revision
d114 1
a114 1
            rewind_to_text();
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/uu2fido/vcs/send.c_v  $
d68 1
d74 2
d102 1
a102 1
    if( remap_addr( fido_to, uu_dest ) == Err ) // Can't find out
d129 4
a132 1
    fm.add_attr( FF_Private|FF_Local|FF_KillSent );
d179 1
a179 1
        if( send_file( &fm, uu_dest, ui ) == Err )  // Form attachment
d208 1
a208 1
            ui.from,
@
