head	1.8;
access;
symbols;
locks;
comment	@ * @;


1.8
date	96.03.08.22.35.08;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.02.29.12.26.41;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	95.11.05.17.37.36;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	95.11.05.13.50.34;	author dz;	state Exp;
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
date	95.08.02.13.11.23;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.07.00.20.29;	author dz;	state Exp;
branches;
next	;


desc
@@


1.8
log
@2.09g
@
text
@// RCS Log /*FOLD00*/
/************************ UUCP to FIDO gate ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	Send stdin as letter
 *
 *      $Log: SendLett.c $
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



// Via 2:51/2@@fidonet @@19950821.200818.91 Itrack+ 1.2 EB000001
// ^AVia addr @@YYYYmmDD.hhMMss.uu Name Ver/subver

static string received2via( string s, string netname )
    {
    char date[50], via[120];

    string via_addr("Unknown");
    time_t t = 0;
    int i, j;
    
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
            debug("getindate(%s)", (s.c_str()) + i + 1 );
            break;
            }
        }
    
    static const tm tm_v = *(localtime(&t));
    
    int year = tm_v.tm_year;
    if( year < 1900 ) year += 1900;
    
    //strftime( date, 39, "%Y%m%d.%H%M%S", &tm_v );
    sprintf( date, "%02d%02d%02d.%02d%02d%02d",
            year,
            tm_v.tm_mon,
            tm_v.tm_mday,
            tm_v.tm_hour,
            tm_v.tm_min,
            tm_v.tm_sec
            );


    sprintf( via, "Via %.32s @@%s (%.10s)",
            (const char *)via_addr, 
            date, netname
           );
    
    return via;
    }

static void load_via( txt_Bag & via, const hl_val_Bag & vb, string netname )
    {
    hl_val_Cursor hvc( vb );

    for( hvc.setToFirst(); hvc.isValid(); hvc.setToNext() )
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

@


1.7
log
@received -> via
@
text
@d9 3
d159 4
d165 1
a165 1
            tm_v.tm_year,
@


1.6
log
@Using FTN_Msg
@
text
@d9 3
d67 1
a67 1

d75 2
d81 4
a84 1

d94 1
d104 2
d107 4
d112 69
@


1.5
log
@Before fido_msg rewrite
@
text
@d9 3
d63 1
a63 1
static void     add_hl( fido_msg &fm, char *hl );
d68 1
a68 1
send_letter( RFC_Msg &msg, fido_msg &fm, const string &received_for ) /*FOLD00*/
d104 1
a104 1
add_hl( fido_msg &fm, char *hl )
@


1.4
log
@First RFC_Msg-based version, that compiles and does something
@
text
@d9 3
a58 13
#if 0
#define		BUFS	1500

				/*********************************************
							   Local functions
				*********************************************/


static bool     is_first_from	( uchar *s );
static bool     is_new_hl		( uchar *s );
static bool     is_cont_hl		( uchar *s );

