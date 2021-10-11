/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	:  main
 *
 *      $Log: Mp.c $
 *      Revision 1.6  1996/07/10 18:27:24  dz
 *      something goes through, framer/frame classes
 *      are work somehow.
 *
 *      Revision 1.5  1996/07/09 16:03:58  dz
 *      debugging depacketizer
 *
 *      Revision 1.4  1996/07/09 02:12:58  dz
 *      Debugging has begone
 *
 *      Revision 1.3  1996/07/09 00:40:10  dz
 *      spinlock test
 *
 *      Revision 1.2  1996/07/07 10:01:14  dz
 *      *** empty log message ***
 *
 *      Revision 1.1  1996/07/02 01:55:57  dz
 *      Initial revision
 *
 *
 *
 *
 *
\*/

#include "Mp.h"
#include <stdio.h>

#include <fstream.h>

FILE *logfile = NULL;


#if 0

static MP_SpinLock sl;

extern "OPTLINK" void sltest( void *_i )
    {
    int i = (int)_i;

    while(1)
        {
        MP_Message( "%d: unlocked", i );
        MP_Sleep(512);
        MP_Message( "%d: locking", i );

            {
            MP_SLKey key(sl);
            MP_Message( "%d: locked", i );

            MP_Sleep(512);
            }

        
        }
    
    }

_beginthread( sltest, NULL, 64*1024, (void *)1 );
_beginthread( sltest, NULL, 64*1024, (void *)2 );
_beginthread( sltest, NULL, 64*1024, (void *)3 );
_beginthread( sltest, NULL, 64*1024, (void *)4 );

#endif


#if 0
extern "OPTLINK" void test_thread( void *o )
    {
    printf("thread started, got %d\n", o );
    MP_I_am_crazy();
    MP_Message("thread started, got %d\n", (int)o );
    
    }
MP_Thread( test_thread, (void *)12345 );
  //_beginthread( test_thread, 0, 8192, (void *)12345 );
#endif


struct port_reader_param
    {
    char *fn;
    MP_Port &src;
    };

extern "OPTLINK" void port_reader( void *o )
    {
    port_reader_param p = *(port_reader_param*) o;

    ofstream of( p.fn );
    of.setbuf( 0, 0 ); // unbuffered!

    try
        {
        int n;
        const bs = 1;
        char buf[bs];
        
        while( (n = p.src.read( buf, bs )) > 0 )
            of.write( buf, n );
        }
    catch( Mp_Ex ex )
        {
        MP_Message(
                   "Exc in %s: %s (%s), port_reader (%s)",
                   ex.where.c_str(), ex.what.c_str(), ex.why.c_str(), p.fn
                  );
        }
    catch(...)
        {
        MP_Message("unknown exc in port_reader (%s)", p.fn );
        }
    
    }


#define TEST_ON_PIPE 1

int main()
    {

    try
        {

#if TEST_ON_LOOP
        logfile = fopen( "log", "w" );
        setbuf( logfile, NULL );
        
        MP_LoopPort    lp1, lp2;
        
        lp1.set_pair( lp2 );
        lp2.set_pair( lp1 );
        
        //MP_Message("Initializing 1st session");
        MP_Session    s1( Yes, lp1 );
        //MP_Message("Initializing 2nd session");
        MP_Session    s2( No,  lp2 );
        
#if 0
        //MP_Message("Initializing 1st channels group");
        MP_Channels ch1( Yes, (MP_Port &)s1 );
        //MP_Message("Initializing 2nd channels group");
        MP_Channels ch2( No,  (MP_Port &)s2 );
#endif

        port_reader_param p1 = { "recv1", s1 };
        port_reader_param p2 = { "recv2", s2 };

        MP_Thread( port_reader, (void *)&p1 );
        MP_Thread( port_reader, (void *)&p2 );


        char c;
        while( (c = getchar()) != 'q' )
            {
            s1.write( "char for c1: ", 5 );
            s1.write( &c, 1 );
            s1.write( "\r\n", 2 );

            s2.write( "char for c2: ", 5 );
            s2.write( &c, 1 );
            s2.write( "\r\n", 2 );
            }
#endif

#if TEST_ON_PIPE
        MP_NPPort    np("mpipe");
        
        logfile = fopen( np.caller() ? "call_log" : "answer_log", "w" );
        setbuf( logfile, NULL );
        
        MP_Session    s( np.caller(), np );
        
#if 0
        MP_Channels ch( np.caller(), (MP_Port &)s );
#endif
        
        port_reader_param p = { np.caller() ? "call_side" : "answer_side", s };
        
        MP_Thread( port_reader, (void *)&p );
        
        
        char buf[1024];
        while( fgets(buf, 1000, stdin) != NULL )
            {
            s.write( buf, strlen(buf) );
            }
#endif
        
        MP_Message("EOF, exiting");
        }
    catch( Mp_Ex ex )
        {
        MP_Message("Exc in %s: %s (%s), main", ex.where.c_str(), ex.what.c_str(), ex.why.c_str() );
        }
    
    return 0;
    }




