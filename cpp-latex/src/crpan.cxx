/* PANCHANG */

#include "ast.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cassert>

#define N_PANJ1_COLS 12
#define N_PANJ2_COLS 13
#define N_PANJ3_COLS 20
#define N_PANJ4_COLS 15

// using std::string;
using namespace std;

bool debugging = true;

const char* lday_name[] = {
  "<m>n~aayaR</m>",
  "<m>tin\"kaL</m>",
  "<m>covva</m>",
  "<m>budhan</m>",
  "<m>vyaazhaM</m>",
  "<m>veLLi</m>",
  "<m>shani</m>"
};

static const char* mmonths[] = {
  "<m>meeTa</m>",
  "<m>iTava</m>",
  "<m>mithuna</m>",
  "<m>kaRkkaTaka</m>",
  "<m>cin\"n\"a</m>",
  "<m>kanni</m>",
  "<m>tulaa</m>",
  "<m>v.rshcika</m>",
  "<m>dhanu</m>",
  "<m>makara</m>",
  "<m>kuMbha</m>",
  "<m>miina</m>",
};

static const char* mal_months[] = {
  "MEDAM", "IDAVAM", "MITHUNAM", "KARKADAKAM", "CHINGAM", "KANNI", "THULAM", "VRUSHCHIKAM", "DHANU", "MAKARAM", "KUMBHAM", "MEENAM"
};


static const char* mmmonths[] = {
  "\\RasiA", "\\RasiB", "\\RasiC", "\\RasiD", "\\RasiE", "\\RasiF", 
  "\\RasiG", "\\RasiH", "\\RasiI", "\\RasiJ", "\\RasiK", "\\RasiL"
};

static const char* msmonths[] = {
  "<m>caitraM</m>", "<m>vaishaakhaM</m>", "<m>jyeeSThaM</m>", "<m>aaSaaDhaM</m>", "<m>shraavaNaM</m>", "<m>bhaadrapadaM</m>",
  "<m>aashvinaM</m>", "<m>kaaRttikaM</m>", "<m>maaRggashiiRSaM</m>", "<m>pauSaM</m>", "<m>maaghaM</m>", "<m>phaalgunaM</m>"
};

static const char* short_mmonths[] = {
  "<m>meeTaM</m>",
  "<m>iTavaM</m>",
  "<m>mithu.</m>",
  "<m>kaRkka.</m>",
  "<m>cin\"n\"aM</m>",
  "<m>kanni</m>",
  "<m>tulaaM</m>",
  "<m>v.rshci.</m>",
  "<m>dhanu</m>",
  "<m>makaraM</m>",
  "<m>kuMbhaM</m>",
  "<m>miinaM</m>",
};

static const char* very_short_mmonths[] = {
  "<m>mee</m>",
  "<m>i</m>",
  "<m>mi</m>",
  "<m>kaR</m>",
  "<m>ci</m>",
  "<m>ka</m>",
  "<m>tu</m>",
  "<m>v.r</m>",
  "<m>dha</m>",
  "<m>ma</m>",
  "<m>ku</m>",
  "<m>mii</m>",
};

static const char* engMonths[] = {
  "<m>januvari </m>",
  "<m>phebruvari</m>",
  "<m>maaRcc</m>",
  "<m>eepril</m>",
  "<m>mey</m>",
  "<m>juuN</m>",
  "<m>juulai</m>",
  "<m>oogasRR</m>",
  "<m>sepRRaMbaR</m>",
  "<m>okToobaR</m>",
  "<m>navaMbaR</m>",
  "<m>DisambaR</m>"
};

static void divide(int num, int den, int& quot, int& rem)
{
  div_t d = div(num, den);
  quot = d.quot;
  rem = d.rem;
}

static bool isLeapYear(int year)
{
  return ((year % 400 == 0)|| ((year % 4 == 0) && (year % 100 != 0)));
}

static int get_day_number_in_year(int y, int m, int d)
{
  return ( (275 * m / 9)
           - ((isLeapYear(y)? 1 : 2) * ((m + 9) / 12))
           + d - 30 );
}

static int get_year_day_of_date(struct date& d)
{
  return get_day_number_in_year(d.da_year, d.da_mon, d.da_day);
}

static bool get_month_and_day_from_day_number(int day_number, int year, int &month, int &day)
{
  if (day_number < 1 || day_number > 366) return false;
  if (!isLeapYear(year) && day_number == 366) return false;
#if 1
  if (day_number < 32) {
    month = 1;
    day = day_number;
  } else {
    int k = isLeapYear(year) ? 1 : 2;
    month = int(9.0 * (k + day_number)/275.0 + 0.98);
    day = day_number - (275 * month / 9) + k * ((month+9)/12) + 30;
  }
#else
  static int month_starts[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
  static int leap_month_starts[] = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};
  int* p;
  int* start = isLeapYear(year) ? month_starts : leap_month_starts;
  p = find_if(start, start+12, bind2nd(greater<int>(), day_number));
  --p;
  month = (p - start) + 1;
  day = day_number - *p;
#endif
  return true;
}

// Converting Gregorian Date to (Modern) Saka Date
static void gregorianToSaka(int gy, int gm, int gd, int& sy, int& sm, int &sd)
{
  static int saka_month_beginnings[] = {21, // Last december
                                        20, // January
                                        19, 20, 20, 21, 21, 22, 22, 22, 22, 21, 
                                        21  // December
  };

  static int days_in_greg_months[] = {31,  // Last december,
                                      31,  // January
                                      28, 31, 30, 31, 30, 31, 31, 30, 31, 30,
                                      31  // December
  };

  if (isLeapYear(gy)) {
    saka_month_beginnings[3] = 20;
    days_in_greg_months[2] = 29;
  } else {
    saka_month_beginnings[3] = 21;
    days_in_greg_months[2] = 28;
  }

  int prevLast = saka_month_beginnings[gm];
  if (gd > prevLast) {
    sd = gd - prevLast;
    sm = gm - 2;
  } else {
    sd = gd + days_in_greg_months[gm-1] - saka_month_beginnings[gm-1];
    sm = gm -1;
  }

  if (gm < 3) {
    sy = gy - 79;
  } else if (gm > 3) {
    sy = gy - 78;
  } else { // gm = 3
    sy = gy - (gd > prevLast ? 78 : 79);
  }
}

int get_easter_year_day(int year)
{
  int a, b, c, d, e, f, g, h, i, k, l, m, n, p;
  int dummy;
   
  divide(year, 19, dummy, a);
  divide(year, 100, b, c);
  divide(b, 4, d, e);
  divide(b+28, 25, f, dummy);
  divide(b-f+1, 3, g, dummy);
  divide(19*a + b - d - g + 15, 30, dummy, h);
  divide(c, 4, i, k);
  divide(32 + 2*e + 2*i - h - k, 7, dummy, l);
  divide(9 + 11*h + 22*l, 451, m, dummy);
  divide(h + l - 7*m + 114, 31, n, p);
  return get_day_number_in_year(year, n, p+1);
}

static int get_hind_rasi(double longitude)
{
  longitude = norm_angle(longitude - ayanamsa);
  return ((int)(longitude / 30.0));
}

static char* ast_angle_string(double angle, char direction, char* str, bool abbrev, bool suppress_repeat)
{
  ST_DMS dms;
  dms_time(angle, &dms);
  int rasi = dms.degrees / 30;
  int d = dms.degrees % 30;
  int m = dms.minutes;

#if 1
  if (suppress_repeat) {
    if (direction == 'R') {
      sprintf(str, "& {\\bf\\em %02d:%02d}\n", d, m);
    } else {
      sprintf(str, "& %02d:%02d\n", d, m);
    }
  } else {
    if (direction == 'R') {
      sprintf(str, "{\\bf\\em %s} & {\\bf\\em %02d:%02d}\n", (abbrev ? very_short_mmonths[rasi] : mmmonths[rasi]), d, m);
    } else {
      sprintf(str, " %s & %02d:%02d", (abbrev ? very_short_mmonths[rasi] : mmmonths[rasi]), d, m);
    }
  }
#else
  if (suppress_repeat) {
    sprintf(str, "& %c02d^\\circ{} %02d\'$\n", direction, d, m);
     
  } else {
    sprintf(str, " %s & %c$%02d^\\circ{} %02d\'$\n", (abbrev ? very_short_mmonths[rasi] : mmmonths[rasi]), direction, d, m);
  }
#endif

  // sprintf(str, "%d & $%02d^\\circ{} %02d\'$\n", rasi, d, m);
  // cout << angle << endl;
  // cout << str << endl << endl;
  return str;
}

static char* hind_lsphutam(ST_PL_DET& pl_det, char* str, bool abbrev, bool suppress_repeat)
{
  return (ast_angle_string(norm_angle(pl_det.geo_long - ayanamsa), pl_det.direction, str, abbrev, suppress_repeat));
}

static char *disp_time_string(double j_day, char *any_str)
{
  struct date dd ;
  struct time tt ;
  char am_pm;

  get_date_time(j_day, &dd, &tt);

  am_pm = (tt.ti_hour > 11 ? 'P' : 'A');
  if (tt.ti_hour > 12) {
    tt.ti_hour -= 12;
  } else {
    if (tt.ti_hour == 0){
      tt.ti_hour = 12;
    }
  }
   
  sprintf(any_str,"%04d/%02d/%02d  %02d:%02d %cM",dd.da_year,dd.da_mon, dd.da_day,
          tt.ti_hour,tt.ti_min, am_pm);
  return(any_str);
}

static char *hm_string(double j_day,char *any_str)
{
  struct date dd ;
  struct time tt ;
  char am_pm;
  int h, m;

  if (j_day < 0) {
    strcpy(any_str, " ");
    return any_str;
  }
  get_date_time(j_day, &dd, &tt);

  h = tt.ti_hour;
  m = tt.ti_min;

  if ( h < 12 ) {
    am_pm = 'A';
    if (0 == h) {
      h = 12;
    }
  } else {
    am_pm = 'P';
    if (h > 12) {
      h -= 12;
    }
  }
  sprintf(any_str,"%d:%02d%c", h, m, am_pm);
  return(any_str);
}


static void get_date(double curr_j_date,int *curr_year, int *curr_month,
                     int *curr_day)
{
  struct date dd ;
  struct time tt ;

  get_date_time(curr_j_date, &dd, &tt);
  *curr_year = dd.da_year ;
  *curr_month = dd.da_mon ;
  *curr_day = dd.da_day ;
}

static char* get_naks_string(int nak, double j_day, int left, char* naks_str)
{
  char str[20];
  char na_name[15];
  int nak1;
  if (nak < 0) {
    strcpy(naks_str, " & ");
  } else if (j_day < 0.0) {
    sprintf(na_name, "\\Star%c", ((nak < 26) ? 'A' + nak : 'a' + nak-26));
    strcpy(naks_str, "{\\bf\\em ");
    strcat(naks_str, na_name);
    strcat(naks_str, "}");
    strcat(naks_str, "&");
  } else {
    sprintf(na_name, "\\Star%c", ((nak < 26) ? 'A' + nak : 'a' + nak-26));
    if (left) {
      strcpy(naks_str, "{\\bf\\em ");
    }
    strcat(naks_str, na_name);
    if (left) {
      strcat(naks_str, "}");
    }
    strcat(naks_str, "/");
    nak1 = (nak+1)%27;
    sprintf(na_name, "\\Star%c", ((nak1 < 26) ? 'A' + nak1 : 'a' + nak1-26) );
    if (!left) {
      strcat(naks_str, "{\\bf\\em ");
    }
    strcat(naks_str, na_name);
    if (!left) {
      strcat(naks_str, "}");
    }
    strcat(naks_str, " & ");
    hm_string(j_day, str);
    strcat(naks_str, str);
  }
  return naks_str;
}

static char* get_alt_naks_string(const char* prefix, int nak, double j_day, char* naks_str)
{
  char str[20];
  char hm_str[20];

  if (nak < 0) {
    strcpy(naks_str, "  ");
    return naks_str;
   } else {
     sprintf(str,  "%s", nak_name_alt[nak]);
  }
  hm_string(j_day, hm_str);
  if (strlen(hm_str) == 6) {
    sprintf(naks_str, "%-15s %s%-8s", str, prefix, hm_str);
  } else {
    sprintf(naks_str, "%-15s  %s%-7s", str, prefix, hm_str);
  }

  if (debugging) {
    fprintf(stderr, "nak = %d,  str = %s\n", nak, naks_str);
  }
  return naks_str;
}

static char* get_alt_thithis_string(const char* prefix, int thi, double j_day, char* thith_str)
{
  char hm_str[20];
  char t_str[20];
  char th_name[15];
  if (thi < 0) {
    strcpy(thith_str, " ");
    return thith_str;
   } else {
     sprintf(t_str, "%s", thithi_name_2[thi]);
  }
  hm_string(j_day, hm_str);
  if (strlen(hm_str) == 6) {
    sprintf(thith_str, "%-15s %s%-8s", t_str, prefix, hm_str);
  } else {
    sprintf(thith_str, "%-15s  %s%-7s", t_str, prefix, hm_str);
  }
  if (debugging) {
    fprintf(stderr, "thi = %d, str = %s\n", thi, thith_str);
  }
  return thith_str;
}


