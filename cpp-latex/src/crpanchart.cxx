
/* PANCHANG */

#include "ast.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cassert>
#include <sstream>
#include <map>

#define N_PANJ1_COLS 12
#define N_PANJ2_COLS 13
#define N_PANJ3_COLS 20
#define N_PANJ4_COLS 15

// using std::string;
using namespace std;

// Debug statements.
bool debugging = false;

// Show photos and slokams.
static const bool with_picture_slokam = true;

// For Boney.
static const bool forBoney = false;

// For Sandhya (Holidays)
static const bool forSandhya = false;

// Will be assigned from places.dat
static bool forUSA = false;

static const bool showMalayalam = true;

static const bool showSaka = true;
static const bool showMalayalamDigits = false;
static const bool showHindiDigits = false;
static const bool showNazhikas = false;

static const int yearSize = 60;
static const int gregDateSize = 45;
static const int malDateSize = 12;
static const int sakaDateSize = 12;
static const int engHeadingSize1 = 20;
static const int engHeadingSize2 = 30;
static const int malHeadingSize = 20;
static const int sakaHeadingSize = 20;
static const int starThithiSize = 12;
static const int holidaySize = 11;
static const double sunRahuSize = 7.1;
static const int weekNameSize = 20;

const double x_scale = 5.0;
const double y_scale = 7.0;
const double max_x = 7 * x_scale;
const double max_y = 5 * y_scale;
const double min_x = 0.0;
const double min_y = 0.0;

const char* KERALA_HOLIDAY_COLOR = "Black";
bool nextDayIsVishu = false;

const char* engMonthColor = "HeadingEMonthColor";
const char* yearColor = "HeadingYearColor";
const char* mEngMonthColor = "HeadingMMonthColor";


const char* lday_name[] = {
  "\u0D1E\u0D3E\u0D2F\u0D7C", // Njaayar
  "\u0D24\u0D3F\u0D19\u0D4D\u0D15\u0D7E",
  "\u0D1A\u0D4A\u0D35\u0D4D\u0D35",
  "\u0D2C\u0D41\u0D27\u0D7B",
  "\u0D35\u0D4D\u0D2F\u0D3E\u0D34\u0D02",
  "\u0D35\u0D46\u0D33\u0D4D\u0D33\u0D3F",
  "\u0D36\u0D28\u0D3F" // Sani
};

static const char* mmonths[] = {
  "\u0D2E\u0D47\u0D1F", // Meta
  "\u0D07\u0D1F\u0D35", // Itava
  "\u0D2E\u0D3F\u0D25\u0D41\u0D28",
  "\u0D15\u0D7C\u0D15\u0D4D\u0D15\u0D1F\u0D15",
  "\u0D1A\u0D3F\u0D19\u0D4D\u0D19",
  "\u0D15\u0D28\u0D4D\u0D28\u0D3F",
  "\u0D24\u0D41\u0D32\u0D3E",
  "\u0D35\u0D43\u0D36\u0D4D\u0D1A\u0D3F\u0D15",
  "\u0D27\u0D28\u0D41",
  "\u0D2E\u0D15\u0D30",
  "\u0D15\u0D41\u0D02\u0D2D",
  "\u0D2E\u0D40\u0D28", // Meena
};

static const char* mmmonths[] = {
  "\u0D2E\u0D47\u0D1F\u0D02", // Meta
  "\u0D07\u0D1F\u0D35\u0D02", // Itava
  "\u0D2E\u0D3F\u0D25\u0D41\u0D28\u0D02",
  "\u0D15\u0D7C\u0D15\u0D4D\u0D15\u0D1F\u0D15\u0D02",
  "\u0D1A\u0D3F\u0D19\u0D4D\u0D19\u0D02",
  "\u0D15\u0D28\u0D4D\u0D28\u0D3F",
  "\u0D24\u0D41\u0D32\u0D3E\u0D02",
  "\u0D35\u0D43\u0D36\u0D4D\u0D1A\u0D3F\u0D15\u0D02",
  "\u0D27\u0D28\u0D41",
  "\u0D2E\u0D15\u0D30\u0D02",
  "\u0D15\u0D41\u0D02\u0D2D\u0D02",
  "\u0D2E\u0D40\u0D28\u0D02", // Meena
};



static const char* msmonths[] = {
  "ചൈത്രം",
  "വൈശാഖം",
  "ജ്യേഷ്ഠം",
  "ആഷാഢം",
  "ശ്രാവണം",
  "ഭാദ്രപാദം",
  "ആശ്വിനം",
  "കാർത്തികം",
  "അഗ്രഹായനം",
  "പൌഷം",
  "മാഘം",
  "ഫാൽഗുനം"
};

static const char* hsmonths[] = {
  "चैत्र",
  "वैशाख",
  "जयेष्ठ",
  "आषाढ़",
  "श्रावण",
  "भाद्रपद",
  "आश्विन",
  "कार्तिक",
  "अग्रहायण",
  "पौष",
  "माघ",
  "फाल्गुन"
};

static const char* short_mmonths[] = {
  "\u0D2E\u0D47\u0D1F\u0D02", // Metam
  "\u0D07\u0D1F\u0D35\u0D02", // Itavam
  "\u0D2E\u0D3F\u0D25\u0D41",
  "\u0D15\u0D7C\u0D15\u0D4D\u0D15.",
  "\u0D1A\u0D3F\u0D19\u0D4D\u0D19\u0D02",
  "\u0D15\u0D28\u0D4D\u0D28\u0D3F",
  "\u0D24\u0D41\u0D32\u0D3E\u0D02",
  "\u0D35\u0D43\u0D36\u0D4D\u0D1A\u0D3F.",
  "\u0D27\u0D28\u0D41",
  "\u0D2E\u0D15\u0D30\u0D02",
  "\u0D15\u0D41\u0D02\u0D2D\u0D02",
  "\u0D2E\u0D40\u0D28\u0D02" // Meenam
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
  "JANUARY",
  "FEBRUARY",
  "MARCH",
  "APRIL",
  "MAY",
  "JUNE",
  "JULY",
  "AUGUST",
  "SEPTEMBER",
  "OCTOBER",
  "NOVEMBER",
  "DECEMBER"
};

static const char* mEngMonths[] = {
  "ജനുവരി",
  "ഫെബ്രുവരി",
  "മാർച്ച്",
  "ഏപ്രിൽ",
  "മെയ്",
  "ജൂൺ",
  "ജൂലൈ",
  "ഓഗസ്റ്റ്",
  "സെപ്റ്റംബർ",
  "ഒക്റ്റോബർ",
  "നവംബർ",
  "ഡിസംബർ"
};


static const char* mdigs[] = {
  "\u0d66",
  "\u0d67",
  "\u0d68",
  "\u0d69",
  "\u0d6A",
  "\u0d6B",
  "\u0d6C",
  "\u0d6D",
  "\u0d6E",
  "\u0d6F"
};


