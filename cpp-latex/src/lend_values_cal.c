#include "ast.h"
#include <assert.h>
#include <float.h>

#define SMALL_VALUE (1.0 / 24 / 60 / 4)

int find_matches(double start_j_day, double end_j_day, END_VALUES* end_values, AstFunc func, int cycle)
{
   int i = 0;
   int val = (*func)(start_j_day);
   int start_val = val;
   double j_day = start_j_day;
   while (j_day < end_j_day) {
      assert (i < 13);
      j_day = find_next_match(val, j_day, end_j_day, func, cycle);
      
      if (j_day < 0.0) {
         break;
      }
      end_values->values[i].value = val;
      end_values->values[i].end_j_time = j_day;
      ++i;
      val = (val + 1) % cycle;
      
   }

   if (i == 0) {
     // The same value applies throughout the day.
     // This is needed to show the value.
     end_values->values[0].value = start_val;
     end_values->values[0].end_j_time = -1.0;
   }
   end_values->n_values = i;

   /* printf("Matches = ["); */
   /* for (int j = 0; j < i; ++j) { */
   /*   printf("%d:%f ", end_values->values[j].value, end_values->values[j].end_j_time); */
   /* } */
   /* printf("]\n"); */

   return i;
}

double find_next_match(int val, double start_j_day, double end_j_day, AstFunc func, int cycle)
{
   double j_day = start_j_day;
   double low, high, mid;
   int new_val = val, mid_val;
   int next_val = ((val == cycle-1) ? 0 : val + 1);

#if 1
   int end_of_day_val = (*func)(end_j_day);
   while (new_val != end_of_day_val) {
     j_day += 1.0/ 24.0;
     new_val = (*func)(j_day);
     if (new_val != val) {
       if (new_val != next_val) {
         assert(false);
       }

       /* Now, do a bi-section iteration to find the exact instance */
       low = j_day - 1.0 / 24.0;
       high = j_day;
       while (high - low > 1.0/1440.0) {
         mid = 0.5 * (high + low);
         mid_val = (*func)(mid);
         if (mid_val == val) {
           low = mid;
         } else {
           high = mid;
         }
       }
       return low;
     }

   }
   return -1.0;
#else
   /* Find an instance where the value is different */
   do {
     j_day += 1.0/ 24.0;
     if (j_day > end_j_day) {
       j_day = end_j_day;
       new_val = (*func)(j_day);
       if (new_val == val) {
         return -1.0;
       } else {
         break;
       }
     }
     new_val = (*func)(j_day);
   } while (new_val == val);


   if (new_val != next_val) {
      assert(false);
   }
   /* Now, do a bi-section iteration to find the exact instance */
   low = j_day - 1.0 / 24.0;
   high = j_day;
   while (high - low > 1.0/1440.0) {
      mid = 0.5 * (high + low);
      mid_val = (*func)(mid);

      if (mid_val == val) {
         low = mid;
      } else {
         high = mid;
      }
   }

   return low;
#endif
}

// Finds the zero of a continuous function - uses bisection
double find_zero(double low, double high, AstDFunc func) 
{
   double low_value, high_value, mid_value, mid;
   int incr;
   assert (low < high);
   low_value = (*func)(low);
   high_value = (*func)(high);

   if (low_value < 0.0 && high_value > 0.0) {
      incr = 1;
   } else if (low_value > 0.0 && high_value < 0.0) {
      incr = -1;
   } else {
      assert(false);
   }
   
   while (low + SMALL_VALUE < high) {
      mid = 0.5 * (low + high);
      mid_value = (*func)(mid);

      //       low, low_value, high, high_value, mid, mid_value);

      if (incr * mid_value < 0.0) {
         low = mid;
         low_value = mid_value;
      } else {
         high = mid;
         high_value = mid_value;
      }
   }
   return high;
}

int find_naks_in_day (double j_day, END_VALUES* end_values)
{
   return find_matches(j_day, j_day+1.0, end_values, inakshatram, 27);
}

int find_thiths_in_day (double j_day, END_VALUES* end_values)
{
   return find_matches(j_day, j_day+1.0, end_values,ithithi, 30);
}

int find_naks_in_day_gen (double j_day1, double j_day2, END_VALUES* end_values)
{
   return find_matches(j_day1, j_day2, end_values, inakshatram, 27);
}

int find_thiths_in_day_gen (double j_day1, double j_day2, END_VALUES* end_values)
{
   return find_matches(j_day1, j_day2, end_values,ithithi, 30);
}


int inakshatram(double j_day)
{
   double l_sunt, l_earth, au_earth, l_moont, theta;

   double j_cent = (j_day - J_DAY_1900_PRE_NOO) / D_IN_C ;
   find_parameters(j_cent, &l_sunt, &l_earth, &au_earth, &l_moont, &theta);
   return (int)(nakshatra(norm_angle(l_moont - ayanamsa)));
}