static char* get_thithis_string(int thi, double j_day, int left, char* thith_str)
{
  char str[20];
  char th_name[15];
  int thi1;
  if (thi < 0) {
    strcpy(thith_str, " & ");
  } else if (j_day < 0.0) {
    sprintf(th_name, "\\Thithi%c", ((thi < 26) ? 'A' + thi : 'a' + thi-26));
    strcpy(thith_str, "{\\bf\\em ");
    strcat(thith_str, th_name);
    strcat(thith_str, "}");
    strcat(thith_str, "& ");
  } else {
    sprintf(th_name, "\\Thithi%c", ((thi < 26) ? 'A' + thi : 'a' + thi-26));
    if (left) {
      strcpy(thith_str, "{\\bf\\em ");
    }
    strcat(thith_str, th_name);
    if (left) {
      strcat(thith_str, "}");
    }
    strcat(thith_str, "/");
    thi1 = (thi+1)%30;
    sprintf(th_name, "\\Thithi%c", ((thi1 < 26) ? 'A' + thi1 : 'a' + thi1-26));
    if (!left) {
      strcat(thith_str, "{\\bf\\em ");
    }
    strcat(thith_str, th_name);
    if (!left) {
      strcat(thith_str, "}");
    }
    strcat(thith_str, " & ");
    hm_string(j_day, str);
    strcat(thith_str, str);
  }
  return thith_str;
}


static char *conv_to_str(double longitude, char *s)
{
  int z ;
  double d ;
  char temp_str[12] ;

  while (longitude >= FULL_CIRCLE)
    longitude -= FULL_CIRCLE ;
  while (longitude < 0.0 )
    longitude += FULL_CIRCLE ;
  z = (int)(longitude / 30.0) ;
  d = longitude - 30.0 * z ;
  time_string(d, DEGREE, PART, temp_str);
  sprintf(s,"* %c%c%c%s",zod_name[z][0], zod_name[z][1],zod_name[z][2],
          temp_str) ;
  return s ;
}

static int getDayNumberFromDate(int year, int month, int day)
{
  struct date d;
  d.da_year = year;
  d.da_mon = month;
  d.da_day = day;
  return int(julian_day(&d, 0.0));
}

static void getDateFromDayNumber(int dayNumber, int& year, int& month, int& day)
{
  struct date d;
  struct time t;
  double j_day = dayNumber + 0.5;
  get_date_time(j_day, &d, &t);
  year = d.da_year;
  month = d.da_mon;
  day = d.da_day;
}

// Returns 0 for sunday, ...., 6 for saturday
int weekdayFromDayNumber(int dayNumber)
{
  return (dayNumber+2) % 7;
}

int weekdayOfDate(int year, int month, int day)
{
  return weekdayFromDayNumber(getDayNumberFromDate(year, month, day));
}

static int getWeekdayNumberOnOrBefore(int dayNumber, int weekday)
{
  return dayNumber - weekdayFromDayNumber(dayNumber - weekday);
}

static int getWeekdayNumberBefore(int dayNumber, int weekday)
{
  return getWeekdayNumberOnOrBefore(dayNumber-1, weekday);
}

static int getWeekdayNumberOnOrAfter(int dayNumber, int weekday)
{
  return getWeekdayNumberOnOrBefore(dayNumber+6, weekday);
}

static void getDateOfWeekdayOnOrBefore(int year, int month, int day, 
                                       int weekday,
                                       int& y, int& m, int& d)
{
  int dayNumber = getDayNumberFromDate(year, month, day);
  int newDayNumber = getWeekdayNumberOnOrBefore(dayNumber, weekday);
  getDateFromDayNumber(newDayNumber, y, m, d);
}

static void getDateOfWeekdayBefore(int year, int month, int day, 
                                   int weekday,
                                   int& y, int& m, int& d)
{
  int dayNumber = getDayNumberFromDate(year, month, day);
  int newDayNumber = getWeekdayNumberBefore(dayNumber, weekday);
  getDateFromDayNumber(newDayNumber, y, m, d);
}

static void getDateOfWeekdayOnOrAfter(int year, int month, int day, 
                                      int weekday,
                                      int& y, int& m, int& d)
{
  int dayNumber = getDayNumberFromDate(year, month, day);
  int newDayNumber = getWeekdayNumberOnOrAfter(dayNumber, weekday);
  getDateFromDayNumber(newDayNumber, y, m, d);
}

static int getFirstNthDay(int year, int month, 
                          int weekday,   // 0 = sunday, ...., 6 = saturday
                          int n
                          )
{
  assert(n < 6);
  int x = getWeekdayNumberOnOrAfter(getDayNumberFromDate(year, month, 7 * n - 6), weekday);
  return x;
}

static int getLastNthDay(int year, int month, 
                         int weekday,   // 0 = sunday, ...., 6 = saturday
                         int n
                         )
{
  assert (n<6);
  ++month;
  if (month > 12) {
    month = 1;
    ++year;
  }
  int x = getWeekdayNumberBefore(getDayNumberFromDate(year, month, 1), weekday);
  return x;
}

// American holidays.  Returns the day, because we know the month in all cases.

static int getPresidentsDay(int year, int& month)
{
  // Third Monday in February
  month = 2;
  return getFirstNthDay(year, month, 1, 3);
}

static int getThanksgivingDay(int year, int& month)
{
  // Fourth Thursday in November
  month = 11;
  return getFirstNthDay(year, month, 4, 4);
}
static int getMemorialDay(int year, int& month)
{
  // Last Monday in May
  month = 5;
  return getLastNthDay(year, month, 1, 1);

}

static int getLaborDay(int year, int& month)
{
  // First Monday in September
  month = 9;
  return getFirstNthDay(year, month, 1, 1);
}

static int getDSTStartDay(int year, int& month)
{
  // First Sunday of April
  month = 4;
  return getFirstNthDay(year, month, 0, 1);
}

static int getDSTEndDay(int year, int& month)
{
  // Last Sunday in May
  month = 10;
  return getLastNthDay(year, month, 0, 1);
}

static int getFathersDay(int year, int& month)
{
  // Third Sunday in June
  month = 6;
  return getFirstNthDay(year, month, 0, 3);
}

static int getMothersDay(int year, int& month)
{
  // Second Sunday in May
  month = 5;
  return getFirstNthDay(year, month, 0, 2);
}

static int getColumbusDay(int year, int& month)
{
  // Second Monday in October
  month = 10;
  return getFirstNthDay(year, month, 1, 2);
}


static int getMLKDay(int year, int& month)
{
  // Third Monday in January
  month = 1;
  return getFirstNthDay(year, month, 1, 3);
}



class Vishesham {
 public:
  enum Type { INDIAN, WORLD, AMERICAN};
  Vishesham(const std::string& desc, int d, Type t) : shortDescription(desc), day(d), type (t) {}
  bool operator < (const Vishesham& another) const
  {
    return ((day < another.day) || ((day == another.day) && shortDescription < another.shortDescription));
  }

 public:
  std::string shortDescription;
  int day;
  Type type;
};


class CalendarCreator {
 public:
  CalendarCreator(const string& dataFileName, const string& outFileName1, const string& outFileName2, const string& outFileName3, const string& outFileName4, const string& outFileName5 );

  bool createCalendar(const string& PlaceId);
  void print_panjang (double j_day, int n_days, const char *time_zone, double latitude, double longitude);
  double find_lagnam(double j_day);
  double getLatitude() const { return latitude; }
  double getLongitude() const { return longitude; }
  double localJulianDay(double jDay)
  {
    return jDay + (timeZone + dstCorrection) / 24.0;
  }

  int yearDayNumber(int dayNumber)
  {
    return dayNumber - previousYearLastDayNumber;
  }

 private:
  bool checkIfValid();

  static void trimLine(string& str)
  {
  }


  /*   ------------------------------------------------
       Returns the day number from the beginning of the year
       Jan 01 : 1, Dec 31 : 365 or 366
       ------------------------------------------------
  */


  void print_doc_head() const;
  void print_doc_main_head(FILE* fp, const char* section_heading) const;
  void print_doc_tail() const;
  void print_month_head (int curr_year, int curr_month, const char *time_zone, double j_day);
  void print_month_top_heading(FILE* fp, int nColumns, int curr_year, int curr_month, const char* samkrama_str);
  void print_month_tail(int curr_year, int curr_month, const char *time_zone);
  void print_common_month_tail(FILE* fp, int nColumns, int curr_year, int curr_month, const char *time_zone);
  void print_details(double j_day, struct date& curr_date, ST_PL_DET pl_det[], double latitude, double longitude);
  int  dayNumberOfMalMonthBeginning(double transitJDay);
  void printHolidays(FILE* fp);
  void find_rahu_kalam(double udayam, double astamayam, DURATION rahuKalam);
  void find_gulika_kalam(double udayam, double astamayam, DURATION gulikaKalam);
  void find_yamakantaka_kalam(double udayam, double astamayam, DURATION yamakantakaKalam);
  void find_madhyahnam(double udayam, double astamayam, DURATION kalam);
  char* duration_to_str(const DURATION duration, char* str);
  char* alt_duration_to_str(const DURATION duration, char* str);
  void printVisheshams(FILE* fp, const std::vector<Vishesham>& visheshams);
 private:
  int year;
  string name;
  double latitude;
  double longitude;
  string latitudeString;
  string longitudeString;
  double timeZone;
  bool dst;
  bool northernHemisphere;
  string shortStdTZ;
  string longStdTZ;
  string shortDstTZ;
  string longDstTZ;
  string dstStart;
  string dstEnd;
  int dstStartDayNumber;
  int dstEndDayNumber;
  int dst2stdMonth;
  int std2dstMonth;
  int nDays;
  string dataFile;
  string outFile1;
  string outFile2;
  string outFile3;
  string outFile4;
  string outFile5;
  int nextNewMonthDay;
  int malDay;
  int malMonth;
  double dstCorrection;
  FILE* outFp1;    // General
  FILE* outFp2;    // Kalams
  FILE* outFp3;    // Sphutams
  FILE* outFp4;    // Lagnams
  FILE* outFp5;    // Uranus, Neptune, Pluto, Ayanamsham
  FILE* alt_fp;
  int prev_rasi[DESC_NODE+1];
  int last_lagnam;
  int todaysStar;
  int yesterdaysStar;
  int todaysThithi;
  int yesterdaysThithi;
  int tonightsThithi;
  int vishuDay;
  int thiruvonamDay;
  int durgaashtamiDay;
  int mahaanavamiDay;
  int vijayadasamiDay;
  int sivaratriDay;
  int deepavaliDay;
  std::vector<int> pradoshams;
  std::vector<int> ekadasis;
  std::vector<int> shastis;
  bool lastDayWasPradosham;
  bool inNavaratri;
  int easterYearDay;
  int easterDay;
  int ashWDay;
  int passionSDay;
  int palmSDay;
  int maundyTDay;
  int goodFDay;
  int regationSDay;
  int ascensionDay;
  int penteCostDay;
  int trinitySDay;
  int previousYearLastDayNumber;
  bool is_month_at_sunrise;
  bool alt_fp_written;
};

class MatchFinder {
 public:
  MatchFinder(double latitude, double longitude, int cycle) 
      : myLatitude(latitude), myLongitude(longitude), myCycle(cycle) {}
  virtual ~MatchFinder();
  virtual int findMatches(double start_j_day, double end_j_day);
  virtual int matchFunction(double j_day) = 0;
  std::vector< std::pair<int, double> > get_end_values() {
    return myEndValues;
  }
  int getCycle() const { return myCycle; }
 
 private:
  double findNextMatch(int val, double start_j_day, double end_j_day);
   
 private:
  double myLatitude;
  double myLongitude;
  std::vector< std::pair<int, double> > myEndValues;
  int myCycle;
};


class LagnamMatchFinder : public MatchFinder {
 public:
  LagnamMatchFinder(CalendarCreator* p) 
      : MatchFinder(p->getLatitude(), p->getLongitude(), 12),  myCCPtr(p) {}
  virtual int matchFunction(double j_day);
 private:
  CalendarCreator* myCCPtr;
};

MatchFinder::~MatchFinder() {}

int LagnamMatchFinder::matchFunction(double j_day) 
{
  double l = myCCPtr->find_lagnam(j_day);
  return (int)(l / 30.0);
}

CalendarCreator::CalendarCreator(const string& dataFileName, 
                                 const string& outFileName1, 
                                 const string& outFileName2, 
                                 const string& outFileName3, 
                                 const string& outFileName4,
                                 const string& outFileName5)
    : dataFile(dataFileName), 
      outFile1(outFileName1),
      outFile2(outFileName2),  
      outFile3(outFileName3), 
      outFile4(outFileName4), 
      outFile5(outFileName5), 
      year(0), 
      latitude(0.0), 
      longitude(0.0), 
      timeZone(0.0), 
      dst(false), 
      nDays(0), 
      nextNewMonthDay(0), 
      malDay(0), 
      dstStartDayNumber(0), 
      dstEndDayNumber(0), 
      dstCorrection(0.0), 
      dst2stdMonth(0),
      std2dstMonth(0),
      outFp1(0), 
      outFp2(0), 
      outFp3(0),
      outFp4(0),
      outFp5(0),
      alt_fp(0),
      last_lagnam(-1),
      todaysStar(-1),
      todaysThithi(-1),
      tonightsThithi(-1),
      lastDayWasPradosham(false),
      inNavaratri(false),
      previousYearLastDayNumber(0),
      thiruvonamDay(-1),
      vishuDay(-1),
      durgaashtamiDay(-1),
      mahaanavamiDay(-1),
      vijayadasamiDay(-1),
      sivaratriDay(-1),
      deepavaliDay(-1),
      easterDay(-1), 
      ashWDay(-1), 
      passionSDay(-1), 
      palmSDay(-1),  
      maundyTDay(-1), 
      goodFDay (-1), 
      regationSDay(-1), 
      ascensionDay(-1), 
      penteCostDay(-1), 
      trinitySDay(-1),
      is_month_at_sunrise(false)

{
}

