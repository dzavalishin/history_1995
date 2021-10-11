#include    "rfc_msg.h"
/****************************************************************************
				   Encoding functions prototypes
****************************************************************************/

//	General engine entry point

bool	encode_attaches( FTN_Msg &fm, RFC_Msg &out, long *files_sent );

//	Specific ones

bool	uuencode( char *infn, RFC_Msg &out );
