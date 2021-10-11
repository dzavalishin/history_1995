/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	FRIP regression tests
 *
 *      $Log: regress.c $
 *      Revision 1.3  1997/01/01 16:08:42  dz
 *      Before changing
 *
 *      Revision 1.2  1996/12/28 13:42:00  dz
 *      Ver. 27
 *
 *      Revision 1.1  1996/12/28 03:11:55  dz
 *      Initial revision
 *
 *
 *      
 *      
\*/

#include "frip.h"
#include "strng.h"
#include "datetime.h"

static void do_R( int line, const char *text );
#define R(a) do_R( __LINE__, a )




void datetimes()
    {
    time_t now = time(0);
    time_t zero = 0;
    
    datetime dd1(  now, datetime::UTC );
    datetime dd2( zero, datetime::UTC );
    string s1;
    string s2;
    tm tm1;

    s1 = dd1.gmtime_string();
    dd2 = datetime(s1);

    if( dd2 != dd1 )      R("to gmt string/from string");
    //printf(" dd1 = %d, dd2 = %d, s = %s\n", (time_t)dd1, (time_t)dd2, s1.c_str() );

    s1 = dd1.localtime_string();
    dd2 = datetime(s1);

    if( dd2 != dd1 )      R("to local string/from string");
    //printf(" dd1 = %d, dd2 = %d, s = %s\n", (time_t)dd1, (time_t)dd2, s1.c_str() );

    tm1 = dd1.gmtime_tm();
    dd2 = datetime(tm1,datetime::UTC);

    if( dd2 != dd1 )      R("to gmt tm/from tm");

    tm1 = dd1.localtime_tm();
    dd2 = datetime(tm1,_timezone);

    if( dd2 != dd1 )      R("to local tm/from tm");
    


    }
    










void strings()
    {
    string s1 = "AbCdEfG";
    string s2 = "abCdEfG";

    if( s1 == s2 )     R("string ==");
    if( !(s1 != s2) )  R("string !=");
    if(!(s1 <= s2) )   R("string <=");
    if( s1 >= s2 )     R("string >=");

    istring is1 = "AbCdEfG";
    istring is2 = "abCdEfG";
    istring is3 = "1bCdEfG";

    if( is1 != is2 )   R("istring ==");
    if( !(is1 == is2)) R("istring !=");
    if( is1 < is2 )    R("istring <=");
    if( is1 > is2 )    R("istring >=");
    
    if( is1 == is3 )   R("istring ==");
    if( !(is1 != is3)) R("istring !=");
    if(!(is3 <= is1))  R("istring <=");
    if( is3 >= is1 )   R("istring >=");

    if(!(is3 <= is3))  R("istring <=");
    if(!(is3 >= is3))  R("istring >=");

    string & isr1 = is1;
    string & isr2 = is2;
    string & isr3 = is3;
    
    if( isr1 != isr2 )   R("istring ==");
    if( !(isr1 == isr2)) R("istring !=");
    if( isr1 < isr2 )    R("istring <=");
    if( isr1 > isr2 )    R("istring >=");
    
    if( isr1 == isr3 )   R("istring ==");
    if( !(isr1 != isr3)) R("istring !=");
    if(!(isr3 <= isr1))  R("istring <=");
    if( isr3 >= isr1 )   R("istring >=");

    string & isr_m1 = s1;
    string & isr_m2 = is2;

    if( isr_m1 != isr_m2 )  R("istring/string !=");
    if( isr_m2 != isr_m1 )  R("istring/string !=");
    if( !(isr_m1 == isr_m2) )  R("istring/string ==");
    if( !(isr_m2 == isr_m1) )  R("istring/string ==");
    
    
    domain_s ds1 = "fidonet.org";
    domain_s ds2 = "FidoNet";
    domain_s ds3 = "FidoDa.Yes";
    string   ds4 = "fidoda";

    if( ds1 != ds2 )            R("domain_s !=");
    if( ds2 != ds1 )            R("domain_s !=");
    if( !(ds2 == ds1) )         R("domain_s ==");

    if( ds1 == ds3 )            R("domain_s ==");
    if( ds3 == ds1 )            R("domain_s ==");
    if( !(ds3 != ds1) )         R("domain_s !=");

    if( ds3 != ds4 )            R("domain_s !=");
    if( ds4 != ds3 )            R("domain_s !=");
    if( !(ds3 == ds4) )         R("domain_s ==");
    if( !(ds4 == ds3) )         R("domain_s ==");

    string &dsr3 = ds3;
    
    if( dsr3 != ds4 )           R("domain_s !=");
    if( ds4 != dsr3 )           R("domain_s !=");
    if( !(dsr3 == ds4) )        R("domain_s ==");
    if( !(ds4 == dsr3) )        R("domain_s ==");
    
    }










static err = 0;
int main()
    {
    tzset();

    datetimes();
    
    strings();

    if( err )
        printf("There were %d regressions!\7\n", err );
    
    return err;
    }

static void do_R( int line, const char *text )
    {
    printf("Here (%d) is a regression: %s\n", line, text );
    err++;
    }

bool volatile Stop_Frip = No;
string started_in_dir = "?:\\";


static int frip_exit_code = 0;
void add_exit_code( exit_code e )
    {
    int oec = frip_exit_code;
    frip_exit_code |= e;
    printf("add_exit_code( 0x%X ), result was 0x%X, now 0x%X\n", e, oec, frip_exit_code );
    }




