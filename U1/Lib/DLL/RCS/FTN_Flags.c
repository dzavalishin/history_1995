head	1.1;
access;
symbols;
locks;
comment	@ *      @;


1.1
date	95.11.05.17.45.44;	author dz;	state Exp;
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
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:		FLAGS kludge defs.
 *
 *      $Log: FTN_Flags.c $
 *      Revision 1.1  1995/11/05 17:37:36  dz
 *      Initial revision
 *
 * Revision 1.2  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 *      
 *         Rev 1.0   09 Apr 1993 10:12:38   dz
 *      Initial revision.
 *
 *
\*/

#include	"ftn_msg.h"
#include	<ctype.h>


struct flags_tab
	{
	fmsg_attr	attr;
	char		*text;
	};


static flags_tab ftab[] =
	{

		FF_Private				,	"PVT",
		FF_Crash				,	"CRA",
		FF_Recd					,	"RCV",
		FF_Sent					,	"SNT",
		FF_FileAttached			,	"FIL",
		FF_KillSent				,	"K/S",
		FF_HoldForPickup		,	"HLD",
		FF_FileRequest			,	"FRQ",
		FF_ReturnReceiptRequest	,	"RRQ",
		FF_ArchiveSent			,	"A/S",
		FF_Direct				,	"DIR",
		FF_Immediate			,	"IMM",
		FF_ZoneGate				,	"ZON",
		FF_HubHostRoute			,	"HUB",
		FF_Xmail				,	"XMA",
		FF_KillFileSent			,	"KFS",
		FF_TruncateFileSent		,	"TFS",
		FF_Lock					,	"LOK",
		FF_ConfirmReceiptRequest,	"CFM",
		FF_FAX_HiRes			,	"HIR",
		FF_FAX_CoverLetter		,	"COV",
		FF_FAX_Signature		,	"SIG",
		FF_FAX_LetterHead		,	"LET",
	};

static int	nftab = sizeof(ftab) / sizeof(flags_tab);



bool
FTN_Msg::print_flags( string &buf, fmsg_attr f )
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
	return Ok;
	}



fmsg_attr
FTN_Msg::scan_flags( const string &b )
    {
    const char *buf = b;
    
    if( *buf == '\x1' )
        buf++;

    if( stricmp( buf, "flags" ) == 0 )
        buf += 5;

    while( isspace( *buf ) )
        buf++;
    
    fmsg_attr r = 0;
    
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
        
        while( !isspace( *buf ) )
            buf++;
        while( isspace( *buf ) )
            buf++;
        }
    
    return r;
    }




@
