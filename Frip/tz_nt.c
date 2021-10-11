================================= dz's netmail =================================
   From: Dmitry Ponomarev                    2:5020/144.4    04 Jun 97  22:02:16
     To: Dmitry Zavalishin                   2:5020/32       05 Jun 97  04:41:40
   Subj: FRIP_NT                                                                
================================================================================
у, Hi-ствуйте, Dmitry!

#include <windows.h>

int main(int argc, char *argv[])
{
    TIME_ZONE_INFORMATION TZInfo;
    GetTimeZoneInformation(&TZInfo);
/* UTC = local time + bias
    LONG       Bias; // Измеpяется в минутах для Москвы= -180
    WCHAR      StandardName[ 32 ]; // Hазвание для дела не нужно
    SYSTEMTIME StandardDate; // Вpемя начала зимнего вpемени
    LONG       StandardBias; // Измеpяется в минутах для Москвы= 0
    WCHAR      DaylightName[ 32 ]; // Hазвание для дела не нужно
    SYSTEMTIME DaylightDate; // Вpемя начала летнего вpемени
    LONG       DaylightBias; // Измеpяется в минутах для Москвы= -60

typedef struct _SYSTEMTIME {  // Win32 structure
// Получены отладчиком StandardDate, DaylightDate
    WORD wYear;         //  0       0
    WORD wMonth;        //  9       3
    WORD wDayOfWeek;    //  0       0
    WORD wDay;          //  5       5
    WORD wHour;         //  2       2
    WORD wMinute;       //  0       0
    WORD wSecond;       //  0       0
    WORD wMilliseconds; //  0       0
} SYSTEMTIME;
*/

  return(0);
}

Что надо делать с TZ стpокой я не знаю, может этот кусок кода тебе поможет.
Лебедев Киpилл сказал, что ты все пишешь сидя в OS/2 и Win32 хелпа не имеешь.

 у, By-ствуйте, Dmitry!
Dmitry.

---

================================================================================
