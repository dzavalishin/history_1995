/************************** Pack byte utilities ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *				Dmitry	Zavalishin	[DZ],
 *			      	Serge	Mednov		[MS],
 *				Gregory	Nickonov	[GN].
 *
 *	Module 	:	Pack input stream header.
 *
 *      $Log:   D:/animate/pack/vcs/pack.h_v  $
 *      
 *         Rev 1.0   24 Dec 1990 07:33:38   gn
 *      Initial revision.
 *
 *
\*/

#ifndef	PACK_H
#define	PACK_H

#ifndef	STYLE_H
#	include	<Style.h>
#endif	STYLE_H

#ifdef	PACK_C
#	define	STARTED		0x00
#	define	REPEAT		0x01
#	define	COMPRESS	0x02
#endif

struct	Pack {
	void	(*make_sym)(byte b);
	bool	exit_flag;

	/*	Internal variables ( may be zero! )	*/

	byte	b;
	char	count;
	uchar	status;

	Pack	( void (*make_sym) (byte b) );
	~Pack	( void );

	void	Do( byte sym );
	void	release_bytes( void );
	};

struct	Unpack {
	int	(*make_sym) (void);
	bool	exit_flag;

	/*	Internal variables ( may be zero! )	*/

	int	b;
	char	count;
	uchar	status;

	Unpack	( int (*make_sym) (void) );
	~Unpack	( void );

	int	Do( void );
	bool	Do( byte huge *to, ulong size );

	};






#endif	PACK_H