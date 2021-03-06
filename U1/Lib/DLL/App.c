/************************ U1 App class ***************************\
 *
 *	Copyright (C) 1995 by Infinity Soft
 *
 *	Module 	:	App class impl.
 *
 *      $Log: App.c $
 *      Revision 1.3  1995/11/05 13:52:48  dz
 *      current.
 *
 *      Revision 1.2  1995/08/07 03:21:44  dz
 *      *** empty log message ***
 *
 *      Revision 1.1  1995/08/07 03:17:31  dz
 *      Initial revision
 *
 *
 *
\*/

#include "App.h"

#include <string.h>
#include <stdio.h>

#include <log.h>

const char *U1App::_program_name = "Undefined U1 Module " U1_VER_STR " (" __DATE__ ")";
const char *U1App::_usage = "Program args: None\n";
bool        U1App::_announceable = Yes;

const static char SW = '-';


void U1App::
program_name(const char *s) {_program_name = s;}

void U1App::
usage(const char *s) {_usage = s;}

void U1App::
announceable(bool v) {_announceable = v;}



const char * U1App::
program_name() {return _program_name;}

const char * U1App::
usage() {return _usage;}

bool U1App::
announceable() {return _announceable;}






void
U1App::PrintName( void ) 
	{
	static bool done = No;

	if( !done )
		{
		printf( "%s\n" U1_COPYRIGHT "\n", _program_name );
		done = Yes;
		}
	}


void
U1App::PrintUsage( const char *msg ) 
	{
	PrintName();

	char *p = av[0];
	char *p1;

	p1 = strrchr( p, '/' );
	if( p1 ) p = p1+1;

	p1 = strrchr( p, '\\' );
	if( p1 ) p = p1+1;

	printf(

		"\n"
        	"%s%s\n"
	        "Usage: %s [U1 args] [Program args]\n"
		"\n"
		"U1 Universal args:\n"
		"    -?          - Print usage\n"
		"    -X          - Enable debug log\n"
		"    -x <number> - Ignored\n"
		"\n"
		"%s"
		"\n"

		,

		msg, strlen( msg ) ? "\n" : "",
		p,
		_usage

		);

	}




U1App::opt_t	
U1App::Real_GetOpt()
	{
	unsigned char ch;
	char *optP;

	if( !agr_parser_ready ) return o_Err;
 
	if( ac <= optind )
		{
		letP = NULL;  
		return o_Eof;
		}

	if( no_more_opts )
		{
		OptArg = av[optind++];
		return o_Arg;
		}

	if( letP == NULL )
		{
		letP = av[optind];

		if( letP == NULL )  
			return o_Eof;

		if( *(letP++) != SW)  
			{
			letP = NULL;
			OptArg = av[optind++];
			return o_Arg;
			}

		if (*letP == SW) 
			{
			optind++;
			letP = NULL;
			no_more_opts = Yes;
			return GetOpt();
			}
		}

	ch = *(letP++);

	if (0 == ch) 
		{
		optind++;
		letP = NULL;
		if( optind >= ac )
			return o_Eof;
		OptArg = av[optind];
		return o_Arg;
		}

	if (':' == ch || (optP = strchr(optionS, ch)) == NULL)
		{
		fprintf( stderr, "Invalid option '%c' on command line\n", ch );
		return o_Err;
		}

	if (':' == *(++optP)) 
		{
		optind++;
		if (0 == *letP) 
			{
			if (ac <= optind)
				{
				fprintf( stderr, "No argument for option '%c' on command line\n", ch );
				return o_Err;
				}
			letP = av[optind++];
			}

		OptArg = letP;
		letP = NULL;
		} 
	else 
		{
		if (0 == *letP) 
			{
			optind++;
			letP = NULL;
			}

		OptArg = "";
		}

	OptChar = ch;
	return o_Opt;

	}



/*

gopEOF:
	optarg = letP = NULL;  
	return EOF;
 
gopError:
	optarg = NULL;
	errno  = EINVAL;
	if (opterr)
		perror ("get command line option");
	return ('?');

*/









/*
  Parse the command line options, System V style.
 
  Standard option syntax is:
 
    option ::= SW [optLetter]* [argLetter space* argument]
 
  where
    - SW is either '/' or '-', according to the current setting
      of the MSDOS switchar (int 21h function 37h).
    - there is no space before any optLetter or argLetter.
    - opt/arg letters are alphabetic, not punctuation characters.
    - optLetters, if present, must be matched in optionS.
    - argLetters, if present, are found in optionS followed by ':'.
    - argument is any white-space delimited string.  Note that it
      can include the SW character.
    - upper and lower case letters are distinct.
 
  There may be multiple option clusters on a command line, each
  beginning with a SW, but all must appear before any non-option
  arguments (arguments not introduced by SW).  Opt/arg letters may
  be repeated: it is up to the caller to decide if that is an error.
 
  The character SW appearing alone as the last argument is an error.
  The lead-in sequence SWSW ("--" or "//") causes itself and all the
  rest of the line to be ignored (allowing non-options which begin
  with the switch char).

  The string *optionS allows valid opt/arg letters to be recognized.
  argLetters are followed with ':'.  Getopt () returns the value of
  the option character found, or EOF if no more options are in the
  command line.	 If option is an argLetter then the global optarg is
  set to point to the argument string (having skipped any white-space).
 
  The global optind is initially 1 and is always left as the index
  of the next argument of argv[] which getopt has not taken.  Note
  that if "--" or "//" are used then optind is stepped to the next
  argument before getopt() returns EOF.
 
  If an error occurs, that is an SW char precedes an unknown letter,
  then getopt() will return a '?' character and normally prints an
  error message via perror().  If the global variable opterr is set
  to false (zero) before calling getopt() then the error message is
  not printed.
 
  For example, if the MSDOS switch char is '/' (the MSDOS norm) and
 
    *optionS == "A:F:PuU:wXZ:"
 
  then 'P', 'u', 'w', and 'X' are option letters and 'F', 'U', 'Z'
  are followed by arguments.  A valid command line may be:
 
    aCommand  /uPFPi /X /A L someFile
 
  where:
    - 'u' and 'P' will be returned as isolated option letters.
    - 'F' will return with "Pi" as its argument string.
    - 'X' is an isolated option.
    - 'A' will return with "L" as its argument.
    - "someFile" is not an option, and terminates getOpt.  The
      caller may collect remaining arguments using argv pointers.
*/




      U1App::opt_t	
      U1App::GetOpt()
      {
      
      while( 1 )
          {
          opt_t res = Real_GetOpt();
          
          if( res == o_Err || res == o_Eof || res == o_Arg )	
              return res;
          
          if( strchr( UNIV_OPTS, OptChar ) == NULL )	return res;
          
          Universal_Opt( res );
          }
      
      }
      
      void
      U1App::Universal_Opt(opt_t type)
      {
      
      switch( OptChar )
          {
          case '?':
              PrintUsage("");
              exit(0);
              
          case 'X':
              conf.log_debug = Yes;
              break;
              
          case 'x':
              // conf.log_debug = Yes;
              break;
              
          default:
              fprintf( stderr, "Internal error: option '%c' is not known to Universal_Opt()\n", OptChar );
          }
      
      }
      
      
      U1App::~U1App() 
      {
      }
      

