/************************ UUCP to FIDO gate ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	Address conversion
 *
 *      $Log: Address.c $
 *      Revision 1.7  1995/11/05 13:50:34  dz
 *      Before fido_msg rewrite
 *
 *      Revision 1.6  1995/11/04 18:32:53  dz
 *      Now returning message to sender in a case of
 *      address conversion errors.
 *      Some debugging done.
 *
 *      Revision 1.5  1995/11/04 01:21:46  dz
 *      First RFC_Msg-based version, that compiles and does something
 *
 *      Revision 1.4  1995/11/02 14:25:20  dz
 *      Before switching to rfc_msg
 *
 *      Revision 1.3  1995/08/02 13:04:48  dz
 *      rexx support added
 *
 *      Revision 1.1  1995/03/07  00:20:29  dz
 *      Initial revision
 *
 *
 *         Rev 1.10   12 Apr 1993 12:08:26   dz
 *      stdin replaced with in - support for uupc 5.06
 *
 *         Rev 1.9   28 Nov 1992 23:20:44   dz
 *      Style fixes
 *
 *         Rev 1.8   11 Nov 1992 06:26:24   dz
 *      New address conversion scheme used
 *
 *         Rev 1.7   23 Oct 1992 14:42:06   dz
 *      Added bang-path-type p.f.n.z or z!n!f!p addresses support
 *      In 'First.Last@pfnz' point gets replaced with space.
 *
 *         Rev 1.6   18 Sep 1992 02:56:30   dz
 *      removed call to group2area
 *
 *         Rev 1.5   18 Jun 1992 11:24:06   dz
 *      remap_addr() - makes local copy of address to not to modify arg.
 *      Bug fix: does'nt make echo-articles with empty AREA:
 *      kiae unbatcher support added
 *      
 *         Rev 1.4   07 Apr 1992 12:24:58   dz
 *      pass_unknown_back mode support.
 *      
 *         Rev 1.3   03 Feb 1992 23:48:02   dz
 *      Free gating mode support written - addresses
 *      in form 'First.Last@p.f.n.z.fidonet.org' are
 *      supported.
 *      
 *         Rev 1.2   11 Oct 1991 05:33:20   dz
 *      Newsgroup name to echo area tag conversion written
 *      News letter detector written
 *      Cosmetical changes
 *      
 *         Rev 1.1   23 Sep 1991 17:20:42   dz
 *      Bang path address recognition improved, but not tested.
 *      Added '%' recognition.
 *
 *         Rev 1.0   28 Aug 1991 00:25:50   dz
 *      Initial revision.
 *
 *
\*/

#include	"uu2fido.h"

#include	<rexx.h>

#define		USE_CONTAINER	0
#define		TO_POSTMASTER	0




bool Uu2FidoApp::
map_addr( RFC_Msg &msg, fido_user &fto, String &filedir, string to_a )
    {
    char        uname[100];
    uucp_addr   ua( to_a );
    string      Rexx_Out;

    if( Ok == CallRexx( conf.rexx_u2f_addr.c_str(), Rexx_Out, to_a, NULL ) )
        to_a = Rexx_Out.c_str();

    filedir = "";

    switch( ua.convert_to_fido( fto, uname, 100 ) )
        {
    case Err:
        error( EI_None, "Unparsable address: %s", to_a );
        break;

    case Yes:
        if( !conf.free_mode )
                {
                error( EI_None, "Free mode letter to %s", to_a );
                break;
                }
        return Yes;

    case No:
        fido_user_def	ud;

        if( fuser.get( ud, uname ) != Ok )
                break;

        fto = ud.fido_address();
        filedir = ud.filedir();
        return Yes;
        }
    
    // Address is definitely a piece of junk :)
    warning( EI_None, "Can't deliver msg to '%s'", to_a );
    
    if( conf.pass_unknown_back )
        if( Err == run_uusmail( msg, to_a )) // Pass it back to uucp
            return Err;
    
    if( conf.send_error_notification )          // Send notifocation letter
        if( Err == reply_error( msg, to_a )) // back to sender
            return Err;
    
    if( !conf.pass_unknown_along )              // Send to gatemaster
        return No;

    fido_user_def	ud;

    if( fuser.get( ud, conf.postmaster.c_str() ) != Ok )
        {
        error( EI_None, "postmaster not found! Message to %s lost!", to_a );
        return Err;
        }

    fto = ud.fido_address();
    filedir = ud.filedir();

    return Yes;
    }





#if USE_CONTAINER

#include	<strng.h>
#include	<dlistimp.h>
#include	<deques.h>


typedef TDoubleListImp<string> AddrDoubleList;

struct trav_data
	{
	AddrDoubleList	*l;
    String      *u;
	};

