head	1.1;
access;
symbols;
locks;
comment	@ * @;


1.1
date	95.03.11.18.26.07;	author dz;	state Exp;
branches;
next	;


desc
@@


1.1
log
@Initial revision
@
text
@#include <alloc.h>
#include <alloc.h>
#include "log.h"

static void
heap_tst( void )
	{
	if( heapcheck() == _HEAPCORRUPT )
		{
		error( EI_None, "Heap corrupt!");
//		geninterrupt(3);
		}
	}


void *
operator new( size_t s )
	{
	void *m;
	
	heap_tst();
	m = malloc( s );
	heap_tst();

	return m;
	}

void
operator delete( void *m )
	{
	heap_tst();
	free( m );
	heap_tst();
	}


	@
