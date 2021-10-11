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

#ifndef	FILE2UU_H
#define	FILE2UU_H



#include	<style.h>
#include	<fido_msg.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<ctype.h>
#include	<errno.h>
#include	<time.h>

#include	"setup.h"
#include	"log.h"
#include	"tic.h"

#include	<elist.h>

/****************************************************************************
							   Constants
****************************************************************************/

const long	uu_bytes_in_line = 45;	// UUENCODE puts 45 bytes in 60char line
const long	part_lines = 1000;		// It will be 4500 characters per part (UUDENCODE)
const long	part_bytes = part_lines * uu_bytes_in_line;

/****************************************************************************
                               Data types
****************************************************************************/

/****************************************************************************
                                Functions
****************************************************************************/

// scan.c
bool		scan_tics( void );

// process.c
bool		process_tic( const char *tic_name );

// enc_part.c
bool		encode_part
				(
				int 					part_no,		// 1-nnn
				int						n_parts,		// nnn
				long					p_bytes,		// bytes per part
				echo_def 			&	def,			// Echo definition
				tic_file			&	tic,			// Original file info
				FILE				*	fo,				// Put here
				FILE				*	fi				// From here
				);


// uuencode.h
bool	uuencode( FILE *fo, FILE *fi, long nbytes );



#endif	FILE2UU_H
