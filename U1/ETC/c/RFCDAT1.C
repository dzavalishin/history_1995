� REL.SOURCES (2:5020/23.77@fidonet.org) ������������������������� REL.SOURCES �
 Msg  : #39 [41]                                                                
 From : vak@relcom.kiae.su          2:5020/23               Wed 13 May 92 18:26 
 To   : All                                                 Thu 14 May 92 02:14 
 Subj : (1) [src] ����� ⮫�࠭�� �८�ࠧ���⥫� ���� (RFC 1036)             
��������������������������������������������������������������������������������
 * Real name: Serge V.Vakulenko


/*
 * �८�ࠧ������ �६��� � �६����� ���� �
 * ��ப� ���� � �⠭���� RFC 1036.
 *
 *     char *genrfcdate (long tim, int tzm)
 *
 * ��ࠬ��� tim ������ �६�, tzm - ᤢ�� �६����� ����
 * � ������, ������⥫�� ���祭�� � ������ �� �ਭ���.
 * �����頥� 㪠��⥫� �� ����७��� ����᪨� ����.
 *
 * �ਢ������ ��ப� ���� � �⠭����� RFC 1036.
 *
 *     char *rfcdate (char *ctim)
 *
 * ��ப� ctim ������ �।�⠢���� ᮡ�� ���� � �⠭����
 * RFC 822 ��� � �⠭���� Unix.
 * �����頥� 㪠��⥫� �� ����७��� ����᪨� ����.
 * �᫨ �室��� ��ப� �訡�筠, �����頥� 0.
 *
 * ����: ��ࣥ� ���㫥���, vak@kiae.su
 * Tue May 12 23:13:46 MSD 1992
 */

/*
 * �뤥প� �� RFC 1036:
 * ~~~~~~~~~~~~~~~~~~~~~
 * The "Date" line (formerly "Posted") is the date that the message was
 * originally posted to the network.  Its format must be acceptable
 * both in RFC-822 and to the getdate(3) routine that is provided with
 * the Usenet software.  This date remains unchanged as the message is
 * propagated throughout the network.  One format that is acceptable to
 * both is:
 *
 *              Wdy, DD Mon YY HH:MM:SS TIMEZONE
 *
 * Several examples of valid dates appear in the sample message above.
 * Note in particular that ctime(3) format:
 *
 *              Wdy Mon DD HH:MM:SS YYYY
 *
 * is not acceptable because it is not a valid RFC-822 date.  However,
 * since older software still generates this format, news
 * implementations are encouraged to accept this format and translate
 * it into an acceptable format.
 *
 * There is no hope of having a complete list of timezones.  Universal
 * Time (GMT), the North American timezones (PST, PDT, MST, MDT, CST,
 * CDT, EST, EDT) and the +/-hhmm offset specifed in RFC-822 should be
 * supported.  It is recommended that times in message headers be
 * transmitted in GMT and displayed in the local time zone.
 */