bool
CalendarCreator::checkIfValid() 
{
  if (0 == nDays) {
    nDays = (isLeapYear(year) ? 366 : 365);
    // fprintf(stderr, "Year = %d, nDays = %d\n", year, nDays);
  } else if (nDays < 0 || nDays > 1000) {
    assert(false);
    // return false;
  }
  if (dst) {
    struct date dd;
    // cout << "DST Start = " << dstStart << endl;

    // cout << "DST End = " << dstEnd << endl;
    if (northernHemisphere) {
      // Read DST start day first, then DST end day
      if (sscanf(dstStart.c_str(), "%d/%d/%d", &dd.da_year, &dd.da_mon, &dd.da_day) < 0) {
        assert(false);
      }
      std2dstMonth = dd.da_mon;
      dstStartDayNumber = get_year_day_of_date(dd);
      if (sscanf(dstEnd.c_str(), "%d/%d/%d", &dd.da_year, &dd.da_mon, &dd.da_day) < 0) {
        assert(false);
      }
      dst2stdMonth = dd.da_mon;
      dstEndDayNumber = get_year_day_of_date(dd);
      assert(dstStartDayNumber < dstEndDayNumber);

      // Start with Standard time
      this->dstCorrection = 0.0;
    } else {
      // Read DST end day first, then DST start day
      if (sscanf(dstEnd.c_str(), "%d/%d/%d", &dd.da_year, &dd.da_mon, &dd.da_day) < 0) {
        assert(false);
      }
      dst2stdMonth = dd.da_mon;
      dstEndDayNumber = get_year_day_of_date(dd);
      if (sscanf(dstStart.c_str(), "%d/%d/%d", &dd.da_year, &dd.da_mon, &dd.da_day) < 0) {
        assert(false);
      }
      std2dstMonth = dd.da_mon;
      dstStartDayNumber = get_year_day_of_date(dd);
      assert(dstEndDayNumber < dstStartDayNumber);

      // Start with Daylight saving time
      this->dstCorrection = 1.0;

    }
  } else {
    dstStartDayNumber = dstEndDayNumber = 0;
  }

  char buf[50];
  if (latitude < -90.0 || latitude > 90.0) {
    assert(false);
  }
  ST_DMS dms;

  char dir;
  if (latitude < 0) {
    dir = 'S';
    dms_time (-latitude, &dms);
  } else {
    dir = 'N';
    dms_time (latitude, &dms);
  }
  sprintf(buf, "$%2d^\\circ{} %2d\'$ %c", dms.degrees, dms.minutes, dir);
  latitudeString = buf;

  if (longitude < 0) {
    dir = 'E';
    dms_time (-longitude, &dms);
  } else {
    dir = 'W';
    dms_time (longitude, &dms);
  }
  sprintf(buf, "$%2d^\\circ{} %2d\'$ %c", dms.degrees, dms.minutes, dir);
  longitudeString = buf;

  easterYearDay = get_easter_year_day(year);
  previousYearLastDayNumber = getDayNumberFromDate(year, 1, 1) - 1;
  
  return true;
}

bool
CalendarCreator::createCalendar(const string& placeId)
{

  ifstream is(dataFile.c_str());
  if (is.bad()) {
    cerr << "Cannot open " << dataFile << endl;
  }

  char buf[512];
  bool placeFound = false;
  string::size_type idx;
  bool error = false;

  while (is.getline(buf, 100)) {
    string line(buf);
    trimLine(line);
    if (line[0] == '[' && line[line.size()-1] == ']') {
      if (placeFound) {
        break;
      }
      string pid = line.substr(1, line.size()-2);
      if (pid == placeId) {
        placeFound = true;
        continue;
      }
    } else if (!placeFound) {
      continue;
    } else if ((idx = line.find('=')) != string::npos){
      string var = line.substr(0, idx);
      string value = line.substr(idx+1);
      if (var == "Year") {
        year = atoi(value.c_str());
      } else if (var == "Name") {
        name = value;
      } else if (var == "Longitude") {
        longitude = atof(value.c_str());
      } else if (var == "Latitude") {
        latitude = atof(value.c_str());
        if (latitude >= 0.0) {
          northernHemisphere = true;
        } else {
          northernHemisphere = false;
        }
      } else if (var == "Timezone") {
        timeZone = atof(value.c_str());
      } else if (var == "DST") {
        if (value == "Yes") {
          dst = true;
        } else if (value == "No") {
          dst = false;
        } else {
          error = true;
          assert(false);
        }
      } else if (var == "StandardTimeZoneShortName") {
        shortStdTZ = value;
      } else if (var == "StandardTimeZoneLongName") {
        longStdTZ = value;
      } else if (var == "DaylightTimeZoneShortName") {
        shortDstTZ = value;
      } else if (var == "DaylightTimeZoneLongName") {
        longDstTZ = value;
      } else if (var == "DaylightStart") {
        dstStart = value;
      } else if (var == "DaylightEnd") {
        dstEnd = value;
      } else if (var == "days") {
        nDays = atoi(value.c_str());
      } else {
        error = true;
        assert(false);
      }
    } else {
      continue;
    }
  }

  if (error || !checkIfValid()) {
    assert(false);
    // return false;
  }

  is.close();

  cout << "Place              : " << this->name << endl;
  cout << "Latitude           : " << this->latitudeString << endl;
  cout << "Longitude         : " << this->longitudeString << endl;
  cout << "Standard Time Zone : " << this->longStdTZ << endl;
  if (this->dst) {
    cout << "Daylight Time Zone : " << this->longDstTZ << endl;
  }

  struct date dd;

  dd.da_year = year ;
  dd.da_mon = 1 ;
  dd.da_day = 1 ;

  double startJulianDay = julian_day(&dd, 0.0 - timeZone);

  if ((outFp1 = fopen(outFile1.c_str(),"w")) == (FILE *) 0 )
  {
    cerr << "Cannot open " << outFile1 << endl;
    return false;
  }


  if ((outFp3 = fopen(outFile3.c_str(),"w")) == (FILE *) 0 )
  {
    cerr << "Cannot open " << outFile3 << endl;
    return false;
  }

  if ((outFp4 = fopen(outFile4.c_str(),"w")) == (FILE *) 0 )
  {
    cerr << "Cannot open " << outFile4 << endl;
    return false;
  }

  if ((outFp5 = fopen(outFile5.c_str(),"w")) == (FILE *) 0 )
  {
    cerr << "Cannot open " << outFile5 << endl;
    return false;
  }

  if ((alt_fp = fopen("alt.txt", "w")) == (FILE *) 0 )
  {
    cerr << "Cannot open " << "alt.txt" << endl;
    return false;
  }

  print_panjang(startJulianDay, nDays, shortStdTZ.c_str(), latitude, longitude) ;
  if (outFp1) fclose (outFp1) ;
  if (outFp2) fclose (outFp2) ;
  if (outFp3) fclose (outFp3) ;
  if (outFp4) fclose (outFp4) ;
  if (outFp5) fclose (outFp5) ;
	return true;
}

int CalendarCreator::dayNumberOfMalMonthBeginning(double transitJDay)
{
  int dayNumber = 0;
  // double lJday = localJulianDay(transitJDay);
  // Get date
  // Get date number
  double sunAltitude = get_sun_altitude(transitJDay, this->latitude, this->longitude);
  struct date dd;
  struct time tt;

  if (sunAltitude < 0) {
    // Night
    // Return the next day by adding half day and returning the date
    get_date_time(this->localJulianDay(transitJDay + 0.5), &dd, &tt);
    is_month_at_sunrise = true;
    return get_year_day_of_date(dd);
  } else{
    // Day
      
    double jDay, prevJDay, alt;

    // Find the sunrise before
    jDay = transitJDay;

    do {
      prevJDay = jDay;
      jDay -= 0.1;
      alt = get_sun_altitude(jDay, this->latitude, this->longitude);
    } while (alt > 0);

    double sunRiseBefore = sunrise_between(jDay, prevJDay, this->latitude, this->longitude);

    // Find the sunset after
    jDay = transitJDay;

    do {
      prevJDay = jDay;
      jDay += 0.1;
      alt = get_sun_altitude(jDay, this->latitude, this->longitude);
    } while (alt > 0);

    double sunSetAfter = sunset_between(prevJDay, jDay, this->latitude, this->longitude);


    double madhyahnam = 0.4 * sunRiseBefore + 0.6 * sunSetAfter;  // = sunRiseBefore + 0.6 * (sunSetAfter - sunRiseBefore)
    if (transitJDay > madhyahnam) {
      // Return next day
      get_date_time(this->localJulianDay(transitJDay + 0.75), &dd, &tt);
      is_month_at_sunrise = true;
      return get_year_day_of_date(dd);
    } else {
      // Return current day
      get_date_time(this->localJulianDay(transitJDay), &dd, &tt);
      is_month_at_sunrise = false;
      return get_year_day_of_date(dd);
    }
  }
}

void CalendarCreator::print_month_top_heading(FILE* fp, int nColumns, int curr_year, int curr_month, const char* samkrama_str)
{
  if (!fp) {
    return;
  }

  // Print English month
  fprintf(fp, "\\multicolumn{%d}{c}{\\Large %s %04d \\normalsize}\\\\\n", nColumns, engMonths[curr_month - 1],
  curr_year);
  if (alt_fp != NULL && !alt_fp_written) {
    fprintf(alt_fp, "\n\n\n\n\nMonth = %d %s\n", curr_year, mnth_name[curr_month - 1]);
  }

  // Print timezone information
  if (dst) {
    if (curr_month == std2dstMonth) {
      fprintf(fp, "\\multicolumn{%d}{c}{(%s / %s)}\\\\\n", nColumns, longStdTZ.c_str(), longDstTZ.c_str());
    } else if  (curr_month == dst2stdMonth){
      fprintf(fp, "\\multicolumn{%d}{c}{(%s / %s)}\\\\\n", nColumns, longDstTZ.c_str(), longStdTZ.c_str());
    } else {
      if (northernHemisphere) {
        if (curr_month > std2dstMonth && curr_month < dst2stdMonth) {
          fprintf(fp, "\\multicolumn{%d}{c}{(%s)}\\\\\n", nColumns, longDstTZ.c_str());
        } else {
          fprintf(fp, "\\multicolumn{%d}{c}{(%s)}\\\\\n", nColumns, longStdTZ.c_str());
        }
      } else {
        if (curr_month > dst2stdMonth && curr_month < std2dstMonth) {
          fprintf(fp, "\\multicolumn{%d}{c}{(%s)}\\\\\n", nColumns, longStdTZ.c_str());
        } else {
          fprintf(fp, "\\multicolumn{%d}{c}{(%s)}\\\\\n", nColumns, longDstTZ.c_str());
        }
      }
    }
  } else {
    fprintf(fp, "\\multicolumn{%d}{c}{(%s)}\\\\\n", nColumns, longStdTZ.c_str());
  }

  int mal_year, mal_month1, mal_month2;

  // Print malayala month information
  if (curr_month < 8) {
    mal_year = curr_year - 825;
  } else {
    mal_year = curr_year - 824;
  }
  mal_month1 = (curr_month + 7) % 12;
  mal_month2 = (mal_month1 + 1 ) % 12;
  printf("AAA: %d %d %d\n", curr_month, mal_month1, mal_month2);
  if (mal_month2 == 4) {
    fprintf(fp, "\\multicolumn{%d}{c}{{\\bf <m>kollavaRSaM</m>} %d %s{} - %d %s, %s%s : %s}\\\\\n", nColumns, mal_year-1, mmmonths[mal_month1], 
            mal_year, mmmonths[mal_month2], mmonths[mal_month2], "<m>saMkramaM</m>", samkrama_str);
    if (alt_fp != NULL && !alt_fp_written) {
      fprintf(alt_fp, "Malayalam Months = %d %s - %d %s, Sankaramam = %s\n\n\n", mal_year-1, mal_months[mal_month1], mal_year, mal_months[mal_month2], samkrama_str);
    }
  } else {
    fprintf(fp, "\\multicolumn{%d}{c}{{\\bf <m>kollavaRSaM</m>} %d %s{} - %s, %s%s : %s}\\\\\n", nColumns, mal_year, mmmonths[mal_month1], 
            mmmonths[mal_month2], mmonths[mal_month2], "<m>saMkramaM</m>", samkrama_str);
    if (alt_fp != NULL && !alt_fp_written) {
      fprintf(alt_fp, "Malayalam Months = %d %s - %s, Sankaramam = %s\n\n\n", mal_year, mal_months[mal_month1], mal_months[mal_month2], samkrama_str);
    }
  }

  if (alt_fp != NULL  && !alt_fp_written) {
    fprintf(alt_fp, "------------------------------------------------------------------------------------------------------\n");
    fprintf (alt_fp, "%2s  %2s  %-10s  %-17s%6s    %-17s%6s    %14s  %12s\n",
             "E", "M", "Weekday", "Star", "T1", "Thithi", "T2", "Rahukalam", "Sunrise/set");
    fprintf(alt_fp, "------------------------------------------------------------------------------------------------------\n");

  }
  malMonth = mal_month1;

  // Print saka month information
  int saka_year;
  int saka_year1 = curr_year - 79;
  int saka_year2 = saka_year1 + 1;
  int saka_month1 = (curr_month + 8) % 12 + 1;
  int saka_month2 = saka_month1 % 12 + 1;
  if (curr_month == 3) {
    fprintf(fp, "\\multicolumn{%d}{c}{{\\bf <m>shakavaRSaM</m>} %d %s{} - %d %s", nColumns, saka_year1, msmonths[saka_month1-1], 
            saka_year2, msmonths[saka_month2-1]);
     
  } else if (curr_month < 3) {
    fprintf(fp, "\\multicolumn{%d}{c}{{\\bf <m>shakavaRSaM</m>} %d %s - %s ", nColumns, saka_year1, msmonths[saka_month1-1], msmonths[saka_month2-1]);
  } else {
    fprintf(fp, "\\multicolumn{%d}{c}{{\\bf <m>shakavaRSaM</m>} %d %s - %s ", nColumns, saka_year2, msmonths[saka_month1-1], msmonths[saka_month2-1]);
  }

  fprintf(fp, "}\\\\\n");

  fprintf(fp, 
          "\\multicolumn{%d}{c}{<m>sthalaM</m>: %s. <m>akSaamshaM</m>: %s. <m>reekhaaMshaM:</m> %s.}\\\\\n",
          nColumns, name.c_str(), latitudeString.c_str(), longitudeString.c_str());

#if 0
  thiruvonamDay = -1;
  vishuDay = -1;
  durgaashtamiDay = -1;
  mahaanavamiDay = -1;
  vijayadasamiDay = -1;
  sivaratriDay = -1;
  deepavaliDay = -1;
  easterDay = ashWDay = passionSDay = palmSDay =  maundyTDay = goodFDay 
            = regationSDay = ascensionDay = penteCostDay = trinitySDay = -1;
#endif
  pradoshams.clear();
  ekadasis.clear();
  shastis.clear();

  alt_fp_written = true;
}

