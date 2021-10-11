// RCS Log /*FOLD00*/
/************************ UUCP to FIDO gate ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	Send stdin as letter
 *
 *      $Log: SendLett.c $
 *      Revision 1.8  1996/03/08 22:35:08  dz
 *      2.09g
 *
 *      Revision 1.7  1996/02/29 12:26:41  dz
 *      received -> via
 *
 *      Revision 1.6  1995/11/05 17:37:36  dz
 *      Using FTN_Msg
 *
 *      Revision 1.5  1995/11/05 13:50:34  dz
 *      Before fido_msg rewrite
 *
 *      Revision 1.4  1995/11/04 01:21:46  dz
 *      First RFC_Msg-based version, that compiles and does something
 *
 *      Revision 1.3  1995/11/02 14:25:20  dz
 *      Before switching to rfc_msg
 *
 *      Revision 1.2  1995/08/02 13:11:23  dz
 *      C Set fixes
 *
 *      Revision 1.1  1995/03/07  00:20:29  dz
 *      Initial revision
 *
 *      
 *         Rev 1.8   07 Jun 1993 16:55:30   dz
 *      String --> const char *
 *      
 *         Rev 1.7   12 Apr 1993 12:08:32   dz
 *      stdin replaced with in - support for uupc 5.06
 *      
 *         Rev 1.6   28 Nov 1992 23:24:38   dz
 *      String to const char *
 *      
 *         Rev 1.5   11 Nov 1992 06:28:34   dz
 *      Flexible headlines elimination scheme used
 *      
 *         Rev 1.4   23 Oct 1992 14:48:00   dz
 *      Received: ... for dest_addr
 *      
 *         Rev 1.3   14 Jul 1992 15:29:08   dz
 *      Recoder rewritten using general mechanism
 *      Bug fix: last headline was not copied to letter sometimes.
 *      
 *         Rev 1.2   18 Jun 1992 11:27:14   dz
 *      Received: headline
 *      
 *         Rev 1.1   07 Apr 1992 12:29:22   dz
 *      conditional decoding (-u)
 *
 *         Rev 1.0   11 Oct 1991 05:38:12   dz
 *      Initial revision.
 *
 *
 *
\*/

// includes & definitions /*FOLD00*/

#include	"uu2fido.h"



static void     add_hl( FTN_Msg &fm, char *hl );
static void     load_via( txt_Bag & via, const hl_val_Bag & vb, string netname );


bool Uu2FidoApp::
send_letter( RFC_Msg &msg, FTN_Msg &fm, const string &received_for ) /*FOLD00*/
    {
    hl_Cursor hc( msg.headline );
    
    txt_Bag   via;
    
    for( hc.setToFirst(); hc.isValid(); hc.setToNext() )
        {
        const char *k = hc.element().key();
        const hl_val_Bag &vb = hc.element().val();
        
        if( stricmp( k, "received" ) == 0 ) load_via( via, vb, "UUCP" );
        if( stricmp( k, "x400-received" ) == 0 ) load_via( via, vb, "X400" );
        
        hl_val_Cursor hvc( vb );

        for( hvc.setToFirst(); hvc.isValid(); hvc.setToNext() )
            {
            const char *v = (const char *)hvc.element();
            char *s = new char[strlen(k)+strlen(v)+10];

            sprintf( s, "%s: %s", k, v );
            add_hl( fm, s );                      // Write to the letter
            // BUG? Who'll delete s?
            }
        }

    fm.puts( "" ); // Empty line

    txt_Cursor tc( msg.text );

    for( tc.setToFirst(); tc.isValid(); tc.setToNext() )
        fm.puts( tc.element() );
    
    txt_Cursor tvia( via );

    for( tvia.setToFirst(); tvia.isValid(); tvia.setToNext() )
        fm.puta( tvia.element() );
    
    
    return Ok;
    }



// Via 2:51/2@fidonet @19950821.200818.91 Itrack+ 1.2 EB000001
// ^AVia addr @YYYYmmDD.hhMMss.uu Name Ver/subver

static string received2via( string s, string netname )
    {
    char date[50], via[120];

    string via_addr("Unknown");
    time_t t = 0;
    int i, j;
//printf("received2via( '%s', '%s' )\n", s.c_str(), netname.c_str() );
    for( i = 0; s[i]; i++ )
        {
        if( (s[i] != 'b' && s[i] != 'B') || (s[i+1] != 'y' && s[i+1] != 'Y') )
            continue;
        if( i > 0 && !isspace(s[i-1]) )
            continue;
        if( !isspace(s[i+2]) )
            continue;
        
        i += 2; while( isspace(s[i])) i++;
        
        for( j = i; s[j]; j++ )
            if( isspace( s[j] ) )
                break;
        
        via_addr = s.substr( i, j-i );
        }
    
    for( i = s.length(); i >= 0; i-- )
        {
        if( s[i] == ';' )
            {
            t = getindate( (s.c_str()) + i + 1 );
//printf("getindate(%s) = %d\n", (s.c_str()) + i + 1, t );
            break;
            }
        }
    
    if( t > 0 )
        {
        /* static */ const tm tm_v = *(localtime(&t));
    
        int year = tm_v.tm_year;
        if( year < 1900 )
            year += 1900;
    
        strftime( date, 39, "%Y%m%d.%H%M%S.00.UTC", &tm_v );
        /*
        sprintf( date, "%02d%02d%02d.%02d%02d%02d",
                year,
                tm_v.tm_mon+1,
                tm_v.tm_mday,
                tm_v.tm_hour,
                tm_v.tm_min,
                tm_v.tm_sec
                );
        */
    
        }
    else
        sprintf( date, "??????.??????" );


    sprintf( via, "Via %.32s @%s (%.10s)",
            (const char *)via_addr, 
            date, netname
           );
    
//printf("out: %s\n", via );
    return via;
    }

static void load_via( txt_Bag & via, const hl_val_Bag & vb, string netname )
    {
    hl_val_Cursor hvc( vb );

    // for( hvc.setToFirst(); hvc.isValid(); hvc.setToNext() )
    for( hvc.setToLast(); hvc.isValid(); hvc.setToPrevious() )
        via.add( received2via(hvc.element(),netname) );
    }



// add_hl /*FOLD00*/

void
add_hl( FTN_Msg &fm, char *hl )
    {
	char	*orig = hl;

	if( strnicmp( hl, "from ", 5 ) == 0 )
		hl = "from_";

	if( !hl_match_list( hl, conf.mail_pass_hl.c_str() ) )
        return;

	if( hl_match_list( hl, conf.mail_hide_hl.c_str() ) )
        {
        fm.puta( orig );                  // Write it as hidden line
        return;
        }

	if( hl_match_list( hl, conf.mail_kill_hl.c_str() ) )
        return;

	fm.puts( orig );                      // Write it as normal line
	}

