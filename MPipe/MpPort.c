/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: port class
 *
 *      $Log: MpPort.c $
 *      Revision 1.3  1996/07/09 16:03:58  dz
 *      debugging depacketizer
 *
 *      Revision 1.2  1996/07/09 02:12:58  dz
 *      Debugging has begone
 *
 *      Revision 1.1  1996/07/02 01:55:57  dz
 *      Initial revision
 *
 *      Revision 1.3  1996/06/30 22:58:25  dz
 *      update
 *
 *      Revision 1.2  1996/06/30 20:14:53  dz
 *      update
 *
 *      Revision 1.1  1996/06/30 16:11:38  dz
 *      Initial revision
 *
 *
 *
 *
\*/

//#define INCL_DOSPROCESS      // Sleep
#define INCL_DOSSEMAPHORES   /* Semaphore values */
//#define INCL_DOSDATETIME     /* Timer support    */
#define INCL_DOSERRORS       /* DOS error values */
#define INCL_DOSNMPIPES
#include <os2.h>


#include "MpPort.h"



void MP_LoopPort::set_pair( MP_LoopPort &p )
    { 
    pair = &p;
    connected_v = Yes;
    pwait.post();
    }

int MP_LoopPort::read( char *data, int size, bool sure )
    {
    //MP_Message( "MP_LoopPort::read waits for pair" );
    while( pair == 0 )
        pwait.sleep();
    
    //MP_Message( "MP_LoopPort::read waits for read spinlock" );
    MP_SLKey key(pair->rlock);   // obtain access

    //MP_Message( "MP_LoopPort::read looks for data" );
    while( connected_v && pair->wlen <= 0 )     // no data?
        pair->rwait.sleep();
    
    //MP_Message( "MP_LoopPort::read looks for 'sure' data" );
    while( sure && connected_v && pair->wlen < size ) // not enough data?
        pair->rwait.sleep();
    
    if( !connected_v ) throw Mp_Ex_Disconnect("MP_LoopPort::read");
    
    if( pair->wlen < size ) size = pair->wlen;
    memcpy( data, (void *)pair->wdata, size );
    pair->wlen -= size;
    pair->wwait.post();

    //MP_Dump( "MP_LoopPort::read", data, size );
    
    return size;
    }

int MP_LoopPort::write( char *data, int size )
    {
    MP_SLKey key(wlock);  // Lock it up!

    //MP_Dump( "MP_LoopPort::write", data, size );
    
    wdata = data;
    wlen = size;
    
    rwait.post(); // wakeup reader
    
    while( connected_v && wlen > 0 )
        wwait.sleep();

    return size - wlen;
    }
    
    
    
void MP_LoopPort::hangup()                      // Blocking!!
    {
    connected_v = No;
    rwait.post();
    wwait.post();
    if( pair ) 
        {
        pair->connected_v = No;
        pair->rwait.post();
        pair->wwait.post();
        }
    }




  // --------------------------------------------------------------------------
  // Named Pipes
  // --------------------------------------------------------------------------

void MP_NPPort::create_pipe()
    {
    caller_v = Yes;
    
    APIRET rc = DosCreateNPipe(
                                name_v,
                                &pipe_h,
                                NP_NOINHERIT|NP_ACCESS_DUPLEX,
                                NP_NOWAIT|NP_TYPE_BYTE|1, // one instance only
                                48*1024, 48*1024,
                                0 // 50 msec wait for instance
                               );

    if( rc != NO_ERROR )
        throw(Mp_Ex_Errno("MP_NPPort::create_pipe","DosCreateNPipe",rc));

    MP_Message("Waiting for Named Pipe Client to connect");
    while(1)
        {
        rc = DosConnectNPipe( pipe_h );
        if( rc == NO_ERROR ) return;
        if( rc != ERROR_PIPE_NOT_CONNECTED )
            throw(Mp_Ex_Errno("MP_NPPort::create_pipe","DosConnectNPipe",rc));
        MP_Sleep(256);
        }
    }

bool MP_NPPort::create_sem()
    {
    MP_Message("create sem %s",sem_name_v.c_str());
    APIRET rc = DosCreateEventSem( sem_name_v, &sem_h, DC_SEM_SHARED, FALSE );
    if( NO_ERROR == rc )
        return Yes;
    
    if( ERROR_DUPLICATE_NAME == rc )
        {
        MP_Message("sem %s exist",sem_name_v.c_str());
        return No;
        }

    throw(Mp_Ex_Errno("MP_NPPort::create_sem","DosCreateEventSem",rc));
    }