#endif // 0
a65 5
{
    
#if 1
    
/*
a66 11
        char buf[2000];
        sprintf( buf, "Received:  by %s (UU2FIDO %s) for %s; %s",
                (const char *)conf.def_domain.c_str(),
                U1_VER_STR,
                received_for,
                timeline( time(NULL), (const uchar *)conf.tz.c_str() )
               );
        fm.puts( buf );
    }
*/
    
d70 1
a70 1
    {
d77 1
a77 1
        {
d83 1
a85 3
    }
    

d94 1
a95 56
#else

	char			buf[ BUFS ];
	char			hl[ BUFS * 2 ];

	bool			header	= Yes;
	bool			have_hl	= No;
	recoder			r( conf.code_uf.c_str() );

//	if( use_recoder && (!r.valid()) )
//		error( EI_None, "Code table '%s' not found", (const char *)conf.code_uf.c_str() );


	sprintf( buf, "Received:  by %s (UU2FIDO %s) for %s; %s",
			(const char *)conf.def_domain.c_str(),
			U1_VER_STR,
			received_for,
			timeline( time(NULL), (const uchar *)conf.tz.c_str() )
			);
	fm->puts( buf );							// So it's text follows

//	rewind_to_text();
//	rewind( stdin );

	while( 1 )
		{
		char	*ee;

		if( fgets( buf, BUFS, stdin  ) == NULL )		// Next line
			{

			if( ferror( stdin ) )
				error( EI_Full, "Problems reading stdin" );

			if( have_hl )
				{
				add_hl( *fm, hl );                      // Write to the letter
				hl[0] = '\0';
				have_hl = No;							// Forget it
				}

			break;
			}

//		if( use_recoder )
//			r.rs( (uchar *)buf );								// Recode

		if( (ee = strpbrk( buf, "\r\n" )) != NULL )	// Kill any CRLFs
			*ee = '\0';

		if( header && is_first_from( (uchar *)buf ) )					// From[^:] ??
			{
			strcpy( hl, buf );						// Get beg. of headline
			have_hl = Yes;							// We have one
			continue;
			}
a96 38
		if( header && is_cont_hl( (uchar *)buf ) )			// headline continuation?
			{
			char	*pp = buf;

			if( !have_hl )
				fatal( EC_Got_Insane, EI_None, "Continuation withno headline!!");

			while( *pp == ' ' || *pp == ' ' )		// Skip ws
				pp++;

			strcat( hl, " " );						// Delimiter
			strcat( hl, pp );						// Add continuation
			continue;
			}

		if( have_hl )								// Not a continuation
			{
			add_hl( *fm, hl );                      // Write to the letter
			hl[0] = '\0';
			have_hl = No;							// Forget it
			}

		if( header && is_new_hl( (uchar *)buf ) )			// Oh, new headline
			{
			strcpy( hl, buf );						// Save it
			have_hl = Yes;							// Mark - we have one
			continue;
			}

		header = No;								// End of hdr reached

		fm->puts( buf );							// So it's text follows

		}

	return Ok;
#endif
}
a121 59


// obsolette functions /*fold00*/

#if 0

/****************************************************************************
						Headline detectors
****************************************************************************/

// Message leader?
bool
is_first_from( uchar *s ) /*FOLD01*/
	{
	if( strncmp( (const char *)s, "From", 4 ) )			// `From' withno `:'
		return No;

	if( s[4] == ':' )
		return No;

	return Yes;
	}

// Headline?
bool
is_new_hl( uchar *s ) /*FOLD01*/
	{
	while( *s )
		{
		if( isalpha( *s ) || *s == '-' )
			{
			s++;
			continue;
			}

		if( *s == ':' )
			return Yes;

		return No;
		}

	return No;
	}


// Headline continuation ?
bool
is_cont_hl( uchar *s ) /*FOLD01*/
	{
	if( *s == ' ' || *s == '\t' )
			return Yes;

	return No;
	}


#endif // 0


@


1.3
log
@Before switching to rfc_msg
@
text
@d1 1
d9 3
d51 2
d56 1
d68 2
d74 51
a124 3
bool
send_letter( fido_msg *fm, char *received_for )
	{
d216 2
a217 1
	}
d219 1
d245 1
d247 1
d253 1
d255 1
a255 1
is_first_from( uchar *s )					// Message leader?
d266 1
d268 1
a268 1
is_new_hl( uchar *s )						// Headline?
d288 1
d290 1
a290 1
is_cont_hl( uchar *s )						// Headline continuation ?
d299 1
a299 1

@


1.2
log
@C Set fixes
@
text
@d8 5
a12 2
 * Revision 1.1  1995/03/07  00:20:29  dz
 * Initial revision
d75 2
a76 2
	if( use_recoder && (!r.valid()) )
		error( EI_None, "Code table '%s' not found", (const char *)conf.code_uf.c_str() );
d87 1
a87 1
	rewind_to_text();
d94 1
a94 1
		if( fgets( buf, BUFS, in  ) == NULL )		// Next line
d97 1
a97 1
			if( ferror( in ) )
d110 2
a111 2
		if( use_recoder )
			r.rs( (uchar *)buf );								// Recode
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/uu2fido/vcs/sendlett.c_v  $
d76 1
a76 3
	sprintf( buf, "Received:  by %s (UU2FIDO %s)\r\n"
				  "           for %s;\r\n"
				  "           %s",
d80 1
a80 1
			timeline( time(NULL), (const char *)conf.tz.c_str() )
a83 1

d108 1
a108 1
			r.rs( buf );								// Recode
d113 1
a113 1
		if( header && is_first_from( buf ) )					// From[^:] ??
d120 1
a120 1
		if( header && is_cont_hl( buf ) )			// headline continuation?
d142 1
a142 1
		if( header && is_new_hl( buf ) )			// Oh, new headline
d192 1
a192 1
	if( strncmp( s, "From", 4 ) )			// `From' withno `:'
@
