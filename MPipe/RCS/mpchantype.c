head	1.1;
access;
symbols;
locks;
comment	@ * @;


1.1
date	96.07.09.02.12.58;	author dz;	state Exp;
branches;
next	;


desc
@@


1.1
log
@Initial revision
@
text
@

#include "MpChanType.h"


const char *MP_SC_Control::type_name() const
    {
    return "control";
    }

int MP_SC_Control::send( const char *data, int len )
    {
    return 0;
    }


const char *MP_RC_Control::type_name() const
    {
    return "control";
    }

void MP_RC_Control::receive( const char *data, int len )
    {
    }



// -------------------------------------------------------------------------


const char *MP_SC_Main::type_name() const
    {
    return "main";
    }
    
    
int MP_SC_Main::send( const char *data, int len )
    {
    return 0;
    }


const char *MP_RC_Main::type_name() const
    {
    return "main";
    }

void MP_RC_Main::receive( const char *data, int len )
    {
    }
    





// -------------------------------------------------------------------------

const char *MP_SC_Chat::type_name() const
    {
    return "chat";
    }
int MP_SC_Chat::send( const char *data, int len )
    {
    return 0;
    }
    


const char *MP_RC_Chat::type_name() const
    {
    return "chat";
    }

void MP_RC_Chat::receive( const char *data, int len )
    {
    }




// -------------------------------------------------------------------------


const char *MP_SC_File::type_name() const
    {
    return "file";
    }

int MP_SC_File::send( const char *data, int len )
    {
    return 0;
    }


const char *MP_RC_File::type_name() const
    {
    return "file";
    }

void MP_RC_File::receive( const char *data, int len )
    {
    }




@
