/************************ UU2FIDO ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :	SendMail (or Rmail :) starter
 *
 *      $Log: RunSMail.c $
 *      Revision 1.6  1996/03/08 22:35:08  dz
 *      2.09g
 *
 *      Revision 1.5  1996/02/29 12:25:27  dz
 *      noinherit
 *
 *      Revision 1.4  1995/11/04 18:32:53  dz
 *      Now returning message to sender in a case of
 *      address conversion errors.
 *      Some debugging done.
 *
 *      Revision 1.3  1995/11/04 18:29:57  dz
 *      Using pipe now.
 *
 *      Revision 1.2  1995/11/04 01:21:46  dz
 *      First RFC_Msg-based version, that compiles and does something
 *
 *      Revision 1.1  1995/03/07  00:20:29  dz
 *      Initial revision
 *      
 *         Rev 1.4   12 Apr 1993 12:08:40   dz
 *      stdin replaced with in - support for uupc 5.06
 *      
 *         Rev 1.3   28 Nov 1992 23:27:48   dz
 *      String to const char *
 *      
 *         Rev 1.2   23 Oct 1992 14:49:54   dz
 *      error()
 *      
 *         Rev 1.1   18 Jun 1992 11:31:22   dz
 *      Handling of different UUCP packages added
 *      
 *         Rev 1.0   07 Apr 1992 12:32:34   dz
 *      Initial revision.
 *
 *
\*/


#include "uu2fido.h"
#include <process.h>

#define INCL_DOSQUEUES   /* Queue values */
#include <os2.h>
#include <stdio.h>



bool Uu2FidoApp::
run_uusmail( RFC_Msg &msg, const string &to )
    {
    int		ex;
    
    debug("run_uusmail( msg, %s )", (const char *)to);
    
    string from = msg.envelope.from_string();
    
    if( from.length() == 0 )
        from = "uucp";


    ulong    ReadHandle;    /* Pointer to the read handle      */
    ulong    WriteHandle;   /* Pointer to the write handle     */
    APIRET   rc;            /* Return code                     */
    const PipeSize = 4096;

    rc = DosCreatePipe(&ReadHandle, &WriteHandle, PipeSize);
    if (rc != 0)
        {
        error( EI_Full, "DosCreatePipe error: return code = %ld", rc );
        return Err;
        }
    
    int stdin_save = dup( 0 );
    
    if( 0 != dup2( ReadHandle, 0 ) )
        {
        error( EI_Full, "Can't dup2" );
        return Err;
        }
    
    DosClose( ReadHandle );
    
    DosSetFHState( WriteHandle, OPEN_FLAGS_NOINHERIT );
    DosSetFHState( stdin_save, OPEN_FLAGS_NOINHERIT ); 

    const char *argv[20];
    int         argc = 1;
    
    argv[0] = "rmail";
    
    if( conf.uucp == UUCP_Ache30 || conf.uucp_rmail_ffrom )
        {
        argv[argc++] = "-f";
        argv[argc++] = from;
        debug("-f %s", (const char *)from );
        }
    
    if( conf.uucp == UUCP_Ache30 && from_Unix )
        {
        argv[argc++] = "-u";
        debug("-u");
        }
    
    argv[argc++] = to;
    
    argv[argc++] = 0;     // End of args
    
    bool ret = Ok;
    
    if( spawnv( 
               P_NOWAIT,
               (char *)(const char *)conf.rmail_exe_name,
               // "./rmail.exe",
               (char **)argv )
       < 0 )
        {
        error( EI_Full, "Can't execute %s", 
              (const char *)conf.rmail_exe_name.c_str() );
        
        ret = Err;
        }
    
    
    FILE *fp = fdopen( WriteHandle, "w" );
    if( fp == NULL )
        {
        error( EI_Full, "Can't fdopen our end of pipe" );
        DosClose( WriteHandle );
        ret = Err;
        }
    
    if( ret != Err )
        {
        // Stuff RFC_Msg into the open end of pipe.
        // Copy_data will handle recode
        if( Err == copy_data( msg, fp ) ) ret = Err;
        }
    
    if( fp != NULL )
        fclose( fp );
    
    if( 0 != dup2( stdin_save, 0 ) )
        {
        error( EI_Full, "Can't dup2( %d, 0 )", stdin_save );
        ret = Err;
        }
    
    close( stdin_save );

    return ret;
    }


bool Uu2FidoApp::
reply_error( RFC_Msg &msg, const string &was_to )
    {
    RFC_Msg  re;
    
    static   id = 0;
    char     ids[40], ts[40];
    sprintf( ids, "%d", id++ );
    sprintf( ts, "%ld", time(NULL) );
    
    string   tl = string(timeline(time(NULL), conf.tz));
    
    re.headline.add_hl( "Subject", "U1: Unable to deliver message" );
    re.headline.add_hl( "From", string("U1@")+conf.def_domain );
    re.headline.add_hl( "To", msg.errors_reply_address() );
    re.headline.add_hl( "Date", tl );
    re.headline.add_hl( "Message-ID",
                    string("ErrMsg-")+
                    string(ids)+
                    string("-")+
                    string(ts)+
                    string("@")+
                    conf.def_domain
                   );
    
    re.text.add( string("Your message to ")+was_to+string(" cannot be delivered") );
    re.text.add( "because address is either incorrect or mistyped." );
    re.text.add( "");
    re.text.add( "If you sending mail to a Fido-type network address, please,");
    re.text.add( "make sure that address is formed correctly. Here is an example:");
    re.text.add( "");
    re.text.add( "Fido address:      Joe Foobar, 32:1024/16.8");
    re.text.add( "Internet notation: Joe_Foobar@p8.f16.n1024.z32.fidonet.org");
    re.text.add( "");
    re.text.add( "Text of your message follows:");
    re.text.add( "-----------------------------");
    re.text.add( "");
    
    msg.set_saver_recoder( 0 );
    msg.save( re );
    
    return run_uusmail( re, msg.errors_reply_address() );
    }





