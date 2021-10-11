head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	96.03.27.10.46.53;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.03.09.11.33.00;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.24.01;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@current
@
text
@#include    "rfc_msg.h"
/****************************************************************************
				   Encoding functions prototypes
****************************************************************************/

//	General engine entry point

bool	encode_attaches( FTN_Msg &fm, RFC_Msg &out, long *files_sent );

//	Specific ones

bool	uuencode( char *infn, RFC_Msg &out );
@


1.2
log
@FTN_Msg
@
text
@d1 1
d8 1
a8 1
bool	encode_attaches( FTN_Msg &fm, FILE *out, long *files_sent );
d12 1
a12 1
bool	uuencode( char *infn, FILE *out );
@


1.1
log
@Initial revision
@
text
@d7 1
a7 1
bool	encode_attaches( fido_msg *fm, FILE *out, long *files_sent );
@
