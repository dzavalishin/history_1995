head	1.8;
access;
symbols;
locks;
comment	@ * @;


1.8
date	96.02.29.12.26.20;	author dz;	state Exp;
branches;
next	1.7;

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
date	95.08.02.13.10.31;	author dz;	state Exp;
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
@codeback
@
text
@/************************ UUCP to FIDO gate ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	Send letter as file attachment.
 *
 *      $Log: SendFile.c $
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
 *      Revision 1.2  1995/08/02 13:10:31  dz
 *      C Set fixes (findfirst/next)
 *      rexx stuff
 *
 *      Revision 1.1  1995/03/07  00:20:29  dz
 *      Initial revision
 *
 *      
 *         Rev 1.8   12 Apr 1993 12:08:36   dz
 *      stdin replaced with in - support for uupc 5.06
 *      
 *         Rev 1.7   28 Nov 1992 23:26:28   dz
 *      String to const char *
 *      
 *         Rev 1.6   23 Oct 1992 14:48:50   dz
 *      Received for...
 *      
 *         Rev 1.5   18 Sep 1992 02:58:10   dz
 *      using class recoder
 *      
 *         Rev 1.4   07 Apr 1992 12:29:58   dz
 *      conditional decoding
 *      
 *         Rev 1.3   03 Feb 1992 23:50:34   dz
 *      StoreDir support written. Used to put
 *      generated file-attaches to instead of
 *      inboud files dir.
 *      
 *         Rev 1.2   27 Sep 1991 07:16:30   dz
 *      KOI8 -> CP866 conversion added.
 *      
 *         Rev 1.1   27 Sep 1991 01:05:36   dz
 *      'FileAttached' atribute was forgotten - fixed.
 *      Started to copy not from the beginning of file - fixed.
 *      
 *         Rev 1.0   25 Sep 1991 21:25:50   dz
 *      Initial revision.
 *
 *
 *
\*/

#include	"uu2fido.h"
#include	<rexx.h>

#include	<share.h>
#include	<uu2io.h>
#include	<process.h>

#define INCL_DOSFILEMGR
#define INCL_DOSERRORS
#include	<os2.h>

#define ATTRS2FIND	0x27




static int
newmax( char *mn )
	{
	char	*pp;

	if( !isdigit( *mn ) )
		return -1;

	if( (pp = strpbrk( mn, ". \t" )) != NULL && pp - mn <= 9 )
		pp = '\0';

	return atoi( mn );
	}


#ifndef max
inline int
max( int a, int b )
	{
	if( a > b )
		return a;
	return b;
	}
#endif


static int
new_temp( const char *filedir )
    {
    int			maxmsg = 0;
    char		mask[150];
    HDIR		h = HDIR_SYSTEM;
    FILEFINDBUF3	ff = { 0 };
    ULONG		count = 1;
    APIRET		rc;

    sprintf( mask, "%.120s\\%.4s*.*", filedir, (const char *)conf.attach_stem.c_str() );

    rc = DosFindFirst( mask, &h, ATTRS2FIND, &ff, sizeof(ff), &count, FIL_STANDARD );

    if( rc != NO_ERROR )
        {
        if( rc == ERROR_NO_MORE_FILES )
            {
            log( "#", "Nothing to do - no messages found" );
            return 0;
            }

        fatal( EC_OS_Error, EI_Full, "FindFirst rc = %d", rc );
        }



    do { maxmsg = max( maxmsg, newmax( ff.achName+4 ) ); } 
    while(
          count = 1, 
          (rc = DosFindNext( h, &ff, sizeof(ff), &count )) 
          == NO_ERROR );

    if( rc != ERROR_NO_MORE_FILES )
        fatal( EC_OS_Error, EI_Full, "FindNext rc = %d", rc );

    if( DosFindClose( h ) != NO_ERROR )
        error( EI_Full, "FindClose rc = %d", rc );

    return maxmsg;
    }



