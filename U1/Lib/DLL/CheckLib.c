/************************ UU2 Library test ***************************\
 *
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :       Main
 *
 *      $Log: CheckLib.c $
 *      Revision 1.4  1995/11/06 02:33:43  dz
 *      *** empty log message ***
 *
 *      Revision 1.3  1995/11/05 13:52:48  dz
 *      current.
 *
 * Revision 1.2  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 *      
 *         Rev 1.0   06 Nov 1992 19:20:02   dz
 *      Ver 1.12a save
 *
 *
\*/

#include        <style.h>
#include        <stdlib.h>
#include        <stdio.h>
#include        <string.h>

#include		"setup.h"
#include		"log.h"


#include        <ftn_msg.h>


void
main( void ) {

	FILE *fp;

//	printf("Tester started\n");

//	set_program_name ( "CheckLib  (" __DATE__ ")" );


	if((fp = fopen("n:\\net\\uu2\\setup.u1","r")) == NULL) {
		perror("Unable to open setup\n\n");
		return;
		}
	printf("Setup opened in Main\n");


	get_setup();

//	check_ignname("AssHole");
/*
	fido_addr	a;
	char		s[200];

	gets( s );

	a.fparse( s );

	printf( "Result: %s\n", (const char *) a );
*/
	printf("Exiting checklib\n");
	exit(0);
	}

