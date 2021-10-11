/************************ Animate ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module 	:	Make IS graph. file from "tiff" image file.
 *
 *      $Log:   D:/animate/tif2isg/vcs/tif2isg.h_v  $
 *      
 *         Rev 1.1   24 Dec 1990 02:49:50   gn
 *      New field (METHOD of squeeze) added.
 *      
 *         Rev 1.0   18 Dec 1990 22:34:14   gn
 *      Initial revision.
 *
 *
\*/


#ifndef	TIFF2IS_H
#define	TIFF2IS_H

#ifndef	STYLE_H
#	include	<style.h>
#endif


#include	"tiff.h"

#define	PRODUCER	0x01		// Tiff2Is converter


enum	IS_GBlocks {
	Header		= 0x00,		// This is a graph. file header
	One_plane	= 0x01,		// This is a one graph. plane
	All_planes	= 0x02,		// All planes (see ALLPL structure)
	};


struct	FHeader {
	byte	magic[4];		// Magic value.
	byte	nblocks;		// How many blocks in file
	long	first;			// Offset of a first block in file
	byte	who;			// Who is produce this file
	byte	method;			// How this file packed (0 - nonpacked)
	};

struct	_Block {
	byte		btype;		// Block type
	long		bsize;		// Block size
	long		bnext;		// Next block offset
	};


struct  PHeader {
	int		xsize;		// Picture X-size
	int		ysize;		// Picture Y-size
	byte		bpp;		// How many bytes per pixel
	};

struct	Summpl	{
	byte		color     : 4;	// 4 bits color (All 4 planes)
	byte		unused    : 2;	// Now unused
	byte		invisible : 2;	// This is a 'invisible' color
	};

#endif	TIFF2IS