bool
Uu2FidoApp::copy_data( RFC_Msg &msg, FILE *out, bool codeback )
    {
    msg.set_saver_recoder( 0 );
        
    if( use_recoder && codeback )
        {
        recoder      r( conf.code_fu.c_str() );

        if( !r.valid() )
            error( EI_None, "Code table '%s' not found", (const char *)conf.code_uf.c_str() );
        else
            msg.set_saver_recoder( &r );
        }

    return msg.save( out );
    }




bool
Uu2FidoApp::prepare_file( RFC_Msg &msg, char *arc_name, const char *filedir )
    {
    char	tmp_name[150];
    int		fd;
    int		last;

    if( filedir == NULL || strlen( filedir ) == 0 )
        filedir = (const char *)conf.store_dir.c_str();

    while( 1 )
        {
        last = new_temp( filedir );

        sprintf( tmp_name, "%s\\%.4s%04d.txt", filedir, (const char *)conf.attach_stem.c_str(), last+1 );

        fd	= _sopen( tmp_name, O_WRONLY|O_CREAT|O_EXCL|O_TEXT, SH_DENYRW, 0666 );

        if( fd >= 0 )	// Opened!
                break;

        if( errno != EEXIST )
                return Err;
        }

    sprintf( arc_name, "%s\\%.4s%04d.zip", filedir, (const char *)conf.attach_stem.c_str(), last+1 );

    FILE *out = fdopen( fd, "w" );
    
    if( out == 0 )
        {
        _close( fd );
        return Err;
        }
    
    // Don't recode it to original codetable
    if( copy_data( msg, out, No ) == Err )
        {
        fclose( out );
        return Err;
        }

    fclose( out );

    string		Rexx_Out;

    if( Ok == CallRexx( conf.rexx_u2f_pack.c_str(), Rexx_Out, tmp_name, filedir ) )
            {
            strcpy( arc_name, Rexx_Out.c_str() );
            return Ok;
            }

    if( spawnlp( P_WAIT, "zip", "zip", "-j", arc_name, tmp_name, NULL ) != 0 )
            {
            error( EI_Full, "Can't compress attachment" );
            strcpy( arc_name, tmp_name );
            return Ok;
            }


    unlink( tmp_name );
    return Ok;									// Send packed :)
    }





bool
Uu2FidoApp::send_file( RFC_Msg &msg, FTN_Msg &fm, const string &received_for, const String &filedir )
    {
    char		arc_name[150];

    if( prepare_file( msg, arc_name, filedir.c_str() ) == Err )
        {
        fm.puts(">>> Error preparing file for transfer");
        fm.puts(">>> Message was lost, sorry :-(");
        return Ok;
        }
    else
        {
        char temp[200];
        sprintf( temp, "\r>>> Message for %.100s was sent in attached file",
                                   received_for );
        fm.puts(temp);

        sprintf( temp, ">>> Subject: %.150s", msg.headline["subject"] );
        fm.puts(temp);

        {
        long     sz = msg.size();
        char     prefix[2] = " ";
        
        if( sz > 1024*10 )
            {
            prefix[0] = 'K';
            sz /= 1024;
        
            if( sz > 1024*10 )
                {
                prefix[0] = 'M';
                sz /= 1024;
                fm.puts(">>> [UU2: More than 10Mb?? Gate sysop, probably, will kill");
                fm.puts(">>> either you or that file I made for you ;)]");
                }
            }
        
        sprintf( temp, ">>> Size: %ld %sbytes", sz, prefix );
        fm.puts(temp);
        }
        
        string newsgroups = msg.headline["newsgroups"];
        if( newsgroups.length() )
        {
             sprintf( temp, ">>> Newsgroups: %.150s", (const char *)newsgroups );
             fm.puts(temp);
        }

        {
        FILE	*hf = fopen( conf.attach_help_file.c_str(), "r" );
        if( hf != NULL )
            {
            const bs = 1024;
            char	buf[bs];

            while( fgets( buf, bs, hf ) != NULL )
                    fm.puts( buf );

            fclose( hf );
            }
        else
            error( EI_Full, "Can't open '%s'", (const char *)conf.attach_help_file.c_str() );
        }


        fm.add_attr(
                conf.truncate_sent_attaches ? 
                        FF_TruncateFileSent|FF_FileAttached :
                        FF_KillFileSent|FF_FileAttached
                );
        fm.set_subj( arc_name );
        }

    return Ok;
    }






