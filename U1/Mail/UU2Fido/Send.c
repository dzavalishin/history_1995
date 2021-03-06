/************************ UUCP to FIDO gate ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *  Module  :   FIDO message handling
 *
 *      $Log: Send.c $
 *      Revision 1.7  1995/11/05 17:37:36  dz
 *      Using FTN_Msg
 *
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
	if( (p = strpbrk( name, "@%" ) ) != NULL )
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