void CalendarCreator::print_month_head (int curr_year, int curr_month, const char *time_zone, double j_day)
{

  double transit_j_day;
  char t_str[50];
  next_sun_transit(j_day, &transit_j_day);
  nextNewMonthDay = dayNumberOfMalMonthBeginning(transit_j_day);
  disp_time_string(localJulianDay(transit_j_day), t_str);

  // Panjangam 1
  if (outFp1) {
    fprintf(outFp1, "\\clearpage\n");
    fprintf(outFp1, "\\section[%s]{}\n", engMonths[curr_month - 1]);
    fprintf(outFp1, "\\begin{tabular}{|r|r|r|l|l r|l r|r r|r r|}\n");
      
    alt_fp_written = false;
    print_month_top_heading(outFp1, N_PANJ1_COLS, curr_year, curr_month, t_str);
    alt_fp_written = true;

    fprintf(outFp1, "\\hline\n");
    fprintf(outFp1, "\\multicolumn{3}{|c|}{\\Date} & \\WeekDay & \\multicolumn{2}{|c|}{\\Nakshatram} & \\multicolumn{2}{|c|}{\\Thithi} & \\multicolumn{2}{c|}{\\Udayastamayam} & \\multicolumn{2}{c|}{\\Rahukalam}\\\\\n");
    fprintf(outFp1, "\\cline{1-3}\n");
    fprintf(outFp1, "<m>iM.</m> & <m>ma.</m> & <m>sha.</m>  & & \\multicolumn{2}{|c|}{} & \\multicolumn{2}{|c|}{} & \\multicolumn{2}{c|}{} & \\multicolumn{2}{c|}{} \\\\\n");
    fprintf(outFp1, "\\hline\n");
  }

  // Panjangam 2
  if (outFp2) {
    fprintf(outFp2, "\\clearpage\n");
      
    fprintf(outFp2, "\\section[%s]{}\n", engMonths[curr_month - 1]);
    fprintf(outFp2, "\\begin{tabular}{|r|r|l|r|r|rr|rr|rr|rr|}\n");
    print_month_top_heading(outFp2, N_PANJ2_COLS, curr_year, curr_month, t_str);
    fprintf(outFp2, "\\hline\n");
    fprintf(outFp2, "\\multicolumn{2}{|c|}{\\Date} & \\WeekDay & \\multicolumn{2}{c|}{\\Udayastamayam}");
    fprintf(outFp2, "& \\multicolumn{2}{c|}{\\Madhyahnam} & \\multicolumn{2}{c|}{\\Rahukalam}"); 
    fprintf(outFp2, "& \\multicolumn{2}{c|}{\\Gulikakalam} & \\multicolumn{2}{c|}{\\Yamakantakakalam}\\\\\n");
    fprintf(outFp2, "\\cline{1-2}\n");
    fprintf(outFp2, "<m>iM.</m> & <m>ma.</m> & & \\multicolumn{2}{c|}{} & \\multicolumn{2}{c|}{} & \\multicolumn{2}{c|}{} & \\multicolumn{2}{c|}{} & \\multicolumn{2}{c|}{} \\\\\n");
    fprintf(outFp2, "\\hline\n");
  }
   
  // Panjangam 3
  float dateLen = 0.2;
  float sphLen1 = 0.19;
  float sphLen2 = 0.37;
   
  if (outFp3) {
    fprintf(outFp3, "\\clearpage\n");
      
    fprintf(outFp3, "\\section[%s]{}\n", engMonths[curr_month - 1]);
    fprintf(outFp3, "\\begin{tabular}{|p{%4.2fin}|p{%4.2fin}|p{%4.2fin}p{%4.2fin}|p{%4.2fin}p{%4.2fin}|p{%4.2fin}p{%4.2fin}|p{%4.2fin}p{%4.2fin}|p{%4.2fin}p{%4.2fin}|p{%4.2fin}p{%4.2fin}|p{%4.2fin}p{%4.2fin}|p{%4.2fin}p{%4.2fin}|p{%4.2fin}p{%4.2fin}|}\n",
            dateLen, dateLen, sphLen1, sphLen2, sphLen1, sphLen2, sphLen1, sphLen2, 
            sphLen1, sphLen2, sphLen1, sphLen2, sphLen1, sphLen2, sphLen1, sphLen2, 
            sphLen1, sphLen2, sphLen1, sphLen2);
    print_month_top_heading(outFp3, N_PANJ3_COLS, curr_year, curr_month, t_str);
    fprintf(outFp3, "\\hline\n");
    fprintf(outFp3, "\\multicolumn{2}{|c|}{\\Date} & ");
    fprintf(outFp3, "\\multicolumn{2}{c|}{\\Sun}  & \\multicolumn{2}{c|}{\\Moon}  & \\multicolumn{2}{c|}{\\Mars} &");
    fprintf(outFp3, " \\multicolumn{2}{c|}{\\Mercury}  & \\multicolumn{2}{c|}{\\Jupiter} & \\multicolumn{2}{c|}{\\Venus} & ");
    fprintf(outFp3, "\\multicolumn{2}{c|}{\\Saturn}  & \\multicolumn{2}{c|}{\\Rahu}  & \\multicolumn{2}{c|}{\\Ketu} \\\\\n");
    fprintf(outFp3, "\\cline{1-2}\n");
    fprintf(outFp3, "<m>iM.</m> & <m>ma.</m> & \\multicolumn{2}{c|}{} & \\multicolumn{2}{c|}{} & \\multicolumn{2}{c|}{} & ");
    fprintf(outFp3, "\\multicolumn{2}{c|}{} & \\multicolumn{2}{c|}{}  & \\multicolumn{2}{c|}{} & ");
    fprintf(outFp3, "\\multicolumn{2}{c|}{} & \\multicolumn{2}{c|}{}  & \\multicolumn{2}{c|}{} \\\\\n");
    fprintf(outFp3, "\\hline\n");
  }

  if (outFp4) {
    fprintf(outFp4, "\\clearpage\n");
      
    fprintf(outFp4, "\\section[%s]{}\n", engMonths[curr_month - 1]);
    fprintf(outFp4, "\\begin{tabular}{|r|r|r|r|r|r|r|r|r|r|r|r|r|r|r|}\n");
    print_month_top_heading(outFp4, N_PANJ4_COLS, curr_year, curr_month, t_str);
    fprintf(outFp4, "\\hline\n");
    fprintf(outFp4, "\\multicolumn{2}{|c|}{\\Date} & ");
    fprintf(outFp4, "\\multicolumn{13}{|c|}{<m>lagnan\"n\"aLuM tiirunna samayan\"n\"aLuM</m>} \\\\\n");
    fprintf(outFp4, "\\hline\n");
    fprintf(outFp4, "<m>iM.</m> & <m>ma.</m> ");
    fprintf(outFp4, " & 1 & 2 & 3 & 4 & 5 & 6 & 7 & 8 & 9 & 10 & 11 & 12 & 13 \\\\\n");
  }
}

void CalendarCreator::printVisheshams(FILE* fp, const std::vector<Vishesham>& visheshams)
{
  if (visheshams.size() > 0) {
    fprintf(fp, "\\begin{tabular}{rp{1.2in}rp{1.2in}rp{1.2in}rp{1.2in}rp{1.2in}}\n");
    int count = 0;
    int nColumns = 10;
    std::vector<Vishesham>::const_iterator iter;
    for (iter = visheshams.begin(); iter != visheshams.end() ; ++iter) {
      if (count > 0) {
        fprintf(fp, " & ");
      }
      int m = 0;
      int d = 0;
      get_month_and_day_from_day_number(iter->day, this->year, m, d);
            
      fprintf(fp, "$%d$ & %s", d, iter->shortDescription.c_str());
      count += 2;
      if (count >= nColumns) {
        fprintf(fp, " \\\\\n");
        count = 0;
      }
    }
    if (count != 0) {
      while (count++ < nColumns) {
        fprintf(fp, " & ");
      }
      fprintf(fp, "\\\\\n");
    }
    fprintf(fp, "\\end{tabular}\n");
  }
}

void CalendarCreator::print_month_tail(int curr_year, int curr_month, const char *time_zone)
{ 
  print_common_month_tail(outFp1, N_PANJ1_COLS, curr_year, curr_month, time_zone);
  print_common_month_tail(outFp2, N_PANJ2_COLS, curr_year, curr_month, time_zone);
  print_common_month_tail(outFp3, N_PANJ3_COLS, curr_year, curr_month, time_zone);
  print_common_month_tail(outFp4, N_PANJ4_COLS, curr_year, curr_month, time_zone);

  if (alt_fp != NULL) {
    fprintf(alt_fp, "------------------------------------------------------------------------------------------------------\n");
  }


  if (outFp1) {
    std::vector<Vishesham> visheshams;

    vector<int>::iterator it;
    for (it = pradoshams.begin(); it != pradoshams.end() ; ++it) {
      visheshams.push_back(Vishesham("<m>pradooSam</m>", *it, Vishesham::INDIAN));
    }
    for (it = ekadasis.begin(); it != ekadasis.end() ; ++it) {
      visheshams.push_back(Vishesham("<m>eekaadashi</m>", *it, Vishesham::INDIAN));
    }
    for (it = shastis.begin(); it != shastis.end() ; ++it) {
      visheshams.push_back(Vishesham("<m>SaSThi</m>", *it, Vishesham::INDIAN));
    }
    std::sort(visheshams.begin(), visheshams.end());
    printVisheshams(outFp1, visheshams);
  }

}

void CalendarCreator::print_common_month_tail(FILE* fp, int nColumns, int curr_year, int curr_month, const char *time_zone)
{ 
  if (!fp) {
    return;
  }

  fprintf(fp, "\\hline\n");
  /* fprintf(fp, 
     "\\multicolumn{%d}{l}{ \\copyright~Umesh~Nair, 2004. Malayalam typesetting system \\copyright~Alex~A.J., 2004.}\n", nColumns);
  */
  fprintf(fp, "\\end{tabular}\n");
}


void CalendarCreator::print_doc_tail() const
{
  if (outFp5) {
    fprintf(outFp5, "\\end{tabular}\n");
  }
}


void CalendarCreator::print_doc_main_head(FILE* fp, const char* section_heading) const
{
  // For main panjang
  if (fp) {
    fprintf(fp, "\\clearpage\n");
    fprintf(fp, "\\chapter{%s}\n", section_heading);
    fprintf(fp, "\\begin{tabular}{rcl}\n");
    fprintf(fp, "{\\bf Place} & : & %s\\\\\n", name.c_str());
      
    fprintf(fp, "{\\bf Latitude} & : & %s\\\\\n", this->latitudeString.c_str());
      
    fprintf(fp, "{\\bf Longitude} & : & %s\\\\\n", this->longitudeString.c_str());
      
    fprintf(fp, "{\\bf Standard Time Zone} & : & %s (%4.1f hours from UT)\\\\\n", longStdTZ.c_str(), timeZone);
      
    if (dst) {
      fprintf(fp, "{\\bf Daylight Time Zone} & : & %s (%4.1f hours from UT)\\\\\n", longDstTZ.c_str(), timeZone+1.0);
    }
    fprintf(fp, "\\end{tabular}\n\n\n");
    fprintf(fp, "\\vspace{\\stretch{1}}\n\n\n");
  }
  // For additional panjang
}

