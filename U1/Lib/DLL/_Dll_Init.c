#include <stdlib.h>
#include <stdio.h>

//#include "dllinits.h"

#ifdef __cplusplus
#error Use plain C compiler
#endif

 /*+-------------------------------------------------------------------+*/
 /*| _CRT_init is the C run-time environment initialization function.  |*/
 /*|It will return 0 to indicate success and -1 to indicate failure.   |*/
 /*| _CRT_term is the C run-time environment termination function.     |*/
 /*+-------------------------------------------------------------------+*/

int _CRT_init (void);
void _CRT_term (unsigned long);

//#pragma linkage (_DLL_InitTerm, system)

unsigned long _System _DLL_InitTerm( unsigned long modhandle, unsigned long flag )
	{
    /* If flag is zero then the DLL is being loaded so initialization  */
    /* should be performed.  If flag is 1 then the DLL is being freed  */
    /* so termination should be performed.                             */

	switch (flag)
		{
	case 0:
          /* The C run-time environment initialization function must   */
          /* be called before any calls to C run-time functions that   */
          /* are not inlined.                                          */
 
		if (_CRT_init () == -1)
			return 0UL;
 
//		printf("Runtime started\n");

		// dll-specific init goes here
		mark_program_start_time();
//		printf("Mark start passed\n");

		Register_Rexx_Subcommand_Handler();
		Register_Rexx_Function_Handlers();
 
		break;
 
	case 1:

		// dll-specific termination goes here

		close_stat();
		close_list();
		close_elf();
		close_log();

		// end of dll-specific termination

		_CRT_term(0UL);
		break;
 
	default:
		printf("flag = %lu\n",flag);
	          return 0UL;
		}
 
    /* A nonzero value must be returned to indicate success. */
	return 1UL;
 	}