/*
 * �뤥প� �� RFC 822:
 * ~~~~~~~~~~~~~~~~~~~~~
 * date-time   =  [ day "," ] date time        ; dd mm yy
 *                                             ;  hh:mm:ss zzz
 *
 * day         =  "Mon"  / "Tue" /  "Wed"  / "Thu"
 *             /  "Fri"  / "Sat" /  "Sun"
 *
 * date        =  1*2DIGIT month 2DIGIT        ; day month year
 *                                             ;  e.g. 20 Jun 82
 *
 * month       =  "Jan"  /  "Feb" /  "Mar"  /  "Apr"
 *             /  "May"  /  "Jun" /  "Jul"  /  "Aug"
 *             /  "Sep"  /  "Oct" /  "Nov"  /  "Dec"
 *
 * time        =  hour zone                    ; ANSI and Military
 *
 * hour        =  2DIGIT ":" 2DIGIT [":" 2DIGIT]
 *                                             ; 00:00:00 - 23:59:59
 *
 * zone        =  "UT"  / "GMT"                ; Universal Time
 *                                             ; North American : UT
 *             /  "EST" / "EDT"                ;  Eastern:  - 5/ - 4
 *             /  "CST" / "CDT"                ;  Central:  - 6/ - 5
 *             /  "MST" / "MDT"                ;  Mountain: - 7/ - 6
 *             /  "PST" / "PDT"                ;  Pacific:  - 8/ - 7
 *             /  1ALPHA                       ; Military: Z = UT;
 *                                             ;  A:-1; (J not used)
 *                                             ;  M:-12; N:+1; Y:+12
 *             / ( ("+" / "-") 4DIGIT )        ; Local differential
 *                                             ;  hours+min. (HHMM)
 *
 *      If included, day-of-week must be the day implied by the date
 * specification.
 *
 *      Time zone may be indicated in several ways.  "UT" is Univer-
 * sal  Time  (formerly called "Greenwich Mean Time"); "GMT" is per-
 * mitted as a reference to Universal Time.  The  military  standard
 * uses  a  single  character for each zone.  "Z" is Universal Time.
 * "A" indicates one hour earlier, and "M" indicates 12  hours  ear-
 * lier;  "N"  is  one  hour  later, and "Y" is 12 hours later.  The
 * letter "J" is not used.  The other remaining two forms are  taken
 * from ANSI standard X3.51-1975.  One allows explicit indication of
 * the amount of offset from UT; the other uses  common  3-character
 * strings for indicating time zones in North America.
 */

# define ERROR  -32000          /* ��� �訡�� ��� getword */

# define HRS    *60             /* ������ ��� ���樠����樨 ⠡��� */
# define HALFHR 30

# define KEYSIZE(tab) (sizeof (tab) / sizeof (struct keytab))

struct keytab {                 /* ⠡���� ��� ���᪠ ���祢�� ᫮� */
  char *name;
  int value;
};

static struct keytab daytab [] = {      /* ��� ������ */
  "fri",          5,
  "friday",       5,
  "mon",          1,
  "monday",       1,
  "sat",          6,
  "saturday",     6,
  "sun",          0,
  "sunday",       0,
  "thu",          4,
  "thur",         4,
  "thurs",        4,
  "thursday",     4,
  "tue",          2,
  "tues",         2,
  "tuesday",      2,
  "wed",          3,
  "wednes",       3,
  "wednesday",    3,
};

static struct keytab monthtab [] = {    /* ������ */
  "apr",          4,
  "april",        4,
  "aug",          8,
  "august",       8,
  "dec",          12,
  "december",     12,
  "feb",          2,
  "february",     2,
  "jan",          1,
  "january",      1,
  "jul",          7,
  "july",         7,
  "jun",          6,
  "june",         6,
  "mar",          3,
  "march",        3,

---
 * Origin: Gated from RelCom by NetDialogue (2:5020/23.0)

� REL.SOURCES (2:5020/23.77@fidonet.org) ������������������������� REL.SOURCES �
 Msg  : #40 [41]                                                                
 From : vak@relcom.kiae.su          2:5020/23               Wed 13 May 92 18:26 
 To   : All                                                 Thu 14 May 92 02:14 
 Subj : (2) [src] ����� ⮫�࠭�� �८�ࠧ���⥫� ���� (RFC 1036)             
��������������������������������������������������������������������������������
 * Real name: Serge V.Vakulenko

  "may",          5,
  "nov",          11,
  "november",     11,
  "oct",          10,
  "october",      10,
  "sep",          9,
  "sept",         9,
  "september",    9,
};

