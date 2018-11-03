/*
 *
 * sani.c
 * Special handling of calculations related to saturn in Hindu astrology
 *
 */

#include <ast.h>

typedef double (*MapFunc)(double);

#define LEAST_COUNT 10.0
#define EPSILON (1.0 / 3600)

// For temporary date strings
static char dt_string[30];

// For debugging
static int debug_level = 10;

// Determines whether an angle 'longitude' is between angles 'start_longitude' and 'end_longitude'
// All angles are normalized to the 0-360 range
bool is_between(double longitude, double start_longitude, double end_longitude)
{
   if (end_longitude >= start_longitude) {
      return (longitude >= start_longitude && longitude <= end_longitude);
   } else {
      return ((longitude >= start_longitude && longitude <= FULL_CIRCLE) ||
              (longitude >= 0.0 && longitude <= end_longitude));
   }
}

// Gets saturn's hindu longitude and whether it is
// forward-moving or retrograding for an instance
// corresponding to the Julian date 'j_day'
double sani_longitude_and_direction(double j_day, char& direction)
{
   ST_PL_DET pl_details[N_PLANETS];
   calculate_planets(j_day, pl_details);
   direction = pl_details[SATURN].direction;
   return hind_long(pl_details[SATURN].geo_long);
}

// Gets saturn's hindu longitude for an instance
// corresponding to the Julian date 'j_day'
double sani_longitude(double j_day)
{
   ST_PL_DET pl_details[N_PLANETS];
   calculate_planets(j_day, pl_details);
   return hind_long(pl_details[SATURN].geo_long);
}

// Iterates for finding the time when a planets goes out of
// a specified limit of longitude
double when_exceeds_limit(double j_day, double limit, bool is_increasing, double increment, MapFunc func)
{
   double longitude;
   bool condition;
   if (debug_level > 5) {
      fprintf(stderr, "      5. j_day = %17.6f (%s) limit = %5.1f  is_increasing = %c  increment = %f\n",
              j_day, date_time_string(j_day, dt_string), limit, (is_increasing ? 'Y' : 'N'), increment);
   }
   do {
      j_day += increment;
      longitude = func(j_day);
      if ((is_increasing && (increment > 0)) || (!is_increasing && (increment < 0))) {
         if (limit > (FULL_CIRCLE - LEAST_COUNT)) {
            condition = (longitude < limit && longitude > LEAST_COUNT);
         } else {
            condition = (longitude < limit);
         }
      } else {
         if (limit < LEAST_COUNT) {
            condition = (longitude > limit && (longitude < (FULL_CIRCLE - LEAST_COUNT)));
         } else {
            condition = (longitude > limit);
         }
      }
      if (debug_level > 6) {
         fprintf (stderr, "            6. j_day = %17.6f longitude = %15.6f\n", j_day, longitude);
      }
   } while (condition);
   return j_day;
}

static inline bool equal(double x, double y)
{
   return (fabs(x - y) < EPSILON);
}

// Finds the time when a planet enteres a range.  Called
// from a time inside the range.
double exact_start_time (double j_day, double start_long, bool is_retrograde)
{
   if (debug_level > 3) {
      fprintf(stderr, "    3. GOING BACK and checking start time from %f (%s)...\n", j_day, date_time_string(j_day, dt_string));
   }
   if (equal(j_day, start_long)) {
      return j_day;
   }
   j_day = when_exceeds_limit(j_day, start_long, !is_retrograde, -1.0, sani_longitude);
   if (equal(j_day, start_long)) {
      return j_day;
   }
   j_day = when_exceeds_limit(j_day, start_long, !is_retrograde, (1.0/1440.0), sani_longitude);
   if (equal(j_day, start_long)) {
      return j_day;
   }
   j_day = when_exceeds_limit(j_day, start_long, !is_retrograde, -1.0/86400.0, sani_longitude);


   return j_day;
}

// Finds the time when a planet leaves a range.  Called
// from a time inside the range.
double exact_end_time (double j_day, double end_long, bool is_retrograde)
{
   if (debug_level > 3) {
      fprintf(stderr, "    4. GOING FORWARD and checking end time from %f (%s)...\n", j_day, date_time_string(j_day, dt_string));
   }
   if (equal(j_day, end_long)) {
      return j_day;
   }
   j_day = when_exceeds_limit(j_day, end_long, !is_retrograde, 1.0, sani_longitude);
   if (equal(j_day, end_long)) {
      return j_day;
   }
   j_day = when_exceeds_limit(j_day, end_long, !is_retrograde, (-1.0/1440.0), sani_longitude);
   if (equal(j_day, end_long)) {
      return j_day;
   }
   j_day = when_exceeds_limit(j_day, end_long, !is_retrograde, 1.0/86400.0, sani_longitude);

   return j_day;
}


