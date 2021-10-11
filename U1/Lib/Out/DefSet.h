/************************ UUCP to FIDO convertor ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Setup defaults
 *
 *      $Log: DefSet.h $
 * Revision 1.2  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *      
 *         Rev 1.1   07 Jun 1993 17:56:50   dz
 *      update
 *      
 *         Rev 1.0   29 Jun 1992 21:44:10   dz
 *      Initial revision.
 *
 *
 *
\*/


#ifndef	DEFSET_H
#define	DEFSET_H


#define     DEF_ZIP_LIMIT               (8 * 1024)
#define     DEF_PKT_SIZE                (50 * 1024ul)
#define     DEF_PART_SIZE               (12 * 1024)
#define     DEF_FIDO_FROM_FIELD_SIZE    35
#define     MAX_FIDO_FROM_FIELD_SIZE    36
#define     DEF_RMAIL_EXE               "uusmail.exe"
#define     DEF_REXITL                  0
#define     DEF_REXITR                  48
#define     DEF_CODE_UF                 "KOI8-CP866"
#define     DEF_CODE_FU                 "none"
#define     DEF_LOG_LEVEL               "abcefnu"
#define     DEF_FREE_DOMAIN             "fidonet.org"
#define     DEF_ATTACH_STEM             "Gate"

//	Directories

#define     DEF_FLAG_DIR                "flags"
#define     DEF_STAT_DIR                "stat"
#define     DEF_NEWSIN_DIR              "news.in"
#define     DEF_NEWSOUT_DIR             "news.out"
#define     DEF_STORE_DIR               "gate.out"
#define     DEF_FILEIN_DIR              "file.in"
#define     DEF_FILEOUT_DIR             "file.out"
#define     DEF_RECODE_DIR              "."

//	Files

#define     DEF_USERLIST_FILE           "users.u1"
#define     DEF_ECHOLIST_FILE           "echoes.u1"
#define     DEF_FEEDLIST_FILE           "feeds.u1"
#define     DEF_RULELIST_FILE           "rules.u1"
#define     DEF_NEWUSR_FILE             "newusr.-$-"

// Headlines

#define     DEF_MAIL_PASS_HL            "*"
#define     DEF_MAIL_HIDE_HL            "X-* Message-ID "
#define     DEF_MAIL_KILL_HL            "Fido* Status Resent-* Nntp-*"

#define     DEF_NEWS_PASS_HL            "*"
#define     DEF_NEWS_HIDE_HL            "Path Message-ID "
#define     DEF_NEWS_KILL_HL            "Fido* Status Resent-* Nntp-*"

#endif

