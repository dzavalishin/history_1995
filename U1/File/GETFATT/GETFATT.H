/************************ UU2 GetFatt ***************************\
 *
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :       Header
 *
 *      $Log:   C:/net/gate/fido2uu/vcs/fido2uu.h_v  $
 *
 *
\*/

#ifndef	GETFATT_H
#define	GETFATT_H

#include	<style.h>
#include	<fidoaddr.h>
#include	<uucpaddr.h>

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"setup.h"
#include	"log.h"


/****************************************************************************
							Data Types
****************************************************************************/

enum get_mode
	{
	GCopy,			// Just copy
	GMove,			// Kill original
	GTrunc,			// Copy & truncate
	};

/****************************************************************************
						Function prototypes
****************************************************************************/

//		getflo.c
bool			get_flo( const char *flo_name );
bool			scan_netmail( void );

//		getfile.c

bool			get_file( get_mode mode, const char *src_file );








#endif //	GETFATT_H
