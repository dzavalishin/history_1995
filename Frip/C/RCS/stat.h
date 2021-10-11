head	1.2;
access;
symbols;
locks;
comment	@ * @;


1.2
date	97.03.28.22.03.34;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@no summary printed from d'tor - can't, log file is closed at that time.
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module: Stat
 *
 *      $Log: stat.h $
 *      Revision 1.1  1996/08/04 17:28:17  dz
 *      Initial revision
 *
 *
 *
\*/

#ifndef STAT_H
#define STAT_H


class statistics
    {
    private:
        int      no_good_v;
        int      new_addr_v;
        int      better_repl_v;
        int      timeout_repl_v;
        int      filtered_v;
        int      rips_total_v;
        
        int      rifs_total_v;
        int      rifs_for_us_v;
        int      rifs_forw_v;
        int      rifs_held_v;
        
    public:
        statistics()
            {
            no_good_v = 0;
            new_addr_v = 0;
            better_repl_v = 0;
            timeout_repl_v = 0;
            filtered_v = 0;
            rips_total_v = 0;
            
            rifs_total_v = 0;
            rifs_for_us_v = 0; 
            rifs_forw_v = 0;   
            rifs_held_v = 0;
            }

        ~statistics() { /*report();*/ }

        
        void     rip_no_good()      { no_good_v++; rips_total_v++; }
        void     rip_new_addr()     { new_addr_v++; rips_total_v++; }
        void     rip_better_repl()  { better_repl_v++; rips_total_v++; }
        void     rip_timeout_repl() { timeout_repl_v++; rips_total_v++; }
        void     rip_filtered()     { filtered_v++; rips_total_v++; }
        
        void     rif_for_us()       { rifs_for_us_v++; rifs_total_v++; }
        void     rif_forw()         { rifs_forw_v++; rifs_total_v++; }
        void     rif_held()         { rifs_held_v++; rifs_total_v++; }
        
        void     report();
    };

extern statistics stats;


#endif // STAT_H

@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: frip.h $
d50 1
a50 1
        ~statistics() { report(); }
@