static const char* ddigs[] = {
  "\u0966",
  "\u0967",
  "\u0968",
  "\u0969",
  "\u096A",
  "\u096B",
  "\u096C",
  "\u096D",
  "\u096E",
  "\u096F"
};

static string mal_digits(int day) {
  char digits[4];
  sprintf(digits, "%d", day);
  if (showMalayalamDigits) {
    string s("{\\MalDayFont \\textbf{");
    int i = 0;
    while (digits[i] != 0) {
      int dig_value = digits[i] - '0';
      if (dig_value < 0 || dig_value > 9) {
        s += digits[i];
      } else {
        s += mdigs[dig_value];
      }
      ++i;
    }
    s+= "}";
    return s;
  } else {
    string s("{\\MalDayEngFont \\textbf{");
    s += digits;
    s += "}}";
    return s;
  }
}

static string dn_digits(int day) {
  char digits[4];
  sprintf(digits, "%d", day);
  if (showHindiDigits) {
    string s ("\\textbf{");
    int i = 0;
    while (digits[i] != 0) {
      int dig_value = digits[i] - '0';
      if (dig_value < 0 || dig_value > 9) {
        s += digits[i];
      } else {
        s += ddigs[dig_value];
        s += "}";
      }
      ++i;
    }
    return s;
  } else {
    string s("{\\SakaDayEngFont \\textbf{");
    s += digits;
    s += "}}";
    return s;
  }
}

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
  sprintf(any_str,"%2d:%02d%c", h, m, am_pm);
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


string star_string(int nak) {
  assert(nak < 27);
  string s = "\\Star";
  s += ((nak < 26) ? ('A' + nak) : ('a' + nak-26));
  return s;
}

string thithi_string(int thithi) {
  assert(thithi < 30);
  string s = "\\Thithi";
  s += ((thithi < 26) ? ('A' + thithi) : ('a' + thithi-26));
  return s;
}

string end_time_string(double end_j_day) {
  char temp[10];
  hm_string(end_j_day, temp);
  return string(temp);
}

