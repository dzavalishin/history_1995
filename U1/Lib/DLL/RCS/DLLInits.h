head	1.1;
access;
symbols;
locks; strict;
comment	@ * @;


1.1
date	95.04.13.11.06.28;	author dz;	state Exp;
branches;
next	;


desc
@DLL startup/termination func. prototypes
@


1.1
log
@Initial revision
@
text
@extern "SYSTEM" void	mark_program_start_time();


extern "SYSTEM" void	close_log( void );
extern "SYSTEM" void	close_elf( void );
extern "SYSTEM" void	close_list( void );
extern "SYSTEM" void	close_stat( void );

@