int ithithi(double j_day)
{
   double l_sunt, l_earth, au_earth, l_moont, theta, j_cent, smd, d_thithi;
/*    int paksham; */
   int ith;

   j_cent = (j_day - J_DAY_1900_PRE_NOO) / D_IN_C ;
   find_parameters(j_cent, &l_sunt, &l_earth, &au_earth, &l_moont, &theta);
   smd = norm_angle(l_moont - l_sunt);
   d_thithi = smd / 12.0 + 1.0;
   ith = (int) d_thithi;
   /*
   paksham = (int)(smd / 180.0);
   d_thithi = (smd - 180.0 * paksham) / 12.0 + 1.0 ;
   if (d_thithi > 15.0 )
      d_thithi -= 15.0 ;
   */
   return (ith % 30);
}

static int get_rasi(double j_day)
{
   double l_sunt, l_earth, au_earth, l_moont, theta;
   double j_cent = (j_day - J_DAY_1900_PRE_NOO) / D_IN_C ;
   find_parameters(j_cent, &l_sunt, &l_earth, &au_earth, &l_moont, &theta);
   return ((int) ((norm_angle(l_sunt - get_ayanamsa(j_day))) / 30.0));
}

/* Returns new rasi */
int next_sun_transit(double j_day, double* transit_j_day)
{
   int next = sun_transit_between(j_day, j_day+33, transit_j_day);
   if (next == -1) {
     next = sun_transit_between(j_day, j_day+27, transit_j_day);
   }
   if (next == -1) {
     assert(false);
   }
   return next;
}

int sun_transit_between(double low, double high, double* transit_j_day)
{

   double mid;
   int low_rasi, high_rasi, mid_rasi;

   low_rasi = get_rasi(low);
   high_rasi = get_rasi(high);

   if ((low_rasi + 1)%12 != high_rasi) {
     return -1;
   }

   while ((low+1.0/1440) < high) {
      mid = 0.5 * (low + high);
      mid_rasi = get_rasi(mid);
      if (mid_rasi == low_rasi) {
         low = mid;
      } else {
         high = mid;
      }
   }
   *transit_j_day = high;
   return high_rasi;

}

double get_sun_altitude(double j_day, double geo_latitude, double geo_longitude)
{
   double l_sunt, l_earth, au_earth, l_moont, theta, e;
   double num, den;
   double altitude, sin_altitude, hour_angle, lst, rad_declination, right_ascension;

   double j_cent = (j_day - J_DAY_1900_PRE_NOO) / D_IN_C ;

   /*  Finf the longitude of the sun */
   find_parameters(j_cent, &l_sunt, &l_earth, &au_earth, &l_moont, &theta);


   /* Find the obliquity of the ecliptic (Degrees) */
   e = get_e(j_day);

   /* Find right ascension (Degrees) */
   num = d_sin(l_sunt) * d_cos(e);
   den = d_cos(l_sunt);
 
   right_ascension = sc_d_angle(num, den);

   /* Find declination (Radians) */
   rad_declination = asin(d_sin(l_sunt) * d_sin(e));

   /* Find local sidereal time (hours)*/
   lst = local_sidereal_time(j_day, geo_longitude);

   /* Convert LST to degrees */
   lst = lst * 15.0 ;

   /* Find the hour angle */
   hour_angle = norm_angle(lst - right_ascension);

   /* Find altitude */
   sin_altitude = sin(rad_declination) * d_sin(geo_latitude) + cos(rad_declination) * d_cos (geo_latitude) * d_cos(hour_angle);

   altitude = D_IN_R * asin(sin_altitude) ;

#if 0
   /* This is the centre of the sun's disk.  Find the declination at top of the disk. */
   altitude += 34.0/60.0;
#endif

#if 0
   /* Apply corrections for refraction, aberrations etc. */
   altitude += 0.266 / au_earth;
#endif

   /* Apply the correction due to parallax */

   /* Apply correction for ephermeris time */   

   return altitude;
   
}

static double temp_latitude;
static double temp_longitude;

static double temp_sun_altitude(double j_day)
{
   return get_sun_altitude(j_day, temp_latitude, temp_longitude);
}

double sunrise_between(double low, double high, double latitude, double longitude)
{
   double alt_low, alt_high;

   alt_low = get_sun_altitude(low, latitude, longitude);
   alt_high = get_sun_altitude(high, latitude, longitude); 

   temp_latitude = latitude;
   temp_longitude = longitude;

   return find_zero(low, high, temp_sun_altitude) ;
  
}

double sunset_between(double low, double high, double latitude, double longitude)
{
   double alt_low, alt_high;

   alt_low = get_sun_altitude(low, latitude, longitude);
   alt_high = get_sun_altitude(high, latitude, longitude); 

   temp_latitude = latitude;
   temp_longitude = longitude;

   return find_zero(low, high, temp_sun_altitude) ;
  
}