void CalendarCreator::print_doc_head() const
{
  if (outFp1) {
    fprintf(outFp1,"\\vspace{\\stretch{1}}\n");
    fprintf(outFp1,"\\begin{center}\n");
    fprintf(outFp1,"\\Huge\n");
    fprintf(outFp1,"<m>keeraLapan~caaMgaM</m>\\\\\n");
    fprintf(outFp1,"\\vspace{\\stretch{1}}\n");
    fprintf(outFp1,"\\normalsize\n");
    fprintf(outFp1,"<m>tayyaaRaakkiyat</m> \\\\\n");
    fprintf(outFp1,"\\Large\n");
    fprintf(outFp1,"<m>umeeS pi. en.</m>\\\\\n");
    fprintf(outFp1,"\\Huge\n");
    fprintf(outFp1,"\\vspace{\\stretch{1}}\n");
    fprintf(outFp1,"%d\\\\\n", this->year);
    fprintf(outFp1,"\\normalsize\n");
    fprintf(outFp1,"\\vspace{\\stretch{2}}\n");
    fprintf(outFp1,"  For %s.\n", this->name.c_str());
    fprintf(outFp1,"\\end{center}\n");
    fprintf(outFp1,"\\cleardoublepage\n");
    fprintf(outFp1,"\\tableofcontents\n");
    fprintf(outFp1,"\\addtolength{\\parskip}{0.5\\baselineskip}\n");

    /* fprintf(outFp1,"\\cleardoublepage\n");
     fprintf(outFp1,"\\input{panjang_mukh}\n");*/
    fprintf(outFp1,"\\cleardoublepage\n");
    print_doc_main_head(outFp1, "<m>pan~caaMgaM</m>");
      
    fprintf(outFp1, "\\begin{tabular}{lcp{4.5in}}\n");
    fprintf(outFp1, "<m>tiiyati - iM.</m> & : & <m>iMgliiS tiiyati</m> \\\\\n");
    fprintf(outFp1, "<m>tiiyati - ma.</m> & : & <m>malayaaLaM (kollavaRSa) tiiyati</m> \\\\\n");
    fprintf(outFp1, "<m>tiiyati - sha.</m> & : & <m>shakavaRSattiiyati</m> \\\\\n");
    fprintf(outFp1, "<m>aazhca </m> & : & <m>aazhca (vaaraM)</m> \\\\\n");
    fprintf(outFp1, "<m>nakSatraM </m> & : & <m>aa divasattiluLLa raNTu nakSatran\"n\"aL - '/' -nu iruvashattumaayi.  aadyatteetilninnu raNTaamatte nakSatrattileekku kaTakkunna samayaM (maNikkuuR:miniTT) valattuvashattu koTuttirikkunnu.</m> \\\\\n");
    fprintf(outFp1, "<m>tithi </m> & : & <m>aa divasattiluLLa raNTu tithikaL - '/' -nu iruvashattumaayi.  aadyatteetilninnu raNTaamatte tithiyileekku kaTakkunna samayaM (maNikkuuR:miniTT) valattuvashattu koTuttirikkunnu.</m> \\Shuklapaksham{} <m>veLutta pakSatteyuM</m> \\Krishnapaksham{} <m>kaRutta pakSatteyuM suucippikkunnu.</m>\\\\\n");
    fprintf(outFp1, "<m>udayaastamayaM </m> & : & <m>suuRyoodayavuM (iTatt) suuRyaastamayavuM (valatt) (maNikkuuR:miniTT)</m> \\\\\n");
    fprintf(outFp1, "<m>raahukaalaM </m> & : & <m> raahukaalaM tuTan\"n\"ukayuM (iTatt) avasaanikkukayuM (valatt) ceyyunna samayan\"n\"aL (maNikkuuR:miniTT)</m> \\\\\n");
    fprintf(outFp1, "\\end{tabular}\n");
      
  }
  if (outFp2) {
    print_doc_main_head(outFp2, "<m></m>");
  }
  if (outFp3) {
    print_doc_main_head(outFp3, "<m>grahasphuTan\"n\"aL</m>");
    fprintf(outFp3, "\\begin{tabular}{lcp{4.5in}}\n");
    fprintf(outFp3, "<m>tiiyati - iM.</m> & : & <m>iMgliiS tiiyati</m> \\\\\n");
    fprintf(outFp3, "<m>tiiyati - ma.</m> & : & <m>malayaaLaM tiiyati</m> \\\\\n");
    fprintf(outFp3, "<m>aazhca </m> & : & <m>aazhca (vaaraM)</m> \\\\\n");
    fprintf(outFp3, "\\end{tabular}\n");

 
    fprintf(outFp3,"\\vspace{\\stretch{1}}\n");
    fprintf(outFp3, "\\begin{tabular}{||c|c||c|c||c|c||c|c||c|c||c|c||}\n");
    fprintf(outFp3, "\\hline\n");
    fprintf(outFp3, "\\hline\n");
    fprintf(outFp3, "\\multicolumn{12}{||c||}{<m>raashikaLuTe saMkSiptaruupan\"n\"aLuM puuRNNaruupan\"n\"aLuM</m>} \\\\\n");
    fprintf(outFp3, "\\hline\n");
    for (int i=0 ; i < 12 ; ++i) {
      fprintf(outFp3, "%s & %s", very_short_mmonths[i], mmmonths[i]);
      if (i % 6 == 5) {
        fprintf(outFp3, "\\\\\n");
      } else {
        fprintf(outFp3, " & ");
      }
    }
    fprintf(outFp3, "\\hline\n");
    fprintf(outFp3, "\\end{tabular}\\\\\n");
    fprintf(outFp3,"\\vspace{\\stretch{1}}\n");
    fprintf(outFp3,"<m>{\\bf\\em pratyeeka lipiyiluLLa sphuTan\"n\"aL} grahaM vakranaaNennu suucippikkunnu.</m>\n");
  }

  if (outFp4) {
    print_doc_main_head(outFp4, "<m>lagnan\"n\"aL</m>");
    fprintf(outFp4, "\\begin{tabular}{lcp{4.5in}}\n");
    fprintf(outFp4, "<m>tiiyati - iM.</m> & : & <m>iMgliiS tiiyati</m> \\\\\n");
    fprintf(outFp4, "<m>tiiyati - ma.</m> & : & <m>malayaaLaM tiiyati</m> \\\\\n");
    fprintf(outFp4, "\\end{tabular}\n");
    fprintf(outFp4, "<m>praadeeshika sRRaanDeeRD samayaM aRddharaatri mutal aRddharaatri vareyuLLa lagnan\"n\"aLuM ava avasaanikkunna samayavuM (maNikkuuR:miniTT) ceeRttirikkunnu.</m>\n\n");
    fprintf(outFp4,"<m>{\\bf\\em pratyeeka lipiyiluLLa  samayan\"n\"aL} udayalagnatte suucippikkunnu.</m>\n\n");
    fprintf(outFp4,"Daylight savings time <m>anuvaRttikkunna kaalayaLavil lagnasamayaman\"n\"aL</m> DST-<m>yilttanne koTuttirikkunnu.  ennaal, oru divasatte lagnasamayan\"n\"aL koTuttirikkunnat sRRaanDeeRD samayaM aRddharaatri mutal aRddharaatri vareyaaN<<+.  atukoNT aRddharaatriykkuM</m> 1 A.M.<m>-nuM iTaykku lagnaM maaRunnuNTen\"kil atu talee divasattile avasaanatte lagnamaayaavuM koTuttirikkuka.</m>\n\n");
  }

  if (outFp5) {
    int n = 11;
    fprintf(outFp5, "\\clearpage\n");
    fprintf(outFp5, "\\section[<m>yuRaanas, nepTyuuN, pluuTToo, ayanaaMshaM</m>]{}\n");
    fprintf(outFp5, "\\begin{tabular}{|l|r|lr|lr|lr|lr|r|}\n");
    fprintf(outFp5, "\\multicolumn{%d}{c}{\\Large %04d \\normalsize}\\\\\n", n, this->year);
    fprintf(outFp5, "\\multicolumn{%d}{c}{(%s / %s)}\\\\\n", n, longStdTZ.c_str(), longDstTZ.c_str());
    fprintf(outFp5, 
            "\\multicolumn{%d}{c}{<m>sthalaM</m>: %s. <m>akSaamshaM</m>: %s. <m>reekhaaMshaM:</m> %s.}\\\\\n",
            n, name.c_str(), latitudeString.c_str(), longitudeString.c_str());
    fprintf(outFp5, "\\hline\n");
    fprintf(outFp5, "\\multicolumn{4}{|c|}{<m>tiiyati</m>} & \\multicolumn{2}{c|}{<m>yuRaanas</m>}& \\multicolumn{2}{c|}{<m>nepTyuuN</m>}& \\multicolumn{2}{c|}{<m>pluuTToo</m>} & <m>ayanaaMshaM</m> \\\\\n");
    fprintf(outFp5, "\\cline{1-4}\n");
    fprintf(outFp5, "\\multicolumn{2}{|c|}{<m>iMgliiS</m>} & \\multicolumn{2}{c|}{<m>malayaaLaM</m>} & & & & & & & \\\\\n");
    fprintf(outFp5, "\\hline\n");
  }

}

void CalendarCreator::print_panjang (double j_day, int n_days, const char *time_zone, double latitude, double longitude)
{
  int prev_month = 0 ;
  double curr_j_date = j_day ;
  int curr_year, curr_month, curr_day ;
  ST_DT det ;
  ST_PL_DET pl_det[15] ;
  struct date curr_date;
  get_date(localJulianDay(curr_j_date), &curr_year, &curr_month, &curr_day);

  print_doc_head();

  // Find last day1

  double transitJDay;
  next_sun_transit(j_day - 30, &transitJDay);
  malDay = (isLeapYear(curr_year-1) ? 366 : 365) - dayNumberOfMalMonthBeginning(transitJDay) + 1;

  if (0) {
    char dummy[40];
    std::cerr << "curr_year = " << curr_year << endl;
    std::cerr << "Transit Jday = " << transitJDay << endl;
    std::cerr << "Day number = " <<  dayNumberOfMalMonthBeginning(transitJDay) << endl;
    std::cerr << "Days after = " << malDay << endl;
    std::cerr << "Date = " << date_time_string(transitJDay, dummy) << endl;
  }

  // fprintf(stderr, "n_days = %d\n", n_days);
  while (n_days--)
  {
    // Save last days star and thithi, there may be gaps and overlaps
    yesterdaysStar = todaysStar;
    yesterdaysThithi = todaysThithi;

    // fprintf(stderr, "%d\n", n_days);
    find_e_and_ayanamsa(curr_j_date) ;
    get_date(localJulianDay(curr_j_date), &curr_year, &curr_month, &curr_day);
    // fprintf(stderr, "%d/%d/%d\n", curr_year, curr_month, curr_day);

 
    if (curr_month != prev_month )
    {
      if (prev_month)
        print_month_tail (curr_year, curr_month-1, time_zone);
      print_month_head (curr_year, curr_month, time_zone, curr_j_date);
      int i;
      for (i = SUN ; i <= DESC_NODE ; ++i) {
        prev_rasi[i] = -1;
      }
      last_lagnam = -1;
    }
    full_details(curr_j_date, pl_det);
    curr_date.da_year = curr_year;
    curr_date.da_mon = curr_month;
    curr_date.da_day = curr_day;
    print_details(curr_j_date, curr_date, pl_det, latitude, longitude);

    int currDayNumber = get_year_day_of_date(curr_date);
    if (currDayNumber == dstStartDayNumber) {        
      if (outFp1) fprintf(outFp1, "\\hline\n");
      if (outFp2) fprintf(outFp2, "\\hline\n");
      if (outFp3) fprintf(outFp3, "\\hline\n");
      if (outFp4) fprintf(outFp4, "\\hline\n");
      if (outFp1) fprintf(outFp1, "\\multicolumn{%d}{|c|}{%s ends; %s starts}\\\\\n", N_PANJ1_COLS, longStdTZ.c_str(), longDstTZ.c_str());
      if (outFp2) fprintf(outFp2, "\\multicolumn{%d}{|c|}{%s ends; %s starts}\\\\\n", N_PANJ2_COLS, longStdTZ.c_str(), longDstTZ.c_str());
      if (outFp3) fprintf(outFp3, "\\multicolumn{%d}{|c|}{%s ends; %s starts}\\\\\n", N_PANJ3_COLS, longStdTZ.c_str(), longDstTZ.c_str());
      if (outFp4) fprintf(outFp4, "\\multicolumn{%d}{|c|}{%s ends; %s starts}\\\\\n", N_PANJ4_COLS, longStdTZ.c_str(), longDstTZ.c_str());
      if (outFp1) fprintf(outFp1, "\\hline\n");
      if (outFp2) fprintf(outFp2, "\\hline\n");
      if (outFp3) fprintf(outFp3, "\\hline\n");
      if (outFp4) fprintf(outFp4, "\\hline\n");
      this->dstCorrection = 1.0;
      --malDay;
      print_details(curr_j_date, curr_date, pl_det, latitude, longitude);
    } else if (currDayNumber == dstEndDayNumber) {
      if (outFp1) fprintf(outFp1, "\\hline\n");
      if (outFp2) fprintf(outFp2, "\\hline\n");
      if (outFp3) fprintf(outFp3, "\\hline\n");
      if (outFp4) fprintf(outFp4, "\\hline\n");
      if (outFp1) fprintf(outFp1, "\\multicolumn{%d}{|c|}{%s ends; %s starts}\\\\\n", N_PANJ1_COLS, longDstTZ.c_str(), longStdTZ.c_str());
      if (outFp2) fprintf(outFp2, "\\multicolumn{%d}{|c|}{%s ends; %s starts}\\\\\n", N_PANJ2_COLS, longDstTZ.c_str(), longStdTZ.c_str());
      if (outFp3) fprintf(outFp3, "\\multicolumn{%d}{|c|}{%s ends; %s starts}\\\\\n", N_PANJ3_COLS, longDstTZ.c_str(), longStdTZ.c_str());
      if (outFp4) fprintf(outFp4, "\\multicolumn{%d}{|c|}{%s ends; %s starts}\\\\\n", N_PANJ4_COLS, longDstTZ.c_str(), longStdTZ.c_str());
      if (outFp1) fprintf(outFp1, "\\hline\n");
      if (outFp2) fprintf(outFp2, "\\hline\n");
      if (outFp3) fprintf(outFp3, "\\hline\n");
      if (outFp4) fprintf(outFp4, "\\hline\n");
      this->dstCorrection = 0.0;
      --malDay;
      print_details(curr_j_date, curr_date, pl_det, latitude, longitude);
    } 
    curr_j_date += 1.0 ;
    prev_month = curr_month ;
  }

  print_month_tail (curr_year, curr_month, time_zone);

  // Print the holidays to the outFp1
  printHolidays(outFp1);
  print_doc_tail();
}

