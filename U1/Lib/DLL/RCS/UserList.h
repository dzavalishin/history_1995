head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	95.11.05.13.52.48;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.04.13.11.06.28;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.03.11.18.04.17;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.07.00.14.00;	author dz;	state Exp;
branches;
next	;


desc
@@


1.4
log
@current.
@
text
@/************************ UU2 userlist ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	UserList module header
 *
 *      $Log: UserList.h $
 * Revision 1.3  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.2  1995/03/11  18:04:17  dz
 * -filedir modifier
 *
 * Revision 1.1  1995/03/07  00:14:00  dz
 * Initial revision
 *
 *
\*/


#ifndef	USERLIST_H
#define	USERLIST_H

#include "strng.h"

#include	<fidoaddr.h>
#include	<malloc.h>

#pragma pack(1)


	/*************************************************************
						FIDO user definition
	*************************************************************/


class _Export fido_user_def
    {
private:
    String		    uu_name_v;          // Username
    String		    uu_to_domain_v;     // Used to compare with when delivering to fido
    String		    uu_from_domain_v;   // Used to produce uucp From: headline

    String		    filedir_v;          // Fileattaches for this user will go here

    fido_user               fido_user_v;

    bool                    uu_side_v;          // User is at the UUCP side of gate!

    void		    clear( void );

public:
                            fido_user_def( void ) {}
                            fido_user_def( const char *setup_line );
                            fido_user_def( const fido_user_def &copy );
                            ~fido_user_def( void );

    bool		    invalid( void )
        {
        return
        ( uu_name_v.is_empty() || uu_from_domain_v.is_empty() || (!fido_user_v.valid()) )
            ? Yes : No;
        }
                            

    inline const String &   uu_name( void ) const;
    inline void		    uu_name( const String & val );

    inline const String &   uu_from_domain( void ) const;
    inline void		    uu_from_domain( const String & val );

    inline const String &   uu_to_domain( void ) const;
    inline void		    uu_to_domain( const String & val );

    inline const String &   filedir( void ) const;


    inline fido_user        fido_address( void ) const;
    inline void             fido_address( const fido_user& val );

    inline bool             uu_side( void ) const;
    inline void             uu_side( bool );

    fido_user_def &	    operator = ( const fido_user_def &r );
    };

	/*************************************************************
				 FIDO user definition inline methods
	*************************************************************/


inline const String & fido_user_def::	
filedir( void ) const { return filedir_v; }


inline const String & fido_user_def::
uu_name( void ) const { return uu_name_v; }

inline void fido_user_def::
uu_name( const String & val )
	{
	uu_name_v = val;
	}


inline const String & fido_user_def::
uu_from_domain( void ) const { return uu_from_domain_v; }

inline void fido_user_def::
uu_from_domain( const String & val )
	{
	uu_from_domain_v = val;
	}


inline const String & fido_user_def::
uu_to_domain( void ) const { return uu_to_domain_v; }

inline void fido_user_def::
uu_to_domain( const String & val )
	{
	uu_to_domain_v = val;
	}



inline fido_user fido_user_def::
fido_address( void ) const { return fido_user_v; }

inline void fido_user_def::
fido_address( const fido_user& val ) { fido_user_v = val; }


inline bool
fido_user_def::uu_side( void ) const    { return uu_side_v; }

inline void
fido_user_def::uu_side( bool v )        { uu_side_v = v; }




        /*************************************************************
                         FIDO userlist definition
        *************************************************************/

const int		fido_userlist_max_users = 1000;

class _Export	fido_userlist
	{
	int					last_free_slot;

	fido_user_def		*users[fido_userlist_max_users];

public:
			fido_userlist( void );
			~fido_userlist( void );

	bool	add( const char *userfile_string );
	bool	get( fido_user_def &result, const char *uucp_name ) const;
        bool    get( fido_user_def &result, const fido_user &fa ) const;

	bool	foreach( bool (*func)( const fido_user_def &, void *arg ), void *arg );
	};


extern	fido_userlist _Export	fuser;



#pragma pack()


#endif

@


1.3
log
@debug
@
text
@d8 2
a9 2
 * Revision 1.2  1995/03/11  18:04:17  dz
 * -filedir modifier
d38 1
a38 1
	{
d40 3
a42 3
	String			uu_name_v;          // Username
	String			uu_to_domain_v;     // Used to compare with when delivering to fido
	String			uu_from_domain_v;   // Used to produce uucp From: headline
d44 1
a44 1
	String			filedir_v;          // Fileattaches for this user will go here
d46 1
a46 1
	fido_user       fido_user_v;
d48 1
a48 1
    bool            uu_side_v;          // User is at the UUCP side of gate!
d50 1
a50 1
	void			clear( void );
d53 4
a56 4
					fido_user_def( void ) {}
					fido_user_def( const char *setup_line );
					fido_user_def( const fido_user_def &copy );
					~fido_user_def( void );
d58 7
a64 6
	bool			invalid( void )
		{
		return
            ( uu_name_v.is_empty() || uu_from_domain_v.is_empty() || (!fido_user_v.valid()) )
			? Yes : No;
		}
d66 2
a67 2
	inline const String &	uu_name( void ) const;
	inline void				uu_name( const String & val );
d69 2
a70 2
	inline const String &	uu_from_domain( void ) const;
	inline void				uu_from_domain( const String & val );
d72 2
a73 2
	inline const String &	uu_to_domain( void ) const;
	inline void				uu_to_domain( const String & val );
d75 1
a75 1
	inline const String &	filedir( void ) const;
d78 1
a78 1
	inline fido_user        fido_address( void ) const;
d84 2
a85 3
	fido_user_def &		    operator = ( const fido_user_def &r );

	};
d143 3
a145 3
	/*************************************************************
					  FIDO userlist definition
	*************************************************************/
d147 1
a147 1
const int		fido_userlist_max_users = 400;
d161 1
a161 1
    bool    get( fido_user_def &result, const fido_user &fa ) const;
@


1.2
log
@-filedir modifier
@
text
@d8 6
d27 1
a27 1
#include	<alloc.h>
d37 1
a37 1
class fido_user_def
d61 1
a61 1
            ( uu_name_v == "" || uu_from_domain_v == "" || (!fido_user_v.valid()) )
d149 1
a149 1
class	fido_userlist
d167 1
a167 1
extern	fido_userlist	fuser;
@


1.1
log
@Initial revision
@
text
@d7 3
a9 4
 *      $Log:   Q:/lib/uu2/vcs/userlist.h_v  $
 *      
 *         Rev 1.1   07 Jun 1993 18:01:36   dz
 *      char[] --> String
a20 1
//#include	<string.h>
d38 2
d68 3
d84 4
@
