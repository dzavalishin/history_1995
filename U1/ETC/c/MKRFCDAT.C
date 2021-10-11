─ REL.SOURCES (2:5020/23.77@fidonet.org) ───────────────────────── REL.SOURCES ─
 Msg  : #12 [14]                                                                
 From : vak@jee                     2:5020/23               Tue 12 May 92 19:49 
 To   : All                                                 Wed 13 May 92 02:32 
 Subj : (1) [src] Преобразование даты в соответствии с RFC 822, RFC 1036        
────────────────────────────────────────────────────────────────────────────────
 * Real name: Serge V.Vakulenko


/*
 * Преобразование времени и временной зоны в
 * строку даты в стандарте RFC 1036.
 *
 *     char *genrfcdate (long tim, int tzm)
 *
 * Параметр tim задает время, tzm - сдвиг временной зоны
 * в минутах, положительные значения к западу от Гринвича.
 * Возвращает указатель на внутренний статический буфер.
 *
 * Приведение строки даты к стандарту RFC 1036.
 *
 *     char *rfcdate (char *ctim)
 *
 * Строка ctim должна представлять собой дату в стандарте
 * RFC 822 или в стандарте Unix.
 * Возвращает указатель на внутренний статический буфер.
 * Если входная строка ошибочна, возвращает 0.
 *
 * Автор: Сергей Вакуленко, vak@kiae.su
 * Tue May 12 23:13:46 MSD 1992
 */

/*
 * Выдержка из RFC 1036:
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
 * Выдержка из RFC 822:
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

# define ERROR  -32000

# define HRS    *60
# define HALFHR 30

struct keytab {
  char *name;
  int value;
};

static struct keytab daytab [] = {
  "sun",          0,
  "mon",          1,
  "tue",          2,
  "wed",          3,
  "thu",          4,
  "fri",          5,
  "sat",          6,
  0,              0,
};

static struct keytab monthtab [] = {
  "jan",          1,
  "feb",          2,
  "mar",          3,
  "apr",          4,
  "may",          5,
  "jun",          6,
  "jul",          7,
  "aug",          8,
  "sep",          9,
  "oct",          10,
  "nov",          11,
  "dec",          12,
  0,              0,
};

static struct keytab zonetab [] = {
  "cdt",          5 HRS,          /* Central Summer */
  "cst",          6 HRS,          /* Central */
  "edt",          4 HRS,          /* Eastern Summer */
  "est",          5 HRS,          /* Eastern */
  "gmt",          0 HRS,          /* Universal Time */
  "mdt",          6 HRS,          /* Mountain Summer */
  "mst",          7 HRS,          /* Mountain */
  "msk",          -3 HRS,         /* Moscow */
  "msd",          -4 HRS,         /* Moscow Summer */

---
 * Origin: Gated from RelCom by NetDialogue (2:5020/23.0)

─ REL.SOURCES (2:5020/23.77@fidonet.org) ───────────────────────── REL.SOURCES ─
 Msg  : #13 [14]                                                                
 From : vak@jee                     2:5020/23               Tue 12 May 92 19:49 
 To   : All                                                 Wed 13 May 92 02:32 
 Subj : (2) [src] Преобразование даты в соответствии с RFC 822, RFC 1036        
────────────────────────────────────────────────────────────────────────────────
 * Real name: Serge V.Vakulenko

  "pdt",          7 HRS,          /* Pacific Summer */
  "pst",          8 HRS,          /* Pacific */
  "ut",           0 HRS,          /* Universal Time */
  "a",            1 HRS,
  "b",            2 HRS,
  "c",            3 HRS,
  "d",            4 HRS,
  "e",            5 HRS,
  "f",            6 HRS,
  "g",            7 HRS,
  "h",            8 HRS,
  "i",            9 HRS,
  "k",            10 HRS,
  "l",            11 HRS,
  "m",            12 HRS,
  "n",            -1 HRS,
  "o",            -2 HRS,
  "p",            -3 HRS,
  "q",            -4 HRS,
  "r",            -5 HRS,
  "s",            -6 HRS,
  "t",            -7 HRS,
  "u",            -8 HRS,
  "v",            -9 HRS,
  "w",            -10 HRS,
  "x",            -11 HRS,
  "y",            -12 HRS,
  "z",            0 HRS,
  0,              0,
};

static char *dayname [] = {
  "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat",
};

static char *monthname [] = {
  "???",
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
};

static char buf [40];
extern char *ctime ();

static settz (b, tz)
register char *b;
register tz;
{
  switch (tz) {
  case 0 HRS:                             /* GMT Universal */
   strcpy (b+24, "GMT");
   break;
  case 5 HRS:                             /* EST Eastern: -5 */
   strcpy (b+24, "EST");
   break;
  case 6 HRS:                             /* CST Central: -6 */
   strcpy (b+24, "CST");
   break;
  case 7 HRS:                             /* MST Mountain: -7 */
   strcpy (b+24, "MST");
   break;
  case 8 HRS:                             /* PST Pacific: -8 */
   strcpy (b+24, "PST");
   break;
  default:
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
   break;
  }
}

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

static getword (p, tab)
register char **p;
struct keytab *tab;
{
  register n;
  char key [20];

  if (! (**p>='A' && **p<='Z' || **p>='a' && **p<='z'))
   return (ERROR);
  n = 0;
  while (n<sizeof(key) && (**p>='A' && **p<='Z' || **p>='a' && **p<='z'))
   key [n++] = *(*p)++;
  if (n >= sizeof (key))
   return (ERROR);
  key [n] = 0;
  for (n=0; key[n]; ++n)
   if (key[n]>='A' && key[n]<='Z')
      key [n] += 'a' - 'A';
  for (n=0; tab[n].name; ++n)
   if (! strcmp (key, tab[n].name))
      return (tab[n].value);
  return (ERROR);
}

/*
 * Вычисление дня недели.
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
 * Преобразование строки даты из Unix-формата в RFC 1036.
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
 * Генерация даты в стандарте RFC 1036.
 */
char *genrfcdate (tim, tz)
long tim;
{
  return (ctim2rfc (ctime (&tim), tz));
}

/*
 * Преобразование даты в стандарт RFC 1036.
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
   wday = getword (&ctim, daytab);
   if (wday == ERROR)
      return (0);
   if (*ctim == ',')
      ++ctim;
   skip (ctim);
  } else
   wday = -1;       /* ??? */

  day = getint (&ctim);
  if (day < 0)
   return (0);

  skip (ctim);
  mon = getword (&ctim, monthtab);
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

  skip (ctim);
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
   tz = getword (&ctim, zonetab);
   if (tz == ERROR)
      return (0);
  } else
   tz = 0;

  if (wday < 0)
   wday = weekday (day, mon, year);

  sprintf (buf, "%s, %02d %s %02d %02d:%02d:%02d *HHMM",
   dayname [wday], day, monthname [mon],
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

---
 * Origin: Gated from RelCom by NetDialogue (2:5020/23.0)

─ REL.SOURCES (2:5020/23.77@fidonet.org) ───────────────────────── REL.SOURCES ─
 Msg  : #14 [14]                                                                
 From : vak@jee                     2:5020/23               Tue 12 May 92 19:49 
 To   : All                                                 Wed 13 May 92 02:32 
 Subj : (3) [src] Преобразование даты в соответствии с RFC 822, RFC 1036        
────────────────────────────────────────────────────────────────────────────────
 * Real name: Serge V.Vakulenko

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
}
# endif

---
 * Origin: Gated from RelCom by NetDialogue (2:5020/23.0)