void MP_NPPort::open_pipe()
    {
    caller_v = No;

    ULONG action;
    MP_Message("Waiting for Named Pipe Server to respond");
    APIRET rc = DosOpen(
                        name_v,
                        &pipe_h,
                        &action,
                        0, FILE_NORMAL, // new size & attribs
                        OPEN_ACTION_OPEN_IF_EXISTS,
                          //OPEN_FLAGS_FAIL_ON_ERROR|OPEN_FLAGS_NOINHERIT|OPEN_ACCESS_READWRITE,
                        OPEN_ACCESS_READWRITE|OPEN_SHARE_DENYNONE,
                        NULL // ea
                       );
    
    if( rc == NO_ERROR ) return;
    throw(Mp_Ex_Errno("MP_NPPort::open_pipe","DosOpen",rc));
    }

bool MP_NPPort::open_sem()
    {
    MP_Message("open sem %s",sem_name_v.c_str());
    sem_h = 0;
    APIRET rc = DosOpenEventSem( sem_name_v.c_str(), &sem_h );
    if( NO_ERROR == rc )
        return Yes;
    
    if( ERROR_INVALID_NAME == rc )
        return No;
    
    throw(Mp_Ex_Errno("MP_NPPort::open_sem","DosOpenEventSem",rc));
    }
    

void MP_NPPort::sleep() // Wait for event
    {
      //MP_Message("sleep on sema %d", h );
    APIRET rc = DosWaitEventSem(sem_h, (ULONG) SEM_INDEFINITE_WAIT);
    if( rc == NO_ERROR)
        {
        ULONG count;
        DosResetEventSem( sem_h, &count );
        return;
        }
    
      //MP_Message("sleep: exc in sema %d", h );
    throw(Mp_Ex_Errno("MP_NPPort::sleep","DosWaitEventSem",rc));
    }

void MP_NPPort::post()  // trigger
    {
      //MP_Message("posting sema %d", h );
    APIRET rc = DosPostEventSem(sem_h);
    if( 
       rc == NO_ERROR || 
       rc == ERROR_ALREADY_POSTED || 
       rc == ERROR_TOO_MANY_POSTS
      )
        return;
    
      //MP_Message("post: exc in sema %d", h );
    throw(Mp_Ex_Errno("MP_NPPort::post","DosWaitEventSem",rc));
    }


MP_NPPort::MP_NPPort( const char *name, const char *server )
    {
    if( server == NULL || strlen(server)==0 )
        {
        name_v = "\\pipe\\"+string(name);
          //sem_name_v = "\\sem32\\pipe\\"+string(name);
        sem_name_v = "\\sem32\\"+string(name);
        }
    else
        {
        name_v = "\\\\"+string(server)+"\\pipe\\"+string(name);
          //sem_name_v = "\\\\"+string(server)+"\\sem32\\pipe\\"+string(name);
        sem_name_v = "\\\\"+string(server)+"\\sem32\\"+string(name);
        }

    if( create_sem() == No )
        {
          // Sem exist. Try to open
        if( open_sem() == No )
            throw(Mp_Ex_Fail("MP_NPPort::MP_NPPort","can't create & open sema",sem_name_v));
        else
            open_pipe();
        }
    else
        create_pipe();
    
    connected_v = Yes;
    }

MP_NPPort::~MP_NPPort()
    {
    DosClose( pipe_h );
    DosCloseEventSem( sem_h );
    }


int MP_NPPort::read( char *data, int size, bool sure )
    {
    int done = 0;
    int len = size;
    
    while( 1 )
        {
        ULONG bread;
        APIRET rc = DosRead( pipe_h, data, len, &bread );
        
        if( rc == ERROR_NO_DATA )
            {
            sleep();
            continue;
            }
        
        if( rc == ERROR_BROKEN_PIPE )
            {
            hangup();
            break;
            }
        
        if( rc != NO_ERROR )
            throw(Mp_Ex_Errno("MP_NPPort::read","DosRead",rc));

        if( bread > 0 ) post();
        
        done += bread;
        data += bread;
        len -= bread;
        
        if( done >= size  || (!connected_v) || ( done > 0 && sure == No ))
            break;
        
        sleep();
        }
    
    return done;
    }

int MP_NPPort::write( char *data, int size )
    {
    int done = 0;
    int len = size;

    while( 1 )
        {
        ULONG written;
        APIRET rc = DosWrite( pipe_h, data, len, &written );
    
        if( rc == ERROR_BROKEN_PIPE )
            {
            hangup();
            break;
            }

        if( rc != NO_ERROR )
            throw(Mp_Ex_Errno("MP_NPPort::write","DosWrite",rc));

        if( written > 0 ) post();

        done += written;
        data += written;
        len -= written;
        
        if(done >= size || !connected_v)
            break;
        
        sleep();
        }
    
    return done;
    }

void MP_NPPort::hangup()
    {
    connected_v = No;
    post();
    }












