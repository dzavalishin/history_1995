/************************ Gate remote control ***************************\
 *
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :       Header
 *
 *      $Log: GRemote.h $
 *
 *
\*/


#ifndef BATCHMAIL_H
#define	BATCHMAIL_H



#include	<app.h>
#include	<vector.h>
//#include	<uucpaddr.h>

#include	<stdio.h>
#include	<style.h>
#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>
#include	<setup.h>
#include	<log.h>


class BatchMailApp : public U1App
    {
    bool            flush_mail;

    string          host;
    vector <string> addr;
    
public:
    int	        Run();
    BatchMailApp()
        {
        flush_mail = No;
        }
    
protected:

    void        do_add_mail();
    void        do_flush_mail();

      // add mail workers
    const char *load_stdin();
    

    };


#endif	// BATCHMAIL_H

