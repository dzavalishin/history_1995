/************************ Animate ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module 	:	IS graph. file description.
 *
 *      $Log:   D:/animate/vcs/isg_fmt.h_v  $
 *      
 *         Rev 1.1   24 Dec 1990 02:51:20   gn
 *      New field (METHOD of squeezing added).
 *      
 *         Rev 1.0   18 Dec 1990 04:26:56   dz
 *      Initial revision.
 *
 *
\*/


#ifndef	ISG_FMT_H
#define	ISG_FMT_H

#ifndef	STYLE_H
#	include	<style.h>
#endif

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
	byte	method;			// How this file packed
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
	byte		unused    : 2;	// Reserved for VGA
	byte		invisible : 2;	// This is a 'invisible' color
	};

#endif	ISG_FMT_H