void CalendarCreator::printHolidays(FILE* fp)
{
  static int lastHolidayMonth = -1;
  if (fp) {

    std::vector<Vishesham> visheshams;
    std::vector<Vishesham> alt_visheshams;
    if (thiruvonamDay > 0) {
      visheshams.push_back(Vishesham("<m>tiruvooNaM</m>", thiruvonamDay, Vishesham::INDIAN));
      alt_visheshams.push_back(Vishesham("Thiruvonam", thiruvonamDay, Vishesham::INDIAN));
    }
    if (vishuDay > 0) {
      visheshams.push_back(Vishesham("<m>viSu</m>", vishuDay, Vishesham::INDIAN));
      alt_visheshams.push_back(Vishesham("Vishu", vishuDay, Vishesham::INDIAN));
    }
    if (durgaashtamiDay > 0) {
      visheshams.push_back(Vishesham("<m>duRggaaSTami</m>", durgaashtamiDay, Vishesham::INDIAN));
      alt_visheshams.push_back(Vishesham("Durgaashtami", durgaashtamiDay, Vishesham::INDIAN));
    }
    if (mahaanavamiDay > 0) {
      visheshams.push_back(Vishesham("<m>mahaanavami</m>", mahaanavamiDay, Vishesham::INDIAN));
      alt_visheshams.push_back(Vishesham("Mahaanavami", mahaanavamiDay, Vishesham::INDIAN));
    }
    if (vijayadasamiDay > 0) {
      visheshams.push_back(Vishesham("<m>vijayadashami</m>", vijayadasamiDay, Vishesham::INDIAN));
      alt_visheshams.push_back(Vishesham("Vjayadashami", vijayadasamiDay, Vishesham::INDIAN));
    }
    if (sivaratriDay > 0) {
      visheshams.push_back(Vishesham("<m>shivaraatri</m>", sivaratriDay, Vishesham::INDIAN));
      alt_visheshams.push_back(Vishesham("shivaraathri", sivaratriDay, Vishesham::INDIAN));
    }
    if (deepavaliDay > 0) {
      visheshams.push_back(Vishesham("<m>diipaavali</m>", deepavaliDay, Vishesham::INDIAN));
      alt_visheshams.push_back(Vishesham("Deepavali", deepavaliDay, Vishesham::INDIAN));
    }
    if (easterDay > 0) {
      visheshams.push_back(Vishesham("<m>uyiRttezhunneelppu perunnaaL</m> (Easter)", easterDay, Vishesham::WORLD));
      alt_visheshams.push_back(Vishesham("Easter", easterDay, Vishesham::WORLD));
    }
    if (ashWDay > 0) {
      visheshams.push_back(Vishesham("<m>kSaarabudhan</m> (Ash Wednesday)", ashWDay, Vishesham::WORLD));
      alt_visheshams.push_back(Vishesham("Ash Wednesday", ashWDay, Vishesham::WORLD));
    }
    if (passionSDay > 0) {
      visheshams.push_back(Vishesham("Passion Sunday", passionSDay, Vishesham::WORLD));
      alt_visheshams.push_back(Vishesham("Passion Sunday", passionSDay, Vishesham::WORLD));
    }
    if (palmSDay > 0) {
      visheshams.push_back(Vishesham("<m>kuruttoolapperunnaaL</m> (Palm Sunday)", palmSDay, Vishesham::WORLD));
      alt_visheshams.push_back(Vishesham("Palm Sunday", palmSDay, Vishesham::WORLD));
    }
    if (maundyTDay > 0) {
      visheshams.push_back(Vishesham("<m>pesahaa vyaazhaazhca</m> (Maundy Thursday)", maundyTDay, Vishesham::WORLD));
      alt_visheshams.push_back(Vishesham("Maundy Thursday", maundyTDay, Vishesham::WORLD));
    }
    if (goodFDay > 0) {
      visheshams.push_back(Vishesham("<m>duHkhaveLLiyaazhca</m> (Good Friday)", goodFDay, Vishesham::WORLD));
      alt_visheshams.push_back(Vishesham("Good Friday", goodFDay, Vishesham::WORLD));
    }
    if (regationSDay > 0) {
      visheshams.push_back(Vishesham("Regation Sunday", regationSDay, Vishesham::WORLD));
      alt_visheshams.push_back(Vishesham("Regation Sunday", regationSDay, Vishesham::WORLD));
    }
    if (ascensionDay > 0) {
      visheshams.push_back(Vishesham("Ascension Day", ascensionDay, Vishesham::WORLD));
      alt_visheshams.push_back(Vishesham("Ascension Day", ascensionDay, Vishesham::WORLD));
    }
    if (penteCostDay > 0) {
      visheshams.push_back(Vishesham("Pentecost", penteCostDay, Vishesham::WORLD));
      alt_visheshams.push_back(Vishesham("Pentecost", penteCostDay, Vishesham::WORLD));
    }
    if (trinitySDay > 0) {
      visheshams.push_back(Vishesham("Trinity Sunday", trinitySDay, Vishesham::WORLD));
      alt_visheshams.push_back(Vishesham("Trinity Sunday", trinitySDay, Vishesham::WORLD));
    }

    int dummy;
    // Fixed ones
    visheshams.push_back(Vishesham("New Year", get_day_number_in_year(year, 1, 1), Vishesham::WORLD));
    if (year >= 1950) {
      visheshams.push_back(Vishesham("<m>Rippablik dinaM</m>", get_day_number_in_year(year, 1, 26), Vishesham::INDIAN));
      alt_visheshams.push_back(Vishesham("Indian Republic Day", get_day_number_in_year(year, 1, 26), Vishesham::INDIAN));
    }
    if (year >= 1947) {
      visheshams.push_back(Vishesham("<m>svaatantryadinaM</m>", get_day_number_in_year(year, 8, 15), Vishesham::INDIAN));
      alt_visheshams.push_back(Vishesham("Indian Independence Day", get_day_number_in_year(year, 8, 15), Vishesham::INDIAN));
    }
    visheshams.push_back(Vishesham("Christmas", get_day_number_in_year(year, 12, 25), Vishesham::WORLD));
    visheshams.push_back(Vishesham("Valentines's day", get_day_number_in_year(year, 2, 14), Vishesham::WORLD));
    visheshams.push_back(Vishesham("US Independence day", get_day_number_in_year(year, 7, 4), Vishesham::AMERICAN));
    visheshams.push_back(Vishesham("Halloween", get_day_number_in_year(year, 10, 31), Vishesham::AMERICAN));
    visheshams.push_back(Vishesham("<m>gaandhi jayanti</m>", get_day_number_in_year(year, 10, 2), Vishesham::INDIAN));
    alt_visheshams.push_back(Vishesham("Christmas", get_day_number_in_year(year, 12, 25), Vishesham::WORLD));
    alt_visheshams.push_back(Vishesham("Valentines's day", get_day_number_in_year(year, 2, 14), Vishesham::WORLD));
    alt_visheshams.push_back(Vishesham("US Independence day", get_day_number_in_year(year, 7, 4), Vishesham::AMERICAN));
    alt_visheshams.push_back(Vishesham("Halloween", get_day_number_in_year(year, 10, 31), Vishesham::AMERICAN));
    alt_visheshams.push_back(Vishesham("Gandhi Jayanthi", get_day_number_in_year(year, 10, 2), Vishesham::INDIAN));
    if (year >= 1956) {
      visheshams.push_back(Vishesham("<m>keeraLappiRavi</m>", get_day_number_in_year(year, 11, 1), Vishesham::INDIAN));
      alt_visheshams.push_back(Vishesham("Keralappiravi", get_day_number_in_year(year, 11, 1), Vishesham::INDIAN));
    }

    // US Holidays
    visheshams.push_back(Vishesham("Martin Luther King's day", yearDayNumber(getMLKDay(year, dummy)), Vishesham::AMERICAN));
    visheshams.push_back(Vishesham("President's day", yearDayNumber(getPresidentsDay(year, dummy)), Vishesham::AMERICAN));
    visheshams.push_back(Vishesham("Thanksgiving day", yearDayNumber(getThanksgivingDay(year, dummy)), Vishesham::AMERICAN));
    visheshams.push_back(Vishesham("Memorial day", yearDayNumber(getMemorialDay(year, dummy)), Vishesham::AMERICAN));
    visheshams.push_back(Vishesham("Labor Day", yearDayNumber(getLaborDay(year, dummy)), Vishesham::AMERICAN));
    visheshams.push_back(Vishesham("Daylight savings starts", dstStartDayNumber, Vishesham::AMERICAN));
    visheshams.push_back(Vishesham("Daylight savings ends", dstEndDayNumber, Vishesham::AMERICAN));
    visheshams.push_back(Vishesham("Father's day", yearDayNumber(getFathersDay(year, dummy)), Vishesham::AMERICAN));
    visheshams.push_back(Vishesham("Mother's day", yearDayNumber(getMothersDay(year, dummy)), Vishesham::AMERICAN));
    visheshams.push_back(Vishesham("Columbus day", yearDayNumber(getColumbusDay(year, dummy)), Vishesham::AMERICAN));
    alt_visheshams.push_back(Vishesham("Martin Luther King's day", yearDayNumber(getMLKDay(year, dummy)), Vishesham::AMERICAN));
    alt_visheshams.push_back(Vishesham("President's day", yearDayNumber(getPresidentsDay(year, dummy)), Vishesham::AMERICAN));
    alt_visheshams.push_back(Vishesham("Thanksgiving day", yearDayNumber(getThanksgivingDay(year, dummy)), Vishesham::AMERICAN));
    alt_visheshams.push_back(Vishesham("Memorial day", yearDayNumber(getMemorialDay(year, dummy)), Vishesham::AMERICAN));
    alt_visheshams.push_back(Vishesham("Labor Day", yearDayNumber(getLaborDay(year, dummy)), Vishesham::AMERICAN));
    alt_visheshams.push_back(Vishesham("Daylight savings starts", dstStartDayNumber, Vishesham::AMERICAN));
    alt_visheshams.push_back(Vishesham("Daylight savings ends", dstEndDayNumber, Vishesham::AMERICAN));
    alt_visheshams.push_back(Vishesham("Father's day", yearDayNumber(getFathersDay(year, dummy)), Vishesham::AMERICAN));
    alt_visheshams.push_back(Vishesham("Mother's day", yearDayNumber(getMothersDay(year, dummy)), Vishesham::AMERICAN));
    alt_visheshams.push_back(Vishesham("Columbus day", yearDayNumber(getColumbusDay(year, dummy)), Vishesham::AMERICAN));

    sort(visheshams.begin(), visheshams.end());

    fprintf(fp, "\\clearpage\n");
    fprintf(fp, "\\section{%s (%d)}\n", "<m>visheeSadivasan\"n\"aL</m>", year);
    fprintf(fp, "\\begin{tabular}{|rl|l|l|l|l|}\n");
    fprintf(fp, "\\hline\n");
    fprintf(fp, "\\multicolumn{2}{|c|}{{\\bf Date}} & {\\bf Day} & {\\bf Kerala/India} & {\\bf Christian/International} & {\\bf American}\\\\\n");
    std::vector<Vishesham>::iterator it;
    for (it = visheshams.begin(); it != visheshams.end() ; ++it) {
      int m, d;
      if (get_month_and_day_from_day_number(it->day, year, m, d)) {
        if (lastHolidayMonth != m) {
          fprintf(fp, "\\hline\n");
          lastHolidayMonth = m;
        }
        const char *pattern = "%d & %s & %s & & %s  &\\\\\n";
        switch (it->type) {
          case Vishesham::INDIAN :
            pattern = "%d & %s & %s& %s & &\\\\\n";
            break;
          case Vishesham::WORLD :
            pattern = "%d & %s & %s& & %s  &\\\\\n";
            break;
          case Vishesham::AMERICAN :
            pattern = "%d & %s& %s & & & %s \\\\\n";
            break;
          default:
            pattern = "%d & %s & %s & & %s  &\\\\\n";
            break;
        }
        fprintf(fp, pattern, d, engMonths[m-1], lday_name[weekdayOfDate(year, m, d)], it->shortDescription.c_str());
      }
    }
      
    if (alt_fp != NULL) {
      fprintf(alt_fp, "\n\n\n\nHOLIDAYS\n\n\n\n");
      sort(alt_visheshams.begin(), alt_visheshams.end());
      for (it = alt_visheshams.begin(); it != alt_visheshams.end() ; ++it) {
        int m, d;
        if (get_month_and_day_from_day_number(it->day, year, m, d)) {
          fprintf(alt_fp, "%02d %-10s %-10s %-20s\n", d, mnth_name[m-1], day_name[weekdayOfDate(year, m, d)], it->shortDescription.c_str());
        }
      }
    }

    fprintf(fp, "\\hline\n");
    fprintf(fp, "\\end{tabular}\n");
  }
}


