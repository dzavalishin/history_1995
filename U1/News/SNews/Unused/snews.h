/************************ UUCP to FIDO convertor ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Header
 *
 *      $Log:   C:/net/gate/echo2uu/vcs/echo2uu.h_v  $
 *      
 *         Rev 1.0   06 Oct 1991 00:23:50   dz
 *      Initial revision.
 *
 *
\*/

#include	<style.h>
#include	<fido_msg.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<ctype.h>
#include	<errno.h>
#include	<time.h>

#include	"setup.h"
#include	"log.h"



/****************************************************************************
                               Data types
****************************************************************************/

struct fido_info
    {
    fido_addr       origin;
    char            msgid[80];
	char            reply[80];
	char			area[80];
    };




/****************************************************************************
                                Functions
****************************************************************************/


void		scan( void );						// Scan netmail for packets
bool		unpack( char *pkt_name );
bool		send( FILE *pkt_file );

void		error_letter( void );				// Send letter to master
bool        read_origin( fido_info &info, FILE *fp );

FILE *		mk_spool_file( void );
bool		abort_spool_file( void );
bool		close_spool_file( char *list_info );

bool		readl( char *buf, int len, FILE *fp );
bool		reads( char *buf, int len, FILE *fp );
bool		read_form( char *buf, FILE *fp );			// 80 bytes/line




