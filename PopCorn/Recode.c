/************************ PopCorn server ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	:	Code table convertor kernel
 *
 *      $Log: Recode.c $
 *      Revision 1.1  1996/09/17 23:20:43  dz
 *      Initial revision
 *
 *
 *
 *
 *
\*/

#include	<style.h>
//#include	<setup.h>
//#include	<log.h>
#include	<stddef.h>
#include	<string.h>
#include	<strng.h>
#include	"recode.h"
#include	"except.h"


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
        { "CP1251-KOI8",                (char *)cp1251_to_koi8  },
        { "KOI8-CP1251",                (char *)koi8_to_cp1251  },
        { NULL,  NULL },
        { NULL,  NULL },
        { NULL,  NULL },
        { NULL,  NULL }
    };

#define	CTT_SIZE	(sizeof(ct_tab)/sizeof(ctt))

  //extern setup_string recode_dir;
string recode_dir("codetab");

static char *load_table( const char *tabname )
    {
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
          throw Internal_Error( "load_table", "No more recoder slots", "load_table" );
    
    string fn = recode_dir + string("/") + string(tabname);
    FILE *fp = fopen( fn, "rb" );
    if( fp == NULL )
        throw Invalid_Condition( "load_table", "can't open file", fn );

    
    memset( buf, 0, 128 );
    ;
    
    if( (128 != fread( buf, 1, 128, fp )) || ferror( fp ) )
        {
        fclose( fp );
        throw Invalid_Condition( "load_table", "Can't read codetable", fn );
        }
    fclose( fp );
    
    ct_tab[i].name = strdup(tabname);
    ct_tab[i].tab  = new char [128];
    
    memcpy( ct_tab[i].tab, buf, 128 );
    return ct_tab[i].tab;
    }






recoder::recoder( const string & tabname )
    {
    tab = NULL;

    if( stricmp( tabname, "none" ) == 0 || tabname.is_empty() )
        return;

    for( int i = 0; i < CTT_SIZE; i++ )
        if( ct_tab[i].name && !stricmp( tabname, ct_tab[i].name ) )
            {
            tab = ct_tab[i].tab;
            return;
            }
    
    // Ok, here we can try to load something from file
    tab = load_table( tabname );
    //debug("loaded table %s", tabname );
    }





// Recode character
uchar recoder::rc( uchar c ) 
    {
    if( c < 0x80 || tab == NULL )
        return c;
    
    return tab[c-0x80];
    }


// Recode string
void recoder::rs( uchar *s )  
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


// Recode string
void recoder::rs( string &s )
    {
    if( tab == NULL )
        return;

    s.recode_upper_128( tab );
    }