void CalendarCreator::print_details(double j_day, struct date& curr_date, ST_PL_DET pl_det[], double latitude, double longitude)
{
  int i ;
  char z_str[15] ;       
  double hind_long, d_nak, d_thithi, sun_moon_diff, moon_long, sun_long ;
  int nak, paksham, thithi ;
  char aya_str[20] ;
  END_VALUES nak_ev, thi_ev, alt_nak_ev, alt_thi_ev;
  char naks_str[50], naks_str1[50], thithi_str[50], thithi_str1[50];
  char alt_naks_str[50], alt_naks_str1[50], alt_thithi_str[50], alt_thithi_str1[50];
  char udayam_str[20], astamayam_str[20];
  double udayam, astamayam;

  int curr_day_number = get_year_day_of_date(curr_date);

  int saka_year, saka_month, saka_day;
  gregorianToSaka(curr_date.da_year, curr_date.da_mon, curr_date.da_day, saka_year, saka_month, saka_day);


  printf("j_day = %f\n", j_day);

  time_string(ayanamsa,DEGREE, FULL, aya_str) ;
  moon_long = norm_angle(pl_det[MOON].geo_long - ayanamsa) ;
  sun_long = norm_angle(pl_det[SUN].geo_long - ayanamsa);
   
  d_nak = nakshatra (moon_long);
  nak = (int) d_nak ;
  d_nak = (d_nak - nak) * 100.0 ;
  sun_moon_diff = norm_angle(moon_long - sun_long);

  d_thithi = sun_moon_diff / 12.0;
  thithi = (((int) d_thithi) + 1 ) % 30 ;

  strcpy(naks_str, "");
  strcpy(thithi_str, "");
  strcpy(naks_str1, "");
  strcpy(thithi_str1, "");

  strcpy(alt_naks_str, "");
  strcpy(alt_thithi_str, "");
  strcpy(alt_naks_str1, "");
  strcpy(alt_thithi_str1, "");

  udayam = sunrise_between(j_day, j_day + 0.5, latitude, longitude);
  astamayam = sunset_between(j_day + 0.5, j_day + 1.0, latitude, longitude);
  find_naks_in_day(j_day, &nak_ev);
  double next_udayam = sunrise_between(j_day + 1.0, j_day + 1.5, latitude, longitude);
  find_naks_in_day_gen(udayam, next_udayam, &alt_nak_ev);

  if (debugging) {
    fprintf(stderr, "Date = %d/%d/%d, udayam = %f, astamayam = %f\n", curr_date.da_year,
            curr_date.da_mon, curr_date.da_day, udayam, astamayam);
    fprintf(stderr, "Nakshathram: ");
    for (int i = 0; i < nak_ev.n_values; ++i) {
      fprintf(stderr, "%d  %d %15.4f : ", i, nak_ev.values[i].value, nak_ev.values[i].end_j_time);
    }
    fprintf(stderr, "\n");
    fprintf(stderr, "Alt Nakshathram: ");
    for (int i = 0; i < alt_nak_ev.n_values; ++i) {
      fprintf(stderr, "%d  %d %15.4f : ", i, alt_nak_ev.values[i].value, alt_nak_ev.values[i].end_j_time);
    }
    fprintf(stderr, "\n");
  }

  if (nak_ev.n_values > 0) {
    int left;
    if (udayam < nak_ev.values[0].end_j_time) {
      left = 1;
      todaysStar = nak_ev.values[0].value;
    } else {
      left = 0;
      todaysStar = (nak_ev.values[0].value+1)%27;
    }


    get_naks_string(nak_ev.values[0].value, localJulianDay(nak_ev.values[0].end_j_time), left, naks_str);
    if (nak_ev.n_values > 1) {
      get_naks_string(nak_ev.values[1].value, localJulianDay(nak_ev.values[1].end_j_time), 0, naks_str1);
    } else {
      get_naks_string(-1, -1.0, 0, naks_str1);
    }

  } else {
    get_naks_string(nak, -1.0, 1, naks_str);
  }

  printf("Found star\n");
  char nak_prefix1[5];
  char nak_prefix2[5];
  char thi_prefix1[5];
  char thi_prefix2[5];
  strcpy(nak_prefix1, " ");
  strcpy(nak_prefix2, " ");
  strcpy(thi_prefix1, " ");
  strcpy(thi_prefix2, " ");
  if (alt_fp != NULL) {
  
    if (alt_nak_ev.n_values > 0) {
      int left;
      if (udayam < alt_nak_ev.values[0].end_j_time) {
        left = 1;
        todaysStar = alt_nak_ev.values[0].value;
      } else {
        left = 0;
        todaysStar = (alt_nak_ev.values[0].value+1)%27;
      }

        if (debugging) {
          fprintf(stderr, "Debugging for alt star:\n");
          for (int i = 0; i < alt_nak_ev.n_values; ++i) {
            int nak = alt_nak_ev.values[i].value;
            fprintf(stderr, "  %d: %d (%s), %f\n", i, nak, nak_name[nak], alt_nak_ev.values[i].end_j_time);  // xxx
          }
        }

      if (alt_nak_ev.values[0].end_j_time > j_day + 1.0) {
        strcpy(nak_prefix1, "*");
      }
      get_alt_naks_string(nak_prefix1, alt_nak_ev.values[0].value, localJulianDay(alt_nak_ev.values[0].end_j_time), alt_naks_str);
      if (alt_nak_ev.n_values > 1) {
        if (alt_nak_ev.values[1].end_j_time > j_day + 1.0) {
          strcpy(nak_prefix2, "*");
        }
        get_alt_naks_string(nak_prefix2, alt_nak_ev.values[1].value, localJulianDay(alt_nak_ev.values[1].end_j_time), alt_naks_str1);
      } else {
        get_alt_naks_string(nak_prefix2, -1, -1.0, alt_naks_str1);
      }

    } else {
      get_alt_naks_string(nak_prefix1, (nak+1)%27, -1.0, alt_naks_str);
    }
  printf("Found alt star 0\n");

  }
  printf("Found alt star\n");

  find_thiths_in_day(j_day, &thi_ev);
  if (thi_ev.n_values > 0) {
    int left;
    if (udayam < thi_ev.values[0].end_j_time) {
      left = 1;
      todaysThithi = thi_ev.values[0].value;
    } else {
      left = 0;
      todaysThithi = (thi_ev.values[0].value+1)%30;
    }
    int j = 0;
    tonightsThithi = -1;
    while (j < thi_ev.n_values) {
      if (thi_ev.values[j].end_j_time > astamayam) {
        tonightsThithi = thi_ev.values[j].value;
        break;
      } else {
        ++j;
      }
    }

    if (tonightsThithi < 0) {
      tonightsThithi = (thi_ev.values[thi_ev.n_values-1].value + 1) % 30;
    }

    get_thithis_string(thi_ev.values[0].value, localJulianDay(thi_ev.values[0].end_j_time), left, thithi_str);
    if (thi_ev.n_values > 1) {
      get_thithis_string(thi_ev.values[1].value, localJulianDay(thi_ev.values[1].end_j_time), 0, thithi_str1);
    }
  } else {
    tonightsThithi = todaysThithi = (thithi+1)%30;
    get_thithis_string(thithi, -1.0, 1, thithi_str);
  }

  printf("Found thithi\n");

  if (alt_fp != NULL) {
    find_thiths_in_day_gen(udayam, next_udayam,  &alt_thi_ev);
    if (alt_thi_ev.n_values > 0) {
      int left;
      if (udayam < alt_thi_ev.values[0].end_j_time) {
        left = 1;
        todaysThithi = alt_thi_ev.values[0].value;
      } else {
        left = 0;
        todaysThithi = (alt_thi_ev.values[0].value+1)%30;
      }
      int j = 0;
      tonightsThithi = -1;
      while (j < alt_thi_ev.n_values) {
        if (alt_thi_ev.values[j].end_j_time > astamayam) {
          tonightsThithi = alt_thi_ev.values[j].value;
          break;
        } else {
          ++j;
        }
      }

      if (tonightsThithi < 0) {
        tonightsThithi = (alt_thi_ev.values[alt_thi_ev.n_values-1].value + 1) % 30;
      }

      if (alt_thi_ev.values[0].end_j_time > j_day + 1.0) {
        strcpy(thi_prefix1, "*");
      }
      get_alt_thithis_string(thi_prefix1, alt_thi_ev.values[0].value, localJulianDay(alt_thi_ev.values[0].end_j_time), alt_thithi_str);
      if (alt_thi_ev.n_values > 1) {
        if (alt_thi_ev.values[1].end_j_time > j_day + 1.0) {
          strcpy(thi_prefix2, "*");
        }
        get_alt_thithis_string(thi_prefix2, alt_thi_ev.values[1].value, localJulianDay(alt_thi_ev.values[1].end_j_time), alt_thithi_str1);
      } else {
        get_alt_thithis_string(thi_prefix2, -1, -1.0, alt_thithi_str1);
      }
    } else {
      tonightsThithi = todaysThithi = (thithi+1)%30;
      get_alt_thithis_string(thi_prefix1, todaysThithi, -1.0, alt_thithi_str);
    }
    printf("Found alt thithi 0\n");
  }

    printf("Found alt thithi\n");

  hm_string(localJulianDay(udayam), udayam_str);
  hm_string(localJulianDay(astamayam), astamayam_str);

  int d1 = get_year_day_of_date(curr_date);
  // printf("%d %d\n", d1, this->nextNewMonthDay);
  if (get_year_day_of_date(curr_date) == this->nextNewMonthDay) {
    if (outFp1) fprintf(outFp1, "\\hline\n");
    if (outFp2) fprintf(outFp2, "\\hline\n");
    if (outFp3) fprintf(outFp3, "\\hline\n");
    if (outFp4) fprintf(outFp4, "\\hline\n");
    malDay = 1;
    malMonth = (malMonth + 1 ) % 12;
  } else {
    ++malDay;
  }

  if (debugging) {
    fprintf(stderr, "Date = %d/%d/%d, Day Number = %d\n", curr_date.da_year, curr_date.da_mon, curr_date.da_day, curr_day_number);
    fprintf(stderr, "Todays Star = %d, Yesterday's star = %d, Todays's thithi = %d, Yesterday's thithi = %d\n",
            todaysStar, yesterdaysStar, todaysThithi, yesterdaysThithi);
  }

  // Check for special days
  if (malMonth == 5 /* KANNI */ && ((todaysThithi == 0) || (todaysThithi == 1))) {
    // Need to check the end of the month also
    if (malDay < 6) {
      double transit_day;
      sun_transit_between(j_day, j_day+35, &transit_day);
      int it = ithithi(transit_day);
      if (it > 7) {
        // Today is is the last amavasi
        inNavaratri = true;
      }
    } else {
      inNavaratri = true;
    }
  }

  if (todaysThithi > 10) {
    inNavaratri = false;
  }
   
  if (malMonth == 4 /* CHINGAM */ && todaysStar == 21 /* THIRUVONAM */ ) {
    thiruvonamDay = curr_day_number;
  } else if (malMonth == 4 /* CHINGAM */ && (todaysStar == 22 && yesterdaysStar == 20)) {
    fprintf(stderr, "============= %d ===================\n", curr_date.da_year);
    thiruvonamDay = curr_day_number - 1;
  }
   
  if (malMonth == 0 /* MEDAM */ && malDay == 1) {
    vishuDay = is_month_at_sunrise ? curr_day_number : curr_day_number + 1;
  }

  if (inNavaratri) {    
    if (todaysThithi == 8) {
      durgaashtamiDay = curr_day_number;
    } else if (todaysThithi == 9 && yesterdaysThithi == 7) {
      durgaashtamiDay = curr_day_number -1;
    }
      
    if (todaysThithi == 9) {
      mahaanavamiDay = curr_day_number;
    } else if (todaysThithi == 10 && yesterdaysThithi == 8) {
      mahaanavamiDay = curr_day_number -1;
    }
      
      
    if (todaysThithi == 10) {
      vijayadasamiDay = curr_day_number;
    } else if (todaysThithi == 11 && yesterdaysThithi == 9) {
      vijayadasamiDay = curr_day_number -1;
    }
  }

#if 1
  if (!lastDayWasPradosham && (tonightsThithi == 13 || tonightsThithi == 28)) {
    pradoshams.push_back(curr_day_number);
    lastDayWasPradosham = true;
  } else {
    lastDayWasPradosham = false;
  }

  if (todaysThithi == 11 || todaysThithi == 26 ) {
    ekadasis.push_back(curr_day_number);
  } else if ((todaysThithi == 12 && yesterdaysThithi == 10) || (todaysThithi == 27 && yesterdaysThithi == 25)) {
  }

  if (todaysThithi == 6 ) {
    shastis.push_back(curr_day_number);
  } else if (todaysThithi == 7 && yesterdaysThithi == 5) {
    shastis.push_back(curr_day_number-1);
  }


#endif

  if (malMonth == 10 /* KUMBHAM */ && thithi == 29) {
    sivaratriDay = curr_day_number -1;
  } 


  if (malMonth == 6 /* THULAM */ && todaysThithi == 29) {
    deepavaliDay = curr_day_number;
  } else if ((malMonth == 6) && (todaysThithi == 0 && yesterdaysThithi == 28)){
    deepavaliDay = curr_day_number -1;
  }

  if (curr_day_number == easterYearDay) {
    easterDay = curr_day_number;

  } else {
    int easterDiff = curr_day_number - easterYearDay;
    switch(easterDiff) {
      case -46 : ashWDay = curr_day_number; break;
      case -14 : passionSDay = curr_day_number; break;
      case -7 : palmSDay = curr_day_number; break;
      case -3 : maundyTDay = curr_day_number; break;
      case -2 : goodFDay = curr_day_number; break;
      case 35 : regationSDay = curr_day_number; break;
      case 39 : ascensionDay = curr_day_number; break;
      case 49 : penteCostDay = curr_day_number; break;
      case 56 : trinitySDay = curr_day_number; break;
      default: break;
    }
  }

  char rahu_str[40], alt_rahu_str[40], gulika_str[40], yamakantaka_str[40], madhyahna_str[40];
  char rb[20];
  char re[20];

  ST_DURATION kalam;

  find_rahu_kalam(udayam, astamayam, &kalam);
  duration_to_str(&kalam, rahu_str);
  alt_duration_to_str(&kalam, alt_rahu_str);
   
  find_gulika_kalam(udayam, astamayam, &kalam);
  duration_to_str(&kalam, gulika_str);
   
  find_yamakantaka_kalam(udayam, astamayam, &kalam);
  duration_to_str(&kalam, yamakantaka_str);
   
  find_madhyahnam(udayam, astamayam, &kalam);
  duration_to_str(&kalam, madhyahna_str);

  double mid_day = (udayam + astamayam) / 2.0;

  ST_DURATION abhijith;
  char abhijith_str[50];
  abhijith.beg = mid_day - 1.0 / 60.0;
  abhijith.end = mid_day + 1.0 / 60.0;
  duration_to_str(&abhijith, abhijith_str);
   

  if (outFp1) {
    fprintf (outFp1, "%2d & %2d & %2d & %s & %s &  %s & %s & %s & %s\\\\\n",
             curr_date.da_day, malDay, saka_day, lday_name[weekday(localJulianDay(j_day), 0.0)],
             naks_str, thithi_str, udayam_str, astamayam_str, rahu_str);

    if (alt_fp != NULL) {
      fprintf (alt_fp, "%2d  %2d  %-10s  %-23s  %-23s  %14s   %s/%s\n",
               curr_date.da_day, malDay, day_name[weekday(localJulianDay(j_day), 0.0)],
               alt_naks_str, alt_thithi_str, alt_rahu_str, udayam_str, astamayam_str);
      printf("Abhijith: %04d/%02d/%02d %s\n", curr_date.da_year, curr_date.da_mon, curr_date.da_day, abhijith_str);
    }

    if ((nak_ev.n_values) > 1 || (thi_ev.n_values) > 1) {
      fprintf (outFp1, "&&&& %s  &  %s &&&&\\\\\n",
               naks_str1, thithi_str1);
    }
    if (alt_fp != NULL && (alt_nak_ev.n_values > 1 || alt_thi_ev.n_values > 1)) {
      fprintf (alt_fp, "%2s  %2s  %-10s  %-23s    %-23s\n",
               "", "", "",
               alt_naks_str1, alt_thithi_str1);
    }
  }

  if (outFp2) {
    fprintf (outFp2, "%2d & %2d & %s & %s &  %s & %s & %s & %s & %s \\\\\n",
             curr_date.da_day, malDay, lday_name[weekday(localJulianDay(j_day), 0.0)],
             udayam_str, astamayam_str, madhyahna_str, rahu_str, gulika_str, yamakantaka_str );
  }

  if (outFp3) {
    char str1[50], str2[50], str3[50], str4[50], str5[50], str6[50], str7[50], str8[50], str9[50];
      
    bool b;
    int rasi;
      
    rasi = get_hind_rasi(pl_det[SUN].geo_long);
    b = (rasi == prev_rasi[SUN]);
    hind_lsphutam(pl_det[SUN], str1, true, b);
    prev_rasi[SUN] = rasi;
      
    rasi = get_hind_rasi(pl_det[MOON].geo_long);
    b = (rasi == prev_rasi[MOON]);
    hind_lsphutam(pl_det[MOON], str2, true, b);
    prev_rasi[MOON] = rasi;
      
    rasi = get_hind_rasi(pl_det[MARS].geo_long);
    b = (rasi == prev_rasi[MARS]);
    hind_lsphutam(pl_det[MARS], str3, true, b);
    prev_rasi[MARS] = rasi;
      
    rasi = get_hind_rasi(pl_det[MERCURY].geo_long);
    b = (rasi == prev_rasi[MERCURY]);
    hind_lsphutam(pl_det[MERCURY], str4, true, b);
    prev_rasi[MERCURY] = rasi;
      
    rasi = get_hind_rasi(pl_det[JUPITER].geo_long);
    b = (rasi == prev_rasi[JUPITER]);
    hind_lsphutam(pl_det[JUPITER], str5, true, b);
    prev_rasi[JUPITER] = rasi;
      
    rasi = get_hind_rasi(pl_det[VENUS].geo_long);
    b = (rasi == prev_rasi[VENUS]);
    hind_lsphutam(pl_det[VENUS], str6, true, b);
    prev_rasi[VENUS] = rasi;
      
    rasi = get_hind_rasi(pl_det[SATURN].geo_long);
    b = (rasi == prev_rasi[SATURN]);
    hind_lsphutam(pl_det[SATURN], str7, true, b);
    prev_rasi[SATURN] = rasi;
      
    rasi = get_hind_rasi(pl_det[ASC_NODE].geo_long);
    b = (rasi == prev_rasi[ASC_NODE]);
    hind_lsphutam(pl_det[ASC_NODE], str8, true, b);
    prev_rasi[ASC_NODE] = rasi;
      
    rasi = get_hind_rasi(pl_det[DESC_NODE].geo_long);
    b = (rasi == prev_rasi[DESC_NODE]);
    hind_lsphutam(pl_det[DESC_NODE], str9, true, b);
    prev_rasi[DESC_NODE] = rasi;
   

    fprintf(outFp3, "%2d & %2d & %s & %s & %s & %s & %s & %s & %s & %s & %s  \\\\\n",
            curr_date.da_day, malDay, str1, str2, str3, str4, str5, str6, str7, str8, str9);
  }

  bool bb;
  if (outFp5) {
    if (curr_date.da_day == 1 || curr_date.da_day == 16) {
      int rasi;
      char str1[50], str2[50], str3[50], aya_str[50];
         
      rasi = get_hind_rasi(pl_det[URANUS].geo_long);
      bb = (rasi == prev_rasi[URANUS]);
      hind_lsphutam(pl_det[URANUS], str1, true, bb);
      prev_rasi[URANUS] = rasi;
         
      rasi = get_hind_rasi(pl_det[NEPTUNE].geo_long);
      bb = (rasi == prev_rasi[NEPTUNE]);
      hind_lsphutam(pl_det[NEPTUNE], str2, true, bb);
      prev_rasi[NEPTUNE] = rasi;
         
      rasi = get_hind_rasi(pl_det[PLUTO].geo_long);
      bb = (rasi == prev_rasi[PLUTO]);
      hind_lsphutam(pl_det[PLUTO], str3, true, bb);
      prev_rasi[PLUTO] = rasi;
         
      ST_DMS dmtime;
      dms_time(get_ayanamsa(j_day), &dmtime);
      sprintf(aya_str, "$%02d^{\\circ}~%02d\'~%02d\"$", dmtime.degrees, dmtime.minutes, (int)dmtime.seconds);

      if (curr_date.da_day == 1) {
        fprintf(outFp5, "\\multirow{2}{1in}{%s} & %d & %s & %d & %s & %s & %s & %s \\\\\n",
                engMonths[curr_date.da_mon-1], curr_date.da_day, mmmonths[malMonth], malDay, str1, str2, str3, aya_str);
      } else {
        fprintf(outFp5, "& %d & %s & %d & %s & %s & %s & %s \\\\\n",
                curr_date.da_day, mmmonths[malMonth], malDay, str1, str2, str3, aya_str);
        fprintf(outFp5, "\\hline\n");
      }
    }
  }

  if (outFp4) {
    LagnamMatchFinder lmf(this);
    lmf.findMatches(j_day, j_day + 1.0);
    std::vector< std::pair<int, double> > matches = lmf.get_end_values();
    std::vector< std::pair<int, double> >::iterator it;
    int i = 0;
    char str[30];
    bool udayam_over = false;
    // cout << "last_lagnam = " << last_lagnam << endl;
    for (it = matches.begin(); it != matches.end(); ++it) {
      if (i > 12) {
        break;
      }
      int lag = it->first;
      double t = it->second;
      // cout << "i = " << i << ", lag = " << lag << ", last_lagnam = " << last_lagnam << endl;
      if (i == 0 ) {
        if (lag != last_lagnam) {
          int j = 0;
          int l = lag;
          fprintf(outFp4, "\\hline\n");
          fprintf(outFp4, "&");
          while (j < 13) {
            fprintf(outFp4, "& %s ", short_mmonths[l]);
            l = (l + 1) % 12;
            ++j;
          }
          fprintf(outFp4, "\\\\\n");
          fprintf(outFp4, "\\hline\n");
        } 
        last_lagnam = lag;
        fprintf(outFp4, "%2d & %2d ", curr_date.da_day, malDay);
      }
      // cout << "i = " << i << ", lag = " << lag << ", last_lagnam = " << last_lagnam << endl;
      if (!udayam_over && t > udayam) {
        udayam_over = true;
        fprintf(outFp4, "& {\\bf\\em %s }",  hm_string(localJulianDay(t), str));
      } else {
        fprintf(outFp4, "& %s ",  hm_string(localJulianDay(t), str));
      }
      ++i;
    }
    if (i < 13) {
      i = 13 - i;
      for (int j = 0; j < i ; ++j) {
        fprintf(outFp4, " & ");
      }
    }
    fprintf(outFp4, " \\\\\n");
  }
}

