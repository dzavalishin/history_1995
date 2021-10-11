/************************ UU2 Library ***************************\
 *
 *	Copyright (C) 1991,1992 by Infinity Soft
 *
 *	Module 	:	Code table convertor kernel
 *
 *      $Log: Recode.c $
 *      Revision 1.4  1995/11/05 13:52:48  dz
 *      current.
 *
 * Revision 1.3  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.2  1995/04/07  10:17:05  dz
 * Tables moved out
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *
 *
 *
\*/

#include	<style.h>
#include	<setup.h>
#include	<log.h>
#include	<stddef.h>
#include	<string.h>
#include	<strng.h>
#include	"recode.h"


/****************************************************************************
                               Built-in tables
****************************************************************************/


#include     "recode.tbl"


struct ctt									// CodeTable table
	{
	char	*name;							// CodeTable name
	char	*tab;							// Table itself
	};

ctt	ct_tab[] =
    {
       	{ "CP866-KOI8",			(char *)cp866_to_koi8	},
       	{ "KOI8-CP866",			(char *)koi8_to_cp866	},
       	{ "LEO-CP866",			(char *)leo_to_cp866	},
      	{ "CP866-CP866",		(char *)cp866_to_cp866	},
        { "KOI8U-CP866U",               (char *)koi8u_to_cp866u },
        { "CP866U-KOI8U",               (char *)cp866u_to_koi8u },
        { NULL,  NULL },
        { NULL,  NULL },
        { NULL,  NULL },
        { NULL,  NULL },
        { NULL,  NULL },
        { NULL,  NULL }
    };

#define	CTT_SIZE	(sizeof(ct_tab)/sizeof(ctt))



char *
load_table( const char *tabname )
    {
    char fn[300];
    char buf[128];
    
    bool found = No;
    for( int i = 0; i < CTT_SIZE; i++ )
        {
        if( ct_tab[i].name == NULL )
            {
            found = Yes;
            break;
            }
        }
    
    if( !found )
        {
        error( EI_None, "No more recoder slots" );
        return NULL;
        }
    
    sprintf( fn, "%s/%s", conf.recode_dir, tabname );
    FILE *fp = fopen( fn, "rb" );
    if( fp == NULL )
        return NULL;
    
    memset( buf, 0, 128 );
    fread( buf, 1, 128, fp );
    
    if( ferror( fp ) ) error( EI_None, "Can't read codetable %s", tabname );
    fclose( fp );
    
    ct_tab[i].name = strdup(tabname);
    ct_tab[i].tab  = (char *)malloc( 128 );
    
    if( ct_tab[i].tab == NULL )
        {
        free( ct_tab[i].name );
        ct_tab[i].name = NULL;
        error( EI_None, "out of memory" );
        return NULL;
        }
    
    strncpy( ct_tab[i].tab, buf, 128 );
    return ct_tab[i].tab;
    }









recoder::
recoder( const char *tabname )
    {
    
    tab = NULL;
    
    if( stricmp( tabname, "none" ) == 0 || strlen( tabname ) == 0 )
        {
        alloced = No;
        bad = No;
        return;
        }
    
    bad = Yes;
    
    for( int i = 0; i < CTT_SIZE; i++ )
        if( ct_tab[i].name && !stricmp( tabname, ct_tab[i].name ) )
            {
            tab = ct_tab[i].tab;
            alloced = No;
            bad = No;
            return;
            }
    
    // Ok, here we can try to load something from file
    tab = load_table( tabname );
    if( tab != NULL )
        {
        debug("loaded table %s", tabname );
        alloced = No;
        bad = No;
        }
    }



recoder::~recoder()
    {
    }

bool
recoder::valid()						// This instnce was constructed
    {
    return (bool)!bad;
    }


uchar
recoder::rc( uchar c )					// Recode character
    {
    
    if( c < 0x80 || tab == NULL )
        return c;
    
    return tab[c-0x80];
    }


void
recoder::rs( uchar *s )					// Recode string
    {
    
    if( tab == NULL )
        return;
    
    while( *s )
        {
        if( *s < 0x80 )
            {
            s++;
            continue;
            }
        *s = tab[(*s)-0x80];
        s++;
        }
    }



void
recoder::rs( string &s )      // Recode string
    {
    if( tab == NULL )
        return;

    s.recode_upper_128( tab );
    }