static bool
uu_traverse( uucp_addr::el_type t, const char *el, int len, void *arg );


bool Uu2FidoApp::
remap_addr( fido_user &fto, String &filedir, const char *to_a )
	{
	char		uname[100];
	uucp_addr	ua( to_a );
	string		Rexx_Out;

	if( Ok == CallRexx( conf.rexx_u2f_addr.c_str(), Rexx_Out, to_a, NULL ) )
		to_a = Rexx_Out.c_str();


	filedir = "";

    debug( "Traversing address: %s", to_a );

    AddrDoubleList  dl;
    String      user;

    trav_data   td = { &dl, &user };

    if( ua.traverse( uu_traverse, (void *)&td ) != Yes )
        {
        error( EI_None, "Can't parse address: '%s'", to_a );
        return Err;
        }

    //
    // Empty? Put our domain in (or AddrDoubleList will crash)
    //

    if( dl.getItemsInContainer() == 0 )
        {
        String  *s = new String( conf.def_domain );
        if( s == NULL || s == Object::ZERO )
            fatal( EC_Out_Of_Memory, EI_None, "No RAM :(" );

	    dl.addAtTail( *s );
        }

cerr << "Domains: " << dl << "\n";

    //
    // A lot of stuff inside with our domain at beginning?
    // Remove it, 'cause it means nothing anyway...
    //

    while( dl.getItemsInContainer() > 1 )
        {
        if( stricmp( ((String&)dl.peekAtHead()), conf.def_domain ) == 0 )
            dl.destroyFromHead( dl.peekAtHead() );
        else
            break;
        }

cerr << "\n(strip our " << conf.def_domain << ", if leading)\nDomains: " << dl << "\n";

    ftn_def       fdef;

    if( Err != ftn::match( fdef, (uucp_addr)(String &)dl.peekAtHead() ) )
        {
cerr << "FTN matched: " << fdef.fullname << "\n";
cerr << "Killing " << (String&)dl.peekAtHead() << "\n";
        dl.destroyFromHead( dl.peekAtHead() );

        bool   hz = No, hn = No, hf = No, hp = No;




        }

//#endif

	switch( ua.convert_to_fido( fto, uname, 100 ) )
		{
	case Err:
		error( EI_None, "Unparsable address: %s", to_a );
		break;

	case Yes:
		if( !conf.free_mode )
			{
			error( EI_None, "Free mode letter to %s", to_a );
			break;
			}
		return Ok;

	case No:
		fido_user_def	ud;

		if( fuser.get( ud, uname ) != Ok )
			break;

		fto = ud.fido_address();
		filedir = ud.filedir();
		return Ok;
		}

    
        if( conf.pass_unknown_back )		// Don't try to send to postmaster
		return Err;

	error( EI_None, "Can't deliver msg to '%s'", to_a );

	fido_user_def	ud;

	if( fuser.get( ud, conf.postmaster.c_str() ) != Ok )
		{
		error( EI_None, "postmaster not found! Message to %s lost!", to_a );
		return Err;
		}

	fto = ud.fido_address();
	filedir = ud.filedir();

	return Ok;
	}




static bool
uu_traverse( uucp_addr::el_type t, const char *el, int len, void *arg )
	{
	trav_data	*e = (trav_data *)arg;

    const      bl = 200;
    char       buf[bl];

    if( len > bl )
        {
        error( EI_None, "Address fragment too long: '%*.*s'", len, len, el );
        len = bl-1;
        }

    strncpy( buf, el, len );
    buf[len] = '\0';

    debug( "Address part: %s", buf );

	if( t == uucp_addr::UU_name )		// No, no... domains only
        {
        *(e->u) = buf;
		return Yes;
        }

    //
    // If beginning of domain looks like part of fidonet-type address,
    // cut it off, recursively parse rest of domain, than add piece we
    // got to the list.
    //
    // This lets us break things like p22.f32.n5020.z2.fidonet.org
    // into the list: (fidonet.org z2 n5020 f32 p22)
    //

    char     fa_type;
    int      fa_val, fa_len = 0;

    if( 2 == sscanf( buf, "%c%d.%n", &fa_type, &fa_val, &fa_len ) )
        {
        if( fa_len != 0 && strchr( "pfnzPFNZ", fa_type ) )
            {
            uu_traverse( t, buf + fa_len, len - fa_len, arg );

            sprintf( buf, "%c%d", fa_type, fa_val );
            debug("FTN piece: %s", buf );

            }
        }

    String *s = new String( buf );

    if( s == NULL /*|| s == Object::ZERO*/ )
        fatal( EC_Out_Of_Memory, EI_None, "No RAM :(" );

	e->l->AddAtTail( *s );

	return Yes;
	}

#endif