static struct keytab zonetab [] = {     /* �६���� ���� */
  "a",            1 HRS,
  "a.c.s.s.t.",   -(10 HRS + HALFHR),
  "a.c.s.t.",     -(9 HRS + HALFHR),
  "a.d.t.",       3 HRS,
  "a.e.s.s.t.",   -11 HRS,
  "a.e.s.t.",     -10 HRS,
  "a.s.t.",       4 HRS,
  "a.w.s.t.",     -8 HRS,         /* (no daylight time there, I'm told */
  "acsst",        -(10 HRS + HALFHR), /* Australian Central Summer */
  "acst",         -(9 HRS + HALFHR), /* Australian Central Time */
  "adt",          3 HRS,
  "aesst",        -11 HRS,        /* Australian Eastern Summer Time */
  "aest",         -10 HRS,        /* Australian Eastern Time */
  "ast",          4 HRS,          /* Atlantic */
  "awst",         -8 HRS,         /* Australian Western Time */
  "b",            2 HRS,
  "b.s.t.",       -1 HRS,
  "bst",          -1 HRS,         /* British Summer Time */
  "c",            3 HRS,
  "c.d.t.",       5 HRS,
  "c.s.t.",       6 HRS,
  "cdt",          5 HRS,          /* Central Summer */
  "cst",          6 HRS,          /* Central */
  "d",            4 HRS,
  "e",            5 HRS,
  "e.d.t.",       4 HRS,
  "e.e.s.t.",     -1 HRS,
  "e.e.t.",       0 HRS,
  "e.s.t.",       5 HRS,
  "edt",          4 HRS,          /* Eastern Summer */
  "eest",         -1 HRS,         /* European Eastern Summer Time */
  "eet",          0 HRS,          /* European Eastern Time */
  "est",          5 HRS,          /* Eastern */
  "f",            6 HRS,
  "g",            7 HRS,
  "g.m.t.",       0 HRS,
  "gmt",          0 HRS,          /* Universal Time */
  "h",            8 HRS,
  "h.d.t.",       9 HRS,
  "h.s.t.",       10 HRS,
  "hdt",          9 HRS,
  "hst",          10 HRS,         /* Hawaii */
  "i",            9 HRS,
  "j.s.t.",       -9 HRS,         /* Japan Standard Time */
  "jst",          -9 HRS,         /* Japan Standard Time */
  "k",            10 HRS,
  "l",            11 HRS,
  "m",            12 HRS,
  "m.d.t.",       6 HRS,
  "m.e.s.t.",     -2 HRS,
  "m.e.t.",       -1 HRS,
  "m.s.t.",       7 HRS,
  "mdt",          6 HRS,          /* Mountain Summer */
  "mest",         -2 HRS,         /* Middle European Summer Time */
  "met",          -1 HRS,         /* Middle European Time */
  "msd",          -4 HRS,         /* Moscow Summer */
  "msk",          -3 HRS,         /* Moscow */
  "mst",          7 HRS,          /* Mountain */
  "n",            -1 HRS,
  "n.s.t.",       3 HRS + HALFHR,
  "nst",          3 HRS + HALFHR, /* Newfoundland */
  "o",            -2 HRS,
  "p",            -3 HRS,
  "p.d.t.",       7 HRS,
  "p.s.t.",       8 HRS,
  "pdt",          7 HRS,          /* Pacific Summer */
  "pst",          8 HRS,          /* Pacific */
  "q",            -4 HRS,
  "r",            -5 HRS,
  "s",            -6 HRS,
  "t",            -7 HRS,
  "u",            -8 HRS,
  "ut",           0 HRS,          /* Universal Time */
  "v",            -9 HRS,
  "w",            -10 HRS,
  "w.e.s.t.",     -3 HRS,
  "w.e.t.",       -2 HRS,
  "west",         -3 HRS,         /* Western European Summer Time */
  "wet",          -2 HRS,         /* Western European Time */
  "x",            -11 HRS,
  "y",            -12 HRS,
  "y.d.t.",       8 HRS,
  "y.s.t.",       9 HRS,
  "ydt",          8 HRS,
  "yst",          9 HRS,          /* Yukon */
  "z",            0 HRS,
};

/*
 * H������� ���� ������ ��� ����.
 */
static char *dayname [] = {
  "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat",
};

/*
 * H������� ����楢 ��� ����.
 */
static char *monthname [] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
};

static char buf [40];                   /* ����, � ���஬ ��⮢���� ��� */

