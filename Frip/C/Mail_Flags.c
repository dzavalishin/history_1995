/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	FLAGS kludge
 *
 *      $Log: Mail_Flags.c $
 *      Revision 1.1  1997/01/31 09:40:11  dz
 *      Initial revision
 *
 *
 *
 *
\*/

#include	"mail.h"
#include	<ctype.h>


struct flags_tab
    {
    ulong    attr;
    char     *text;
    };


static flags_tab ftab[] =
    {
    FF_Private				,	"PVT",
    FF_Crash				,	"CRA",
    FF_Recd	       			,	"RCV",
    FF_Sent	       			,	"SNT",
    FF_FileAttached			,	"FIL",
    FF_KillSent				,	"K/S",
    FF_HoldForPickup		        ,       "HLD",
    FF_FileRequest			,	"FRQ",
    FF_ReturnReceiptRequest	        ,       "RRQ",
    FF_ArchiveSent			,	"A/S",
    FF_Direct				,	"DIR",
    FF_Immediate			,	"IMM",
    FF_ZoneGate				,	"ZON",
    FF_HubHostRoute			,	"HUB",
    FF_Xmail				,	"XMA",
    FF_KillFileSent			,	"KFS",
    FF_TruncateFileSent		        ,	"TFS",
    FF_Lock				,	"LOK",
    FF_ConfirmReceiptRequest            ,	"CFM",
    FF_FAX_HiRes			,	"HIR",
    FF_FAX_CoverLetter		        ,	"COV",
    FF_FAX_Signature		        ,	"SIG",
    FF_FAX_LetterHead		        ,	"LET",
    };

static int	nftab = sizeof(ftab) / sizeof(flags_tab);



void mail_print_flags( string &buf, ulong f )
    {
    buf = "FLAGS";
    
    for( int i = 0; i < nftab; i++ )
        {
        if( ftab[i].attr & f )
            {
            buf += " ";
            buf += ftab[i].text;
            }
        }
    }



ulong mail_scan_flags( const string &b )
    {
    const char *buf = b;

/*
    if( *buf == '\x1' )
        buf++;

    if( stricmp( buf, "flags" ) == 0 )
        buf += 5;
*/

    while( isspace( *buf ) )
        buf++;

    ulong r = 0;
    
    while( *buf )
        {
        
        for( int i = 0; i < nftab; i++ )
            {
            int l = strlen( ftab[i].text );
            
            if( strncmp( ftab[i].text, buf, l ) == 0 )
                {
                r |= ftab[i].attr;
                break;
                }
            }
        
        while( *buf && !isspace( *buf ) )
            buf++;
        while( isspace( *buf ) )
            buf++;
        }
    
    return r;
    }




