/************************ UU2 FILE2UU ***************************\
 *
 *  Copyright (C) 1991, 1992 by Infinity Soft
 *
 *	Module 	:	FILE2UU header
 *
 *      $Log:   Q:/news/snews/vcs/snews.h_v  $
 *
 *
\*/

#ifndef	UU2FILE_H
#define	UU2FILE_H



#include	<style.h>
#include	<fido_msg.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<ctype.h>
#include	<errno.h>
#include	<time.h>
#include	<bag.h>

#include	"setup.h"
#include	"log.h"
#include	"tic.h"

#include	<elist.h>

/****************************************************************************
							   Constants
****************************************************************************/

/****************************************************************************
							   Data types
****************************************************************************/

/****************************************************************************
								Functions
****************************************************************************/

// scan.c
bool		scan_spool( const uchar *dir );

// ids.c
bool		get_ids( const uchar *name, String &ident, int &part_no, int &parts );

// xtract.c
bool		extract( Bag &group );

// mktemp.c
FILE *		mk_temp_file( char *o_fn );

// uudecode.c
bool		uudecode( const char *buf, FILE *out);

#endif	UU2FILE_H