extern char *ctime (), *strcpy (), *strncpy (), *gets ();

/*
 * ����ᥭ�� � ���� ����� �६����� ����.
 */
static settz (b, tz)
register char *b;
register tz;
{
  if (tz == 0) {
   strcpy (b+24, "GMT");
   return;
  }
  if (tz < 0) {
   b[24] = '+';
   tz = -tz;
  } else
   b[24] = '-';
  b[25] = tz / 60 / 10 + '0';
  b[26] = tz / 60 % 10 + '0';
  tz %= 60;
  b[27] = tz / 10 + '0';
  b[28] = tz % 10 + '0';
}

/*
 * ������ ���� � ⠡��� ���祢�� ᫮�.
 */
static struct keytab *binsearch (name, tab, tabsz)
char *name;
struct keytab *tab;
{
  int min, max, middle, cmp;

  min = 0;
  max = tabsz - 1;
  while (max >= min) {
   middle = (min + max) / 2;
   cmp = strcmp (name, tab[middle].name);
   if (! cmp)
      return (tab + middle);
   if (cmp < 0)
      max = middle - 1;
   else
      min = middle + 1;
  }
  return (0);
}

/*
 * �⥭�� 楫��� �᫠.
 */
static getint (p)
register char **p;
{
  register n;

  if (**p<'0' || **p>'9')
   return (-1);
  n = 0;
  while (**p>='0' && **p<='9')
   n = n*10 + *(*p)++ - '0';
  return (n);
}

/*
 * �⥭�� ���祢��� ᫮��, ����饣� �� �㪢 � �祪.
 * �᫨ ᫮�� �������, �����頥� ��� ���祭�� (value),
 * ���� 0.
 */
static getword (p, tab, tabsz)
register char **p;
struct keytab *tab;
{
  register n;
  char key [20];

  if (! (**p>='A' && **p<='Z' || **p>='a' && **p<='z'))
   return (ERROR);
  n = 0;
  while (n<sizeof(key) && (**p>='A' && **p<='Z' ||
      **p>='a' && **p<='z' || **p=='.'))
   key [n++] = *(*p)++;
  if (n >= sizeof (key))
   return (ERROR);
  key [n] = 0;
  for (n=0; key[n]; ++n)
   if (key[n]>='A' && key[n]<='Z')
      key [n] += 'a' - 'A';

---
 * Origin: Gated from RelCom by NetDialogue (2:5020/23.0)

� REL.SOURCES (2:5020/23.77@fidonet.org) ������������������������� REL.SOURCES �
 Msg  : #41 [41]                                                                
 From : vak@relcom.kiae.su          2:5020/23               Wed 13 May 92 18:26 
 To   : All                                                 Thu 14 May 92 02:14 
 Subj : (3) [src] ����� ⮫�࠭�� �८�ࠧ���⥫� ���� (RFC 1036)             
��������������������������������������������������������������������������������
 * Real name: Serge V.Vakulenko

  tab = binsearch (key, tab, tabsz);
  if (tab)
   return (tab->value);
  return (ERROR);
}

/*
 * ���᫥��� ��� ������.
 */
static weekday (day, mon, year)
{
  static dmsize [12] = {0,31,59,90,120,151,181,212,243,273,304,334};

  day = day-1 + dmsize [mon-1] + (year - 1970) * 365L +
   (year - 1969) / 4;
  if (year%4 == 0 && mon > 2)
   day += 1;
  return ((day + 4) % 7);
}

/*
 * �८�ࠧ������ ��ப� ���� �� Unix-�ଠ� � RFC 1036.
 */
static char *ctim2rfc (ct, tz)          /* Wdy, DD Mon YY HH:MM:SS *HHMM */
register char *ct;                      /* 0         0         0         0 */
{                                       /* Wdy Mon DD HH:MM:SS YYYY */
  if (ct[8] == ' ')
   ct[8] = '0';
  strcpy (buf, "Wdy, DD Mon YY HH:MM:SS *HHMM");
  strncpy (buf, ct, 3);           /* Wdy */
  strncpy (buf+5, ct+8, 2);       /* DD */
  strncpy (buf+8, ct+4, 3);       /* Mon */
  strncpy (buf+12, ct+22, 2);     /* YY */
  strncpy (buf+15, ct+11, 8);     /* HH:MM:SS */
  settz (buf, tz);
  return (buf);
}