@


1.7
log
@Using FTN_Msg
@
text
@d8 3
d154 1
a154 1
Uu2FidoApp::copy_data( RFC_Msg &msg, FILE *out )
d158 1
a158 1
    if( use_recoder )
d209 2
a210 1
    if( copy_data( msg, out ) == Err )
@


1.6
log
@Before fido_msg rewrite
@
text
@d8 3
d239 1
a239 1
Uu2FidoApp::send_file( RFC_Msg &msg, fido_msg *fm, const string &received_for, const String &filedir )
d245 2
a246 2
        fm->puts(">>> Error preparing file for transfer");
        fm->puts(">>> Message was lost, sorry :-(");
d254 1
a254 1
        fm->puts(temp);
d257 1
a257 1
        fm->puts(temp);
d272 2
a273 2
                fm->puts(">>> [UU2: More than 10Mb?? Gate sysop, probably, will kill");
                fm->puts(">>> either you or that file I made for you ;)]");
d278 1
a278 1
        fm->puts(temp);
d285 1
a285 1
             fm->puts(temp);
d296 1
a296 1
                    fm->puts( buf );
d305 1
a305 1
        fm->add_attr( 
d310 1
a310 1
        fm->set_subj( arc_name );
@


1.5
log
@Now returning message to sender in a case of
address conversion errors.
Some debugging done.
@
text
@d8 5
a63 1
//#include	<dir.h>
a101 2
//#define ATTRS2FIND  FA_RDONLY | FA_HIDDEN | FA_SYSTEM | FA_ARCH

d105 7
a111 8
	{
//	ffblk		ff;
	int			maxmsg = 0;
	char		mask[150];
	HDIR		h = HDIR_SYSTEM;
	FILEFINDBUF3	ff = { 0 };
	ULONG		count = 1;
	APIRET		rc;
d115 1
a115 5
/*
    if( findfirst( mask, &ff, ATTRS2FIND ) )
		{
		if( errno == ENOENT )
			return 0;			// It will start from 1st
d117 7
a123 5
		perror("Findfirst");
		fatal( EC_OS_Error, EI_Full, "findfirst error");
		}
*/
	rc = DosFindFirst( mask, &h, ATTRS2FIND, &ff, sizeof(ff), &count, FIL_STANDARD );
d125 2
a126 7
	if( rc != NO_ERROR )
		{
		if( rc == ERROR_NO_MORE_FILES )
			{
			log( "#", "Nothing to do - no messages found" );
			return 0;
			}
a127 2
		fatal( EC_OS_Error, EI_Full, "FindFirst rc = %d", rc );
		}
d130 5
d136 2
a137 1
	do {
d139 2
a140 1
		maxmsg = max( maxmsg, newmax( ff.achName+4 ) );
d142 2
a143 19
//		} while( findnext(&ff) == 0 );
		} while( 
			count = 1, 
			(rc = DosFindNext( h, &ff, sizeof(ff), &count )) 
				== NO_ERROR );


	if( rc != ERROR_NO_MORE_FILES )
		fatal( EC_OS_Error, EI_Full, "FindNext rc = %d", rc );

	if( DosFindClose( h ) != NO_ERROR )
		error( EI_Full, "FindClose rc = %d", rc );


//	if( errno != ENOENT )
//		fatal( EC_OS_Error, EI_Full, "findnext error");

	return maxmsg;
	}
d149 1
a149 1
{
a153 1
        //        recoder      r( conf.code_uf.c_str() );
d163 1
a163 1
}
d198 1
a198 1
    {
d201 1
a201 1
    }
@


1.4
log
@First RFC_Msg-based version, that compiles and does something
@
text
@d8 3
d172 2
a173 1
        recoder      r( conf.code_uf.c_str() );
@


1.3
log
@Before switching to rfc_msg
@
text
@d8 3
d54 1
d162 8
a169 5
static bool
copy_data( int fd )
	{
	char	buf[1000];
	int		len;
d171 5
a175 1
//	rewind_to_text();
d177 2
a178 5
	while( (len = fread( buf, 1, 1000, in  )) > 0 )
		{
		if( use_recoder )
			{
			recoder			r( conf.code_uf.c_str() );
a179 2
			if( !r.valid() )
				error( EI_None, "Code table '%s' not found", (const char *)conf.code_uf.c_str() );
a180 2
			r.rs( (uchar *)buf );								// Recode
			}
a181 3
		if( write( fd, buf, len ) != len )
			return Err;
		}
d183 6
a188 2
	return Ok;
	}
d190 2
d193 3
d197 1
d199 1
a199 6
static bool
prepare_file( char *arc_name, const char *filedir )
	{
	char	tmp_name[150];
	int		fd;
	int		last;
d201 2
a202 2
	if( filedir == NULL || strlen( filedir ) == 0 )
		filedir = (const char *)conf.store_dir.c_str();
d204 3
a206 3
	while( 1 )
		{
		last = new_temp( filedir );
d208 1
a208 1
		sprintf( tmp_name, "%s\\%.4s%04d.txt", filedir, (const char *)conf.attach_stem.c_str(), last+1 );
d210 13
a222 1
		fd	= _sopen( tmp_name, O_WRONLY|O_CREAT|O_EXCL|O_TEXT, SH_DENYRW, 0666 );
d224 1
a224 2
		if( fd >= 0 )	// Opened!
			break;
d226 1
a226 3
		if( errno != EEXIST )
			return Err;
		}
d228 5
a232 1
	sprintf( arc_name, "%s\\%.4s%04d.zip", filedir, (const char *)conf.attach_stem.c_str(), last+1 );
d234 6
a240 5
	if( copy_data( fd ) == Err )
		{
		close( fd );
		return Err;
		}
d242 3
a244 21
	close( fd );

	string		Rexx_Out;

	if( Ok == CallRexx( conf.rexx_u2f_pack.c_str(), Rexx_Out, tmp_name, filedir ) )
		{
		strcpy( arc_name, Rexx_Out.c_str() );
                return Ok;
		}

	if( spawnlp( P_WAIT, "zip", "zip", "-j", arc_name, tmp_name, NULL ) != 0 )
                {
                error( EI_Full, "Can't compress attachment" );
                strcpy( arc_name, tmp_name );
                return Ok;
                }


	unlink( tmp_name );
	return Ok;									// Send packed :)
	}
d251 3
a253 3
send_file( fido_msg *fm, char *received_for, const uuinfo &ui, const String &filedir )
	{
	char		arc_name[150];
d255 12
a266 12
	if( prepare_file( arc_name, filedir.c_str() ) == Err )
		{
		fm->puts(">>> Error preparing file for transfer");
		fm->puts(">>> Message was lost, sorry :-(");
		return Ok;
		}
	else
		{
		char temp[200];
		sprintf( temp, "\r>>> Message for %.100s was sent in attached file",
					   received_for );
		fm->puts(temp);
d268 2
a269 2
		sprintf( temp, ">>> Subject: %.150s", ui.subject );
		fm->puts(temp);
d272 1
a272 1
        long     sz = ui.textsize;
d274 1
a274 1

d279 1
a279 1

d288 10
a297 3

		sprintf( temp, ">>> Size: %ld %sbytes", sz, prefix );
		fm->puts(temp);
d300 6
a305 5
		if( strlen( ui.newsgroups ) )
			{
			sprintf( temp, ">>> Newsgroups: %.150s", ui.newsgroups );
			fm->puts(temp);
		    }
d307 2
a308 6
		{
		FILE	*hf = fopen( conf.attach_help_file.c_str(), "r" );
		if( hf != NULL )
			{
			const bs = 1024;
			char	buf[bs];
d310 5
a314 2
			while( fgets( buf, bs, hf ) != NULL )
				fm->puts( buf );
a315 5
			fclose( hf );
			}
		else
			error( EI_Full, "Can't open '%s'", (const char *)conf.attach_help_file.c_str() );
		}
d317 7
d325 2
a326 10
		fm->add_attr( 
			conf.truncate_sent_attaches ? 
				FF_TruncateFileSent|FF_FileAttached :
                                FF_KillFileSent|FF_FileAttached
			);
		fm->set_subj( arc_name );
		}

	return Ok;
	}
@


1.2
log
@C Set fixes (findfirst/next)
rexx stuff
@
text
@d8 6
a13 2
 * Revision 1.1  1995/03/07  00:20:29  dz
 * Initial revision
d164 1
a164 1
	rewind_to_text();
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/uu2fido/vcs/sendfile.c_v  $
d48 2
a49 2
#include	<dir.h>
#include	<dos.h>
d53 6
d77 1
a77 1

d85 1
d87 1
a87 2

#define ATTRS2FIND  FA_RDONLY | FA_HIDDEN | FA_SYSTEM | FA_ARCH
d91 1
a91 1
new_temp( void )
d93 1
a93 1
	ffblk		ff;
d96 4
d101 1
a101 2
//	sprintf( mask, "%.120s\\gate*.*", (const char *)conf.store_dir );
        sprintf( mask, "%.120s\\%.4s*.*", (const char *)conf.store_dir.c_str(), (const char *)conf.attach_stem.c_str() );
d103 1
d112 14
d130 7
a136 1
		maxmsg = max( maxmsg, newmax( ff.ff_name+4 ) );
a137 1
		} while( findnext(&ff) == 0 );
