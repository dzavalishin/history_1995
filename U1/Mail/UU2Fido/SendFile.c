/************************ UUCP to FIDO gate ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	Send letter as file attachment.
 *
 *      $Log: SendFile.c $
 *      Revision 1.8  1996/02/29 12:26:20  dz
 *      codeback
 *
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





bool Uu2FidoApp::send_file( RFC_Msg &msg, FTN_Msg &fm, const string &received_for, const String &filedir )
    {
    char		arc_name[150];

    if( prepare_file( msg, arc_name, filedir.c_str() ) == Err )
        {
        fm.puts(">>> Error preparing file for transfer");
        fm.puts(">>> Message was lost, sorry :-(");
        return Ok;
        }

    char temp[200];
    fido_user fm_to;
    fm.get_to(fm_to);
    sprintf(temp, "frip send -kill %s %s",
            (const char *)arc_name,
            (const char *)((string)((fido_addr&)fm_to))
           );

    bool frip_success = No;
    if( conf.enable_frip && system(temp) == 0 )
        {
        frip_success = Yes;
        sprintf( temp, "\r>>> Message for %.100s was sent with FRIP",
                received_for );
        }
    else
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
                FF_KillFileSent| (frip_success ? 0 : FF_FileAttached)
               );
    fm.set_subj( arc_name );
    return Ok;
    }