double check_and_print_kandaka_sani(FILE* fp, double j_day, double start_long, double birth_jday, bool is_retrograde)
{
   double increment = D_IN_Y;
   double end_long = start_long + 30.0;
   if (debug_level > 1) {
      fprintf (stderr, "KS1 : CHECKING the duration between %6.2f and %6.2f ...\n", 
               start_long, end_long);
   }
   char direction;
   double slong = sani_longitude_and_direction(j_day, direction);
   double start_jday, end_jday;
   char from_time[30];
   char to_time[30];
   if (is_between(slong, start_long, end_long)) {
      start_jday = exact_start_time(j_day, start_long, is_retrograde);
      if (start_jday < birth_jday) {
         start_jday = birth_jday;
      }
      end_jday = exact_end_time(j_day, end_long, is_retrograde);
      fprintf(fp, "  FROM %s TO %s\n", 
              date_time_string(start_jday, from_time), 
              date_time_string(end_jday, to_time));
      if (debug_level > 0) {
         fprintf(stderr, "KS:  FROM %s TO %s\n", 
                 date_time_string(start_jday, from_time), 
                 date_time_string(end_jday, to_time));
      }
      return end_jday;
   } else {
      return -1.0;
   }
}

// Calculates the "Kandakassani" in one's lifetime
void print_kandaka_sani(FILE* fp, int moon_house, double birth_jday)
{
   double start_long, end_long;
   double j_day = birth_jday;
   int i;
   double slong;
   double increment = D_IN_Y;

   fprintf(fp, "\nKANDAKA SANI DURATIONs are as follows:");
   fprintf(fp, "\n----------------------------------------------\n\n");

   if (debug_level > 1) {
      fprintf(stderr, "KANDAKA SANI\n");
   }
   // First, check the status at every year

   double eol_jday = birth_jday + 120.0 * D_IN_Y;
   int start_rasi;
   double temp_jday;
   while (j_day < eol_jday) {
      if (debug_level > 0) {
         fprintf (stderr, "0 : CHECKING %f (%s) ...\n", j_day, date_time_string(j_day, dt_string));
      }
      char direction;
      bool is_retrograde = (direction == 'R');
      for (start_rasi = 0; start_rasi <= 9 ; start_rasi += 3) {
         temp_jday = check_and_print_kandaka_sani(fp, j_day, norm_angle((moon_house + start_rasi) * 30.0), birth_jday, is_retrograde);
         if (temp_jday > 0.0) {
            j_day = temp_jday;
            break;
         }
      }
         
      j_day += increment;
   }
}

// Calculates the "Ezharassani" in one's lifetime
// "Ezharassani" is the duration during which the saturn is
// in the three rasi's - the rasi of birth moon, the one
// before that, and the one after that
void print_seven_and_half_sani(FILE* fp, int moon_house, double birth_jday)
{
   double start_long = norm_angle((moon_house - 1) * 30.0);
   double end_long = norm_angle((moon_house + 2) * 30.0);
   double j_day = birth_jday;
   int i;
   double slong;
   char from_time[30];
   char to_time[30];
   double start_jday, end_jday;
   double increment = D_IN_Y;
   if (debug_level > 1) {
      fprintf (stderr, "1 : CHECKING the duration between %6.2f and %6.2f ...\n", 
               start_long, end_long);
   }

   fprintf(fp, "\nSEVEN AND A HALF SANI DURATIONs are as follows:");
   fprintf(fp, "\n----------------------------------------------\n\n");

   // First, check the status at every year

   double eol_jday = birth_jday + 120.0 * D_IN_Y;
   while (j_day < eol_jday) {
      if (debug_level > 0) {
         fprintf (stderr, "0 : CHECKING %f (%s) ...\n", j_day, date_time_string(j_day, dt_string));
      }
      char direction;
      slong = sani_longitude_and_direction(j_day, direction);
      bool is_retrograde = (direction == 'R');
      if (is_between(slong, start_long, end_long)) {
         start_jday = j_day;
         while (j_day < eol_jday && is_between(j_day, start_long, end_long)) {
            j_day += increment;
         }
         end_jday = j_day - increment;
         if (end_jday < start_jday) {
            end_jday = start_jday;
         }
         if (debug_level > 1) {
            fprintf (stderr, "1 : CHECKING back from %f (%s) and forward from ", start_jday, date_time_string(start_jday, dt_string));
            fprintf (stderr, "%f (%s) ....\n", end_jday, date_time_string(end_jday, dt_string));
         }

         start_jday = exact_start_time(start_jday, start_long, is_retrograde);
         if (start_jday < birth_jday) {
            start_jday = birth_jday;
         }
         end_jday = exact_end_time(end_jday, end_long, is_retrograde);
         fprintf(fp, "  FROM %s TO %s\n", 
                 date_time_string(start_jday, from_time), 
                 date_time_string(end_jday, to_time));
         if (debug_level > 0) {
            fprintf(stderr, "  FROM %s TO %s\n", 
                    date_time_string(start_jday, from_time), 
                    date_time_string(end_jday, to_time));
         }
         j_day = end_jday + 20 * D_IN_Y;
      } else {
         j_day += increment;
      }
   }
}

