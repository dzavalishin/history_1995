Ä My netmail (2:5020/23.77@fidonet.org) ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ MY_NETMAIL Ä
 Msg  : #401 [79]                           Rcv Pvt K/s                         
 From : dz@rdrel.relcom.msk.su              2:5020/23       Fri 13 Nov 92 01:23 
 To   : Dmitry Zavalishin                   2:5020/23.77                        
 Subj : parse generic command line function                                     
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Received:  by gate.dialnet.msk.su (UU2FIDO 1.11f)
           for dz@pd.dialnet.msk.su;
           Wed, 25 Nov 1992 23:30:26 +0300
Received: by sovcom.kiae.su; Wed, 25 Nov 92 19:48:42 +0300
Received: by cpuv1.net.kiae.su (5.51/RelCom/SU)
Received: by rdrel.relcom.msk.su id AA12416
Resent-To: dz@pd.dialnet.msk.su
To: dz@isoft.msk.su
Cc: dz@rdrel.relcom.msk.su
Message-Id: <LNNEm0hOo3@rd>
From: Dmitry Zavalishin <dz@rdrel.relcom.msk.su>
Date: Fri, 13 Nov 1992 03:23:35 +0200
X-Mailer: Mail [v1.3 INTERACTIVE UNIX]
Subject: parse generic command line function


---------------------------------------------
#include <stdio.h>
#include <string.h>

typedef enum
       {
       Ok      = 0,
       Err     = -1,
       Cancel  = 1,
       Cant    = 2
       } result;

typedef enum
       {
       Yes     = 1,
       No      = 0
       } bool;

struct arg_def
       {
       char    key;
       char    *value;
       };


result
get_one_arg( arg_def &res, arg_def *atab, char arg_key )
       {
       for( arg_def *dp = atab; dp->key && dp->value; dp++ )
         if( dp->key == arg_key )
          {
          res = *dp;
          return Ok;
          }

       return Err;
       }


result
printargs(
       int buflen, char *buf, char *fmt,
       arg_def *tab )
       {
       bool    error = No;

       // Reserving one byte for terminating zero
       buflen--;

       // Reserving place for 2 characters: two '%' signs

       while( buflen > 1 && *fmt )
         {
         if( fmt[0] == '%' && fmt[1] == '%' )
          {
          *buf++ = *fmt++;
          buflen--;
          *buf++ = *fmt++;
          buflen--;
          continue;
          }

         if( *fmt != '%' )
          {
          *buf++ = *fmt++;
          buflen--;
          continue;
          }

         arg_def ad;

         if( get_one_arg( ad, tab, fmt[1] ) != Ok )
          {
          *buf++ = *fmt++;
          buflen--;
          continue;
          }

         int len = strlen( ad.value );
         if( len > buflen )
          {
          error = Yes;
          break;
          }

         strcat( buf, ad.value );
         buf += len;
         buflen -= len;
         fmt += 2;               // Eat '%' sign & keyletter
         }

       *buf++ = '\0';
       if( *fmt || error )
         return Err;

       return Ok;
       }


arg_def args[] =
       {
       { 'a',  "arg_a" },
       { 'b',  "arg_b" },
       { 'c',  "arg_c" },
       { '1',  "arg_1" },
       { '2',  "arg_2" },
       { '$',  "arg_$" },
       { 0, NULL }
       };



int
main( int, char ** )
       {
       char    buf[300];

       printf( "Printargs = %d\n",
         printargs( 300, buf,
          "a = %a, b = %b, c = %c, 1 = %1, 2 = %2, $ = %$.",
          args ) );
       printf( "Result = '%s'\n", buf );

       return 0;
       }












---------------------------------------------
--
Inet : dz@pd.dialnet.msk.su at office, dz@rd.relcom.msk.su at Relcom
Fido : Dmitry Zavalishin, 2:5020/23.77
Voice: +7 (095) 110-6728 Home, +7 (095) 267-4023 Office
Fax  : +7 (095) 267-4492