/*
 * ������� ���� � �⠭���� RFC 1036.
 */
char *genrfcdate (tim, tz)
long tim;
{
  return (ctim2rfc (ctime (&tim), tz));
}

/*
 * �८�ࠧ������ ���� � �⠭���� RFC 1036.
 */
char *rfcdate (ctim)
char *ctim;
{
# define skip(p) while (*(p)==' ' || *(p)=='\t') ++(p)
  int wday, day, mon, year, h, m, s, tz;

  skip (ctim);
  if (ctim[3]==' ' && ctim[7]==' ' && ctim[10]==' ' &&
      ctim[13]==':' && ctim[16]==':' && ctim[19]==' ')
   return (ctim2rfc (ctim, 0));

  /* [Wdy,] DD Mon YY HH:MM[:SS] TIMEZONE */

  if (*ctim>='A' && *ctim<='Z' || *ctim>='a' && *ctim<='z') {
   wday = getword (&ctim, daytab, KEYSIZE (daytab));
   if (wday == ERROR)
      return (0);
   if (*ctim == ',')
      ++ctim;
   skip (ctim);
  } else
   wday = -1;

  day = getint (&ctim);
  if (day < 0)
   return (0);

  skip (ctim);
  mon = getword (&ctim, monthtab, KEYSIZE (monthtab));
  if (mon == ERROR)
   return (0);

  skip (ctim);
  year = getint (&ctim);
  if (year < 0)
   return (0);
  if (year < 70)
   year += 2000;
  else if (year < 100)
   year += 1900;
  if (year == 1970)               /* 1 Jan 1970? illegal date */
   return (0);

  skip (ctim);
  if (*ctim>='0' && *ctim<='9') {
   h = getint (&ctim);
   if (h<0 || *ctim!=':')
      return (0);
   ++ctim;

   skip (ctim);
   m = getint (&ctim);
   if (m < 0)
      return (0);

   if (*ctim == ':') {
      ++ctim;
      skip (ctim);
      s = getint (&ctim);
      if (s < 0)
       return (0);
   } else
      s = 0;

   skip (ctim);
  } else {
   h = 12;
   m = s = 0;
  }
  if (*ctim=='+' || *ctim=='-') {
   int sign = (*ctim == '+');
   ++ctim;
   skip (ctim);
   tz = getint (&ctim);
   if (tz < 0)
      return (0);
   tz = tz / 100 * 60 + tz % 100;
   if (sign)
      tz = -tz;
  } else if (*ctim) {
   tz = getword (&ctim, zonetab, KEYSIZE (zonetab));
   if (tz == ERROR)
      return (0);
  } else
   tz = 0;

  if (wday < 0)
   wday = weekday (day, mon, year);

  sprintf (buf, "%s, %02d %s %02d %02d:%02d:%02d *HHMM",
   dayname [wday], day, monthname [mon-1],
   year % 100, h, m, s);
  settz (buf, tz);
  return (buf);
}

# ifdef DEBUG1
main ()
{
  long t;
  extern long time ();

  srand ((int) time ((long *) 0));
  for (;;) {
   t = (long) rand () * rand ();
   printf ("%s\n", genrfcdate (t, (rand () % 24 - 12) * 60));
  }
}
# endif

# ifdef DEBUG2
main ()
{
  char line [256];
  char *p;

  while (gets (line)) {
   p = rfcdate (line);
   if (p)
      printf ("%-48s %s\n", line, p);
   else
      printf ("Bad date: %s\n", line);
  }
  return (0);
}
# endif

---
 * Origin: Gated from RelCom by NetDialogue (2:5020/23.0)

