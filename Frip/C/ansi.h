/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	ANSI Sequences
 *
 *      $Log: ansi.h $
 *      Revision 1.3  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.2  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.1  1996/01/07 20:21:01  dz
 *      Initial revision
 *
 *
 *
\*/

#define ansi_esc "\x1b["

#define ansi_lighgray       ansi_esc"37m"
#define ansi_cyan           ansi_esc"36m"
#define ansi_magenta        ansi_esc"35m"
#define ansi_blue           ansi_esc"34m"
#define ansi_brown          ansi_esc"33m"
#define ansi_green          ansi_esc"32m"
#define ansi_red            ansi_esc"31m"
#define ansi_black          ansi_esc"30m"

#define ansi_invisible      ansi_esc"8m"
#define ansi_reverse        ansi_esc"7m"
#define ansi_darkgray_bg    ansi_esc"5m"
//#define ansi_blue           ansi_esc"4m"
#define ansi_intense        ansi_esc"1m"
#define ansi_normal         ansi_esc"0m"