char* CalendarCreator::duration_to_str(const DURATION duration, char* str)
{
  char from[20], to[20];
  hm_string(localJulianDay(duration->beg), from);
  hm_string(localJulianDay(duration->end), to);
  sprintf(str, "%s & %s", from, to);
  return str;
}

char* CalendarCreator::alt_duration_to_str(const DURATION duration, char* str)
{
  char from[20], to[20];
  hm_string(localJulianDay(duration->beg), from);
  hm_string(localJulianDay(duration->end), to);
  sprintf(str, "%s-%s", from, to);
  return str;
}


void CalendarCreator::find_rahu_kalam(double udayam, double astamayam, DURATION kalam)
{
  static int i=0;
  static int rahuParts[7] = { 7, 1, 6, 4, 5, 3, 2};
  assert((udayam < astamayam) && (astamayam < (udayam + 1.0)));
  double ashtaamsam = 0.125 * (astamayam - udayam);
  int wd = weekday(localJulianDay(udayam), 0.0);
  kalam->beg = udayam + rahuParts[wd] * ashtaamsam;
  kalam->end = kalam->beg + ashtaamsam;
}

void CalendarCreator::find_yamakantaka_kalam(double udayam, double astamayam, DURATION kalam)
{
  static int i=0;
  static int rahuParts[7] = { 4, 3, 2, 1, 0, 6, 5};
  assert((udayam < astamayam) && (astamayam < (udayam + 1.0)));
  double ashtaamsam = 0.125 * (astamayam - udayam);
  int wd = weekday(localJulianDay(udayam), 0.0);
  kalam->beg = udayam + rahuParts[wd] * ashtaamsam;
  kalam->end = kalam->beg + ashtaamsam;
}

void CalendarCreator::find_gulika_kalam(double udayam, double astamayam, DURATION kalam)
{
  static int i=0;
  static int rahuParts[7] = { 6, 5, 4, 3, 2, 1, 0};
  assert((udayam < astamayam) && (astamayam < (udayam + 1.0)));
  double ashtaamsam = 0.125 * (astamayam - udayam);
  int wd = weekday(localJulianDay(udayam), 0.0);
  kalam->beg = udayam + rahuParts[wd] * ashtaamsam;
  kalam->end = kalam->beg + ashtaamsam;
}

void CalendarCreator::find_madhyahnam(double udayam, double astamayam, DURATION kalam)
{
  static int i=0;
  assert((udayam < astamayam) && (astamayam < (udayam + 1.0)));
  double ahnam = 0.2 * (astamayam - udayam);
  kalam->beg = udayam + 2 * ahnam;
  kalam->end = kalam->beg + ahnam;
}


double CalendarCreator::find_lagnam(double j_day)
{
  return get_lagnam(j_day, this->latitude, this->longitude);
}

int MatchFinder::findMatches(double start_j_day, double end_j_day)
{
  int val = matchFunction(start_j_day);
  double j_day = start_j_day;
  while (j_day < end_j_day) {
    j_day = findNextMatch(val, j_day, end_j_day);
      
    if (j_day < 0.0) {
      break;
    }
    myEndValues.push_back(std::make_pair(val, j_day));
    val = (val + 1) % getCycle();
      
  }
  return myEndValues.size();
}

double MatchFinder::findNextMatch(int val, double start_j_day, double end_j_day)
{
  double j_day = start_j_day;
  double low, high, mid;
  int new_val, mid_val;
  int next_val = ((val == getCycle()-1) ? 0 : val + 1);
  bool eod = false;

  /* Find an instance where the value is different */
  do {
    j_day += 1.0 / 24.0;
    if (j_day > end_j_day) {
      eod = true;
      break;
    }
    new_val = matchFunction(j_day);
    // printf("j_day = %lf, new_val = %d\n", j_day, new_val);
  } while (new_val == val);

  if (eod) {
    high = end_j_day;
    low = high - 1.0 / 24.0;
    // val = matchFunction(low);
    new_val = matchFunction(high);
    if (val == new_val) {
      return -1.0;
    }
  } else {
    low = j_day - 1.0 / 24.0;
    high = j_day;
  }
  if (new_val != next_val) {
    printf("val = %d, next_val = %d, new_val = %d\n", val, next_val, new_val);
    // assert(false);
  }

  /* Now, do a bi-section iteration to find the exact instance */
  while (high - low > 1.0/1440.0) {
    mid = 0.5 * (high + low);
    mid_val = matchFunction(mid);
    // printf("High = %lf, Low = %lf, Mid = %lf, Mid Val = %d\n", high, low, mid, mid_val);
    if (mid_val == val) {
      low = mid;
    } else {
      high = mid;
    }
  }
  // printf ("BORDER = %lf\n", low);
  return low;
}

int main (int argc, char *argv[] )
{

  if (argc != 2) {
    cerr << "Usage: " << argv[0] << "<placeID>" << endl;
    exit (1);
  }

  if (::getenv("CRPAN_DEBUG") == 0) {
    debugging = false;
  } else {
    debugging = true;
  }

  CalendarCreator cc("places.dat", "p1.mm", "dummy.mm", "p2.mm", "p4.mm", "p3.mm");
  string placeId(argv[1]);
  cc.createCalendar(placeId);
  return 0;
}
