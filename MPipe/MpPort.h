/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: port class
 *
 *      $Log: MpPort.h $
 *      Revision 1.6  1996/07/09 02:12:58  dz
 *      Debugging has begone
 *
 *      Revision 1.5  1996/07/07 10:01:14  dz
 *      *** empty log message ***
 *
 *      Revision 1.4  1996/07/02 01:55:57  dz
 *      writing
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

#ifndef MPPORT_H
#define MPPORT_H

#include "style.h"

#include "MpUtil.h"


class MP_Port
    {
    protected:
        bool         connected_v;
        
    public:
        MP_Port() { connected_v = No; }
        ~MP_Port() { /*hangup();*/ }
        
        // Both read and write are blocking.
        //
        // read is guaranteed to return requested number of bytes
        // or throw exception if 'sure' flag is set.
        virtual int  read( char *data, int size, bool sure = No )  = 0;  
        virtual int  write( char *data, int size ) = 0;
        
        virtual void hangup() = 0;                       // Blocking!!

        virtual bool connected() const { return connected_v; }
    };


class MP_ComPort : public MP_Port
    {
    public:
        MP_ComPort( const char *name );
        ~MP_ComPort() { hangup(); }
            
        virtual int read( char *data, int size, bool sure = No );   // Blocking!!
        virtual int write( char *data, int size );  // Blocking!!
    
        virtual void hangup();                      // Blocking!!
    };

class MP_NPPort : public MP_Port
    {
    ulong     pipe_h;
    ulong     sem_h;
    string    name_v;
    string    sem_name_v;
    
    void      create_pipe();
    void      open_pipe();
    
    bool      create_sem();
    bool      open_sem();
    void      sleep();
    void      post();

    bool      caller_v;
    
    public:
        MP_NPPort( const char *name, const char *server = NULL );
        ~MP_NPPort();
            
        virtual int read( char *data, int size, bool sure = No );
        virtual int write( char *data, int size ); 
        virtual void hangup();

        bool    caller() { return caller_v; }

    };

class MP_TcpPort : public MP_Port
    {
    public:
        MP_TcpPort( const char *name );
            
        virtual int read( char *data, int size, bool sure = No );  
        virtual int write( char *data, int size );
        virtual void hangup();                
    };

class MP_LoopPort : public MP_Port
    {
    private:
        MP_LoopPort * volatile    pair;
        MP_Sem                    pwait;
        MP_Sem                    rwait;
        MP_Sem                    wwait;
        MP_SpinLock               rlock;
        MP_SpinLock               wlock;
        volatile char *           wdata;
        volatile int              wlen;
            
    public:
        MP_LoopPort( MP_LoopPort *p = NULL) : pair(p), wlen(0) {}
            
        virtual void set_pair( MP_LoopPort &p ); // { pair = p; wait.post(); }
        
        virtual int read( char *data, int size, bool sure = No );
        virtual int write( char *data, int size );
        virtual void hangup();
    };




#endif // MPPORT_H


