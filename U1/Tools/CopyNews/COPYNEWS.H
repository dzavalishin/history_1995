/************************ UU2 CopyNEWS ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :       Header
 *
 *      $Log:   C:/net/gate/fido2uu/vcs/fido2uu.h_v  $
 *
 *
\*/

#ifndef	COPYNEWS_H
#define	COPYNEWS_H

#include	<style.h>
//#include	<fidoaddr.h>
//#include	<uucpaddr.h>

#include	<io.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

//#include	"setup.h"
//#include	"log.h"


extern bool        f_silent;


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

//		getfile.c

bool			get_file( get_mode mode, const char *src_file, const char *dst_file );








#endif //	COPYNEWS_H
