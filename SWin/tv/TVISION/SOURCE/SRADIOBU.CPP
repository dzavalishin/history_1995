/*------------------------------------------------------------*/
/* filename -       sradiobu.cpp                              */
/*                                                            */
/* Registeration object for the class TRadioButtons           */
/*------------------------------------------------------------*/

/*------------------------------------------------------------*/
/*                                                            */
/*    Turbo Vision -  Version 1.0                             */
/*                                                            */
/*                                                            */
/*    Copyright (c) 1991 by Borland International             */
/*    All Rights Reserved.                                    */
/*                                                            */
/*------------------------------------------------------------*/

#ifdef __WATCOMC__
#pragma off(unreferenced)
#endif

#define Uses_TRadioButtons
#define Uses_TStreamableClass
#include <tv.h>
__link( RCluster )

TStreamableClass RRadioButtons( TRadioButtons::name,
                                TRadioButtons::build,
                                __DELTA(TRadioButtons)
                              );

