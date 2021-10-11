head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	95.11.05.13.52.48;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.04.06.23.33.30;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.03.11.18.04.17;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.07.00.11.45;	author dz;	state Exp;
branches;
next	;


desc
@@


1.4
log
@current.
@
text
@/************************ UU2 Config ***************************\
 *
 *	Copyright (C) 1991,1992 by Infinity Soft
 *
 *	Module 	:	User table
 *
 *      $Log: FUserDef.c $
 * Revision 1.3  1995/04/06  23:33:30  dz
 * checkpoint
 *
 * Revision 1.3  1995/04/06  23:33:30  dz
 * checkpoint
 *
 * Revision 1.1  1995/03/07  00:11:45  dz
 * Initial revision
 *
 *
 *
\*/


#include	"userlist.h"
#include	"setup.h"
#include	"log.h"

#include	<stdio.h>


	/*************************************************************
							  C'tor/D'tor
	*************************************************************/


fido_user_def::
fido_user_def( const char *def )
	{
	char	*p;
	char	buf[120];

	while( *def && *def <= ' ' )
		def++;

	if( *def == 0 )
		{
		error( EI_None, "Empty user definition!");
		return;
		}

	strncpy( buf, def, 120 );
	buf[119] = '\0';
	for( p = buf; *p; p++ )
		if( *p <= ' ' )
			{
			*p = '\0';
			break;
			}

	if( (p = strchr( buf, '@@' )) != NULL )
		{
		int	l;
		char	db[81];

		*p = '\0';
		uu_name_v   = buf;
		strncpy( db, p+1, 80 );
		db[79] = '\0';

		p = db;

		l = strlen(p);
		if( l == 0 || p[l-1] == '.' )
			{
			if( conf.def_domain.length() + l >= 79 )
				{
				error( EI_None, "User definition has too long domain: %s", def );
				return;
				}

			strcat( p, conf.def_domain.c_str() );
			}

		uu_to_domain_v = strdup( db );

		}
	else
		{
		uu_name_v = buf;
		uu_to_domain_v = "";
		}

	while( *def && *def > ' ' )
		def++;

	while( *def && *def <= ' ' )
		def++;

    while( *def == '/' || *def == '-' )
           {
           def++;

           char     key[11], val[101];
           int      len;

           if( 2 != sscanf( def, "%10[^=]=%100s %n", key, val, &len ) )
               {
               error( EI_None, "Wrong option: '%s'", def );
               break;
               }

           def += len;

           if( stricmp( key, "side" ) == 0 )
               {
               if( stricmp( val, "uucp" ) )     uu_side_v = Yes;
               else
               if( stricmp( val, "u" ) )        uu_side_v = Yes;
               else
               if( stricmp( val, "fido" ) )     uu_side_v = No;
               else
               if( stricmp( val, "f" ) )        uu_side_v = No;
               else
                   error( EI_None, "Unknown side: %s", val );
               }
           else
           if( stricmp( key, "from" ) == 0 )
               uu_from_domain_v = val;
           else
           if( stricmp( key, "filedir" ) == 0 )
               filedir_v = val;
           else
               error( EI_None, "Unknown user modifier: -%s=%s", key, val );

           while( *def && *def <= ' ' )
		       def++;

           }

    if( strlen( uu_from_domain_v.c_str() ) == 0 )
        uu_from_domain_v = conf.def_domain;

	if( *def == 0 )
		{
		error( EI_None, "No FIDO addr. in user definition!");
		return;
		}

    fido_user_v.parse( def );
    }



fido_user_def::
fido_user_def( const fido_user_def &c )
	{
	uu_name_v           = c.uu_name_v;
	uu_from_domain_v    = c.uu_from_domain_v;
	uu_to_domain_v      = c.uu_to_domain_v;
    fido_user_v         = c.fido_user_v;
    uu_side_v           = c.uu_side_v;
	}

fido_user_def::
~fido_user_def( void )
	{
	}


	/*************************************************************
						   Operations
	*************************************************************/


fido_user_def & fido_user_def::
operator = ( const fido_user_def &c )
    {

    uu_name_v           = c.uu_name_v;
    uu_from_domain_v    = c.uu_from_domain_v;
    uu_to_domain_v      = c.uu_to_domain_v;
    fido_user_v         = c.fido_user_v;
    uu_side_v           = c.uu_side_v;
    filedir_v           = c.filedir_v;

    return *this;
    }






























@


1.3
log
@checkpoint
@
text
@d8 6
@


1.2
log
@-filedir modifier
@
text
@@


1.1
log
@Initial revision
@
text
@d7 3
a9 7
 *      $Log:   Q:/lib/uu2/vcs/fuserdef.c_v  $
 *      
 *         Rev 1.2   07 Jun 1993 17:49:36   dz
 *      Using String instead of allocated char[]
 *      
 *         Rev 1.0   15 Nov 1992 00:09:00   dz
 *      Initial revision.
d122 3
d169 1
a169 1
	{
d171 3
a173 3
	uu_name_v           = c.uu_name_v;
	uu_from_domain_v    = c.uu_from_domain_v;
	uu_to_domain_v      = c.uu_to_domain_v;
d176 1
d178 2
a179 2
	return *this;
	}
@
