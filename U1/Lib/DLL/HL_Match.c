/************************ UU2 library ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:   Headline management functions
 *
 *      $Log: HL_Match.c $
 * Revision 1.2  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *      
 *         Rev 1.1   14 Nov 1992 23:47:32   dz
 *      Infinite loop avoided
 *      
 *         Rev 1.0   24 Oct 1992 18:01:24   dz
 *      Initial revision.
 *
 *
\*/

#include	<style.h>
#include	<stdlib.h>
#include	<ctype.h>

#include	<log.h>

#include	"hl_match.h"



static bool
go_next_hl( const char * &c )
    {
    while( *c && !isspace( *c ) )
        c++;
    while( isspace( *c ) )
        c++;
    return *c ? Yes : No;
    }

bool
hl_match_list( const char *hl, const char *list )
    {

//    debug( "hl_match_list( \"%s\", \"%s\" )", hl, list );

    while( isspace( *hl ) )
        hl++;
    while( isspace( *list ) )
        list++;

    while( 1 )
           {
           const char *hp = hl;
           bool match = Yes;

           for( hp = hl; *hp && *hp != ':'; hp++ )
               {
               if( tolower(*hp) == tolower( *list ) )
                   {
                   list++;
                   continue;
                   }

               if( *list == '*' )                   // Allways match!
                   return Yes;

               match = No;
               break;
               }

           if( isspace( *list ) || *list == '\0' )
               return Yes;
           else
               match = No;

           if( !match )
               if( go_next_hl( list ) == Yes )
                   continue;
               else
                   return No;
           }
    }

























