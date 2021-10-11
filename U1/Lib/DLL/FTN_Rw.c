/************************ FIDO MSG file class ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:		Text/kludge lines read/write
 *
 *      $Log: FTN_Rw.c $
 *      Revision 1.2  1995/11/06 02:33:43  dz
 *      Fixes & cleanups caused by FTN_Msg introduction
 *
 *      Revision 1.1  1995/11/05 17:45:44  dz
 *      Initial revision
 *
 *      Revision 1.1  1995/11/05 17:37:36  dz
 *      Initial revision
 *
 *      Revision 1.2  1995/04/13  11:06:28  dz
 *      debug
 *
 *      Revision 1.1  1995/03/11  18:00:54  dz
 *      Initial revision
 *
 *      Revision 1.1  1995/03/11  18:00:54  dz
 *      Initial revision
 *      
 *         Rev 1.7   07 Jun 1993 17:38:06   dz
 *      io.h --> uu2io.h
 *      
 *         Rev 1.6   06 Nov 1992 19:20:06   dz
 *      Ver 1.12a save
 *      
 *         Rev 1.5   18 Jun 1992 17:30:42   dz
 *      Typo: was '0' instead of '\0'
 *      
 *         Rev 1.4   17 Jun 1992 20:50:42   dz
 *      Bug in formatter fixed: formatter wold fall into the
 *      infinite loop when longer then 80 characters word
 *      occured in the input .MSG
 *      
 *         Rev 1.3   04 Feb 1992 22:08:46   dz
 *      Added const modifier in appropriate places
 *      
 *         Rev 1.2   11 Oct 1991 05:07:10   dz
 *      Text formatting written
 *      
 *         Rev 1.1   12 Sep 1991 01:40:56   dz
 *      Reads() instead of fgets().
 *      
 *         Rev 1.0   28 Aug 1991 01:01:10   dz
 *      Initial revision.
 *
 *
\*/

#include	<style.h>
#include	<uu2io.h>
#include	<string.h>
#include	<time.h>

#include	"ftn_msg.h"


/****************************************************************************
                                Text management
****************************************************************************/



bool
FTN_Msg::rewind( void )
    {
    if( error )		return Err;
    
    cursor.setToFirst();
    
    return Ok;
    }


bool
FTN_Msg::clear_text( void )
    {
    if( error )		return Err;
    text.removeAll();
    return Ok;
    }


        /*************************************************************
                                    R/W msg
        *************************************************************/


bool
FTN_Msg::gets( string &s )
    {
    if( error )  return Err;

    do
        {
        if( !cursor.isValid() )
            return Err;
        s = cursor.element();
        cursor.setToNext();
        } while( s[0] == '\1' ); // Клуджи по боку

    return Ok;
    }



bool
FTN_Msg::geta( string &s )
    {
    if( error )  return Err;

    do
        {
        if( !cursor.isValid() )
            return Err;
        s = cursor.element();
        cursor.setToNext();
        } while( s[0] != '\1' ); // Hе клуджи по боку
    
    s = s.substr( 1 );           // Выкинем ^A
    
    return Ok;
    }

bool
FTN_Msg::puts( const string &s )
    {
    text.add( s );
    rewrite = Yes;
    return Ok;
    }


bool
FTN_Msg::puta( const string &s )
    {
    text.add( string("\1") + s );       // Добавим ^A
    rewrite = Yes;
    return Ok;
    }