static char* get_naks_string(int nak, double j_day, int left, char* naks_str)
{
  char str[20];
  char na_name[15];
  int nak1;
  if (nak < 0) {
    strcpy(naks_str, " & ");
  } else if (j_day < 0.0) {
    assert (nak < 27);
    sprintf(na_name, "\\Star%c", ((nak < 26) ? 'A' + nak : 'a' + nak-26));
    strcpy(naks_str, "{\\bf\\em ");
    strcat(naks_str, na_name);
    strcat(naks_str, "}");
    strcat(naks_str, "&");
  } else {
    assert (nak < 27);
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


typedef std::map<int, vector<string> > HolidayMap;

class CalendarCreator {
 public:
  CalendarCreator(const string& dataFileName, const string& outFileName);

  bool createCalendar(const string& PlaceId);
  void print_calendar (double j_day, int n_days, const char *time_zone, double latitude, double longitude);
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
  void print_holidays_during(int start, int end, int wd);
  int  dayNumberOfMalMonthBeginning(double transitJDay);
  void printHolidays(FILE* fp);
  void find_rahu_kalam(double udayam, double astamayam, DURATION rahuKalam);
  void find_gulika_kalam(double udayam, double astamayam, DURATION gulikaKalam);
  void find_yamakantaka_kalam(double udayam, double astamayam, DURATION yamakantakaKalam);
  void find_madhyahnam(double udayam, double astamayam, DURATION kalam);
  char* duration_to_str(const DURATION duration, char* str);
  void printVisheshams(FILE* fp, const std::vector<Vishesham>& visheshams);
  void addHoliday(HolidayMap& holidayMap, int day_in_year, string name);
  void addFixedHoliday(HolidayMap& holidayMap, int year, int month, int day, string name);
  void add_fixed_holidays (int year);
  void addUSTaxDay(int year);
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
  string outFile;
  int nextNewMonthDay;
  int malDay;
  int malMonth;
  double dstCorrection;
  FILE* outFp; 
  FILE* outFp1; 
  FILE* outFp2; 
  FILE* outFp3; 
  FILE* outFp4; 
  FILE* outFp5; 
  int row;
  int col;
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
  HolidayMap indian_holidays;
  HolidayMap foreign_holidays;

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

void CalendarCreator::addFixedHoliday(HolidayMap& holidayMap, int year, int month, int day, string name) {
  struct date d;
  d.da_year = year;
  d.da_mon = month;
  d.da_day = day;
  int curr_day_number = get_year_day_of_date(d);
  addHoliday(holidayMap, curr_day_number, name);
}




void CalendarCreator::addHoliday(HolidayMap& holidayMap, int day_in_year, string name)
{
  cout << "Adding " << name << " (" << day_in_year << ") to " << (holidayMap == indian_holidays ? "Indian" : "Foreign") << endl;
  HolidayMap::iterator it = holidayMap.find(day_in_year);
  if (it == holidayMap.end()) {
    vector<string> v;
    v.push_back(name);
    holidayMap.insert(std::pair<int,vector<string> >(day_in_year, v));
  } else {
    it->second.push_back(name);
  }
}

CalendarCreator::CalendarCreator(const string& dataFileName, 
                                const string& outFileName)
    : dataFile(dataFileName), 
      outFile(outFileName), 
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
      outFp(0), 
      outFp1(0), 
      outFp2(0), 
      outFp3(0), 
      outFp4(0), 
      outFp5(0), 
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
  addHoliday(indian_holidays, easterYearDay, "ഈസ്റ്റർ");
  addHoliday(indian_holidays, easterYearDay-2, "ദുഃഖവെള്ളിയാഴ്ച");
  addHoliday(indian_holidays, easterYearDay-3, "പെസഹാ വ്യാഴാഴ്ച");
  addHoliday(indian_holidays, easterYearDay-7, "ഓശാന ഞായറാഴ്ച");

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
        if (name.find(", USA") != string::npos) {
          forUSA = true;
        }
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
        std::cout << "Invalid var = " << var << std::endl;
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

  if ((outFp = fopen(outFile.c_str(),"w")) == (FILE *) 0 )
  {
    cerr << "Cannot open " << outFile << endl;
    return false;
  }


  this->print_calendar(startJulianDay, nDays, shortStdTZ.c_str(), latitude, longitude) ;
  if (outFp) fclose (outFp) ;
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
  fprintf(fp, "\\multicolumn{%d}{c}{\\Large %s %04d \\normalsize}\\\\\n", nColumns, engMonths[curr_month - 1], curr_year);

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
  if (mal_month2 == 4) {
    fprintf(fp, "\\multicolumn{%d}{c}{{\\bf <m>kollavaRSaM</m>} %d %s{} - %d %s, %s%s : %s}\\\\\n", nColumns, mal_year-1, mmmonths[mal_month1], 
            mal_year, mmmonths[mal_month2], mmonths[mal_month2], "<m>saMkramaM</m>", samkrama_str);
  } else {
    fprintf(fp, "\\multicolumn{%d}{c}{{\\bf <m>kollavaRSaM</m>} %d %s{} - %s, %s%s : %s}\\\\\n", nColumns, mal_year, mmmonths[mal_month1], 
            mmmonths[mal_month2], mmonths[mal_month2], "<m>saMkramaM</m>", samkrama_str);
  }
  malMonth = mal_month1;

  // Print saka month information
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
}

void CalendarCreator::print_month_head (int curr_year, int curr_month, const char *time_zone, double j_day)
{

  double transit_j_day;
  char t_str[50];
  next_sun_transit(j_day, &transit_j_day);
  nextNewMonthDay = dayNumberOfMalMonthBeginning(transit_j_day);
  disp_time_string(localJulianDay(transit_j_day), t_str);


  if (outFp) {
    fprintf(outFp, "\\clearpage\n");
    fprintf(outFp, "\\begin{center}\n");
    if (with_picture_slokam) {
      fprintf(outFp, "\\includegraphics[width=0.9\\textwidth]{%02d.jpg}\n", curr_month);
      fprintf(outFp, "{\\color{teal}\n");
      fprintf(outFp, "\\begin{tabular}{p{1in}p{5in}p{1in}}\n");
      fprintf(outFp, "& \\input{slokam-%02d} & \n", curr_month);
      fprintf(outFp, "\\end{tabular}}\n");
    }
    fprintf(outFp, "\\begin{tikzpicture}[scale=0.8]\n");
    fprintf(outFp, "\\node at (%6.1f, %6.1f) {{\\fontsize{%d}{0}\\selectfont{\\color{%s}\\YearFont \\textbf{%04d}}}};\n",
            0.5 * (min_x + max_x), max_y + y_scale,  yearSize, yearColor, curr_year);
    fprintf(outFp, "\\node at (%6.1f, %6.1f) {{\\fontsize{%d}{0}\\selectfont{\\color{%s}%s}/}{\\fontsize{%d}{0}\\selectfont{\\color{%s}\\MalFont %s}}};\n",
            0.5 * (min_x + max_x), max_y + 0.7 * y_scale, engHeadingSize1, engMonthColor,  engMonths[curr_month-1], engHeadingSize2, mEngMonthColor, mEngMonths[curr_month-1]);


    char mal_month_info[150];
    char saka_month_info[150];
    char saka_year_info[150];

    int mal_year, mal_month1, mal_month2;

    if (showMalayalam) {
      if (curr_month < 8) {
        mal_year = curr_year - 825;
      } else {
        mal_year = curr_year - 824;
      }
      mal_month1 = (curr_month + 7) % 12;
      mal_month2 = (mal_month1 + 1 ) % 12;

      char mal_year_str[50];
      char left_month_name[100];
      char right_month_name[100];

      if (mal_month2 == 4 /* CHINGAM */) {
        sprintf(mal_year_str, "%s - %s", mal_digits(mal_year -1).c_str(), mal_digits(mal_year).c_str());
      } else {
        sprintf(mal_year_str, "%s", mal_digits(mal_year).c_str());
      }

      sprintf(left_month_name, "%s", mmmonths[mal_month1]);
      sprintf(right_month_name, "%s", mmmonths[mal_month2]);


      fprintf(outFp, "\\node at (%6.1f, %6.1f) [anchor=west] {{\\fontsize{%d}{0}\\selectfont{\\color{MalDateColor}%s}}};\n",
              min_x, max_y + y_scale, malHeadingSize + 2, mal_year_str);

      fprintf(outFp, "\\node at (%6.1f, %6.1f) [anchor=west] {{\\fontsize{%d}{0}\\selectfont{\\color{MalDateColor} {\\MalFont %s - %s}}}};\n",
              min_x, max_y + 0.85 * y_scale, malHeadingSize,left_month_name, right_month_name);

    }


    if (showSaka) {
      // Print saka month information
      int saka_year1 = curr_year - 79;
      int saka_year2 = saka_year1 + 1;
      int saka_month1 = (curr_month + 8) % 12 + 1;
      int saka_month2 = saka_month1 % 12 + 1;
      if (showHindiDigits) {
        if (curr_month == 3) {
          sprintf(saka_year_info, "{\\HindiFont %s{} - %s}", dn_digits(saka_year1).c_str(),
                  dn_digits(saka_year2).c_str());
        } else if (curr_month < 3) {
          sprintf(saka_year_info, "{\\HindiFont %s}", dn_digits(saka_year1).c_str());
        } else {
          sprintf(saka_year_info, "{\\HindiFont %s}", dn_digits(saka_year2).c_str());
        }
        sprintf(saka_month_info, "{\\HindiFont %s{} - %s}", hsmonths[saka_month1-1], hsmonths[saka_month2-1]);
      } else {
        if (curr_month == 3) {
          sprintf(saka_year_info, "%s{} - %s", dn_digits(saka_year1).c_str(),
                  dn_digits(saka_year2).c_str());
        } else if (curr_month < 3) {
          sprintf(saka_year_info, "%s", dn_digits(saka_year1).c_str());
        } else {
          sprintf(saka_year_info, "%s", dn_digits(saka_year2).c_str());
        }
        sprintf(saka_month_info, "{\\MalFont %s{} - %s}", msmonths[saka_month1-1], msmonths[saka_month2-1]);
      }

      // fprintf(outFp, "\\node at (%6.1f, %6.1f) {{\\fontsize{%d}{0}\\selectfont{\\color{ForestGreen} {\\SakaDayFont %s}}}};\n",
      //         0.5 * (min_x + max_x), max_y + 0.6 * y_scale, sakaHeadingSize, saka_month_info);
      fprintf(outFp, "\\node at (%6.1f, %6.1f) [anchor=east] {{\\fontsize{%d}{0}\\selectfont{\\color{SakaDateColor} %s}}};\n",
              max_x, max_y + y_scale, sakaHeadingSize + 2, saka_year_info);
      fprintf(outFp, "\\node at (%6.1f, %6.1f) [anchor=east] {{\\fontsize{%d}{0}\\selectfont{\\color{SakaDateColor} {%s}}}};\n",
              max_x, max_y + y_scale * 0.85, sakaHeadingSize, saka_month_info);
      
    }

    int r, c;
    for (r = 0; r <= 5; ++r) {
      double y = y_scale * r;
      fprintf(outFp, "\\draw (%6.2f, %6.2f) -- (%6.2f, %6.2f);\n",
              min_x, y, max_x, y);
    }
    for (c = 0; c <= 7; ++c) {
      double x = x_scale * c;
      fprintf(outFp, "\\draw (%6.2f, %6.2f) -- (%6.2f, %6.2f);\n",
              x, min_y, x, max_y);
    }

    fprintf(outFp, "\\fill [WeekdayHeadBackgroundColor] (%6.2f, %6.2f) rectangle (%6.2f, %6.2f);\n", min_x, max_y,  max_x, max_y + y_scale * 0.4);

    double week_y = 5.15 * y_scale;

    fprintf(outFp, "\\node at (%6.2f, %6.2f) {{\\fontsize{%d}{0}\\selectfont{\\color{WeekdayHeadEngColor}\\EngWeekNameFont SUNDAY}}};\n", x_scale * 0.5, week_y, weekNameSize / 2);
    fprintf(outFp, "\\node at (%6.2f, %6.2f) {{\\fontsize{%d}{0}\\selectfont{\\color{WeekdayHeadEngColor}\\EngWeekNameFont MONDAY}}};\n", x_scale * 1.5, week_y, weekNameSize / 2);
    fprintf(outFp, "\\node at (%6.2f, %6.2f) {{\\fontsize{%d}{0}\\selectfont{\\color{WeekdayHeadEngColor}\\EngWeekNameFont TUESDAY}}};\n", x_scale * 2.5, week_y, weekNameSize / 2);
    fprintf(outFp, "\\node at (%6.2f, %6.2f) {{\\fontsize{%d}{0}\\selectfont{\\color{WeekdayHeadEngColor}\\EngWeekNameFont WEDNESDAY}}};\n", x_scale * 3.5, week_y, weekNameSize / 2);
    fprintf(outFp, "\\node at (%6.2f, %6.2f) {{\\fontsize{%d}{0}\\selectfont{\\color{WeekdayHeadEngColor}\\EngWeekNameFont THURSDAY}}};\n", x_scale * 4.5, week_y, weekNameSize / 2);
    fprintf(outFp, "\\node at (%6.2f, %6.2f) {{\\fontsize{%d}{0}\\selectfont{\\color{WeekdayHeadEngColor}\\EngWeekNameFont FRIDAY}}};\n", x_scale * 5.5, week_y, weekNameSize / 2);
    fprintf(outFp, "\\node at (%6.2f, %6.2f) {{\\fontsize{%d}{0}\\selectfont{\\color{WeekdayHeadEngColor}\\EngWeekNameFont SATURDAY}}};\n", x_scale * 6.5, week_y, weekNameSize / 2);

    week_y += 1;
    fprintf(outFp, "\\node at (%6.2f, %6.2f) {{\\fontsize{%d}{0}\\selectfont{\\color{WeekdayHeadMalColor}\\MalWeekNameFont ഞായർ}}};\n", x_scale * 0.5, week_y, weekNameSize);
    fprintf(outFp, "\\node at (%6.2f, %6.2f) {{\\fontsize{%d}{0}\\selectfont{\\color{WeekdayHeadMalColor}\\MalWeekNameFont തിങ്കൾ}}};\n", x_scale * 1.5, week_y, weekNameSize);
    fprintf(outFp, "\\node at (%6.2f, %6.2f) {{\\fontsize{%d}{0}\\selectfont{\\color{WeekdayHeadMalColor}\\MalWeekNameFont ചൊവ്വ}}};\n", x_scale * 2.5, week_y - 0.1, weekNameSize);
    fprintf(outFp, "\\node at (%6.2f, %6.2f) {{\\fontsize{%d}{0}\\selectfont{\\color{WeekdayHeadMalColor}\\MalWeekNameFont ബുധൻ}}};\n", x_scale * 3.5, week_y - 0.1, weekNameSize);
    fprintf(outFp, "\\node at (%6.2f, %6.2f) {{\\fontsize{%d}{0}\\selectfont{\\color{WeekdayHeadMalColor}\\MalWeekNameFont വ്യാഴം}}};\n", x_scale * 4.5, week_y - 0.1, weekNameSize);
    fprintf(outFp, "\\node at (%6.2f, %6.2f) {{\\fontsize{%d}{0}\\selectfont{\\color{WeekdayHeadMalColor}\\MalWeekNameFont വെള്ളി}}};\n", x_scale * 5.5, week_y, weekNameSize);
    fprintf(outFp, "\\node at (%6.2f, %6.2f) {{\\fontsize{%d}{0}\\selectfont{\\color{WeekdayHeadMalColor}\\MalWeekNameFont ശനി}}};\n", x_scale * 6.5, week_y, weekNameSize);

  }

  // Panjangam 1
  if (outFp1) {
    fprintf(outFp1, "\\clearpage\n");
    fprintf(outFp1, "\\section[%s]{}\n", engMonths[curr_month - 1]);
    fprintf(outFp1, "\\begin{tabular}{|r|r|r|l|l r|l r|r r|r r|}\n");
      
    print_month_top_heading(outFp1, N_PANJ1_COLS, curr_year, curr_month, t_str);
      
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

  if (outFp) {

    string bottom_line("All times are in ");

    if (dst) {
      if (curr_month == std2dstMonth) {
        bottom_line += longDstTZ;
        bottom_line += " from ";
        bottom_line += dstStart;
        bottom_line += "; ";
        bottom_line += longStdTZ;
        bottom_line += " before that";
      } else if  (curr_month == dst2stdMonth){
        bottom_line += longStdTZ;
        bottom_line += " from ";
        bottom_line += dstEnd;
        bottom_line += "; ";
        bottom_line += longDstTZ;
        bottom_line += " before that";
      } else {
        if (northernHemisphere) {
          if (curr_month > std2dstMonth && curr_month < dst2stdMonth) {
            // Daylight Savings time
            bottom_line += longDstTZ;
           } else {
            // Standard time
            bottom_line += longStdTZ;
          }
        } else {
          // Southern hemisphere.
          if (curr_month > dst2stdMonth && curr_month < std2dstMonth) {
            // Standard time
            bottom_line += longStdTZ;
          } else {
            // Daylight Savings time
            bottom_line += longDstTZ;
          }
        }
      }
    } else {
      // Standard time
      bottom_line += longStdTZ;
    }

    bottom_line += ". ";
    bottom_line += name;
    // bottom_line += " (";
    // bottom_line += latitudeString;
    // bottom_line += ", ";
    // bottom_line += longitudeString;
    // bottom_line += ").";
    bottom_line += ". Prepared by \\textsf{Umesh P. Narendran}. Photos by \\textsf{Manjith Kainickara}.";


    fprintf(outFp, "\\node at (%6.2f, %6.2f) {%s};", 0.5 * (min_x + max_x), -1.0, bottom_line.c_str());
    fprintf(outFp, "\\end{tikzpicture}\n");
    fprintf(outFp, "\\end{center}\n");
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


void CalendarCreator::addUSTaxDay(int year) {
  int wd = weekdayOfDate(year, 4, 15);
  int d = 15;
  if (wd == 5 || wd == 6) {
    d = 18;
  } else if (wd == 0) {
    d = 17;
  }
  addFixedHoliday(foreign_holidays, year, 4, d, "US Tax day");
}

void CalendarCreator::add_fixed_holidays (int year) {
  int dummy = 0;
  addFixedHoliday(foreign_holidays, year, 1, 1, "New Year");
  addFixedHoliday(indian_holidays, year, 1, 26, "റിപ്പബ്ലിക് ദിനം");
  addFixedHoliday(indian_holidays, year, 8, 15, "സ്വാതന്ത്ര്യദിനം");
  addFixedHoliday(foreign_holidays, year, 2, 14, "Valentine's day");
  addFixedHoliday(foreign_holidays, year, 10, 31, "Halloween");
  addFixedHoliday(indian_holidays, year, 10, 2, "ഗാന്ധിജയന്തി");
  addFixedHoliday(indian_holidays, year, 11, 1, "കേരളപ്പിറവി");
  addFixedHoliday(indian_holidays, year, 12, 25, "ക്രിസ്തുമസ്");
  if (dst) {
    addHoliday(foreign_holidays, dstStartDayNumber, "DST starts");
    addHoliday(foreign_holidays, dstEndDayNumber, "DST ends");
  }

  if (forUSA) {
    addFixedHoliday(foreign_holidays, year, 7, 4, "US Independence day");
    addFixedHoliday(foreign_holidays, year, 9, 11, "Patriot day");
    addHoliday(foreign_holidays, yearDayNumber(getMLKDay(year, dummy)), "MLK day");
    addHoliday(foreign_holidays, yearDayNumber(getPresidentsDay(year, dummy)), "Presidents' day");
    addHoliday(foreign_holidays, yearDayNumber(getThanksgivingDay(year, dummy)), "Thanksgiving day");
    addHoliday(foreign_holidays, yearDayNumber(getMemorialDay(year, dummy)), "Memorial day");
    addHoliday(foreign_holidays, yearDayNumber(getLaborDay(year, dummy)), "Labor day");
    addHoliday(foreign_holidays, yearDayNumber(getMothersDay(year, dummy)), "Mother's day");
    addHoliday(foreign_holidays, yearDayNumber(getFathersDay(year, dummy)), "Father's day");
    addHoliday(foreign_holidays, yearDayNumber(getColumbusDay(year, dummy)), "Columbus day");
    addFixedHoliday(foreign_holidays, year, 11, 11, "Veterans day");
    addUSTaxDay(year);
  }


  if (latitude > 0.0) {
    addFixedHoliday(foreign_holidays, 2018, 3, 20, "Spring starts");
    addFixedHoliday(foreign_holidays, 2018, 6, 21, "Summer starts");
    addFixedHoliday(foreign_holidays, 2018, 9, 23, "Fall starts");
    addFixedHoliday(foreign_holidays, 2018, 12, 21, "Winter starts");
  } else {
    addFixedHoliday(foreign_holidays, 2018, 3, 20, "Fall starts");
    addFixedHoliday(foreign_holidays, 2018, 6, 21, "Winter starts");
    addFixedHoliday(foreign_holidays, 2018, 9, 23, "Spring starts");
    addFixedHoliday(foreign_holidays, 2018, 12, 21, "Summer starts");
  }

  if (forBoney) {
    addFixedHoliday(indian_holidays, year, 3, 22, "റമദാൻ ആരംഭം");
    addFixedHoliday(indian_holidays, year, 3, 8, "ഹോളി");
    addFixedHoliday(indian_holidays, year, 4, 22, "ഈദ് അൽ ഫിത്തർ");
    addFixedHoliday(indian_holidays, year, 6, 29, "ബക്രീഡ്");
  }

  if (forSandhya) {
    addFixedHoliday(foreign_holidays, 2021, 11, 14, "Vasudevan Birthday");
    addFixedHoliday(indian_holidays, 2021, 11, 3, "Vasudevan Pirannal");
    addFixedHoliday(foreign_holidays, 2021, 11, 30, "Sandhya Birthday");
    addFixedHoliday(indian_holidays, 2021, 12, 12, "Sandhya Pirannal");
    addFixedHoliday(foreign_holidays, 2021, 11, 9, "Megha Birthday");
    // addFixedHoliday(indian_holidays, 2021, 10, 20, "Megha Pirannal");
    addFixedHoliday(indian_holidays, 2021, 10, 21, "Megha Pirannal");
    addFixedHoliday(foreign_holidays, 2021, 9, 30, "Varsha Birthday");
    addFixedHoliday(indian_holidays, 2021, 9, 30, "Varsha Pirannal");
    addFixedHoliday(foreign_holidays, 2021, 9, 13, "Wedding Anniversary");
  }
}

void CalendarCreator::print_calendar (double j_day, int n_days, const char *time_zone, double latitude, double longitude)
{
  int prev_month = 0 ;
  double curr_j_date = j_day ;
  int curr_year, curr_month, curr_day ;
  ST_DT det ;
  ST_PL_DET pl_det[15] ;
  struct date curr_date;
  get_date(localJulianDay(curr_j_date), &curr_year, &curr_month, &curr_day);

  print_doc_head();

  add_fixed_holidays(curr_year);
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
  malMonth = 8;
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
      if (prev_month > 0) {
        // Print holidays from last month.


        int start = get_day_number_in_year(curr_year, prev_month, 1);
        int end = get_day_number_in_year(curr_year, curr_month, 1) - 1;
        int wd = weekdayOfDate(curr_year, prev_month, 1);

        print_holidays_during(start, end, wd);

        print_month_tail (curr_year, curr_month-1, time_zone);
      }
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
    // print_details(curr_j_date, curr_date, pl_det, latitude, longitude);

    int currDayNumber = get_year_day_of_date(curr_date);
    if (currDayNumber == dstStartDayNumber) {        
      this->dstCorrection = 1.0;
      // --malDay;
      // print_details(curr_j_date, curr_date, pl_det, latitude, longitude);
    } else if (currDayNumber == dstEndDayNumber) {
      this->dstCorrection = 0.0;
      // --malDay;
      // print_details(curr_j_date, curr_date, pl_det, latitude, longitude);
    }
    print_details(curr_j_date, curr_date, pl_det, latitude, longitude);
    curr_j_date += 1.0 ;
    prev_month = curr_month ;
  }

  int start = get_day_number_in_year(curr_year, 12, 1);
  int end = get_day_number_in_year(curr_year, 12, 31);
  int wd = weekdayOfDate(curr_year, 12, 1);

  print_holidays_during(start, end, wd);
  print_month_tail (curr_year, curr_month, time_zone);

  // Print the holidays to the outFp1
  // printHolidays(outFp1);
  print_doc_tail();
}

void CalendarCreator::print_holidays_during(int start, int end, int wd)
{
  int row = 0;
  int col = wd;
  for (int d = start; d <= end; ++d) {
    double row_mid = (col + 0.5) * x_scale;
    double col_mid = (5 - row - 0.5) * y_scale;
    if (col_mid < 0.0) {
      col_mid = 4.5 * y_scale;
    }


    HolidayMap::iterator indian = indian_holidays.find(d);
    HolidayMap::iterator foreign = foreign_holidays.find(d);
    if (indian != indian_holidays.end() || foreign != foreign_holidays.end()) {
      double y = col_mid + 0.38 * y_scale;
      fprintf(outFp, "\\draw [Red, line width=0.5mm] (%6.1f, %6.1f) -- (%6.1f, %6.1f);\n", row_mid - 0.45 * x_scale, y, row_mid+0.45 * x_scale, y);


      if (indian != indian_holidays.end()) {
        vector<string>::iterator it =  indian->second.begin();
        while (it != indian->second.end()) {
          y -= 0.4;
          cout << "Displaying " << *it << endl;
          fprintf(outFp, "\\node at (%6.1f, %6.1f)  {{\\fontsize{%d}{0}\\selectfont {\\color{MalHolidayColor}\\MalFont \\textbf{%s}}}};\n",
                  row_mid, y, holidaySize, it->c_str());
          ++it;
        }
      }
      if (foreign != foreign_holidays.end()) {
        vector<string>::iterator it =  foreign->second.begin();
        while (it != foreign->second.end()) {
          y -= 0.4;
          cout << "Displaying " << *it << endl;
          fprintf(outFp, "\\node at (%6.1f, %6.1f)  {{\\fontsize{%d}{0}\\selectfont {\\color{EngHolidayColor} \\textbf{%s}}}};\n",
                  row_mid, y, holidaySize, it->c_str());
          ++it;
        }
      }

      y -= 0.4;
      fprintf(outFp, "\\draw [Red, line width=0.5mm] (%6.1f, %6.1f) -- (%6.1f, %6.1f);\n", row_mid - 0.45 * x_scale, y, row_mid+0.45 * x_scale, y);
    }
    ++col;
    if (col > 6) {
      col = 0;
      ++row;
    }
  }

}

void CalendarCreator::printHolidays(FILE* fp)
{
  static int lastHolidayMonth = -1;
  if (fp) {

    std::vector<Vishesham> visheshams;
    if (thiruvonamDay > 0) {
      visheshams.push_back(Vishesham("<m>tiruvooNaM</m>", thiruvonamDay, Vishesham::INDIAN));
    }
    if (vishuDay > 0) {
      visheshams.push_back(Vishesham("<m>viSu</m>", vishuDay, Vishesham::INDIAN));
    }
    if (durgaashtamiDay > 0) {
      visheshams.push_back(Vishesham("<m>duRggaaSTami</m>", durgaashtamiDay, Vishesham::INDIAN));
    }
    if (mahaanavamiDay > 0) {
      visheshams.push_back(Vishesham("<m>mahaanavami</m>", mahaanavamiDay, Vishesham::INDIAN));
    }
    if (vijayadasamiDay > 0) {
      visheshams.push_back(Vishesham("<m>vijayadashami</m>", vijayadasamiDay, Vishesham::INDIAN));
    }
    if (sivaratriDay > 0) {
      visheshams.push_back(Vishesham("<m>shivaraatri</m>", sivaratriDay, Vishesham::INDIAN));
    }
    if (deepavaliDay > 0) {
      visheshams.push_back(Vishesham("<m>diipaavali</m>", deepavaliDay, Vishesham::INDIAN));
    }
    if (easterDay > 0) {
      visheshams.push_back(Vishesham("<m>uyiRttezhunneelppu perunnaaL</m> (Easter)", easterDay, Vishesham::WORLD));
    }
    if (ashWDay > 0) {
      visheshams.push_back(Vishesham("<m>kSaarabudhan</m> (Ash Wednesday)", ashWDay, Vishesham::WORLD));
    }
    if (passionSDay > 0) {
      visheshams.push_back(Vishesham("Passion Sunday", passionSDay, Vishesham::WORLD));
    }
    if (palmSDay > 0) {
      visheshams.push_back(Vishesham("<m>kuruttoolapperunnaaL</m> (Palm Sunday)", palmSDay, Vishesham::WORLD));
    }
    if (maundyTDay > 0) {
      visheshams.push_back(Vishesham("<m>pesahaa vyaazhaazhca</m> (Maundy Thursday)", maundyTDay, Vishesham::WORLD));
    }
    if (goodFDay > 0) {
      visheshams.push_back(Vishesham("<m>duHkhaveLLiyaazhca</m> (Good Friday)", goodFDay, Vishesham::WORLD));
    }
    if (regationSDay > 0) {
      visheshams.push_back(Vishesham("Regation Sunday", regationSDay, Vishesham::WORLD));
    }
    if (ascensionDay > 0) {
      visheshams.push_back(Vishesham("Ascension Day", ascensionDay, Vishesham::WORLD));
    }
    if (penteCostDay > 0) {
      visheshams.push_back(Vishesham("Pentecost", penteCostDay, Vishesham::WORLD));
    }
    if (trinitySDay > 0) {
      visheshams.push_back(Vishesham("Trinity Sunday", trinitySDay, Vishesham::WORLD));
    }

    int dummy;
    // Fixed ones
    visheshams.push_back(Vishesham("New Year", get_day_number_in_year(year, 1, 1), Vishesham::WORLD));
    if (year >= 1950) {
      visheshams.push_back(Vishesham("<m>Rippablik dinaM</m>", get_day_number_in_year(year, 1, 26), Vishesham::INDIAN));
     }
    if (year >= 1947) {
      visheshams.push_back(Vishesham("<m>svaatantryadinaM</m>", get_day_number_in_year(year, 8, 15), Vishesham::INDIAN));
    }
    visheshams.push_back(Vishesham("Christmas", get_day_number_in_year(year, 12, 25), Vishesham::WORLD));

    visheshams.push_back(Vishesham("Valentines's day", get_day_number_in_year(year, 2, 14), Vishesham::WORLD));

    visheshams.push_back(Vishesham("US Independence day", get_day_number_in_year(year, 7, 4), Vishesham::AMERICAN));
    visheshams.push_back(Vishesham("Halloween", get_day_number_in_year(year, 10, 31), Vishesham::AMERICAN));
    visheshams.push_back(Vishesham("<m>gaandhi jayanti</m>", get_day_number_in_year(year, 10, 2), Vishesham::INDIAN));
    if (year >= 1956) {
      visheshams.push_back(Vishesham("<m>keeraLappiRavi</m>", get_day_number_in_year(year, 11, 1), Vishesham::INDIAN));
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
  END_VALUES nak_ev, thi_ev;
  char naks_str[50], naks_str1[50], thithi_str[50], thithi_str1[50];
  char udayam_str[20], astamayam_str[20];
  double udayam, astamayam;


  int curr_day_number = get_year_day_of_date(curr_date);

  int saka_year, saka_month, saka_day;

  string gregDateColor = "black";

  gregorianToSaka(curr_date.da_year, curr_date.da_mon, curr_date.da_day, saka_year, saka_month, saka_day);

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

  udayam = sunrise_between(j_day, j_day + 0.5, latitude, longitude);
  astamayam = sunset_between(j_day + 0.5, j_day + 1.0, latitude, longitude);
  double next_udayam = sunrise_between(j_day + 1.0, j_day + 1.5, latitude, longitude);
  find_naks_in_day_gen(udayam, next_udayam, &nak_ev);

  if (debugging) {
    fprintf(stderr, "Date = %d/%d/%d, udayam = %f, astamayam = %f\n", curr_date.da_year,
            curr_date.da_mon, curr_date.da_day, udayam, astamayam);
    for (int i = 0; i < nak_ev.n_values; ++i) {
      fprintf(stderr, "%d  %d %15.4f : ", i, nak_ev.values[i].value, nak_ev.values[i].end_j_time);
    }
    fprintf(stderr, "\n");
  }

  char na_name[15];


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

  find_thiths_in_day_gen(udayam, next_udayam, &thi_ev);
  if (thi_ev.n_values > 0) {
    int left;
    if (udayam < thi_ev.values[0].end_j_time || thi_ev.values[0].end_j_time < 0.0) {
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
    } else {
      get_thithis_string(-1, -1.0, 0, thithi_str1);
    }
  } else {
    tonightsThithi = todaysThithi = thithi;
    get_thithis_string(thithi, -1.0, 1, thithi_str);
  }


  hm_string(localJulianDay(udayam), udayam_str);
  hm_string(localJulianDay(astamayam), astamayam_str);

  int d1 = get_year_day_of_date(curr_date);
  // printf("%d %d\n", d1, this->nextNewMonthDay);
  if (get_year_day_of_date(curr_date) == this->nextNewMonthDay) {
    // New Malayalam month
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
    gregDateColor = KERALA_HOLIDAY_COLOR;
    addHoliday(indian_holidays, curr_day_number, "തിരുവോണം");
  } else if (malMonth == 4 /* CHINGAM */ && (todaysStar == 22 && yesterdaysStar == 20)) {
    fprintf(stderr, "============= %d ===================\n", curr_date.da_year);
    thiruvonamDay = curr_day_number - 1;
    addHoliday(indian_holidays, curr_day_number-1, "തിരുവോണം");
  }
   
  if (malMonth == 0 /* MEDAM */ && malDay == 1) {
    if (is_month_at_sunrise) {
      vishuDay = curr_day_number;
      addHoliday(indian_holidays, curr_day_number, "വിഷു");
    } else {
      vishuDay = curr_day_number + 1;
      addHoliday(indian_holidays, curr_day_number+1, "വിഷു");
    }
  }


  if (inNavaratri) {    
    if (todaysThithi == 8) {
      durgaashtamiDay = curr_day_number;
      addHoliday(indian_holidays, curr_day_number, "ദുർഗ്ഗാഷ്ടമി");
     } else if (todaysThithi == 9 && yesterdaysThithi == 7) {
       addHoliday(indian_holidays, curr_day_number-1, "ദുർഗ്ഗാഷ്ടമി");
   }
      
    if (todaysThithi == 9) {
      mahaanavamiDay = curr_day_number;
      addHoliday(indian_holidays, curr_day_number, "മഹാനവമി");
    } else if (todaysThithi == 10 && yesterdaysThithi == 8) {
      addHoliday(indian_holidays, curr_day_number-1, "മഹാനവമി");
    }
      
      
    if (todaysThithi == 10) {
      vijayadasamiDay = curr_day_number;
      addHoliday(indian_holidays, curr_day_number, "വിജയദശമി");
    } else if (todaysThithi == 11 && yesterdaysThithi == 9) {
      addHoliday(indian_holidays, curr_day_number-1, "വിജയദശമി");
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
      addHoliday(indian_holidays, curr_day_number, "ദീപാവലി");
  } else if ((malMonth == 6) && (todaysThithi == 0 && yesterdaysThithi == 28)){
      addHoliday(indian_holidays, curr_day_number-1, "ദീപാവലി");
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

  char rahu_str[40], gulika_str[40], yamakantaka_str[40], madhyahna_str[40];
  char rb[20];
  char re[20];

  ST_DURATION kalam;

  find_rahu_kalam(udayam, astamayam, &kalam);
  duration_to_str(&kalam, rahu_str);
   
  find_gulika_kalam(udayam, astamayam, &kalam);
  duration_to_str(&kalam, gulika_str);
   
  find_yamakantaka_kalam(udayam, astamayam, &kalam);
  duration_to_str(&kalam, yamakantaka_str);
   
  find_madhyahnam(udayam, astamayam, &kalam);
  duration_to_str(&kalam, madhyahna_str);
   

  if (outFp) {
    int wd = weekday(localJulianDay(j_day), 0.0);

    if (wd == 0) gregDateColor = "Red";

    if (curr_date.da_day == 1) {
      row = 0;
      col = wd;
    } else {
      ++col;
      if (col > 6) {
        col = 0;
        ++row;
      }
    }


    double row_mid = (col + 0.5) * x_scale;
    double col_mid = (5 - row - 0.5) * y_scale;
    if (col_mid < 0.0) {
      col_mid = 4.5 * y_scale;
    }

    double date_col = col_mid + 0.7;
    fprintf(outFp, "\\node at (%5.2f, %5.2f) {{\\fontsize{%d}{0}\\selectfont\\GregDayFont\\color{%s} %d}};\n",
            row_mid, date_col, gregDateSize, gregDateColor.c_str(), curr_date.da_day);

    if (showMalayalam) {
      if (showMalayalamDigits) {
        fprintf(outFp, "\\node at (%5.2f, %5.2f) [anchor=west]{{\\fontsize{%d}{0}\\selectfont {\\color{%s}%s}}};\n",
                row_mid - 0.48 * x_scale, col_mid + 0.44 * y_scale, malDateSize, "blue", mal_digits(malDay).c_str());
      } else {
        fprintf(outFp, "\\node at (%5.2f, %5.2f) [anchor=west]{{\\fontsize{%d}{0}\\selectfont {\\color{%s}%s}}};\n",
                row_mid - 0.48 * x_scale, col_mid + 0.44 * y_scale, malDateSize, "blue", mal_digits(malDay).c_str());
      }
    }

    if (showSaka) {
      if (showHindiDigits) {
        fprintf(outFp, "\\node at (%5.2f, %5.2f) [anchor=east] {{\\fontsize{%d}{0}\\selectfont {\\color{%s} {\\SakaDayFont %s}}}};\n",
                row_mid + 0.48 * x_scale, col_mid + 0.44 * y_scale, sakaDateSize, "ForestGreen", dn_digits(saka_day).c_str());
      } else {
        fprintf(outFp, "\\node at (%5.2f, %5.2f) [anchor=east] {{\\fontsize{%d}{0}\\selectfont {\\color{%s} {\\SakaDayEngFont %s}}}};\n",
                row_mid + 0.48 * x_scale, col_mid + 0.44 * y_scale, sakaDateSize, "ForestGreen", dn_digits(saka_day).c_str());
      }
    }





    vector<string> left;
    vector<string> right;
    char temp[300];
    

    int i;
    double col_pos;
    // Nakshathram
    col_pos = col_mid + 1.8;
    char t[50];

    int cnt = 0;
    if (nak_ev.n_values > 0) {
      for (i=0; i < nak_ev.n_values; ++i) {
        double end_j_time = localJulianDay(nak_ev.values[i].end_j_time);
        if (udayam < nak_ev.values[i].end_j_time) {
          ++cnt;
          const char* prefix = nak_ev.values[i].end_j_time > j_day + 1.0 ? "$\\Downarrow$" : "~";
          string nakStr = star_string(nak_ev.values[i].value);
          string endTimeStr;
          if (showNazhikas) {
            char temp[50];
            ::sprintf(temp, "%5.2f", (nak_ev.values[i].end_j_time - udayam) * 60.0);
            endTimeStr = temp;
          } else {
            endTimeStr = end_time_string(localJulianDay(nak_ev.values[i].end_j_time));
          }
          // fprintf(outFp, "\\node at (%6.1f, %6.1f) [anchor=west] {{\\fontsize{%d}{0}\\selectfont {\\MalFont %s}}};\n",
          //         row_mid - 2.4, col_pos, starThithiSize, nakStr.c_str());
          // fprintf(outFp, "\\node at (%6.1f, %6.1f) [anchor=east] {{\\fontsize{%d}{0}\\selectfont{\\color{Red}%s}%s}};\n",
          //         row_mid + 2.4, col_pos, starThithiSize, prefix, endTimeStr.c_str());
          
          sprintf(temp, "{{\\fontsize{%d}{0}\\selectfont {\\MalFont %s}}}", starThithiSize, nakStr.c_str());
          left.push_back(string(temp));
          sprintf(temp, "{{\\fontsize{%d}{0}\\selectfont{\\color{Red}%s}%s}}", starThithiSize, prefix, endTimeStr.c_str());
          right.push_back(string(temp));
        }
      }
    }
    if (cnt == 0) {
      sprintf(temp, "{{\\fontsize{%d}{0}\\selectfont {\\MalFont %s}}}", starThithiSize, star_string((nak_ev.values[0].value)%27).c_str());
      left.push_back(string(temp));
      right.push_back(string("{}"));
      // fprintf(outFp, "\\node at (%6.1f, %6.1f) [anchor=west] {{\\fontsize{%d}{0}\\selectfont{\\MalFont %s}}};\n",
      //         row_mid - 2.4, col_mid + 1.8, starThithiSize, star_string((nak_ev.values[0].value + 1)%27).c_str());
    }

        // col_pos -= 0.35;


    // Thithi
    col_pos = col_mid - 1.2;

    cnt = 0;
    if (thi_ev.n_values > 0) {
      for (i=0; i < thi_ev.n_values; ++i) {
        if (udayam < thi_ev.values[i].end_j_time) {
          ++cnt;
          const char* prefix = thi_ev.values[i].end_j_time > j_day + 1.0 ? "$\\Downarrow$" : "~";
          string thiStr = thithi_string(thi_ev.values[i].value);
          string endTimeStr;
          if (showNazhikas) {
            char temp[50];
            ::sprintf(temp, "%5.2f", (thi_ev.values[i].end_j_time - udayam) * 60.0);
            endTimeStr = temp;
          } else {
            endTimeStr = end_time_string(localJulianDay(thi_ev.values[i].end_j_time));
          }
          // fprintf(outFp, "\\node at (%6.1f, %6.1f) [anchor=west]{{\\fontsize{%d}{0}\\selectfont{\\MalFont %s}}};\n",
          //         row_mid - 2.4, col_pos, starThithiSize, thiStr.c_str());
          // fprintf(outFp, "\\node at (%6.1f, %6.1f) [anchor=east] {{\\fontsize{%d}{0}\\selectfont{\\color{Red}%s}%s}};\n",
          //         row_mid + 2.4, col_pos, starThithiSize, prefix, endTimeStr.c_str());
          sprintf(temp, "{{\\fontsize{%d}{0}\\selectfont {\\MalFont %s}}}", starThithiSize, thiStr.c_str());
          left.push_back(string(temp));
          sprintf(temp, "{{\\fontsize{%d}{0}\\selectfont{\\color{Red}%s}%s}}", starThithiSize, prefix, endTimeStr.c_str());
          right.push_back(string(temp));
        }
      }
    }
    if (cnt == 0) {
      // fprintf(outFp, "\\node at (%6.1f, %6.1f) [anchor=west] {{\\fontsize{%d}{0}\\selectfont{\\MalFont %s}}};\n",
      //         row_mid - 2.4, col_mid - 1.2, starThithiSize, thithi_string((thi_ev.values[0].value+1)%30).c_str());
      sprintf(temp, "{{\\fontsize{%d}{0}\\selectfont {\\MalFont%s}}}", starThithiSize, thithi_string((thi_ev.values[0].value)%30).c_str());
      left.push_back(string(temp));
      right.push_back(string("{}"));
    }
    col_pos = col_mid - 0.15 * y_scale;
    int lines = left.size();
    for (i = 0; i < lines; ++i) {
      fprintf(outFp, "\\node at (%6.1f, %6.1f) [anchor=west] %s;\n", row_mid - 2.4, col_pos, left[i].c_str());
      fprintf(outFp, "\\node at (%6.1f, %6.1f) [anchor=east] %s;\n", row_mid + 2.4, col_pos, right[i].c_str());
      col_pos -= 0.08 * y_scale;
    }
        // col_pos -= 0.35;

    char last_line[150];
    sprintf(last_line, "{\\color{Red}\\textbf{\\sun}}~%s/%s~~~~{\\color{Sepia}\\textbf{\\ascnode}}~%s", udayam_str, astamayam_str, rahu_str);
    fprintf(outFp, "\\node at (%6.1f, %6.1f) {{\\fontsize{%f}{0}\\selectfont%s}};\n",
            row_mid, col_mid - 0.46 * y_scale, sunRahuSize, last_line);


    // fprintf (outFp1, "%2d & %2d & %2d & %s & %s &  %s & %s & %s & %s\\\\\n",
    //          curr_date.da_day, malDay, saka_day, lday_name[weekday(localJulianDay(j_day), 0.0)],
    //          naks_str, thithi_str, udayam_str, astamayam_str, rahu_str);

    // if ((nak_ev.n_values) > 1 || (thi_ev.n_values) > 1) {
    //   fprintf (outFp1, "&&&& %s  &  %s &&&&\\\\\n",
    //            naks_str1, thithi_str1);
    // }
  }

}


char* CalendarCreator::duration_to_str(const DURATION duration, char* str)
{
  char from[20], to[20];
  hm_string(localJulianDay(duration->beg), from);
  hm_string(localJulianDay(duration->end), to);
  sprintf(str, "%s -- %s", from, to);
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

  // if (::getenv("CRPAN_DEBUG") == 0) {
  //   debugging = false;
  // } else {
  //   debugging = true;
  // }

  CalendarCreator cc("places.dat", "cal-sample.tex");
  string placeId(argv[1]);
  cc.createCalendar(placeId);
  return 0;
}
