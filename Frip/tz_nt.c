================================= dz's netmail =================================
   From: Dmitry Ponomarev                    2:5020/144.4    04 Jun 97  22:02:16
     To: Dmitry Zavalishin                   2:5020/32       05 Jun 97  04:41:40
   Subj: FRIP_NT                                                                
================================================================================
�, Hi-����, Dmitry!

#include <windows.h>

int main(int argc, char *argv[])
{
    TIME_ZONE_INFORMATION TZInfo;
    GetTimeZoneInformation(&TZInfo);
/* UTC = local time + bias
    LONG       Bias; // ����p���� � ������ ��� ��᪢�= -180
    WCHAR      StandardName[ 32 ]; // H������� ��� ���� �� �㦭�
    SYSTEMTIME StandardDate; // �p��� ��砫� ������� �p�����
    LONG       StandardBias; // ����p���� � ������ ��� ��᪢�= 0
    WCHAR      DaylightName[ 32 ]; // H������� ��� ���� �� �㦭�
    SYSTEMTIME DaylightDate; // �p��� ��砫� ��⭥�� �p�����
    LONG       DaylightBias; // ����p���� � ������ ��� ��᪢�= -60

typedef struct _SYSTEMTIME {  // Win32 structure
// ����祭� �⫠�稪�� StandardDate, DaylightDate
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

�� ���� ������ � TZ ��p���� � �� ����, ����� ��� ��᮪ ���� ⥡� �������.
������� ��p��� ᪠���, �� �� �� ����� ᨤ� � OS/2 � Win32 奫�� �� ������.

 �, By-����, Dmitry!
Dmitry.

---

================================================================================
