/************************ U1 App class ***************************\
 *
 *	Copyright (C) 1995 by Infinity Soft
 *
 *	Module 	:	App class definition
 *
 *      $Log: App.h $
 *      Revision 1.3  1995/11/05 13:52:48  dz
 *      current.
 *
 *      Revision 1.2  1995/08/07 03:23:28  dz
 *      *** empty log message ***
 *
 *      Revision 1.1  1995/08/07 03:17:31  dz
 *      Initial revision
 *
 *
 *
\*/

#ifndef	APP_H
#define	APP_H

#include "style.h"
#include "strng.h"
#include <stdio.h>
#include <setup.h>
#include <../../version.h>

#define UNIV_OPTS	"?Xx:"


class _Export U1App
	{
	static const char * _Export      _program_name;
public:
	static const char * _Export      _usage;
	static       bool   _Export      _announceable;

	static void       program_name(const char *s);
	static void       usage(const char *s);
	static void       announceable(bool v);

	static const char *       program_name();
	static const char *       usage();
	static bool               announceable();

		U1App( void )
			{
			agr_parser_ready = No;
			optionS = NULL;
			get_setup();
			};

		~U1App();

	enum opt_t { o_Arg, o_Opt, o_Err, o_Eof };

private:

	int	      ac;
	char       ** av;
	char *        optionS;
	int	      optind;		/* index of which argument is next	*/
	char       *  letP;		/* remember next option char's location */
	bool          no_more_opts;
	bool          agr_parser_ready;

	opt_t	Real_GetOpt();
	void	Universal_Opt(opt_t type);


protected:
	opt_t	GetOpt();
	int	OptChar;
	String	OptArg;

	// If GetOPt returns o_Arg, there is no switch char on cmd line
	// and next argv element is assigned to OptArg as a whole.
	// else there is an option letter in OptChar and possible arg in
	// OptArg.


public:
	void	Setup( int argc, char **argv, const char *opts ) 
			{ 
			ac = argc; av = argv; 
			optind	= 1;
			letP = NULL;
			no_more_opts = No;
			agr_parser_ready = Yes;

			if( optionS != NULL ) free( optionS );

			optionS = (char *) malloc(strlen(UNIV_OPTS) + strlen(opts) + 1);
			if( NULL == optionS )
				{
				fprintf( stderr, "Out of memory in App::Setup!" );
				exit(33);
				}

			strcpy( optionS, UNIV_OPTS );
			strcat( optionS, opts );

			if( conf.announce_module && _announceable )
				PrintName();
			}


	virtual int	Run() = 0;

	void		PrintUsage( const char *msg );
	void		PrintName( void );

	};


#endif // APP_H