d139 9
a147 2
	if( errno != ENOENT )
		fatal( EC_OS_Error, EI_Full, "findnext error");
a160 1
//	rewind( stdin );
d171 1
a171 1
			r.rs( buf );								// Recode
d185 1
a185 1
prepare_file( char *arc_name )
d191 3
d196 1
a196 1
		last = new_temp();
d198 1
a198 2
//		sprintf( tmp_name, "%s\\gate%04d.txt", (const char *)conf.store_dir, last+1 );
		sprintf( tmp_name, "%s\\%.4s%04d.txt", (const char *)conf.store_dir.c_str(), (const char *)conf.attach_stem.c_str(), last+1 );
d200 1
a200 1
		fd	= open( tmp_name, O_WRONLY|O_CREAT|O_EXCL|O_TEXT|O_DENYALL, 0666 );
d209 1
a209 2
//	sprintf( arc_name, "%s\\gate%04d.zip", (const char *)conf.store_dir, last+1 );
	sprintf( arc_name, "%s\\%.4s%04d.zip", (const char *)conf.store_dir.c_str(), (const char *)conf.attach_stem.c_str(), last+1 );
d220 9
a228 2
#ifdef __OS2__
        if( spawnlp( P_WAIT, "zip", "zip", "-j", arc_name, tmp_name, NULL ) != 0 )
d230 1
a230 1
                error( EI_Full, "Can't run pkzip" );
d234 1
a234 8
#else
	if( spawnlp( P_WAIT, "pkzip", "pkzip", arc_name, tmp_name, NULL ) != 0 )
		{
		error( EI_Full, "Can't run pkzip" );
		strcpy( arc_name, tmp_name );
		return Ok;								// Send unpacked :(
		}
#endif
d245 1
a245 1
send_file( fido_msg *fm, char *received_for, const uuinfo &ui )
d249 1
a249 1
	if( prepare_file( arc_name ) == Err )
@
