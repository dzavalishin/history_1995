head	1.1;
access;
symbols;
locks;
comment	@ * @;


1.1
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	;


desc
@@


1.1
log
@Initial revision
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Main
 *
 *      $Log: frip.h $
 *
 *
 *
\*/


#ifndef FILER_H
#define FILER_H


class filer
    {
#ifdef __WATCOMC__
    DIR    *dirp;
    dirent *direntp;
#endif
#ifdef __IBMCPP__
    HDIR    h;
    bool    restart_flag;
    bool    valid_v;
    bool    opened_flag;
    string  result;
    
    bool    do_restart( string &name );
    bool    do_continue( string &name );
    
#endif
    string  mask;
    
    bool    fnmatch( const char *fn, const char *mask );
    public:
        
#ifdef __WATCOMC__
        filer() { dirp = direntp = 0; }
#endif
#ifdef __IBMCPP__
        filer();
#endif
        filer( string dir, string mask);
        ~filer();
        
        bool    valid();
        void    next();
        
        string  current();
        
    };

#endif // FILER_H


@
