/**--------------------   Graphics Windows   -------------------**\
 *                                                               
 *     Copyright (C) 1989 by DZ - Moscow, 110.6728		 
 *                                                               
 *     Module : syserr.c - system error list ( Russian )         
 *
 *	$Log:   C:/c/gw/vcs/syserr.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:19:08   dz
 *      Initial revision.
 *                                                               
 *     Revision 1.1     20 Dec 89				 
 *       Initial revision.                                       
 *                                                               
 *                                                               
\**-------------------------------------------------------------**/


/*****************************************************************
 *
 *   Defines the System Error List, containing the full messages for
 *   all errno values set by the library routines.
 *   Defines sys_errlist, sys_nerr.
 *
******************************************************************/

char *sys_errlist[] =
{
    /*  0 EZERO		*/  	"Error 0",
    /*  1 EINVFUNC	*/  	"Invalid function number",
    /*  2 ENOENT	*/	"No such file or directory",
    /*  3 ESRCH		*/	"No path",
    /*  4 EINTR		*/	"",
    /*  5 EIO		*/	"",
    /*  6 ENXIO		*/	"",
    /*  7 ECONTR	*/	"Memory block destroyed",
    /*  9 EINVMEM	*/	"Invalid memory block address",
    /* 20 E2BIG		*/	"Arg list too long",
    /* 21 ENOEXEC	*/	"Exec format error",
    /*  6 EBADF		*/	"Bad file number",
    /* 10 EINVENV	*/	"Invalid environment",	/* uniforum differs */
    /* 11 EINVFMT	*/	"Invalid format",
    /* 12 EINVACC	*/	"Invalid access code",
    /* 13 EINVDAT	*/	"Invalid data",
    /* 15 EINVDRV	*/	"Invalid drive",
    /* 16 ECURDIR	*/	"Attempt to remove CurDir",
    /* 17 ENOTSAM	*/	"Not same device",
    /* 18 ENMFILE	*/	"No more files",

    /*  8 ENOMEM	*/	"Not enough core",	/* uniforum differs */
    /*  5 EACCES	*/	"Permission denied",
    /* 14 EFAULT	*/	"",
    /* 15 ENOTBLK	*/	"",
    /* 16 EBUSY		*/	"",			/* uniforum differs */
    /* 35 EEXIST	*/	"File exists",
    /* 22 EXDEV		*/	"Cross-device link",
    /* 15 ENODEV	*/	"",
    /* 20 ENOTDIR	*/	"",
    /* 21 EISDIR	*/	"",
    /* 19 EINVAL	*/	"Invalid argument",
    /* 23 ENFILE	*/	"",
    /*  4 EMFILE	*/	"Too many open files",
    /* 25 ENOTTY	*/	"",			/* uniforum differs */
    /* 26 ETXTBSY	*/	"",
    /* 27 EFBIG		*/	"",
    /* 28 ENOSPC	*/	"No space left on device",
    /* 29 ESPIPE	*/	"",
    /* 30 EROFS		*/	"",
    /* 31 EMLINK	*/	"",
    /* 32 EPIPE		*/	"",
    /* 33 EDOM		*/	"Math argument",
    /* 34 ERANGE	*/	"Result too large",
    /* 35 EUCLEAN	*/	"",
    /* 36 EDEADLOCK	*/	"Resource deadlock would occur", /* uniforum differs */
    /* 37 UNKNOWN	*/	"Unknown error"
};

int sys_nerr = sizeof( sys_errlist ) / sizeof( sys_errlist[ 0 ] ) - 1;

/* The above array contains all the errors including unknown error # 37
   which is used if msg_num is unknown */


/* ***NOTE: Parameter _SYS_MSGMAX (in file syserr.h) indicates the length of
   the longest systerm error message in the above table.  When you add or
   modify a message, you must update the value _SYS_MSGMAX, if appropriate. */
